#include "Declaration.h"

#include "Type.h"
#include "Identifier.h"
#include "Expression.h"

#include "utils.h"

Declaration::Declaration(Type *_type, Identifier *_id, Expression *_expr) {
    type = _type;
    id = _id;
    expr = _expr;
}

Declaration* Declaration::convert(parser::declaration *d) {
    Type *type = Type::convert(d->t0);
    Identifier *name = Identifier::convert(d->t2);
    Expression *expr = Expression::convert(d->t6);
    return new Declaration(type, name, expr);
}

bool Declaration::is_well_formed() {
    if(asm_debug) fout << indent() << "# initialize local variable : " << type->to_string() << " " << id->name << "\n";
    Variable *v = emit_initialize_variable(type, id, expr);
    if(asm_debug) fout << indent() << "# done initialize local variable : " << type->to_string() << " " << id->name << "\n";

    if(v == nullptr) {
        return false;
    }

    return true;
}

Declaration* Declaration::make_copy() {
    return new Declaration(type->make_copy(), id->make_copy(), expr->make_copy());
}

bool Declaration::replace_templated_types(TemplateMapping *mapping) {
    if(!type->replace_templated_types(mapping)) return false;
    if(!expr->replace_templated_types(mapping)) return false;
    return true;
}

void Declaration::look_for_templates(){
    type->look_for_templates();
    expr->look_for_templates();
}