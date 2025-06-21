#include "Constructor.h"
#include "Type.h"
#include "Parameter.h"
#include "Statement.h"
#include "ConstructorSignature.h"
#include "utils.h"
#include "Identifier.h"

Constructor::Constructor(Type* _type, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
    type = _type;
    parameters = _parameters;
    body = _body;
}

Constructor* convert(parser::constructor *c) {
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
    return new Constructor(type, parameters, body);
}

ConstructorSignature* Constructor::resolve_constructor_signature() const {
    std::vector<Type*> input_types;
    for(int i = 0; i < parameters.size(); i++){
        input_types.push_back(parameters[i]->type->make_copy());
    }
    return new ConstructorSignature(type->make_copy(), input_types);
}

bool Constructor::equals(const Constructor* other) const {
    return this->resolve_constructor_signature()->equals(other->resolve_constructor_signature());
}

bool Constructor::is_well_formed() {
    ConstructorSignature *cc = resolve_constructor_signature();
    std::cout << "CHECKING CONSTRUCTOR : " << cc->to_string() << std::endl;
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
        if(*(parameters[i]->type) == BaseType("void")) {
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
        v->stack_offset = local_offset;
        local_offset -= 8;
    }
    for(int i = 0; i < parameters.size(); i++){
        Variable* v = add_variable(parameters[i]->type, parameters[i]->id);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << parameters[i]->type->to_string() << " " << parameters[i]->id->name << "\n";
            return false;
        }
        v->stack_offset = local_offset;
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