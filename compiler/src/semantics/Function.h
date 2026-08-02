#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"
#include <optional>
#include <vector>

struct CompoundStatement;
struct Identifier;
struct Type;
struct FunctionSignature;
struct OperatorSignature;
struct Parameter;
struct TemplateMapping;
struct FunctionCall;
struct CompilationContext;

struct Function : public ASTNode {    
    std::optional<Type*> enclosing_type;        // type of containing struct
    std::optional<std::string> intrinsic_name;  // if this has a value, this function is defining an intrinsic
    bool is_export;         // if true, will generate label equal to function id
    Type *type;             // return type
    Identifier *id;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;

    //should be set to true externally when generated. 
    bool is_generated = false;

    Function(parser::token *tok);
    Function(const Function& other);
    Function(std::optional<Type*> _enclosing_type, bool _is_export, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body);
    Function(Type *_type, Identifier *_id, std::vector<Type*> input_types);

    static Function* convert(parser::function *f);
    FunctionSignature* resolve_function_signature() const;
    bool equals(const Function* other) const;
    bool operator==(const Function& other) const;
    bool operator!=(const Function& other) const;

    bool is_well_formed(CompilationContext* ctx);
    virtual Function* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates(CompilationContext* ctx);
    std::string to_string();

    bool is_main();
    bool is_valid_call(CompilationContext *ctx, FunctionCall *fc);       //returns true if the given function call can be used to call the function
};