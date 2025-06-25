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
    CompoundStatement *body;
    Constructor(Type* _type, std::vector<Parameter*> _parameters, CompoundStatement *_body);

    static Constructor* convert(parser::constructor *c);
    ConstructorSignature* resolve_constructor_signature() const;
    bool equals(const Constructor* other) const;
    bool is_well_formed();
    Constructor* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};