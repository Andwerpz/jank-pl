#include "OperatorSignature.h"
#include "utils.h"
#include "Type.h"

OperatorSignature::OperatorSignature(std::optional<Type*> _left, std::string _op, std::optional<Type*> _right) {
    if(_left.has_value()) assert(_left.value() != nullptr);
    if(_right.has_value()) assert(_right.value() != nullptr);
    left = _left;
    op = _op;
    right = _right;
}

OperatorSignature::OperatorSignature(Type *_left, std::string _op, Type *_right) {
    assert(_left != nullptr);
    assert(_right != nullptr);
    left = _left;
    op = _op;
    right = _right;
}

OperatorSignature::OperatorSignature(std::string _op, Type *_right) {
    assert(_right != nullptr);
    left = std::nullopt;
    op = _op;
    right = _right;
}

OperatorSignature::OperatorSignature(Type *_left, std::string _op) {
    assert(_left != nullptr);
    left = _left;
    op = _op;
    right = std::nullopt;
}

OperatorSignature::OperatorSignature(Type *from, Type *to) {
    left = from;
    op = "$";
    right = to;
}

size_t OperatorSignature::hash() const {
    size_t hash = 0;
    if(left.has_value()) hash_combine(hash, left.value()->hash());
    else hash_combine(hash, 0);
    hash_combine(hash, std::hash<std::string>()(op));
    if(right.has_value()) hash_combine(hash, right.value()->hash());
    else hash_combine(hash, 0);
    return hash;
}

bool OperatorSignature::equals(const OperatorSignature* other) const {
    if(left.has_value() != other->left.has_value()) return false;
    if(left.has_value() && *(left.value()) != *(other->left.value())) return false;
    if(right.has_value() != other->right.has_value()) return false;
    if(right.has_value() && *(right.value()) != *(other->right.value())) return false;
    return op == other->op;
}

std::string OperatorSignature::to_string() {
    std::string res = "";
    if(left.has_value()) res += left.value()->to_string() + " ";
    res += op;
    if(right.has_value()) res += " " + right.value()->to_string();
    return res;
}