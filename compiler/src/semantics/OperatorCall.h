#pragma once
#include "../parser/parser.h"
#include <optional>

struct Type;
struct Identifier;
struct ExprNode;
struct Function;
struct TemplateMapping;
struct Operator;
struct CompilationContext;

struct OperatorCall {
    std::optional<ExprNode*> left;
    std::string op;
    std::optional<ExprNode*> right;

    OperatorCall(std::optional<ExprNode*> _left, std::string _op, std::optional<ExprNode*> _right);

    Operator* resolve_called_operator(CompilationContext *ctx);
    Type* resolve_type(CompilationContext *ctx);
    void emit_asm(CompilationContext *ctx);
    std::string to_string();
    size_t hash();
    bool equals(OperatorCall *other);
    OperatorCall* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates(CompilationContext* ctx);
};