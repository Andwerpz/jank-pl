#include "StructDefinition.h"

#include "Type.h"
#include "Identifier.h"
#include "Function.h"
#include "utils.h"
#include "FunctionSignature.h"
#include "Constructor.h"
#include "ConstructorSignature.h"

MemberVariable::MemberVariable(Type *_type, Identifier *_id) {
    type = _type;
    id = _id;
}

MemberVariable* MemberVariable::convert(parser::member_variable_declaration *mvd) {
    Type *type = Type::convert(mvd->t0);
    Identifier *id = Identifier::convert(mvd->t2);
    return new MemberVariable(type, id);
}


StructDefinition::StructDefinition(BaseType *_base_type, std::vector<MemberVariable*> _member_variables, std::vector<Function*> _functions, std::vector<Constructor*> _constructors) {
    base_type = _base_type;
    member_variables = _member_variables;
    functions = _functions;
    constructors = _constructors;
}

StructDefinition* StructDefinition::convert(parser::struct_definition *s) {
    BaseType *type = BaseType::convert(s->t2);
    std::vector<MemberVariable*> member_variables;
    std::vector<Function*> functions;
    std::vector<Constructor*> constructors;
    for(int i = 0; i < s->t6.size(); i++){
        if(s->t6[i]->t0->is_c0) {   //member variable declaration
            member_variables.push_back(MemberVariable::convert(s->t6[i]->t0->t0->t0));
        }
        else if(s->t6[i]->t0->is_c1) {  //function
            Function *f = Function::convert(s->t6[i]->t0->t1->t0);
            f->enclosing_type = type;
            functions.push_back(f);
        }
        else if(s->t6[i]->t0->is_c2) {  //constructor
            constructors.push_back(Constructor::convert(s->t6[i]->t0->t2->t0));
        }
        else assert(false);
    }
    return new StructDefinition(type, member_variables, functions, constructors);
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
    // - are there any duplicate function definitions?
    // - are all of the functions not overloads?
    //add all functions to global list to check later
    for(int i = 0; i < functions.size(); i++){
        Function *f = functions[i];
        if(dynamic_cast<OperatorOverload*>(f) != nullptr) {
            std::cout << "Cannot declare operator overload within struct\n";
            return false;
        }
        if(!add_function(f)) {
            std::cout << "Failed to add struct member function : " << f->resolve_function_signature()->to_string() << "\n";
            return false;
        } 
    }

    // - are there any duplicate constructors?
    //add all constructors to global list to check later
    for(int i = 0; i < constructors.size(); i++) {
        Constructor *c = constructors[i];
        std::cout << "NEXT CONSTRUCTOR : " << c << " " << i << " " << std::endl;
        if(!add_constructor(c)) {
            std::cout << "Failed to add struct constructor : " << c->resolve_constructor_signature()->to_string() << "\n";
            return false;
        }
    }
    std::cout << "DONE ADDING CONSTRUCTORS" << std::endl;

    // - is there a default constructor?
    {
        ConstructorSignature *cid = new ConstructorSignature(base_type->make_copy(), {});
        if(!is_constructor_declared(cid)) {
            std::cout << "Default constructor for " << base_type->to_string() << " not defined\n";
            return false;
        }
    }
    // - is there a copy constructor?
    {
        ConstructorSignature *cid = new ConstructorSignature(base_type->make_copy(), {new ReferenceType(base_type->make_copy())});
        if(!is_constructor_declared(cid)) {
            std::cout << "Copy constructor for " << base_type->to_string() << " not defined\n";
            return false;
        }
    }

    //construct StructLayout
    std::unordered_map<std::string, int> offset_map;
    int size = 0;
    for(int i = 0; i < member_variables.size(); i++){
        MemberVariable *mv = member_variables[i];
        offset_map.insert({mv->id->name, size});
        size += mv->type->calc_size();
    }

    // - make sure we can actually add StructLayout into the controller
    StructLayout *sl = new StructLayout(member_variables, offset_map, size);
    if(!add_struct_layout(base_type, sl)) {
        std::cout << "Failed to add StructLayout for " << base_type->to_string() << "\n";
        return false;
    }

    return true;
}