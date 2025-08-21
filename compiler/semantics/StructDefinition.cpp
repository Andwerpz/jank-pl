#include "StructDefinition.h"

#include "Type.h"
#include "Identifier.h"
#include "Function.h"
#include "utils.h"
#include "FunctionSignature.h"
#include "Constructor.h"
#include "ConstructorSignature.h"
#include "TemplateMapping.h"
#include "Destructor.h"
#include "TemplatedFunction.h"

MemberVariable::MemberVariable(Type *_type, Identifier *_id) {
    type = _type;
    id = _id;
}

MemberVariable* MemberVariable::convert(parser::member_variable_declaration *mvd) {
    Type *type = Type::convert(mvd->t0);
    Identifier *id = Identifier::convert(mvd->t2);
    return new MemberVariable(type, id);
}

MemberVariable* MemberVariable::make_copy() {
    return new MemberVariable(type->make_copy(), id->make_copy());
}

bool MemberVariable::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x; 
    else if(!type->replace_templated_types(mapping)) return false;
    return true;
}

bool MemberVariable::look_for_templates(){
    return type->look_for_templates();
}


StructDefinition::StructDefinition(Type *_type, std::vector<MemberVariable*> _member_variables, std::vector<TemplatedFunction*> _functions, std::vector<Constructor*> _constructors, std::vector<Destructor*> _destructors) {
    type = _type;
    member_variables = _member_variables;
    functions = _functions;
    constructors = _constructors;
    destructors = _destructors;

    assert(_type != nullptr);
}

StructDefinition* StructDefinition::convert(parser::struct_definition *s) {
    BaseType *type = BaseType::convert(s->t2);
    std::vector<MemberVariable*> member_variables;
    std::vector<TemplatedFunction*> functions;
    std::vector<Constructor*> constructors;
    std::vector<Destructor*> destructors;
    for(int i = 0; i < s->t6.size(); i++){
        if(s->t6[i]->t0->is_c0) {   //member variable declaration
            member_variables.push_back(MemberVariable::convert(s->t6[i]->t0->t0->t0));
        }
        else if(s->t6[i]->t0->is_c1) {  //function
            TemplatedFunction *f = TemplatedFunction::convert(s->t6[i]->t0->t1->t0);
            f->function->enclosing_type = type->make_copy();
            functions.push_back(f);
        }
        else if(s->t6[i]->t0->is_c2) {  //constructor
            constructors.push_back(Constructor::convert(s->t6[i]->t0->t2->t0));
        }
        else if(s->t6[i]->t0->is_c3) {  //destructor
            destructors.push_back(Destructor::convert(s->t6[i]->t0->t3->t0));
        }
        else assert(false);
    }
    return new StructDefinition(type, member_variables, functions, constructors, destructors);
}

bool StructDefinition::is_well_formed() {
    // - do all member variable types exist?
    for(int i = 0; i < member_variables.size(); i++){
        Type *vt = member_variables[i]->type;
        if(!is_type_declared(vt)) {
            std::cout << "Member variable type does not exist : " << vt->to_string() << "\n";
            return false;
        }
    }
    // - are there any duplicate member variable identifiers?
    for(int i = 0; i < member_variables.size(); i++){
        Identifier *id = member_variables[i]->id;
        for(int j = i + 1; j < member_variables.size(); j++){
            if(*id == *(member_variables[j]->id)) {
                std::cout << "Duplicate member variable identifier : " << id->name << "\n";
                return false;
            }
        }
    }
    // - are all member variables not references?
    for(int i = 0; i < member_variables.size(); i++){
        Type *vt = member_variables[i]->type;
        if(dynamic_cast<ReferenceType*>(vt)) {
            std::cout << "Struct member variable cannot be reference type : " << vt->to_string() << "\n";
            return false;
        }
    }

    // - are all the constructors actually constructing this type?
    for(int i = 0; i < constructors.size(); i++){
        if(!type->equals(constructors[i]->type)) {
            std::cout << "Constructor in " << type->to_string() << " is of wrong type : " << constructors[i]->type->to_string() << "\n";
            return false;
        }
    }
    
    return true;
}

StructDefinition* StructDefinition::make_copy() {
    Type *_type = type->make_copy();
    std::vector<MemberVariable*> _member_variables;
    std::vector<TemplatedFunction*> _functions;
    std::vector<Constructor*> _constructors;
    std::vector<Destructor*> _destructors;
    for(int i = 0; i < member_variables.size(); i++){
        _member_variables.push_back(member_variables[i]->make_copy());
    }
    for(int i = 0; i < functions.size(); i++){
        _functions.push_back(functions[i]->make_copy());
    }
    for(int i = 0; i < constructors.size(); i++){
        _constructors.push_back(constructors[i]->make_copy());
    }
    for(int i = 0; i < destructors.size(); i++){
        _destructors.push_back(destructors[i]->make_copy());
    }
    return new StructDefinition(_type, _member_variables, _functions, _constructors, _destructors);
}

bool StructDefinition::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    for(int i = 0; i < member_variables.size(); i++){
        if(!member_variables[i]->replace_templated_types(mapping)) return false;
    }
    for(int i = 0; i < functions.size(); i++){
        if(!functions[i]->replace_templated_types(mapping)) return false;
    }
    for(int i = 0; i < constructors.size(); i++){
        if(!constructors[i]->replace_templated_types(mapping)) return false;
    }
    for(int i = 0; i < destructors.size(); i++){
        if(!destructors[i]->replace_templated_types(mapping)) return false;
    }
    return true;
}

bool StructDefinition::look_for_templates() {
    if(!type->look_for_templates()) return false;
    for(int i = 0; i < member_variables.size(); i++) if(!member_variables[i]->look_for_templates()) return false;
    for(int i = 0; i < constructors.size(); i++) if(!constructors[i]->look_for_templates()) return false;
    for(int i = 0; i < destructors.size(); i++) if(!destructors[i]->look_for_templates()) return false;
    return true;
}