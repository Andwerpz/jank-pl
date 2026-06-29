#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct Type;
struct Expression;
struct Destructor;
struct TemplateMapping;

struct DestructorCall : public ASTNode {
    Type *type;

    // DestructorCall(parser::tok *tok);
    DestructorCall(const DestructorCall& other);
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