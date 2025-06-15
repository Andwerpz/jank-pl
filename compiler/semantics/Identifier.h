#pragma once
#include "../parser/parser.h"

struct Identifier {
    std::string name;
    Identifier(std::string _name) {
        name = _name;
    }
    static Identifier* convert(parser::identifier *i);

    size_t hash() const;
    bool equals(Identifier* other) const;
    bool operator==(const Identifier& other) const;
    bool operator!=(const Identifier& other) const;
};

