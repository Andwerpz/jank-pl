#pragma once
#include "../parser/parser.h"

struct Type;

struct ConstructorSignature {
    Type *type;
    std::vector<Type*> input_types;
    ConstructorSignature(Type *_type, std::vector<Type*> _input_types);

    size_t hash() const;
    bool equals(const ConstructorSignature* other) const;
    std::string to_string();
};  