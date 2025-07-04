#include "Constructor.h"
#include "Type.h"
#include "Parameter.h"
#include "Statement.h"
#include "ConstructorSignature.h"
#include "utils.h"
#include "Identifier.h"
#include "TemplateMapping.h"
#include "Expression.h"
#include "primitives.h"

Constructor* Constructor::convert(parser::constructor *c) {
    Type *type = BaseType::convert(c->t0->t0);
    parser::parameter_list *pl = c->t0->t4;
    std::vector<Parameter*> parameters;
    if(pl->t0 != nullptr) {
        parameters.push_back(Parameter::convert(pl->t0->t0));
        std::vector<parser::parameter_list::a0::b0*> tmp = pl->t0->t1;
        for(int i = 0; i < tmp.size(); i++){
            parameters.push_back(Parameter::convert(tmp[i]->t3));
        }
    }   
    CompoundStatement *body = CompoundStatement::convert(c->t2);
    return new StructConstructor(type, parameters, body);
}

// -- CONSTRUCTOR --
StructConstructor::StructConstructor(Type* _type, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
    type = _type;
    parameters = _parameters;
    body = _body;
}

PrimitiveConstructor::PrimitiveConstructor(Type *_type, bool _is_copy_constructor) {
    type = _type;
    is_copy_constructor = _is_copy_constructor;
    assert(type != nullptr);
    assert(is_type_primitive(type));

    if(is_copy_constructor) parameters.push_back(new Parameter(type->make_copy(), new Identifier("x")));
}

// -- RESOLVE CONSTRUCTOR SIGNATURE --
ConstructorSignature* Constructor::resolve_constructor_signature() const {
    std::vector<Type*> input_types;
    for(int i = 0; i < parameters.size(); i++){
        input_types.push_back(parameters[i]->type->make_copy());
    }
    return new ConstructorSignature(type->make_copy(), input_types);
}

// -- EQUALS --
bool StructConstructor::equals(Constructor* _other) const {
    if(!dynamic_cast<StructConstructor*>(_other)) return false;
    StructConstructor *other = dynamic_cast<StructConstructor*>(_other);
    return this->resolve_constructor_signature()->equals(other->resolve_constructor_signature());
}

bool PrimitiveConstructor::equals(Constructor *_other) const {
    if(!dynamic_cast<PrimitiveConstructor*>(_other)) return false;
    PrimitiveConstructor *other = dynamic_cast<PrimitiveConstructor*>(_other);
    if(!type->equals(other->type)) return false;
    if(is_copy_constructor != other->is_copy_constructor) return false;
    return true;
}

// -- IS WELL FORMED --
bool StructConstructor::is_well_formed() {
    ConstructorSignature *cc = resolve_constructor_signature();
    std::cout << "CHECKING CONSTRUCTOR : " << cc->to_string() << std::endl;

    // - can all the templates be resolved?
    if(!look_for_templates()) {
        std::cout << "Cannot resolve all templates in constructor : " << resolve_constructor_signature()->to_string();
        return false;
    }

    push_declaration_stack();

    if(asm_debug) {
        fout << "# " << cc->to_string() << "\n";
    }
    std::string label = get_constructor_label(cc);
    fout << label << ":\n";

    //setup stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";

    for(int i = 0; i < parameters.size(); i++){
        // - does parameter correspond to existing type?
        if(!is_type_declared(parameters[i]->type)) {
            std::cout << "Undeclared type : " << parameters[i]->type->to_string() << "\n";
            return false;
        }
        // - is parameter type not void?
        if(parameters[i]->type->equals(primitives::_void)) {
            std::cout << "Parameter can't have type void\n";
            return false;
        }
    }

    // - is return type of function existing?
    if(!is_type_declared(type)) {
        std::cout << "Constructor undeclared return type : " << type->to_string() << "\n";
        return false;
    }

    //register self as variable (Type* this)
    local_offset = 8 + 8 * parameters.size();
    {
        //adjust local offset for 'extra variable'
        local_offset += 8;

        //register self as variable (Type this)
        Type *vt = this->type->make_copy();
        Identifier *vid = new Identifier("this");
        Variable* v = add_variable(vt, vid);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << vt << " " << vid << "\n";
            return false;
        }
        v->addr = std::to_string(local_offset) + "(%rbp)";
        local_offset -= 8;
    }
    for(int i = 0; i < parameters.size(); i++){
        Variable* v = add_variable(parameters[i]->type, parameters[i]->id);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << parameters[i]->type->to_string() << " " << parameters[i]->id->name << "\n";
            return false;
        }
        v->addr = std::to_string(local_offset) + "(%rbp)";
        local_offset -= 8;
    }

    //set local offset equal to %rsp
    local_offset = 0;

    //check that the local stack is empty so far
    assert(stack_desc.size() == 0);

    // - make sure body is well formed
    if(!body->is_well_formed()) {
        std::cout << "Constructor body not well formed\n";
        return false;
    }

    //add trailing return
    fout << indent() << "pop %rbp\n";   //should not be managed by local_offset
    fout << indent() << "ret\n";

    fout << "\n";

    //unregister parameters as variables
    pop_declaration_stack();

    //local stack should be empty before returning
    assert(stack_desc.size() == 0);

    return true;
}

bool PrimitiveConstructor::is_well_formed() {
    assert(is_type_primitive(type));
    ConstructorSignature *cs = this->resolve_constructor_signature();

    //constructor label
    if(asm_debug) {
        fout << "# " << cs->to_string() << "\n";
    }
    std::string label = get_constructor_label(cs);
    fout << label << ":\n";

    //setup stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";

    if(is_copy_constructor) {
        //really jank solution (holy moly dylan janky)
        //just do 'this = x'
        //will have to rethink this if we prohibit assigning to 'this'
        //may have to do this a bit more low level

        assert(parameters.size() == 1);
        push_declaration_stack();

        local_offset = 8 + 8 * parameters.size() + 8;
        //register 'this' as variable
        Identifier *thisid = new Identifier("this");
        Variable *vthis = add_variable(type, thisid);
        assert(vthis != nullptr);
        vthis->addr = std::to_string(local_offset) + "(%rbp)";
        local_offset -= 8;

        //register only input as variable
        Identifier *xid = parameters[0]->id;
        Variable *vx = add_variable(parameters[0]->type, xid);
        vx->addr = std::to_string(local_offset) + "(%rbp)";
        local_offset -= 8;

        //set local offset equal to %rsp
        local_offset = 0;

        //check that the local stack is empty so far
        assert(stack_desc.size() == 0);

        Expression *a_expr = new Expression(new ExprBinary(new ExprPrimary(thisid), "=", new ExprPrimary(xid)));
        assert(a_expr->resolve_type()->equals(type));
        a_expr->emit_asm();
        
        pop_declaration_stack();
    }

    //add trailing return
    fout << indent() << "pop %rbp\n";   //should not be managed by local_offset
    fout << indent() << "ret\n";

    fout << "\n";
    
    return true;
}

// -- MAKE COPY --
Constructor* StructConstructor::make_copy() {
    Type *_type = type->make_copy();
    std::vector<Parameter*> _parameters;
    for(int i = 0; i < parameters.size(); i++){
        _parameters.push_back(parameters[i]->make_copy());
    }
    CompoundStatement *_body = dynamic_cast<CompoundStatement*>(body->make_copy());
    return new StructConstructor(_type, _parameters, _body);
}

Constructor* PrimitiveConstructor::make_copy() {
    Type *_type = type->make_copy();
    return new PrimitiveConstructor(type->make_copy(), is_copy_constructor);
}

// -- REPLACE TEMPLATED TYPES --
bool StructConstructor::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    for(int i = 0; i < parameters.size(); i++){
        if(!parameters[i]->replace_templated_types(mapping)) return false;
    }
    if(!body->replace_templated_types(mapping)) return false;
    return true;
}

bool PrimitiveConstructor::replace_templated_types(TemplateMapping *mapping) {
    assert(!mapping->find_mapped_type(type));
    assert(type->replace_templated_types(mapping));
    return true;
}

// -- LOOK FOR TEMPLATES --
bool StructConstructor::look_for_templates() {
    if(!type->look_for_templates()) return false;
    for(int i = 0; i < parameters.size(); i++){
        if(!parameters[i]->look_for_templates()) return false;
    }
    if(!body->look_for_templates()) return false;
    return true;
}

bool PrimitiveConstructor::look_for_templates() {
    assert(type->look_for_templates());
    return true;
}