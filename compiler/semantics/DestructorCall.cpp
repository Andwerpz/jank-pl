#include "DestructorCall.h"
#include "utils.h"
#include "primitives.h"
#include "Type.h"
#include "Destructor.h"
#include "Expression.h"
#include "TemplateMapping.h"
#include "StructLayout.h"
#include "StructDefinition.h"
#include "Identifier.h"

DestructorCall::DestructorCall(Type *_type) {
    type = _type;
}

Destructor* DestructorCall::resolve_called_destructor() {
    return get_called_destructor(this);
}

Type* DestructorCall::resolve_type() {
    Destructor *d = this->resolve_called_destructor();
    if(d == nullptr) {
        std::cout << "Cannot resolve destructor call : " << to_string() << "\n";
        return nullptr;
    }
    return primitives::_void->make_copy();
}

//need to call destructor, then dealloc the struct itself. 
//this is what should happen:
// - top level destructor runs
// - member variables are destructed in reverse order
// - top level releases memory (this is only top level, should add flag to say to not release memory for member variables)
void DestructorCall::emit_asm(bool should_dealloc) {
    assert(is_type_declared(type));
    assert(!is_type_primitive(type));

    if(asm_debug) fout << indent() << "# calling destructor : " << type->to_string() << "\n";

    //find destructor
    Destructor *d = this->resolve_called_destructor();
    assert(d != nullptr);

    //pass in struct as argument
    emit_push("%rax", "DestructorCall::emit_asm() : target struct");

    //call destructor
    std::string label = get_destructor_label(type);
    fout << indent() << "call " << label << "\n";

    //clean up target struct argument
    emit_pop("%rax", "DestructorCall::emit_asm() : target struct");

    //call member variable destructors
    StructLayout *sl = get_struct_layout(type);
    assert(sl != nullptr);
    if(auto atype = dynamic_cast<ArrayType*>(type)) {
        Type *bt = atype->type;
        int bt_sz = bt->calc_size();
        assert(bt != nullptr);
        if(!is_type_primitive(bt)) {
            for(int i = atype->amt - 1; i >= 0; i--){
                //save base struct address
                emit_push("%rax", "DestructorCall::emit_asm() : target struct");

                //move member variable address into %rax
                fout << indent() << "add $" << i * bt_sz << ", %rax\n";

                //call destructor, no dealloc
                emit_destructor_call(bt, false);

                //retrieve base struct address
                emit_pop("%rax", "DestructorCall::emit_asm() : target struct");
            }
        }
    }
    else {
        for(int i = (int) sl->member_variables.size() - 1; i >= 0; i--){
            Type *mvt = sl->member_variables[i]->type;
            Identifier *mvid = sl->member_variables[i]->id;
            int offset = sl->get_offset(mvid);
            if(!is_type_primitive(mvt)) {
                //save base struct address
                emit_push("%rax", "DestructorCall::emit_asm() : target struct");

                //move member variable address into %rax
                fout << indent() << "add $" << offset << ", %rax\n";

                //call destructor, no dealloc
                emit_destructor_call(mvt, false);

                //retrieve base struct address
                emit_pop("%rax", "DestructorCall::emit_asm() : target struct");
            }
        }
    }

    //clean up struct memory. %rax should hold the struct address
    if(should_dealloc) {
        int sz = sl->get_size();
        emit_free(sz);
    }
}

std::string DestructorCall::to_string() {
    return "~" + type->to_string() + "()";
}

size_t DestructorCall::hash() {
    return type->hash();
}

bool DestructorCall::equals(DestructorCall *other){
    if(!type->equals(other->type)) return false;
    return true;
}

DestructorCall* DestructorCall::make_copy(){
    return new DestructorCall(type->make_copy());
}

bool DestructorCall::replace_templated_types(TemplateMapping *mapping){
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    return true;
}

bool DestructorCall::look_for_templates(){
    if(!type->look_for_templates()) return false;
    return true;
}
