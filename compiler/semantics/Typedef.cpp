#include "Typedef.h"
#include "Type.h"
#include "utils.h"
#include "TemplateMapping.h"

Typedef::Typedef(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Typedef::Typedef(const Typedef& other) : ASTNode(other) {
    type = other.type->make_copy();
    base_type = dynamic_cast<BaseType*>(other.base_type->make_copy());
}

Typedef::Typedef(Type *_type, BaseType *_base_type) : ASTNode() {
    type = _type;
    base_type = _base_type;
    assert(type != nullptr);
    assert(base_type != nullptr);
}

Typedef* Typedef::convert(parser::_typedef *t) {
    Typedef* result = new Typedef(t);
    result->type = Type::convert(t->t2);
    result->base_type = BaseType::convert(t->t4);
    return result;
}   

bool Typedef::equals(const Typedef *other) const {
    if(!type->equals(other->type)) return false;
    if(!base_type->equals(other->base_type)) return false;
    return true;
}

size_t Typedef::hash() const {
    size_t hash = 0;
    hash_combine(hash, type->hash());
    hash_combine(hash, base_type->hash());
    return hash;
}

std::string Typedef::to_string() const {
    return "typedef " + type->to_string() + " " + base_type->to_string() + ";";
}

Typedef* Typedef::make_copy() {
    return new Typedef(*this);
}

bool Typedef::replace_templated_types(TemplateMapping *mapping) {
    //the base type in a typedef should never be replaced
    if(!(mapping->find_mapped_type(base_type) == nullptr)) {
        assert(false);
        return false;
    }
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    return true;
}
