#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
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

struct Type {
    std::string name;
};

struct Variable {
    std::string name;
    Type *type;
};

struct Function {
    parser::function *parse_function;
    std::string name;
    std::vector<Type*> parameters;

    Function(parser::function *_parse_function) {
        parse_function = _parse_function;
        
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

    
    std::cout << p->to_string() << "\n";
    
    return 0;
}