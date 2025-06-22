#pragma once
#include "../parser/parser.h"

struct Type;
struct Identifier;
struct TemplateMapping;

struct Parameter {
    Type *type;
    Identifier *id;
    Parameter(Type *_type, Identifier *_id);
    static Parameter* convert(parser::parameter *p);
    Parameter* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
};