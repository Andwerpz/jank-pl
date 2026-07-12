#include "FunctionCall.h"

#include "Identifier.h"
#include "Expression.h"
#include "Type.h"
#include "utils.h"
#include "Function.h"
#include "Parameter.h"
#include "TemplateMapping.h"
#include "CompilationContext.h"

FunctionCall::FunctionCall(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

FunctionCall::FunctionCall(const FunctionCall& other) : ASTNode(other) {
    target_type = std::nullopt;
    if(other.target_type.has_value()) target_type = other.target_type.value()->make_copy();
    id = other.id->make_copy();
    for(int i = 0; i < other.argument_list.size(); i++) argument_list.push_back(other.argument_list[i]->make_copy());
}

FunctionCall::FunctionCall(Identifier *_id, std::vector<Expression*> _argument_list) : ASTNode() {
    target_type = std::nullopt;
    id = _id;
    argument_list = _argument_list;
}

FunctionCall::FunctionCall(Type *_target_type, Identifier *_id, std::vector<Expression*> _argument_list) : ASTNode() {
    target_type = _target_type;
    id = _id;
    argument_list = _argument_list;
}

FunctionCall::FunctionCall(std::optional<Type*> _target_type, Identifier *_id, std::vector<Expression*> _argument_list) : ASTNode() {
    target_type = _target_type;
    id = _id;
    argument_list = _argument_list;
}

FunctionCall* FunctionCall::convert(parser::function_call *f) {
    FunctionCall* result = new FunctionCall(f);
    result->target_type = std::nullopt;         // need context for this
    result->id = Identifier::convert(f->t0);
    result->argument_list = convert_argument_list(f->t4);
    return result;
}

Function* FunctionCall::resolve_called_function(CompilationContext *ctx) {
    return ctx->get_called_function(this);
}

Type* FunctionCall::resolve_type(CompilationContext *ctx) {
    Function *f = this->resolve_called_function(ctx);
    if(f == nullptr) {
        std::cout << "Cannot resolve function call : " << to_string() << "\n";
        return nullptr;
    }
    return f->type;
}

void FunctionCall::emit_asm(CompilationContext *ctx) {
    if(asm_debug) fout << indent() << "# calling function : " << id->name << "\n";
    
    //find original function
    Function *f = this->resolve_called_function(ctx);
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
        if(asm_debug) fout << indent() << "# function call member variable : " << f->parameters[i]->id->name << "\n";
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Variable *v = emit_initialize_stack_variable(ctx, f->parameters[i]->type, id, argument_list[i]);
        assert(v != nullptr);
    }

    //call function
    std::string label = get_function_label(f->resolve_function_signature());
    fout << indent() << "call " << label << "\n";

    //clean up argument temp variables, freeing them is handled by the function itself
    pop_declaration_stack(ctx, false);
    
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
    return new FunctionCall(*this);
}

bool FunctionCall::replace_templated_types(TemplateMapping *mapping) {
    if(target_type.has_value()) {
        Type *val = target_type.value();
        if(auto x = mapping->find_mapped_type(val)) val = x;
        else if(!val->replace_templated_types(mapping)) return false;
        target_type = val;
    }
    for(int i = 0; i < argument_list.size(); i++){
        if(!argument_list[i]->replace_templated_types(mapping)) return false;
    }
    return true;
}

bool FunctionCall::look_for_templates(CompilationContext *ctx) {
    if(target_type.has_value()) if(!target_type.value()->look_for_templates(ctx)) return false;
    for(int i = 0; i < argument_list.size(); i++) if(!argument_list[i]->look_for_templates(ctx)) return false;
    return true;
}