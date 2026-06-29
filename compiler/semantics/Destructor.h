#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct ASTNode;
struct Type;
struct CompoundStatement;
struct TemplateMapping;

struct Destructor : public ASTNode {
    Type *type;
    CompoundStatement *body;

    Destructor(parser::token *tok);
    Destructor(const Destructor& other);
    Destructor(Type *_type, CompoundStatement *_body);

    static Destructor* convert(parser::destructor *d);
    bool equals(Destructor *other) const;
    bool is_well_formed();
    Destructor* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};