#pragma once
#include "../parser/parser.h"
#include <optional>

struct Declaration;
struct TemplateMapping;
struct Identifier;

struct GlobalDeclaration {
    std::optional<Identifier*> node_id;
    bool is_extern;
    Declaration *declaration;

    GlobalDeclaration(std::optional<Identifier*> _node_id, bool _is_extern, Declaration *_declaration);

    static GlobalDeclaration* convert(parser::global_declaration *gd);
    GlobalDeclaration* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};