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
    bool is_terminal = false;
    bool is_identifier = false;

    alternation *a;
    terminal *t;
    identifier *i;

    term(string type, alternation *_a) {
        if(type == "()") is_grouping = true;
        else if(type == "[]") is_zo = true;
        else if(type == "{}") is_zm = true;
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

struct grammar {
    vector<rule*> rs;
    grammar(vector<rule*> _rs) {
        rs = _rs;
    }
};

// -- PARSE CONTROLLER --
//the grammar to be parsed
string s;

//where we are in the string
int ptr;

//this is so we know where to backtrack to
stack<int> ptr_stack;

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

string next_chars(int n) {
    assert(n > 0);
    if(ptr + n > s.size()) return "";
    string ans = s.substr(ptr, n);
    ptr += n;
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
        cout << "PARSE RULE : " << ptr << endl;
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

grammar* parse_grammar() {
    if(parse_debug) {
        cout << "PARSE GRAMMAR : " << ptr << endl;
    }
    push_stack();
    owspace *ws;
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
    return new grammar(rs);
}

// -- PRINT FUNCTIONS --
void print_letter(letter *l);
void print_digit(digit *d);
void print_symbol(symbol *s);
void print_escape(escape *e);
void print_identifier(identifier *i);
void print_comment_char(comment_char *c);
void print_comment(comment *c);
void print_wspace(wspace *w);
void print_rwspace(rwspace *w);
void print_owspace(owspace *o);
void print_terminal_char(terminal_char *c);
void print_terminal(terminal *t);
void print_term(term *t);
void print_concatenation(concatenation *c);
void print_alternation(alternation *a);
void print_rule(rule *r);
void print_grammar(grammar *g);

void print_letter(letter *l) {
    cout << l->c;
}

void print_digit(digit *d) {
    cout << d->c;
}

void print_symbol(symbol *s) {
    cout << s->c;
}

void print_escape(escape *e) {
    cout << "\\" << e->c;
}

void print_identifier(identifier *i) {
    for(int j = 0; j < i->ls.size(); j++) {
        print_letter(i->ls[j]);
    }
}

void print_comment_char(comment_char* c) {
    if(c->is_letter) print_letter(c->l);
    else if(c->is_digit) print_digit(c->d);
    else if(c->is_wspace) print_wspace(c->w);
    else if(c->is_literal) cout << c->c;
    else assert(false);
}

void print_comment(comment *c) {
    for(int i = 0; i < c->s.size(); i++){
        print_comment_char(c->s[i]);
    }
}

void print_wspace(wspace *w) {
    cout << w->c;
}

void print_rwspace(rwspace *w){
    for(int i = 0; i < w->ss.size(); i++){
        print_wspace(w->ss[i]);
    }
}

void print_owspace(owspace *w){
    for(int i = 0; i < w->ss.size(); i++){
        print_wspace(w->ss[i]);
    }
}

void print_terminal_char(terminal_char *c) {
    if(c->is_letter) print_letter(c->l);
    else if(c->is_digit) print_digit(c->d);
    else if(c->is_symbol) print_symbol(c->s);
    else if(c->is_escape) print_escape(c->e);
    else if(c->is_literal) cout << c->c;
    else assert(false);
}

void print_terminal(terminal *t) {
    cout << "\"";
    for(int i = 0; i < t->cs.size(); i++){
        print_terminal_char(t->cs[i]);
    }
    cout << "\"";
}

void print_term(term *t) {
    if(t->is_grouping) {
        cout << "( ";
        print_alternation(t->a);
        cout << " )";
    }
    else if(t->is_zo) {
        cout << "[ ";
        print_alternation(t->a);
        cout << " ]";
    }
    else if(t->is_zm) {
        cout << "{ "; 
        print_alternation(t->a);
        cout << " }";
    }
    else if(t->is_terminal) print_terminal(t->t);
    else if(t->is_identifier) print_identifier(t->i);
    else assert(false);
}

void print_concatenation(concatenation *c){
    print_term(c->ts[0]);
    for(int i = 1; i < c->ts.size(); i++){
        cout << " , ";
        print_term(c->ts[i]);
    }
}

void print_alternation(alternation *a) {
    print_concatenation(a->cs[0]);
    for(int i = 1; i < a->cs.size(); i++){
        cout << " | ";
        print_concatenation(a->cs[i]);
    }
}

void print_rule(rule *r){
    print_identifier(r->i);
    cout << " = ";
    print_alternation(r->a);
    cout << " ;";
}

void print_grammar(grammar *g) {
    for(int i = 0; i < g->rs.size(); i++){
        print_rule(g->rs[i]);
        cout << "\n";
    }
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

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    
    
    // read it in
    string filename;
    cin >> filename;
    s = read_file(filename);
    cout << "S : \n" << s << endl;

    // build parse tree  
    ptr = 0;
    grammar *g = parse_grammar();
    if(ptr_stack.size() != 0){
        cout << "PTR STACK SIZE : " << ptr_stack.size() << "\n";
    }
    assert(ptr_stack.size() == 0);
    if(g == nullptr) {
        cout << "FAILED\n";
    }
    else {
        cout << "SUCCESS : " << g->rs.size() << "\n";
        print_grammar(g);
        // for(int i = 0; i < g->rs.size(); i++){
        //     cout << "RULE : " << g->rs[i]
        // }
    }
    
    // spit out code
    
    return 0;
}