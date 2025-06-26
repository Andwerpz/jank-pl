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

Program::Program(std::vector<StructDefinition*> _structs, std::vector<Function*> _functions, std::vector<TemplatedStructDefinition*> _templated_structs, std::vector<TemplatedFunction*> _templated_functions, std::vector<Overload*> _overloads, std::vector<TemplatedOverload*> _templated_overloads) {
    structs = _structs;
    functions = _functions;
    overloads = _overloads;

    templated_structs = _templated_structs;
    templated_functions = _templated_functions;
    templated_overloads = _templated_overloads;
}

Program* Program::convert(parser::program *p) {
    std::vector<StructDefinition*> structs;
    std::vector<Function*> functions;
    std::vector<Overload*> overloads;

    std::vector<TemplatedStructDefinition*> templated_structs;
    std::vector<TemplatedFunction*> templated_functions;
    std::vector<TemplatedOverload*> templated_overloads;
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
        else assert(false);
    }
    return new Program(structs, functions, templated_structs, templated_functions, overloads, templated_overloads);
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

    // - check all functions and constructors, make sure they're well formed
    int function_ptr = 0;
    int constructor_ptr = 0;
    int overload_ptr = 0;
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
            std::cout << "CONSTRUCTOR PTR : " << constructor_ptr << "\n";
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
    }

    enclosing_program = nullptr;

    assert(declaration_stack.size() == 0);
    assert(declared_variables.size() == 0);

    return true;
}