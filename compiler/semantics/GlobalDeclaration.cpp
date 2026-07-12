#include "GlobalDeclaration.h"
#include "Declaration.h"
#include <cstring>
#include <string>
#include <optional>
#include "utils.h"
#include "Type.h"
#include "Identifier.h"
#include "Expression.h"
#include "CompilationContext.h"

GlobalDeclaration::GlobalDeclaration(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

GlobalDeclaration::GlobalDeclaration(const GlobalDeclaration& other) : ASTNode(other) {
    node_id = std::nullopt;
    if(other.node_id.has_value()) node_id = other.node_id.value()->make_copy();
    is_extern = other.is_extern;
    declaration = other.declaration->make_copy();
}

GlobalDeclaration::GlobalDeclaration(std::optional<Identifier*> _node_id, bool _is_extern, Declaration *_declaration) : ASTNode() {
    node_id = _node_id;
    is_extern = _is_extern;
    declaration = _declaration;
    if(node_id.has_value()) assert(node_id.value() != nullptr);
    assert(declaration != nullptr);
}

GlobalDeclaration* GlobalDeclaration::convert(parser::global_declaration *gd) {
    GlobalDeclaration* result = new GlobalDeclaration(gd);
    result->node_id = std::nullopt;
    if(gd->t0.has_value()) result->node_id = Identifier::convert(gd->t0.value()->t2);
    result->is_extern = gd->t1.has_value();
    result->declaration = Declaration::convert(gd->t2);
    return result;
}

GlobalDeclaration* GlobalDeclaration::make_copy() {
    return new GlobalDeclaration(*this);
}

bool GlobalDeclaration::replace_templated_types(TemplateMapping *mapping) {
    return declaration->replace_templated_types(mapping);
}

bool GlobalDeclaration::look_for_templates(CompilationContext *ctx) {
    return declaration->look_for_templates(ctx);
}

std::string GlobalDeclaration::generate_init_label() {
    return "__global_init__" + declaration->id->to_string();
}

std::string GlobalDeclaration::generate_cleanup_label() {
    return "__global_cleanup__" + declaration->id->to_string();
}
