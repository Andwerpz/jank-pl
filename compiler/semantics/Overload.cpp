#include "Overload.h"
#include "Type.h"
#include "Statement.h"
#include "Parameter.h"
#include "utils.h"
#include "TemplateMapping.h"
#include "Identifier.h"
#include "primitives.h"

Overload::Overload(Type *_type, std::string _op, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
    assert(_type != nullptr);
    assert(_body != nullptr);
    type = _type;
    op = _op;
    parameters = _parameters;
    body = _body;
}

Overload* Overload::convert(parser::overload *o) {
    parser::overload_definition *def = o->t0;
    parser::parameter_list *pl = def->t7;
    Type *type = Type::convert(def->t0);
    std::string op = def->t3->to_string();
    std::vector<Parameter*> parameters;
    if(pl->t0 != nullptr) {
        parameters.push_back(Parameter::convert(pl->t0->t0));
        std::vector<parser::parameter_list::a0::b0*> tmp = pl->t0->t1;
        for(int i = 0; i < tmp.size(); i++){
            parameters.push_back(Parameter::convert(tmp[i]->t3));
        }
    }
    CompoundStatement *body = CompoundStatement::convert(o->t2);
    return new Overload(type, op, parameters, body);
}

bool Overload::is_well_formed() {
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
    std::string label = get_overload_label(os);
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
        Variable* v = add_variable(parameters[i]->type, parameters[i]->id);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << parameters[i]->type->to_string() << " " << parameters[i]->id->name << "\n";
            return false;
        }
        v->addr = std::to_string(local_offset) + "(%rbp)";
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

OperatorSignature* Overload::resolve_operator_signature() const {
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

Overload* Overload::make_copy() {
    Type *_type = type->make_copy();
    std::string _op = op;
    std::vector<Parameter*> _parameters;
    for(int i = 0; i < parameters.size(); i++){
        _parameters.push_back(parameters[i]->make_copy());
    }
    CompoundStatement *_body = dynamic_cast<CompoundStatement*>(body->make_copy());
    return new Overload(_type, _op, _parameters, _body);
}

bool Overload::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    for(int i = 0; i < parameters.size(); i++) {
        if(!parameters[i]->replace_templated_types(mapping)) return false;
    }
    if(!body->replace_templated_types(mapping)) return false;
    return true;
}

bool Overload::look_for_templates() {
    if(!type->look_for_templates()) return false;
    for(int i = 0; i < parameters.size(); i++) if(!parameters[i]->look_for_templates()) return false;
    if(!body->look_for_templates()) return false;
    return true;
}