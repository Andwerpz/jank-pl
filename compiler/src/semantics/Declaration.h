#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"
#include <optional>

struct Type;
struct Identifier;
struct Expression;
struct TemplateMapping;
struct CompilationContext;

struct Declaration : public ASTNode {
    Type *type;
    Identifier *id;
    std::optional<Expression*> expr;
    
    Declaration(parser::token *tok);
    Declaration(const Declaration& other);
    Declaration(Type *_type, Identifier *_id, std::optional<Expression*> _expr);

    static Declaration* convert(parser::declaration *d);
    bool is_well_formed(CompilationContext* ctx);
    Declaration* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates(CompilationContext* ctx);
    std::string to_string();
};