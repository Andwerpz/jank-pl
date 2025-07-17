#include "GlobalDeclaration.h"
#include "Declaration.h"
#include <cstring>
#include <string>
#include "utils.h"
#include "Type.h"
#include "Identifier.h"
#include "Expression.h"

GlobalDeclaration::GlobalDeclaration(bool _is_extern, int _tier, Declaration *_declaration) {
    is_extern = _is_extern;
    tier = _tier;
    declaration = _declaration;
    assert(declaration != nullptr);
}

GlobalDeclaration* GlobalDeclaration::convert(parser::global_declaration *gd) {
    std::string tier_str = "";
    if(gd->t2 != nullptr) tier_str += "-";
    for(int i = 0; i < gd->t3.size(); i++) tier_str += gd->t3[i]->to_string();
    int tier = std::stoi(tier_str);
    bool is_extern = gd->t7 != nullptr;
    Declaration *declaration = Declaration::convert(gd->t8);
    return new GlobalDeclaration(is_extern, tier, declaration);
}

GlobalDeclaration* GlobalDeclaration::make_copy() {
    return new GlobalDeclaration(is_extern, tier, declaration->make_copy());
}

bool GlobalDeclaration::replace_templated_types(TemplateMapping *mapping) {
    return declaration->replace_templated_types(mapping);
}

bool GlobalDeclaration::look_for_templates() {
    return declaration->look_for_templates();
}