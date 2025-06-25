#pragma once
#include "../parser/parser.h"
#include <optional>

struct Type;
struct Identifier;
struct ExprNode;
struct Function;
struct TemplateMapping;
struct Overload;

struct OverloadCall {
    std::optional<ExprNode*> left;
    std::string op;
    std::optional<ExprNode*> right;

    OverloadCall(std::optional<ExprNode*> _left, std::string _op, std::optional<ExprNode*> _right);

    Overload* resolve_called_overload();
    Type* resolve_type();
    void emit_asm();
    std::string to_string();
    size_t hash();
    bool equals(OverloadCall *other);
    OverloadCall* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};