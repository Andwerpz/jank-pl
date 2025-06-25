#pragma once
#include "../parser/parser.h"

struct Parameter;
struct CompoundStatement;
struct Type;
struct OperatorSignature;
struct TemplateMapping;

struct Overload {
    Type *type;
    std::string op;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;
    Overload(Type *_type, std::string _op, std::vector<Parameter*> _parameters, CompoundStatement *_body);
    static Overload* convert(parser::overload *o);
    OperatorSignature* resolve_operator_signature() const;
    bool is_well_formed();
    Overload* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};