#include "Program.h"

#include "../parser/parser.h"
#include "StructDefinition.h"
#include "Function.h"
#include "utils.h"
#include "Type.h"
#include <queue>
#include "FunctionSignature.h"
#include "Identifier.h"


Program::Program(std::vector<StructDefinition*> _structs, std::vector<Function*> _functions) {
    structs = _structs;
    functions = _functions;
}

Program* Program::convert(parser::program *p) {
    std::vector<StructDefinition*> structs;
    std::vector<Function*> functions;
    for(int i = 0; i < p->t0.size(); i++){
        if(p->t0[i]->t1->is_c0) {   //function
            functions.push_back(Function::convert(p->t0[i]->t1->t0->t0));
        }
        else if(p->t0[i]->t1->is_c1) {  //struct definition
            structs.push_back(StructDefinition::convert(p->t0[i]->t1->t1->t0));
        }
        else assert(false);
    }
    return new Program(structs, functions);
}

bool Program::is_well_formed() {
    // - set up semantic checker controller
    reset_controller();
    std::cout << "DONE INIT CONTROLLER" << std::endl;

    fout << ".section .text\n";

    //for all structs, register them as types
    for(int i = 0; i < structs.size(); i++) {
        if(!add_type(structs[i]->type)) {
            std::cout << "Failed to add type : " << structs[i]->type->to_string() << "\n";
            return false;
        }
    }
    std::cout << "DONE REGISTER STRUCTS AS TYPES" << std::endl;

    // - are there circular dependencies in the struct member variables?
    //build struct graph, do topological sort on the graph.
    std::vector<int> top_ord(0);
    {
        int n = structs.size();
        std::unordered_map<Type*, int> indmp;
        for(int i = 0; i < n; i++) indmp[structs[i]->type] = i;
        std::vector<std::vector<int>> c(n);
        std::vector<int> indeg(n, 0);
        for(int i = 0; i < n; i++){
            StructDefinition *sd = structs[i];
            for(int j = 0; j < sd->member_variables.size(); j++){
                Type *vt = sd->member_variables[j]->type;
                if(indmp.count(vt)) {
                    int x = indmp[vt];
                    c[i].push_back(x);
                    indeg[x] ++;
                }
            }
        }
        std::queue<int> q;
        for(int i = 0; i < n; i++){
            if(indeg[i] == 0) q.push(i);
        }
        while(q.size() != 0){
            int cur = q.front();
            q.pop();
            top_ord.push_back(cur);
            for(int x : c[cur]) {
                indeg[x] --;
                if(indeg[x] == 0) q.push(x);
            }
        }
    }
    if(top_ord.size() != structs.size()) {
        std::cout << "There exists a circular dependency in the struct graph\n";
        return false;
    }
    std::cout << "DONE CHECK CIRCULAR DEPENDENCIES" << std::endl;

    // - are all structs well formed?
    //do this in topological order
    for(int i = 0; i < structs.size(); i++){
        StructDefinition *s = structs[top_ord[i]];
        if(!s->is_well_formed()) {
            std::cout << "Struct not well formed : " << s->type->to_string() << "\n";
            return false;
        }
    }

    // - are there any duplicate global function definitions?
    for(int i = 0; i < functions.size(); i++) {
        if(!add_function(functions[i])) {
            std::cout << "Failed to add global function : " << functions[i]->resolve_function_signature()->to_string() << "\n";
            return false;
        }
    }

    // - there must be a global function with function signature 'int main()'
    {
        FunctionSignature *main_fs = new FunctionSignature(new Identifier("main"), {});
        Function *f = get_function(main_fs);
        if(f == nullptr) {
            std::cout << "Missing main function\n";
            return false;
        }
        if(*(f->type) != BaseType("int")) {
            std::cout << "main has wrong return type (must be int)\n";
            return false;
        }
    }

    // - make sure every function is well formed
    for(int i = 0; i < declared_functions.size(); i++){
        enclosing_function = declared_functions[i];

        if(!declared_functions[i]->is_well_formed()) {
            std::cout << "Function not well formed : " << declared_functions[i]->resolve_function_signature()->to_string() << "\n";
            return false;
        }
    }

    assert(declaration_stack.size() == 0);
    assert(declared_variables.size() == 0);

    return true;
}