#pragma once
#include "../parser/parser.h"
#include <vector>
#include <string>

struct Type;
struct Expression;
struct Constructor;

struct ConstructorCall {
    Type *type;
    std::vector<Expression*> argument_list;
    ConstructorCall(Type *_type, std::vector<Expression*> _argument_list);

    static ConstructorCall* convert(parser::constructor_call *c);
    Constructor* resolve_called_constructor();
    Type* resolve_type();
    void emit_asm();
    std::string to_string();
    size_t hash();
    bool equals(ConstructorCall *other);
    ConstructorCall* make_copy();
};