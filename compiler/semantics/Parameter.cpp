#include "Parameter.h"

#include "Type.h"
#include "Identifier.h"
#include "TemplateMapping.h"

Parameter::Parameter(Type *_type, Identifier *_id) {
    assert(_type != nullptr);
    assert(_id != nullptr);
    type = _type;
    id = _id;
}

Parameter* Parameter::convert(parser::parameter *p) {
    Type *type = Type::convert(p->t0);
    Identifier* name = Identifier::convert(p->t2);
    return new Parameter(type, name);
}

Parameter* Parameter::make_copy() {
    return new Parameter(type->make_copy(), id->make_copy());
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