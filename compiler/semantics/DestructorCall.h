#pragma once
#include "../parser/parser.h"

struct Type;
struct Expression;
struct Destructor;
struct TemplateMapping;

struct DestructorCall {
    Type *type;
    DestructorCall(Type *_type);

    // static DestructorCall* convert(parser::destructor_call *d);
    Destructor* resolve_called_destructor();
    Type* resolve_type();
    void emit_asm(bool should_dealloc = true);
    std::string to_string();
    size_t hash();
    bool equals(DestructorCall *other);
    DestructorCall* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};