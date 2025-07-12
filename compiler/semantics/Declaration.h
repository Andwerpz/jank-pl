#pragma once
#include "../parser/parser.h"
#include <optional>

struct Type;
struct Identifier;
struct Expression;
struct TemplateMapping;

struct Declaration {
    Type *type;
    Identifier *id;
    std::optional<Expression*> expr;
    
    Declaration(Type *_type, Identifier *_id, std::optional<Expression*> _expr);

    static Declaration* convert(parser::declaration *d);
    bool is_well_formed();
    Declaration* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
    std::string to_string();
};