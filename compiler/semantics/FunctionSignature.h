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

namespace std {
    template<>
    struct hash<FunctionSignature*> {
        size_t operator()(const FunctionSignature *fs) const {
            if(fs == nullptr) return 0;
            return fs->hash();
        }
    };
    template<> 
    struct equal_to<FunctionSignature*> {
        bool operator()(const FunctionSignature *lhs, const FunctionSignature *rhs) const { 
            if(lhs == nullptr || rhs == nullptr) return lhs == rhs;
            return lhs->equals(rhs);
        }
    };
}
