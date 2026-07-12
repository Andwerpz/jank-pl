#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct Type;
struct Identifier;
struct TemplateMapping;
struct CompilationContext;

struct Parameter : public ASTNode {
    Type *type;
    Identifier *id;

    Parameter(parser::token *tok);
    Parameter(const Parameter& other);
    Parameter(Type *_type, Identifier *_id);

    static Parameter* convert(parser::parameter *p);
    Parameter* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates(CompilationContext* ctx);
    std::string to_string();
};