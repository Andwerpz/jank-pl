#include "Identifier.h"

Identifier* Identifier::convert(parser::identifier *i) {
    return new Identifier(i->to_string());
}

size_t Identifier::hash() const {
    return std::hash<std::string>()(name);
}

bool Identifier::equals(Identifier* other) const {
    if(other == nullptr) return false;
    return name == other->name;
}

bool Identifier::operator==(const Identifier& other) const {
    return name == other.name;
}

bool Identifier::operator!=(const Identifier& other) const {
    return !(*this == other);
}