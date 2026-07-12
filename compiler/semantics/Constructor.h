#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"
#include <vector>

struct Type;
struct Parameter;
struct CompoundStatement;
struct ConstructorSignature;
struct TemplateMapping;
struct ArrayType;
struct ASTNode;
struct CompilationContext;

struct Constructor : public ASTNode {
    Type *type;
    std::vector<Parameter*> parameters;

    bool is_generated = false;

    Constructor(parser::token *tok);
    Constructor(const Constructor& other);
    Constructor(Type* _type, std::vector<Parameter*> _parameters);

    static Constructor* convert(parser::constructor *c);
    virtual Constructor* make_copy() = 0;
    virtual bool is_well_formed(CompilationContext* ctx) = 0;
    ConstructorSignature* resolve_constructor_signature() const;
    virtual bool equals(Constructor* other) const = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates(CompilationContext* ctx) = 0;
};  

struct StructConstructor : public Constructor {
    CompoundStatement *body;

    StructConstructor(parser::token *c);
    StructConstructor(const StructConstructor& other);
    StructConstructor(Type* _type, std::vector<Parameter*> _parameters, CompoundStatement* _body);

    Constructor* make_copy() override;
    bool is_well_formed(CompilationContext* ctx) override;
    bool equals(Constructor* other) const override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates(CompilationContext* ctx) override;
};

struct PrimitiveConstructor : public Constructor {
    bool is_copy_constructor;

    PrimitiveConstructor(const PrimitiveConstructor& other);
    PrimitiveConstructor(Type *_type, bool _is_copy_constructor);

    Constructor* make_copy() override;
    bool is_well_formed(CompilationContext* ctx) override;
    bool equals(Constructor* other) const override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates(CompilationContext* ctx) override;
};