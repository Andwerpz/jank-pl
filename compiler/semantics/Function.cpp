#include "Function.h"
#include "Identifier.h"
#include "Type.h"
#include "FunctionSignature.h"
#include "utils.h"
#include "Statement.h"
#include "Parameter.h"
#include "TemplateMapping.h"
#include "primitives.h"
#include "GlobalDeclaration.h"
#include "Program.h"
#include "Declaration.h"
#include "Expression.h"
#include <algorithm>

Function::Function(std::optional<Type*> _enclosing_type, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
    enclosing_type = _enclosing_type;
    id = _id;
    type = _type;
    parameters = _parameters;
    body = _body;

    assert(type != nullptr);
    assert(body != nullptr);
    assert(id != nullptr);
}

Function::Function(Type *_type, Identifier *_id, std::vector<Type*> input_types) {
    enclosing_type = std::nullopt;
    id = _id;
    type = _type;
    for(int i = 0; i < input_types.size(); i++){
        parameters.push_back(new Parameter(input_types[i], new Identifier("v" + std::to_string(i))));
    }
    body = nullptr;

    assert(type != nullptr);
    assert(id != nullptr);
}

FunctionSignature* Function::resolve_function_signature() const {
    std::vector<Type*> input_types;
    for(int i = 0; i < parameters.size(); i++) input_types.push_back(parameters[i]->type);
    if(enclosing_type.has_value()) return new FunctionSignature(enclosing_type.value(), id, input_types);
    return new FunctionSignature(id, input_types);
}

bool Function::equals(const Function* other) const {
    return this->resolve_function_signature()->equals(other->resolve_function_signature());
}

bool Function::operator==(const Function& other) const {
    return this->equals(&other);
}

bool Function::operator!=(const Function& other) const {
    return !(*this == other);
}

Function* Function::convert(parser::function *f) {
    parser::function_definition *def = f->t0;
    parser::parameter_list *pl = def->t6;
    Type *type = Type::convert(def->t0);
    Identifier *name = new Identifier(def->t2->to_string());
    std::vector<Parameter*> parameters;
    if(pl->t0 != nullptr) {
        parameters.push_back(Parameter::convert(pl->t0->t0));
        std::vector<parser::parameter_list::a0::b0*> tmp = pl->t0->t1;
        for(int i = 0; i < tmp.size(); i++){
            parameters.push_back(Parameter::convert(tmp[i]->t3));
        }
    }
    CompoundStatement *body = CompoundStatement::convert(f->t2);
    return new Function(std::nullopt, type, name, parameters, body);
}

bool Function::is_well_formed() {
    FunctionSignature *fs = resolve_function_signature();

    if(body == nullptr) {
        std::cout << "SKIPPING SYS FUNCTION : " << fs->to_string() << std::endl;
        return true;
    }
    std::cout << "CHECKING FUNCTION : " << fs->to_string() << std::endl;

    // - are templates all resolvable?
    if(!look_for_templates()) {
        std::cout << "Unable to resolve templates in function : " << resolve_function_signature()->to_string() << "\n";
        return false;
    }

    bool is_main = this->is_main();

    //print function label
    //if function signature is main(), substitute main for _start
    if(asm_debug) {
        fout << "# " << fs->to_string() << "\n";
    }
    if(is_main) {
        fout << ".global _start\n";
        fout << "_start:\n";
    }
    else {
        std::string label = get_function_label(fs);
        fout << label << ":\n";
    }

    //if this is main, initialize global variables
    if(is_main) {
        std::cout << "INITIALIZING GLOBAL VARIABLES" << std::endl;

        if(asm_debug) fout << indent() << "# start initialize global variables\n";

        //resolve global declaration templates
        std::vector<GlobalDeclaration*> global_declarations = enclosing_program->global_declarations;
        std::cout << "GLOBAL AMT : " << global_declarations.size() << "\n";
        for(int i = 0; i < global_declarations.size(); i++){
            if(!global_declarations[i]->look_for_templates()) {
                std::cout << "Unable to resolve templates in declaration of global variable " << global_declarations[i]->declaration->id->name << "\n";
                return false;
            }
        }

        //save global base pointer to %r15
        fout << indent() << "mov %rsp, %r15\n";

        //reserve enough memory. This should not be managed by local_offset
        fout << indent() << "sub $" << global_declarations.size() * 8 << ", %rsp\n";

        //setup fake stack frame. We need to do this because pop_declaration_stack() will not actually
        //move %rsp if it's the last element on the stack. 
        push_declaration_stack();

        //%rbp should start by pointing towards %rsp for proper local variable behaviour
        fout << indent() << "mov %rsp, %rbp\n";

        //sort by tier and initialize
        std::sort(global_declarations.begin(), global_declarations.end(), [](GlobalDeclaration *a, GlobalDeclaration *b) -> bool {
            return a->tier < b->tier;
        });
        for(int i = 0; i < global_declarations.size(); i++){
            Type *type = global_declarations[i]->declaration->type;
            Identifier *id = global_declarations[i]->declaration->id;
            Expression *expr = global_declarations[i]->declaration->expr;
            std::string addr_str = std::to_string(i * 8) + "(%r15)";
            std::cout << "GLOBAL : " << type->to_string() << " " << id->name << "\n";

            if(asm_debug) fout << indent() << "# initialize global variable : " << type->to_string() << " " << id->name << "\n";
            Variable *v = emit_initialize_variable(type, id, expr, addr_str, true);
            if(asm_debug) fout << indent() << "# done initialize global variable : " << type->to_string() << " " << id->name << "\n";
        
            if(v == nullptr) {
                std::cout << "Failed to initialize global variable : " << type->to_string() << " " << id->name << "\n";
                return false;
            }
        }

        if(asm_debug) fout << indent() << "# done initialize global variables\n";

        // remove fake stack frame
        pop_declaration_stack();

        std::cout << "DONE INITIALIZE GLOBAL VARIABLES" << std::endl;
    }

    //setup function stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";

    push_declaration_stack();
    
    for(int i = 0; i < parameters.size(); i++){
        // - does parameter correspond to existing type?
        if(!is_type_declared(parameters[i]->type)) {
            std::cout << "Undeclared type : " << parameters[i]->type->to_string() << "\n";
            return false;
        }
        // - is parameter type not void?
        if(parameters[i]->type->equals(primitives::_void)) {
            std::cout << "Parameter can't have type void\n";
            return false;
        }
    }

    // - is return type of function existing?
    if(!is_type_declared(type)) {
        std::cout << "Function undeclared return type : " << type->to_string() << " " << id->name << "\n";
        return false;
    }
    
    //if has enclosing type, register self as variable (Type& this)
    local_offset = 8 + 8 * parameters.size();
    if(enclosing_type.has_value()) {
        //adjust local offset for 'extra variable'
        local_offset += 8;

        //register self as variable (Type& this)
        Type *vt = new ReferenceType(enclosing_type.value());
        Identifier *vid = new Identifier("this");
        Variable* v = add_variable(vt, vid);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << vt << " " << vid << "\n";
            return false;
        }
        v->addr = std::to_string(local_offset) + "(%rbp)";
        local_offset -= 8;
    }
    for(int i = 0; i < parameters.size(); i++){
        Variable* v = add_variable(parameters[i]->type, parameters[i]->id);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << parameters[i]->type->to_string() << " " << parameters[i]->id->name << "\n";
            return false;
        }
        v->addr = std::to_string(local_offset) + "(%rbp)";
        local_offset -= 8;
    }

    //set local offset equal to %rsp
    local_offset = 0;

    //check that the local stack is empty so far
    assert(stack_desc.size() == 0);

    // - make sure body is well formed
    if(!body->is_well_formed()) {
        std::cout << "Function body not well formed\n";
        return false;
    }
    
    // - if type is not void, check for existence of return statement as last reachable statement
    // - constructors also don't have to have return statements, they're treated as void functions
    if(!type->equals(primitives::_void)) {
        // Note that if there is a statement before the last that is always returning, then any statement after
        // it is unreachable code, in which case we should print some warnings. 
        if(!body->is_always_returning()) {
            std::cout << "Non-void function doesn't always return\n";
            return false;
        }
    }
    else {
        //add trailing return for void functions
        fout << indent() << "pop %rbp\n";   //should not be managed by local_offset
        fout << indent() << "ret\n";
    }

    fout << "\n";

    //unregister parameters as variables
    pop_declaration_stack();

    //local stack should be empty before returning
    assert(stack_desc.size() == 0);

    return true;
}

Function* Function::make_copy() {
    std::optional<Type*> _enclosing_type = std::nullopt;
    if(enclosing_type.has_value()) _enclosing_type = enclosing_type.value()->make_copy();
    Type *_type = type->make_copy();
    Identifier *_id = id->make_copy();
    std::vector<Parameter*> _parameters;
    for(int i = 0; i < parameters.size(); i++){
        _parameters.push_back(parameters[i]->make_copy());
    }
    CompoundStatement *_body = dynamic_cast<CompoundStatement*>(body->make_copy());
    return new Function(_enclosing_type, _type, _id, _parameters, _body);
}

bool Function::replace_templated_types(TemplateMapping *mapping) {
    if(enclosing_type.has_value()) {
        Type *val = enclosing_type.value();
        if(auto x = mapping->find_mapped_type(val)) val = x;
        else if(!val->replace_templated_types(mapping)) return false;
        enclosing_type = val;
    }
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    for(int i = 0; i < parameters.size(); i++){
        if(!parameters[i]->replace_templated_types(mapping)) return false;
    }
    if(!body->replace_templated_types(mapping)) return false;
    return true;
}

bool Function::look_for_templates() {
    if(enclosing_type.has_value()) if(!enclosing_type.value()->look_for_templates()) return false;
    if(!type->look_for_templates()) return false;
    for(int i = 0; i < parameters.size(); i++) if(!parameters[i]->look_for_templates()) return false;
    if(!body->look_for_templates()) return false;
    return true;
}

bool Function::is_main() {
    FunctionSignature *fs = resolve_function_signature();
    assert(fs != nullptr);
    return fs->equals(new FunctionSignature(new Identifier("main"), {}));
}