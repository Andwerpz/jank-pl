#pragma once
#include "../parser/parser.h"

struct Type;
struct CompoundStatement;
struct TemplateMapping;

struct Destructor {
    Type *type;
    CompoundStatement *body;
    Destructor(Type *_type, CompoundStatement *_body);

    static Destructor* convert(parser::destructor *d);
    bool equals(Destructor *other) const;
    bool is_well_formed();
    Destructor* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};