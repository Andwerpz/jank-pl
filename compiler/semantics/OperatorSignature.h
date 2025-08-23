#pragma once
#include "../parser/parser.h"
#include <optional>

struct Type;

struct OperatorSignature {
    std::optional<Type*> left;
    std::string op;
    std::optional<Type*> right;
    OperatorSignature(std::optional<Type*> _left, std::string _op, std::optional<Type*> _right);
    OperatorSignature(Type *_left, std::string _op, Type *_right);
    OperatorSignature(std::string _op, Type *_right);
    OperatorSignature(Type *_left, std::string _op);
    OperatorSignature(Type *from, Type *to);

    size_t hash() const;
    bool equals(const OperatorSignature* other) const;
    std::string to_string();
};  
