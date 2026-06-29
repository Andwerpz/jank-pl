#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct Identifier : public ASTNode {
    std::string name;

    Identifier(parser::token *tok);
    Identifier(const Identifier& other);
    Identifier(std::string _name);

    static Identifier* convert(parser::identifier *i);
    size_t hash() const;
    bool equals(const Identifier* other) const;
    bool operator==(const Identifier& other) const;
    bool operator!=(const Identifier& other) const;
    Identifier* make_copy() override;
};