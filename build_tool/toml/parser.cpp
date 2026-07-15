// Date Generated : 07-14-2026 23:22:38
#include "parser.h"

namespace parser {
    // -- PARSE CONTROLLER --
    //the grammar to be parsed
    std::string s;

    //what is the furthest we've gotten into the string
    int max_parse;

    parse_context ctx;

    parse_context get_ctx() {
        return ctx;
    }

    void set_ctx(parse_context nctx) {
        ctx = nctx;
    }

    //this is so we know where to backtrack to
    //the stack should be unaffected by any parse function. 
    std::stack<parse_context> ctx_stack;

    //generated during the postprocess phase
    //list of all error tokens within the CST
    std::vector<error*> errors;

    std::vector<error*> get_errors() {
        return errors;
    }
    
    //if true, will try to parse errors
    //if false, error::parse() will always fail
    bool gen_errors = false;

    void set_gen_errors(bool b) {
        gen_errors = b;
    }

    //does nice printout of lines surrounding the position where ind is
    void prettyprint_at_ind(int ind) {
        assert(ind >= 0 && ind < s.size());

        //figure out for each line, what character indices are on it
        //newlines will count as characters for the line that they broke, not the next line. 
        std::vector<std::vector<int>> lines(2); //start with a dummy line to make them 1-indexed
        for(int i = 0; i < s.size(); i++){
            lines.rbegin()->push_back(i);
            if(s[i] == '\n') lines.push_back({});
        }

        //find which line ind is on
        int indline = -1;
        for(int i = 0; i < lines.size(); i++){
            for(int j = 0; j < lines[i].size(); j++) {
                if(lines[i][j] == ind) indline = i;
            }
        }

        //figure out maximum width of numbers
        int line_start = std::max(0, indline - 5);
        int line_end = std::min((int) lines.size(), indline + 5);
        int nr_width = 0;
        for(int i = line_start; i < line_end; i++){
            nr_width = std::max(nr_width, (int) std::to_string(i).size());
        }

        //print relevant lines <= indline
        for(int i = line_start; i <= indline; i++){
            std::string istr = std::to_string(i);
            for(int j = istr.size(); j < nr_width; j++) std::cout << " ";
            std::cout << istr;
            std::cout << "| ";
            for(int j = 0; j < lines[i].size(); j++){
                int nind = lines[i][j];
                if(s[nind] == '\n') std::cout << " ";
                else std::cout << s[nind];
            }
            std::cout << "\n";
        }

        //print indptr
        std::cout << std::string(nr_width, ' ') << "| ";
        for(int i = 0; i < lines[indline].size(); i++){
            if(lines[indline][i] == ind) std::cout << "^";
            else std::cout << " ";
        }   
        std::cout << "\n";

        //print relevant lines > indline
        for(int i = indline + 1; i < line_end; i++){
            std::string istr = std::to_string(i);
            for(int j = istr.size(); j < nr_width; j++) std::cout << " ";
            std::cout << istr;
            std::cout << "| ";
            for(int j = 0; j < lines[i].size(); j++){
                int nind = lines[i][j];
                if(s[nind] == '\n') std::cout << " ";
                else std::cout << s[nind];
            }
            std::cout << "\n";
        }
    }

    //call this when you think you are done
    bool check_finished_parsing(bool prettyprint) {
        if(ctx.ptr != s.size()) {
            assert(max_parse >= 0 && max_parse <= s.size());
            //it could be the case that all the tokens are consumed, but the pattern isn't done parsing
            if(max_parse == s.size()) max_parse -= 1;   
            if(prettyprint) prettyprint_at_ind(max_parse);
            return false;
        }
        return true;
    }

    //use before trying an optional grammar rule
    void push_stack() {
        ctx_stack.push(ctx);
        max_parse = std::max(max_parse, ctx.ptr);
    }

    //use when grammar rule fails to parse
    void pop_stack() {
        assert(ctx_stack.size() != 0);
        ctx = ctx_stack.top();
        ctx_stack.pop();
    }

    //use when grammar rule parses successfully. 
    void rm_stack() {
        assert(ctx_stack.size() != 0);
        ctx_stack.pop();
    }
    
    char next_char() {
        if(ctx.ptr >= s.size()) return '\0';
        char ret = s[ctx.ptr];
        ctx.ptr ++;
        ctx.col ++;
        if(ret == '\n') {
            ctx.line ++;
            ctx.col = 0;
        }
        return ret;
    }

    std::string next_chars(int n) {
        assert(n > 0);
        if(ctx.ptr + n > s.size()) return "";
        std::string ans(n, '\0');
        for(int i = 0; i < n; i++) ans[i] = next_char();
        return ans;
    }
    
    bool is_eof() {
        return ctx.ptr == s.size();
    }

    terminal* terminal::parse(std::string val) {
        parse_context _start_ctx = get_ctx();
        push_stack();
        if(next_chars(val.size()) != val) {pop_stack(); return nullptr;}
        rm_stack();
        terminal* retval = new terminal(val);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string terminal::to_string() {
        return val;
    }

    void terminal::postprocess() {
        token_type = "terminal";
    }
    
    //initializes the parse controller
    void set_s(std::string& ns) {
        s = ns;
        max_parse = 0;
        ctx = {0, 0, 0};
        while(ctx_stack.size() != 0) ctx_stack.pop();
        errors.clear();
    }

    value::a0* value::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        basic_string *_t0 = basic_string::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        value::a0* retval = new value::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string value::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void value::a0::postprocess() {
        token_type = "value::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    value::a1* value::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        integer *_t0 = integer::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        value::a1* retval = new value::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string value::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void value::a1::postprocess() {
        token_type = "value::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    value::a2* value::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        boolean *_t0 = boolean::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        value::a2* retval = new value::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string value::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void value::a2::postprocess() {
        token_type = "value::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    value::a3* value::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        array *_t0 = array::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        value::a3* retval = new value::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string value::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void value::a3::postprocess() {
        token_type = "value::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    value* value::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = value::a0::parse()) {
            value* retval = new value(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = value::a1::parse()) {
            value* retval = new value(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = value::a2::parse()) {
            value* retval = new value(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = value::a3::parse()) {
            value* retval = new value(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string value::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        assert(false);
    }

    void value::postprocess() {
        token_type = "value";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_a2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_a3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
    }

    basic_string_char::a0* basic_string_char::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        basic_string_char::a0* retval = new basic_string_char::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string basic_string_char::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void basic_string_char::a0::postprocess() {
        token_type = "basic_string_char::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    basic_string_char::a1* basic_string_char::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        basic_string_char::a1* retval = new basic_string_char::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string basic_string_char::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void basic_string_char::a1::postprocess() {
        token_type = "basic_string_char::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    basic_string_char::a2* basic_string_char::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        basic_string_char::a2* retval = new basic_string_char::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string basic_string_char::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void basic_string_char::a2::postprocess() {
        token_type = "basic_string_char::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    basic_string_char::a3* basic_string_char::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        escape *_t0 = escape::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        basic_string_char::a3* retval = new basic_string_char::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string basic_string_char::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void basic_string_char::a3::postprocess() {
        token_type = "basic_string_char::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    basic_string_char::a4* basic_string_char::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        basic_string_char::a4* retval = new basic_string_char::a4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string basic_string_char::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void basic_string_char::a4::postprocess() {
        token_type = "basic_string_char::a4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    basic_string_char::a5* basic_string_char::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("'");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        basic_string_char::a5* retval = new basic_string_char::a5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string basic_string_char::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void basic_string_char::a5::postprocess() {
        token_type = "basic_string_char::a5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    basic_string_char* basic_string_char::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = basic_string_char::a0::parse()) {
            basic_string_char* retval = new basic_string_char(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = basic_string_char::a1::parse()) {
            basic_string_char* retval = new basic_string_char(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = basic_string_char::a2::parse()) {
            basic_string_char* retval = new basic_string_char(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = basic_string_char::a3::parse()) {
            basic_string_char* retval = new basic_string_char(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = basic_string_char::a4::parse()) {
            basic_string_char* retval = new basic_string_char(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = basic_string_char::a5::parse()) {
            basic_string_char* retval = new basic_string_char(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string basic_string_char::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        if(is_a4) return t4->to_string();
        if(is_a5) return t5->to_string();
        assert(false);
    }

    void basic_string_char::postprocess() {
        token_type = "basic_string_char";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_a2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_a3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_a4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        if(is_a5) {
            token_children.push_back(t5);
            t5->postprocess();
        }
    }

    basic_string::a0* basic_string::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        basic_string_char *_t0 = basic_string_char::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        basic_string::a0* retval = new basic_string::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string basic_string::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void basic_string::a0::postprocess() {
        token_type = "basic_string::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    basic_string* basic_string::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\"");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<basic_string::a0*> _t1;
        while(true) {
            basic_string::a0 *tmp = basic_string::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        terminal *_t2 = terminal::parse("\"");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        basic_string* retval = new basic_string(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string basic_string::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2->to_string();
        return ans;
    }

    void basic_string::postprocess() {
        token_type = "basic_string";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
        token_children.push_back(t2);
        t2->postprocess();
    }

    nonzero_digit::a0* nonzero_digit::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("1");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        nonzero_digit::a0* retval = new nonzero_digit::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string nonzero_digit::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void nonzero_digit::a0::postprocess() {
        token_type = "nonzero_digit::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    nonzero_digit::a1* nonzero_digit::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("2");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        nonzero_digit::a1* retval = new nonzero_digit::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string nonzero_digit::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void nonzero_digit::a1::postprocess() {
        token_type = "nonzero_digit::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    nonzero_digit::a2* nonzero_digit::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("3");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        nonzero_digit::a2* retval = new nonzero_digit::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string nonzero_digit::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void nonzero_digit::a2::postprocess() {
        token_type = "nonzero_digit::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    nonzero_digit::a3* nonzero_digit::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("4");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        nonzero_digit::a3* retval = new nonzero_digit::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string nonzero_digit::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void nonzero_digit::a3::postprocess() {
        token_type = "nonzero_digit::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    nonzero_digit::a4* nonzero_digit::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("5");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        nonzero_digit::a4* retval = new nonzero_digit::a4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string nonzero_digit::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void nonzero_digit::a4::postprocess() {
        token_type = "nonzero_digit::a4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    nonzero_digit::a5* nonzero_digit::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("6");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        nonzero_digit::a5* retval = new nonzero_digit::a5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string nonzero_digit::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void nonzero_digit::a5::postprocess() {
        token_type = "nonzero_digit::a5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    nonzero_digit::a6* nonzero_digit::a6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("7");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        nonzero_digit::a6* retval = new nonzero_digit::a6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string nonzero_digit::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void nonzero_digit::a6::postprocess() {
        token_type = "nonzero_digit::a6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    nonzero_digit::a7* nonzero_digit::a7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("8");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        nonzero_digit::a7* retval = new nonzero_digit::a7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string nonzero_digit::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void nonzero_digit::a7::postprocess() {
        token_type = "nonzero_digit::a7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    nonzero_digit::a8* nonzero_digit::a8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("9");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        nonzero_digit::a8* retval = new nonzero_digit::a8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string nonzero_digit::a8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void nonzero_digit::a8::postprocess() {
        token_type = "nonzero_digit::a8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    nonzero_digit* nonzero_digit::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = nonzero_digit::a0::parse()) {
            nonzero_digit* retval = new nonzero_digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = nonzero_digit::a1::parse()) {
            nonzero_digit* retval = new nonzero_digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = nonzero_digit::a2::parse()) {
            nonzero_digit* retval = new nonzero_digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = nonzero_digit::a3::parse()) {
            nonzero_digit* retval = new nonzero_digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = nonzero_digit::a4::parse()) {
            nonzero_digit* retval = new nonzero_digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = nonzero_digit::a5::parse()) {
            nonzero_digit* retval = new nonzero_digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = nonzero_digit::a6::parse()) {
            nonzero_digit* retval = new nonzero_digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = nonzero_digit::a7::parse()) {
            nonzero_digit* retval = new nonzero_digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = nonzero_digit::a8::parse()) {
            nonzero_digit* retval = new nonzero_digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string nonzero_digit::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        if(is_a4) return t4->to_string();
        if(is_a5) return t5->to_string();
        if(is_a6) return t6->to_string();
        if(is_a7) return t7->to_string();
        if(is_a8) return t8->to_string();
        assert(false);
    }

    void nonzero_digit::postprocess() {
        token_type = "nonzero_digit";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_a2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_a3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_a4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        if(is_a5) {
            token_children.push_back(t5);
            t5->postprocess();
        }
        if(is_a6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
        if(is_a7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_a8) {
            token_children.push_back(t8);
            t8->postprocess();
        }
    }

    unsigned_integer::a0* unsigned_integer::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("0");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        unsigned_integer::a0* retval = new unsigned_integer::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string unsigned_integer::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void unsigned_integer::a0::postprocess() {
        token_type = "unsigned_integer::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    unsigned_integer::a1::b0::c0* unsigned_integer::a1::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("_");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        unsigned_integer::a1::b0::c0* retval = new unsigned_integer::a1::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string unsigned_integer::a1::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void unsigned_integer::a1::b0::c0::postprocess() {
        token_type = "unsigned_integer::a1::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    unsigned_integer::a1::b0* unsigned_integer::a1::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::optional<unsigned_integer::a1::b0::c0*> _t0 = std::nullopt;
        if(auto x = unsigned_integer::a1::b0::c0::parse()) _t0 = x;
        digit *_t1 = digit::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        unsigned_integer::a1::b0* retval = new unsigned_integer::a1::b0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string unsigned_integer::a1::b0::to_string() {
        std::string ans = "";
        if(t0.has_value()) ans += t0.value()->to_string();
        ans += t1->to_string();
        return ans;
    }

    void unsigned_integer::a1::b0::postprocess() {
        token_type = "unsigned_integer::a1::b0";
        if(t0.has_value()) {
            token_children.push_back(t0.value());
            t0.value()->postprocess();
        }
        token_children.push_back(t1);
        t1->postprocess();
    }

    unsigned_integer::a1* unsigned_integer::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        nonzero_digit *_t0 = nonzero_digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<unsigned_integer::a1::b0*> _t1;
        while(true) {
            unsigned_integer::a1::b0 *tmp = unsigned_integer::a1::b0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        unsigned_integer::a1* retval = new unsigned_integer::a1(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string unsigned_integer::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void unsigned_integer::a1::postprocess() {
        token_type = "unsigned_integer::a1";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    unsigned_integer* unsigned_integer::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = unsigned_integer::a0::parse()) {
            unsigned_integer* retval = new unsigned_integer(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = unsigned_integer::a1::parse()) {
            unsigned_integer* retval = new unsigned_integer(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string unsigned_integer::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        assert(false);
    }

    void unsigned_integer::postprocess() {
        token_type = "unsigned_integer";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    integer::a0::b0* integer::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("+");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        integer::a0::b0* retval = new integer::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string integer::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void integer::a0::b0::postprocess() {
        token_type = "integer::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    integer::a0::b1* integer::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("-");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        integer::a0::b1* retval = new integer::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string integer::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void integer::a0::b1::postprocess() {
        token_type = "integer::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    integer::a0* integer::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = integer::a0::b0::parse()) {
            integer::a0* retval = new integer::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = integer::a0::b1::parse()) {
            integer::a0* retval = new integer::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string integer::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void integer::a0::postprocess() {
        token_type = "integer::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    integer* integer::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::optional<integer::a0*> _t0 = std::nullopt;
        if(auto x = integer::a0::parse()) _t0 = x;
        unsigned_integer *_t1 = unsigned_integer::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        integer* retval = new integer(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string integer::to_string() {
        std::string ans = "";
        if(t0.has_value()) ans += t0.value()->to_string();
        ans += t1->to_string();
        return ans;
    }

    void integer::postprocess() {
        token_type = "integer";
        if(t0.has_value()) {
            token_children.push_back(t0.value());
            t0.value()->postprocess();
        }
        token_children.push_back(t1);
        t1->postprocess();
    }

    boolean::a0* boolean::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("true");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        boolean::a0* retval = new boolean::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string boolean::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void boolean::a0::postprocess() {
        token_type = "boolean::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    boolean::a1* boolean::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("false");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        boolean::a1* retval = new boolean::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string boolean::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void boolean::a1::postprocess() {
        token_type = "boolean::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    boolean* boolean::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = boolean::a0::parse()) {
            boolean* retval = new boolean(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = boolean::a1::parse()) {
            boolean* retval = new boolean(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string boolean::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        assert(false);
    }

    void boolean::postprocess() {
        token_type = "boolean";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    value_list::a0* value_list::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(",");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ws *_t1 = ws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        value *_t2 = value::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        value_list::a0* retval = new value_list::a0(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string value_list::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void value_list::a0::postprocess() {
        token_type = "value_list::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    value_list::a1* value_list::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ws *_t0 = ws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(",");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        value_list::a1* retval = new value_list::a1(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string value_list::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void value_list::a1::postprocess() {
        token_type = "value_list::a1";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    value_list* value_list::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        value *_t0 = value::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ws *_t1 = ws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::vector<value_list::a0*> _t2;
        while(true) {
            value_list::a0 *tmp = value_list::a0::parse();
            if(tmp == nullptr) break;
            _t2.push_back(tmp);
        }
        std::optional<value_list::a1*> _t3 = std::nullopt;
        if(auto x = value_list::a1::parse()) _t3 = x;
        rm_stack();
        value_list* retval = new value_list(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string value_list::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        for(int i = 0; i < t2.size(); i++) ans += t2[i]->to_string();
        if(t3.has_value()) ans += t3.value()->to_string();
        return ans;
    }

    void value_list::postprocess() {
        token_type = "value_list";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        for(int i = 0; i < t2.size(); i++) {
            token_children.push_back(t2[i]);
            t2[i]->postprocess();
        }
        if(t3.has_value()) {
            token_children.push_back(t3.value());
            t3.value()->postprocess();
        }
    }

    array* array::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("[");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ws *_t1 = ws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        value_list *_t2 = value_list::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ws *_t3 = ws::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse("]");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        array* retval = new array(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string array::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void array::postprocess() {
        token_type = "array";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
        token_children.push_back(t4);
        t4->postprocess();
    }

    alpha::a0* alpha::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("A");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a0* retval = new alpha::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a0::postprocess() {
        token_type = "alpha::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a1* alpha::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("B");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a1* retval = new alpha::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a1::postprocess() {
        token_type = "alpha::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a2* alpha::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("C");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a2* retval = new alpha::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a2::postprocess() {
        token_type = "alpha::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a3* alpha::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("D");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a3* retval = new alpha::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a3::postprocess() {
        token_type = "alpha::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a4* alpha::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("E");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a4* retval = new alpha::a4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a4::postprocess() {
        token_type = "alpha::a4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a5* alpha::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("F");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a5* retval = new alpha::a5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a5::postprocess() {
        token_type = "alpha::a5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a6* alpha::a6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("G");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a6* retval = new alpha::a6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a6::postprocess() {
        token_type = "alpha::a6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a7* alpha::a7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("H");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a7* retval = new alpha::a7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a7::postprocess() {
        token_type = "alpha::a7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a8* alpha::a8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("I");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a8* retval = new alpha::a8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a8::postprocess() {
        token_type = "alpha::a8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a9* alpha::a9::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("J");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a9* retval = new alpha::a9(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a9::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a9::postprocess() {
        token_type = "alpha::a9";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a10* alpha::a10::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("K");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a10* retval = new alpha::a10(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a10::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a10::postprocess() {
        token_type = "alpha::a10";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a11* alpha::a11::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("L");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a11* retval = new alpha::a11(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a11::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a11::postprocess() {
        token_type = "alpha::a11";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a12* alpha::a12::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("M");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a12* retval = new alpha::a12(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a12::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a12::postprocess() {
        token_type = "alpha::a12";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a13* alpha::a13::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("N");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a13* retval = new alpha::a13(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a13::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a13::postprocess() {
        token_type = "alpha::a13";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a14* alpha::a14::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("O");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a14* retval = new alpha::a14(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a14::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a14::postprocess() {
        token_type = "alpha::a14";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a15* alpha::a15::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("P");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a15* retval = new alpha::a15(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a15::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a15::postprocess() {
        token_type = "alpha::a15";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a16* alpha::a16::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("Q");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a16* retval = new alpha::a16(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a16::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a16::postprocess() {
        token_type = "alpha::a16";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a17* alpha::a17::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("R");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a17* retval = new alpha::a17(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a17::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a17::postprocess() {
        token_type = "alpha::a17";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a18* alpha::a18::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("S");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a18* retval = new alpha::a18(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a18::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a18::postprocess() {
        token_type = "alpha::a18";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a19* alpha::a19::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("T");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a19* retval = new alpha::a19(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a19::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a19::postprocess() {
        token_type = "alpha::a19";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a20* alpha::a20::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("U");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a20* retval = new alpha::a20(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a20::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a20::postprocess() {
        token_type = "alpha::a20";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a21* alpha::a21::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("V");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a21* retval = new alpha::a21(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a21::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a21::postprocess() {
        token_type = "alpha::a21";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a22* alpha::a22::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("W");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a22* retval = new alpha::a22(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a22::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a22::postprocess() {
        token_type = "alpha::a22";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a23* alpha::a23::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("X");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a23* retval = new alpha::a23(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a23::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a23::postprocess() {
        token_type = "alpha::a23";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a24* alpha::a24::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("Y");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a24* retval = new alpha::a24(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a24::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a24::postprocess() {
        token_type = "alpha::a24";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a25* alpha::a25::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("Z");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a25* retval = new alpha::a25(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a25::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a25::postprocess() {
        token_type = "alpha::a25";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a26* alpha::a26::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("a");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a26* retval = new alpha::a26(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a26::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a26::postprocess() {
        token_type = "alpha::a26";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a27* alpha::a27::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("b");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a27* retval = new alpha::a27(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a27::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a27::postprocess() {
        token_type = "alpha::a27";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a28* alpha::a28::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("c");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a28* retval = new alpha::a28(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a28::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a28::postprocess() {
        token_type = "alpha::a28";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a29* alpha::a29::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("d");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a29* retval = new alpha::a29(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a29::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a29::postprocess() {
        token_type = "alpha::a29";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a30* alpha::a30::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("e");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a30* retval = new alpha::a30(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a30::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a30::postprocess() {
        token_type = "alpha::a30";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a31* alpha::a31::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("f");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a31* retval = new alpha::a31(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a31::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a31::postprocess() {
        token_type = "alpha::a31";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a32* alpha::a32::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("g");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a32* retval = new alpha::a32(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a32::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a32::postprocess() {
        token_type = "alpha::a32";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a33* alpha::a33::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("h");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a33* retval = new alpha::a33(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a33::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a33::postprocess() {
        token_type = "alpha::a33";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a34* alpha::a34::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("i");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a34* retval = new alpha::a34(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a34::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a34::postprocess() {
        token_type = "alpha::a34";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a35* alpha::a35::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("j");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a35* retval = new alpha::a35(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a35::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a35::postprocess() {
        token_type = "alpha::a35";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a36* alpha::a36::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("k");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a36* retval = new alpha::a36(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a36::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a36::postprocess() {
        token_type = "alpha::a36";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a37* alpha::a37::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("l");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a37* retval = new alpha::a37(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a37::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a37::postprocess() {
        token_type = "alpha::a37";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a38* alpha::a38::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("m");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a38* retval = new alpha::a38(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a38::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a38::postprocess() {
        token_type = "alpha::a38";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a39* alpha::a39::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("n");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a39* retval = new alpha::a39(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a39::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a39::postprocess() {
        token_type = "alpha::a39";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a40* alpha::a40::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("o");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a40* retval = new alpha::a40(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a40::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a40::postprocess() {
        token_type = "alpha::a40";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a41* alpha::a41::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("p");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a41* retval = new alpha::a41(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a41::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a41::postprocess() {
        token_type = "alpha::a41";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a42* alpha::a42::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("q");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a42* retval = new alpha::a42(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a42::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a42::postprocess() {
        token_type = "alpha::a42";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a43* alpha::a43::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("r");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a43* retval = new alpha::a43(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a43::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a43::postprocess() {
        token_type = "alpha::a43";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a44* alpha::a44::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("s");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a44* retval = new alpha::a44(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a44::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a44::postprocess() {
        token_type = "alpha::a44";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a45* alpha::a45::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("t");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a45* retval = new alpha::a45(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a45::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a45::postprocess() {
        token_type = "alpha::a45";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a46* alpha::a46::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("u");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a46* retval = new alpha::a46(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a46::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a46::postprocess() {
        token_type = "alpha::a46";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a47* alpha::a47::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("v");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a47* retval = new alpha::a47(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a47::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a47::postprocess() {
        token_type = "alpha::a47";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a48* alpha::a48::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("w");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a48* retval = new alpha::a48(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a48::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a48::postprocess() {
        token_type = "alpha::a48";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a49* alpha::a49::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("x");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a49* retval = new alpha::a49(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a49::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a49::postprocess() {
        token_type = "alpha::a49";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a50* alpha::a50::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("y");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a50* retval = new alpha::a50(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a50::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a50::postprocess() {
        token_type = "alpha::a50";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha::a51* alpha::a51::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("z");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        alpha::a51* retval = new alpha::a51(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string alpha::a51::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void alpha::a51::postprocess() {
        token_type = "alpha::a51";
        token_children.push_back(t0);
        t0->postprocess();
    }

    alpha* alpha::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = alpha::a0::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a1::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a2::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a3::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a4::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a5::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a6::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a7::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a8::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a9::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a10::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a11::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a12::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a13::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a14::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a15::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a16::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a17::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a18::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a19::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a20::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a21::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a22::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a23::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a24::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a25::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a26::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a27::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a28::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a29::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a30::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a31::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a32::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a33::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a34::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a35::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a36::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a37::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a38::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a39::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a40::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a41::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a42::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a43::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a44::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a45::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a46::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a47::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a48::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a49::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a50::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = alpha::a51::parse()) {
            alpha* retval = new alpha(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string alpha::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        if(is_a4) return t4->to_string();
        if(is_a5) return t5->to_string();
        if(is_a6) return t6->to_string();
        if(is_a7) return t7->to_string();
        if(is_a8) return t8->to_string();
        if(is_a9) return t9->to_string();
        if(is_a10) return t10->to_string();
        if(is_a11) return t11->to_string();
        if(is_a12) return t12->to_string();
        if(is_a13) return t13->to_string();
        if(is_a14) return t14->to_string();
        if(is_a15) return t15->to_string();
        if(is_a16) return t16->to_string();
        if(is_a17) return t17->to_string();
        if(is_a18) return t18->to_string();
        if(is_a19) return t19->to_string();
        if(is_a20) return t20->to_string();
        if(is_a21) return t21->to_string();
        if(is_a22) return t22->to_string();
        if(is_a23) return t23->to_string();
        if(is_a24) return t24->to_string();
        if(is_a25) return t25->to_string();
        if(is_a26) return t26->to_string();
        if(is_a27) return t27->to_string();
        if(is_a28) return t28->to_string();
        if(is_a29) return t29->to_string();
        if(is_a30) return t30->to_string();
        if(is_a31) return t31->to_string();
        if(is_a32) return t32->to_string();
        if(is_a33) return t33->to_string();
        if(is_a34) return t34->to_string();
        if(is_a35) return t35->to_string();
        if(is_a36) return t36->to_string();
        if(is_a37) return t37->to_string();
        if(is_a38) return t38->to_string();
        if(is_a39) return t39->to_string();
        if(is_a40) return t40->to_string();
        if(is_a41) return t41->to_string();
        if(is_a42) return t42->to_string();
        if(is_a43) return t43->to_string();
        if(is_a44) return t44->to_string();
        if(is_a45) return t45->to_string();
        if(is_a46) return t46->to_string();
        if(is_a47) return t47->to_string();
        if(is_a48) return t48->to_string();
        if(is_a49) return t49->to_string();
        if(is_a50) return t50->to_string();
        if(is_a51) return t51->to_string();
        assert(false);
    }

    void alpha::postprocess() {
        token_type = "alpha";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_a2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_a3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_a4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        if(is_a5) {
            token_children.push_back(t5);
            t5->postprocess();
        }
        if(is_a6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
        if(is_a7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_a8) {
            token_children.push_back(t8);
            t8->postprocess();
        }
        if(is_a9) {
            token_children.push_back(t9);
            t9->postprocess();
        }
        if(is_a10) {
            token_children.push_back(t10);
            t10->postprocess();
        }
        if(is_a11) {
            token_children.push_back(t11);
            t11->postprocess();
        }
        if(is_a12) {
            token_children.push_back(t12);
            t12->postprocess();
        }
        if(is_a13) {
            token_children.push_back(t13);
            t13->postprocess();
        }
        if(is_a14) {
            token_children.push_back(t14);
            t14->postprocess();
        }
        if(is_a15) {
            token_children.push_back(t15);
            t15->postprocess();
        }
        if(is_a16) {
            token_children.push_back(t16);
            t16->postprocess();
        }
        if(is_a17) {
            token_children.push_back(t17);
            t17->postprocess();
        }
        if(is_a18) {
            token_children.push_back(t18);
            t18->postprocess();
        }
        if(is_a19) {
            token_children.push_back(t19);
            t19->postprocess();
        }
        if(is_a20) {
            token_children.push_back(t20);
            t20->postprocess();
        }
        if(is_a21) {
            token_children.push_back(t21);
            t21->postprocess();
        }
        if(is_a22) {
            token_children.push_back(t22);
            t22->postprocess();
        }
        if(is_a23) {
            token_children.push_back(t23);
            t23->postprocess();
        }
        if(is_a24) {
            token_children.push_back(t24);
            t24->postprocess();
        }
        if(is_a25) {
            token_children.push_back(t25);
            t25->postprocess();
        }
        if(is_a26) {
            token_children.push_back(t26);
            t26->postprocess();
        }
        if(is_a27) {
            token_children.push_back(t27);
            t27->postprocess();
        }
        if(is_a28) {
            token_children.push_back(t28);
            t28->postprocess();
        }
        if(is_a29) {
            token_children.push_back(t29);
            t29->postprocess();
        }
        if(is_a30) {
            token_children.push_back(t30);
            t30->postprocess();
        }
        if(is_a31) {
            token_children.push_back(t31);
            t31->postprocess();
        }
        if(is_a32) {
            token_children.push_back(t32);
            t32->postprocess();
        }
        if(is_a33) {
            token_children.push_back(t33);
            t33->postprocess();
        }
        if(is_a34) {
            token_children.push_back(t34);
            t34->postprocess();
        }
        if(is_a35) {
            token_children.push_back(t35);
            t35->postprocess();
        }
        if(is_a36) {
            token_children.push_back(t36);
            t36->postprocess();
        }
        if(is_a37) {
            token_children.push_back(t37);
            t37->postprocess();
        }
        if(is_a38) {
            token_children.push_back(t38);
            t38->postprocess();
        }
        if(is_a39) {
            token_children.push_back(t39);
            t39->postprocess();
        }
        if(is_a40) {
            token_children.push_back(t40);
            t40->postprocess();
        }
        if(is_a41) {
            token_children.push_back(t41);
            t41->postprocess();
        }
        if(is_a42) {
            token_children.push_back(t42);
            t42->postprocess();
        }
        if(is_a43) {
            token_children.push_back(t43);
            t43->postprocess();
        }
        if(is_a44) {
            token_children.push_back(t44);
            t44->postprocess();
        }
        if(is_a45) {
            token_children.push_back(t45);
            t45->postprocess();
        }
        if(is_a46) {
            token_children.push_back(t46);
            t46->postprocess();
        }
        if(is_a47) {
            token_children.push_back(t47);
            t47->postprocess();
        }
        if(is_a48) {
            token_children.push_back(t48);
            t48->postprocess();
        }
        if(is_a49) {
            token_children.push_back(t49);
            t49->postprocess();
        }
        if(is_a50) {
            token_children.push_back(t50);
            t50->postprocess();
        }
        if(is_a51) {
            token_children.push_back(t51);
            t51->postprocess();
        }
    }

    digit::a0* digit::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("0");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a0* retval = new digit::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a0::postprocess() {
        token_type = "digit::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit::a1* digit::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("1");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a1* retval = new digit::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a1::postprocess() {
        token_type = "digit::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit::a2* digit::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("2");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a2* retval = new digit::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a2::postprocess() {
        token_type = "digit::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit::a3* digit::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("3");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a3* retval = new digit::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a3::postprocess() {
        token_type = "digit::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit::a4* digit::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("4");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a4* retval = new digit::a4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a4::postprocess() {
        token_type = "digit::a4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit::a5* digit::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("5");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a5* retval = new digit::a5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a5::postprocess() {
        token_type = "digit::a5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit::a6* digit::a6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("6");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a6* retval = new digit::a6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a6::postprocess() {
        token_type = "digit::a6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit::a7* digit::a7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("7");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a7* retval = new digit::a7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a7::postprocess() {
        token_type = "digit::a7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit::a8* digit::a8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("8");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a8* retval = new digit::a8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a8::postprocess() {
        token_type = "digit::a8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit::a9* digit::a9::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("9");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        digit::a9* retval = new digit::a9(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string digit::a9::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void digit::a9::postprocess() {
        token_type = "digit::a9";
        token_children.push_back(t0);
        t0->postprocess();
    }

    digit* digit::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = digit::a0::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = digit::a1::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = digit::a2::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = digit::a3::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = digit::a4::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = digit::a5::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = digit::a6::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = digit::a7::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = digit::a8::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = digit::a9::parse()) {
            digit* retval = new digit(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string digit::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        if(is_a4) return t4->to_string();
        if(is_a5) return t5->to_string();
        if(is_a6) return t6->to_string();
        if(is_a7) return t7->to_string();
        if(is_a8) return t8->to_string();
        if(is_a9) return t9->to_string();
        assert(false);
    }

    void digit::postprocess() {
        token_type = "digit";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_a2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_a3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_a4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        if(is_a5) {
            token_children.push_back(t5);
            t5->postprocess();
        }
        if(is_a6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
        if(is_a7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_a8) {
            token_children.push_back(t8);
            t8->postprocess();
        }
        if(is_a9) {
            token_children.push_back(t9);
            t9->postprocess();
        }
    }

    escape::a0::b0* escape::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("n");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape::a0::b0* retval = new escape::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void escape::a0::b0::postprocess() {
        token_type = "escape::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    escape::a0::b1* escape::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("t");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape::a0::b1* retval = new escape::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void escape::a0::b1::postprocess() {
        token_type = "escape::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    escape::a0::b2* escape::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("r");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape::a0::b2* retval = new escape::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void escape::a0::b2::postprocess() {
        token_type = "escape::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    escape::a0::b3* escape::a0::b3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("f");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape::a0::b3* retval = new escape::a0::b3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void escape::a0::b3::postprocess() {
        token_type = "escape::a0::b3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    escape::a0::b4* escape::a0::b4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("b");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape::a0::b4* retval = new escape::a0::b4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::a0::b4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void escape::a0::b4::postprocess() {
        token_type = "escape::a0::b4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    escape::a0::b5* escape::a0::b5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\"");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape::a0::b5* retval = new escape::a0::b5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::a0::b5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void escape::a0::b5::postprocess() {
        token_type = "escape::a0::b5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    escape::a0::b6* escape::a0::b6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\\");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape::a0::b6* retval = new escape::a0::b6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::a0::b6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void escape::a0::b6::postprocess() {
        token_type = "escape::a0::b6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    escape::a0* escape::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = escape::a0::b0::parse()) {
            escape::a0* retval = new escape::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = escape::a0::b1::parse()) {
            escape::a0* retval = new escape::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = escape::a0::b2::parse()) {
            escape::a0* retval = new escape::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = escape::a0::b3::parse()) {
            escape::a0* retval = new escape::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = escape::a0::b4::parse()) {
            escape::a0* retval = new escape::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = escape::a0::b5::parse()) {
            escape::a0* retval = new escape::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = escape::a0::b6::parse()) {
            escape::a0* retval = new escape::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string escape::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        if(is_b4) return t4->to_string();
        if(is_b5) return t5->to_string();
        if(is_b6) return t6->to_string();
        assert(false);
    }

    void escape::a0::postprocess() {
        token_type = "escape::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_b2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_b3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_b4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        if(is_b5) {
            token_children.push_back(t5);
            t5->postprocess();
        }
        if(is_b6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
    }

    escape* escape::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\\");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        escape::a0 *_t1 = escape::a0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape* retval = new escape(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void escape::postprocess() {
        token_type = "escape";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    symbol::a0* symbol::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("[");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a0* retval = new symbol::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a0::postprocess() {
        token_type = "symbol::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a1* symbol::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("]");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a1* retval = new symbol::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a1::postprocess() {
        token_type = "symbol::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a2* symbol::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("{");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a2* retval = new symbol::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a2::postprocess() {
        token_type = "symbol::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a3* symbol::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("}");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a3* retval = new symbol::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a3::postprocess() {
        token_type = "symbol::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a4* symbol::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("(");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a4* retval = new symbol::a4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a4::postprocess() {
        token_type = "symbol::a4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a5* symbol::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(")");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a5* retval = new symbol::a5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a5::postprocess() {
        token_type = "symbol::a5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a6* symbol::a6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a6* retval = new symbol::a6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a6::postprocess() {
        token_type = "symbol::a6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a7* symbol::a7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a7* retval = new symbol::a7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a7::postprocess() {
        token_type = "symbol::a7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a8* symbol::a8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a8* retval = new symbol::a8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a8::postprocess() {
        token_type = "symbol::a8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a9* symbol::a9::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("|");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a9* retval = new symbol::a9(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a9::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a9::postprocess() {
        token_type = "symbol::a9";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a10* symbol::a10::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(".");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a10* retval = new symbol::a10(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a10::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a10::postprocess() {
        token_type = "symbol::a10";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a11* symbol::a11::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(",");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a11* retval = new symbol::a11(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a11::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a11::postprocess() {
        token_type = "symbol::a11";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a12* symbol::a12::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(";");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a12* retval = new symbol::a12(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a12::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a12::postprocess() {
        token_type = "symbol::a12";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a13* symbol::a13::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("-");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a13* retval = new symbol::a13(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a13::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a13::postprocess() {
        token_type = "symbol::a13";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a14* symbol::a14::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("+");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a14* retval = new symbol::a14(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a14::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a14::postprocess() {
        token_type = "symbol::a14";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a15* symbol::a15::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("_");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a15* retval = new symbol::a15(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a15::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a15::postprocess() {
        token_type = "symbol::a15";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a16* symbol::a16::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a16* retval = new symbol::a16(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a16::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a16::postprocess() {
        token_type = "symbol::a16";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a17* symbol::a17::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("?");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a17* retval = new symbol::a17(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a17::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a17::postprocess() {
        token_type = "symbol::a17";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a18* symbol::a18::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(":");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a18* retval = new symbol::a18(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a18::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a18::postprocess() {
        token_type = "symbol::a18";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a19* symbol::a19::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("!");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a19* retval = new symbol::a19(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a19::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a19::postprocess() {
        token_type = "symbol::a19";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a20* symbol::a20::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("@");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a20* retval = new symbol::a20(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a20::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a20::postprocess() {
        token_type = "symbol::a20";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a21* symbol::a21::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("$");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a21* retval = new symbol::a21(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a21::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a21::postprocess() {
        token_type = "symbol::a21";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a22* symbol::a22::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("%");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a22* retval = new symbol::a22(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a22::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a22::postprocess() {
        token_type = "symbol::a22";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a23* symbol::a23::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("^");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a23* retval = new symbol::a23(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a23::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a23::postprocess() {
        token_type = "symbol::a23";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a24* symbol::a24::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("&");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a24* retval = new symbol::a24(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a24::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a24::postprocess() {
        token_type = "symbol::a24";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a25* symbol::a25::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("/");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a25* retval = new symbol::a25(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a25::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a25::postprocess() {
        token_type = "symbol::a25";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a26* symbol::a26::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("~");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a26* retval = new symbol::a26(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a26::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a26::postprocess() {
        token_type = "symbol::a26";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol::a27* symbol::a27::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("`");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a27* retval = new symbol::a27(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a27::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a27::postprocess() {
        token_type = "symbol::a27";
        token_children.push_back(t0);
        t0->postprocess();
    }

    symbol* symbol::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = symbol::a0::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a1::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a2::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a3::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a4::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a5::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a6::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a7::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a8::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a9::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a10::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a11::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a12::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a13::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a14::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a15::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a16::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a17::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a18::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a19::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a20::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a21::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a22::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a23::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a24::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a25::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a26::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = symbol::a27::parse()) {
            symbol* retval = new symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string symbol::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        if(is_a4) return t4->to_string();
        if(is_a5) return t5->to_string();
        if(is_a6) return t6->to_string();
        if(is_a7) return t7->to_string();
        if(is_a8) return t8->to_string();
        if(is_a9) return t9->to_string();
        if(is_a10) return t10->to_string();
        if(is_a11) return t11->to_string();
        if(is_a12) return t12->to_string();
        if(is_a13) return t13->to_string();
        if(is_a14) return t14->to_string();
        if(is_a15) return t15->to_string();
        if(is_a16) return t16->to_string();
        if(is_a17) return t17->to_string();
        if(is_a18) return t18->to_string();
        if(is_a19) return t19->to_string();
        if(is_a20) return t20->to_string();
        if(is_a21) return t21->to_string();
        if(is_a22) return t22->to_string();
        if(is_a23) return t23->to_string();
        if(is_a24) return t24->to_string();
        if(is_a25) return t25->to_string();
        if(is_a26) return t26->to_string();
        if(is_a27) return t27->to_string();
        assert(false);
    }

    void symbol::postprocess() {
        token_type = "symbol";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_a2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_a3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_a4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        if(is_a5) {
            token_children.push_back(t5);
            t5->postprocess();
        }
        if(is_a6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
        if(is_a7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_a8) {
            token_children.push_back(t8);
            t8->postprocess();
        }
        if(is_a9) {
            token_children.push_back(t9);
            t9->postprocess();
        }
        if(is_a10) {
            token_children.push_back(t10);
            t10->postprocess();
        }
        if(is_a11) {
            token_children.push_back(t11);
            t11->postprocess();
        }
        if(is_a12) {
            token_children.push_back(t12);
            t12->postprocess();
        }
        if(is_a13) {
            token_children.push_back(t13);
            t13->postprocess();
        }
        if(is_a14) {
            token_children.push_back(t14);
            t14->postprocess();
        }
        if(is_a15) {
            token_children.push_back(t15);
            t15->postprocess();
        }
        if(is_a16) {
            token_children.push_back(t16);
            t16->postprocess();
        }
        if(is_a17) {
            token_children.push_back(t17);
            t17->postprocess();
        }
        if(is_a18) {
            token_children.push_back(t18);
            t18->postprocess();
        }
        if(is_a19) {
            token_children.push_back(t19);
            t19->postprocess();
        }
        if(is_a20) {
            token_children.push_back(t20);
            t20->postprocess();
        }
        if(is_a21) {
            token_children.push_back(t21);
            t21->postprocess();
        }
        if(is_a22) {
            token_children.push_back(t22);
            t22->postprocess();
        }
        if(is_a23) {
            token_children.push_back(t23);
            t23->postprocess();
        }
        if(is_a24) {
            token_children.push_back(t24);
            t24->postprocess();
        }
        if(is_a25) {
            token_children.push_back(t25);
            t25->postprocess();
        }
        if(is_a26) {
            token_children.push_back(t26);
            t26->postprocess();
        }
        if(is_a27) {
            token_children.push_back(t27);
            t27->postprocess();
        }
    }

    comment::a0::b0* comment::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        comment::a0::b0* retval = new comment::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void comment::a0::b0::postprocess() {
        token_type = "comment::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    comment::a0::b1* comment::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        comment::a0::b1* retval = new comment::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void comment::a0::b1::postprocess() {
        token_type = "comment::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    comment::a0::b2* comment::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        comment::a0::b2* retval = new comment::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void comment::a0::b2::postprocess() {
        token_type = "comment::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    comment::a0::b3* comment::a0::b3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        comment::a0::b3* retval = new comment::a0::b3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void comment::a0::b3::postprocess() {
        token_type = "comment::a0::b3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    comment::a0::b4* comment::a0::b4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\"");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        comment::a0::b4* retval = new comment::a0::b4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::a0::b4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void comment::a0::b4::postprocess() {
        token_type = "comment::a0::b4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    comment::a0::b5* comment::a0::b5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("'");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        comment::a0::b5* retval = new comment::a0::b5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::a0::b5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void comment::a0::b5::postprocess() {
        token_type = "comment::a0::b5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    comment::a0::b6* comment::a0::b6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        comment::a0::b6* retval = new comment::a0::b6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::a0::b6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void comment::a0::b6::postprocess() {
        token_type = "comment::a0::b6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    comment::a0::b7* comment::a0::b7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\\");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        comment::a0::b7* retval = new comment::a0::b7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::a0::b7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void comment::a0::b7::postprocess() {
        token_type = "comment::a0::b7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    comment::a0::b8* comment::a0::b8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\t");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        comment::a0::b8* retval = new comment::a0::b8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::a0::b8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void comment::a0::b8::postprocess() {
        token_type = "comment::a0::b8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    comment::a0* comment::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = comment::a0::b0::parse()) {
            comment::a0* retval = new comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = comment::a0::b1::parse()) {
            comment::a0* retval = new comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = comment::a0::b2::parse()) {
            comment::a0* retval = new comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = comment::a0::b3::parse()) {
            comment::a0* retval = new comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = comment::a0::b4::parse()) {
            comment::a0* retval = new comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = comment::a0::b5::parse()) {
            comment::a0* retval = new comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = comment::a0::b6::parse()) {
            comment::a0* retval = new comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = comment::a0::b7::parse()) {
            comment::a0* retval = new comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = comment::a0::b8::parse()) {
            comment::a0* retval = new comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string comment::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        if(is_b4) return t4->to_string();
        if(is_b5) return t5->to_string();
        if(is_b6) return t6->to_string();
        if(is_b7) return t7->to_string();
        if(is_b8) return t8->to_string();
        assert(false);
    }

    void comment::a0::postprocess() {
        token_type = "comment::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_b2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_b3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_b4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        if(is_b5) {
            token_children.push_back(t5);
            t5->postprocess();
        }
        if(is_b6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
        if(is_b7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_b8) {
            token_children.push_back(t8);
            t8->postprocess();
        }
    }

    comment* comment::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<comment::a0*> _t1;
        while(true) {
            comment::a0 *tmp = comment::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        comment* retval = new comment(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string comment::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void comment::postprocess() {
        token_type = "comment";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    ws::a0::b0* ws::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a0::b0* retval = new ws::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a0::b0::postprocess() {
        token_type = "ws::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a0::b1* ws::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\n");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a0::b1* retval = new ws::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a0::b1::postprocess() {
        token_type = "ws::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a0::b2* ws::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\t");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a0::b2* retval = new ws::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a0::b2::postprocess() {
        token_type = "ws::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a0::b3* ws::a0::b3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\r");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a0::b3* retval = new ws::a0::b3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a0::b3::postprocess() {
        token_type = "ws::a0::b3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a0::b4* ws::a0::b4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        comment *_t0 = comment::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a0::b4* retval = new ws::a0::b4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a0::b4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a0::b4::postprocess() {
        token_type = "ws::a0::b4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a0* ws::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = ws::a0::b0::parse()) {
            ws::a0* retval = new ws::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a0::b1::parse()) {
            ws::a0* retval = new ws::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a0::b2::parse()) {
            ws::a0* retval = new ws::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a0::b3::parse()) {
            ws::a0* retval = new ws::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a0::b4::parse()) {
            ws::a0* retval = new ws::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string ws::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        if(is_b4) return t4->to_string();
        assert(false);
    }

    void ws::a0::postprocess() {
        token_type = "ws::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_b2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_b3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_b4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
    }

    ws* ws::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::vector<ws::a0*> _t0;
        while(true) {
            ws::a0 *tmp = ws::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        rm_stack();
        ws* retval = new ws(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        return ans;
    }

    void ws::postprocess() {
        token_type = "ws";
        for(int i = 0; i < t0.size(); i++) {
            token_children.push_back(t0[i]);
            t0[i]->postprocess();
        }
    }

    hws::a0::b0* hws::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        hws::a0::b0* retval = new hws::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string hws::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void hws::a0::b0::postprocess() {
        token_type = "hws::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    hws::a0::b1* hws::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\t");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        hws::a0::b1* retval = new hws::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string hws::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void hws::a0::b1::postprocess() {
        token_type = "hws::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    hws::a0* hws::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = hws::a0::b0::parse()) {
            hws::a0* retval = new hws::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = hws::a0::b1::parse()) {
            hws::a0* retval = new hws::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string hws::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void hws::a0::postprocess() {
        token_type = "hws::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    hws* hws::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::vector<hws::a0*> _t0;
        while(true) {
            hws::a0 *tmp = hws::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        rm_stack();
        hws* retval = new hws(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string hws::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        return ans;
    }

    void hws::postprocess() {
        token_type = "hws";
        for(int i = 0; i < t0.size(); i++) {
            token_children.push_back(t0[i]);
            t0[i]->postprocess();
        }
    }

    bare_key::a0::b0* bare_key::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        bare_key::a0::b0* retval = new bare_key::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string bare_key::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void bare_key::a0::b0::postprocess() {
        token_type = "bare_key::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    bare_key::a0::b1* bare_key::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        bare_key::a0::b1* retval = new bare_key::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string bare_key::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void bare_key::a0::b1::postprocess() {
        token_type = "bare_key::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    bare_key::a0::b2* bare_key::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("_");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        bare_key::a0::b2* retval = new bare_key::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string bare_key::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void bare_key::a0::b2::postprocess() {
        token_type = "bare_key::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    bare_key::a0::b3* bare_key::a0::b3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("-");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        bare_key::a0::b3* retval = new bare_key::a0::b3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string bare_key::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void bare_key::a0::b3::postprocess() {
        token_type = "bare_key::a0::b3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    bare_key::a0* bare_key::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = bare_key::a0::b0::parse()) {
            bare_key::a0* retval = new bare_key::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = bare_key::a0::b1::parse()) {
            bare_key::a0* retval = new bare_key::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = bare_key::a0::b2::parse()) {
            bare_key::a0* retval = new bare_key::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = bare_key::a0::b3::parse()) {
            bare_key::a0* retval = new bare_key::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string bare_key::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        assert(false);
    }

    void bare_key::a0::postprocess() {
        token_type = "bare_key::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_b2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_b3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
    }

    bare_key* bare_key::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::vector<bare_key::a0*> _t0;
        while(true) {
            bare_key::a0 *tmp = bare_key::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        if(_t0.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        bare_key* retval = new bare_key(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string bare_key::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        return ans;
    }

    void bare_key::postprocess() {
        token_type = "bare_key";
        for(int i = 0; i < t0.size(); i++) {
            token_children.push_back(t0[i]);
            t0[i]->postprocess();
        }
    }

    quoted_key::a0::b0::c0* quoted_key::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a0::b0::c0* retval = new quoted_key::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a0::b0::c0::postprocess() {
        token_type = "quoted_key::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a0::b0::c1* quoted_key::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a0::b0::c1* retval = new quoted_key::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a0::b0::c1::postprocess() {
        token_type = "quoted_key::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a0::b0::c2* quoted_key::a0::b0::c2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a0::b0::c2* retval = new quoted_key::a0::b0::c2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a0::b0::c2::postprocess() {
        token_type = "quoted_key::a0::b0::c2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a0::b0::c3* quoted_key::a0::b0::c3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("'");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a0::b0::c3* retval = new quoted_key::a0::b0::c3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a0::b0::c3::postprocess() {
        token_type = "quoted_key::a0::b0::c3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a0::b0::c4* quoted_key::a0::b0::c4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a0::b0::c4* retval = new quoted_key::a0::b0::c4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a0::b0::c4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a0::b0::c4::postprocess() {
        token_type = "quoted_key::a0::b0::c4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a0::b0::c5* quoted_key::a0::b0::c5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a0::b0::c5* retval = new quoted_key::a0::b0::c5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a0::b0::c5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a0::b0::c5::postprocess() {
        token_type = "quoted_key::a0::b0::c5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a0::b0* quoted_key::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = quoted_key::a0::b0::c0::parse()) {
            quoted_key::a0::b0* retval = new quoted_key::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a0::b0::c1::parse()) {
            quoted_key::a0::b0* retval = new quoted_key::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a0::b0::c2::parse()) {
            quoted_key::a0::b0* retval = new quoted_key::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a0::b0::c3::parse()) {
            quoted_key::a0::b0* retval = new quoted_key::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a0::b0::c4::parse()) {
            quoted_key::a0::b0* retval = new quoted_key::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a0::b0::c5::parse()) {
            quoted_key::a0::b0* retval = new quoted_key::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string quoted_key::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        if(is_c4) return t4->to_string();
        if(is_c5) return t5->to_string();
        assert(false);
    }

    void quoted_key::a0::b0::postprocess() {
        token_type = "quoted_key::a0::b0";
        if(is_c0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_c1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_c2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_c3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_c4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        if(is_c5) {
            token_children.push_back(t5);
            t5->postprocess();
        }
    }

    quoted_key::a0* quoted_key::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\"");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<quoted_key::a0::b0*> _t1;
        while(true) {
            quoted_key::a0::b0 *tmp = quoted_key::a0::b0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        terminal *_t2 = terminal::parse("\"");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a0* retval = new quoted_key::a0(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2->to_string();
        return ans;
    }

    void quoted_key::a0::postprocess() {
        token_type = "quoted_key::a0";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
        token_children.push_back(t2);
        t2->postprocess();
    }

    quoted_key::a1::b0::c0* quoted_key::a1::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a1::b0::c0* retval = new quoted_key::a1::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a1::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a1::b0::c0::postprocess() {
        token_type = "quoted_key::a1::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a1::b0::c1* quoted_key::a1::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a1::b0::c1* retval = new quoted_key::a1::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a1::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a1::b0::c1::postprocess() {
        token_type = "quoted_key::a1::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a1::b0::c2* quoted_key::a1::b0::c2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a1::b0::c2* retval = new quoted_key::a1::b0::c2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a1::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a1::b0::c2::postprocess() {
        token_type = "quoted_key::a1::b0::c2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a1::b0::c3* quoted_key::a1::b0::c3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\"");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a1::b0::c3* retval = new quoted_key::a1::b0::c3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a1::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a1::b0::c3::postprocess() {
        token_type = "quoted_key::a1::b0::c3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a1::b0::c4* quoted_key::a1::b0::c4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a1::b0::c4* retval = new quoted_key::a1::b0::c4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a1::b0::c4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a1::b0::c4::postprocess() {
        token_type = "quoted_key::a1::b0::c4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a1::b0::c5* quoted_key::a1::b0::c5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a1::b0::c5* retval = new quoted_key::a1::b0::c5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a1::b0::c5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void quoted_key::a1::b0::c5::postprocess() {
        token_type = "quoted_key::a1::b0::c5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    quoted_key::a1::b0* quoted_key::a1::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = quoted_key::a1::b0::c0::parse()) {
            quoted_key::a1::b0* retval = new quoted_key::a1::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a1::b0::c1::parse()) {
            quoted_key::a1::b0* retval = new quoted_key::a1::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a1::b0::c2::parse()) {
            quoted_key::a1::b0* retval = new quoted_key::a1::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a1::b0::c3::parse()) {
            quoted_key::a1::b0* retval = new quoted_key::a1::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a1::b0::c4::parse()) {
            quoted_key::a1::b0* retval = new quoted_key::a1::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a1::b0::c5::parse()) {
            quoted_key::a1::b0* retval = new quoted_key::a1::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string quoted_key::a1::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        if(is_c4) return t4->to_string();
        if(is_c5) return t5->to_string();
        assert(false);
    }

    void quoted_key::a1::b0::postprocess() {
        token_type = "quoted_key::a1::b0";
        if(is_c0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_c1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_c2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_c3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        if(is_c4) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        if(is_c5) {
            token_children.push_back(t5);
            t5->postprocess();
        }
    }

    quoted_key::a1* quoted_key::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("'");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<quoted_key::a1::b0*> _t1;
        while(true) {
            quoted_key::a1::b0 *tmp = quoted_key::a1::b0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        terminal *_t2 = terminal::parse("'");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        quoted_key::a1* retval = new quoted_key::a1(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string quoted_key::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2->to_string();
        return ans;
    }

    void quoted_key::a1::postprocess() {
        token_type = "quoted_key::a1";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
        token_children.push_back(t2);
        t2->postprocess();
    }

    quoted_key* quoted_key::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = quoted_key::a0::parse()) {
            quoted_key* retval = new quoted_key(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = quoted_key::a1::parse()) {
            quoted_key* retval = new quoted_key(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string quoted_key::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        assert(false);
    }

    void quoted_key::postprocess() {
        token_type = "quoted_key";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    key::a0::b0* key::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        quoted_key *_t0 = quoted_key::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        key::a0::b0* retval = new key::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string key::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void key::a0::b0::postprocess() {
        token_type = "key::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    key::a0::b1* key::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        bare_key *_t0 = bare_key::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        key::a0::b1* retval = new key::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string key::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void key::a0::b1::postprocess() {
        token_type = "key::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    key::a0* key::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = key::a0::b0::parse()) {
            key::a0* retval = new key::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = key::a0::b1::parse()) {
            key::a0* retval = new key::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string key::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void key::a0::postprocess() {
        token_type = "key::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    key::a1::b0::c0* key::a1::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        quoted_key *_t0 = quoted_key::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        key::a1::b0::c0* retval = new key::a1::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string key::a1::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void key::a1::b0::c0::postprocess() {
        token_type = "key::a1::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    key::a1::b0::c1* key::a1::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        bare_key *_t0 = bare_key::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        key::a1::b0::c1* retval = new key::a1::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string key::a1::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void key::a1::b0::c1::postprocess() {
        token_type = "key::a1::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    key::a1::b0* key::a1::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = key::a1::b0::c0::parse()) {
            key::a1::b0* retval = new key::a1::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = key::a1::b0::c1::parse()) {
            key::a1::b0* retval = new key::a1::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string key::a1::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        assert(false);
    }

    void key::a1::b0::postprocess() {
        token_type = "key::a1::b0";
        if(is_c0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_c1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    key::a1* key::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        hws *_t0 = hws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(".");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        hws *_t2 = hws::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        key::a1::b0 *_t3 = key::a1::b0::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        key::a1* retval = new key::a1(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string key::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void key::a1::postprocess() {
        token_type = "key::a1";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    key* key::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        key::a0 *_t0 = key::a0::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<key::a1*> _t1;
        while(true) {
            key::a1 *tmp = key::a1::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        key* retval = new key(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string key::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void key::postprocess() {
        token_type = "key";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    table::a0* table::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("[");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        hws *_t1 = hws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        key *_t2 = key::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        hws *_t3 = hws::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse("]");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        table::a0* retval = new table::a0(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string table::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void table::a0::postprocess() {
        token_type = "table::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
        token_children.push_back(t4);
        t4->postprocess();
    }

    table::a1* table::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("[[");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        hws *_t1 = hws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        key *_t2 = key::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        hws *_t3 = hws::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse("]]");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        table::a1* retval = new table::a1(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string table::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void table::a1::postprocess() {
        token_type = "table::a1";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
        token_children.push_back(t4);
        t4->postprocess();
    }

    table* table::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = table::a0::parse()) {
            table* retval = new table(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = table::a1::parse()) {
            table* retval = new table(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string table::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        assert(false);
    }

    void table::postprocess() {
        token_type = "table";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    key_value* key_value::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        key *_t0 = key::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        hws *_t1 = hws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("=");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        hws *_t3 = hws::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        value *_t4 = value::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        key_value* retval = new key_value(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string key_value::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void key_value::postprocess() {
        token_type = "key_value";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
        token_children.push_back(t4);
        t4->postprocess();
    }

    statement::a0* statement::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        key_value *_t0 = key_value::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        statement::a0* retval = new statement::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string statement::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void statement::a0::postprocess() {
        token_type = "statement::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    statement::a1* statement::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        table *_t0 = table::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        statement::a1* retval = new statement::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string statement::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void statement::a1::postprocess() {
        token_type = "statement::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    statement* statement::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = statement::a0::parse()) {
            statement* retval = new statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = statement::a1::parse()) {
            statement* retval = new statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string statement::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        assert(false);
    }

    void statement::postprocess() {
        token_type = "statement";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    toml::a0* toml::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ws *_t0 = ws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        statement *_t1 = statement::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        toml::a0* retval = new toml::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string toml::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void toml::a0::postprocess() {
        token_type = "toml::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    toml* toml::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::vector<toml::a0*> _t0;
        while(true) {
            toml::a0 *tmp = toml::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        ws *_t1 = ws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        toml* retval = new toml(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string toml::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        ans += t1->to_string();
        return ans;
    }

    void toml::postprocess() {
        token_type = "toml";
        for(int i = 0; i < t0.size(); i++) {
            token_children.push_back(t0[i]);
            t0[i]->postprocess();
        }
        token_children.push_back(t1);
        t1->postprocess();
    }

};
