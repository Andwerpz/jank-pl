#include "InlineASMAccess.h"
#include "Identifier.h"
#include "utils.h"
#include "Type.h"
#include "StructLayout.h"

// -- CONVERT CONSTRUCTOR --
InlineASMAccess::InlineASMAccess(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

InlineASMVariable::InlineASMVariable(parser::token *tok) : InlineASMAccess(tok) {
    // do nothing
}

InlineASMMemberVariable::InlineASMMemberVariable(parser::token *tok) : InlineASMAccess(tok) {
    // do nothing
}

InlineASMDereferencing::InlineASMDereferencing(parser::token *tok) : InlineASMAccess(tok) {
    // do nothing
}

// -- COPY CONSTRUCTOR --
InlineASMAccess::InlineASMAccess(const InlineASMAccess& other) : ASTNode(other) {
    // do nothing
}

InlineASMVariable::InlineASMVariable(const InlineASMVariable& other) : InlineASMAccess(other) {
    id = other.id->make_copy();
}

InlineASMMemberVariable::InlineASMMemberVariable(const InlineASMMemberVariable& other) : InlineASMAccess(other) {
    id = other.id->make_copy();
    op = other.op;
    member_id = other.member_id->make_copy();
}

InlineASMDereferencing::InlineASMDereferencing(const InlineASMDereferencing& other) : InlineASMAccess(other) {
    id = other.id->make_copy();
}

// -- SYNTHESIS CONSTRUCTOR --
InlineASMAccess::InlineASMAccess() : ASTNode() {
    // do nothing
}

InlineASMVariable::InlineASMVariable(Identifier *_id) : InlineASMAccess() { 
    id = _id;
    assert(id != nullptr);
}

InlineASMMemberVariable::InlineASMMemberVariable(Identifier *_id, std::string _op, Identifier *_member_id) : InlineASMAccess() {
    id = _id;
    op = _op;
    member_id = _member_id;
    assert(id != nullptr);
    assert(op == "." || op == "->");
    assert(member_id != nullptr);
}

InlineASMDereferencing::InlineASMDereferencing(Identifier *_id) : InlineASMAccess() {
    id = _id;
    assert(id != nullptr);
}

// -- CONVERT --
InlineASMAccess* InlineASMAccess::convert(parser::inline_access *ia) {
    if(ia->t2->is_b0) {         // inline member variable
        return InlineASMMemberVariable::convert(ia->t2->t0->t0);
    }
    else if(ia->t2->is_b1) {    // inline dereferencing
        return InlineASMDereferencing::convert(ia->t2->t1->t0);
    }
    else if(ia->t2->is_b2) {    // inline variable
        return InlineASMVariable::convert(ia->t2->t2->t0);
    }
    else assert(false);
}

InlineASMVariable* InlineASMVariable::convert(parser::inline_variable *ia) {
    InlineASMVariable* result = new InlineASMVariable(ia);
    result->id = Identifier::convert(ia->t0);
    return result;
}

InlineASMMemberVariable* InlineASMMemberVariable::convert(parser::inline_member_variable *ia) {
    InlineASMMemberVariable* result = new InlineASMMemberVariable(ia);
    result->id = Identifier::convert(ia->t0);
    result->op = ia->t1->to_string();
    result->member_id = Identifier::convert(ia->t2);
    return result;
}

InlineASMDereferencing* InlineASMDereferencing::convert(parser::inline_dereferencing *ia) {
    InlineASMDereferencing* result = new InlineASMDereferencing(ia);
    result->id = Identifier::convert(ia->t2);
    return result;
}

// -- IS WELL FORMED --
bool InlineASMVariable::is_well_formed() {
    // - does the struct variable exist?
    Variable *v = get_variable(id);
    if(v == nullptr) {
        std::cout << "Inline ASM variable does not exist : " << id->name << "\n";
        return false;
    }
    Type *vt = v->type->make_copy();

    // - strip reference
    if(dynamic_cast<ReferenceType*>(vt)) {
        vt = vt->remove_reference();
    }

    // - can only access primitive variables (whatever can fit in a register)
    if(!is_type_primitive(vt)) {
        std::cout << "Cannot access non-primitive type via inline asm : " << vt->to_string() << "\n";
        return false;
    }
    return true;
}

bool InlineASMMemberVariable::is_well_formed() {
    // - does the struct variable exist?
    Variable *v = get_variable(id);
    if(v == nullptr) {
        std::cout << "Inline ASM variable does not exist : " << id->name << "\n";
        return false;
    }
    Type *vt = v->type->make_copy();

    // - strip reference
    if(dynamic_cast<ReferenceType*>(vt)) {
        vt = vt->remove_reference();
    }
    
    if(op == ".") {
        //do nothing
    }
    else if(op == "->") {
        // - if we're dereferencing, this type must be pointer
        PointerType *pt = dynamic_cast<PointerType*>(vt);
        if(pt == nullptr) {
            std::cout << "Cannot dereference non-pointer type : " << vt->to_string() << "\n";
            return false;
        }
        vt = pt->type;
    }
    else assert(false);

    // - does the member variable exist?
    StructLayout *sl = get_struct_layout(vt);
    if(sl == nullptr) {
        std::cout << "Unable to find StructLayout for : " << vt->to_string() << "\n";
        return false;
    }
    Type *mvt = sl->get_type(member_id);
    if(mvt == nullptr) {
        std::cout << "Unknown member variable identifier \"" << member_id->name << "\" for type " << vt->to_string() << "\n";
        return false;
    }

    // - can only access primitive member variables (whatever can fit in a register)
    if(!is_type_primitive(mvt)) {
        std::cout << "Cannot access non-primitive type via inline asm : " << mvt->to_string() << "\n";
        return false;
    }

    return true;
}

bool InlineASMDereferencing::is_well_formed() {
    // - does the variable exist?
    Variable *v = get_variable(id);
    if(v == nullptr) {
        std::cout << "Inline ASM variable does not exist : " << id->name << "\n";
        return false;
    }
    Type *vt = v->type->make_copy();

    // - strip reference
    if(dynamic_cast<ReferenceType*>(vt)) {
        vt = vt->remove_reference();
    }

    // - this type must be pointer
    PointerType *pt = dynamic_cast<PointerType*>(vt);
    if(pt == nullptr) {
        std::cout << "Cannot dereference non-pointer type : " << vt->to_string() << "\n";
        return false;
    }
    vt = pt;

    // - can only access primitive variables (whatever can fit in a register)
    if(!is_type_primitive(vt)) {
        std::cout << "Cannot access non-primitive type via inline asm : " << vt->to_string() << "\n";
        return false;
    }

    return true;
}

// -- EMIT ASM --
// use %r15 as a temp register. In the end, %r15 should hold addr
// perhaps should have some way to specify which register to use
// maybe something like 'asm!("movq %0x100, {var:%r14}");'
// it will be optional, if it's not specified will use %r15 automatically
void InlineASMVariable::emit_asm() {
    Variable *v = get_variable(id);
    assert(v != nullptr);
    Type *vt = v->type->make_copy();

    //load variable addr into %r15
    fout << indent() << "lea " << v->addr << ", %r15\n";

    //strip reference
    if(dynamic_cast<ReferenceType*>(vt)) {
        vt = vt->remove_reference();
        fout << indent() << "movq (%r15), %r15\n";
    }
}

void InlineASMMemberVariable::emit_asm() {
    Variable *v = get_variable(id);
    assert(v != nullptr);
    Type *vt = v->type->make_copy();

    //%r15 now has pointer to struct
    fout << indent() << "movq " << v->addr << ", %r15\n";

    //strip reference
    if(dynamic_cast<ReferenceType*>(vt)) {
        vt = vt->remove_reference();
        //no need to emit anything
    }
    
    if(op == ".") {
        //do nothing
    }
    else if(op == "->") {
        PointerType *pt = dynamic_cast<PointerType*>(vt);
        assert(pt != nullptr);
        vt = pt->type;
    }
    else assert(false);

    //get member variable offset
    StructLayout *sl = get_struct_layout(vt);
    assert(sl != nullptr);
    Type *mvt = sl->get_type(member_id);
    assert(mvt != nullptr);
    assert(is_type_primitive(mvt));
    int offset = sl->get_offset(member_id);

    //%r15 now has pointer to member variable
    fout << indent() << "lea " << offset << "(%r15), %r15\n";
}

void InlineASMDereferencing::emit_asm() {
    Variable *v = get_variable(id);
    assert(v != nullptr);
    Type *vt = v->type->make_copy();

    //load variable addr into %r15
    fout << indent() << "lea " << v->addr << ", %r15\n";

    //strip reference
    if(dynamic_cast<ReferenceType*>(vt)) {
        vt = vt->remove_reference();
        fout << indent() << "movq (%r15), %r15\n";
    }

    //%r15 now holds the pointer
    fout << indent() << "movq (%r15), %r15\n";
}

// -- GET ADDR --
std::string InlineASMVariable::get_addr() {
    return "(%r15)";
}

std::string InlineASMMemberVariable::get_addr() {
    return "(%r15)";
}

std::string InlineASMDereferencing::get_addr() {
    return "(%r15)";
}

// -- MAKE COPY --
InlineASMAccess* InlineASMVariable::make_copy() {
    return new InlineASMVariable(*this);
}

InlineASMAccess* InlineASMMemberVariable::make_copy() {
    return new InlineASMMemberVariable(*this);
}

InlineASMAccess* InlineASMDereferencing::make_copy() {
    return new InlineASMDereferencing(*this);
}

// -- TO STRING --
std::string InlineASMVariable::to_string() {
    return "{" + id->name + "}";
}

std::string InlineASMMemberVariable::to_string() {
    return "{" + id->name + op + member_id->name + "}";
}

std::string InlineASMDereferencing::to_string() {
    return "{*" + id->name + "}";
}