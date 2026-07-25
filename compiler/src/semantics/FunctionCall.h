#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"
#include <optional>
#include <vector>

struct Type;
struct Identifier;
struct Expression;
struct Function;
struct TemplateMapping;
struct CompilationContext;

struct FunctionCall : public ASTNode {
    std::optional<Type*> target_type;           // type of enclosing struct, probably should rename this
    Identifier *id;
    std::vector<Expression*> argument_list;
    
    FunctionCall(parser::token *tok);
    FunctionCall(const FunctionCall& other);
    FunctionCall(Identifier *_id, std::vector<Expression*> _argument_list);
    FunctionCall(Type *_target_type, Identifier *_id, std::vector<Expression*> _argument_list);
    FunctionCall(std::optional<Type*> _target_type, Identifier *_id, std::vector<Expression*> _argument_list);

    static FunctionCall* convert(parser::function_call *f);
    Function* resolve_called_function(CompilationContext *ctx);
    Type* resolve_type(CompilationContext *ctx);
    void emit_asm(CompilationContext *ctx);
    std::string to_string();
    size_t hash();
    bool equals(FunctionCall *other);
    FunctionCall* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates(CompilationContext* ctx);
};