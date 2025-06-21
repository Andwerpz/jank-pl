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

struct Function {    
    std::optional<Type*> enclosing_type;
    Type *type;
    Identifier *id;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;

    Function(std::optional<Type*> _enclosing_type, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body);

    //use when inputting pre-defined asm functions
    Function(Type *_type, Identifier *_id, std::vector<Type*> input_types);

    virtual ~Function() = default;

    FunctionSignature* resolve_function_signature() const;
    bool equals(const Function* other) const;
    bool operator==(const Function& other) const;
    bool operator!=(const Function& other) const;

    static Function* convert(parser::function *f);
    bool is_well_formed();
};

struct OperatorOverload : public Function {
    std::string op;
    OperatorOverload(std::string _op, std::optional<Type*> _enclosing_type, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body);
    OperatorSignature* resolve_operator_signature() const;
};