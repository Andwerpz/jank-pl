#pragma once
#include "../parser/parser.h"
#include <vector>

struct Type;
struct Parameter;
struct CompoundStatement;
struct ConstructorSignature;
struct TemplateMapping;

struct Constructor {
    Type *type;
    std::vector<Parameter*> parameters;

    static Constructor* convert(parser::constructor *c);
    ConstructorSignature* resolve_constructor_signature() const;
    virtual bool equals(Constructor* other) const = 0;
    virtual bool is_well_formed() = 0;
    virtual Constructor* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates() = 0;
};  

struct StructConstructor : public Constructor {
    CompoundStatement *body;
    StructConstructor(Type* _type, std::vector<Parameter*> _parameters, CompoundStatement *_body);

    bool equals(Constructor* other) const override;
    bool is_well_formed() override;
    Constructor* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
};

struct PrimitiveConstructor : public Constructor {
    bool is_copy_constructor;
    PrimitiveConstructor(Type *_type, bool _is_copy_constructor);

    bool equals(Constructor* other) const override;
    bool is_well_formed() override;
    Constructor* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
};