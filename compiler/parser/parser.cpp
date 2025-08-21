// Date Generated : 08-21-2025 16:12:07
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

    //this is so we know where to backtrack to
    //the stack should be unaffected by any parse function. 
    std::stack<parse_context> ctx_stack;

    //initializes the parse controller
    void set_s(std::string& ns) {
        s = ns;
        max_parse = 0;
        ctx = {0, 0, 0};
        while(ctx_stack.size() != 0) ctx_stack.pop();
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
        ctx.line_off ++;
        if(ret == '\n') {
            ctx.line ++;
            ctx.line_off = 0;
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
    

    function_definition* function_definition::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse("(");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        parameter_list *_t6 = parameter_list::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t8 = terminal::parse(")");
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        function_definition* retval = new function_definition(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string function_definition::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8->to_string();
        return ans;
    }

    void function_definition::postprocess() {
        token_type = "function_definition";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
        token_children.push_back(t8);
        t8->postprocess();
    }

    function::a0* function::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("export");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        function::a0* retval = new function::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string function::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void function::a0::postprocess() {
        token_type = "function::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    function* function::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        function::a0 *_t0 = function::a0::parse();
        function_definition *_t1 = function_definition::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        compound_statement *_t3 = compound_statement::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        function* retval = new function(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string function::to_string() {
        std::string ans = "";
        if(t0 != nullptr) ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void function::postprocess() {
        token_type = "function";
        if(t0 != nullptr) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    templated_function::a0* templated_function::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        template_header *_t0 = template_header::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        templated_function::a0* retval = new templated_function::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string templated_function::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void templated_function::a0::postprocess() {
        token_type = "templated_function::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    templated_function* templated_function::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        templated_function::a0 *_t0 = templated_function::a0::parse();
        function *_t1 = function::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        templated_function* retval = new templated_function(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string templated_function::to_string() {
        std::string ans = "";
        if(t0 != nullptr) ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void templated_function::postprocess() {
        token_type = "templated_function";
        if(t0 != nullptr) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        token_children.push_back(t1);
        t1->postprocess();
    }

    function_call* function_call::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        identifier *_t0 = identifier::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("(");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        argument_list *_t4 = argument_list::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t6 = terminal::parse(")");
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        function_call* retval = new function_call(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string function_call::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        return ans;
    }

    void function_call::postprocess() {
        token_type = "function_call";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
    }

    function_pointer_call* function_pointer_call::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t1 = identifier::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t3 = terminal::parse("(");
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        argument_list *_t5 = argument_list::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        ows *_t6 = ows::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t7 = terminal::parse(")");
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        function_pointer_call* retval = new function_pointer_call(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string function_pointer_call::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        return ans;
    }

    void function_pointer_call::postprocess() {
        token_type = "function_pointer_call";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
    }

    literal_sizeof* literal_sizeof::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("sizeof");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("(");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        type *_t4 = type::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t6 = terminal::parse(")");
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_sizeof* retval = new literal_sizeof(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_sizeof::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        return ans;
    }

    void literal_sizeof::postprocess() {
        token_type = "literal_sizeof";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
    }

    literal_integer::a0* literal_integer::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_integer::a0* retval = new literal_integer::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_integer::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_integer::a0::postprocess() {
        token_type = "literal_integer::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_integer* literal_integer::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::vector<literal_integer::a0*> _t0;
        while(true) {
            literal_integer::a0 *tmp = literal_integer::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        if(_t0.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        literal_integer* retval = new literal_integer(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_integer::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        return ans;
    }

    void literal_integer::postprocess() {
        token_type = "literal_integer";
        for(int i = 0; i < t0.size(); i++) {
            token_children.push_back(t0[i]);
            t0[i]->postprocess();
        }
    }

    literal_float::a0* literal_float::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_float::a0* retval = new literal_float::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_float::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_float::a0::postprocess() {
        token_type = "literal_float::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_float::a1* literal_float::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_float::a1* retval = new literal_float::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_float::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_float::a1::postprocess() {
        token_type = "literal_float::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_float* literal_float::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::vector<literal_float::a0*> _t0;
        while(true) {
            literal_float::a0 *tmp = literal_float::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        if(_t0.size() == 0) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(".");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::vector<literal_float::a1*> _t2;
        while(true) {
            literal_float::a1 *tmp = literal_float::a1::parse();
            if(tmp == nullptr) break;
            _t2.push_back(tmp);
        }
        if(_t2.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        literal_float* retval = new literal_float(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_float::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        ans += t1->to_string();
        for(int i = 0; i < t2.size(); i++) ans += t2[i]->to_string();
        return ans;
    }

    void literal_float::postprocess() {
        token_type = "literal_float";
        for(int i = 0; i < t0.size(); i++) {
            token_children.push_back(t0[i]);
            t0[i]->postprocess();
        }
        token_children.push_back(t1);
        t1->postprocess();
        for(int i = 0; i < t2.size(); i++) {
            token_children.push_back(t2[i]);
            t2[i]->postprocess();
        }
    }

    literal_char::a0::b0* literal_char::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_char::a0::b0* retval = new literal_char::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_char::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_char::a0::b0::postprocess() {
        token_type = "literal_char::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_char::a0::b1* literal_char::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_char::a0::b1* retval = new literal_char::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_char::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_char::a0::b1::postprocess() {
        token_type = "literal_char::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_char::a0::b2* literal_char::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        escape *_t0 = escape::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_char::a0::b2* retval = new literal_char::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_char::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_char::a0::b2::postprocess() {
        token_type = "literal_char::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_char::a0::b3* literal_char::a0::b3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_char::a0::b3* retval = new literal_char::a0::b3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_char::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_char::a0::b3::postprocess() {
        token_type = "literal_char::a0::b3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_char::a0::b4* literal_char::a0::b4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_char::a0::b4* retval = new literal_char::a0::b4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_char::a0::b4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_char::a0::b4::postprocess() {
        token_type = "literal_char::a0::b4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_char::a0* literal_char::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = literal_char::a0::b0::parse()) {
            literal_char::a0* retval = new literal_char::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_char::a0::b1::parse()) {
            literal_char::a0* retval = new literal_char::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_char::a0::b2::parse()) {
            literal_char::a0* retval = new literal_char::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_char::a0::b3::parse()) {
            literal_char::a0* retval = new literal_char::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_char::a0::b4::parse()) {
            literal_char::a0* retval = new literal_char::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string literal_char::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        if(is_b4) return t4->to_string();
        assert(false);
    }

    void literal_char::a0::postprocess() {
        token_type = "literal_char::a0";
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
    literal_char* literal_char::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("'");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        literal_char::a0 *_t1 = literal_char::a0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("'");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_char* retval = new literal_char(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_char::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void literal_char::postprocess() {
        token_type = "literal_char";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    literal_string::a0::b0* literal_string::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_string::a0::b0* retval = new literal_string::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_string::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_string::a0::b0::postprocess() {
        token_type = "literal_string::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_string::a0::b1* literal_string::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_string::a0::b1* retval = new literal_string::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_string::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_string::a0::b1::postprocess() {
        token_type = "literal_string::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_string::a0::b2* literal_string::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        escape *_t0 = escape::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_string::a0::b2* retval = new literal_string::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_string::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_string::a0::b2::postprocess() {
        token_type = "literal_string::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_string::a0::b3* literal_string::a0::b3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_string::a0::b3* retval = new literal_string::a0::b3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_string::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_string::a0::b3::postprocess() {
        token_type = "literal_string::a0::b3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_string::a0::b4* literal_string::a0::b4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_string::a0::b4* retval = new literal_string::a0::b4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_string::a0::b4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_string::a0::b4::postprocess() {
        token_type = "literal_string::a0::b4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_string::a0* literal_string::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = literal_string::a0::b0::parse()) {
            literal_string::a0* retval = new literal_string::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_string::a0::b1::parse()) {
            literal_string::a0* retval = new literal_string::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_string::a0::b2::parse()) {
            literal_string::a0* retval = new literal_string::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_string::a0::b3::parse()) {
            literal_string::a0* retval = new literal_string::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_string::a0::b4::parse()) {
            literal_string::a0* retval = new literal_string::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string literal_string::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        if(is_b4) return t4->to_string();
        assert(false);
    }

    void literal_string::a0::postprocess() {
        token_type = "literal_string::a0";
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
    literal_string* literal_string::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\"");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<literal_string::a0*> _t1;
        while(true) {
            literal_string::a0 *tmp = literal_string::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        terminal *_t2 = terminal::parse("\"");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_string* retval = new literal_string(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_string::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2->to_string();
        return ans;
    }

    void literal_string::postprocess() {
        token_type = "literal_string";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
        token_children.push_back(t2);
        t2->postprocess();
    }

    literal_syscall::a0* literal_syscall::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(",");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        argument_list *_t3 = argument_list::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_syscall::a0* retval = new literal_syscall::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_syscall::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void literal_syscall::a0::postprocess() {
        token_type = "literal_syscall::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    literal_syscall* literal_syscall::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("syscall");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("(");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        literal_integer *_t4 = literal_integer::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t6 = terminal::parse(",");
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        type *_t8 = type::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        literal_syscall::a0 *_t9 = literal_syscall::a0::parse();
        ows *_t10 = ows::parse();
        if(_t10 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t11 = terminal::parse(")");
        if(_t11 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_syscall* retval = new literal_syscall(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10, _t11);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_syscall::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8->to_string();
        if(t9 != nullptr) ans += t9->to_string();
        ans += t10->to_string();
        ans += t11->to_string();
        return ans;
    }

    void literal_syscall::postprocess() {
        token_type = "literal_syscall";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
        token_children.push_back(t8);
        t8->postprocess();
        if(t9 != nullptr) {
            token_children.push_back(t9);
            t9->postprocess();
        }
        token_children.push_back(t10);
        t10->postprocess();
        token_children.push_back(t11);
        t11->postprocess();
    }

    literal_hex::a0::b0* literal_hex::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b0* retval = new literal_hex::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b0::postprocess() {
        token_type = "literal_hex::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b1* literal_hex::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("a");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b1* retval = new literal_hex::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b1::postprocess() {
        token_type = "literal_hex::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b2* literal_hex::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("b");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b2* retval = new literal_hex::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b2::postprocess() {
        token_type = "literal_hex::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b3* literal_hex::a0::b3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("c");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b3* retval = new literal_hex::a0::b3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b3::postprocess() {
        token_type = "literal_hex::a0::b3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b4* literal_hex::a0::b4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("d");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b4* retval = new literal_hex::a0::b4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b4::postprocess() {
        token_type = "literal_hex::a0::b4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b5* literal_hex::a0::b5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("e");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b5* retval = new literal_hex::a0::b5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b5::postprocess() {
        token_type = "literal_hex::a0::b5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b6* literal_hex::a0::b6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("f");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b6* retval = new literal_hex::a0::b6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b6::postprocess() {
        token_type = "literal_hex::a0::b6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b7* literal_hex::a0::b7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("A");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b7* retval = new literal_hex::a0::b7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b7::postprocess() {
        token_type = "literal_hex::a0::b7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b8* literal_hex::a0::b8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("B");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b8* retval = new literal_hex::a0::b8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b8::postprocess() {
        token_type = "literal_hex::a0::b8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b9* literal_hex::a0::b9::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("C");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b9* retval = new literal_hex::a0::b9(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b9::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b9::postprocess() {
        token_type = "literal_hex::a0::b9";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b10* literal_hex::a0::b10::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("D");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b10* retval = new literal_hex::a0::b10(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b10::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b10::postprocess() {
        token_type = "literal_hex::a0::b10";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b11* literal_hex::a0::b11::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("E");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b11* retval = new literal_hex::a0::b11(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b11::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b11::postprocess() {
        token_type = "literal_hex::a0::b11";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0::b12* literal_hex::a0::b12::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("F");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex::a0::b12* retval = new literal_hex::a0::b12(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::a0::b12::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_hex::a0::b12::postprocess() {
        token_type = "literal_hex::a0::b12";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_hex::a0* literal_hex::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = literal_hex::a0::b0::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b1::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b2::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b3::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b4::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b5::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b6::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b7::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b8::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b9::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b10::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b11::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_hex::a0::b12::parse()) {
            literal_hex::a0* retval = new literal_hex::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string literal_hex::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        if(is_b4) return t4->to_string();
        if(is_b5) return t5->to_string();
        if(is_b6) return t6->to_string();
        if(is_b7) return t7->to_string();
        if(is_b8) return t8->to_string();
        if(is_b9) return t9->to_string();
        if(is_b10) return t10->to_string();
        if(is_b11) return t11->to_string();
        if(is_b12) return t12->to_string();
        assert(false);
    }

    void literal_hex::a0::postprocess() {
        token_type = "literal_hex::a0";
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
        if(is_b9) {
            token_children.push_back(t9);
            t9->postprocess();
        }
        if(is_b10) {
            token_children.push_back(t10);
            t10->postprocess();
        }
        if(is_b11) {
            token_children.push_back(t11);
            t11->postprocess();
        }
        if(is_b12) {
            token_children.push_back(t12);
            t12->postprocess();
        }
    }
    literal_hex* literal_hex::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("0x");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<literal_hex::a0*> _t1;
        while(true) {
            literal_hex::a0 *tmp = literal_hex::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        if(_t1.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        literal_hex* retval = new literal_hex(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_hex::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void literal_hex::postprocess() {
        token_type = "literal_hex";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    literal_binary::a0::b0* literal_binary::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("0");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_binary::a0::b0* retval = new literal_binary::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_binary::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_binary::a0::b0::postprocess() {
        token_type = "literal_binary::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_binary::a0::b1* literal_binary::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("1");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_binary::a0::b1* retval = new literal_binary::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_binary::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal_binary::a0::b1::postprocess() {
        token_type = "literal_binary::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal_binary::a0* literal_binary::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = literal_binary::a0::b0::parse()) {
            literal_binary::a0* retval = new literal_binary::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal_binary::a0::b1::parse()) {
            literal_binary::a0* retval = new literal_binary::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string literal_binary::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void literal_binary::a0::postprocess() {
        token_type = "literal_binary::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    literal_binary* literal_binary::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("0b");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<literal_binary::a0*> _t1;
        while(true) {
            literal_binary::a0 *tmp = literal_binary::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        if(_t1.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        literal_binary* retval = new literal_binary(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_binary::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void literal_binary::postprocess() {
        token_type = "literal_binary";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    literal_function_pointer* literal_function_pointer::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("<");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t3 = identifier::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t5 = terminal::parse("(");
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        ows *_t6 = ows::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        type_list *_t7 = type_list::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        ows *_t8 = ows::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t9 = terminal::parse(")");
        if(_t9 == nullptr) {pop_stack(); return nullptr;}
        ows *_t10 = ows::parse();
        if(_t10 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t11 = terminal::parse(">");
        if(_t11 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal_function_pointer* retval = new literal_function_pointer(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10, _t11);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal_function_pointer::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8->to_string();
        ans += t9->to_string();
        ans += t10->to_string();
        ans += t11->to_string();
        return ans;
    }

    void literal_function_pointer::postprocess() {
        token_type = "literal_function_pointer";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
        token_children.push_back(t8);
        t8->postprocess();
        token_children.push_back(t9);
        t9->postprocess();
        token_children.push_back(t10);
        t10->postprocess();
        token_children.push_back(t11);
        t11->postprocess();
    }

    literal::a0* literal::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_hex *_t0 = literal_hex::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal::a0* retval = new literal::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal::a0::postprocess() {
        token_type = "literal::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal::a1* literal::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_binary *_t0 = literal_binary::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal::a1* retval = new literal::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal::a1::postprocess() {
        token_type = "literal::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal::a2* literal::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_float *_t0 = literal_float::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal::a2* retval = new literal::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal::a2::postprocess() {
        token_type = "literal::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal::a3* literal::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_integer *_t0 = literal_integer::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal::a3* retval = new literal::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal::a3::postprocess() {
        token_type = "literal::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal::a4* literal::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_sizeof *_t0 = literal_sizeof::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal::a4* retval = new literal::a4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal::a4::postprocess() {
        token_type = "literal::a4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal::a5* literal::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_char *_t0 = literal_char::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal::a5* retval = new literal::a5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal::a5::postprocess() {
        token_type = "literal::a5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal::a6* literal::a6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_string *_t0 = literal_string::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal::a6* retval = new literal::a6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal::a6::postprocess() {
        token_type = "literal::a6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal::a7* literal::a7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_syscall *_t0 = literal_syscall::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal::a7* retval = new literal::a7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal::a7::postprocess() {
        token_type = "literal::a7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal::a8* literal::a8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_function_pointer *_t0 = literal_function_pointer::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        literal::a8* retval = new literal::a8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string literal::a8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void literal::a8::postprocess() {
        token_type = "literal::a8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    literal* literal::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = literal::a0::parse()) {
            literal* retval = new literal(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal::a1::parse()) {
            literal* retval = new literal(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal::a2::parse()) {
            literal* retval = new literal(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal::a3::parse()) {
            literal* retval = new literal(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal::a4::parse()) {
            literal* retval = new literal(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal::a5::parse()) {
            literal* retval = new literal(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal::a6::parse()) {
            literal* retval = new literal(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal::a7::parse()) {
            literal* retval = new literal(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = literal::a8::parse()) {
            literal* retval = new literal(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string literal::to_string() {
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

    void literal::postprocess() {
        token_type = "literal";
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
    member_variable_declaration* member_variable_declaration::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t3 = terminal::parse(";");
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        member_variable_declaration* retval = new member_variable_declaration(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string member_variable_declaration::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void member_variable_declaration::postprocess() {
        token_type = "member_variable_declaration";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    constructor_definition* constructor_definition::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        base_type *_t0 = base_type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("(");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        parameter_list *_t4 = parameter_list::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t6 = terminal::parse(")");
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        constructor_definition* retval = new constructor_definition(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string constructor_definition::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        return ans;
    }

    void constructor_definition::postprocess() {
        token_type = "constructor_definition";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
    }

    constructor* constructor::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        constructor_definition *_t0 = constructor_definition::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        compound_statement *_t2 = compound_statement::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        constructor* retval = new constructor(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string constructor::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void constructor::postprocess() {
        token_type = "constructor";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    constructor_call* constructor_call::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("new");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        type *_t2 = type::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse("(");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        argument_list *_t6 = argument_list::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t8 = terminal::parse(")");
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        constructor_call* retval = new constructor_call(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string constructor_call::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8->to_string();
        return ans;
    }

    void constructor_call::postprocess() {
        token_type = "constructor_call";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
        token_children.push_back(t8);
        t8->postprocess();
    }

    destructor* destructor::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("~");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        base_type *_t1 = base_type::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t3 = terminal::parse("()");
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        compound_statement *_t5 = compound_statement::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        destructor* retval = new destructor(_t0, _t1, _t2, _t3, _t4, _t5);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string destructor::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        return ans;
    }

    void destructor::postprocess() {
        token_type = "destructor";
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
        token_children.push_back(t5);
        t5->postprocess();
    }

    struct_definition::a0::b0::c0* struct_definition::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        member_variable_declaration *_t0 = member_variable_declaration::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        struct_definition::a0::b0::c0* retval = new struct_definition::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string struct_definition::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void struct_definition::a0::b0::c0::postprocess() {
        token_type = "struct_definition::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    struct_definition::a0::b0::c1* struct_definition::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        templated_function *_t0 = templated_function::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        struct_definition::a0::b0::c1* retval = new struct_definition::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string struct_definition::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void struct_definition::a0::b0::c1::postprocess() {
        token_type = "struct_definition::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    struct_definition::a0::b0::c2* struct_definition::a0::b0::c2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        constructor *_t0 = constructor::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        struct_definition::a0::b0::c2* retval = new struct_definition::a0::b0::c2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string struct_definition::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void struct_definition::a0::b0::c2::postprocess() {
        token_type = "struct_definition::a0::b0::c2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    struct_definition::a0::b0::c3* struct_definition::a0::b0::c3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        destructor *_t0 = destructor::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        struct_definition::a0::b0::c3* retval = new struct_definition::a0::b0::c3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string struct_definition::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void struct_definition::a0::b0::c3::postprocess() {
        token_type = "struct_definition::a0::b0::c3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    struct_definition::a0::b0* struct_definition::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = struct_definition::a0::b0::c0::parse()) {
            struct_definition::a0::b0* retval = new struct_definition::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = struct_definition::a0::b0::c1::parse()) {
            struct_definition::a0::b0* retval = new struct_definition::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = struct_definition::a0::b0::c2::parse()) {
            struct_definition::a0::b0* retval = new struct_definition::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = struct_definition::a0::b0::c3::parse()) {
            struct_definition::a0::b0* retval = new struct_definition::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string struct_definition::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        assert(false);
    }

    void struct_definition::a0::b0::postprocess() {
        token_type = "struct_definition::a0::b0";
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
    }
    struct_definition::a0* struct_definition::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        struct_definition::a0::b0 *_t0 = struct_definition::a0::b0::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        struct_definition::a0* retval = new struct_definition::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string struct_definition::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void struct_definition::a0::postprocess() {
        token_type = "struct_definition::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    struct_definition* struct_definition::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("struct");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        base_type *_t2 = base_type::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse("{");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::vector<struct_definition::a0*> _t6;
        while(true) {
            struct_definition::a0 *tmp = struct_definition::a0::parse();
            if(tmp == nullptr) break;
            _t6.push_back(tmp);
        }
        terminal *_t7 = terminal::parse("}");
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        struct_definition* retval = new struct_definition(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string struct_definition::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        for(int i = 0; i < t6.size(); i++) ans += t6[i]->to_string();
        ans += t7->to_string();
        return ans;
    }

    void struct_definition::postprocess() {
        token_type = "struct_definition";
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
        token_children.push_back(t5);
        t5->postprocess();
        for(int i = 0; i < t6.size(); i++) {
            token_children.push_back(t6[i]);
            t6[i]->postprocess();
        }
        token_children.push_back(t7);
        t7->postprocess();
    }

    templated_struct_definition* templated_struct_definition::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        template_header *_t0 = template_header::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        struct_definition *_t2 = struct_definition::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        templated_struct_definition* retval = new templated_struct_definition(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string templated_struct_definition::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void templated_struct_definition::postprocess() {
        token_type = "templated_struct_definition";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    expr_primary::a0* expr_primary::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal *_t0 = literal::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_primary::a0* retval = new expr_primary::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_primary::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_primary::a0::postprocess() {
        token_type = "expr_primary::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_primary::a1* expr_primary::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        constructor_call *_t0 = constructor_call::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_primary::a1* retval = new expr_primary::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_primary::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_primary::a1::postprocess() {
        token_type = "expr_primary::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_primary::a2* expr_primary::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        function_call *_t0 = function_call::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_primary::a2* retval = new expr_primary::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_primary::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_primary::a2::postprocess() {
        token_type = "expr_primary::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_primary::a3* expr_primary::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        identifier *_t0 = identifier::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_primary::a3* retval = new expr_primary::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_primary::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_primary::a3::postprocess() {
        token_type = "expr_primary::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_primary::a4* expr_primary::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("(");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        expression *_t2 = expression::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse(")");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_primary::a4* retval = new expr_primary::a4(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_primary::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void expr_primary::a4::postprocess() {
        token_type = "expr_primary::a4";
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

    expr_primary* expr_primary::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_primary::a0::parse()) {
            expr_primary* retval = new expr_primary(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_primary::a1::parse()) {
            expr_primary* retval = new expr_primary(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_primary::a2::parse()) {
            expr_primary* retval = new expr_primary(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_primary::a3::parse()) {
            expr_primary* retval = new expr_primary(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_primary::a4::parse()) {
            expr_primary* retval = new expr_primary(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_primary::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        if(is_a4) return t4->to_string();
        assert(false);
    }

    void expr_primary::postprocess() {
        token_type = "expr_primary";
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
    }
    expr_postfix::a0::b0::c0* expr_postfix::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("[");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        expression *_t2 = expression::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse("]");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c0* retval = new expr_postfix::a0::b0::c0(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c0::postprocess() {
        token_type = "expr_postfix::a0::b0::c0";
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

    expr_postfix::a0::b0::c1* expr_postfix::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(".");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        function_call *_t2 = function_call::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c1* retval = new expr_postfix::a0::b0::c1(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c1::postprocess() {
        token_type = "expr_postfix::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    expr_postfix::a0::b0::c2* expr_postfix::a0::b0::c2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("->");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        function_call *_t2 = function_call::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c2* retval = new expr_postfix::a0::b0::c2(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c2::postprocess() {
        token_type = "expr_postfix::a0::b0::c2";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    expr_postfix::a0::b0::c3* expr_postfix::a0::b0::c3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(".");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c3* retval = new expr_postfix::a0::b0::c3(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c3::postprocess() {
        token_type = "expr_postfix::a0::b0::c3";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    expr_postfix::a0::b0::c4* expr_postfix::a0::b0::c4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("->");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c4* retval = new expr_postfix::a0::b0::c4(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c4::postprocess() {
        token_type = "expr_postfix::a0::b0::c4";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    expr_postfix::a0::b0::c5* expr_postfix::a0::b0::c5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("++");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c5* retval = new expr_postfix::a0::b0::c5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c5::postprocess() {
        token_type = "expr_postfix::a0::b0::c5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_postfix::a0::b0::c6* expr_postfix::a0::b0::c6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("--");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c6* retval = new expr_postfix::a0::b0::c6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c6::postprocess() {
        token_type = "expr_postfix::a0::b0::c6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_postfix::a0::b0::c7* expr_postfix::a0::b0::c7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#(");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        argument_list *_t1 = argument_list::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse(")");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c7* retval = new expr_postfix::a0::b0::c7(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c7::postprocess() {
        token_type = "expr_postfix::a0::b0::c7";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    expr_postfix::a0::b0::c8* expr_postfix::a0::b0::c8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(".");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("~()");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c8* retval = new expr_postfix::a0::b0::c8(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c8::postprocess() {
        token_type = "expr_postfix::a0::b0::c8";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    expr_postfix::a0::b0::c9* expr_postfix::a0::b0::c9::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("->");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("~()");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0::b0::c9* retval = new expr_postfix::a0::b0::c9(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::b0::c9::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void expr_postfix::a0::b0::c9::postprocess() {
        token_type = "expr_postfix::a0::b0::c9";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    expr_postfix::a0::b0* expr_postfix::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_postfix::a0::b0::c0::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_postfix::a0::b0::c1::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_postfix::a0::b0::c2::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_postfix::a0::b0::c3::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_postfix::a0::b0::c4::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_postfix::a0::b0::c5::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_postfix::a0::b0::c6::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_postfix::a0::b0::c7::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_postfix::a0::b0::c8::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_postfix::a0::b0::c9::parse()) {
            expr_postfix::a0::b0* retval = new expr_postfix::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_postfix::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        if(is_c4) return t4->to_string();
        if(is_c5) return t5->to_string();
        if(is_c6) return t6->to_string();
        if(is_c7) return t7->to_string();
        if(is_c8) return t8->to_string();
        if(is_c9) return t9->to_string();
        assert(false);
    }

    void expr_postfix::a0::b0::postprocess() {
        token_type = "expr_postfix::a0::b0";
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
        if(is_c6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
        if(is_c7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_c8) {
            token_children.push_back(t8);
            t8->postprocess();
        }
        if(is_c9) {
            token_children.push_back(t9);
            t9->postprocess();
        }
    }
    expr_postfix::a0* expr_postfix::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expr_postfix::a0::b0 *_t1 = expr_postfix::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_postfix::a0* retval = new expr_postfix::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void expr_postfix::a0::postprocess() {
        token_type = "expr_postfix::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    expr_postfix* expr_postfix::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_primary *_t0 = expr_primary::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_postfix::a0*> _t1;
        while(true) {
            expr_postfix::a0 *tmp = expr_postfix::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_postfix* retval = new expr_postfix(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_postfix::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_postfix::postprocess() {
        token_type = "expr_postfix";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_unary::a0::b0::c0* expr_unary::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("++");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0::b0::c0* retval = new expr_unary::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_unary::a0::b0::c0::postprocess() {
        token_type = "expr_unary::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_unary::a0::b0::c1* expr_unary::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("--");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0::b0::c1* retval = new expr_unary::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_unary::a0::b0::c1::postprocess() {
        token_type = "expr_unary::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_unary::a0::b0::c2* expr_unary::a0::b0::c2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("+");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0::b0::c2* retval = new expr_unary::a0::b0::c2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_unary::a0::b0::c2::postprocess() {
        token_type = "expr_unary::a0::b0::c2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_unary::a0::b0::c3* expr_unary::a0::b0::c3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("-");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0::b0::c3* retval = new expr_unary::a0::b0::c3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_unary::a0::b0::c3::postprocess() {
        token_type = "expr_unary::a0::b0::c3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_unary::a0::b0::c4* expr_unary::a0::b0::c4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("~");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0::b0::c4* retval = new expr_unary::a0::b0::c4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::b0::c4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_unary::a0::b0::c4::postprocess() {
        token_type = "expr_unary::a0::b0::c4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_unary::a0::b0::c5* expr_unary::a0::b0::c5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("!");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0::b0::c5* retval = new expr_unary::a0::b0::c5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::b0::c5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_unary::a0::b0::c5::postprocess() {
        token_type = "expr_unary::a0::b0::c5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_unary::a0::b0::c6* expr_unary::a0::b0::c6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0::b0::c6* retval = new expr_unary::a0::b0::c6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::b0::c6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_unary::a0::b0::c6::postprocess() {
        token_type = "expr_unary::a0::b0::c6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_unary::a0::b0::c7* expr_unary::a0::b0::c7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("@");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0::b0::c7* retval = new expr_unary::a0::b0::c7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::b0::c7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_unary::a0::b0::c7::postprocess() {
        token_type = "expr_unary::a0::b0::c7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_unary::a0::b0::c8* expr_unary::a0::b0::c8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("$");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        type *_t1 = type::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0::b0::c8* retval = new expr_unary::a0::b0::c8(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::b0::c8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void expr_unary::a0::b0::c8::postprocess() {
        token_type = "expr_unary::a0::b0::c8";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    expr_unary::a0::b0* expr_unary::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_unary::a0::b0::c0::parse()) {
            expr_unary::a0::b0* retval = new expr_unary::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_unary::a0::b0::c1::parse()) {
            expr_unary::a0::b0* retval = new expr_unary::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_unary::a0::b0::c2::parse()) {
            expr_unary::a0::b0* retval = new expr_unary::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_unary::a0::b0::c3::parse()) {
            expr_unary::a0::b0* retval = new expr_unary::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_unary::a0::b0::c4::parse()) {
            expr_unary::a0::b0* retval = new expr_unary::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_unary::a0::b0::c5::parse()) {
            expr_unary::a0::b0* retval = new expr_unary::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_unary::a0::b0::c6::parse()) {
            expr_unary::a0::b0* retval = new expr_unary::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_unary::a0::b0::c7::parse()) {
            expr_unary::a0::b0* retval = new expr_unary::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_unary::a0::b0::c8::parse()) {
            expr_unary::a0::b0* retval = new expr_unary::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_unary::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        if(is_c4) return t4->to_string();
        if(is_c5) return t5->to_string();
        if(is_c6) return t6->to_string();
        if(is_c7) return t7->to_string();
        if(is_c8) return t8->to_string();
        assert(false);
    }

    void expr_unary::a0::b0::postprocess() {
        token_type = "expr_unary::a0::b0";
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
        if(is_c6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
        if(is_c7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_c8) {
            token_children.push_back(t8);
            t8->postprocess();
        }
    }
    expr_unary::a0* expr_unary::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_unary::a0::b0 *_t0 = expr_unary::a0::b0::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        expr_unary *_t2 = expr_unary::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a0* retval = new expr_unary::a0(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void expr_unary::a0::postprocess() {
        token_type = "expr_unary::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    expr_unary::a1* expr_unary::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_postfix *_t0 = expr_postfix::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_unary::a1* retval = new expr_unary::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_unary::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_unary::a1::postprocess() {
        token_type = "expr_unary::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_unary* expr_unary::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_unary::a0::parse()) {
            expr_unary* retval = new expr_unary(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_unary::a1::parse()) {
            expr_unary* retval = new expr_unary(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_unary::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        assert(false);
    }

    void expr_unary::postprocess() {
        token_type = "expr_unary";
        if(is_a0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_a1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    expr_multiplicative::a0::b0::c0* expr_multiplicative::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_multiplicative::a0::b0::c0* retval = new expr_multiplicative::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_multiplicative::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_multiplicative::a0::b0::c0::postprocess() {
        token_type = "expr_multiplicative::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_multiplicative::a0::b0::c1* expr_multiplicative::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("/");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_multiplicative::a0::b0::c1* retval = new expr_multiplicative::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_multiplicative::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_multiplicative::a0::b0::c1::postprocess() {
        token_type = "expr_multiplicative::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_multiplicative::a0::b0::c2* expr_multiplicative::a0::b0::c2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("%");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_multiplicative::a0::b0::c2* retval = new expr_multiplicative::a0::b0::c2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_multiplicative::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_multiplicative::a0::b0::c2::postprocess() {
        token_type = "expr_multiplicative::a0::b0::c2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_multiplicative::a0::b0* expr_multiplicative::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_multiplicative::a0::b0::c0::parse()) {
            expr_multiplicative::a0::b0* retval = new expr_multiplicative::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_multiplicative::a0::b0::c1::parse()) {
            expr_multiplicative::a0::b0* retval = new expr_multiplicative::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_multiplicative::a0::b0::c2::parse()) {
            expr_multiplicative::a0::b0* retval = new expr_multiplicative::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_multiplicative::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        assert(false);
    }

    void expr_multiplicative::a0::b0::postprocess() {
        token_type = "expr_multiplicative::a0::b0";
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
    }
    expr_multiplicative::a0* expr_multiplicative::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expr_multiplicative::a0::b0 *_t1 = expr_multiplicative::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_unary *_t3 = expr_unary::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_multiplicative::a0* retval = new expr_multiplicative::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_multiplicative::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_multiplicative::a0::postprocess() {
        token_type = "expr_multiplicative::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_multiplicative* expr_multiplicative::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_unary *_t0 = expr_unary::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_multiplicative::a0*> _t1;
        while(true) {
            expr_multiplicative::a0 *tmp = expr_multiplicative::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_multiplicative* retval = new expr_multiplicative(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_multiplicative::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_multiplicative::postprocess() {
        token_type = "expr_multiplicative";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_additive::a0::b0::c0* expr_additive::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("+");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_additive::a0::b0::c0* retval = new expr_additive::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_additive::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_additive::a0::b0::c0::postprocess() {
        token_type = "expr_additive::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_additive::a0::b0::c1* expr_additive::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("-");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_additive::a0::b0::c1* retval = new expr_additive::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_additive::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_additive::a0::b0::c1::postprocess() {
        token_type = "expr_additive::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_additive::a0::b0* expr_additive::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_additive::a0::b0::c0::parse()) {
            expr_additive::a0::b0* retval = new expr_additive::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_additive::a0::b0::c1::parse()) {
            expr_additive::a0::b0* retval = new expr_additive::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_additive::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        assert(false);
    }

    void expr_additive::a0::b0::postprocess() {
        token_type = "expr_additive::a0::b0";
        if(is_c0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_c1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    expr_additive::a0* expr_additive::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expr_additive::a0::b0 *_t1 = expr_additive::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_multiplicative *_t3 = expr_multiplicative::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_additive::a0* retval = new expr_additive::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_additive::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_additive::a0::postprocess() {
        token_type = "expr_additive::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_additive* expr_additive::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_multiplicative *_t0 = expr_multiplicative::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_additive::a0*> _t1;
        while(true) {
            expr_additive::a0 *tmp = expr_additive::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_additive* retval = new expr_additive(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_additive::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_additive::postprocess() {
        token_type = "expr_additive";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_shift::a0::b0::c0* expr_shift::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<<");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_shift::a0::b0::c0* retval = new expr_shift::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_shift::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_shift::a0::b0::c0::postprocess() {
        token_type = "expr_shift::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_shift::a0::b0::c1* expr_shift::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">>");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_shift::a0::b0::c1* retval = new expr_shift::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_shift::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_shift::a0::b0::c1::postprocess() {
        token_type = "expr_shift::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_shift::a0::b0* expr_shift::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_shift::a0::b0::c0::parse()) {
            expr_shift::a0::b0* retval = new expr_shift::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_shift::a0::b0::c1::parse()) {
            expr_shift::a0::b0* retval = new expr_shift::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_shift::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        assert(false);
    }

    void expr_shift::a0::b0::postprocess() {
        token_type = "expr_shift::a0::b0";
        if(is_c0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_c1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    expr_shift::a0* expr_shift::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expr_shift::a0::b0 *_t1 = expr_shift::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_additive *_t3 = expr_additive::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_shift::a0* retval = new expr_shift::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_shift::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_shift::a0::postprocess() {
        token_type = "expr_shift::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_shift* expr_shift::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_additive *_t0 = expr_additive::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_shift::a0*> _t1;
        while(true) {
            expr_shift::a0 *tmp = expr_shift::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_shift* retval = new expr_shift(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_shift::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_shift::postprocess() {
        token_type = "expr_shift";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_relational::a0::b0::c0* expr_relational::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_relational::a0::b0::c0* retval = new expr_relational::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_relational::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_relational::a0::b0::c0::postprocess() {
        token_type = "expr_relational::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_relational::a0::b0::c1* expr_relational::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_relational::a0::b0::c1* retval = new expr_relational::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_relational::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_relational::a0::b0::c1::postprocess() {
        token_type = "expr_relational::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_relational::a0::b0::c2* expr_relational::a0::b0::c2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_relational::a0::b0::c2* retval = new expr_relational::a0::b0::c2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_relational::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_relational::a0::b0::c2::postprocess() {
        token_type = "expr_relational::a0::b0::c2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_relational::a0::b0::c3* expr_relational::a0::b0::c3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_relational::a0::b0::c3* retval = new expr_relational::a0::b0::c3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_relational::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_relational::a0::b0::c3::postprocess() {
        token_type = "expr_relational::a0::b0::c3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_relational::a0::b0* expr_relational::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_relational::a0::b0::c0::parse()) {
            expr_relational::a0::b0* retval = new expr_relational::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_relational::a0::b0::c1::parse()) {
            expr_relational::a0::b0* retval = new expr_relational::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_relational::a0::b0::c2::parse()) {
            expr_relational::a0::b0* retval = new expr_relational::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_relational::a0::b0::c3::parse()) {
            expr_relational::a0::b0* retval = new expr_relational::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_relational::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        assert(false);
    }

    void expr_relational::a0::b0::postprocess() {
        token_type = "expr_relational::a0::b0";
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
    }
    expr_relational::a0* expr_relational::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expr_relational::a0::b0 *_t1 = expr_relational::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_shift *_t3 = expr_shift::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_relational::a0* retval = new expr_relational::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_relational::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_relational::a0::postprocess() {
        token_type = "expr_relational::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_relational* expr_relational::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_shift *_t0 = expr_shift::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_relational::a0*> _t1;
        while(true) {
            expr_relational::a0 *tmp = expr_relational::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_relational* retval = new expr_relational(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_relational::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_relational::postprocess() {
        token_type = "expr_relational";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_equality::a0::b0::c0* expr_equality::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("==");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_equality::a0::b0::c0* retval = new expr_equality::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_equality::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_equality::a0::b0::c0::postprocess() {
        token_type = "expr_equality::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_equality::a0::b0::c1* expr_equality::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("!=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_equality::a0::b0::c1* retval = new expr_equality::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_equality::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_equality::a0::b0::c1::postprocess() {
        token_type = "expr_equality::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_equality::a0::b0* expr_equality::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_equality::a0::b0::c0::parse()) {
            expr_equality::a0::b0* retval = new expr_equality::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_equality::a0::b0::c1::parse()) {
            expr_equality::a0::b0* retval = new expr_equality::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_equality::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        assert(false);
    }

    void expr_equality::a0::b0::postprocess() {
        token_type = "expr_equality::a0::b0";
        if(is_c0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_c1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    expr_equality::a0* expr_equality::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expr_equality::a0::b0 *_t1 = expr_equality::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_relational *_t3 = expr_relational::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_equality::a0* retval = new expr_equality::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_equality::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_equality::a0::postprocess() {
        token_type = "expr_equality::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_equality* expr_equality::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_relational *_t0 = expr_relational::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_equality::a0*> _t1;
        while(true) {
            expr_equality::a0 *tmp = expr_equality::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_equality* retval = new expr_equality(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_equality::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_equality::postprocess() {
        token_type = "expr_equality";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_bit_and::a0* expr_bit_and::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("&");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_equality *_t3 = expr_equality::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_bit_and::a0* retval = new expr_bit_and::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_bit_and::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_bit_and::a0::postprocess() {
        token_type = "expr_bit_and::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_bit_and* expr_bit_and::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_equality *_t0 = expr_equality::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_bit_and::a0*> _t1;
        while(true) {
            expr_bit_and::a0 *tmp = expr_bit_and::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_bit_and* retval = new expr_bit_and(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_bit_and::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_bit_and::postprocess() {
        token_type = "expr_bit_and";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_bit_xor::a0* expr_bit_xor::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("^");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_bit_and *_t3 = expr_bit_and::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_bit_xor::a0* retval = new expr_bit_xor::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_bit_xor::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_bit_xor::a0::postprocess() {
        token_type = "expr_bit_xor::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_bit_xor* expr_bit_xor::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_bit_and *_t0 = expr_bit_and::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_bit_xor::a0*> _t1;
        while(true) {
            expr_bit_xor::a0 *tmp = expr_bit_xor::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_bit_xor* retval = new expr_bit_xor(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_bit_xor::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_bit_xor::postprocess() {
        token_type = "expr_bit_xor";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_bit_or::a0* expr_bit_or::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("|");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_bit_xor *_t3 = expr_bit_xor::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_bit_or::a0* retval = new expr_bit_or::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_bit_or::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_bit_or::a0::postprocess() {
        token_type = "expr_bit_or::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_bit_or* expr_bit_or::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_bit_xor *_t0 = expr_bit_xor::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_bit_or::a0*> _t1;
        while(true) {
            expr_bit_or::a0 *tmp = expr_bit_or::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_bit_or* retval = new expr_bit_or(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_bit_or::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_bit_or::postprocess() {
        token_type = "expr_bit_or";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_logical_and::a0* expr_logical_and::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("&&");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_bit_or *_t3 = expr_bit_or::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_logical_and::a0* retval = new expr_logical_and::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_logical_and::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_logical_and::a0::postprocess() {
        token_type = "expr_logical_and::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_logical_and* expr_logical_and::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_bit_or *_t0 = expr_bit_or::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_logical_and::a0*> _t1;
        while(true) {
            expr_logical_and::a0 *tmp = expr_logical_and::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_logical_and* retval = new expr_logical_and(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_logical_and::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_logical_and::postprocess() {
        token_type = "expr_logical_and";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_logical_or::a0* expr_logical_or::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("||");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_logical_and *_t3 = expr_logical_and::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_logical_or::a0* retval = new expr_logical_or::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_logical_or::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_logical_or::a0::postprocess() {
        token_type = "expr_logical_or::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_logical_or* expr_logical_or::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_logical_and *_t0 = expr_logical_and::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_logical_or::a0*> _t1;
        while(true) {
            expr_logical_or::a0 *tmp = expr_logical_or::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_logical_or* retval = new expr_logical_or(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_logical_or::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_logical_or::postprocess() {
        token_type = "expr_logical_or";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expr_assignment::a0::b0::c0* expr_assignment::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c0* retval = new expr_assignment::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c0::postprocess() {
        token_type = "expr_assignment::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c1* expr_assignment::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("+=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c1* retval = new expr_assignment::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c1::postprocess() {
        token_type = "expr_assignment::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c2* expr_assignment::a0::b0::c2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("-=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c2* retval = new expr_assignment::a0::b0::c2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c2::postprocess() {
        token_type = "expr_assignment::a0::b0::c2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c3* expr_assignment::a0::b0::c3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c3* retval = new expr_assignment::a0::b0::c3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c3::postprocess() {
        token_type = "expr_assignment::a0::b0::c3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c4* expr_assignment::a0::b0::c4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("/=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c4* retval = new expr_assignment::a0::b0::c4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c4::postprocess() {
        token_type = "expr_assignment::a0::b0::c4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c5* expr_assignment::a0::b0::c5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("%=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c5* retval = new expr_assignment::a0::b0::c5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c5::postprocess() {
        token_type = "expr_assignment::a0::b0::c5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c6* expr_assignment::a0::b0::c6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<<=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c6* retval = new expr_assignment::a0::b0::c6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c6::postprocess() {
        token_type = "expr_assignment::a0::b0::c6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c7* expr_assignment::a0::b0::c7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">>=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c7* retval = new expr_assignment::a0::b0::c7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c7::postprocess() {
        token_type = "expr_assignment::a0::b0::c7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c8* expr_assignment::a0::b0::c8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("&=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c8* retval = new expr_assignment::a0::b0::c8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c8::postprocess() {
        token_type = "expr_assignment::a0::b0::c8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c9* expr_assignment::a0::b0::c9::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("^=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c9* retval = new expr_assignment::a0::b0::c9(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c9::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c9::postprocess() {
        token_type = "expr_assignment::a0::b0::c9";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c10* expr_assignment::a0::b0::c10::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("|=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c10* retval = new expr_assignment::a0::b0::c10(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c10::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c10::postprocess() {
        token_type = "expr_assignment::a0::b0::c10";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0::c11* expr_assignment::a0::b0::c11::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(":=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0::b0::c11* retval = new expr_assignment::a0::b0::c11(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::b0::c11::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expr_assignment::a0::b0::c11::postprocess() {
        token_type = "expr_assignment::a0::b0::c11";
        token_children.push_back(t0);
        t0->postprocess();
    }

    expr_assignment::a0::b0* expr_assignment::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = expr_assignment::a0::b0::c0::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c1::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c2::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c3::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c4::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c5::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c6::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c7::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c8::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c9::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c10::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = expr_assignment::a0::b0::c11::parse()) {
            expr_assignment::a0::b0* retval = new expr_assignment::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string expr_assignment::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        if(is_c4) return t4->to_string();
        if(is_c5) return t5->to_string();
        if(is_c6) return t6->to_string();
        if(is_c7) return t7->to_string();
        if(is_c8) return t8->to_string();
        if(is_c9) return t9->to_string();
        if(is_c10) return t10->to_string();
        if(is_c11) return t11->to_string();
        assert(false);
    }

    void expr_assignment::a0::b0::postprocess() {
        token_type = "expr_assignment::a0::b0";
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
        if(is_c6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
        if(is_c7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_c8) {
            token_children.push_back(t8);
            t8->postprocess();
        }
        if(is_c9) {
            token_children.push_back(t9);
            t9->postprocess();
        }
        if(is_c10) {
            token_children.push_back(t10);
            t10->postprocess();
        }
        if(is_c11) {
            token_children.push_back(t11);
            t11->postprocess();
        }
    }
    expr_assignment::a0* expr_assignment::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expr_assignment::a0::b0 *_t1 = expr_assignment::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_logical_or *_t3 = expr_logical_or::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expr_assignment::a0* retval = new expr_assignment::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void expr_assignment::a0::postprocess() {
        token_type = "expr_assignment::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    expr_assignment* expr_assignment::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_logical_or *_t0 = expr_logical_or::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<expr_assignment::a0*> _t1;
        while(true) {
            expr_assignment::a0 *tmp = expr_assignment::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        expr_assignment* retval = new expr_assignment(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expr_assignment::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void expr_assignment::postprocess() {
        token_type = "expr_assignment";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    expression* expression::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expr_assignment *_t0 = expr_assignment::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        expression* retval = new expression(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string expression::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void expression::postprocess() {
        token_type = "expression";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a0* overloadable_operator::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("++x");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a0* retval = new overloadable_operator::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a0::postprocess() {
        token_type = "overloadable_operator::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a1* overloadable_operator::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("--x");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a1* retval = new overloadable_operator::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a1::postprocess() {
        token_type = "overloadable_operator::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a2* overloadable_operator::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("x++");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a2* retval = new overloadable_operator::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a2::postprocess() {
        token_type = "overloadable_operator::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a3* overloadable_operator::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("x--");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a3* retval = new overloadable_operator::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a3::postprocess() {
        token_type = "overloadable_operator::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a4* overloadable_operator::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*x");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a4* retval = new overloadable_operator::a4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a4::postprocess() {
        token_type = "overloadable_operator::a4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a5* overloadable_operator::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("+=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a5* retval = new overloadable_operator::a5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a5::postprocess() {
        token_type = "overloadable_operator::a5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a6* overloadable_operator::a6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("-=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a6* retval = new overloadable_operator::a6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a6::postprocess() {
        token_type = "overloadable_operator::a6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a7* overloadable_operator::a7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a7* retval = new overloadable_operator::a7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a7::postprocess() {
        token_type = "overloadable_operator::a7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a8* overloadable_operator::a8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("/=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a8* retval = new overloadable_operator::a8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a8::postprocess() {
        token_type = "overloadable_operator::a8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a9* overloadable_operator::a9::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("%=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a9* retval = new overloadable_operator::a9(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a9::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a9::postprocess() {
        token_type = "overloadable_operator::a9";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a10* overloadable_operator::a10::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("&=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a10* retval = new overloadable_operator::a10(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a10::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a10::postprocess() {
        token_type = "overloadable_operator::a10";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a11* overloadable_operator::a11::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("|=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a11* retval = new overloadable_operator::a11(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a11::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a11::postprocess() {
        token_type = "overloadable_operator::a11";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a12* overloadable_operator::a12::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("^=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a12* retval = new overloadable_operator::a12(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a12::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a12::postprocess() {
        token_type = "overloadable_operator::a12";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a13* overloadable_operator::a13::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<<=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a13* retval = new overloadable_operator::a13(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a13::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a13::postprocess() {
        token_type = "overloadable_operator::a13";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a14* overloadable_operator::a14::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">>=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a14* retval = new overloadable_operator::a14(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a14::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a14::postprocess() {
        token_type = "overloadable_operator::a14";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a15* overloadable_operator::a15::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("+");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a15* retval = new overloadable_operator::a15(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a15::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a15::postprocess() {
        token_type = "overloadable_operator::a15";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a16* overloadable_operator::a16::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("-");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a16* retval = new overloadable_operator::a16(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a16::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a16::postprocess() {
        token_type = "overloadable_operator::a16";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a17* overloadable_operator::a17::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a17* retval = new overloadable_operator::a17(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a17::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a17::postprocess() {
        token_type = "overloadable_operator::a17";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a18* overloadable_operator::a18::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("/");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a18* retval = new overloadable_operator::a18(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a18::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a18::postprocess() {
        token_type = "overloadable_operator::a18";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a19* overloadable_operator::a19::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("%");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a19* retval = new overloadable_operator::a19(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a19::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a19::postprocess() {
        token_type = "overloadable_operator::a19";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a20* overloadable_operator::a20::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("&");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a20* retval = new overloadable_operator::a20(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a20::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a20::postprocess() {
        token_type = "overloadable_operator::a20";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a21* overloadable_operator::a21::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("|");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a21* retval = new overloadable_operator::a21(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a21::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a21::postprocess() {
        token_type = "overloadable_operator::a21";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a22* overloadable_operator::a22::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("^");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a22* retval = new overloadable_operator::a22(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a22::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a22::postprocess() {
        token_type = "overloadable_operator::a22";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a23* overloadable_operator::a23::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<<");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a23* retval = new overloadable_operator::a23(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a23::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a23::postprocess() {
        token_type = "overloadable_operator::a23";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a24* overloadable_operator::a24::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">>");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a24* retval = new overloadable_operator::a24(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a24::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a24::postprocess() {
        token_type = "overloadable_operator::a24";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a25* overloadable_operator::a25::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("==");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a25* retval = new overloadable_operator::a25(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a25::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a25::postprocess() {
        token_type = "overloadable_operator::a25";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a26* overloadable_operator::a26::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("!=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a26* retval = new overloadable_operator::a26(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a26::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a26::postprocess() {
        token_type = "overloadable_operator::a26";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a27* overloadable_operator::a27::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a27* retval = new overloadable_operator::a27(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a27::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a27::postprocess() {
        token_type = "overloadable_operator::a27";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a28* overloadable_operator::a28::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a28* retval = new overloadable_operator::a28(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a28::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a28::postprocess() {
        token_type = "overloadable_operator::a28";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a29* overloadable_operator::a29::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a29* retval = new overloadable_operator::a29(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a29::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a29::postprocess() {
        token_type = "overloadable_operator::a29";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a30* overloadable_operator::a30::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a30* retval = new overloadable_operator::a30(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a30::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a30::postprocess() {
        token_type = "overloadable_operator::a30";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a31* overloadable_operator::a31::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("[]");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a31* retval = new overloadable_operator::a31(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a31::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a31::postprocess() {
        token_type = "overloadable_operator::a31";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator::a32* overloadable_operator::a32::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overloadable_operator::a32* retval = new overloadable_operator::a32(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overloadable_operator::a32::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void overloadable_operator::a32::postprocess() {
        token_type = "overloadable_operator::a32";
        token_children.push_back(t0);
        t0->postprocess();
    }

    overloadable_operator* overloadable_operator::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = overloadable_operator::a0::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a1::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a2::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a3::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a4::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a5::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a6::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a7::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a8::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a9::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a10::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a11::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a12::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a13::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a14::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a15::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a16::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a17::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a18::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a19::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a20::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a21::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a22::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a23::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a24::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a25::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a26::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a27::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a28::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a29::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a30::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a31::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = overloadable_operator::a32::parse()) {
            overloadable_operator* retval = new overloadable_operator(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string overloadable_operator::to_string() {
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
        assert(false);
    }

    void overloadable_operator::postprocess() {
        token_type = "overloadable_operator";
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
    }
    overload_definition* overload_definition::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("operator");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        overloadable_operator *_t3 = overloadable_operator::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t5 = terminal::parse("(");
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        ows *_t6 = ows::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        parameter_list *_t7 = parameter_list::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        ows *_t8 = ows::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t9 = terminal::parse(")");
        if(_t9 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overload_definition* retval = new overload_definition(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overload_definition::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8->to_string();
        ans += t9->to_string();
        return ans;
    }

    void overload_definition::postprocess() {
        token_type = "overload_definition";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
        token_children.push_back(t8);
        t8->postprocess();
        token_children.push_back(t9);
        t9->postprocess();
    }

    overload* overload::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        overload_definition *_t0 = overload_definition::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        compound_statement *_t2 = compound_statement::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        overload* retval = new overload(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string overload::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void overload::postprocess() {
        token_type = "overload";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    templated_overload* templated_overload::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        template_header *_t0 = template_header::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        overload *_t2 = overload::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        templated_overload* retval = new templated_overload(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string templated_overload::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void templated_overload::postprocess() {
        token_type = "templated_overload";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    inline_variable* inline_variable::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        identifier *_t0 = identifier::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_variable* retval = new inline_variable(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_variable::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_variable::postprocess() {
        token_type = "inline_variable";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_member_variable::a0::b0* inline_member_variable::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(".");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_member_variable::a0::b0* retval = new inline_member_variable::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_member_variable::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_member_variable::a0::b0::postprocess() {
        token_type = "inline_member_variable::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_member_variable::a0::b1* inline_member_variable::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("->");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_member_variable::a0::b1* retval = new inline_member_variable::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_member_variable::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_member_variable::a0::b1::postprocess() {
        token_type = "inline_member_variable::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_member_variable::a0* inline_member_variable::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = inline_member_variable::a0::b0::parse()) {
            inline_member_variable::a0* retval = new inline_member_variable::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_member_variable::a0::b1::parse()) {
            inline_member_variable::a0* retval = new inline_member_variable::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string inline_member_variable::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void inline_member_variable::a0::postprocess() {
        token_type = "inline_member_variable::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    inline_member_variable* inline_member_variable::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        identifier *_t0 = identifier::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        inline_member_variable::a0 *_t1 = inline_member_variable::a0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_member_variable* retval = new inline_member_variable(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_member_variable::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void inline_member_variable::postprocess() {
        token_type = "inline_member_variable";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    inline_dereferencing* inline_dereferencing::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_dereferencing* retval = new inline_dereferencing(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_dereferencing::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void inline_dereferencing::postprocess() {
        token_type = "inline_dereferencing";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    inline_symbol::a0* inline_symbol::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("[");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a0* retval = new inline_symbol::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a0::postprocess() {
        token_type = "inline_symbol::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a1* inline_symbol::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("]");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a1* retval = new inline_symbol::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a1::postprocess() {
        token_type = "inline_symbol::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a2* inline_symbol::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("(");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a2* retval = new inline_symbol::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a2::postprocess() {
        token_type = "inline_symbol::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a3* inline_symbol::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(")");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a3* retval = new inline_symbol::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a3::postprocess() {
        token_type = "inline_symbol::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a4* inline_symbol::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a4* retval = new inline_symbol::a4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a4::postprocess() {
        token_type = "inline_symbol::a4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a5* inline_symbol::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(">");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a5* retval = new inline_symbol::a5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a5::postprocess() {
        token_type = "inline_symbol::a5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a6* inline_symbol::a6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("=");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a6* retval = new inline_symbol::a6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a6::postprocess() {
        token_type = "inline_symbol::a6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a7* inline_symbol::a7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("|");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a7* retval = new inline_symbol::a7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a7::postprocess() {
        token_type = "inline_symbol::a7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a8* inline_symbol::a8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(".");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a8* retval = new inline_symbol::a8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a8::postprocess() {
        token_type = "inline_symbol::a8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a9* inline_symbol::a9::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(",");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a9* retval = new inline_symbol::a9(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a9::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a9::postprocess() {
        token_type = "inline_symbol::a9";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a10* inline_symbol::a10::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(";");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a10* retval = new inline_symbol::a10(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a10::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a10::postprocess() {
        token_type = "inline_symbol::a10";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a11* inline_symbol::a11::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("-");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a11* retval = new inline_symbol::a11(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a11::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a11::postprocess() {
        token_type = "inline_symbol::a11";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a12* inline_symbol::a12::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("+");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a12* retval = new inline_symbol::a12(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a12::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a12::postprocess() {
        token_type = "inline_symbol::a12";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a13* inline_symbol::a13::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("_");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a13* retval = new inline_symbol::a13(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a13::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a13::postprocess() {
        token_type = "inline_symbol::a13";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a14* inline_symbol::a14::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a14* retval = new inline_symbol::a14(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a14::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a14::postprocess() {
        token_type = "inline_symbol::a14";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a15* inline_symbol::a15::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("?");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a15* retval = new inline_symbol::a15(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a15::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a15::postprocess() {
        token_type = "inline_symbol::a15";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a16* inline_symbol::a16::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(":");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a16* retval = new inline_symbol::a16(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a16::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a16::postprocess() {
        token_type = "inline_symbol::a16";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a17* inline_symbol::a17::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("!");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a17* retval = new inline_symbol::a17(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a17::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a17::postprocess() {
        token_type = "inline_symbol::a17";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a18* inline_symbol::a18::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("@");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a18* retval = new inline_symbol::a18(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a18::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a18::postprocess() {
        token_type = "inline_symbol::a18";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a19* inline_symbol::a19::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a19* retval = new inline_symbol::a19(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a19::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a19::postprocess() {
        token_type = "inline_symbol::a19";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a20* inline_symbol::a20::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("$");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a20* retval = new inline_symbol::a20(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a20::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a20::postprocess() {
        token_type = "inline_symbol::a20";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a21* inline_symbol::a21::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("%");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a21* retval = new inline_symbol::a21(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a21::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a21::postprocess() {
        token_type = "inline_symbol::a21";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a22* inline_symbol::a22::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("^");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a22* retval = new inline_symbol::a22(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a22::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a22::postprocess() {
        token_type = "inline_symbol::a22";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a23* inline_symbol::a23::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("&");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a23* retval = new inline_symbol::a23(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a23::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a23::postprocess() {
        token_type = "inline_symbol::a23";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a24* inline_symbol::a24::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("/");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a24* retval = new inline_symbol::a24(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a24::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a24::postprocess() {
        token_type = "inline_symbol::a24";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a25* inline_symbol::a25::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("~");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a25* retval = new inline_symbol::a25(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a25::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a25::postprocess() {
        token_type = "inline_symbol::a25";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol::a26* inline_symbol::a26::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("`");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_symbol::a26* retval = new inline_symbol::a26(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_symbol::a26::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_symbol::a26::postprocess() {
        token_type = "inline_symbol::a26";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_symbol* inline_symbol::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = inline_symbol::a0::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a1::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a2::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a3::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a4::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a5::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a6::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a7::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a8::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a9::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a10::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a11::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a12::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a13::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a14::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a15::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a16::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a17::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a18::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a19::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a20::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a21::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a22::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a23::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a24::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a25::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_symbol::a26::parse()) {
            inline_symbol* retval = new inline_symbol(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string inline_symbol::to_string() {
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
        assert(false);
    }

    void inline_symbol::postprocess() {
        token_type = "inline_symbol";
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
    }
    inline_access::a0::b0* inline_access::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        inline_member_variable *_t0 = inline_member_variable::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_access::a0::b0* retval = new inline_access::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_access::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_access::a0::b0::postprocess() {
        token_type = "inline_access::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_access::a0::b1* inline_access::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        inline_dereferencing *_t0 = inline_dereferencing::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_access::a0::b1* retval = new inline_access::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_access::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_access::a0::b1::postprocess() {
        token_type = "inline_access::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_access::a0::b2* inline_access::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        inline_variable *_t0 = inline_variable::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_access::a0::b2* retval = new inline_access::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_access::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_access::a0::b2::postprocess() {
        token_type = "inline_access::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_access::a0* inline_access::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = inline_access::a0::b0::parse()) {
            inline_access::a0* retval = new inline_access::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_access::a0::b1::parse()) {
            inline_access::a0* retval = new inline_access::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_access::a0::b2::parse()) {
            inline_access::a0* retval = new inline_access::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string inline_access::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        assert(false);
    }

    void inline_access::a0::postprocess() {
        token_type = "inline_access::a0";
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
    }
    inline_access* inline_access::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("{");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        inline_access::a0 *_t2 = inline_access::a0::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse("}");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_access* retval = new inline_access(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_access::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void inline_access::postprocess() {
        token_type = "inline_access";
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

    inline_asm_string::a0::b0* inline_asm_string::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        inline_access *_t0 = inline_access::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_asm_string::a0::b0* retval = new inline_asm_string::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_asm_string::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_asm_string::a0::b0::postprocess() {
        token_type = "inline_asm_string::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_asm_string::a0::b1::c0::d0* inline_asm_string::a0::b1::c0::d0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_asm_string::a0::b1::c0::d0* retval = new inline_asm_string::a0::b1::c0::d0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_asm_string::a0::b1::c0::d0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_asm_string::a0::b1::c0::d0::postprocess() {
        token_type = "inline_asm_string::a0::b1::c0::d0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_asm_string::a0::b1::c0::d1* inline_asm_string::a0::b1::c0::d1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_asm_string::a0::b1::c0::d1* retval = new inline_asm_string::a0::b1::c0::d1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_asm_string::a0::b1::c0::d1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_asm_string::a0::b1::c0::d1::postprocess() {
        token_type = "inline_asm_string::a0::b1::c0::d1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_asm_string::a0::b1::c0::d2* inline_asm_string::a0::b1::c0::d2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        inline_symbol *_t0 = inline_symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_asm_string::a0::b1::c0::d2* retval = new inline_asm_string::a0::b1::c0::d2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_asm_string::a0::b1::c0::d2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_asm_string::a0::b1::c0::d2::postprocess() {
        token_type = "inline_asm_string::a0::b1::c0::d2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_asm_string::a0::b1::c0::d3* inline_asm_string::a0::b1::c0::d3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_asm_string::a0::b1::c0::d3* retval = new inline_asm_string::a0::b1::c0::d3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_asm_string::a0::b1::c0::d3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_asm_string::a0::b1::c0::d3::postprocess() {
        token_type = "inline_asm_string::a0::b1::c0::d3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_asm_string::a0::b1::c0* inline_asm_string::a0::b1::c0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = inline_asm_string::a0::b1::c0::d0::parse()) {
            inline_asm_string::a0::b1::c0* retval = new inline_asm_string::a0::b1::c0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_asm_string::a0::b1::c0::d1::parse()) {
            inline_asm_string::a0::b1::c0* retval = new inline_asm_string::a0::b1::c0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_asm_string::a0::b1::c0::d2::parse()) {
            inline_asm_string::a0::b1::c0* retval = new inline_asm_string::a0::b1::c0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_asm_string::a0::b1::c0::d3::parse()) {
            inline_asm_string::a0::b1::c0* retval = new inline_asm_string::a0::b1::c0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string inline_asm_string::a0::b1::c0::to_string() {
        if(is_d0) return t0->to_string();
        if(is_d1) return t1->to_string();
        if(is_d2) return t2->to_string();
        if(is_d3) return t3->to_string();
        assert(false);
    }

    void inline_asm_string::a0::b1::c0::postprocess() {
        token_type = "inline_asm_string::a0::b1::c0";
        if(is_d0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_d1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        if(is_d2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
        if(is_d3) {
            token_children.push_back(t3);
            t3->postprocess();
        }
    }
    inline_asm_string::a0::b1* inline_asm_string::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        inline_asm_string::a0::b1::c0 *_t0 = inline_asm_string::a0::b1::c0::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_asm_string::a0::b1* retval = new inline_asm_string::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_asm_string::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void inline_asm_string::a0::b1::postprocess() {
        token_type = "inline_asm_string::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    inline_asm_string::a0* inline_asm_string::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = inline_asm_string::a0::b0::parse()) {
            inline_asm_string::a0* retval = new inline_asm_string::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = inline_asm_string::a0::b1::parse()) {
            inline_asm_string::a0* retval = new inline_asm_string::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string inline_asm_string::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void inline_asm_string::a0::postprocess() {
        token_type = "inline_asm_string::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    inline_asm_string* inline_asm_string::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\"");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<inline_asm_string::a0*> _t1;
        while(true) {
            inline_asm_string::a0 *tmp = inline_asm_string::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        terminal *_t2 = terminal::parse("\"");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_asm_string* retval = new inline_asm_string(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_asm_string::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2->to_string();
        return ans;
    }

    void inline_asm_string::postprocess() {
        token_type = "inline_asm_string";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
        token_children.push_back(t2);
        t2->postprocess();
    }

    inline_asm* inline_asm::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("asm!");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("(");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        inline_asm_string *_t3 = inline_asm_string::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse(")");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        inline_asm* retval = new inline_asm(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string inline_asm::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void inline_asm::postprocess() {
        token_type = "inline_asm";
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

    escape::a0::b7* escape::a0::b7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("'");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape::a0::b7* retval = new escape::a0::b7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::a0::b7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void escape::a0::b7::postprocess() {
        token_type = "escape::a0::b7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    escape::a0::b8* escape::a0::b8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("0");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        escape::a0::b8* retval = new escape::a0::b8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string escape::a0::b8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void escape::a0::b8::postprocess() {
        token_type = "escape::a0::b8";
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
        if(auto x = escape::a0::b7::parse()) {
            escape::a0* retval = new escape::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = escape::a0::b8::parse()) {
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
        if(is_b7) return t7->to_string();
        if(is_b8) return t8->to_string();
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
        if(is_b7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_b8) {
            token_children.push_back(t8);
            t8->postprocess();
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
        terminal *_t0 = terminal::parse("#");
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
        terminal *_t0 = terminal::parse("$");
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
        terminal *_t0 = terminal::parse("%");
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
        terminal *_t0 = terminal::parse("^");
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
        terminal *_t0 = terminal::parse("&");
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
        terminal *_t0 = terminal::parse("/");
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
        terminal *_t0 = terminal::parse("~");
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

    symbol::a28* symbol::a28::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("`");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        symbol::a28* retval = new symbol::a28(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string symbol::a28::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void symbol::a28::postprocess() {
        token_type = "symbol::a28";
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
        if(auto x = symbol::a28::parse()) {
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
        if(is_a28) return t28->to_string();
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
        if(is_a28) {
            token_children.push_back(t28);
            t28->postprocess();
        }
    }
    line_comment::a0::b0* line_comment::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        line_comment::a0::b0* retval = new line_comment::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string line_comment::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void line_comment::a0::b0::postprocess() {
        token_type = "line_comment::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    line_comment::a0::b1* line_comment::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        line_comment::a0::b1* retval = new line_comment::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string line_comment::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void line_comment::a0::b1::postprocess() {
        token_type = "line_comment::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    line_comment::a0::b2* line_comment::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        line_comment::a0::b2* retval = new line_comment::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string line_comment::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void line_comment::a0::b2::postprocess() {
        token_type = "line_comment::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    line_comment::a0::b3* line_comment::a0::b3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\"");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        line_comment::a0::b3* retval = new line_comment::a0::b3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string line_comment::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void line_comment::a0::b3::postprocess() {
        token_type = "line_comment::a0::b3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    line_comment::a0::b4* line_comment::a0::b4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("'");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        line_comment::a0::b4* retval = new line_comment::a0::b4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string line_comment::a0::b4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void line_comment::a0::b4::postprocess() {
        token_type = "line_comment::a0::b4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    line_comment::a0::b5* line_comment::a0::b5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        line_comment::a0::b5* retval = new line_comment::a0::b5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string line_comment::a0::b5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void line_comment::a0::b5::postprocess() {
        token_type = "line_comment::a0::b5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    line_comment::a0::b6* line_comment::a0::b6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\\");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        line_comment::a0::b6* retval = new line_comment::a0::b6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string line_comment::a0::b6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void line_comment::a0::b6::postprocess() {
        token_type = "line_comment::a0::b6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    line_comment::a0* line_comment::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = line_comment::a0::b0::parse()) {
            line_comment::a0* retval = new line_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = line_comment::a0::b1::parse()) {
            line_comment::a0* retval = new line_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = line_comment::a0::b2::parse()) {
            line_comment::a0* retval = new line_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = line_comment::a0::b3::parse()) {
            line_comment::a0* retval = new line_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = line_comment::a0::b4::parse()) {
            line_comment::a0* retval = new line_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = line_comment::a0::b5::parse()) {
            line_comment::a0* retval = new line_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = line_comment::a0::b6::parse()) {
            line_comment::a0* retval = new line_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string line_comment::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        if(is_b4) return t4->to_string();
        if(is_b5) return t5->to_string();
        if(is_b6) return t6->to_string();
        assert(false);
    }

    void line_comment::a0::postprocess() {
        token_type = "line_comment::a0";
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
    line_comment* line_comment::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("//");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<line_comment::a0*> _t1;
        while(true) {
            line_comment::a0 *tmp = line_comment::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        terminal *_t2 = terminal::parse("\n");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        line_comment* retval = new line_comment(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string line_comment::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2->to_string();
        return ans;
    }

    void line_comment::postprocess() {
        token_type = "line_comment";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
        token_children.push_back(t2);
        t2->postprocess();
    }

    multiline_comment::a0::b0* multiline_comment::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        multiline_comment::a0::b0* retval = new multiline_comment::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string multiline_comment::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void multiline_comment::a0::b0::postprocess() {
        token_type = "multiline_comment::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    multiline_comment::a0::b1* multiline_comment::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        multiline_comment::a0::b1* retval = new multiline_comment::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string multiline_comment::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void multiline_comment::a0::b1::postprocess() {
        token_type = "multiline_comment::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    multiline_comment::a0::b2* multiline_comment::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        multiline_comment::a0::b2* retval = new multiline_comment::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string multiline_comment::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void multiline_comment::a0::b2::postprocess() {
        token_type = "multiline_comment::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    multiline_comment::a0::b3* multiline_comment::a0::b3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\n");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        multiline_comment::a0::b3* retval = new multiline_comment::a0::b3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string multiline_comment::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void multiline_comment::a0::b3::postprocess() {
        token_type = "multiline_comment::a0::b3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    multiline_comment::a0* multiline_comment::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = multiline_comment::a0::b0::parse()) {
            multiline_comment::a0* retval = new multiline_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = multiline_comment::a0::b1::parse()) {
            multiline_comment::a0* retval = new multiline_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = multiline_comment::a0::b2::parse()) {
            multiline_comment::a0* retval = new multiline_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = multiline_comment::a0::b3::parse()) {
            multiline_comment::a0* retval = new multiline_comment::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string multiline_comment::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        assert(false);
    }

    void multiline_comment::a0::postprocess() {
        token_type = "multiline_comment::a0";
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
    multiline_comment* multiline_comment::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("/*");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<multiline_comment::a0*> _t1;
        while(true) {
            multiline_comment::a0 *tmp = multiline_comment::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        terminal *_t2 = terminal::parse("*/");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        multiline_comment* retval = new multiline_comment(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string multiline_comment::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2->to_string();
        return ans;
    }

    void multiline_comment::postprocess() {
        token_type = "multiline_comment";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
        token_children.push_back(t2);
        t2->postprocess();
    }

    ws::a0* ws::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse(" ");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a0* retval = new ws::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a0::postprocess() {
        token_type = "ws::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a1* ws::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\n");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a1* retval = new ws::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a1::postprocess() {
        token_type = "ws::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a2* ws::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\t");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a2* retval = new ws::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a2::postprocess() {
        token_type = "ws::a2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a3* ws::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\r");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a3* retval = new ws::a3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a3::postprocess() {
        token_type = "ws::a3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a4* ws::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\f");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a4* retval = new ws::a4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a4::postprocess() {
        token_type = "ws::a4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a5* ws::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("\b");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a5* retval = new ws::a5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a5::postprocess() {
        token_type = "ws::a5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a6* ws::a6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        line_comment *_t0 = line_comment::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a6* retval = new ws::a6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a6::postprocess() {
        token_type = "ws::a6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws::a7* ws::a7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        multiline_comment *_t0 = multiline_comment::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ws::a7* retval = new ws::a7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ws::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ws::a7::postprocess() {
        token_type = "ws::a7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ws* ws::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = ws::a0::parse()) {
            ws* retval = new ws(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a1::parse()) {
            ws* retval = new ws(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a2::parse()) {
            ws* retval = new ws(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a3::parse()) {
            ws* retval = new ws(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a4::parse()) {
            ws* retval = new ws(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a5::parse()) {
            ws* retval = new ws(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a6::parse()) {
            ws* retval = new ws(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = ws::a7::parse()) {
            ws* retval = new ws(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string ws::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        if(is_a4) return t4->to_string();
        if(is_a5) return t5->to_string();
        if(is_a6) return t6->to_string();
        if(is_a7) return t7->to_string();
        assert(false);
    }

    void ws::postprocess() {
        token_type = "ws";
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
    }
    rws::a0* rws::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ws *_t0 = ws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        rws::a0* retval = new rws::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string rws::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void rws::a0::postprocess() {
        token_type = "rws::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    rws* rws::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::vector<rws::a0*> _t0;
        while(true) {
            rws::a0 *tmp = rws::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        if(_t0.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        rws* retval = new rws(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string rws::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        return ans;
    }

    void rws::postprocess() {
        token_type = "rws";
        for(int i = 0; i < t0.size(); i++) {
            token_children.push_back(t0[i]);
            t0[i]->postprocess();
        }
    }

    ows::a0* ows::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ws *_t0 = ws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        ows::a0* retval = new ows::a0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ows::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void ows::a0::postprocess() {
        token_type = "ows::a0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    ows* ows::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::vector<ows::a0*> _t0;
        while(true) {
            ows::a0 *tmp = ows::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        rm_stack();
        ows* retval = new ows(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string ows::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        return ans;
    }

    void ows::postprocess() {
        token_type = "ows";
        for(int i = 0; i < t0.size(); i++) {
            token_children.push_back(t0[i]);
            t0[i]->postprocess();
        }
    }

    base_type::a0::b0* base_type::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        base_type::a0::b0* retval = new base_type::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string base_type::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void base_type::a0::b0::postprocess() {
        token_type = "base_type::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    base_type::a0::b1* base_type::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        base_type::a0::b1* retval = new base_type::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string base_type::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void base_type::a0::b1::postprocess() {
        token_type = "base_type::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    base_type::a0::b2* base_type::a0::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("_");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        base_type::a0::b2* retval = new base_type::a0::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string base_type::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void base_type::a0::b2::postprocess() {
        token_type = "base_type::a0::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    base_type::a0* base_type::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = base_type::a0::b0::parse()) {
            base_type::a0* retval = new base_type::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = base_type::a0::b1::parse()) {
            base_type::a0* retval = new base_type::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = base_type::a0::b2::parse()) {
            base_type::a0* retval = new base_type::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string base_type::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        assert(false);
    }

    void base_type::a0::postprocess() {
        token_type = "base_type::a0";
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
    }
    base_type* base_type::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<base_type::a0*> _t1;
        while(true) {
            base_type::a0 *tmp = base_type::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        base_type* retval = new base_type(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string base_type::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void base_type::postprocess() {
        token_type = "base_type";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    templated_type::a0::b0* templated_type::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(",");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        templated_type *_t3 = templated_type::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        templated_type::a0::b0* retval = new templated_type::a0::b0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string templated_type::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void templated_type::a0::b0::postprocess() {
        token_type = "templated_type::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    templated_type::a0* templated_type::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        templated_type *_t2 = templated_type::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        std::vector<templated_type::a0::b0*> _t3;
        while(true) {
            templated_type::a0::b0 *tmp = templated_type::a0::b0::parse();
            if(tmp == nullptr) break;
            _t3.push_back(tmp);
        }
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t5 = terminal::parse(">");
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        templated_type::a0* retval = new templated_type::a0(_t0, _t1, _t2, _t3, _t4, _t5);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string templated_type::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        for(int i = 0; i < t3.size(); i++) ans += t3[i]->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        return ans;
    }

    void templated_type::a0::postprocess() {
        token_type = "templated_type::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        for(int i = 0; i < t3.size(); i++) {
            token_children.push_back(t3[i]);
            t3[i]->postprocess();
        }
        token_children.push_back(t4);
        t4->postprocess();
        token_children.push_back(t5);
        t5->postprocess();
    }

    templated_type::a1::b0* templated_type::a1::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("*");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        templated_type::a1::b0* retval = new templated_type::a1::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string templated_type::a1::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void templated_type::a1::b0::postprocess() {
        token_type = "templated_type::a1::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    templated_type::a1::b1* templated_type::a1::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("[");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        literal_integer *_t1 = literal_integer::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("]");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        templated_type::a1::b1* retval = new templated_type::a1::b1(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string templated_type::a1::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void templated_type::a1::b1::postprocess() {
        token_type = "templated_type::a1::b1";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    templated_type::a1* templated_type::a1::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = templated_type::a1::b0::parse()) {
            templated_type::a1* retval = new templated_type::a1(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = templated_type::a1::b1::parse()) {
            templated_type::a1* retval = new templated_type::a1(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string templated_type::a1::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void templated_type::a1::postprocess() {
        token_type = "templated_type::a1";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    templated_type* templated_type::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        base_type *_t0 = base_type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        templated_type::a0 *_t1 = templated_type::a0::parse();
        std::vector<templated_type::a1*> _t2;
        while(true) {
            templated_type::a1 *tmp = templated_type::a1::parse();
            if(tmp == nullptr) break;
            _t2.push_back(tmp);
        }
        rm_stack();
        templated_type* retval = new templated_type(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string templated_type::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        if(t1 != nullptr) ans += t1->to_string();
        for(int i = 0; i < t2.size(); i++) ans += t2[i]->to_string();
        return ans;
    }

    void templated_type::postprocess() {
        token_type = "templated_type";
        token_children.push_back(t0);
        t0->postprocess();
        if(t1 != nullptr) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        for(int i = 0; i < t2.size(); i++) {
            token_children.push_back(t2[i]);
            t2[i]->postprocess();
        }
    }

    function_pointer_type* function_pointer_type::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("fn");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("<");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        type *_t3 = type::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t5 = terminal::parse("(");
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        ows *_t6 = ows::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        type_list *_t7 = type_list::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        ows *_t8 = ows::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t9 = terminal::parse(")");
        if(_t9 == nullptr) {pop_stack(); return nullptr;}
        ows *_t10 = ows::parse();
        if(_t10 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t11 = terminal::parse(">");
        if(_t11 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        function_pointer_type* retval = new function_pointer_type(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10, _t11);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string function_pointer_type::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8->to_string();
        ans += t9->to_string();
        ans += t10->to_string();
        ans += t11->to_string();
        return ans;
    }

    void function_pointer_type::postprocess() {
        token_type = "function_pointer_type";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
        token_children.push_back(t8);
        t8->postprocess();
        token_children.push_back(t9);
        t9->postprocess();
        token_children.push_back(t10);
        t10->postprocess();
        token_children.push_back(t11);
        t11->postprocess();
    }

    type::a0::b0* type::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        function_pointer_type *_t0 = function_pointer_type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        type::a0::b0* retval = new type::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string type::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void type::a0::b0::postprocess() {
        token_type = "type::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    type::a0::b1* type::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        templated_type *_t0 = templated_type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        type::a0::b1* retval = new type::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string type::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void type::a0::b1::postprocess() {
        token_type = "type::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    type::a0* type::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = type::a0::b0::parse()) {
            type::a0* retval = new type::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = type::a0::b1::parse()) {
            type::a0* retval = new type::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string type::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void type::a0::postprocess() {
        token_type = "type::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    type::a1* type::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("&");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        type::a1* retval = new type::a1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string type::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void type::a1::postprocess() {
        token_type = "type::a1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    type* type::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        type::a0 *_t0 = type::a0::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        type::a1 *_t1 = type::a1::parse();
        rm_stack();
        type* retval = new type(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string type::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        if(t1 != nullptr) ans += t1->to_string();
        return ans;
    }

    void type::postprocess() {
        token_type = "type";
        token_children.push_back(t0);
        t0->postprocess();
        if(t1 != nullptr) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }

    template_header::a0* template_header::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(",");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        base_type *_t3 = base_type::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        template_header::a0* retval = new template_header::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string template_header::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void template_header::a0::postprocess() {
        token_type = "template_header::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    template_header* template_header::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("template");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("<");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        base_type *_t4 = base_type::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        std::vector<template_header::a0*> _t5;
        while(true) {
            template_header::a0 *tmp = template_header::a0::parse();
            if(tmp == nullptr) break;
            _t5.push_back(tmp);
        }
        ows *_t6 = ows::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t7 = terminal::parse(">");
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        template_header* retval = new template_header(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string template_header::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        for(int i = 0; i < t5.size(); i++) ans += t5[i]->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        return ans;
    }

    void template_header::postprocess() {
        token_type = "template_header";
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
        for(int i = 0; i < t5.size(); i++) {
            token_children.push_back(t5[i]);
            t5[i]->postprocess();
        }
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
    }

    identifier::a0::b0* identifier::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        identifier::a0::b0* retval = new identifier::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string identifier::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void identifier::a0::b0::postprocess() {
        token_type = "identifier::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    identifier::a0::b1* identifier::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("_");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        identifier::a0::b1* retval = new identifier::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string identifier::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void identifier::a0::b1::postprocess() {
        token_type = "identifier::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    identifier::a0* identifier::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = identifier::a0::b0::parse()) {
            identifier::a0* retval = new identifier::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = identifier::a0::b1::parse()) {
            identifier::a0* retval = new identifier::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string identifier::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void identifier::a0::postprocess() {
        token_type = "identifier::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    identifier::a1::b0* identifier::a1::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        identifier::a1::b0* retval = new identifier::a1::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string identifier::a1::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void identifier::a1::b0::postprocess() {
        token_type = "identifier::a1::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    identifier::a1::b1* identifier::a1::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        identifier::a1::b1* retval = new identifier::a1::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string identifier::a1::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void identifier::a1::b1::postprocess() {
        token_type = "identifier::a1::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    identifier::a1::b2* identifier::a1::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("_");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        identifier::a1::b2* retval = new identifier::a1::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string identifier::a1::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void identifier::a1::b2::postprocess() {
        token_type = "identifier::a1::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    identifier::a1* identifier::a1::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = identifier::a1::b0::parse()) {
            identifier::a1* retval = new identifier::a1(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = identifier::a1::b1::parse()) {
            identifier::a1* retval = new identifier::a1(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = identifier::a1::b2::parse()) {
            identifier::a1* retval = new identifier::a1(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string identifier::a1::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        assert(false);
    }

    void identifier::a1::postprocess() {
        token_type = "identifier::a1";
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
    }
    identifier* identifier::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        identifier::a0 *_t0 = identifier::a0::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<identifier::a1*> _t1;
        while(true) {
            identifier::a1 *tmp = identifier::a1::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        identifier* retval = new identifier(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string identifier::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void identifier::postprocess() {
        token_type = "identifier";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    declaration::a0* declaration::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("=");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expression *_t3 = expression::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        declaration::a0* retval = new declaration::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string declaration::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void declaration::a0::postprocess() {
        token_type = "declaration::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    declaration* declaration::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        declaration::a0 *_t3 = declaration::a0::parse();
        rm_stack();
        declaration* retval = new declaration(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string declaration::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        if(t3 != nullptr) ans += t3->to_string();
        return ans;
    }

    void declaration::postprocess() {
        token_type = "declaration";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        if(t3 != nullptr) {
            token_children.push_back(t3);
            t3->postprocess();
        }
    }

    parameter* parameter::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        parameter* retval = new parameter(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string parameter::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void parameter::postprocess() {
        token_type = "parameter";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    type_list::a0::b0* type_list::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(",");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        type *_t3 = type::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        type_list::a0::b0* retval = new type_list::a0::b0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string type_list::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void type_list::a0::b0::postprocess() {
        token_type = "type_list::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    type_list::a0* type_list::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<type_list::a0::b0*> _t1;
        while(true) {
            type_list::a0::b0 *tmp = type_list::a0::b0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        type_list::a0* retval = new type_list::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string type_list::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void type_list::a0::postprocess() {
        token_type = "type_list::a0";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    type_list* type_list::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        type_list::a0 *_t0 = type_list::a0::parse();
        rm_stack();
        type_list* retval = new type_list(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string type_list::to_string() {
        std::string ans = "";
        if(t0 != nullptr) ans += t0->to_string();
        return ans;
    }

    void type_list::postprocess() {
        token_type = "type_list";
        if(t0 != nullptr) {
            token_children.push_back(t0);
            t0->postprocess();
        }
    }

    parameter_list::a0::b0* parameter_list::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(",");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        parameter *_t3 = parameter::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        parameter_list::a0::b0* retval = new parameter_list::a0::b0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string parameter_list::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void parameter_list::a0::b0::postprocess() {
        token_type = "parameter_list::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    parameter_list::a0* parameter_list::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        parameter *_t0 = parameter::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<parameter_list::a0::b0*> _t1;
        while(true) {
            parameter_list::a0::b0 *tmp = parameter_list::a0::b0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        parameter_list::a0* retval = new parameter_list::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string parameter_list::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void parameter_list::a0::postprocess() {
        token_type = "parameter_list::a0";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    parameter_list* parameter_list::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        parameter_list::a0 *_t0 = parameter_list::a0::parse();
        rm_stack();
        parameter_list* retval = new parameter_list(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string parameter_list::to_string() {
        std::string ans = "";
        if(t0 != nullptr) ans += t0->to_string();
        return ans;
    }

    void parameter_list::postprocess() {
        token_type = "parameter_list";
        if(t0 != nullptr) {
            token_children.push_back(t0);
            t0->postprocess();
        }
    }

    argument_list::a0::b0* argument_list::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(",");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expression *_t3 = expression::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        argument_list::a0::b0* retval = new argument_list::a0::b0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string argument_list::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void argument_list::a0::b0::postprocess() {
        token_type = "argument_list::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    argument_list::a0* argument_list::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expression *_t0 = expression::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<argument_list::a0::b0*> _t1;
        while(true) {
            argument_list::a0::b0 *tmp = argument_list::a0::b0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        argument_list::a0* retval = new argument_list::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string argument_list::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void argument_list::a0::postprocess() {
        token_type = "argument_list::a0";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    argument_list* argument_list::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        argument_list::a0 *_t0 = argument_list::a0::parse();
        rm_stack();
        argument_list* retval = new argument_list(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string argument_list::to_string() {
        std::string ans = "";
        if(t0 != nullptr) ans += t0->to_string();
        return ans;
    }

    void argument_list::postprocess() {
        token_type = "argument_list";
        if(t0 != nullptr) {
            token_children.push_back(t0);
            t0->postprocess();
        }
    }

    identifier_list::a0::b0* identifier_list::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse(",");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t3 = identifier::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        identifier_list::a0::b0* retval = new identifier_list::a0::b0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string identifier_list::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void identifier_list::a0::b0::postprocess() {
        token_type = "identifier_list::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    identifier_list::a0* identifier_list::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        identifier *_t0 = identifier::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<identifier_list::a0::b0*> _t1;
        while(true) {
            identifier_list::a0::b0 *tmp = identifier_list::a0::b0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        identifier_list::a0* retval = new identifier_list::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string identifier_list::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    void identifier_list::a0::postprocess() {
        token_type = "identifier_list::a0";
        token_children.push_back(t0);
        t0->postprocess();
        for(int i = 0; i < t1.size(); i++) {
            token_children.push_back(t1[i]);
            t1[i]->postprocess();
        }
    }

    identifier_list* identifier_list::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        identifier_list::a0 *_t0 = identifier_list::a0::parse();
        rm_stack();
        identifier_list* retval = new identifier_list(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string identifier_list::to_string() {
        std::string ans = "";
        if(t0 != nullptr) ans += t0->to_string();
        return ans;
    }

    void identifier_list::postprocess() {
        token_type = "identifier_list";
        if(t0 != nullptr) {
            token_children.push_back(t0);
            t0->postprocess();
        }
    }

    statement::a0* statement::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        simple_statement *_t0 = simple_statement::parse();
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
        control_statement *_t0 = control_statement::parse();
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

    statement::a2* statement::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        compound_statement *_t0 = compound_statement::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        statement::a2* retval = new statement::a2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string statement::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void statement::a2::postprocess() {
        token_type = "statement::a2";
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
        if(auto x = statement::a2::parse()) {
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
        if(is_a2) return t2->to_string();
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
        if(is_a2) {
            token_children.push_back(t2);
            t2->postprocess();
        }
    }
    simple_statement::a0::b0* simple_statement::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        rws *_t0 = rws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expression *_t1 = expression::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        simple_statement::a0::b0* retval = new simple_statement::a0::b0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string simple_statement::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void simple_statement::a0::b0::postprocess() {
        token_type = "simple_statement::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    simple_statement::a0* simple_statement::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("return");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        simple_statement::a0::b0 *_t1 = simple_statement::a0::b0::parse();
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t3 = terminal::parse(";");
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        simple_statement::a0* retval = new simple_statement::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string simple_statement::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        if(t1 != nullptr) ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void simple_statement::a0::postprocess() {
        token_type = "simple_statement::a0";
        token_children.push_back(t0);
        t0->postprocess();
        if(t1 != nullptr) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    simple_statement::a1* simple_statement::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("break");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse(";");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        simple_statement::a1* retval = new simple_statement::a1(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string simple_statement::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void simple_statement::a1::postprocess() {
        token_type = "simple_statement::a1";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    simple_statement::a2* simple_statement::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("continue");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse(";");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        simple_statement::a2* retval = new simple_statement::a2(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string simple_statement::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void simple_statement::a2::postprocess() {
        token_type = "simple_statement::a2";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    simple_statement::a3* simple_statement::a3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        declaration *_t0 = declaration::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse(";");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        simple_statement::a3* retval = new simple_statement::a3(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string simple_statement::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void simple_statement::a3::postprocess() {
        token_type = "simple_statement::a3";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    simple_statement::a4* simple_statement::a4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expression *_t0 = expression::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse(";");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        simple_statement::a4* retval = new simple_statement::a4(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string simple_statement::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void simple_statement::a4::postprocess() {
        token_type = "simple_statement::a4";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    simple_statement::a5* simple_statement::a5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        inline_asm *_t0 = inline_asm::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse(";");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        simple_statement::a5* retval = new simple_statement::a5(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string simple_statement::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void simple_statement::a5::postprocess() {
        token_type = "simple_statement::a5";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    simple_statement* simple_statement::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = simple_statement::a0::parse()) {
            simple_statement* retval = new simple_statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = simple_statement::a1::parse()) {
            simple_statement* retval = new simple_statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = simple_statement::a2::parse()) {
            simple_statement* retval = new simple_statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = simple_statement::a3::parse()) {
            simple_statement* retval = new simple_statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = simple_statement::a4::parse()) {
            simple_statement* retval = new simple_statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = simple_statement::a5::parse()) {
            simple_statement* retval = new simple_statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string simple_statement::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        if(is_a4) return t4->to_string();
        if(is_a5) return t5->to_string();
        assert(false);
    }

    void simple_statement::postprocess() {
        token_type = "simple_statement";
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
    control_statement::a0::b0* control_statement::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        rws *_t0 = rws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("else");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rws *_t2 = rws::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        statement *_t3 = statement::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        control_statement::a0::b0* retval = new control_statement::a0::b0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string control_statement::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void control_statement::a0::b0::postprocess() {
        token_type = "control_statement::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    control_statement::a0* control_statement::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("if");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("(");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        expression *_t4 = expression::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t6 = terminal::parse(")");
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        statement *_t8 = statement::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        control_statement::a0::b0 *_t9 = control_statement::a0::b0::parse();
        rm_stack();
        control_statement::a0* retval = new control_statement::a0(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string control_statement::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8->to_string();
        if(t9 != nullptr) ans += t9->to_string();
        return ans;
    }

    void control_statement::a0::postprocess() {
        token_type = "control_statement::a0";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
        token_children.push_back(t8);
        t8->postprocess();
        if(t9 != nullptr) {
            token_children.push_back(t9);
            t9->postprocess();
        }
    }

    control_statement::a1* control_statement::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("while");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("(");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        expression *_t4 = expression::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t6 = terminal::parse(")");
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        statement *_t8 = statement::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        control_statement::a1* retval = new control_statement::a1(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string control_statement::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8->to_string();
        return ans;
    }

    void control_statement::a1::postprocess() {
        token_type = "control_statement::a1";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
        token_children.push_back(t8);
        t8->postprocess();
    }

    control_statement::a2::b0* control_statement::a2::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        declaration *_t0 = declaration::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        control_statement::a2::b0* retval = new control_statement::a2::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string control_statement::a2::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void control_statement::a2::b0::postprocess() {
        token_type = "control_statement::a2::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    control_statement::a2::b1* control_statement::a2::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expression *_t0 = expression::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        control_statement::a2::b1* retval = new control_statement::a2::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string control_statement::a2::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void control_statement::a2::b1::postprocess() {
        token_type = "control_statement::a2::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    control_statement::a2::b2* control_statement::a2::b2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        expression *_t0 = expression::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        control_statement::a2::b2* retval = new control_statement::a2::b2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string control_statement::a2::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void control_statement::a2::b2::postprocess() {
        token_type = "control_statement::a2::b2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    control_statement::a2* control_statement::a2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("for");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse("(");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        control_statement::a2::b0 *_t4 = control_statement::a2::b0::parse();
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t6 = terminal::parse(";");
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        control_statement::a2::b1 *_t8 = control_statement::a2::b1::parse();
        ows *_t9 = ows::parse();
        if(_t9 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t10 = terminal::parse(";");
        if(_t10 == nullptr) {pop_stack(); return nullptr;}
        ows *_t11 = ows::parse();
        if(_t11 == nullptr) {pop_stack(); return nullptr;}
        control_statement::a2::b2 *_t12 = control_statement::a2::b2::parse();
        ows *_t13 = ows::parse();
        if(_t13 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t14 = terminal::parse(")");
        if(_t14 == nullptr) {pop_stack(); return nullptr;}
        ows *_t15 = ows::parse();
        if(_t15 == nullptr) {pop_stack(); return nullptr;}
        statement *_t16 = statement::parse();
        if(_t16 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        control_statement::a2* retval = new control_statement::a2(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10, _t11, _t12, _t13, _t14, _t15, _t16);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string control_statement::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        if(t4 != nullptr) ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        if(t8 != nullptr) ans += t8->to_string();
        ans += t9->to_string();
        ans += t10->to_string();
        ans += t11->to_string();
        if(t12 != nullptr) ans += t12->to_string();
        ans += t13->to_string();
        ans += t14->to_string();
        ans += t15->to_string();
        ans += t16->to_string();
        return ans;
    }

    void control_statement::a2::postprocess() {
        token_type = "control_statement::a2";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
        if(t4 != nullptr) {
            token_children.push_back(t4);
            t4->postprocess();
        }
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
        token_children.push_back(t7);
        t7->postprocess();
        if(t8 != nullptr) {
            token_children.push_back(t8);
            t8->postprocess();
        }
        token_children.push_back(t9);
        t9->postprocess();
        token_children.push_back(t10);
        t10->postprocess();
        token_children.push_back(t11);
        t11->postprocess();
        if(t12 != nullptr) {
            token_children.push_back(t12);
            t12->postprocess();
        }
        token_children.push_back(t13);
        t13->postprocess();
        token_children.push_back(t14);
        t14->postprocess();
        token_children.push_back(t15);
        t15->postprocess();
        token_children.push_back(t16);
        t16->postprocess();
    }

    control_statement* control_statement::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = control_statement::a0::parse()) {
            control_statement* retval = new control_statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = control_statement::a1::parse()) {
            control_statement* retval = new control_statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = control_statement::a2::parse()) {
            control_statement* retval = new control_statement(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string control_statement::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        assert(false);
    }

    void control_statement::postprocess() {
        token_type = "control_statement";
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
    }
    compound_statement::a0* compound_statement::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        statement *_t0 = statement::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        compound_statement::a0* retval = new compound_statement::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string compound_statement::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void compound_statement::a0::postprocess() {
        token_type = "compound_statement::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    compound_statement* compound_statement::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("{");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::vector<compound_statement::a0*> _t2;
        while(true) {
            compound_statement::a0 *tmp = compound_statement::a0::parse();
            if(tmp == nullptr) break;
            _t2.push_back(tmp);
        }
        terminal *_t3 = terminal::parse("}");
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        compound_statement* retval = new compound_statement(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string compound_statement::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        for(int i = 0; i < t2.size(); i++) ans += t2[i]->to_string();
        ans += t3->to_string();
        return ans;
    }

    void compound_statement::postprocess() {
        token_type = "compound_statement";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        for(int i = 0; i < t2.size(); i++) {
            token_children.push_back(t2[i]);
            t2[i]->postprocess();
        }
        token_children.push_back(t3);
        t3->postprocess();
    }

    library_path* library_path::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("<");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t1 = identifier::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t2 = terminal::parse(">");
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        library_path* retval = new library_path(_t0, _t1, _t2);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string library_path::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    void library_path::postprocess() {
        token_type = "library_path";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
    }

    include::a0::b0* include::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        literal_string *_t0 = literal_string::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        include::a0::b0* retval = new include::a0::b0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string include::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void include::a0::b0::postprocess() {
        token_type = "include::a0::b0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    include::a0::b1* include::a0::b1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        library_path *_t0 = library_path::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        include::a0::b1* retval = new include::a0::b1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string include::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void include::a0::b1::postprocess() {
        token_type = "include::a0::b1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    include::a0* include::a0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = include::a0::b0::parse()) {
            include::a0* retval = new include::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = include::a0::b1::parse()) {
            include::a0* retval = new include::a0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string include::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    void include::a0::postprocess() {
        token_type = "include::a0";
        if(is_b0) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(is_b1) {
            token_children.push_back(t1);
            t1->postprocess();
        }
    }
    include* include::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#include");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        include::a0 *_t2 = include::a0::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse(";");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        include* retval = new include(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string include::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void include::postprocess() {
        token_type = "include";
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

    _typedef* _typedef::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("typedef");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        type *_t2 = type::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rws *_t3 = rws::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        base_type *_t4 = base_type::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t6 = terminal::parse(";");
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        _typedef* retval = new _typedef(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string _typedef::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6->to_string();
        return ans;
    }

    void _typedef::postprocess() {
        token_type = "_typedef";
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
        token_children.push_back(t5);
        t5->postprocess();
        token_children.push_back(t6);
        t6->postprocess();
    }

    global_node::a0* global_node::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t1 = terminal::parse("[");
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier_list *_t2 = identifier_list::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t3 = terminal::parse("]");
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        global_node::a0* retval = new global_node::a0(_t0, _t1, _t2, _t3);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string global_node::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    void global_node::a0::postprocess() {
        token_type = "global_node::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
    }

    global_node* global_node::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("#global_node");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        global_node::a0 *_t3 = global_node::a0::parse();
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t5 = terminal::parse(";");
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        global_node* retval = new global_node(_t0, _t1, _t2, _t3, _t4, _t5);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string global_node::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        if(t3 != nullptr) ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        return ans;
    }

    void global_node::postprocess() {
        token_type = "global_node";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
        token_children.push_back(t2);
        t2->postprocess();
        if(t3 != nullptr) {
            token_children.push_back(t3);
            t3->postprocess();
        }
        token_children.push_back(t4);
        t4->postprocess();
        token_children.push_back(t5);
        t5->postprocess();
    }

    global_declaration::a0* global_declaration::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("[");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse("]");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        global_declaration::a0* retval = new global_declaration::a0(_t0, _t1, _t2, _t3, _t4, _t5);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string global_declaration::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        return ans;
    }

    void global_declaration::a0::postprocess() {
        token_type = "global_declaration::a0";
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
        token_children.push_back(t5);
        t5->postprocess();
    }

    global_declaration::a1* global_declaration::a1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        terminal *_t0 = terminal::parse("extern");
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        global_declaration::a1* retval = new global_declaration::a1(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string global_declaration::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void global_declaration::a1::postprocess() {
        token_type = "global_declaration::a1";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    global_declaration* global_declaration::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        global_declaration::a0 *_t0 = global_declaration::a0::parse();
        global_declaration::a1 *_t1 = global_declaration::a1::parse();
        declaration *_t2 = declaration::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        terminal *_t4 = terminal::parse(";");
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        global_declaration* retval = new global_declaration(_t0, _t1, _t2, _t3, _t4);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string global_declaration::to_string() {
        std::string ans = "";
        if(t0 != nullptr) ans += t0->to_string();
        if(t1 != nullptr) ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4->to_string();
        return ans;
    }

    void global_declaration::postprocess() {
        token_type = "global_declaration";
        if(t0 != nullptr) {
            token_children.push_back(t0);
            t0->postprocess();
        }
        if(t1 != nullptr) {
            token_children.push_back(t1);
            t1->postprocess();
        }
        token_children.push_back(t2);
        t2->postprocess();
        token_children.push_back(t3);
        t3->postprocess();
        token_children.push_back(t4);
        t4->postprocess();
    }

    program::a0::b0::c0* program::a0::b0::c0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        struct_definition *_t0 = struct_definition::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0::b0::c0* retval = new program::a0::b0::c0(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void program::a0::b0::c0::postprocess() {
        token_type = "program::a0::b0::c0";
        token_children.push_back(t0);
        t0->postprocess();
    }

    program::a0::b0::c1* program::a0::b0::c1::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        templated_function *_t0 = templated_function::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0::b0::c1* retval = new program::a0::b0::c1(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void program::a0::b0::c1::postprocess() {
        token_type = "program::a0::b0::c1";
        token_children.push_back(t0);
        t0->postprocess();
    }

    program::a0::b0::c2* program::a0::b0::c2::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        templated_struct_definition *_t0 = templated_struct_definition::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0::b0::c2* retval = new program::a0::b0::c2(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void program::a0::b0::c2::postprocess() {
        token_type = "program::a0::b0::c2";
        token_children.push_back(t0);
        t0->postprocess();
    }

    program::a0::b0::c3* program::a0::b0::c3::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        overload *_t0 = overload::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0::b0::c3* retval = new program::a0::b0::c3(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void program::a0::b0::c3::postprocess() {
        token_type = "program::a0::b0::c3";
        token_children.push_back(t0);
        t0->postprocess();
    }

    program::a0::b0::c4* program::a0::b0::c4::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        templated_overload *_t0 = templated_overload::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0::b0::c4* retval = new program::a0::b0::c4(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::b0::c4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void program::a0::b0::c4::postprocess() {
        token_type = "program::a0::b0::c4";
        token_children.push_back(t0);
        t0->postprocess();
    }

    program::a0::b0::c5* program::a0::b0::c5::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        include *_t0 = include::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0::b0::c5* retval = new program::a0::b0::c5(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::b0::c5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void program::a0::b0::c5::postprocess() {
        token_type = "program::a0::b0::c5";
        token_children.push_back(t0);
        t0->postprocess();
    }

    program::a0::b0::c6* program::a0::b0::c6::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        global_declaration *_t0 = global_declaration::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0::b0::c6* retval = new program::a0::b0::c6(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::b0::c6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void program::a0::b0::c6::postprocess() {
        token_type = "program::a0::b0::c6";
        token_children.push_back(t0);
        t0->postprocess();
    }

    program::a0::b0::c7* program::a0::b0::c7::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        global_node *_t0 = global_node::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0::b0::c7* retval = new program::a0::b0::c7(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::b0::c7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void program::a0::b0::c7::postprocess() {
        token_type = "program::a0::b0::c7";
        token_children.push_back(t0);
        t0->postprocess();
    }

    program::a0::b0::c8* program::a0::b0::c8::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        _typedef *_t0 = _typedef::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0::b0::c8* retval = new program::a0::b0::c8(_t0);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::b0::c8::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    void program::a0::b0::c8::postprocess() {
        token_type = "program::a0::b0::c8";
        token_children.push_back(t0);
        t0->postprocess();
    }

    program::a0::b0* program::a0::b0::parse() {
        parse_context _start_ctx = get_ctx();
        if(auto x = program::a0::b0::c0::parse()) {
            program::a0::b0* retval = new program::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = program::a0::b0::c1::parse()) {
            program::a0::b0* retval = new program::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = program::a0::b0::c2::parse()) {
            program::a0::b0* retval = new program::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = program::a0::b0::c3::parse()) {
            program::a0::b0* retval = new program::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = program::a0::b0::c4::parse()) {
            program::a0::b0* retval = new program::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = program::a0::b0::c5::parse()) {
            program::a0::b0* retval = new program::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = program::a0::b0::c6::parse()) {
            program::a0::b0* retval = new program::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = program::a0::b0::c7::parse()) {
            program::a0::b0* retval = new program::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        if(auto x = program::a0::b0::c8::parse()) {
            program::a0::b0* retval = new program::a0::b0(x);
            retval->start_ctx = _start_ctx;
            retval->end_ctx = get_ctx();
            return retval;
        }
        return nullptr;
    }

    std::string program::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        if(is_c4) return t4->to_string();
        if(is_c5) return t5->to_string();
        if(is_c6) return t6->to_string();
        if(is_c7) return t7->to_string();
        if(is_c8) return t8->to_string();
        assert(false);
    }

    void program::a0::b0::postprocess() {
        token_type = "program::a0::b0";
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
        if(is_c6) {
            token_children.push_back(t6);
            t6->postprocess();
        }
        if(is_c7) {
            token_children.push_back(t7);
            t7->postprocess();
        }
        if(is_c8) {
            token_children.push_back(t8);
            t8->postprocess();
        }
    }
    program::a0* program::a0::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        program::a0::b0 *_t1 = program::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program::a0* retval = new program::a0(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    void program::a0::postprocess() {
        token_type = "program::a0";
        token_children.push_back(t0);
        t0->postprocess();
        token_children.push_back(t1);
        t1->postprocess();
    }

    program* program::parse() {
        parse_context _start_ctx = get_ctx();
        push_stack();
        std::vector<program::a0*> _t0;
        while(true) {
            program::a0 *tmp = program::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        program* retval = new program(_t0, _t1);
        retval->start_ctx = _start_ctx;
        retval->end_ctx = get_ctx();
        return retval;
    }

    std::string program::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        ans += t1->to_string();
        return ans;
    }

    void program::postprocess() {
        token_type = "program";
        for(int i = 0; i < t0.size(); i++) {
            token_children.push_back(t0[i]);
            t0[i]->postprocess();
        }
        token_children.push_back(t1);
        t1->postprocess();
    }

};
