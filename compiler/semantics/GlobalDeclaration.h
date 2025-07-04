#pragma once
#include "../parser/parser.h"

struct Declaration;
struct TemplateMapping;

struct GlobalDeclaration {
    int tier;
    Declaration *declaration;

    GlobalDeclaration(int _tier, Declaration *_declaration);

    static GlobalDeclaration* convert(parser::global_declaration *gd);
    GlobalDeclaration* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    bool look_for_templates();
};