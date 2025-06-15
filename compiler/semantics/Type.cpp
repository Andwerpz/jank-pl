#include "Type.h"

// -- BaseType --
int BaseType::calc_size() {
    if(name == "int") return 8;
    else if(name == "char") return 1;
    else return 8;
}

bool BaseType::equals(const Type *other) const {
    if(auto x = dynamic_cast<const BaseType*>(other)) return name == x->name;
    return false;
}

size_t BaseType::hash() const {
    return std::hash<std::string>()(name) ^ 0x9e3779b9;
}

std::string BaseType::to_string() {
    return name;
}

// -- PointerType --
int PointerType::calc_size() {
    return 8;
}

bool PointerType::equals(const Type *other) const {
    if(auto x = dynamic_cast<const PointerType*>(other)) return *type == *(x->type);
    return false;
}

size_t PointerType::hash() const {
    return type->hash() ^ 0x13952424;
}

std::string PointerType::to_string() {
    return type->to_string() + "*";
}

// -- ReferenceType --
int ReferenceType::calc_size() {
    return 8;
}

bool ReferenceType::equals(const Type *other) const {
    if(auto x = dynamic_cast<const ReferenceType*>(other)) return *type == *(x->type);
    return false;
}

size_t ReferenceType::hash() const {
    return type->hash() ^ 0xdeadbeef;
}

std::string ReferenceType::to_string() {
    return type->to_string() + "&";
}

// -- convert --
Type* Type::convert(parser::type *t) {
    Type *res = BaseType::convert(t->t0);
    for(int i = 0; i < t->t1.size(); i++){
        std::string suf = t->t1[i]->to_string();
        if(suf == "*") res = new PointerType(res);
        else if(suf == "&") res = new ReferenceType(res);
        else assert(false);
    }
    return res;
}

BaseType* BaseType::convert(parser::base_type *t) {
    return new BaseType(t->to_string());
}