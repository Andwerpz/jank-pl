#include "OperatorCall.h"
#include "utils.h"
#include "Expression.h"
#include "Operator.h"
#include "Identifier.h"
#include "Parameter.h"
#include "Operator.h"
#include "Type.h"
#include "OperatorSignature.h"

OperatorCall::OperatorCall(std::optional<ExprNode*> _left, std::string _op, std::optional<ExprNode*> _right) {
    left = _left;
    op = _op;
    right = _right;
}

Operator* OperatorCall::resolve_called_operator() {
    return get_called_operator(left, op, right);
}

Type* OperatorCall::resolve_type() {
    Operator *o = this->resolve_called_operator();
    if(o == nullptr) {
        std::cout << "Cannot resolve operator call : " << to_string() << "\n";
        return nullptr;
    }
    return o->type->make_copy();
}

//only works for overloads. Builtins should be handled with Expression
//the name is kinda misleading, but we do use this to resolve called operators so ¯\_(:3)_/¯
void OperatorCall::emit_asm() {
    if(asm_debug) fout << indent() << "# calling overload : \n";

    OperatorOverload *o = dynamic_cast<OperatorOverload*>(resolve_called_operator());
    assert(o != nullptr);
    OperatorSignature *os = o->resolve_operator_signature();
    assert(os != nullptr);

    //create temp variables for all arguments
    push_declaration_stack();
    if(left.has_value()) {
        assert(os->left.has_value());
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Variable *v = emit_initialize_stack_variable(os->left.value(), id, new Expression(left.value()));
        assert(v != nullptr);
    }
    if(right.has_value()) {
        assert(os->right.has_value());
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Variable *v = emit_initialize_stack_variable(os->right.value(), id, new Expression(right.value()));
        assert(v != nullptr);
    }

    //call overload
    std::string label = get_operator_label(o->resolve_operator_signature());
    fout << indent() << "call " << label << "\n";

    //clean up temp variables, freeing them is handled by the operator
    pop_declaration_stack(false);
}

std::string OperatorCall::to_string() {
    std::string res = "";
    res += "operator" + op;
    res += "(";
    if(left.has_value()) res += left.value()->to_string();
    if(left.has_value() && right.has_value()) res += ", ";
    if(right.has_value()) res += right.value()->to_string();
    res += ")";
    return res;
}

size_t OperatorCall::hash() {
    size_t hash = 0;
    if(left.has_value()) hash_combine(hash, left.value()->hash());
    else hash_combine(hash, 0);
    hash_combine(hash, std::hash<std::string>()(op));
    if(right.has_value()) hash_combine(hash, right.value()->hash());
    else hash_combine(hash, 0);
    return hash;
}

bool OperatorCall::equals(OperatorCall *other) {
    if(left.has_value() != other->left.has_value()) return false;
    if(left.has_value() && !left.value()->equals(other->left.value())) return false;
    if(op != other->op) return false;
    if(right.has_value() != other->right.has_value()) return false;
    if(right.has_value() && !right.value()->equals(other->right.value())) return false;
    return true;
}

OperatorCall* OperatorCall::make_copy() {
    std::optional<ExprNode*> _left = std::nullopt;
    std::optional<ExprNode*> _right = std::nullopt;
    std::string _op = op;
    if(left.has_value()) _left = left.value()->make_copy();
    if(right.has_value()) _right = right.value()->make_copy();
    return new OperatorCall(_left, _op, _right);
}

bool OperatorCall::replace_templated_types(TemplateMapping *mapping) {
    if(left.has_value()) if(!left.value()->replace_templated_types(mapping)) return false;
    if(right.has_value()) if(!right.value()->replace_templated_types(mapping)) return false;
    return true;
}

bool OperatorCall::look_for_templates() {
    if(left.has_value()) if(!left.value()->look_for_templates()) return false;
    if(right.has_value()) if(!right.value()->look_for_templates()) return false;
    return true;
}