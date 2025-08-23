#include "Program.h"

#include <queue>
#include <map>
#include <algorithm>

#include "../parser/parser.h"
#include "StructDefinition.h"
#include "Function.h"
#include "utils.h"
#include "Type.h"
#include "FunctionSignature.h"
#include "Identifier.h"
#include "TemplatedStructDefinition.h"
#include "TemplatedFunction.h"
#include "Constructor.h"
#include "ConstructorSignature.h"
#include "TemplatedOperator.h"
#include "primitives.h"
#include "Include.h"
#include "GlobalDeclaration.h"
#include "Declaration.h"
#include "Destructor.h"
#include "GlobalNode.h"
#include "Typedef.h"
#include "TemplateMapping.h"
#include "Operator.h"
#include "OperatorSignature.h"

Program::Program() {
    // do nothing
}

Program::Program(std::vector<StructDefinition*> _structs, std::vector<TemplatedStructDefinition*> _templated_structs, std::vector<TemplatedFunction*> _templated_functions, std::vector<TemplatedOperator*> _templated_operators, std::vector<Include*> _includes, std::vector<GlobalDeclaration*> _global_declarations, std::vector<GlobalNode*> _global_nodes, std::vector<Typedef*> _typedefs) {
    structs = _structs;

    templated_structs = _templated_structs;
    templated_functions = _templated_functions;
    templated_operators = _templated_operators;

    includes = _includes;

    global_declarations = _global_declarations;
    global_nodes = _global_nodes;

    typedefs = _typedefs;
}

Program* Program::convert(parser::program *p) {
    std::vector<StructDefinition*> structs;
    std::vector<Overload*> overloads;

    std::vector<TemplatedStructDefinition*> templated_structs;
    std::vector<TemplatedFunction*> templated_functions;
    std::vector<TemplatedOperator*> templated_operators;

    std::vector<Include*> includes;

    std::vector<GlobalDeclaration*> global_declarations;
    std::vector<GlobalNode*> global_nodes;

    std::vector<Typedef*> typedefs;

    for(int i = 0; i < p->t0.size(); i++){
        if(p->t0[i]->t1->is_c0) {  //struct definition
            structs.push_back(StructDefinition::convert(p->t0[i]->t1->t0->t0));
        }
        else if(p->t0[i]->t1->is_c1) {  //templated function
            templated_functions.push_back(TemplatedFunction::convert(p->t0[i]->t1->t1->t0));
        }
        else if(p->t0[i]->t1->is_c2) {  //templated struct definition
            templated_structs.push_back(TemplatedStructDefinition::convert(p->t0[i]->t1->t2->t0));
        }
        else if(p->t0[i]->t1->is_c3) {  //templated overload
            templated_operators.push_back(TemplatedOperator::convert(p->t0[i]->t1->t3->t0));
        }
        else if(p->t0[i]->t1->is_c4) {  //include
            includes.push_back(Include::convert(p->t0[i]->t1->t4->t0));
        }
        else if(p->t0[i]->t1->is_c5) {  //global declaration
            global_declarations.push_back(GlobalDeclaration::convert(p->t0[i]->t1->t5->t0));
        }
        else if(p->t0[i]->t1->is_c6) {  //global node
            global_nodes.push_back(GlobalNode::convert(p->t0[i]->t1->t6->t0));
        }
        else if(p->t0[i]->t1->is_c7) {  //typedef
            typedefs.push_back(Typedef::convert(p->t0[i]->t1->t7->t0));
        }
        else assert(false);
    }
    return new Program(structs, templated_structs, templated_functions, templated_operators, includes, global_declarations, global_nodes, typedefs);
}

void Program::add_all(Program *other) {
    for(int i = 0; i < other->structs.size(); i++) structs.push_back(other->structs[i]);

    for(int i = 0; i < other->templated_structs.size(); i++) templated_structs.push_back(other->templated_structs[i]);
    for(int i = 0; i < other->templated_functions.size(); i++) templated_functions.push_back(other->templated_functions[i]);
    for(int i = 0; i < other->templated_operators.size(); i++) templated_operators.push_back(other->templated_operators[i]);

    for(int i = 0; i < other->includes.size(); i++) includes.push_back(other->includes[i]);
    
    for(int i = 0; i < other->global_declarations.size(); i++) global_declarations.push_back(other->global_declarations[i]);
    for(int i = 0; i < other->global_nodes.size(); i++) global_nodes.push_back(other->global_nodes[i]);

    for(int i = 0; i < other->typedefs.size(); i++) typedefs.push_back(other->typedefs[i]);
}

bool Program::is_well_formed() {
    // - set up semantic checker controller
    reset_controller();
    std::cout << "DONE INIT CONTROLLER" << std::endl;

    enclosing_program = this;

    fout << ".section .text\n";

    //register all basetypes
    std::cout << "REGISTERING BASETYPES" << std::endl;
    for(int i = 0; i < templated_structs.size(); i++){
        BaseType *sbt = dynamic_cast<BaseType*>(templated_structs[i]->struct_def->type);
        assert(sbt != nullptr);
        if(!add_basetype(sbt)) {
            std::cout << "Failed to add basetype : " << sbt->to_string() << "\n";
            return false;
        }
    }
    for(int i = 0; i < structs.size(); i++) {
        BaseType *sbt = dynamic_cast<BaseType*>(structs[i]->type);
        assert(sbt != nullptr);
        if(!add_basetype(sbt)) {
            std::cout << "Failed to add basetype : " << sbt->to_string() << "\n";
            return false;
        }
    }

    // - resolve typedefs
    //at this point all basetypes should be registered in the controller
    std::cout << "RESOLVING TYPEDEFS" << std::endl;
    {
        //register typedefs as basetypes
        for(int i = 0; i < typedefs.size(); i++){
            BaseType *tdbt = typedefs[i]->base_type;
            if(!add_basetype(tdbt)) {
                std::cout << "Failed to add basetype : " << tdbt->to_string() << "\n";
                return false;
            }
        }

        //map basetypes to ints
        std::map<std::string, int> btind;
        for(int i = 0; i < typedefs.size(); i++){
            btind[typedefs[i]->base_type->name] = i;
        }

        //for each typedef, find what other typedefs it depends on
        int n = typedefs.size();
        std::vector<std::vector<int>> c(n);
        for(int i = 0; i < n; i++){
            std::vector<BaseType*> bts;
            typedefs[i]->type->find_all_basetypes(bts);
            
            //make sure all basetypes are either declared or typedef'd
            for(int j = 0; j < bts.size(); j++){
                BaseType *bt = bts[j];
                if(is_basetype_declared(bt)) continue;
                int ind = -1;
                for(int k = 0; k < n; k++){
                    if(typedefs[k]->base_type->equals(bt)) ind = k;
                }
                if(ind == -1) {
                    std::cout << "Basetype \"" << bt->to_string() << "\" not declared in : " << typedefs[i]->to_string() << "\n";
                    return false;
                }
                c[ind].push_back(i);
            }
        }

        //toposort typedefs
        std::vector<int> indeg(n, 0);
        for(int i = 0; i < n; i++){
            for(int x : c[i]) indeg[x] ++;
        }
        std::vector<int> ord(0);
        std::queue<int> q;
        for(int i = 0; i < n; i++) if(indeg[i] == 0) q.push(i);
        while(q.size() != 0){
            int cur = q.front();
            q.pop();
            ord.push_back(cur);
            for(int x : c[cur]) if(-- indeg[x] == 0) q.push(x);
        }
        if(ord.size() != n) {
            std::cout << "Could not find ordering for typedefs\n";
            return false;
        }

        //for each typedef, replace with previously defined basetypes
        TemplateMapping *mapping = new TemplateMapping();
        for(int i = 0; i < n; i++){
            int cur = ord[i];
            mapping->add_mapping(typedefs[cur]->base_type, typedefs[cur]->type);
            if(i + 1 != n) {
                int next = ord[i + 1];
                typedefs[next]->replace_templated_types(mapping);
            }
        }

        std::cout << "RESOLVED TYPEDEFS : \n";
        for(int i = 0; i < n; i++){
            std::cout << typedefs[i]->base_type->to_string() << " : " << typedefs[i]->type->to_string() << "\n";
        }

        //go through everything and replace typedef types
        bool success = true;
        for(int i = 0; i < structs.size(); i++) success &= structs[i]->replace_templated_types(mapping);
        for(int i = 0; i < templated_structs.size(); i++) success &= templated_structs[i]->replace_templated_types(mapping);
        for(int i = 0; i < templated_functions.size(); i++) success &= templated_functions[i]->replace_templated_types(mapping);
        for(int i = 0; i < templated_operators.size(); i++) success &= templated_operators[i]->replace_templated_types(mapping);
        for(int i = 0; i < global_declarations.size(); i++) success &= global_declarations[i]->replace_templated_types(mapping);
        if(!success) {
            std::cout << "Failed to resolve typedefs\n";
            return false;
        }
    }

    //for all templated structs add them as templated structs
    std::cout << "ADDING TEMPLATED STRUCTS" << std::endl;
    for(int i = 0; i < templated_structs.size(); i++){
        BaseType *sbt = dynamic_cast<BaseType*>(templated_structs[i]->struct_def->type);
        if(!add_templated_struct(templated_structs[i])) {
            std::cout << "Failed to add templated struct : " << sbt->to_string() << "\n";
            return false;
        }
    }

    //for all structs, register them as types
    std::cout << "ADDING STRUCTS" << std::endl;
    for(int i = 0; i < structs.size(); i++) {
        BaseType *sbt = dynamic_cast<BaseType*>(structs[i]->type);
        if(!add_struct_type(structs[i])) {
            std::cout << "Failed to add type : " << structs[i]->type->to_string() << "\n";
            return false;
        }
    }

    //for all templated functions add them
    std::cout << "ADDING TEMPLATED FUNCTIONS" << std::endl;
    for(int i = 0; i < templated_functions.size(); i++){
        if(!add_templated_function(templated_functions[i])) {
            std::cout << "Failed to add templated function : " << templated_functions[i]->function->resolve_function_signature()->to_string() << "\n";
            return false;
        }
    }

    //for all templated overloads add them
    std::cout << "ADDING TEMPLATED OVERLOADS" << std::endl;
    for(int i = 0; i < templated_operators.size(); i++){
        if(!add_templated_operator(templated_operators[i])) {
            std::cout << "Failed to add templated overload : " << templated_operators[i]->op->resolve_operator_signature()->to_string() << "\n";
            return false;
        }
    }

    // - are all structs well formed?
    std::cout << "CHECKING STRUCTS WELL FORMED" << std::endl;
    for(int i = 0; i < structs.size(); i++){
        if(!structs[i]->is_well_formed()) {
            std::cout << "Struct not well formed : " << structs[i]->type->to_string() << "\n";
            return false;
        }
    }

    // - can we construct a struct layout for all structs?
    std::cout << "CONSTRUCT BASE STRUCT LAYOUTS" << std::endl;
    for(int i = 0; i < structs.size(); i++){
        Type *t = structs[i]->type;
        if(!construct_struct_layout(t)) {
            std::cout << "Unable to construct struct layout for type : " << t->to_string() << "\n";
            return false;
        }
    }

    // - are all the templated structs well formed?
    std::cout << "CHECKING TEMPLATED STRUCTS WELL FORMED" << std::endl;
    for(int i = 0; i < templated_structs.size(); i++){
        if(!templated_structs[i]->is_well_formed()) {
            std::cout << "Templated struct not well formed : " << templated_structs[i]->struct_def->type->to_string() << "\n";
            return false;
        }
    }

    // - are all the templated functions well formed?
    std::cout << "CHECKING TEMPLATED FUNCTIONS WELL FORMED" << std::endl;
    for(int i = 0; i < templated_functions.size(); i++){
        if(!templated_functions[i]->is_well_formed()) {
            std::cout << "Templated function not well formed : " << templated_functions[i]->function->resolve_function_signature()->to_string() << "\n";
            return false;
        }
    }

    // - are all the templated overloads well formed?
    std::cout << "CHECKING TEMPLATED OVERLOADS WELL FORMED" << std::endl;
    for(int i = 0; i < templated_operators.size(); i++){
        if(!templated_operators[i]->is_well_formed()) {
            std::cout << "Templated overload not well formed : " << templated_operators[i]->op->resolve_operator_signature()->to_string() << "\n";
            return false;
        }
    }

    // - there must be a global function with function signature 'i32 main()'
    {
        FunctionSignature *main_fs = new FunctionSignature(new Identifier("main"), {});
        Function *f = get_function(main_fs);
        if(f == nullptr) {
            std::cout << "Missing main function\n";
            return false;
        }
        if(!f->type->equals(primitives::i32)) {
            std::cout << "main has wrong return type (must be i32)\n";
            return false;
        }
    }

    //make sure i32 main() is the first function to be checked. 
    //This is so that the global variables get initialized before anything else
    std::sort(declared_functions.begin(), declared_functions.end(), [](Function *a, Function *b) -> bool {
        return a->is_main() > b->is_main();
    });

    // - check all functions and constructors, make sure they're well formed
    int function_ptr = 0;
    int constructor_ptr = 0;
    int operator_ptr = 0;
    int destructor_ptr = 0;
    while(function_ptr < declared_functions.size() || constructor_ptr < declared_constructors.size() || operator_ptr < declared_operators.size()) {
        while(function_ptr < declared_functions.size()) {
            ld process_start_time = current_time_seconds();
            Function *f = declared_functions[function_ptr ++];
            enclosing_function = f;
            if(!f->is_well_formed()) {
                enclosing_function = nullptr;
                return false;
            }
            enclosing_function = nullptr;
            add_duration_stat("FUNCTION " + f->resolve_function_signature()->to_string(), current_time_seconds() - process_start_time);
        }
        while(constructor_ptr < declared_constructors.size()) {
            ld process_start_time = current_time_seconds();
            Constructor *c = declared_constructors[constructor_ptr ++];
            if(!c->is_well_formed()) {
                return false;
            }
            add_duration_stat("CONSTRUCTOR " + c->resolve_constructor_signature()->to_string(), current_time_seconds() - process_start_time);
        }
        while(operator_ptr < declared_operators.size()) {
            ld process_start_time = current_time_seconds();
            Operator *o = declared_operators[operator_ptr ++];
            if(dynamic_cast<OperatorOverload*>(o) == nullptr) continue; //only process overloads
            OperatorOverload *oo = dynamic_cast<OperatorOverload*>(o);
            assert(oo != nullptr);
            enclosing_overload = oo;
            if(!oo->is_well_formed()) {
                return false;
            }
            enclosing_overload = nullptr;
            add_duration_stat("OVERLOAD " + o->resolve_operator_signature()->to_string(), current_time_seconds() - process_start_time);
    
        }
        while(destructor_ptr < declared_destructors.size()) {
            ld process_start_time = current_time_seconds();
            Destructor *d = declared_destructors[destructor_ptr ++];
            if(!d->is_well_formed()) {
                return false;
            }
            add_duration_stat("DESTRUCTOR " + d->type->to_string() + "::~()", current_time_seconds() - process_start_time);
        }
    }

    // - emit .data section
    emit_data_section();

    enclosing_program = nullptr;

    assert(declaration_stack.size() == 0);
    assert(declared_variables.size() == global_declarations.size());

    return true;
}