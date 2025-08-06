#include "Typedef.h"
#include "Type.h"
#include "utils.h"

Typedef::Typedef(Type *_type, BaseType *_base_type) {
    type = _type;
    base_type = _base_type;
    assert(type != nullptr);
    assert(base_type != nullptr);
}

Typedef* Typedef::convert(parser::typedef *t) {
    // TODO
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
    return new Typedef(type->make_copy(), base_type->make_copy());
}

bool Typedef::replace_templated_types(TemplateMapping *mapping) {
    //the base type in a typedef should never be replaced
    assert(mapping->find_mapped_type(base_type) == nullptr);
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    return true;
}
