#pragma once
#include "../parser/parser.h"

struct Type;
struct Identifier;
struct Function;
struct BaseType;
struct Constructor;
struct TemplateMapping;

struct MemberVariable {
    Type *type;
    Identifier *id;
    MemberVariable(Type *_type, Identifier *_id);
    static MemberVariable* convert(parser::member_variable_declaration *mvd);
    MemberVariable* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};

struct StructDefinition {
    Type *type;
    std::vector<MemberVariable*> member_variables;
    std::vector<Function*> functions;
    std::vector<Constructor*> constructors;
    StructDefinition(Type *_type, std::vector<MemberVariable*> _member_variables, std::vector<Function*> _functions, std::vector<Constructor*> _constructors);
    static StructDefinition* convert(parser::struct_definition *s);
    bool is_well_formed(); 
    StructDefinition* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};