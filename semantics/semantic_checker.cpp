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

struct Type;
struct Variable;
struct Function;
struct Expression;
struct Statement;
struct CompoundStatement;
struct Declaration;

struct Type {
    std::string name;
    Type(std::string _name) {
        name = _name;
    }
    Type(parser::identifier *i) {
        name = i->to_string();
    }
};

struct Variable {
    std::string name;
    Type *type;
};

struct Expression {
    

    Expression(parser::expression *e) {

    }
};

struct Declaration {
    Type* type;
    std::string name;
    Expression* expr;

    Declaration(parser::declaration *d) {
        type = new Type(d->t0);
        name = d->t2->to_string();
        expr = new Expression(d->t6);
    }
};

struct Statement {
    //for now, all statements are declarations
    Declaration *declaration;

    Statement(parser::statement *s) {
        declaration = new Declaration(s->t0);
    }
};

struct CompoundStatement {
    std::vector<std::variant<Statement*, CompoundStatement*>> statements;

    CompoundStatement(parser::compound_statement *s) {
        std::vector<parser::compound_statement::a0*> slist = s->t2;
        for(int i = 0; i < slist.size(); i++){
            parser::compound_statement::a0* tmp0 = slist[i];
            if(tmp0->t0->is_c0) {   //statement
                statements.push_back(new Statement(tmp0->t0->t0->t0));
            }
            else {  //compound statement
                statements.push_back(new CompoundStatement(tmp0->t0->t1->t0));
            }
        }
    }
};

struct Function {
    struct Parameter {
        Type *type;
        std::string name;
        
        Parameter(parser::parameter *p) {
            type = new Type(p->t0);
            name = p->t2->to_string();
        }
    };

    std::string name;
    Type *out_type;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;

    Function(parser::function *f) {
        parser::function_definition *def = f->t0;
        parser::parameter_list *pl = def->t6;
        name = def->t0->to_string();
        out_type = new Type(def->t2);
        if(pl->t0 != nullptr) {
            parameters.push_back(new Parameter(pl->t0->t0));
            std::vector<parser::parameter_list::a0::b0*> tmp = pl->t0->t1;
            for(int i = 0; i < tmp.size(); i++){
                parameters.push_back(new Parameter(tmp[i]->t3));
            }
        }
        body = new CompoundStatement(f->t2);
    }

    Function(std::string _name, Type *_out_type, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
        name = _name;
        out_type = _out_type;
        parameters = _parameters;
        body = _body;
    }
};

std::vector<Function*> get_defined_functions(parser::program *p) {
    std::vector<Function*> ret;
    for(int i = 0; i < p->t0.size(); i++){
        Function *f = new Function(p->t0[i]->t1);
        ret.push_back(f);
    }
    return ret;
}

bool is_statement_well_formed() {

}

bool is_function_well_formed(Function* f, std::vector<Function*> defined_functions) {

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
    std::string code = read_file(filename);
    parser::s = code;
    parser::ptr = 0;
    parser::program *p = parser::program::parse();
    if(parser::ptr != code.size()) {
        std::cout << "SYNTAX ERROR\n";
        return 1;
    }
    std::cout << "SYNTAX PASS\n";

    std::cout << "FUNCTION DEFINITIONS : \n";
    std::vector<Function*> functions = get_defined_functions(p);
    for(int i = 0; i < functions.size(); i++){
        std::cout << "NAME : " << functions[i]->name << ", TYPE : " << functions[i]->out_type->name << ", PARAMS :\n";
        for(int j = 0; j < functions[i]->parameters.size(); j++){
            Parameter *param = functions[i]->parameters[j];
            std::cout << param->type->name << " " << param->name << "\n";
        }
        std::cout << "NR STATEMENTS : " << functions[i]->body->statements.size() << "\n";
    }
    
    return 0;
}