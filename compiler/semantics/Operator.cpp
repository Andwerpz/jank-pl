#include "Operator.h"
#include "Type.h"
#include "OperatorSignature.h"
#include "Parameter.h"
#include "Statement.h"
#include "TemplateMapping.h"
#include "utils.h"
#include "primitives.h"
#include "Identifier.h"
#include "OperatorCall.h"
#include "Expression.h"

// -- CONSTRUCTORS --
BuiltinOperator::BuiltinOperator(Type *_type, std::optional<Type*> _left, std::string _op, std::optional<Type*> _right, std::vector<std::string> _instructions) {
    assert(_type != nullptr);

    type = _type;
    left = _left;
    op = _op;
    right = _right;
    instructions = _instructions;
}

OperatorOverload::OperatorOverload(Type *_type, std::string _op, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
    assert(_type != nullptr);

    type = _type;
    op = _op;
    parameters = _parameters;
    body = _body;
}

// -- CONVERT --
OperatorOverload* OperatorOverload::convert(parser::overload *o) {
    parser::overload_definition *def = o->t0;
    parser::parameter_list *pl = def->t7;
    Type *type = Type::convert(def->t0);
    std::string op = def->t3->to_string();
    std::vector<Parameter*> parameters = convert_parameter_list(pl);
    CompoundStatement *body = CompoundStatement::convert(o->t2);
    return new OperatorOverload(type, op, parameters, body);
}

// -- RESOLVE OPERATOR SIGNATURE --
OperatorSignature* BuiltinOperator::resolve_operator_signature() const {
    return new OperatorSignature(left, op, right);
}

OperatorSignature* OperatorOverload::resolve_operator_signature() const {
    if( op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || 
        op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>" ||
        op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || 
        op == ">=" || op == "=" || op == "+=" || op == "-=" || op == "*=" ||
        op == "/=" || op == "%=" || op == "&=" || op == "|=" || op == "^=" ||
        op == "<<=" || op == ">>=") {
        //binary operator
        if(this->parameters.size() != 2) return nullptr;
        Type *left = this->parameters[0]->type, *right = this->parameters[1]->type;
        return new OperatorSignature(left, op, right);
    }
    else if(op == "++x" || op == "--x" || op == "*x") {
        //prefix operator
        if(this->parameters.size() != 1) return nullptr;
        Type *right = this->parameters[0]->type;
        if(op == "++x") return new OperatorSignature("++", right);
        else if(op == "--x") return new OperatorSignature("--", right);
        else if(op == "*x") return new OperatorSignature("*x", right);
        else assert(false);
    }
    else if(op == "x++" || op == "x--") {
        //postfix operator
        if(this->parameters.size() != 1) return nullptr;
        Type *left = this->parameters[0]->type;
        if(op == "x++") return new OperatorSignature(left, "++");
        else if(op == "x--") return new OperatorSignature(left, "--");
        else assert(false);
    }
    else if(op == "[]") {
        //indexing
        if(this->parameters.size() != 2) return nullptr;
        Type *left = this->parameters[0]->type, *et = this->parameters[1]->type;
        return new OperatorSignature(left, "[]", et);
    }
    //TODO handle '$' (casting operator)
    else assert(false);
}

// -- MAKE COPY --
Operator* BuiltinOperator::make_copy() {
    Type *_type = type->make_copy();
    std::string _op = op;
    std::optional<Type*> _left = std::nullopt, _right = std::nullopt;
    if(left.has_value()) _left = left.value()->make_copy();
    if(right.has_value()) _right = right.value()->make_copy();
    std::vector<std::string> _instructions;
    for(int i = 0; i < instructions.size(); i++){
        _instructions.push_back(instructions[i]);
    }
    return new BuiltinOperator(_type, _left, _op, _right, _instructions);
}

Operator* OperatorOverload::make_copy() {
    Type *_type = type->make_copy();
    std::string _op = op;
    std::vector<Parameter*> _parameters;
    for(int i = 0; i < parameters.size(); i++){
        _parameters.push_back(parameters[i]->make_copy());
    }
    CompoundStatement *_body = dynamic_cast<CompoundStatement*>(body->make_copy());
    return new OperatorOverload(_type, _op, _parameters, _body);
}

// -- REPLACE TEMPLATED TYPES --
bool BuiltinOperator::replace_templated_types(TemplateMapping *mapping) {
    return true;    //shouldn't be anything to replace
}

bool OperatorOverload::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    for(int i = 0; i < parameters.size(); i++) {
        if(!parameters[i]->replace_templated_types(mapping)) return false;
    }
    if(!body->replace_templated_types(mapping)) return false;
    return true;
}

// -- LOOK FOR TEMPLATES --
bool BuiltinOperator::look_for_templates() {
    return true;    //shouldn't be any templates
}

bool OperatorOverload::look_for_templates() {
    if(!type->look_for_templates()) return false;
    for(int i = 0; i < parameters.size(); i++) if(!parameters[i]->look_for_templates()) return false;
    if(!body->look_for_templates()) return false;
    return true;
}

// -- MISC --
bool Operator::is_valid_call(OperatorCall *oc) {
    OperatorSignature *os = this->resolve_operator_signature();

    // - do the operators match?
    if(os->op != oc->op) {
        return false;
    }

    // - do the left arguments match?
    if(os->left.has_value() != oc->left.has_value()) {
        return false;
    }
    if(os->left.has_value() && !is_declarable(os->left.value()->make_copy(), new Expression(oc->left.value()->make_copy()))) {
        return false;
    }

    // - do the right arguments match?
    if(os->right.has_value() != oc->right.has_value()) {
        return false;
    }
    if(os->right.has_value() && !is_declarable(os->right.value()->make_copy(), new Expression(oc->right.value()->make_copy()))) {
        return false;
    }

    //all checks passed
    return true;
}

void BuiltinOperator::emit_asm() {
    for(int i = 0; i < instructions.size(); i++){
        fout << indent() << instructions[i] << "\n";
    }
}

bool OperatorOverload::is_well_formed() {
    OperatorSignature *os = resolve_operator_signature();
    if(os == nullptr) {
        std::cout << "Overload does not resolve to operator signature\n";
        return false;
    }

    std::cout << "CHECKING OVERLOAD : " << os->to_string() << "\n";

    // - are templates all resolvable?
    if(!look_for_templates()) {
        std::cout << "Unable to resolve templates in overload : " << os->to_string() << "\n";
        return false;
    }

    push_declaration_stack();

    //print function label
    if(asm_debug) {
        fout << "# " << os->to_string() << "\n";
    }
    std::string label = get_operator_label(os);
    fout << label << ":\n";

    //setup stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";

    for(int i = 0; i < parameters.size(); i++){
        // - does parameter correspond to existing type?
        if(!is_type_declared(parameters[i]->type)) {
            std::cout << "Undeclared type : " << parameters[i]->type->to_string() << "\n";
            return false;
        }
        // - is parameter type not void?
        if(parameters[i]->type->equals(primitives::_void)) {
            std::cout << "Parameter can't have type void\n";
            return false;
        }
    }

    // - is return type of function existing?
    if(!is_type_declared(type)) {
        std::cout << "Overload undeclared return type : " << type->to_string() << " " << os->to_string() << "\n";
        return false;
    }

    //setup local variables
    local_offset = 8 + 8 * parameters.size();
    for(int i = 0; i < parameters.size(); i++){
        Variable* v = add_stack_variable(parameters[i]->type, parameters[i]->id);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << parameters[i]->type->to_string() << " " << parameters[i]->id->name << "\n";
            return false;
        }
        local_offset -= 8;
    }

    //set local offset equal to %rsp
    local_offset = 0;

    //check that the local stack is empty so far
    assert(stack_desc.size() == 0);

    // - make sure body is well formed
    if(!body->is_well_formed()) {
        std::cout << "Function body not well formed\n";
        return false;
    }

    // - if type is not void, check for existence of return statement as last reachable statement
    // - constructors also don't have to have return statements, they're treated as void functions
    if(!type->equals(primitives::_void)) {
        // Note that if there is a statement before the last that is always returning, then any statement after
        // it is unreachable code, in which case we should print some warnings. 
        if(!body->is_always_returning()) {
            std::cout << "Non-void function doesn't always return\n";
            return false;
        }
    }
    else {
        //add trailing return for void functions
        fout << indent() << "pop %rbp\n";   //should not be managed by local_offset
        fout << indent() << "ret\n";
    }

    fout << "\n";

    //unregister parameters as variables
    pop_declaration_stack();

    //local stack should be empty before returning
    assert(stack_desc.size() == 0);

    return true;
}