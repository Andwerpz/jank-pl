#include "ConstructorCall.h"
#include "Type.h"
#include "Expression.h"
#include "Constructor.h"
#include "utils.h"

ConstructorCall::ConstructorCall(Type *_type, std::vector<Expression*> _argument_list) {
    assert(_type != nullptr);
    for(int i = 0; i < _argument_list.size(); i++) assert(_argument_list[i] != nullptr);
    type = _type;
    argument_list = _argument_list;
}

ConstructorCall* ConstructorCall::convert(parser::constructor_call *c) {
    Type *type = Type::convert(c->t2);
    parser::argument_list *al = c->t6;
    std::vector<Expression*> argument_list;
    if(al->t0 != nullptr) {
        argument_list.push_back(Expression::convert(al->t0->t0));
        for(int i = 0; i < al->t0->t1.size(); i++){
            argument_list.push_back(Expression::convert(al->t0->t1[i]->t3));
        }
    }
    return new ConstructorCall(type, argument_list);
}

Constructor* ConstructorCall::resolve_called_constructor() {
    return get_called_constructor(this);
}

Type* ConstructorCall::resolve_type() {
    return type;
}

void ConstructorCall::emit_asm() {
    // TODO
}

std::string ConstructorCall::to_string() {
    std::string res = "";
    res += "new ";
    res += type->to_string();
    res += "(";
    for(int i = 0; i < argument_list.size(); i++) {
        res += argument_list[i]->to_string();
        if(i + 1 != argument_list.size()) res += ", ";
    }
    res += ")";
    return res;
}

size_t ConstructorCall::hash() {
    size_t hash = 0;
    hash_combine(hash, type->hash());
    for(int i = 0; i < argument_list.size(); i++){
        hash_combine(hash, argument_list[i]->hash());
    }
    return hash;
}

bool ConstructorCall::equals(ConstructorCall *other) {
    if(other == nullptr) return false;
    if(!type->equals(other->type)) return false;
    if(argument_list.size() != other->argument_list.size()) return false;
    for(int i = 0; i < argument_list.size(); i++) if(!argument_list[i]->equals(other->argument_list[i])) return false;
    return true;
}

ConstructorCall* ConstructorCall::make_copy() {
    std::vector<Expression*> argument_list_copy;
    for(int i = 0; i < argument_list.size(); i++){
        argument_list_copy.push_back(argument_list[i]->make_copy());
    }
    return new ConstructorCall(type->make_copy(), argument_list_copy);
}