#include "OverloadCall.h"
#include "utils.h"
#include "Expression.h"
#include "Overload.h"
#include "Identifier.h"
#include "Parameter.h"

OverloadCall::OverloadCall(std::optional<ExprNode*> _left, std::string _op, std::optional<ExprNode*> _right) {
    left = _left;
    op = _op;
    right = _right;
}

Overload* OverloadCall::resolve_called_overload() {
    OperatorImplementation *oi = find_operator_implementation(left, op, right);
    if(auto x = dynamic_cast<OverloadedOperator*>(oi)) return x->overload;
    return nullptr;
}

Type* OverloadCall::resolve_type() {
    Overload *o = resolve_called_overload();
    if(o == nullptr) {
        std::cout << "Cannot resolve overload call : " << to_string() << "\n";
        return nullptr;
    }
    return o->type;
}

void OverloadCall::emit_asm() {
    if(asm_debug) fout << indent() << "# calling overload : \n";

    Overload *o = this->resolve_called_overload();
    assert(o != nullptr);

    //create temp variables for all arguments
    push_declaration_stack();
    int param_ptr = 0;
    if(left.has_value()) {
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Variable *v = emit_initialize_stack_variable(o->parameters[param_ptr ++]->type, id, new Expression(left.value()));
        assert(v != nullptr);
    }
    if(right.has_value()) {
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Variable *v = emit_initialize_stack_variable(o->parameters[param_ptr ++]->type, id, new Expression(right.value()));
        assert(v != nullptr);
    }

    //call overload
    std::string label = get_overload_label(o->resolve_operator_signature());
    fout << indent() << "call " << label << "\n";

    //clean up temp variables
    pop_declaration_stack();
}

std::string OverloadCall::to_string() {
    std::string res = "";
    res += "operator" + op;
    res += "(";
    if(left.has_value()) res += left.value()->to_string();
    if(left.has_value() && right.has_value()) res += ", ";
    if(right.has_value()) res += right.value()->to_string();
    res += ")";
    return res;
}

size_t OverloadCall::hash() {
    size_t hash = 0;
    if(left.has_value()) hash_combine(hash, left.value()->hash());
    else hash_combine(hash, 0);
    hash_combine(hash, std::hash<std::string>()(op));
    if(right.has_value()) hash_combine(hash, right.value()->hash());
    else hash_combine(hash, 0);
    return hash;
}

bool OverloadCall::equals(OverloadCall *other) {
    if(left.has_value() != other->left.has_value()) return false;
    if(left.has_value() && !left.value()->equals(other->left.value())) return false;
    if(op != other->op) return false;
    if(right.has_value() != other->right.has_value()) return false;
    if(right.has_value() && !right.value()->equals(other->right.value())) return false;
    return true;
}

OverloadCall* OverloadCall::make_copy() {
    std::optional<ExprNode*> _left = std::nullopt;
    std::optional<ExprNode*> _right = std::nullopt;
    std::string _op = op;
    if(left.has_value()) _left = left.value()->make_copy();
    if(right.has_value()) _right = right.value()->make_copy();
    return new OverloadCall(_left, _op, _right);
}

bool OverloadCall::replace_templated_types(TemplateMapping *mapping) {
    if(left.has_value()) if(!left.value()->replace_templated_types(mapping)) return false;
    if(right.has_value()) if(!right.value()->replace_templated_types(mapping)) return false;
    return true;
}

bool OverloadCall::look_for_templates() {
    if(left.has_value()) if(!left.value()->look_for_templates()) return false;
    if(right.has_value()) if(!right.value()->look_for_templates()) return false;
    return true;
}