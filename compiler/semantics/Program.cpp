#include "Program.h"

#include "../parser/parser.h"
#include "StructDefinition.h"
#include "Function.h"
#include "utils.h"
#include "Type.h"
#include <queue>
#include "FunctionSignature.h"
#include "Identifier.h"
#include "TemplatedStructDefinition.h"
#include "TemplatedFunction.h"
#include "Constructor.h"
#include "ConstructorSignature.h"
#include "Overload.h"
#include "TemplatedOverload.h"
#include "primitives.h"
#include "Include.h"
#include "GlobalDeclaration.h"
#include "Declaration.h"
#include <algorithm>
#include "Destructor.h"
#include "GlobalNode.h"

Program::Program() {
    // do nothing
}

Program::Program(std::vector<StructDefinition*> _structs, std::vector<Function*> _functions, std::vector<TemplatedStructDefinition*> _templated_structs, std::vector<TemplatedFunction*> _templated_functions, std::vector<Overload*> _overloads, std::vector<TemplatedOverload*> _templated_overloads, std::vector<Include*> _includes, std::vector<GlobalDeclaration*> _global_declarations, std::vector<GlobalNode*> _global_nodes) {
    structs = _structs;
    functions = _functions;
    overloads = _overloads;

    templated_structs = _templated_structs;
    templated_functions = _templated_functions;
    templated_overloads = _templated_overloads;

    includes = _includes;

    global_declarations = _global_declarations;
    global_nodes = _global_nodes;
}

Program* Program::convert(parser::program *p) {
    std::vector<StructDefinition*> structs;
    std::vector<Function*> functions;
    std::vector<Overload*> overloads;

    std::vector<TemplatedStructDefinition*> templated_structs;
    std::vector<TemplatedFunction*> templated_functions;
    std::vector<TemplatedOverload*> templated_overloads;

    std::vector<Include*> includes;

    std::vector<GlobalDeclaration*> global_declarations;
    std::vector<GlobalNode*> global_nodes;

    for(int i = 0; i < p->t0.size(); i++){
        if(p->t0[i]->t1->is_c0) {   //function
            functions.push_back(Function::convert(p->t0[i]->t1->t0->t0));
        }
        else if(p->t0[i]->t1->is_c1) {  //struct definition
            structs.push_back(StructDefinition::convert(p->t0[i]->t1->t1->t0));
        }
        else if(p->t0[i]->t1->is_c2) {  //templated function
            templated_functions.push_back(TemplatedFunction::convert(p->t0[i]->t1->t2->t0));
        }
        else if(p->t0[i]->t1->is_c3) {  //templated struct definition
            templated_structs.push_back(TemplatedStructDefinition::convert(p->t0[i]->t1->t3->t0));
        }
        else if(p->t0[i]->t1->is_c4) {  //overload
            overloads.push_back(Overload::convert(p->t0[i]->t1->t4->t0));
        }
        else if(p->t0[i]->t1->is_c5) {  //templated overload
            templated_overloads.push_back(TemplatedOverload::convert(p->t0[i]->t1->t5->t0));
        }
        else if(p->t0[i]->t1->is_c6) {  //include
            includes.push_back(Include::convert(p->t0[i]->t1->t6->t0));
        }
        else if(p->t0[i]->t1->is_c7) {  //global declaration
            global_declarations.push_back(GlobalDeclaration::convert(p->t0[i]->t1->t7->t0));
        }
        else if(p->t0[i]->t1->is_c8) {  //global node
            global_nodes.push_back(GlobalNode::convert(p->t0[i]->t1->t8->t0));
        }
        else assert(false);
    }
    return new Program(structs, functions, templated_structs, templated_functions, overloads, templated_overloads, includes, global_declarations, global_nodes);
}

void Program::add_all(Program *other) {
    for(int i = 0; i < other->structs.size(); i++) structs.push_back(other->structs[i]);
    for(int i = 0; i < other->functions.size(); i++) functions.push_back(other->functions[i]);
    for(int i = 0; i < other->overloads.size(); i++) overloads.push_back(other->overloads[i]);

    for(int i = 0; i < other->templated_structs.size(); i++) templated_structs.push_back(other->templated_structs[i]);
    for(int i = 0; i < other->templated_functions.size(); i++) templated_functions.push_back(other->templated_functions[i]);
    for(int i = 0; i < other->templated_overloads.size(); i++) templated_overloads.push_back(other->templated_overloads[i]);

    for(int i = 0; i < other->includes.size(); i++) includes.push_back(other->includes[i]);
    
    for(int i = 0; i < other->global_declarations.size(); i++) global_declarations.push_back(other->global_declarations[i]);
    for(int i = 0; i < other->global_nodes.size(); i++) global_nodes.push_back(other->global_nodes[i]);
}

bool Program::is_well_formed() {
    // - set up semantic checker controller
    reset_controller();
    std::cout << "DONE INIT CONTROLLER" << std::endl;

    enclosing_program = this;

    fout << ".section .text\n";

    //for all templated structs, register their basetype, add them as templated structs
    std::cout << "ADDING TEMPLATED STRUCTS" << std::endl;
    for(int i = 0; i < templated_structs.size(); i++){
        BaseType *sbt = dynamic_cast<BaseType*>(templated_structs[i]->struct_def->type);
        assert(sbt != nullptr);
        if(!add_basetype(sbt)) {
            std::cout << "Failed to add basetype : " << sbt->to_string() << "\n";
            return false;
        }
        if(!add_templated_struct(templated_structs[i])) {
            std::cout << "Failed to add templated struct : " << sbt->to_string() << "\n";
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
    for(int i = 0; i < templated_overloads.size(); i++){
        if(!add_templated_overload(templated_overloads[i])) {
            std::cout << "Failed to add templated overload : " << templated_overloads[i]->overload->resolve_operator_signature()->to_string() << "\n";
            return false;
        }
    }

    //for all structs, register them as types
    std::cout << "ADDING STRUCTS" << std::endl;
    for(int i = 0; i < structs.size(); i++) {
        BaseType *sbt = dynamic_cast<BaseType*>(structs[i]->type);
        assert(sbt != nullptr);
        if(!add_basetype(sbt)) {
            std::cout << "Failed to add basetype : " << sbt->to_string() << "\n";
            return false;
        }
        if(!add_struct_type(structs[i])) {
            std::cout << "Failed to add type : " << structs[i]->type->to_string() << "\n";
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
    for(int i = 0; i < templated_overloads.size(); i++){
        if(!templated_overloads[i]->is_well_formed()) {
            std::cout << "Templated overload not well formed : " << templated_overloads[i]->overload->resolve_operator_signature()->to_string() << "\n";
            return false;
        }
    }

    // - are there any duplicate global function definitions?
    std::cout << "ADDING FUNCTIONS" << std::endl;
    for(int i = 0; i < functions.size(); i++) {
        if(!add_function(functions[i])) {
            std::cout << "Failed to add global function : " << functions[i]->resolve_function_signature()->to_string() << "\n";
            return false;
        }
    }

    // - are there any duplicate overloads?
    std::cout << "ADDING OVERLOADS" << std::endl;
    for(int i = 0; i < overloads.size(); i++){
        if(!add_operator_implementation(overloads[i])) {
            std::cout << "Failed to add overload : " << overloads[i]->resolve_operator_signature()->to_string() << "\n";
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
    int overload_ptr = 0;
    int destructor_ptr = 0;
    while(function_ptr < declared_functions.size() || constructor_ptr < declared_constructors.size() || overload_ptr < declared_overloads.size()) {
        while(function_ptr < declared_functions.size()) {
            Function *f = declared_functions[function_ptr ++];
            enclosing_function = f;
            if(!f->is_well_formed()) {
                enclosing_function = nullptr;
                return false;
            }
            enclosing_function = nullptr;
        }
        while(constructor_ptr < declared_constructors.size()) {
            Constructor *c = declared_constructors[constructor_ptr ++];
            if(!c->is_well_formed()) {
                return false;
            }
        }
        while(overload_ptr < declared_overloads.size()) {
            Overload *o = declared_overloads[overload_ptr ++];
            enclosing_overload = o;
            if(!o->is_well_formed()) {
                return false;
            }
            enclosing_overload = nullptr;
        }
        while(destructor_ptr < declared_destructors.size()) {
            Destructor *d = declared_destructors[destructor_ptr ++];
            if(!d->is_well_formed()) {
                return false;
            }
        }
    }

    // - emit .data section
    emit_data_section();

    enclosing_program = nullptr;

    assert(declaration_stack.size() == 0);
    assert(declared_variables.size() == global_declarations.size());

    return true;
}