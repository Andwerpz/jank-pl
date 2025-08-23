#pragma once
#include "../parser/parser.h"
#include <optional>

struct Parameter;
struct CompoundStatement;
struct Type;
struct OperatorSignature;
struct TemplateMapping;
struct OperatorCall;
struct OperatorSignature;

struct Operator {
    Type *type;

    virtual OperatorSignature* resolve_operator_signature() const = 0;
    virtual Operator* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates() = 0;

    bool is_valid_call(OperatorCall *oc);
};

//just some hardcoded assembly. Is directly emitted when needed
//when returning a l-value, %rcx is where the mem address should go
//for binary operators, assumes left is in %rax, right in %rcx
//for left unary operators, assumes right is in %rax
//for right unary operators, assumes left is in %rax
//for casting, assumes the input is in %rax
//for the indexing operator specifically, '[]', the result of the expression is in %rbx
//will place the answer into %rax, address in %rcx
struct BuiltinOperator : public Operator {
    std::optional<Type*> left;
    std::string op;
    std::optional<Type*> right;
    std::vector<std::string> instructions;

    BuiltinOperator(Type *_type, std::optional<Type*> _left, std::string _op, std::optional<Type*> _right, std::vector<std::string> _instructions);

    OperatorSignature* resolve_operator_signature() const override;
    Operator* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;

    void emit_asm();
};

//acts more like a function call. Gets called by OverloadCall
struct OperatorOverload : public Operator {
    std::string op;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;

    OperatorOverload(Type *_type, std::string _op, std::vector<Parameter*> _parameters, CompoundStatement *_body);
    static OperatorOverload* convert(parser::overload *o);

    OperatorSignature* resolve_operator_signature() const override;
    Operator* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;

    bool is_well_formed();
};