#pragma once
#include "../parser/parser.h"

struct Type;
struct Identifier;
struct Function;
struct BaseType;
struct Constructor;

struct MemberVariable {
    Type *type;
    Identifier *id;
    MemberVariable(Type *_type, Identifier *_id);
    static MemberVariable* convert(parser::member_variable_declaration *mvd);
};

struct StructDefinition {
    BaseType *base_type;
    std::vector<MemberVariable*> member_variables;
    std::vector<Function*> functions;
    std::vector<Constructor*> constructors;
    StructDefinition(BaseType *_base_type, std::vector<MemberVariable*> _member_variables, std::vector<Function*> _functions, std::vector<Constructor*> _constructors);
    static StructDefinition* convert(parser::struct_definition *s);
    bool is_well_formed(); 
};