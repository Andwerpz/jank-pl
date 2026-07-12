#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"
#include <optional>

struct Declaration;
struct TemplateMapping;
struct Identifier;
struct CompilationContext;

struct GlobalDeclaration : public ASTNode {
    std::optional<Identifier*> node_id;
    bool is_extern;
    Declaration *declaration;

    GlobalDeclaration(parser::token *tok);
    GlobalDeclaration(const GlobalDeclaration& other);
    GlobalDeclaration(std::optional<Identifier*> _node_id, bool _is_extern, Declaration *_declaration);

    static GlobalDeclaration* convert(parser::global_declaration *gd);
    GlobalDeclaration* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates(CompilationContext* ctx);

    std::string generate_init_label();
    std::string generate_cleanup_label();
};