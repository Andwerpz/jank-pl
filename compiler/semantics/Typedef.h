#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct Type;
struct BaseType;
struct TemplateMapping;

struct Typedef : public ASTNode {
    Type *type;
    BaseType *base_type;

    Typedef(parser::token *tok);
    Typedef(const Typedef& other);
    Typedef(Type *_type, BaseType *_base_type);

    static Typedef* convert(parser::_typedef *t);
    bool equals(const Typedef *other) const;
    size_t hash() const;
    std::string to_string() const;
    Typedef* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
};