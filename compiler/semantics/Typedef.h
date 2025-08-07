#pragma once
#include "../parser/parser.h"

struct Type;
struct BaseType;
struct TemplateMapping;

struct Typedef {
    Type *type;
    BaseType *base_type;
    Typedef(Type *_type, BaseType *_base_type);
    static Typedef* convert(parser::_typedef *t);

    bool equals(const Typedef *other) const;
    size_t hash() const;
    std::string to_string() const;
    Typedef* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
};