#include "FunctionSignature.h"
#include "Type.h"
#include "Identifier.h"
#include "utils.h"

FunctionSignature::FunctionSignature(Identifier *_id, std::vector<Type*> _input_types) {   
    enclosing_type = std::nullopt;
    id = _id;
    input_types = _input_types;
}

FunctionSignature::FunctionSignature(Type *_enclosing_type, Identifier *_id, std::vector<Type*> _input_types) {    
    assert(_enclosing_type != nullptr);
    enclosing_type = _enclosing_type;
    id = _id;
    input_types = _input_types;
}

size_t FunctionSignature::hash() const {
    size_t h = 0;
    if(enclosing_type.has_value()) hash_combine(h, enclosing_type.value()->hash());
    else hash_combine(h, 0);
    hash_combine(h, std::hash<std::string>()(id->name));
    for(int i = 0; i < input_types.size(); i++) {
        hash_combine(h, input_types[i]->hash());
    }
    return h;
}

bool FunctionSignature::equals(const FunctionSignature* other) const {
    if(enclosing_type.has_value() != other->enclosing_type.has_value()) return false;
    if(enclosing_type.has_value() && *(enclosing_type.value()) != *(other->enclosing_type.value())) return false;
    if(*id != *(other->id)) return false;
    if(input_types.size() != other->input_types.size()) return false;
    for(int i = 0; i < input_types.size(); i++){
        if(*(input_types[i]) != *(other->input_types[i])) return false;
    }
    return true;
}

bool FunctionSignature::operator==(const FunctionSignature& other) const {
    return this->equals(&other);
}

bool FunctionSignature::operator!=(const FunctionSignature& other) const {
    return !this->equals(&other);
}

std::string FunctionSignature::to_string() {
    std::string ans = "";
    if(enclosing_type.has_value()) ans += enclosing_type.value()->to_string() + "::";
    ans += id->name;
    ans += "(";
    for(int i = 0; i < input_types.size(); i++) {
        ans += input_types[i]->to_string();
        if(i + 1 != input_types.size()) ans += ", ";
    }
    ans += ")";
    return ans;
}