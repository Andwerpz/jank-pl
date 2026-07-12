#include "Destructor.h"
#include "Statement.h"
#include "TemplateMapping.h"
#include "Type.h"
#include "utils.h"
#include "Identifier.h"
#include "CompilationContext.h"
#include "DefinitionSpace.h"

Destructor::Destructor(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Destructor::Destructor(const Destructor& other) : ASTNode(other) {
    type = other.type->make_copy();
    body = dynamic_cast<CompoundStatement*>(other.body->make_copy());
    is_generated = other.is_generated;
}

Destructor::Destructor(Type *_type, CompoundStatement *_body) : ASTNode() {
    type = _type;
    body = _body;
    assert(type != nullptr);
    assert(type != nullptr);
}

Destructor* Destructor::convert(parser::destructor *d) {
    Destructor* result = new Destructor(d);
    result->type = BaseType::convert(d->t1);
    result->body = CompoundStatement::convert(d->t5);
    return result;
}

bool Destructor::equals(Destructor *other) const {
    if(!type->equals(other->type)) return false;
    return true;
}

bool Destructor::is_well_formed(CompilationContext *ctx) {
    // - is type of destructor declared?
    if(!ctx->is_type_declared(type)) {
        std::cout << "Destructor undeclared type : " << type->to_string() << "\n";
        return false;
    }

    push_declaration_stack();

    //print destructor header
    if(asm_debug) fout << "# " << type->to_string() << "\n";
    std::string label = get_destructor_label(type);
    assert(label.size() >= 2 && label[0] == '\"' && label[label.size() - 1] == '\"');
    std::string label_noquotes = label.substr(1, label.size() - 2);
    if(is_generated) {
        fout << ".section \".text." << label_noquotes << "\",\"axG\",@progbits," << label << ",comdat\n";
        fout << ".weak " << label << "\n";
    }
    else {
        fout << ".section \".text." << label_noquotes << "\",\"ax\",@progbits\n";
        fout << ".globl " << label << "\n";
    }
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
    if(!body->is_well_formed(ctx)) {
        std::cout << "Constructor body not well formed\n";
        return false;
    }

    //add trailing return
    ReturnStatement *rs = new ReturnStatement(std::nullopt);
    if(!rs->is_well_formed(ctx)) {
        std::cout << "Trailing return failed??";
        assert(0);  
    }

    fout << "\n";

    //unregister this as variable
    pop_declaration_stack(ctx);

    //local stack should be empty before returning
    assert(stack_desc.size() == 0);

    return true;
}

Destructor* Destructor::make_copy() {
    return new Destructor(*this);
}

bool Destructor::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    if(!body->replace_templated_types(mapping)) return false;
    return true;
}

bool Destructor::look_for_templates(CompilationContext *ctx) {
    if(!type->look_for_templates(ctx)) return false;
    if(!body->look_for_templates(ctx)) return false;
    return true;
}
