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

MemberVariable::MemberVariable(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

MemberVariable::MemberVariable(const MemberVariable& other) : ASTNode(other) {
    type = other.type->make_copy();
    id = other.id->make_copy();
}

MemberVariable::MemberVariable(Type *_type, Identifier *_id) : ASTNode() {
    type = _type;
    id = _id;
    assert(type != nullptr);
    assert(id != nullptr);
}

MemberVariable* MemberVariable::convert(parser::member_variable_declaration *mvd) {
    MemberVariable* result = new MemberVariable(mvd);
    result->type = Type::convert(mvd->t0);
    result->id = Identifier::convert(mvd->t2);
    return result;
}

MemberVariable* MemberVariable::make_copy() {
    return new MemberVariable(*this);
}

bool MemberVariable::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x; 
    else if(!type->replace_templated_types(mapping)) return false;
    return true;
}

bool MemberVariable::look_for_templates(){
    return type->look_for_templates();
}



StructDefinition::StructDefinition(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

StructDefinition::StructDefinition(const StructDefinition& other) : ASTNode(other) {
    type = other.type->make_copy();
    for(int i = 0; i < other.member_variables.size(); i++) {
        member_variables.push_back(other.member_variables[i]->make_copy());
    }
    for(int i = 0; i < other.functions.size(); i++) {
        functions.push_back(other.functions[i]->make_copy());
    }
    for(int i = 0; i < other.constructors.size(); i++) {
        constructors.push_back(other.constructors[i]->make_copy());
    }
    for(int i = 0; i < other.destructors.size(); i++) {
        destructors.push_back(other.destructors[i]->make_copy());
    }
}

StructDefinition::StructDefinition(Type *_type, std::vector<MemberVariable*> _member_variables, std::vector<TemplatedFunction*> _functions, std::vector<Constructor*> _constructors, std::vector<Destructor*> _destructors) : ASTNode() {
    type = _type;
    member_variables = _member_variables;
    functions = _functions;
    constructors = _constructors;
    destructors = _destructors;

    assert(_type != nullptr);
    for(int i = 0; i < member_variables.size(); i++) assert(member_variables[i] != nullptr);
    for(int i = 0; i < functions.size(); i++) assert(functions[i] != nullptr);
    for(int i = 0; i < constructors.size(); i++) assert(constructors[i] != nullptr);
    for(int i = 0; i < destructors.size(); i++) assert(destructors[i] != nullptr);
}

StructDefinition* StructDefinition::convert(parser::struct_definition *s) {
    StructDefinition* result = new StructDefinition(s);
    result->type = BaseType::convert(s->t2);
    for(int i = 0; i < s->t6.size(); i++){
        if(s->t6[i]->t0->is_c0) {   //member variable declaration
            result->member_variables.push_back(MemberVariable::convert(s->t6[i]->t0->t0->t0));
        }
        else if(s->t6[i]->t0->is_c1) {  //function
            TemplatedFunction *f = TemplatedFunction::convert(s->t6[i]->t0->t1->t0);
            f->function->enclosing_type = result->type->make_copy();
            result->functions.push_back(f);
        }
        else if(s->t6[i]->t0->is_c2) {  //constructor
            result->constructors.push_back(Constructor::convert(s->t6[i]->t0->t2->t0));
        }
        else if(s->t6[i]->t0->is_c3) {  //destructor
            result->destructors.push_back(Destructor::convert(s->t6[i]->t0->t3->t0));
        }
        else assert(false);
    }
    return result;
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
    return new StructDefinition(*this);
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