#include "Parameter.h"

#include "Type.h"
#include "Identifier.h"
#include "TemplateMapping.h"

Parameter::Parameter(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Parameter::Parameter(const Parameter& other) : ASTNode(other) {
    type = other.type->make_copy();
    id = other.id->make_copy();
}

Parameter::Parameter(Type *_type, Identifier *_id) : ASTNode() {
    type = _type;
    id = _id;
    assert(type != nullptr);
    assert(id != nullptr);
}

Parameter* Parameter::convert(parser::parameter *p) {
    Parameter* result = new Parameter(p);
    result->type = Type::convert(p->t0);
    result->id = Identifier::convert(p->t2);
    return result;
}

Parameter* Parameter::make_copy() {
    return new Parameter(*this);
}

bool Parameter::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    return true;
}

bool Parameter::look_for_templates() {
    return type->look_for_templates();
}

std::string Parameter::to_string() {
    return type->to_string() + " " + id->name;
}