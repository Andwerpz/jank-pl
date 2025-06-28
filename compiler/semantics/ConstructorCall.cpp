#include "ConstructorCall.h"
#include "Type.h"
#include "Expression.h"
#include "Constructor.h"
#include "utils.h"
#include "Identifier.h"
#include "Parameter.h"
#include "TemplateMapping.h"

ConstructorCall::ConstructorCall(Type *_type, std::vector<Expression*> _argument_list) {
    assert(_type != nullptr);
    assert(dynamic_cast<ReferenceType*>(_type) == nullptr);
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
    // - can we even emit code to initialize this struct?
    if(!can_initialize_struct(this->type)) {
        std::cout << "Cannot initialize struct : " << type->to_string() << "\n";
        return nullptr;
    }
    
    return get_called_constructor(this);
}

Type* ConstructorCall::resolve_type() {
    Constructor *c = this->resolve_called_constructor();
    if(c == nullptr){
        std::cout << "Cannot resolve constructor call : " << to_string() << "\n";
        return nullptr;
    }
    return c->type;
}

void ConstructorCall::emit_asm() {
    if(asm_debug) fout << indent() << "# calling constructor : " << type->to_string() << "\n";

    //find constructor
    Constructor *c = this->resolve_called_constructor();
    assert(c != nullptr);

    //create new instance of the object
    emit_initialize_struct(this->type);

    //save reference to type for return value
    emit_push("%rbx", "ConstructorCall::emit_asm() : target struct return ref");

    //put reference into %rax
    fout << indent() << "mov %rbx, %rax\n";

    //also pass in struct as argument
    emit_push("%rax", "ConstructorCall::emit_asm() : target struct");

    //create temp variables for all arguments
    push_declaration_stack();
    assert(c->parameters.size() == argument_list.size());
    for(int i = 0; i < argument_list.size(); i++){
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Variable *v = emit_initialize_variable(c->parameters[i]->type, id, argument_list[i]);
        assert(v != nullptr);
    }

    //call constructor
    std::string label = get_constructor_label(c->resolve_constructor_signature());
    fout << indent() << "call " << label << "\n";

    //clean up argument temp variables
    pop_declaration_stack();

    //clean up target struct argument
    emit_add_rsp(8, "ConstructorCall::emit_asm() : target struct");

    //return reference to type
    emit_pop("%rax", "ConstructorCall::emit_asm() : target struct return ref");
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

bool ConstructorCall::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    for(int i = 0; i < argument_list.size(); i++){
        if(!argument_list[i]->replace_templated_types(mapping)) return false;
    }
    return true;
}

bool ConstructorCall::look_for_templates() {
    if(!type->look_for_templates()) return false;
    for(int i = 0; i < argument_list.size(); i++) if(!argument_list[i]->look_for_templates()) return false;
    return true;
}