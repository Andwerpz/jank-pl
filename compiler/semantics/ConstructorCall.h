#pragma once
#include "../parser/parser.h"
#include <vector>
#include <string>

struct Type;
struct Expression;
struct Constructor;
struct TemplateMapping;

//normal constructor calls return r-value 
//construct-in-place constructor calls return l-value

struct ConstructorCall {
    std::optional<Expression*> cip_expr;
    Type *type;
    std::vector<Expression*> argument_list;
    ConstructorCall(std::optional<Expression*> _cip_expr, Type *_type, std::vector<Expression*> _argument_list);

    static ConstructorCall* convert(parser::constructor_call *c);
    Constructor* resolve_called_constructor();
    Type* resolve_type();
    void emit_asm(bool alloc_new);
    std::string to_string();
    size_t hash();
    bool equals(ConstructorCall *other);
    ConstructorCall* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};