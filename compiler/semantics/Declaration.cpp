#include "Declaration.h"

#include "Type.h"
#include "Identifier.h"
#include "Expression.h"
#include "TemplateMapping.h"

#include "utils.h"

Declaration::Declaration(Type *_type, Identifier *_id, std::optional<Expression*> _expr) {
    type = _type;
    id = _id;
    expr = _expr;

    if(expr.has_value()) assert(expr.value() != nullptr);
}

Declaration* Declaration::convert(parser::declaration *d) {
    Type *type = Type::convert(d->t0);
    Identifier *name = Identifier::convert(d->t2);
    std::optional<Expression*> expr = std::nullopt;
    if(d->t3.has_value()) {
        expr = Expression::convert(d->t3.value()->t3);
    }
    return new Declaration(type, name, expr);
}

bool Declaration::is_well_formed() {
    if(asm_debug) fout << indent() << "# initialize local variable : " << type->to_string() << " " << id->name << "\n";
    Variable *v = emit_initialize_stack_variable(type, id, expr);
    if(asm_debug) fout << indent() << "# done initialize local variable : " << type->to_string() << " " << id->name << "\n";

    if(v == nullptr) {
        std::cout << "Failed to initialize variable : " << type->to_string() << " " << id->name << "\n";
        return false;
    }

    return true;
}

Declaration* Declaration::make_copy() {
    std::optional<Expression*> _expr = std::nullopt;
    if(expr.has_value()) _expr = expr.value()->make_copy();
    return new Declaration(type->make_copy(), id->make_copy(), _expr);
}

bool Declaration::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    if(expr.has_value() && !expr.value()->replace_templated_types(mapping)) return false;
    return true;
}

bool Declaration::look_for_templates(){
    if(!type->look_for_templates()) return false;
    if(expr.has_value() && !expr.value()->look_for_templates()) return false;
    return true;
}

std::string Declaration::to_string() {
    std::string ret = type->to_string() + " " + id->name;
    if(expr.has_value()) ret += " = " + expr.value()->to_string();
    return ret;
}