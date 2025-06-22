#include "ConstructorSignature.h"
#include "Type.h"
#include "utils.h"

ConstructorSignature::ConstructorSignature(Type *_type, std::vector<Type*> _input_types) {
    assert(_type != nullptr);
    for(int i = 0; i < _input_types.size(); i++) assert(_input_types[i] != nullptr);
    type = _type;
    input_types = _input_types;
}

size_t ConstructorSignature::hash() const {
    size_t hash = 0;
    hash_combine(hash, type->hash());
    for(int i = 0; i < input_types.size(); i++){
        hash_combine(hash, input_types[i]->hash());
    }
    return hash;
}

bool ConstructorSignature::equals(const ConstructorSignature* other) const {
    if(other == nullptr) return false;
    if(!type->equals(other->type)) return false;
    if(input_types.size() != other->input_types.size()) return false;
    for(int i = 0; i < input_types.size(); i++) if(!input_types[i]->equals(other->input_types[i])) return false;
    return true;
}

std::string ConstructorSignature::to_string() {
    std::string res = "";
    res += type->to_string();
    res += "(";
    for(int i = 0; i < input_types.size(); i++){
        res += input_types[i]->to_string();
        if(i + 1 != input_types.size()) res += ", ";
    }
    res += ")";
    return res;
}