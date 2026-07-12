#include "Identifier.h"

Identifier::Identifier(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Identifier::Identifier(const Identifier& other) : ASTNode(other) {
    name = other.name;
}

Identifier::Identifier(std::string _name) : ASTNode() {
    name = _name;
}

Identifier* Identifier::convert(parser::identifier *i) {
    Identifier* result = new Identifier(i);
    result->name = i->to_string();
    return result;
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
    return new Identifier(*this);
}

std::string Identifier::to_string() const {
    return this->name;
}