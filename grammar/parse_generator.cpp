#include <bits/stdc++.h>
using namespace std;

/*
This program's job is to take in an EBNF grammar and spit out a parser for that grammar
The EBNF grammar this parser supports is defined in ebnf.ebnf
*/

// -- STRUCT DEFS --
struct letter;
struct digit;
struct symbol;
struct escape;
struct identifier;
struct comment_char;
struct comment;
struct wspace;  //ws is already used by std
struct rwspace;
struct owspace;
struct terminal_char;
struct terminal;
struct term;
struct concatenation;
struct alternation;
struct rule;
struct include;
struct grammar;

struct letter {
    char c;
    letter(char _c) {
        c = _c;
    }
};

struct digit {
    char c;
    digit(char _c) {
        c = _c;
    }
};

struct symbol {
    char c;
    symbol(char _c) {
        c = _c;
    }
};

struct escape {
    char c;
    escape(char _c) {
        c = _c;
    }
};

struct identifier {
    vector<letter*> ls;
    identifier(vector<letter*> _ls){
        ls = _ls;
    }
};

struct comment_char {
    bool is_letter = false;
    letter *l;
    comment_char(letter *_l) {
        is_letter = true;
        l = _l;
    }

    bool is_digit = false;
    digit *d;
    comment_char(digit *_d) {
        is_digit = true;
        d = _d;
    }

    bool is_wspace = false;
    wspace *w;
    comment_char(wspace *_w) {
        is_wspace = true;
        w = _w;
    }

    bool is_literal = false;
    char c;
    comment_char(char _c) {
        is_literal = true;
        c = _c;
    }
};

struct comment {
    vector<comment_char*> s;
    comment(vector<comment_char*> _s) {
        s = _s;
    }
};

struct wspace {
    bool is_comment = false;
    comment *com;
    wspace(comment* _com) {
        is_comment = true;
        com = _com;
    }

    bool is_literal = false;
    char c;
    wspace(char _c) {
        is_literal = true;
        c = _c;
    }
};

struct rwspace {
    vector<wspace*> ss;
    rwspace(vector<wspace*> _ss) {
        ss = _ss;
    }
};

struct owspace {
    vector<wspace*> ss;
    owspace(vector<wspace*> _ss) {
        ss = _ss;
    }
};

struct terminal_char {
    bool is_letter = false;
    letter *l;
    terminal_char(letter *_l) {
        is_letter = true;
        l = _l;
    }

    bool is_digit = false;
    digit *d;
    terminal_char(digit *_d) {
        is_digit = true;
        d = _d;
    }

    bool is_symbol = false;
    symbol *s;
    terminal_char(symbol *_s) {
        is_symbol = true;
        s = _s;
    }

    bool is_escape = false;
    escape *e;
    terminal_char(escape *_e) {
        is_escape = true;
        e = _e;
    }

    bool is_literal = false;
    char c;
    terminal_char(char _c) {
        is_literal = true;
        c = _c;
    }
};

struct terminal {
    vector<terminal_char*> cs;
    terminal(vector<terminal_char*> _cs) {
        cs = _cs;
    }
};

struct term {
    bool is_grouping = false;
    bool is_zo = false;
    bool is_zm = false;
    bool is_om = false;
    bool is_terminal = false;
    bool is_identifier = false;

    alternation *a;
    terminal *t;
    identifier *i;

    term(string type, alternation *_a) {
        if(type == "()") is_grouping = true;
        else if(type == "[]") is_zo = true;
        else if(type == "{}") is_zm = true;
        else if(type == "<>") is_om = true;
        else assert(false);
        a = _a;
    }

    term(terminal *_t){
        is_terminal = true;
        t = _t;
    }

    term(identifier *_i) {
        is_identifier = true;
        i = _i;
    }
};

struct concatenation {
    vector<term*> ts;
    concatenation(vector<term*> _ts) {
        ts = _ts;
    }
};

struct alternation {
    vector<concatenation*> cs;
    alternation(vector<concatenation*> _cs) {
        cs = _cs;
    }
};

struct rule {
    identifier *i;
    alternation *a;
    rule(identifier *_i, alternation *_a) {
        i = _i;
        a = _a;
    }
};

struct include {
    terminal *t;
    include(terminal *_t) {
        t = _t;
    }
};

struct grammar {
    vector<include*> is;
    vector<rule*> rs;
    grammar(vector<include*> _is, vector<rule*> _rs) {
        is = _is;
        rs = _rs;
    }
};

// -- PARSE CONTROLLER --
//the grammar to be parsed
string parse_str;

//where we are in the string
int parse_ptr;

//this is so we know where to backtrack to
stack<int> ptr_stack;

//use before trying an optional grammar rule
void push_stack() {
    ptr_stack.push(parse_ptr);
}

//use when grammar rule fails to parse
void pop_stack() {
    assert(ptr_stack.size() != 0);
    parse_ptr = ptr_stack.top();
    ptr_stack.pop();
}

//use when grammar rule parses successfully. 
void rm_stack() {
    assert(ptr_stack.size() != 0);
    ptr_stack.pop();
}

//the stack should be unaffected by any parse function. 

char next_char() {
    if(parse_ptr >= parse_str.size()) return '\0';
    return parse_str[parse_ptr ++];
}

string next_chars(int n) {
    assert(n > 0);
    if(parse_ptr + n > parse_str.size()) return "";
    string ans = parse_str.substr(parse_ptr, n);
    parse_ptr += n;
    return ans;
}

// -- PARSE FUNCTIONS --
bool parse_debug = false;
letter* parse_letter();
digit* parse_digit();
symbol* parse_symbol();
escape* parse_escape();
identifier* parse_identifier();
comment_char* parse_comment_char();
comment* parse_comment();
wspace* parse_wspace();
rwspace* parse_rwspace();
owspace* parse_owspace();
terminal_char* parse_terminal_char();
terminal* parse_terminal();
term* parse_term();
concatenation* parse_concatenation();
alternation* parse_alternation();
rule* parse_rule();
include* parse_include();
grammar* parse_grammar();

letter* parse_letter() {
    push_stack();
    char c = next_char();
    if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_')) {
        rm_stack();
        return new letter(c);
    }
    pop_stack();
    return nullptr;
}

digit* parse_digit() {
    push_stack();
    char c = next_char();
    if('0' <= c && c <= '9') {
        rm_stack();
        return new digit(c);
    }
    pop_stack();
    return nullptr;
}

symbol* parse_symbol() {
    push_stack();
    char c = next_char();
    bool is_valid = false;
    is_valid |= c == '[' || c == ']' || c == '{' || c == '}';
    is_valid |= c == '(' || c == ')' || c == '<' || c == '>';
    is_valid |= c == '\'' || c == '=' || c == '|' || c == '.';
    is_valid |= c == ',' || c == ';' || c == '-' || c == '+';
    is_valid |= c == '*' || c == '?' || c == ':' || c == '!';
    is_valid |= c == '@' || c == '#' || c == '$' || c == '%';
    is_valid |= c == '^' || c == '&' || c == '/' || c == '~';
    is_valid |= c == '`';
    if(is_valid) {
        rm_stack();
        return new symbol(c);
    }
    pop_stack();
    return nullptr;
}

escape* parse_escape() {
    push_stack();
    char a = next_char();
    if(a != '\\') {
        pop_stack();
        return nullptr;
    }
    char b = next_char();
    if(b == 'n' || b == 't' || b == 'r' || b == 'f' || b == 'b' || b == '\"' || b == '\\') {
        rm_stack();
        return new escape(b);
    }
    pop_stack();
    return nullptr;
}

identifier* parse_identifier() {
    push_stack();
    vector<letter*> ls;
    while(true) {
        letter *l = parse_letter();
        if(l == nullptr) break;
        ls.push_back(l);
    }
    if(ls.size() == 0) {
        pop_stack();
        return nullptr;
    }
    rm_stack();
    return new identifier(ls);
}

comment_char* parse_comment_char() {
    if(auto l = parse_letter()) return new comment_char(l);
    if(auto d = parse_digit()) return new comment_char(d);
    if(auto w = parse_wspace()) return new comment_char(w);
    {   //literal
        push_stack();
        char c = next_char();
        bool is_valid = false;
        is_valid |= c == '[' || c == ']' || c == '{' || c == '}';
        is_valid |= c == '(' || c == ')' || c == '<' || c == '>';
        is_valid |= c == '\'' || c == '=' || c == '|' || c == '.';
        is_valid |= c == ',' || c == ';' || c == '-' || c == '+';
        is_valid |= c == '?' || c == ':' || c == '!';
        is_valid |= c == '@' || c == '#' || c == '$' || c == '%';
        is_valid |= c == '^' || c == '&' || c == '/';
        is_valid |= c == '~' || c == '`' || c == '\\' || c == '\"';
        if(is_valid) {
            rm_stack();
            return new comment_char(c);
        }
        pop_stack();
    }
    return nullptr;
}

comment* parse_comment() {
    push_stack();
    if(next_chars(2) != "(*") {
        pop_stack();
        return nullptr;
    }
    vector<comment_char*> cs;
    while(true){
        comment_char *c = parse_comment_char();
        if(c == nullptr) break;
        cs.push_back(c);
    }
    if(next_chars(2) != "*)") {
        pop_stack();
        return nullptr;
    }
    rm_stack();
    return new comment(cs);
}

wspace* parse_wspace() {
    if(auto c = parse_comment()) return new wspace(c);
    {   //literal
        push_stack();
        char c = next_char();
        if(c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\b') {
            rm_stack();
            return new wspace(c);
        }
        pop_stack();
    }
    return nullptr;
}

rwspace* parse_rwspace() {
    push_stack();
    vector<wspace*> ss;
    while(true) {
        wspace *w = parse_wspace();
        if(w == nullptr) break;
        ss.push_back(w);
    }
    if(ss.size() == 0) {
        pop_stack();
        return nullptr;
    }
    rm_stack();
    return new rwspace(ss);
}

owspace* parse_owspace() {
    push_stack();
    vector<wspace*> ss;
    while(true) {
        wspace *w = parse_wspace();
        if(w == nullptr) break;
        ss.push_back(w);
    }
    rm_stack();
    return new owspace(ss);
}

terminal_char* parse_terminal_char() {
    if(auto l = parse_letter()) return new terminal_char(l);
    if(auto d = parse_digit()) return new terminal_char(d);
    if(auto s = parse_symbol()) return new terminal_char(s);
    if(auto e = parse_escape()) return new terminal_char(e);
    {   //literal
        push_stack();
        char c = next_char();
        if(c == ' ') {
            rm_stack();
            return new terminal_char(c);
        }
        pop_stack();
    }
    return nullptr;
}

terminal* parse_terminal() {
    push_stack();
    if(next_char() != '\"') {
        pop_stack();
        return nullptr;
    }
    vector<terminal_char*> cs;
    while(true){
        terminal_char* c = parse_terminal_char();
        if(c == nullptr) break;
        cs.push_back(c);
    }
    if(cs.size() == 0) {
        pop_stack();
        return nullptr;
    }
    if(next_char() != '\"') {
        pop_stack();
        return nullptr;
    }
    rm_stack();
    return new terminal(cs);
}

term* parse_term() {
    do {    // grouping
        push_stack();
        if(next_char() != '(') {
            pop_stack();
            break;
        }
        rwspace *ws1 = parse_rwspace();
        if(ws1 == nullptr) {
            pop_stack();
            break;
        }
        alternation* a = parse_alternation();
        if(a == nullptr) {
            pop_stack();
            break;
        }
        rwspace *ws2 = parse_rwspace();
        if(ws2 == nullptr) {
            pop_stack();
            break;
        }
        if(next_char() != ')') {
            pop_stack();
            break;
        }
        rm_stack();
        return new term("()", a);
    } while(false);
    do {    // zero or one
        push_stack();
        if(next_char() != '[') {
            pop_stack();
            break;
        }
        rwspace *ws1 = parse_rwspace();
        if(ws1 == nullptr) {
            pop_stack();
            break;
        }
        alternation* a = parse_alternation();
        if(a == nullptr) {
            pop_stack();
            break;
        }
        rwspace *ws2 = parse_rwspace();
        if(ws2 == nullptr) {
            pop_stack();
            break;
        }
        if(next_char() != ']') {
            pop_stack();
            break;
        }
        rm_stack();
        return new term("[]", a);
    } while(false);
    do {    // zero or more
        push_stack();
        if(next_char() != '{') {
            pop_stack();
            break;
        }
        rwspace *ws1 = parse_rwspace();
        if(ws1 == nullptr) {
            pop_stack();
            break;
        }
        alternation* a = parse_alternation();
        if(a == nullptr) {
            pop_stack();
            break;
        }
        rwspace *ws2 = parse_rwspace();
        if(ws2 == nullptr) {
            pop_stack();
            break;
        }
        if(next_char() != '}') {
            pop_stack();
            break;
        }
        rm_stack();
        return new term("{}", a);
    } while(false);
    do {    // one or more
        push_stack();
        if(next_char() != '<') {
            pop_stack();
            break;
        }
        rwspace *ws1 = parse_rwspace();
        if(ws1 == nullptr) {
            pop_stack();
            break;
        }
        alternation* a = parse_alternation();
        if(a == nullptr) {
            pop_stack();
            break;
        }
        rwspace *ws2 = parse_rwspace();
        if(ws2 == nullptr) {
            pop_stack();
            break;
        }
        if(next_char() != '>') {
            pop_stack();
            break;
        }
        rm_stack();
        return new term("<>", a);
    } while(false);
    if(auto t = parse_terminal()) return new term(t);
    if(auto i = parse_identifier()) return new term(i);
    return nullptr;
}

concatenation* parse_concatenation() {
    push_stack();
    vector<term*> ts;
    {   //first term
        term *t = parse_term();
        if(t == nullptr) {
            pop_stack();
            return nullptr;
        }
        ts.push_back(t);
    }
    while(true) {   //rest of terms
        push_stack();
        rwspace *ws1, *ws2;
        ws1 = parse_rwspace();
        if(ws1 == nullptr) {
            pop_stack();
            break;
        }
        if(next_char() != ',') {
            pop_stack();
            break;
        }
        ws2 = parse_rwspace();
        if(ws2 == nullptr) {
            pop_stack();
            break;
        }
        term *t = parse_term();
        if(t == nullptr) {
            pop_stack();
            break;
        }
        rm_stack();
        ts.push_back(t);
    }
    rm_stack();
    return new concatenation(ts);
}

alternation* parse_alternation() {
    push_stack();
    vector<concatenation*> cs;
    {   //first term
        concatenation *c = parse_concatenation();
        if(c == nullptr) {
            pop_stack();
            return nullptr;
        }
        cs.push_back(c);
    }
    while(true) {   //rest of terms
        push_stack();
        rwspace *ws1, *ws2;
        ws1 = parse_rwspace();
        if(ws1 == nullptr) {
            pop_stack();
            break;
        }
        if(next_char() != '|') {
            pop_stack();
            break;
        }
        ws2 = parse_rwspace();
        if(ws2 == nullptr) {
            pop_stack();
            break;
        }
        concatenation *c = parse_concatenation();
        if(c == nullptr) {
            pop_stack();
            break;
        }
        rm_stack();
        cs.push_back(c);
    }
    rm_stack();
    return new alternation(cs);
}

rule* parse_rule() {
    if(parse_debug) {
        cout << "PARSE RULE : " << parse_ptr << endl;
    }
    push_stack();
    rwspace *ws;
    identifier *i = parse_identifier();
    if(i == nullptr) {
        pop_stack();
        return nullptr;
    }
    ws = parse_rwspace();
    if(ws == nullptr) {
        pop_stack();
        return nullptr;
    }
    if(next_char() != '=') {
        pop_stack();
        return nullptr;
    }
    ws = parse_rwspace();
    if(ws == nullptr) {
        pop_stack();
        return nullptr;
    }
    alternation *a = parse_alternation();
    if(a == nullptr) {
        pop_stack();
        return nullptr;
    }
    ws = parse_rwspace();
    if(ws == nullptr) {
        pop_stack();
        return nullptr;
    }
    if(next_char() != ';') {
        pop_stack();
        return nullptr;
    }
    rm_stack();
    return new rule(i, a);
}

include* parse_include() {
    push_stack();
    rwspace *ws;
    if(next_chars(8) != "#include") {
        pop_stack();
        return nullptr;
    }
    ws = parse_rwspace();
    if(ws == nullptr) {
        pop_stack();
        return nullptr;
    }
    terminal *t = parse_terminal();
    if(t == nullptr) {
        pop_stack();
        return nullptr;
    }
    ws = parse_rwspace();
    if(ws == nullptr) {
        pop_stack();
        return nullptr;
    }
    if(next_chars(1) != ";") {
        pop_stack();
        return nullptr;
    }
    rm_stack();
    return new include(t);
}

grammar* parse_grammar() {
    if(parse_debug) {
        cout << "PARSE GRAMMAR : " << parse_ptr << endl;
    }
    push_stack();
    owspace *ws;
    vector<include*> is;
    while(true) {
        push_stack();
        ws = parse_owspace();
        if(ws == nullptr) {
            pop_stack();
            break;
        }
        include *i = parse_include();
        if(i == nullptr) {
            pop_stack();
            break;
        }
        rm_stack();
        is.push_back(i);
    }
    vector<rule*> rs;
    while(true) {
        push_stack();
        ws = parse_owspace();
        if(ws == nullptr) {
            pop_stack();
            break;
        }
        rule *r = parse_rule();
        if(r == nullptr) {
            pop_stack();
            break;
        }
        rm_stack();
        rs.push_back(r);
    }
    ws = parse_owspace();
    if(ws == nullptr) {
        pop_stack();
        return nullptr;
    }
    rm_stack();
    return new grammar(is, rs);
}

// -- PRINT FUNCTIONS --
//these are named print functions as they originally just printed to the console. 
//ah whatever, it's not like anyone else is going to look at this code :P
string print_letter(letter *l);
string print_digit(digit *d);
string print_symbol(symbol *s);
string print_escape(escape *e);
string print_identifier(identifier *i);
string print_comment_char(comment_char *c);
string print_comment(comment *c);
string print_wspace(wspace *w);
string print_rwspace(rwspace *w);
string print_owspace(owspace *o);
string print_terminal_char(terminal_char *c);
string print_terminal(terminal *t);
string print_term(term *t);
string print_concatenation(concatenation *c);
string print_alternation(alternation *a);
string print_rule(rule *r);
string print_include(include *i);
string print_grammar(grammar *g);

string print_letter(letter *l) {
    return string(1, l->c);
}

string print_digit(digit *d) {
    return string(1, d->c);
}

string print_symbol(symbol *s) {
    return string(1, s->c);
}

string print_escape(escape *e) {
    return "\\" + string(1, e->c);
}

string print_identifier(identifier *i) {
    string ans = "";
    for(int j = 0; j < i->ls.size(); j++) {
        ans += print_letter(i->ls[j]);
    }
    return ans;
}

string print_comment_char(comment_char* c) {
    if(c->is_letter) return print_letter(c->l);
    else if(c->is_digit) return print_digit(c->d);
    else if(c->is_wspace) return print_wspace(c->w);
    else if(c->is_literal) return string(1, c->c);
    else assert(false);
}

string print_comment(comment *c) {
    string ans = "";
    for(int i = 0; i < c->s.size(); i++){
        ans += print_comment_char(c->s[i]);
    }
    return ans;
}

string print_wspace(wspace *w) {
    return string(1, w->c);
}

string print_rwspace(rwspace *w){
    string ans = "";
    for(int i = 0; i < w->ss.size(); i++){
        ans += print_wspace(w->ss[i]);
    }
    return ans;
}

string print_owspace(owspace *w){
    string ans = "";
    for(int i = 0; i < w->ss.size(); i++){
        ans += print_wspace(w->ss[i]);
    }
    return ans;
}

string print_terminal_char(terminal_char *c) {
    if(c->is_letter) return print_letter(c->l);
    else if(c->is_digit) return print_digit(c->d);
    else if(c->is_symbol) return print_symbol(c->s);
    else if(c->is_escape) return print_escape(c->e);
    else if(c->is_literal) return string(1, c->c);
    else assert(false);
}

string print_terminal(terminal *t) {
    string ans = "";
    ans += "\"";
    for(int i = 0; i < t->cs.size(); i++){
        ans += print_terminal_char(t->cs[i]);
    }
    ans += "\"";
    return ans;
}

string print_term(term *t) {
    string ans = "";
    if(t->is_grouping) {
        ans += "( ";
        ans += print_alternation(t->a);
        ans += " )";
    }
    else if(t->is_zo) {
        ans += "[ ";
        ans += print_alternation(t->a);
        ans += " ]";
    }
    else if(t->is_zm) {
        ans += "{ "; 
        ans += print_alternation(t->a);
        ans += " }";
    }
    else if(t->is_om) {
        ans += "< ";
        ans += print_alternation(t->a);
        ans += " >";
    }
    else if(t->is_terminal) ans += print_terminal(t->t);
    else if(t->is_identifier) ans += print_identifier(t->i);
    else assert(false);
    return ans;
}

string print_concatenation(concatenation *c){
    string ans = "";
    ans += print_term(c->ts[0]);
    for(int i = 1; i < c->ts.size(); i++){
        ans += " , ";
        ans += print_term(c->ts[i]);
    }
    return ans;
}

string print_alternation(alternation *a) {
    string ans = "";
    ans += print_concatenation(a->cs[0]);
    for(int i = 1; i < a->cs.size(); i++){
        ans += " | ";
        ans += print_concatenation(a->cs[i]);
    }
    return ans;
}

string print_rule(rule *r){
    string ans = "";
    ans += print_identifier(r->i);
    ans += " = ";
    ans += print_alternation(r->a);
    ans += " ;";
    return ans;
}

string print_include(include *i){
    return "#include " + print_terminal(i->t) + " ;";
}

string print_grammar(grammar *g) {
    string ans = "";
    for(int i = 0; i < g->is.size(); i++){
        ans += print_include(g->is[i]);
        ans += "\n";
    }
    ans += "\n";
    for(int i = 0; i < g->rs.size(); i++){
        ans += print_rule(g->rs[i]);
        ans += "\n";
    }
    return ans;
}

// -- GENERATOR FUNCTIONS --

// indent controller
int indent_level;
const int INDENT_WIDTH = 4;
string indent() {
    return string(indent_level * INDENT_WIDTH, ' ');
}

/*
STRUCTS
 - all groupings get their own struct. all concatenations will get their own struct
 - concatenations will get multiple fields in a struct
 - alternations will get one or more concatenations in a struct
 - substructs will be named according to their depth and which one they are within their current depth. 
   - ('a' + depth) + (child index)
   - I consider a struct that is a direct child of the root to be depth 0
 - terminals will become strings
 - fields within a concatenation will be named "t" + (field index)
   - if you ever need to use 't' in a substruct name, then consider reworking the grammar
 - if current struct has alternations, will prepend every concatenation struct with 'bool is_t + (field index)'

PARSERS
 - parse functions will be a part of the structs. This way I don't have to remember the struct geneaology when creating the parsers
 - every struct should have a parse function. It will return a struct pointer on success and nullptr on failure. 
 - going through the parse function should be the way you create new structs. 
   - note that I'm not explicitly making the constructor private, use your best judgement. 
 - parse functions should only reference the parse functions of root level structs and structs of depth + 1. 

IMPLEMENTED FEATURES
 - alternations with just one option should get simplified down into that option
 - add to_string or something equivalent. 

NEW FEATURES
 - concatenations with just a terminal should get simplified down into a string
 - alternations with just strings should get simplified down into one string
 - ability to auto-generate methods that will find all of some structure A within structure B and return a vector of them in order

lets look at an example rule

//concatenation = term , { rws , "," , rws , term } ;
struct concatenation {
    struct a0 {
        rws *t0;
        string t1;
        rws *t2;
        term *t3;
        a0(rws *_t0, string _t1, rws *_t2, term *_t3) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
        }
        static a0* parse();
        string to_string();
    };
    term *t0;
    vector<a0*> t1;
    concatenation(term *_t0, vector<a0*> _t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static concatenation* parse();
    string to_string();
};

concatenation::a0* concatenation::a0::parse() {
    push_stack();
    rws *x0 = rws::parse();
    if(x0 == nullptr) {pop_stack(); return nullptr;}
    string x1 = next_chars(1);
    if(x1 != ",") {pop_stack(); return nullptr;}
    rws *x2 = rws::parse();
    if(x2 == nullptr) {pop_stack(); return nullptr;}
    term *x3 = term::parse();
    if(x3 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new concatenation::a0(x0, x1, x2, x3);
}

concatenation* concatenation::parse() {
    push_stack();
    term *x0 = term::parse();
    if(x0 == nullptr) {pop_stack(); return nullptr;}
    vector<concatenation::a0*> x1;
    while(true) {
        concatenation::a0 *tmp = concatenation::a0::parse();
        if(tmp == nullptr) break;
        x1.push_back(tmp);
    }
    rm_stack();
    return new concatenation(x0, x1);
}

string concatenation::a0::to_string() {
    string ans = "";
    ans += t0->to_string();
    ans += t1->to_string();
    ans += t2->to_string();
    ans += t3->to_string();
    return ans;
}

string concatenation::to_string() {
    string ans = "";
    ans += t0->to_string();
    for(int i = 0; i < t1->size(); i++) ans += t1[i]->to_string();
    return ans;
}

To resolve circular dependencies, all the structs will first be forward declared without their 
parse functions, then all the parse functions will be defined afterwards. 
*/

//struct depth controller
int struct_depth;

void generate_struct_from_concatenation(concatenation *c, string struct_name);
void generate_struct_from_alternation(alternation *a, string struct_name);
void generate_struct_from_rule(rule *r);

void generate_fndef_from_concatenation(concatenation *c, string struct_name);
void generate_fndef_from_alternation(alternation *a, string struct_name);
void generate_fndef_from_rule(rule *r);

string process_escapes(string s) {
    string res = "";
    for(int i = 0; i < s.size(); i++){
        if(s[i] != '\\') {
            res.push_back(s[i]);
            continue;
        }
        assert(i + 1 < s.size());
        if(s[i + 1] == 'n') res.push_back('\n');
        else if(s[i + 1] == 't') res.push_back('\t');
        else if(s[i + 1] == 'r') res.push_back('\r');
        else if(s[i + 1] == 'f') res.push_back('\f');
        else if(s[i + 1] == 'b') res.push_back('\b');
        else if(s[i + 1] == '\\') res.push_back('\\');
        else if(s[i + 1] == '\"') res.push_back('\"');
        else assert(false);
        i ++;
    }
    return res;
}

void generate_struct_from_concatenation(concatenation *c, string struct_name){
    cout << indent() << "struct " << struct_name << " {\n";
    struct_depth ++;
    indent_level ++;

    //gather info
    int substr_ind = 0;
    string layer_char = string(1, 'a' + struct_depth - 1);
    vector<string> type_sid, var_sid;
    for(int i = 0; i < c->ts.size(); i++){
        string ctype = "";
        term *t = c->ts[i];
        if(t->is_grouping) {
            ctype = layer_char + to_string(substr_ind ++);
            generate_struct_from_alternation(t->a, ctype);
        }
        else if(t->is_zo) {
            ctype = layer_char + to_string(substr_ind ++);
            generate_struct_from_alternation(t->a, ctype);
        }
        else if(t->is_zm) {
            ctype = layer_char + to_string(substr_ind ++);
            generate_struct_from_alternation(t->a, ctype);
        }
        else if(t->is_om) {
            ctype = layer_char + to_string(substr_ind ++);
            generate_struct_from_alternation(t->a, ctype);
        }
        else if(t->is_terminal) {
            ctype = "std::string";
        }
        else if(t->is_identifier) {
            ctype = print_identifier(t->i);
        }
        else assert(false);
        type_sid.push_back(ctype);
        var_sid.push_back("t" + to_string(i));
    }   

    //print fields
    for(int i = 0; i < c->ts.size(); i++){  //print out term declarations
        term *t = c->ts[i];
        if(t->is_grouping) {
            cout << indent() << type_sid[i] << " *" << var_sid[i] << ";\n";
        }
        else if(t->is_zo) {
            cout << indent() << type_sid[i] << " *" << var_sid[i] << ";\n";
        }
        else if(t->is_zm) {
            cout << indent() << "std::vector<" << type_sid[i] << "*> " << var_sid[i] << ";\n";
        }
        else if(t->is_om) {
            cout << indent() << "std::vector<" << type_sid[i] << "*> " << var_sid[i] << ";\n";
        }
        else if(t->is_terminal) {
            cout << indent() << type_sid[i] << " " << var_sid[i] << ";\n";
        }
        else if(t->is_identifier) {
            cout << indent() << type_sid[i] << " *" << var_sid[i] << ";\n";
        }
        else assert(false);
    }

    //constructor
    cout << indent() << struct_name << "(";
    for(int i = 0; i < c->ts.size(); i++){
        term *t = c->ts[i];
        if(t->is_grouping) {
            cout << type_sid[i] << " *_" << var_sid[i];
        }
        else if(t->is_zo) {
            cout << type_sid[i] << " *_" << var_sid[i];
        }
        else if(t->is_zm) {
            cout << "std::vector<" << type_sid[i] << "*> _" << var_sid[i];
        }
        else if(t->is_om) {
            cout << "std::vector<" << type_sid[i] << "*> _" << var_sid[i];
        }
        else if(t->is_terminal) {
            cout << type_sid[i] << " _" << var_sid[i];
        }
        else if(t->is_identifier) {
            cout << type_sid[i] << " *_" << var_sid[i];
        }
        else assert(false);
        if(i + 1 != c->ts.size()) {
            cout << ", ";
        }
    }
    cout << ") {\n";
    indent_level ++;
    for(int i = 0; i < c->ts.size(); i++){
        cout << indent() << var_sid[i] << " = _" << var_sid[i] << ";\n";
    }
    indent_level --;
    cout << indent() << "}\n";

    //parser declaration
    cout << indent() << "static " << struct_name << "* parse();\n";

    //to_string declaration
    cout << indent() << "std::string to_string();\n";

    indent_level --;
    struct_depth --;
    cout << indent() << "};\n";
}

void generate_struct_from_alternation(alternation *a, string struct_name){
    if(a->cs.size() == 1){  //there is only one option
        generate_struct_from_concatenation(a->cs[0], struct_name);
    }
    else {  //there are multiple options; need to do some substructs. 
        cout << indent() << "struct " << struct_name << " {\n";
        struct_depth ++;
        indent_level ++;
        
        string layer_char = string(1, 'a' + struct_depth - 1);
        vector<string> type_sid, var_sid;
        for(int i = 0; i < a->cs.size(); i++){
            type_sid.push_back(layer_char + to_string(i));
            var_sid.push_back("t" + to_string(i));
        }

        //substructs
        for(int i = 0; i < a->cs.size(); i++){
            generate_struct_from_concatenation(a->cs[i], type_sid[i]);
        }

        //struct fields
        for(int i = 0; i < a->cs.size(); i++){
            cout << indent() << "bool is_" << type_sid[i] << " = false;\n";
            cout << indent() << type_sid[i] << " *" << var_sid[i] << ";\n";
        }

        //constructors
        for(int i = 0; i < a->cs.size(); i++){
            cout << indent() << struct_name << "(" << type_sid[i] << " *_" << var_sid[i] << ") {\n";
            indent_level ++;
            cout << indent() << "is_" << type_sid[i] << " = true;\n";
            cout << indent() << var_sid[i] << " = _" << var_sid[i] << ";\n";
            indent_level --;
            cout << indent() << "}\n";
        }

        //parser declaration
        cout << indent() << "static " << struct_name << "* parse();\n";

        //to_string declaration
        cout << indent() << "std::string to_string();\n";

        indent_level --;
        struct_depth --;
        cout << indent() << "};\n";
    }
}

//generates the struct definition. The struct will still need to be pre-declared. 
void generate_struct_from_rule(rule *r){
    struct_depth = 0;
    identifier *id = r->i;
    alternation *a = r->a;
    cout << indent() << "// " << print_rule(r) << "\n";
    generate_struct_from_alternation(a, print_identifier(id));
}

void generate_fndef_from_concatenation(concatenation *c, string struct_name) {
    struct_depth ++;

    //gather info, generate child parsers
    int substr_ind = 0;
    string layer_char = string(1, 'a' + struct_depth - 1);
    vector<string> type_sid, var_sid;
    for(int i = 0; i < c->ts.size(); i++){
        string ctype = "";
        term *t = c->ts[i];
        if(t->is_grouping) {
            ctype = struct_name + "::" + layer_char + to_string(substr_ind);
            generate_fndef_from_alternation(t->a, struct_name + "::" + layer_char + to_string(substr_ind ++));
        }
        else if(t->is_zo) {
            ctype = struct_name + "::" + layer_char + to_string(substr_ind);
            generate_fndef_from_alternation(t->a, struct_name + "::" + layer_char + to_string(substr_ind ++));
        }
        else if(t->is_zm) {
            ctype = struct_name + "::" + layer_char + to_string(substr_ind);
            generate_fndef_from_alternation(t->a, struct_name + "::" + layer_char + to_string(substr_ind ++));
        }
        else if(t->is_om) {
            ctype = struct_name + "::" + layer_char + to_string(substr_ind);
            generate_fndef_from_alternation(t->a, struct_name + "::" + layer_char + to_string(substr_ind ++));
        }
        else if(t->is_terminal) {
            ctype = "std::string";
        }
        else if(t->is_identifier) {
            ctype = print_identifier(t->i);
        }
        else assert(false);
        type_sid.push_back(ctype);
        var_sid.push_back("t" + to_string(i));
    }   

    //generate parser
    cout << indent() << struct_name << "* " << struct_name << "::parse() {\n";
    indent_level ++;
    cout << indent() << "push_stack();\n";
    for(int i = 0; i < c->ts.size(); i++) {
        term *t = c->ts[i];
        if(t->is_grouping) {
            cout << indent() << type_sid[i] << " *_" << var_sid[i] << " = " << type_sid[i] << "::parse();\n";
            cout << indent() << "if(_" << var_sid[i] << " == nullptr) {pop_stack(); return nullptr;}\n";
        }
        else if(t->is_zo) {
            cout << indent() << type_sid[i] << " *_" << var_sid[i] << " = " << type_sid[i] << "::parse();\n";
        }
        else if(t->is_zm) {
            cout << indent() << "std::vector<" << type_sid[i] << "*> _" << var_sid[i] << ";\n";
            cout << indent() << "while(true) {\n";
            indent_level ++;
            cout << indent() << type_sid[i] << " *tmp = " << type_sid[i] << "::parse();\n";
            cout << indent() << "if(tmp == nullptr) break;\n";
            cout << indent() << "_" << var_sid[i] << ".push_back(tmp);\n";
            indent_level --;
            cout << indent() << "}\n";
        }
        else if(t->is_om) {
            cout << indent() << "std::vector<" << type_sid[i] << "*> _" << var_sid[i] << ";\n";
            cout << indent() << "while(true) {\n";
            indent_level ++;
            cout << indent() << type_sid[i] << " *tmp = " << type_sid[i] << "::parse();\n";
            cout << indent() << "if(tmp == nullptr) break;\n";
            cout << indent() << "_" << var_sid[i] << ".push_back(tmp);\n";
            indent_level --;
            cout << indent() << "}\n";
            cout << indent() << "if(_" << var_sid[i] << ".size() == 0) {pop_stack(); return nullptr;}\n";
        }
        else if(t->is_terminal) {
            string terminal_str = print_terminal(t->t); //with quotes, escapes
            assert(terminal_str.size() >= 3);
            int real_sz = process_escapes(terminal_str.substr(1, terminal_str.size() - 2)).size();  //need to process all escapes to determine size
            cout << indent() << "std::string _" << var_sid[i] << " = next_chars(" << real_sz << ");\n";
            cout << indent() << "if(_" << var_sid[i] << " != " << terminal_str << ") {pop_stack(); return nullptr;}\n";
        }
        else if(t->is_identifier) {
            cout << indent() << type_sid[i] << " *_" << var_sid[i] << " = " << type_sid[i] << "::parse();\n";
            cout << indent() << "if(_" << var_sid[i] << " == nullptr) {pop_stack(); return nullptr;}\n";
        }
        else assert(false);
    }
    cout << indent() << "rm_stack();\n";
    cout << indent() << "return new " << struct_name << "(";
    for(int i = 0; i < c->ts.size(); i++){
        cout << "_" << var_sid[i];
        if(i + 1 != c->ts.size()) {
            cout << ", ";
        }
    }
    cout << ");\n";
    indent_level --;
    cout << indent() << "}\n";
    cout << "\n";

    //generate to_string
    cout << indent() << "std::string " << struct_name << "::to_string() {\n";
    indent_level ++;
    cout << indent() << "std::string ans = \"\";\n";
    for(int i = 0; i < c->ts.size(); i++){
        term *t = c->ts[i];
        if(t->is_grouping) {
            cout << indent() << "ans += " << var_sid[i] << "->to_string();\n";
        }
        else if(t->is_zo) {
            cout << indent() << "if(" << var_sid[i] << " != nullptr) ans += " << var_sid[i] << "->to_string();\n";
        }
        else if(t->is_zm) {
            cout << indent() << "for(int i = 0; i < " << var_sid[i] << ".size(); i++) ans += " << var_sid[i] << "[i]->to_string();\n";
        }
        else if(t->is_om) {
            cout << indent() << "for(int i = 0; i < " << var_sid[i] << ".size(); i++) ans += " << var_sid[i] << "[i]->to_string();\n";
        }
        else if(t->is_terminal) {
            cout << indent() << "ans += " << var_sid[i] << ";\n";
        }
        else if(t->is_identifier) {
            cout << indent() << "ans += " << var_sid[i] << "->to_string();\n";
        }
        else assert(false);
    }
    cout << indent() << "return ans;\n";
    indent_level --;
    cout << indent() << "}\n";
    cout << "\n";

    struct_depth --;
}

void generate_fndef_from_alternation(alternation *a, string struct_name) {
    if(a->cs.size() == 1) {
        generate_fndef_from_concatenation(a->cs[0], struct_name);
    }
    else {
        struct_depth ++;

        //gather info, generate child parsers
        vector<string> type_sid, var_sid;
        string layer_char = string(1, 'a' + struct_depth - 1);
        for(int i = 0; i < a->cs.size(); i++){
            type_sid.push_back(struct_name + "::" + layer_char + to_string(i));
            var_sid.push_back("t" + to_string(i));
            generate_fndef_from_concatenation(a->cs[i], struct_name + "::" + layer_char + to_string(i));
        }

        //generate parser
        cout << indent() << struct_name << "* " << struct_name << "::parse() {\n";
        indent_level ++;
        for(int i = 0; i < a->cs.size(); i++) {
            cout << indent() << "if(auto x = " << type_sid[i] << "::parse()) return new " << struct_name << "(x);\n";
        }
        cout << indent() << "return nullptr;\n";
        indent_level --;
        cout << indent() << "}\n";
        cout << "\n";
        
        //generate to_string
        cout << indent() << "std::string " << struct_name << "::to_string() {\n";
        indent_level ++;
        for(int i = 0; i < a->cs.size(); i++){
            cout << indent() << "if(is_" << (layer_char + to_string(i)) << ") return " << var_sid[i] << "->to_string();\n";
        }
        cout << indent() << "assert(false);\n";
        indent_level --;    
        cout << indent() << "}\n";
        cout << "\n";

        struct_depth --;
    }
    
}

void generate_fndef_from_rule(rule *r) {
    struct_depth = 0;
    identifier *id = r->i;
    alternation *a = r->a;
    generate_fndef_from_alternation(a, print_identifier(id));
}

string get_current_date_string() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm *parts = localtime(&now_c);

    ostringstream oss;
    oss << put_time(parts, "%m-%d-%Y %H:%M:%S");
    return oss.str();
}

void generate_h(grammar *g) {
    indent_level = 0;
    cout << "// Date Generated : " << get_current_date_string() << "\n";
    cout << "#pragma once\n";
    cout << "#include <vector>\n";
    cout << "#include <string>\n";
    cout << "#include <cassert>\n";
    cout << "#include <iostream>\n";
    cout << "#include <stack>\n";
    cout << "#include <fstream>\n";
    cout << "#include <sstream>\n";
    cout << "#include <stdexcept>\n";
    cout << "\n";
    
    cout << "namespace parser {\n";
    indent_level ++;
    // parse controller
    {
        cout << indent() << "void set_s(std::string& ns);\n";
        cout << indent() << "bool check_finished_parsing();\n";
    }
    cout << "\n";

    // struct defs
    {   
        //forward declare structs
        for(int i = 0; i < g->rs.size(); i++){
            cout << indent() << "struct " << print_identifier(g->rs[i]->i) << ";\n";
        }
        cout << "\n";

        for(int i = 0; i < g->rs.size(); i++){
            generate_struct_from_rule(g->rs[i]);
            cout << "\n";
        }
    }

    indent_level --;
    cout << indent() << "};\n";
}

void generate_cpp(grammar *g) {
    indent_level = 0;
    cout << "// Date Generated : " << get_current_date_string() << "\n";
    cout << "#include \"parser.h\"\n";
    cout << "\n";

    cout << "namespace parser {\n";
    indent_level ++;
    // parse controller
    {
        string tmp = 
    R"(    // -- PARSE CONTROLLER --
    //the grammar to be parsed
    std::string s;

    //where we are in the string
    int ptr;

    //this is so we know where to backtrack to
    //the stack should be unaffected by any parse function. 
    std::stack<int> ptr_stack;

    //initializes the parse controller
    void set_s(std::string& ns) {
        s = ns;
        ptr = 0;
        while(ptr_stack.size() != 0) ptr_stack.pop();
    }

    //call this when you think you are done
    bool check_finished_parsing() {
        if(ptr != s.size()) return false;
        return true;
    }

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
    )";
        cout << tmp << "\n";
    }
    cout << "\n";

    // function defs
    {   
        for(int i = 0; i < g->rs.size(); i++){
            generate_fndef_from_rule(g->rs[i]);
        }
    }

    indent_level --;
    cout << indent() << "};\n";
}

// -- MAIN --

string read_file(const string& filename) {
    ifstream file(filename); 
    if (!file) {
        throw runtime_error("Failed to open file: " + filename);
    }
    ostringstream buffer;
    buffer << file.rdbuf(); 
    return buffer.str();     
}

bool do_semantic_checks(grammar *g) {
    // - are there identifiers that are defined twice
    set<string> defined_identifiers;
    {
        for(int i = 0; i < g->rs.size(); i++){
            identifier *id = g->rs[i]->i;
            string sid = "";
            for(int j = 0; j < id->ls.size(); j++){
                sid.push_back(id->ls[j]->c);
            }
            if(defined_identifiers.count(sid)) {
                cout << "DOUBLE DEFINED IDENTIFIER : " << sid << "\n";
                return false;
            }
            defined_identifiers.insert(sid);
        }
    }

    // - are there identifiers that aren't defined in a rule
    {
        function<bool(identifier*)> is_valid_identifier;
        function<bool(term*)> is_valid_term;
        function<bool(concatenation*)> is_valid_concatenation;
        function<bool(alternation*)> is_valid_alternation;
        function<bool(rule*)> is_valid_rule;
        function<bool(grammar*)> is_valid_grammar;

        is_valid_identifier = [&](identifier *i) -> bool {
            string istr = print_identifier(i);
            if(!defined_identifiers.count(istr)) {
                cout << "UNDEFINED IDENTIFIER : " << istr << "\n";
                return false;
            } 
            return true;
        };

        is_valid_term = [&](term *t) -> bool {
            bool ans = true;
            if(t->is_grouping) {
                ans = is_valid_alternation(t->a);
            }
            else if(t->is_zo) {
                ans = is_valid_alternation(t->a);
            }
            else if(t->is_zm) {
                ans = is_valid_alternation(t->a);
            }
            else if(t->is_om) {
                ans = is_valid_alternation(t->a);
            }
            else if(t->is_terminal) {
                //do nothing
            }
            else if(t->is_identifier) {
                ans = is_valid_identifier(t->i);
            }
            else assert(false);
            return ans;
        };

        is_valid_concatenation = [&](concatenation *c) -> bool {
            bool ans = true;
            for(int i = 0; i < c->ts.size(); i++){
                ans = ans & is_valid_term(c->ts[i]);
            }
            return ans;
        };

        is_valid_alternation = [&](alternation *a) -> bool {
            bool ans = true;
            for(int i = 0; i < a->cs.size(); i++){
                ans = ans & is_valid_concatenation(a->cs[i]);
            }
            return ans;
        };

        is_valid_rule = [&](rule *r) -> bool {
            bool ans = true;
            ans = ans & is_valid_identifier(r->i);
            ans = ans & is_valid_alternation(r->a);
            return ans;
        };

        is_valid_grammar = [&](grammar *g) -> bool {
            bool ans = true;
            for(int i = 0; i < g->rs.size(); i++) {
                ans = ans & is_valid_rule(g->rs[i]);
            }
            return ans;
        };

        if(!is_valid_grammar(g)) {
            return false;
        }
    }

    return true;
}

grammar* parse_string(string s) {
    parse_str = s;
    parse_ptr = 0;
    grammar *g = parse_grammar();
    assert(ptr_stack.size() == 0);
    if(g == nullptr || parse_ptr != s.size()) return nullptr;
    else return g;
}


grammar* parse_file(string filename) {
    // make sure filename ends with ".ebnf"
    if(filename.size() < 5 || filename.substr(filename.size() - 5) != ".ebnf") {
        cout << "<filename> must end with \".ebnf\"\n";
        return nullptr;
    }
    return parse_string(read_file(filename));
}

string read_cstr(char* s) {
    string ans = "";
    int ptr = 0;
    while(s[ptr] != '\0') {
        ans.push_back(s[ptr ++]);
    }
    return ans;
}

signed main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "USAGE : <filename>\n";
        cout << "<filename> must end with \".ebnf\"\n";
        return 1;
    }

    // read it in
    int argptr = 1;
    string filename = read_cstr(argv[argptr ++]);

    // link
    cout << "LINKING\n";
    string combined_ebnf = "";
    {
        set<string> parsed_files;
        function<bool(string)> dfs = [&](string filename) -> bool { 
            if(parsed_files.count(filename)) return true;
            parsed_files.insert(filename);
            cout << filename << "\n";
            grammar *g = parse_file(filename);
            if(g == nullptr) {
                cout << "FAILED TO PARSE\n";
                return false;
            }
            for(int i = 0; i < g->is.size(); i++){
                include *in = g->is[i];
                string n_filename = print_terminal(in->t);
                n_filename = n_filename.substr(1, n_filename.size() - 2);
                if(!dfs(n_filename)) {
                    return false;
                }
            }
            for(int i = 0; i < g->rs.size(); i++){
                combined_ebnf += print_rule(g->rs[i]) + "\n";
            }
            combined_ebnf += "\n";
            return true;
        };
        if(!dfs(filename)) {
            cout << "LINKING FAILED\n";
            return 1;
        }
        cout << "LINKING PASSED\n\n";
    }

    // parse
    cout << "PARSING\n";
    grammar *g = parse_string(combined_ebnf);
    if(g == nullptr) {
        cout << "PARSING FAILED\n";
        return 1;
    }
    cout << "PARSING PASSED\n";
    cout << print_grammar(g) << "\n";
    
    // some semantic checking.
    cout << "RUNNING SEMANTIC CHECKS\n";
    if(!do_semantic_checks(g)) {
        cout << "SEMANTIC CHECKS FAILED\n";
        return 1;
    }
    cout << "SEMANTIC CHECKS PASSED\n\n";
    
    // spit out code
    cout << "GENERATING PROGRAM\n";
    string header_filename = "parser.h", cpp_filename = "parser.cpp";
    streambuf *coutbuf = cout.rdbuf(); 
    {
        ofstream out = ofstream(header_filename);
        if (!out) {
            cout << "Failed to open " << header_filename << "\n";
            return 1;
        }
        cout.rdbuf(out.rdbuf());  
        generate_h(g);
    }
    {
        ofstream out = ofstream(cpp_filename);
        if(!out) {
            cout << "Failed to open " << cpp_filename << "\n";
            return 1;
        }
        cout.rdbuf(out.rdbuf());
        generate_cpp(g);
    }

    cout.rdbuf(coutbuf); // required to not segfault D:
    
    cout << "DONE GENERATING\n";

    return 0;
}