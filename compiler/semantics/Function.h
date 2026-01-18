#pragma once
#include "../parser/parser.h"
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

struct Function {    
    std::optional<Type*> enclosing_type;    //type of containing struct
    bool is_export;         //if true, will generate label equal to function id
    Type *type;             //return type
    Identifier *id;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;

    Function(std::optional<Type*> _enclosing_type, bool _is_export, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body);

    //use when inputting pre-defined asm functions (sys functions)
    Function(Type *_type, Identifier *_id, std::vector<Type*> input_types);

    virtual ~Function() = default;

    FunctionSignature* resolve_function_signature() const;
    bool equals(const Function* other) const;
    bool operator==(const Function& other) const;
    bool operator!=(const Function& other) const;

    static Function* convert(parser::function *f);
    bool is_well_formed();
    virtual Function* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
    std::string to_string();

    bool is_main();
    bool is_valid_call(FunctionCall *fc);       //returns true if the given function call can be used to call the function
};