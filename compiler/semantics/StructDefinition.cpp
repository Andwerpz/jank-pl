#include "StructDefinition.h"

#include "Type.h"
#include "Identifier.h"
#include "Function.h"
#include "utils.h"
#include "FunctionSignature.h"


MemberVariable::MemberVariable(Type *_type, Identifier *_id) {
    type = _type;
    id = _id;
}

MemberVariable* MemberVariable::convert(parser::member_variable_declaration *mvd) {
    Type *type = Type::convert(mvd->t0);
    Identifier *id = Identifier::convert(mvd->t2);
    return new MemberVariable(type, id);
}


StructDefinition::StructDefinition(Type *_type, std::vector<MemberVariable*> _member_variables, std::vector<Function*> _functions) {
    type = _type;
    member_variables = _member_variables;
    functions = _functions;
}

StructDefinition* StructDefinition::convert(parser::struct_definition *s) {
    Type *type = BaseType::convert(s->t2);
    std::vector<MemberVariable*> member_variables;
    std::vector<Function*> functions;
    for(int i = 0; i < s->t6.size(); i++){
        if(s->t6[i]->t0->is_c0) {   //member variable declaration
            member_variables.push_back(MemberVariable::convert(s->t6[i]->t0->t0->t0));
        }
        else if(s->t6[i]->t0->is_c1) {  //function
            Function *f = Function::convert(s->t6[i]->t0->t1->t0);
            f->enclosing_type = type;
            functions.push_back(f);
        }
        else assert(false);
    }
    return new StructDefinition(type, member_variables, functions);
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
    // - is there a default constructor?
    {
        FunctionSignature *fid = new FunctionSignature(new Identifier(type->to_string()), {});
        if(!is_function_declared(fid)) {
            std::cout << "Default constructor for " << type->to_string() << " not defined\n";
            return false;
        }
    }
    // - is there a copy constructor?
    {
        FunctionSignature *fid = new FunctionSignature(new Identifier(type->to_string()), {new ReferenceType(type)});
        if(!is_function_declared(fid)) {
            std::cout << "Copy constructor for " << type->to_string() << " not defined\n";
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
    if(!add_struct_layout(type, sl)) {
        std::cout << "Failed to add StructLayout for " << type->to_string() << "\n";
        return false;
    }

    return true;
}