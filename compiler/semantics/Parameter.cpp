#include "Parameter.h"

#include "Type.h"
#include "Identifier.h"

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