#include "Declaration.h"

#include "Type.h"
#include "Identifier.h"
#include "Expression.h"
#include "TemplateMapping.h"
#include "CompilationContext.h"
#include "DefinitionSpace.h"

#include "utils.h"

Declaration::Declaration(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Declaration::Declaration(const Declaration& other) : ASTNode(other) {
    type = other.type->make_copy();
    id = other.id->make_copy();
    expr = std::nullopt;
    if(other.expr != std::nullopt) {
        expr = other.expr.value()->make_copy();
    }
}

Declaration::Declaration(Type *_type, Identifier *_id, std::optional<Expression*> _expr) : ASTNode() {
    type = _type;
    id = _id;
    expr = _expr;

    if(expr.has_value()) assert(expr.value() != nullptr);
}

Declaration* Declaration::convert(parser::declaration *d) {
    Declaration* result = new Declaration(d);
    result->type = Type::convert(d->t0);
    result->id = Identifier::convert(d->t2);
    result->expr = std::nullopt;
    if(d->t3.has_value()) {
        result->expr = Expression::convert(d->t3.value()->t3);
    }
    return result;
}

bool Declaration::is_well_formed(CompilationContext *ctx) {
    // - make sure type is declared
    if(!ctx->is_type_declared(type)) {
        std::cout << "Declaration type " << type->to_string() << " is not declared\n";
        return false;
    }

    if(asm_debug) fout << indent() << "# initialize local variable : " << type->to_string() << " " << id->name << "\n";
    Variable *v = emit_initialize_stack_variable(ctx, type, id, expr);
    if(asm_debug) fout << indent() << "# done initialize local variable : " << type->to_string() << " " << id->name << "\n";

    if(v == nullptr) {
        std::cout << "Failed to initialize variable : " << type->to_string() << " " << id->name << "\n";
        return false;
    }

    return true;
}

Declaration* Declaration::make_copy() {
    return new Declaration(*this);
}

bool Declaration::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    if(expr.has_value() && !expr.value()->replace_templated_types(mapping)) return false;
    return true;
}

bool Declaration::look_for_templates(CompilationContext *ctx){
    if(!type->look_for_templates(ctx)) return false;
    if(expr.has_value() && !expr.value()->look_for_templates(ctx)) return false;
    return true;
}

std::string Declaration::to_string() {
    std::string ret = type->to_string() + " " + id->name;
    if(expr.has_value()) ret += " = " + expr.value()->to_string();
    return ret;
}