#include "ConstructorCall.h"
#include "Type.h"
#include "Expression.h"
#include "Constructor.h"
#include "utils.h"
#include "Identifier.h"
#include "Parameter.h"
#include "TemplateMapping.h"

ConstructorCall::ConstructorCall(std::optional<Expression*> _cip_expr, Type *_type, std::vector<Expression*> _argument_list) {
    assert(_type != nullptr);
    assert(dynamic_cast<ReferenceType*>(_type) == nullptr);
    cip_expr = _cip_expr;
    for(int i = 0; i < _argument_list.size(); i++) assert(_argument_list[i] != nullptr);
    type = _type;
    argument_list = _argument_list;
}

ConstructorCall* ConstructorCall::convert(parser::constructor_call *c) {
    std::optional<Expression*> cip_expr = std::nullopt;
    if(c->t2.has_value()) {
        cip_expr = Expression::convert(c->t2.value()->t2);
    }
    Type *type = Type::convert(c->t3);
    parser::argument_list *al = c->t7;
    std::vector<Expression*> argument_list = convert_argument_list(al);
    return new ConstructorCall(cip_expr, type, argument_list);
}

Constructor* ConstructorCall::resolve_called_constructor() {
    // - can we even emit code to initialize this struct?
    //equivalent to checking if a struct layout for this type exists if it's not primitive
    if(!is_type_primitive(type) && !get_struct_layout(type)) {
        std::cout << "Cannot initialize struct : " << type->to_string() << "\n";
        return nullptr;
    }
    
    return get_called_constructor(this);
}

Type* ConstructorCall::resolve_type() {
    // - try to resolve called constructor
    Constructor *c = this->resolve_called_constructor();
    if(c == nullptr){
        std::cout << "Cannot resolve constructor call : " << to_string() << "\n";
        return nullptr;
    }

    // - if this is construct-in-place, make sure that cip_expr resolves to a pointer
    if(cip_expr.has_value()) {
        Type *et = cip_expr.value()->resolve_type();
        if(et == nullptr) {
            std::cout << "Construct in place expression does not resolve to type : " << cip_expr.value()->to_string() << "\n";
            return nullptr;
        }
        if(dynamic_cast<PointerType*>(et) == nullptr) {
            std::cout << "Construct in place expression does not resolve to pointer type : " << cip_expr.value()->to_string() << " : " << et->to_string() << "\n";
            return nullptr;
        }
    }

    return c->type;
}

//if addr_provided, assumes memory addr is already in %rax
//calls emit_initialize_struct()
//should return with %rax holding initialized type
void ConstructorCall::emit_asm(bool addr_provided) {
    if(asm_debug) fout << indent() << "# calling constructor : " << type->to_string() << "\n";

    //should never have a provided address and construct-in-place
    if(addr_provided && cip_expr.has_value()) {
        assert(false);
    }

    //find constructor
    Constructor *c = this->resolve_called_constructor();
    assert(c != nullptr);

    if(!is_type_primitive(type)) {
        assert(dynamic_cast<StructConstructor*>(c) != nullptr);

        //somehow place memory address of struct into %rax
        if(!addr_provided) {
            if(cip_expr.has_value()) {  
                //resolve cip expr for memory address
                cip_expr.value()->emit_asm();
            }
            else {
                //if type is not primitive, make sure struct layout exists
                //it should always exist if we've gotten this far
                if(!is_type_primitive(type)) {
                    StructLayout *sl = get_struct_layout(type);
                    if(!sl) {
                        std::cout << "Cannot initialize non-primitive type " << type->to_string() << " without struct layout" << std::endl;
                        assert(false);
                    }
                }

                //malloc the required memory
                int heap_sz = type->calc_size();
                emit_malloc(heap_sz);
            }
        }

        //initialize struct in given memory
        emit_initialize_struct(type);

        //save reference to type for return value
        emit_push("%rax", "ConstructorCall::emit_asm() : target struct return ref %rax");

        //also pass in struct as argument
        emit_push("%rax", "ConstructorCall::emit_asm() : target struct");

        //create temp variables for all arguments
        push_declaration_stack();
        assert(c->parameters.size() == argument_list.size());
        for(int i = 0; i < argument_list.size(); i++){
            Identifier *id = new Identifier(create_new_tmp_variable_name());
            Variable *v = emit_initialize_stack_variable(c->parameters[i]->type, id, argument_list[i]);
            assert(v != nullptr);
        }

        //call constructor
        std::string label = get_constructor_label(c->resolve_constructor_signature());
        fout << indent() << "call " << label << "\n";

        //clean up argument temp variables, freeing them is handled by the constructor
        pop_declaration_stack(false);

        //clean up target struct argument
        emit_add_rsp(8, "ConstructorCall::emit_asm() : target struct");

        //return reference to type
        emit_pop("%rax", "ConstructorCall::emit_asm() : target struct return ref %rax");

        //if we are constructing in place, need to place address in %rcx as we're returning l-value
        if(cip_expr.has_value()) {
            fout << indent() << "mov %rax, %rcx\n";
        }
    }
    else {
        assert(dynamic_cast<PrimitiveConstructor*>(c) != nullptr);

        //create 'this' on the stack
        emit_sub_rsp(8, "ConstructorCall::emit_asm() : primitive this");
        fout << indent() << "lea (%rsp), %rax\n";
        emit_initialize_primitive(type);

        //create temp variables for all arguments
        push_declaration_stack();
        assert(c->parameters.size() == argument_list.size());
        for(int i = 0; i < argument_list.size(); i++){
            Identifier *id = new Identifier(create_new_tmp_variable_name());
            Variable *v = emit_initialize_stack_variable(c->parameters[i]->type, id, argument_list[i]);
            assert(v != nullptr);
        }

        //call constructor
        std::string label = get_constructor_label(c->resolve_constructor_signature());
        fout << indent() << "call " << label << "\n";

        //clean up argument temp variables, freeing them is handled by the constructor
        pop_declaration_stack(false);

        //return primitive
        emit_pop("%rax", "ConstructorCall::emit_asm() : primitive this");

        //if we are constructing in place, put primitive into cip address, and place address into %rcx
        if(cip_expr.has_value()) {
            //resolve cip expr for memory address
            emit_push("%rax", "ConstructorCall::emit_asm() : primitive cip save value");
            cip_expr.value()->emit_asm();

            //place primitive into cip addr
            fout << indent() << "mov %rax, %rbx\n";
            emit_pop("%rax", "ConstructorCall::emit_asm() : primitive cip save value");
            emit_mem_store(type->calc_size());

            //place cip addr into %rcx
            fout << indent() << "mov %rbx, %rcx\n";
        }
    }
}

std::string ConstructorCall::to_string() {
    std::string res = "";
    res += "new ";
    if(cip_expr.has_value()) {
        res += "(" + cip_expr.value()->to_string() + ") ";
    }
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
    std::optional<Expression*> _cip_expr = std::nullopt;
    if(cip_expr.has_value()) {
        _cip_expr = cip_expr.value()->make_copy();
    }
    std::vector<Expression*> argument_list_copy;
    for(int i = 0; i < argument_list.size(); i++){
        argument_list_copy.push_back(argument_list[i]->make_copy());
    }
    return new ConstructorCall(_cip_expr, type->make_copy(), argument_list_copy);
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