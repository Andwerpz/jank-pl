#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <variant>
#include "jank_parser.cpp"

/*
Takes in a .jank file, generates AST using jank_parser, then makes sure the program is well formed 
 - ensure every variable used is declared
 - do type checking for operators and assignments
 - verify function signatures are obeyed when calling them
 - check for existence of return statements in non-void functions. 

If the program is well formed, will spit out equivalent assembly code

For now, I will only support int and string as valid types. Will have to figure out a good way to handle types, and 
operations between types.
*/

struct Number;
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
struct FunctionCall;
struct Program;

struct Variable;

// SEMANTIC ANALYSIS CONTROLLER (or smth like that)
std::vector<Function*> declared_functions;
std::vector<Variable*> declared_variables;

//assuming all numbers are actually integers. We'll consider floats later
struct Number {
    int val;
    Number(int _val) {
        val = _val;
    }
    static Number* convert(parser::number *n);
};

struct Identifier {
    std::string name;
    Identifier(std::string _name) {
        name = _name;
    }
    static Identifier* convert(parser::identifier *i);
};

struct Type {
    std::string name;
    Type(std::string _name) {
        name = _name;
    }
    static Type* convert(parser::type *t);
};

struct Expression {
    struct Factor {
        using val_t = std::variant<FunctionCall*, Identifier*, Number*, Expression*>;
        std::string unary_op = "+";
        val_t val;
        Factor(std::string _unary_op, val_t _val) {
            unary_op = _unary_op;
            val = _val;
        }
        static Factor* convert(parser::expr_factor *f);
    };

    struct Term {
        std::vector<Factor*> factors;
        std::vector<std::string> operators;
        Term(std::vector<Factor*> _factors, std::vector<std::string> _operators) {
            factors = _factors;
            operators = _operators;
        }
        static Term* convert(parser::expr_term *t);
    };

    std::vector<Term*> terms;
    std::vector<std::string> operators;
    Expression(std::vector<Term*> _terms, std::vector<std::string> _operators) {
        terms = _terms;
        operators = _operators;
    }
    static Expression* convert(parser::expression *e);
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
};

struct Statement {
    static Statement* convert(parser::statement *s);
};

struct DeclarationStatement : public Statement {
    Declaration *declaration;
    DeclarationStatement(Declaration *_declaration) {
        declaration = _declaration;
    }
};

struct FunctionCallStatement : public Statement {
    FunctionCall *function_call;
    FunctionCallStatement(FunctionCall *_function_call) {
        function_call = _function_call;
    }
};

struct ReturnStatement : public Statement {
    Expression *expr;
    ReturnStatement(Expression *_expr) {
        expr = _expr;
    }
};

struct CompoundStatement {
    std::vector<std::variant<Statement*, CompoundStatement*>> statements;
    CompoundStatement(std::vector<std::variant<Statement*, CompoundStatement*>> _statements) {
        statements = _statements;
    }
    static CompoundStatement* convert(parser::compound_statement *s);
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

    Type *type;
    Identifier *id;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;
    Function(Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
        id = _id;
        type = _type;
        parameters = _parameters;
        body = _body;
    }   
    static Function* convert(parser::function *f);
};

struct FunctionCall {
    Identifier *id;
    std::vector<Identifier*> argument_list;
    FunctionCall(Identifier *_id, std::vector<Identifier*> _argument_list) {
        id = _id;
        argument_list = _argument_list;
    }
    static FunctionCall* convert(parser::function_call *f);
};

//right now, a program is just a collection of functions. 
//Later, we'll introduce user defined types (structs)
struct Program {
    std::vector<Function*> functions;
    Program(std::vector<Function*> _functions) {
        functions = _functions;
    }
    static Program* convert(parser::program *p);
};

Number* Number::convert(parser::number *n) {
    return new Number(stoi(n->to_string()));
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
        val = Number::convert(body->t2->t0);
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
        Expression *expr = Expression::convert(s->t2->t2);
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
    std::vector<Identifier*> argument_list;
    parser::argument_list *arglist = f->t4;
    if(arglist->t0 != nullptr) {
        argument_list.push_back(Identifier::convert(arglist->t0->t0));
        for(int i = 0; i < arglist->t0->t1.size(); i++){
            argument_list.push_back(Identifier::convert(arglist->t0->t1[i]->t3));
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

struct Variable {
    Identifier *id;
    Type *type;
    
    Variable(Identifier *_id, Type *_type) {
        id = _id;
        type = _type;
    }
};

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
    
    return 0;
}