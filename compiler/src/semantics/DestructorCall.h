#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct Type;
struct Expression;
struct Destructor;
struct TemplateMapping;
struct CompilationContext;

struct DestructorCall : public ASTNode {
    Type *type;

    // DestructorCall(parser::tok *tok);
    DestructorCall(const DestructorCall& other);
    DestructorCall(Type *_type);

    // static DestructorCall* convert(parser::destructor_call *d);
    Destructor* resolve_called_destructor(CompilationContext *ctx);
    Type* resolve_type(CompilationContext *ctx);
    void emit_asm(CompilationContext *ctx, bool should_dealloc = true);
    std::string to_string();
    size_t hash();
    bool equals(DestructorCall *other);
    DestructorCall* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates(CompilationContext* ctx);
};