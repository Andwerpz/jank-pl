#include "FunctionCall.h"

#include "Identifier.h"
#include "Expression.h"
#include "Type.h"
#include "utils.h"
#include "Function.h"
#include "Parameter.h"

FunctionCall::FunctionCall(Identifier *_id, std::vector<Expression*> _argument_list) {
    target_type = std::nullopt;
    id = _id;
    argument_list = _argument_list;
}

FunctionCall::FunctionCall(Type *_target_type, Identifier *_id, std::vector<Expression*> _argument_list) {
    target_type = _target_type;
    id = _id;
    argument_list = _argument_list;
}

FunctionCall::FunctionCall(std::optional<Type*> _target_type, Identifier *_id, std::vector<Expression*> _argument_list) {
    target_type = _target_type;
    id = _id;
    argument_list = _argument_list;
}

FunctionCall* FunctionCall::convert(parser::function_call *f) {
    Identifier *fname = Identifier::convert(f->t0);
    std::vector<Expression*> argument_list;
    parser::argument_list *arglist = f->t4;
    if(arglist->t0 != nullptr) {
        argument_list.push_back(Expression::convert(arglist->t0->t0));
        for(int i = 0; i < arglist->t0->t1.size(); i++){
            argument_list.push_back(Expression::convert(arglist->t0->t1[i]->t3));
        }
    }
    return new FunctionCall(fname, argument_list);
}

Function* FunctionCall::resolve_called_function() {
    return get_called_function(this);
}

Type* FunctionCall::resolve_type() {
    Function *f = this->resolve_called_function();
    if(f == nullptr) {
        std::cout << "Cannot resolve function call : " << to_string() << "\n";
        return nullptr;
    }
    return f->type;
}

void FunctionCall::emit_asm() {
    if(asm_debug) fout << indent() << "# calling function : " << id->name << "\n";
    
    //find original function
    Function *f = this->resolve_called_function();
    assert(f != nullptr);
    
    //if is member function, pass in target struct as an argument
    //expects the target struct to be in %rax
    if(target_type.has_value()) {  //member function
        emit_push("%rax", "FunctionCall::emit_asm() : target struct");
    }

    //create temp variables for all arguments
    push_declaration_stack();
    assert(f->parameters.size() == argument_list.size());
    for(int i = 0; i < argument_list.size(); i++){
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Variable *v = emit_initialize_variable(f->parameters[i]->type, id, argument_list[i]);
        assert(v != nullptr);
    }

    //call function
    std::string label = get_function_label(f->resolve_function_signature());
    fout << indent() << "call " << label << "\n";

    //clean up argument temp variables
    pop_declaration_stack();

    //clean up target struct argument
    if(target_type.has_value()) {
        emit_add_rsp(8, "FunctionCall::emit_asm() : target struct");
    }
}

std::string FunctionCall::to_string() {
    std::string res = "";
    res += id->name;
    res += "(";
    for(int i = 0; i < argument_list.size(); i++){
        res += argument_list[i]->to_string();
        if(i + 1 != argument_list.size()) res += ", ";
    }
    res += ")";
    return res;
}

size_t FunctionCall::hash() {
    size_t hash = 0;
    if(target_type.has_value()) hash_combine(hash, target_type.value()->hash());
    else hash_combine(hash, 0);
    hash_combine(hash, id->hash());
    for(int i = 0; i < argument_list.size(); i++) hash_combine(hash, argument_list[i]->hash());
    return hash;
}

bool FunctionCall::equals(FunctionCall *other) {
    if(target_type.has_value() != other->target_type.has_value()) return false;
    if(target_type.has_value() && !target_type.value()->equals(other->target_type.value())) return false;
    if(!id->equals(other->id)) return false;
    if(argument_list.size() != other->argument_list.size()) return false;
    for(int i = 0; i < argument_list.size(); i++) if(!argument_list[i]->equals(other->argument_list[i])) return false;
    return true;
}

FunctionCall* FunctionCall::make_copy() {
    std::optional<Type*> _target_type = std::nullopt;
    if(target_type.has_value()) _target_type = target_type.value()->make_copy();
    Identifier *_id = id->make_copy();
    std::vector<Expression*> _argument_list(argument_list.size());
    for(int i = 0; i < argument_list.size(); i++){
        _argument_list[i] = argument_list[i]->make_copy();
    }
    return new FunctionCall(_target_type, _id, _argument_list);
}