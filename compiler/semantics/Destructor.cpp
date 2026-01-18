#include "Destructor.h"
#include "Statement.h"
#include "TemplateMapping.h"
#include "Type.h"
#include "utils.h"
#include "Identifier.h"

Destructor::Destructor(Type *_type, CompoundStatement *_body) {
    type = _type;
    body = _body;
    assert(type != nullptr);
    assert(type != nullptr);
}

Destructor* Destructor::convert(parser::destructor *d) {
    Type *type = BaseType::convert(d->t1);
    CompoundStatement *body = CompoundStatement::convert(d->t5);
    return new Destructor(type, body);
}

bool Destructor::equals(Destructor *other) const {
    if(!type->equals(other->type)) return false;
    return true;
}

bool Destructor::is_well_formed() {
    std::cout << "CHECKING DESTRUCTOR : " << type->to_string() << std::endl;

    // - is type of destructor declared?
    if(!is_type_declared(type)) {
        std::cout << "Destructor undeclared type : " << type->to_string() << "\n";
        return false;
    }

    // - can all the templates be resolved?
    if(!look_for_templates()) {
        std::cout << "Cannot resolve all templates in destructor : " << type->to_string() << "\n";
        return false;
    }

    push_declaration_stack();

    if(asm_debug) {
        fout << "# ~" << type->to_string() << "\n";
    }
    std::string label = get_destructor_label(type);
    fout << label << ":\n";

    //setup stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";

    //register self as variable (Type& this)
    local_offset = 16;
    {
        //register self as variable (Type& this)
        Type *vt = new ReferenceType(this->type->make_copy());
        Identifier *vid = new Identifier("this");
        Variable* v = add_stack_variable(vt, vid);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << vt << " " << vid << "\n";
            return false;
        }
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
    ReturnStatement *rs = new ReturnStatement(std::nullopt);
    if(!rs->is_well_formed()) {
        std::cout << "Trailing return failed??";
        assert(0);  
    }

    fout << "\n";

    //unregister this as variable
    pop_declaration_stack();

    //local stack should be empty before returning
    assert(stack_desc.size() == 0);

    return true;
}

Destructor* Destructor::make_copy() {
    return new Destructor(type->make_copy(), dynamic_cast<CompoundStatement*>(body->make_copy()));
}

bool Destructor::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    if(!body->replace_templated_types(mapping)) return false;
    return true;
}

bool Destructor::look_for_templates() {
    if(!type->look_for_templates()) return false;
    if(!body->look_for_templates()) return false;
    return true;
}
