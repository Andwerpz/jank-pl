#pragma once
#include "../parser/parser.h"
#include <optional>

struct Identifier;
struct Type;

//enclosing type, name, and input types. Does not include return type
struct FunctionSignature {
    std::optional<Type*> enclosing_type;
    Identifier *id;
    std::vector<Type*> input_types;

    //global functions
    FunctionSignature(Identifier *_id, std::vector<Type*> _input_types);

    //struct member functions
    FunctionSignature(Type *_enclosing_type, Identifier *_id, std::vector<Type*> _input_types);

    size_t hash() const;
    bool equals(const FunctionSignature* other) const;
    bool operator==(const FunctionSignature& other) const;
    bool operator!=(const FunctionSignature& other) const;
    std::string to_string();
};