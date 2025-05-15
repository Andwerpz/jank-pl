#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <optional>
#include "jank_parser.cpp"

/*
Takes in a .jank file, generates AST using jank_parser, then makes sure the program is well formed 
 - ensure every variable used is declared
 - do type checking for operators and assignments
 - verify function signatures are obeyed when calling them
 - check for existence of return statements in non-void functions. 

If the program is well formed, will spit out equivalent assembly code

For now, I will only support int as a valid type

will have two functions for checking, resolve_type() and is_well_formed(). 
resolve_type() returns nullptr if not well formed
these functions will only work with the proper context from declared_functions and declared_variables. 

TODO 
 - figure out how to nicely handle type converting operations. Maybe just keep a big list of what operations
   are valid, like (int + int = int) and (int * float = float) are valid but (string * string) is invalid. 

*/

// -- STRUCT DEFS --
struct Literal;
struct IntegerLiteral;
struct Identifier;
struct Type;
struct Expression;
struct Declaration;
struct Statement;
struct DeclarationStatement;
struct FunctionCallStatement;
struct ReturnStatement;
struct CompoundStatement;
struct Function;
struct FunctionSignature;
struct FunctionCall;
struct Program;

struct Variable;

struct Literal {
    static Literal* convert(parser::literal *n);
    virtual Type* resolve_type() = 0;
};

struct IntegerLiteral : public Literal {
    int val;
    IntegerLiteral(int _val) {
        val = _val;
    }
    Type* resolve_type() override;
};

struct Identifier {
    std::string name;
    Identifier(std::string _name) {
        name = _name;
    }
    static Identifier* convert(parser::identifier *i);

    bool operator==(const Identifier& other) {
        return name == other.name;
    }
    bool operator!=(const Identifier& other) {
        return !(*this == other);
    }
};

struct Type {
    std::string name;
    Type(std::string _name) {
        name = _name;
    }
    static Type* convert(parser::type *t);

    bool operator==(const Type& other) {
        return name == other.name;
    }
    bool operator!=(const Type& other) {
        return !(*this == other);
    }
};

struct Expression {
    struct Factor {
        using val_t = std::variant<FunctionCall*, Identifier*, Literal*, Expression*>;
        std::string unary_op = "+";
        val_t val;
        Factor(std::string _unary_op, val_t _val) {
            unary_op = _unary_op;
            val = _val;
        }
        static Factor* convert(parser::expr_factor *f);
        Type* resolve_type();
    };

    struct Term {
        std::vector<Factor*> factors;
        std::vector<std::string> operators;
        Term(std::vector<Factor*> _factors, std::vector<std::string> _operators) {
            factors = _factors;
            operators = _operators;
        }
        static Term* convert(parser::expr_term *t);
        Type* resolve_type();
    };

    std::vector<Term*> terms;
    std::vector<std::string> operators;
    Expression(std::vector<Term*> _terms, std::vector<std::string> _operators) {
        terms = _terms;
        operators = _operators;
    }
    static Expression* convert(parser::expression *e);
    Type* resolve_type();
};

struct Declaration {
    Type *type;
    Identifier *id;
    Expression *expr;
    Declaration(Type *_type, Identifier *_id, Expression *_expr) {
        type = _type;
        id = _id;
        expr = _expr;
    }
    static Declaration* convert(parser::declaration *d);
    bool is_well_formed();
};

struct Statement {
    static Statement* convert(parser::statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
    virtual bool has_nonvoid_return() = 0;
};

struct DeclarationStatement : public Statement {
    Declaration *declaration;
    DeclarationStatement(Declaration *_declaration) {
        declaration = _declaration;
    }
    bool is_well_formed() override;
    bool is_always_returning() override;
    bool has_nonvoid_return() override;
};

struct FunctionCallStatement : public Statement {
    FunctionCall *function_call;
    FunctionCallStatement(FunctionCall *_function_call) {
        function_call = _function_call;
    }
    bool is_well_formed() override;
    bool is_always_returning() override;
    bool has_nonvoid_return() override;
};

struct ReturnStatement : public Statement {
    std::optional<Expression*> opt_expr;
    ReturnStatement(Expression* expr) {
        if(expr == nullptr) opt_expr = std::nullopt;
        else opt_expr = expr;
    }
    bool is_well_formed() override;
    bool is_always_returning() override;
    bool has_nonvoid_return() override;
};

struct CompoundStatement {
    std::vector<std::variant<Statement*, CompoundStatement*>> statements;
    CompoundStatement(std::vector<std::variant<Statement*, CompoundStatement*>> _statements) {
        statements = _statements;
    }
    static CompoundStatement* convert(parser::compound_statement *s);
    bool is_well_formed();
    bool is_always_returning();
    bool has_nonvoid_return();
};

//name and input types. Does not include return type
struct FunctionSignature {
    Identifier *id;
    std::vector<Type*> input_types;
    FunctionSignature(Identifier *_id, std::vector<Type*> _input_types) {
        id = _id;
        input_types = _input_types;
    }
    
    bool operator==(FunctionSignature& other) {
        if(*id != *(other.id)) return false;
        if(input_types != other.input_types) return false;
        return true;
    }
    bool operator!=(FunctionSignature& other) {
        return !(*this == other);
    }
};

struct Function {
    struct Parameter {
        Type *type;
        Identifier *id;
        Parameter(Type *_type, Identifier *_id) {
            type = _type;
            id = _id;
        }
        static Parameter* convert(parser::parameter *p);
    };

    Identifier *id;
    Type *type;
    FunctionSignature *fs;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;
    Function(Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
        std::vector<Type*> input_types;
        for(int i = 0; i < _parameters.size(); i++) input_types.push_back(_parameters[i]->type);
        fs = new FunctionSignature(_id, input_types);
        id = _id;
        type = _type;
        parameters = _parameters;
        body = _body;
    }   
    static Function* convert(parser::function *f);
    bool is_well_formed();

    //just checks if the function signature matches
    bool operator==(Function& other) {
        return *fs == *(other.fs);
    }
    bool operator!=(Function& other) {
        return !(*this == other);
    }
};

struct FunctionCall {
    Identifier *id;
    std::vector<Expression*> argument_list;
    FunctionCall(Identifier *_id, std::vector<Expression*> _argument_list) {
        id = _id;
        argument_list = _argument_list;
    }
    static FunctionCall* convert(parser::function_call *f);
    Type* resolve_type();
    FunctionSignature* resolve_function_signature();
};

//right now, a program is just a collection of functions. 
//Later, we'll introduce user defined types (structs)
struct Program {
    std::vector<Function*> functions;
    Program(std::vector<Function*> _functions) {
        functions = _functions;
    }
    static Program* convert(parser::program *p);
    bool is_well_formed();
};

// -- SEMANTIC ANALYSIS CONTROLLER --
struct Variable {
    Type *type;
    Identifier *id;
    Variable(Type *_type, Identifier *_id) {
        id = _id;
        type = _type;
    }
};

Function* enclosing_function;
std::vector<Type*> declared_types;
std::vector<Function*> declared_functions;
std::vector<Variable*> declared_variables;
std::stack<std::vector<Variable*>> declaration_stack;

Type* find_variable_type(Identifier *id) {
    for(int i = 0; i < declared_variables.size(); i++){
        if(*(declared_variables[i]->id) == *id) return declared_variables[i]->type;
    }
    return nullptr;
}

Type* find_function_type(FunctionSignature *fs) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(*(declared_functions[i]->fs) == *fs) return declared_functions[i]->type;
    }
    return nullptr;
}

bool is_type_declared(Type *t) {
    for(int i = 0; i < declared_types.size(); i++){
        if(*(declared_types[i]) == *t) return true;
    }
    return false;
}

bool is_function_declared(FunctionSignature *fs) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(*fs == *(declared_functions[i]->fs)) {
            return true;
        }
    }
    return false;
}

Function* get_function(FunctionSignature *fs) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(*fs == *(declared_functions[i]->fs)) {
            return declared_functions[i];
        }
    }
    return nullptr;
}

//checks against all variables, functions
bool is_identifier_used(Identifier *id) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(*id == *(declared_functions[i]->id)) return false;
    }
    for(int i = 0; i < declared_variables.size(); i++){
        if(*id == *(declared_variables[i]->id)) return false;
    }
    return true;
}

bool add_variable(Type *t, Identifier *id) {
    assert(declaration_stack.size() != 0);
    if(is_identifier_used(id)) return false;
    Variable *v = new Variable(t, id);
    declared_variables.push_back(v);
    declaration_stack.top().push_back(v);
    return true;
}

void remove_variable(Identifier *id) {
    int ind = -1;
    for(int i = 0; i < declared_variables.size(); i++){
        if(*id == *(declared_variables[i]->id)) {
            ind = i;
            break;
        }
    }
    assert(ind != -1);
    declared_variables.erase(declared_variables.begin() + ind);
}

void push_declaration_stack() {
    declaration_stack.push(std::vector<Variable*>(0));
}

void pop_declaration_stack() {
    assert(declaration_stack.size() != 0);
    std::vector<Variable*> top = declaration_stack.top();
    declaration_stack.pop();
    for(int i = 0; i < top.size(); i++){
        remove_variable(top[i]->id);
    }
}

// -- STRUCT FUNCTIONS --

Literal* Literal::convert(parser::literal *n) {
    //for now, only integer literals are supported by the grammar
    return new IntegerLiteral(stoi(n->to_string()));
}

Identifier* Identifier::convert(parser::identifier *i) {
    return new Identifier(i->to_string());
}

Type* Type::convert(parser::type *t) {
    return new Type(t->to_string());
}

Expression::Factor* Expression::Factor::convert(parser::expr_factor *f){
    std::string unary_op = "+";
    if(f->t0 != nullptr) {
        //we have an unary operator
        parser::expr_factor::a0 *unary = f->t0;
        if(unary->is_b0) {
            unary_op = unary->t0->t0;
        }
        else if(unary->is_b1) {
            unary_op = unary->t1->t0;
        }
        else assert(false);
    }
    
    //parse main body 
    parser::expr_factor::a1 *body = f->t1;
    Expression::Factor::val_t val;
    if(body->is_b0) {
        val = FunctionCall::convert(body->t0->t0);
    }
    else if(body->is_b1){
        val = Identifier::convert(body->t1->t0);
    }
    else if(body->is_b2) {
        val = Literal::convert(body->t2->t0);
    }
    else if(body->is_b3) {
        val = Expression::convert(body->t3->t2);
    }
    else assert(false);
    return new Expression::Factor(unary_op, val);
}

Expression::Term* Expression::Term::convert(parser::expr_term *t) {
    std::vector<Factor*> factors;
    std::vector<std::string> operators;
    factors.push_back(Expression::Factor::convert(t->t0));
    for(int i = 0; i < t->t1.size(); i++){
        operators.push_back(t->t1[i]->t1->to_string());
        factors.push_back(Expression::Factor::convert(t->t1[i]->t3));
    }
    return new Expression::Term(factors, operators);
}

Expression* Expression::convert(parser::expression *e) {
    std::vector<Term*> terms;
    std::vector<std::string> operators;
    terms.push_back(Expression::Term::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        operators.push_back(e->t1[i]->t1->to_string());
        terms.push_back(Expression::Term::convert(e->t1[i]->t3));
    }
    return new Expression(terms, operators);
}

Declaration* Declaration::convert(parser::declaration *d) {
    Type *type = Type::convert(d->t0);
    Identifier *name = Identifier::convert(d->t2);
    Expression *expr = Expression::convert(d->t6);
    return new Declaration(type, name, expr);
}

Statement* Statement::convert(parser::statement *s) {
    if(s->is_a0) {  //declaration
        Declaration *declaration = Declaration::convert(s->t0->t0);
        return new DeclarationStatement(declaration);
    }
    else if(s->is_a1) { //function call
        FunctionCall *function_call = FunctionCall::convert(s->t1->t0);
        return new FunctionCallStatement(function_call);
    }
    else if(s->is_a2) { //return 
        Expression *expr = nullptr;
        if(s->t2->t1 != nullptr) {
            //non-void return
            expr = Expression::convert(s->t2->t1->t1);
        }
        return new ReturnStatement(expr);
    }
    else assert(false);
}

CompoundStatement* CompoundStatement::convert(parser::compound_statement *s) {
    std::vector<std::variant<Statement*, CompoundStatement*>> statements;
    std::vector<parser::compound_statement::a0*> slist = s->t2;
    for(int i = 0; i < slist.size(); i++){
        parser::compound_statement::a0* tmp0 = slist[i];
        if(tmp0->t0->is_c0) {   //statement
            statements.push_back(Statement::convert(tmp0->t0->t0->t0));
        }
        else {  //compound statement
            statements.push_back(CompoundStatement::convert(tmp0->t0->t1->t0));
        }
    }
    return new CompoundStatement(statements);
}

Function::Parameter* Function::Parameter::convert(parser::parameter *p) {
    Type *type = Type::convert(p->t0);
    Identifier* name = Identifier::convert(p->t2);
    return new Parameter(type, name);
}

Function* Function::convert(parser::function *f) {
    parser::function_definition *def = f->t0;
    parser::parameter_list *pl = def->t6;
    Type *type = Type::convert(def->t0);
    Identifier *name = Identifier::convert(def->t2);
    std::vector<Function::Parameter*> parameters;
    if(pl->t0 != nullptr) {
        parameters.push_back(Function::Parameter::convert(pl->t0->t0));
        std::vector<parser::parameter_list::a0::b0*> tmp = pl->t0->t1;
        for(int i = 0; i < tmp.size(); i++){
            parameters.push_back(Function::Parameter::convert(tmp[i]->t3));
        }
    }
    CompoundStatement *body = CompoundStatement::convert(f->t2);
    return new Function(type, name, parameters, body);
}

FunctionCall* FunctionCall::convert(parser::function_call *f) {
    Identifier *fname = Identifier::convert(f->t0);
    std::vector<Expression*> argument_list;
    parser::argument_list *arglist = f->t4;
    if(arglist->t0 != nullptr) {
        argument_list.push_back(Expression::convert(arglist->t0->t0));
        for(int i = 0; i < arglist->t0->t1.size(); i++){
            argument_list.push_back(Expression::convert(arglist->t0->t1[i]->t3));
        }
    }
    return new FunctionCall(fname, argument_list);
}

Program* Program::convert(parser::program *p) {
    std::vector<Function*> functions;
    for(int i = 0; i < p->t0.size(); i++){
        functions.push_back(Function::convert(p->t0[i]->t1));
    }
    return new Program(functions);
}

Type* IntegerLiteral::resolve_type() {
    return new Type("int");
}

Type* Expression::Factor::resolve_type() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *f = std::get<FunctionCall*>(val);
        return f->resolve_type();
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        return find_variable_type(id);
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return l->resolve_type();
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->resolve_type();
    }
    else assert(false);
}   

Type* Expression::Term::resolve_type() {
    if(factors.size() == 0) {
        return nullptr;
    }
    assert(factors.size() - 1 == operators.size());
    std::vector<Type*> types;
    for(int i = 0; i < factors.size(); i++){
        types.push_back(factors[i]->resolve_type());
    }
    //just make sure all the types are equal for now
    for(int i = 0; i < factors.size() - 1; i++){
        if(*(types[i]) != *(types[i + 1])) return nullptr;
    }
    return types[0];
}

Type* Expression::resolve_type() {
    if(terms.size() == 0) {
        return nullptr;
    }
    assert(terms.size() - 1 == operators.size());
    std::vector<Type*> types;
    for(int i = 0; i < terms.size(); i++){
        types.push_back(terms[i]->resolve_type());
    }
    //just make sure all the types are equal for now
    for(int i = 0; i < terms.size() - 1; i++){
        if(*(types[i]) != *(types[i + 1])) return nullptr;
    }
    return types[0];
}

Type* FunctionCall::resolve_type() {
    FunctionSignature *fs = resolve_function_signature();
    if(fs == nullptr) {
        return nullptr;
    }
    Function *f = get_function(fs);
    if(f == nullptr) {
        return nullptr;
    }
    return f->type;
}

FunctionSignature* FunctionCall::resolve_function_signature() {
    //for each argument, figure out its type
    std::vector<Type*> types;
    for(int i = 0; i < argument_list.size(); i++){
        Type *t = argument_list[i]->resolve_type();
        if(t == nullptr) return nullptr;
        types.push_back(t);
    }
    return new FunctionSignature(id, types);
}

bool DeclarationStatement::has_nonvoid_return() {
    return false;
}

bool FunctionCallStatement::has_nonvoid_return() {
    return false;
}

bool ReturnStatement::has_nonvoid_return() {
    return opt_expr.has_value();
}

bool CompoundStatement::has_nonvoid_return() {
    for(int i = 0; i < statements.size(); i++){
        if(std::holds_alternative<Statement*>(statements[i])) {
            Statement *s = std::get<Statement*>(statements[i]);
            if(s->has_nonvoid_return()) {
                return true;
            }
        }
        else if(std::holds_alternative<CompoundStatement*>(statements[i])) {
            CompoundStatement *cs = std::get<CompoundStatement*>(statements[i]);
            if(cs->has_nonvoid_return()) {
                return true;
            }
        }
        else assert(false);
    }
    return false;
}

bool DeclarationStatement::is_always_returning() {
    return false;
}

bool FunctionCallStatement::is_always_returning() {
    return false;
}

bool ReturnStatement::is_always_returning() {
    return true;
}

bool CompoundStatement::is_always_returning() {
    for(int i = 0; i < statements.size(); i++){
        if(std::holds_alternative<Statement*>(statements[i])) {
            Statement *s = std::get<Statement*>(statements[i]);
            if(s->is_always_returning()) {
                return true;
            }
        }
        else if(std::holds_alternative<CompoundStatement*>(statements[i])) {
            CompoundStatement *cs = std::get<CompoundStatement*>(statements[i]);
            if(cs->is_always_returning()) {
                return true;
            }
        }
        else assert(false);
    }
    return false;
}

bool Declaration::is_well_formed() {
    // - is the type being used declared?
    if(!is_type_declared(type)) {
        std::cout << "Declaration using undeclared type : " << type->name << "\n";
        return false;
    }
    // - does the expression resolve to a type?
    Type *expr_type = expr->resolve_type();
    if(expr_type == nullptr || !is_type_declared(expr_type)) {
        std::cout << "Expression does not resolve to existing type\n";
        return false;
    }
    // - does the type of the expression match with the type being used?
    if(*expr_type != *type) {
        std::cout << "Declaration and expression type mismatch\n";
        return false;
    }
    // - is the identifier being used already taken?
    if(!add_variable(type, id)) {
        std::cout << "Identifier already used : " << id->name << "\n";
        return false;
    }
    return true;
}

bool DeclarationStatement::is_well_formed() {
    // - is the declaration well formed?
    if(!declaration->is_well_formed()) {
        std::cout << "Declaration not well formed\n";
        return false;
    }
    return true;
}

bool FunctionCallStatement::is_well_formed() {
    // - does the function call resolve properly to a type?
    FunctionSignature *fs = function_call->resolve_function_signature();
    if(fs == nullptr) {
        std::cout << "Unable to resolve function signature\n";
        return false;
    }
    // - is the function we're referencing actually declared?
    if(!is_function_declared(fs)) {
        std::cout << "Reference to undeclared function : " << fs->id << "\n";
        return false;
    }
    return true;
}

bool ReturnStatement::is_well_formed() {
    // - does the expression resolve to a type?
    Type *t = nullptr;
    if(opt_expr.has_value()) {
        Expression *expr = opt_expr.value();
        t = expr->resolve_type();
    }
    else t = new Type("void");
    if(t == nullptr) {
        std::cout << "Return expression does not resolve to type\n";
        return false;
    }
    // - does the expression return type match the return type of the enclosing function?
    if(*t != *(enclosing_function->type)) {
        std::cout << "Return type does not match enclosing function\n";
        return false;
    }
    return true;
}

bool CompoundStatement::is_well_formed() {
    push_declaration_stack();

    // - are all statements within well formed?
    for(int i = 0; i < statements.size(); i++){
        if(std::holds_alternative<Statement*>(statements[i])) {
            Statement *s = std::get<Statement*>(statements[i]);
            if(!s->is_well_formed()) {
                return false;
            }
        }
        else if(std::holds_alternative<CompoundStatement*>(statements[i])) {
            CompoundStatement *cs = std::get<CompoundStatement*>(statements[i]);
            if(!cs->is_well_formed()) {
                return false;
            }
        }
        else assert(false);
    }

    pop_declaration_stack();

    return true;
}

bool Function::is_well_formed() {
    push_declaration_stack();

    // - do all parameters correspond to existing types?
    for(int i = 0; i < parameters.size(); i++){
        if(!is_type_declared(parameters[i]->type)) {
            std::cout << "Undeclared type : " << parameters[i]->type->name << "\n";
            return false;
        }
    }

    //register parameters as variables
    for(int i = 0; i < parameters.size(); i++){
        if(!add_variable(parameters[i]->type, parameters[i]->id)) {
            std::cout << "Identifier already used : " << parameters[i]->id << "\n";
            return false;
        }
    }

    // - make sure body is well formed
    if(!body->is_well_formed()) {
        std::cout << "Function body not well formed\n";
        return false;
    }
    
    if(*type != Type("void")) {
        // - if type is not void, check for existence of return statement as last reachable statement

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
        // - if type is void, check that all the return statements are empty, if they exist
        if(body->has_nonvoid_return()) {
            std::cout << "Function body has non-void return in void function\n";
            return false;
        }
    }

    //unregister parameters as variables
    pop_declaration_stack();

    return true;
}

bool Program::is_well_formed() {
    enclosing_function = nullptr;
    declared_functions.clear();
    declared_types.clear();
    declared_variables.clear();
    while(declaration_stack.size()) declaration_stack.pop();

    push_declaration_stack();

    // - are there any duplicate function definitions?
    for(int i = 0; i < functions.size(); i++){
        for(int j = i + 1; j < functions.size(); j++) {
            if(*(functions[i]) == *(functions[j])) {
                std::cout << "Duplicate function definitions : " << functions[i]->id->name << "\n";
                return false;
            }
        }
    }
    for(int i = 0; i < functions.size(); i++) {
        declared_functions.push_back(functions[i]);
    }

    // - there must be a function with function signature 'int main()'
    // TODO

    //collect all type definitions. For now, just the primitive types are defined
    declared_types.push_back(new Type("int"));

    // - make sure every function is well formed
    for(int i = 0; i < functions.size(); i++){
        enclosing_function = functions[i];
        if(!functions[i]->is_well_formed()) {
            std::cout << "Function not well formed : " << functions[i]->id->name << "\n";
            return false;
        }
    }

    pop_declaration_stack();

    assert(declaration_stack.size() == 0);
    assert(declared_variables.size() == 0);

    return true;
}

std::string read_file(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::ostringstream buffer;
    buffer << file.rdbuf(); 
    return buffer.str();     
}

std::string read_cstr(char* s) {
    std::string ans = "";
    int ptr = 0;
    while(s[ptr] != '\0') {
        ans.push_back(s[ptr ++]);
    }
    return ans;
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "USAGE : <filename>\n";
        std::cout << "<filename> must end with \".jank\"\n";
        return 1;
    }
    int argptr = 1;
    std::string filename = read_cstr(argv[argptr ++]);
    assert(filename.size() >= 5 && filename.substr(filename.size() - 5) == ".jank");

    Program *program = nullptr;
    {
        std::string code = read_file(filename);
        parser::s = code;
        parser::ptr = 0;
        parser::program *p = parser::program::parse();
        if(parser::ptr != code.size()) {
            std::cout << "SYNTAX ERROR\n";
            return 1;
        }
        std::cout << "SYNTAX PASS\n";
        program = Program::convert(p);
    }
   
    std::cout << "FUNCTION DEFINITIONS : \n";
    for(int i = 0; i < program->functions.size(); i++){
        std::cout << "NAME : " << program->functions[i]->id->name << ", TYPE : " << program->functions[i]->type->name << ", PARAMS :\n";
        for(int j = 0; j < program->functions[i]->parameters.size(); j++){
            Function::Parameter *param = program->functions[i]->parameters[j];
            std::cout << param->type->name << " " << param->id->name << "\n";
        }
        std::cout << "NR STATEMENTS : " << program->functions[i]->body->statements.size() << "\n";
    }

    if(!program->is_well_formed()) {
        std::cout << "Program not well formed\n";
        return 1;
    }
    std::cout << "Program is well formed\n";
    
    return 0;
}