#include "Identifier.h"

Identifier::Identifier(std::string _name) {
    name = _name;
}

Identifier* Identifier::convert(parser::identifier *i) {
    return new Identifier(i->to_string());
}

size_t Identifier::hash() const {
    return std::hash<std::string>()(name);
}

bool Identifier::equals(const Identifier* other) const {
    if(other == nullptr) return false;
    return name == other->name;
}

bool Identifier::operator==(const Identifier& other) const {
    return this->equals(&other);
}

bool Identifier::operator!=(const Identifier& other) const {
    return !this->equals(&other);
}

Identifier* Identifier::make_copy() {
    return new Identifier(name);
}