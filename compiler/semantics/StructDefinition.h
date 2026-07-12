#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct Type;
struct Identifier;
struct Function;
struct BaseType;
struct Constructor;
struct TemplateMapping;
struct Destructor;
struct TemplatedFunction;
struct CompilationContext;

struct MemberVariable : public ASTNode {
    Type *type;
    Identifier *id;

    MemberVariable(parser::token *tok);
    MemberVariable(const MemberVariable& other);
    MemberVariable(Type *_type, Identifier *_id);

    static MemberVariable* convert(parser::member_variable_declaration *mvd);
    MemberVariable* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates(CompilationContext* ctx);
};

struct StructDefinition : public ASTNode {
    Type *type;
    std::vector<MemberVariable*> member_variables;
    std::vector<TemplatedFunction*> functions;
    std::vector<Constructor*> constructors;
    std::vector<Destructor*> destructors;

    StructDefinition(parser::token *tok);
    StructDefinition(const StructDefinition& other);
    StructDefinition(Type *_type, std::vector<MemberVariable*> _member_variables, std::vector<TemplatedFunction*> _functions, std::vector<Constructor*> _constructors, std::vector<Destructor*> _destructors);
    
    static StructDefinition* convert(parser::struct_definition *s);
    bool is_well_formed(CompilationContext* ctx); 
    StructDefinition* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates(CompilationContext* ctx);
};