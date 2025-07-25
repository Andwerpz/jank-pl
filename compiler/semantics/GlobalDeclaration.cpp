#include "GlobalDeclaration.h"
#include "Declaration.h"
#include <cstring>
#include <string>
#include <optional>
#include "utils.h"
#include "Type.h"
#include "Identifier.h"
#include "Expression.h"

GlobalDeclaration::GlobalDeclaration(std::optional<Identifier*> _node_id, bool _is_extern, Declaration *_declaration) {
    node_id = _node_id;
    is_extern = _is_extern;
    declaration = _declaration;
    assert(declaration != nullptr);
}

GlobalDeclaration* GlobalDeclaration::convert(parser::global_declaration *gd) {
    std::optional<Identifier*> node_id = std::nullopt;
    if(gd->t0 != nullptr) node_id = Identifier::convert(gd->t0->t2);
    bool is_extern = gd->t1 != nullptr;
    Declaration *declaration = Declaration::convert(gd->t2);
    return new GlobalDeclaration(node_id, is_extern, declaration);
}

GlobalDeclaration* GlobalDeclaration::make_copy() {
    std::optional<Identifier*> _node_id = std::nullopt;
    if(node_id.has_value()) _node_id = node_id.value()->make_copy();
    return new GlobalDeclaration(_node_id, is_extern, declaration->make_copy());
}

bool GlobalDeclaration::replace_templated_types(TemplateMapping *mapping) {
    return declaration->replace_templated_types(mapping);
}

bool GlobalDeclaration::look_for_templates() {
    return declaration->look_for_templates();
}