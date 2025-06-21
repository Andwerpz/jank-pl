#include "Constructor.h"
#include "Type.h"
#include "Parameter.h"
#include "Statement.h"
#include "ConstructorSignature.h"

Constructor::Constructor(Type* _type, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
    type = _type;
    parameters = _parameters;
    body = _body;
}

Constructor* convert(parser::constructor *c) {
    
}