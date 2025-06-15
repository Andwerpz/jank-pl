#pragma once
#include "../parser/parser.cpp"

struct Identifier {
    std::string name;
    Identifier(std::string _name) {
        name = _name;
    }
    static Identifier* convert(parser::identifier *i);

    size_t hash();
    bool operator==(const Identifier& other);
    bool operator!=(const Identifier& other);
};

