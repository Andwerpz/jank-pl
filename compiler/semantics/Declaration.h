#pragma once
#include "../parser/parser.h"

struct Type;
struct Identifier;
struct Expression;
struct TemplateMapping;

struct Declaration {
    Type *type;
    Identifier *id;
    Expression *expr;
    
    Declaration(Type *_type, Identifier *_id, Expression *_expr);

    static Declaration* convert(parser::declaration *d);
    bool is_well_formed();
    Declaration* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    void look_for_templates();
};