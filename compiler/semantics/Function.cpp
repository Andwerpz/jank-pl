#include "Function.h"
#include "Identifier.h"
#include "Type.h"
#include "FunctionSignature.h"
#include "utils.h"
#include "Statement.h"
#include "Parameter.h"

Function::Function(std::optional<Type*> _enclosing_type, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
    enclosing_type = _enclosing_type;
    id = _id;
    type = _type;
    parameters = _parameters;
    body = _body;

    assert(type != nullptr);
    assert(body != nullptr);
    assert(id != nullptr);
}

Function::Function(Type *_type, Identifier *_id, std::vector<Type*> input_types) {
    enclosing_type = std::nullopt;
    id = _id;
    type = _type;
    for(int i = 0; i < input_types.size(); i++){
        parameters.push_back(new Parameter(input_types[i], new Identifier("v" + std::to_string(i))));
    }
    body = nullptr;

    assert(type != nullptr);
    assert(id != nullptr);
}

FunctionSignature* Function::resolve_function_signature() const {
    std::vector<Type*> input_types;
    for(int i = 0; i < parameters.size(); i++) input_types.push_back(parameters[i]->type);
    if(enclosing_type.has_value() && !is_function_constructor(this)) return new FunctionSignature(enclosing_type.value(), id, input_types);
    return new FunctionSignature(id, input_types);
}

bool Function::equals(const Function* other) const {
    return this->resolve_function_signature()->equals(other->resolve_function_signature());
}

bool Function::operator==(const Function& other) const {
    return this->equals(&other);
}

bool Function::operator!=(const Function& other) const {
    return !(*this == other);
}

Function* Function::convert(parser::function *f) {
    parser::function_definition *def = f->t0;
    parser::parameter_list *pl = def->t6;
    Type *type = Type::convert(def->t0);
    Identifier *name = new Identifier(def->t2->to_string());
    std::vector<Parameter*> parameters;
    if(pl->t0 != nullptr) {
        parameters.push_back(Parameter::convert(pl->t0->t0));
        std::vector<parser::parameter_list::a0::b0*> tmp = pl->t0->t1;
        for(int i = 0; i < tmp.size(); i++){
            parameters.push_back(Parameter::convert(tmp[i]->t3));
        }
    }
    CompoundStatement *body = CompoundStatement::convert(f->t2);
    if(def->t2->is_a0) {
        //operator overload
        std::string op = def->t2->t0->t0->t1->to_string();
        return new OperatorOverload(op, std::nullopt, type, name, parameters, body);
    }
    else {
        //regular function
        return new Function(std::nullopt, type, name, parameters, body);
    }
}

bool Function::is_well_formed() {
    FunctionSignature *fs = resolve_function_signature();

    if(body == nullptr) {
        std::cout << "SKIPPING SYS FUNCTION : " << fs->to_string() << std::endl;
        return true;
    }
    std::cout << "CHECKING FUNCTION : " << fs->to_string() << std::endl;

    push_declaration_stack();

    bool is_main = FunctionSignature(new Identifier("main"), {}) == *(fs);
    bool is_constructor = is_function_constructor(this);

    // - if this function name is exactly equal to a type, make sure that it's a constructor
    if(is_type_declared(new BaseType(id->name)) && !is_function_constructor(this)) {
        std::cout << "Invalid function name : " << id->name << "\n";
        return false;
    }

    //print function label
    //if function signature is main(), substitute main for _start
    if(asm_debug) {
        fout << "# " << fs->to_string() << "\n";
    }
    if(is_main) {
        fout << ".global _start\n";
        fout << "_start:\n";
    }
    else {
        std::string label = get_function_label(fs);
        fout << label << ":\n";
    }

    //setup function stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";
    
    for(int i = 0; i < parameters.size(); i++){
        // - does parameter correspond to existing type?
        if(!is_type_declared(parameters[i]->type)) {
            std::cout << "Undeclared type : " << parameters[i]->type->to_string() << "\n";
            return false;
        }
        // - is parameter type not void?
        if(*(parameters[i]->type) == BaseType("void")) {
            std::cout << "Parameter can't have type void\n";
            return false;
        }
    }

    // - is return type of function existing?
    if(!is_type_declared(type)) {
        std::cout << "Function undeclared return type : " << type->to_string() << " " << id->name << "\n";
        return false;
    }
    
    //if has enclosing type, register self as variable (Type* this)
    local_offset = 8 + 8 * parameters.size();
    if(enclosing_type.has_value()) {
        //adjust local offset for 'extra variable'
        local_offset += 8;

        //register self as variable (Type this)
        Type *vt = enclosing_type.value();
        Identifier *vid = new Identifier("this");
        Variable* v = add_variable(vt, vid);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << vt << " " << vid << "\n";
            return false;
        }
        v->stack_offset = local_offset;
        local_offset -= 8;
    }
    for(int i = 0; i < parameters.size(); i++){
        Variable* v = add_variable(parameters[i]->type, parameters[i]->id);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << parameters[i]->type->to_string() << " " << parameters[i]->id->name << "\n";
            return false;
        }
        v->stack_offset = local_offset;
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
    if(*type != BaseType("void") && !is_constructor) {
        // from any point of runnable code, we expect that it should be able to reach a return statement. 
        // this effectively means that the last statement should always be a return statement, regardless of 
        // the rest of the code, as the statement right before it needs to be able to return. 

        // once we introduce conditionals, this logic is going to change. For example
        // if(<condition>) {}
        // else {}
        // should not have to be followed with a return statement if there is a return statement in 
        // the <if> and <else> blocks

        // define a compound statement to be 'always returning' if when code execution reaches it, it's guaranteed
        // to return, regardless of whatever values might be in the variables. 

        // In order for a compound statement to be 'always returning', it just have to have some statement inside of it that is
        // 'always returning'. so we just need to check if the function body is 'always returning'. 

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


OperatorOverload::OperatorOverload(std::string _op, std::optional<Type*> _enclosing_type, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body) : Function(_enclosing_type, _type, _id, _parameters, _body) {
    op = _op;
}

OperatorSignature* OperatorOverload::resolve_operator_signature() const {
    if( op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || 
        op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>" ||
        op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || 
        op == ">=" || op == "=") {
        //binary operator
        if(this->parameters.size() != 2) return nullptr;
        Type *left = this->parameters[0]->type, *right = this->parameters[1]->type;
        return new OperatorSignature(left, op, right);
    }
    else if(op == "++x" || op == "--x" || op == "*x" || op == "(cast)") {
        //prefix operator
        if(this->parameters.size() != 1) return nullptr;
        Type *right = this->parameters[0]->type;
        if(op == "++x") return new OperatorSignature("++", right);
        else if(op == "--x") return new OperatorSignature("--", right);
        else if(op == "*x") return new OperatorSignature("*x", right);
        else if(op == "(cast)") return new OperatorSignature(right, this->type);
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
    else assert(false);
}