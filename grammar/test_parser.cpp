// Date Generated : 05-10-2025
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>

// -- PARSE CONTROLLER --
//the grammar to be parsed
std::string s;

//where we are in the string
int ptr;

//this is so we know where to backtrack to
std::stack<int> ptr_stack;

//use before trying an optional grammar rule
void push_stack() {
    ptr_stack.push(ptr);
}

//use when grammar rule fails to parse
void pop_stack() {
    assert(ptr_stack.size() != 0);
    ptr = ptr_stack.top();
    ptr_stack.pop();
}

//use when grammar rule parses successfully. 
void rm_stack() {
    assert(ptr_stack.size() != 0);
    ptr_stack.pop();
}

//the stack should be unaffected by any parse function. 

char next_char() {
    if(ptr >= s.size()) return '\0';
    return s[ptr ++];
}

std::string next_chars(int n) {
    assert(n > 0);
    if(ptr + n > s.size()) return "";
    std::string ans = s.substr(ptr, n);
    ptr += n;
    return ans;
}

struct dylan;

// dylan = "jank" ;
struct dylan {
    std::string t0;
    dylan(std::string _t0) {
        t0 = _t0;
    }
    static dylan* parse();
};

dylan* dylan::parse() {
    push_stack();
    std::string _t0 = next_chars(4);
    if(_t0 != "jank") {pop_stack(); return nullptr;}
    rm_stack();
    return new dylan(_t0);
}
std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    if(!file) throw std::runtime_error("Failed to open file : " + filename);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    std::string filename;
    std::cin >> filename;
    s = read_file(filename);
    std::cout << "PARSING\n";
    ptr = 0;
    dylan *x = dylan::parse();
    assert(ptr_stack.size() == 0);
    if(x == nullptr) {std::cout << "FAILED\n"; return 0;}
    std::cout << "SUCCESS\n";
    return 0;
}