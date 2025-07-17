// Date Generated : 07-16-2025 19:44:45
#include "parser.h"

namespace parser {
    // -- PARSE CONTROLLER --
    //the grammar to be parsed
    std::string s;

    //where we are in the string
    int ptr;

    //what is the furthest we've gotten into the string
    int max_parse;

    //this is so we know where to backtrack to
    //the stack should be unaffected by any parse function. 
    std::stack<int> ptr_stack;

    //initializes the parse controller
    void set_s(std::string& ns) {
        s = ns;
        ptr = 0;
        max_parse = 0;
        while(ptr_stack.size() != 0) ptr_stack.pop();
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
    bool check_finished_parsing() {
        if(ptr != s.size()) {
            prettyprint_at_ind(max_parse);
            return false;
        }
        return true;
    }

    //use before trying an optional grammar rule
    void push_stack() {
        ptr_stack.push(ptr);
        max_parse = std::max(max_parse, ptr);
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
    

    function_definition* function_definition::parse() {
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        std::string _t4 = next_chars(1);
        if(_t4 != "(") {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        parameter_list *_t6 = parameter_list::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        std::string _t8 = next_chars(1);
        if(_t8 != ")") {pop_stack(); return nullptr;}
        rm_stack();
        return new function_definition(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
    }

    std::string function_definition::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4;
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8;
        return ans;
    }

    function* function::parse() {
        push_stack();
        function_definition *_t0 = function_definition::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        compound_statement *_t2 = compound_statement::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new function(_t0, _t1, _t2);
    }

    std::string function::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    templated_function* templated_function::parse() {
        push_stack();
        template_header *_t0 = template_header::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        function *_t2 = function::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new templated_function(_t0, _t1, _t2);
    }

    std::string templated_function::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    function_call* function_call::parse() {
        push_stack();
        identifier *_t0 = identifier::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "(") {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        argument_list *_t4 = argument_list::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::string _t6 = next_chars(1);
        if(_t6 != ")") {pop_stack(); return nullptr;}
        rm_stack();
        return new function_call(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
    }

    std::string function_call::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6;
        return ans;
    }

    literal_sizeof* literal_sizeof::parse() {
        push_stack();
        std::string _t0 = next_chars(6);
        if(_t0 != "sizeof") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "(") {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        type *_t4 = type::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::string _t6 = next_chars(1);
        if(_t6 != ")") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_sizeof(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
    }

    std::string literal_sizeof::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6;
        return ans;
    }

    literal_integer::a0* literal_integer::a0::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_integer::a0(_t0);
    }

    std::string literal_integer::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_integer* literal_integer::parse() {
        push_stack();
        std::vector<literal_integer::a0*> _t0;
        while(true) {
            literal_integer::a0 *tmp = literal_integer::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        if(_t0.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_integer(_t0);
    }

    std::string literal_integer::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        return ans;
    }

    literal_float::a0* literal_float::a0::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_float::a0(_t0);
    }

    std::string literal_float::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_float::a1* literal_float::a1::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_float::a1(_t0);
    }

    std::string literal_float::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_float* literal_float::parse() {
        push_stack();
        std::vector<literal_float::a0*> _t0;
        while(true) {
            literal_float::a0 *tmp = literal_float::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        if(_t0.size() == 0) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(1);
        if(_t1 != ".") {pop_stack(); return nullptr;}
        std::vector<literal_float::a1*> _t2;
        while(true) {
            literal_float::a1 *tmp = literal_float::a1::parse();
            if(tmp == nullptr) break;
            _t2.push_back(tmp);
        }
        if(_t2.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_float(_t0, _t1, _t2);
    }

    std::string literal_float::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        ans += t1;
        for(int i = 0; i < t2.size(); i++) ans += t2[i]->to_string();
        return ans;
    }

    literal_char::a0::b0* literal_char::a0::b0::parse() {
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_char::a0::b0(_t0);
    }

    std::string literal_char::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_char::a0::b1* literal_char::a0::b1::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_char::a0::b1(_t0);
    }

    std::string literal_char::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_char::a0::b2* literal_char::a0::b2::parse() {
        push_stack();
        escape *_t0 = escape::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_char::a0::b2(_t0);
    }

    std::string literal_char::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_char::a0::b3* literal_char::a0::b3::parse() {
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_char::a0::b3(_t0);
    }

    std::string literal_char::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_char::a0::b4* literal_char::a0::b4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != " ") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_char::a0::b4(_t0);
    }

    std::string literal_char::a0::b4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_char::a0* literal_char::a0::parse() {
        if(auto x = literal_char::a0::b0::parse()) return new literal_char::a0(x);
        if(auto x = literal_char::a0::b1::parse()) return new literal_char::a0(x);
        if(auto x = literal_char::a0::b2::parse()) return new literal_char::a0(x);
        if(auto x = literal_char::a0::b3::parse()) return new literal_char::a0(x);
        if(auto x = literal_char::a0::b4::parse()) return new literal_char::a0(x);
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

    literal_char* literal_char::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "'") {pop_stack(); return nullptr;}
        literal_char::a0 *_t1 = literal_char::a0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "'") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_char(_t0, _t1, _t2);
    }

    std::string literal_char::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        return ans;
    }

    literal_string::a0::b0* literal_string::a0::b0::parse() {
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_string::a0::b0(_t0);
    }

    std::string literal_string::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_string::a0::b1* literal_string::a0::b1::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_string::a0::b1(_t0);
    }

    std::string literal_string::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_string::a0::b2* literal_string::a0::b2::parse() {
        push_stack();
        escape *_t0 = escape::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_string::a0::b2(_t0);
    }

    std::string literal_string::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_string::a0::b3* literal_string::a0::b3::parse() {
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_string::a0::b3(_t0);
    }

    std::string literal_string::a0::b3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_string::a0::b4* literal_string::a0::b4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != " ") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_string::a0::b4(_t0);
    }

    std::string literal_string::a0::b4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_string::a0* literal_string::a0::parse() {
        if(auto x = literal_string::a0::b0::parse()) return new literal_string::a0(x);
        if(auto x = literal_string::a0::b1::parse()) return new literal_string::a0(x);
        if(auto x = literal_string::a0::b2::parse()) return new literal_string::a0(x);
        if(auto x = literal_string::a0::b3::parse()) return new literal_string::a0(x);
        if(auto x = literal_string::a0::b4::parse()) return new literal_string::a0(x);
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

    literal_string* literal_string::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\"") {pop_stack(); return nullptr;}
        std::vector<literal_string::a0*> _t1;
        while(true) {
            literal_string::a0 *tmp = literal_string::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        std::string _t2 = next_chars(1);
        if(_t2 != "\"") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_string(_t0, _t1, _t2);
    }

    std::string literal_string::to_string() {
        std::string ans = "";
        ans += t0;
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2;
        return ans;
    }

    literal_syscall* literal_syscall::parse() {
        push_stack();
        std::string _t0 = next_chars(7);
        if(_t0 != "syscall") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "(") {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        literal_integer *_t4 = literal_integer::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::string _t6 = next_chars(1);
        if(_t6 != ",") {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        type *_t8 = type::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        ows *_t9 = ows::parse();
        if(_t9 == nullptr) {pop_stack(); return nullptr;}
        std::string _t10 = next_chars(1);
        if(_t10 != ",") {pop_stack(); return nullptr;}
        ows *_t11 = ows::parse();
        if(_t11 == nullptr) {pop_stack(); return nullptr;}
        argument_list *_t12 = argument_list::parse();
        if(_t12 == nullptr) {pop_stack(); return nullptr;}
        ows *_t13 = ows::parse();
        if(_t13 == nullptr) {pop_stack(); return nullptr;}
        std::string _t14 = next_chars(1);
        if(_t14 != ")") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_syscall(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10, _t11, _t12, _t13, _t14);
    }

    std::string literal_syscall::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6;
        ans += t7->to_string();
        ans += t8->to_string();
        ans += t9->to_string();
        ans += t10;
        ans += t11->to_string();
        ans += t12->to_string();
        ans += t13->to_string();
        ans += t14;
        return ans;
    }

    literal_hex::a0::b0* literal_hex::a0::b0::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_hex::a0::b0(_t0);
    }

    std::string literal_hex::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal_hex::a0::b1* literal_hex::a0::b1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "a") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_hex::a0::b1(_t0);
    }

    std::string literal_hex::a0::b1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_hex::a0::b2* literal_hex::a0::b2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "b") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_hex::a0::b2(_t0);
    }

    std::string literal_hex::a0::b2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_hex::a0::b3* literal_hex::a0::b3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "c") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_hex::a0::b3(_t0);
    }

    std::string literal_hex::a0::b3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_hex::a0::b4* literal_hex::a0::b4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "d") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_hex::a0::b4(_t0);
    }

    std::string literal_hex::a0::b4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_hex::a0::b5* literal_hex::a0::b5::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "e") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_hex::a0::b5(_t0);
    }

    std::string literal_hex::a0::b5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_hex::a0::b6* literal_hex::a0::b6::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "f") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_hex::a0::b6(_t0);
    }

    std::string literal_hex::a0::b6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_hex::a0* literal_hex::a0::parse() {
        if(auto x = literal_hex::a0::b0::parse()) return new literal_hex::a0(x);
        if(auto x = literal_hex::a0::b1::parse()) return new literal_hex::a0(x);
        if(auto x = literal_hex::a0::b2::parse()) return new literal_hex::a0(x);
        if(auto x = literal_hex::a0::b3::parse()) return new literal_hex::a0(x);
        if(auto x = literal_hex::a0::b4::parse()) return new literal_hex::a0(x);
        if(auto x = literal_hex::a0::b5::parse()) return new literal_hex::a0(x);
        if(auto x = literal_hex::a0::b6::parse()) return new literal_hex::a0(x);
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
        assert(false);
    }

    literal_hex* literal_hex::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "0x") {pop_stack(); return nullptr;}
        std::vector<literal_hex::a0*> _t1;
        while(true) {
            literal_hex::a0 *tmp = literal_hex::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        if(_t1.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_hex(_t0, _t1);
    }

    std::string literal_hex::to_string() {
        std::string ans = "";
        ans += t0;
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    literal_binary::a0::b0* literal_binary::a0::b0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "0") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_binary::a0::b0(_t0);
    }

    std::string literal_binary::a0::b0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_binary::a0::b1* literal_binary::a0::b1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "1") {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_binary::a0::b1(_t0);
    }

    std::string literal_binary::a0::b1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    literal_binary::a0* literal_binary::a0::parse() {
        if(auto x = literal_binary::a0::b0::parse()) return new literal_binary::a0(x);
        if(auto x = literal_binary::a0::b1::parse()) return new literal_binary::a0(x);
        return nullptr;
    }

    std::string literal_binary::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    literal_binary* literal_binary::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "0b") {pop_stack(); return nullptr;}
        std::vector<literal_binary::a0*> _t1;
        while(true) {
            literal_binary::a0 *tmp = literal_binary::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        if(_t1.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal_binary(_t0, _t1);
    }

    std::string literal_binary::to_string() {
        std::string ans = "";
        ans += t0;
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    literal::a0* literal::a0::parse() {
        push_stack();
        literal_hex *_t0 = literal_hex::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal::a0(_t0);
    }

    std::string literal::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal::a1* literal::a1::parse() {
        push_stack();
        literal_binary *_t0 = literal_binary::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal::a1(_t0);
    }

    std::string literal::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal::a2* literal::a2::parse() {
        push_stack();
        literal_float *_t0 = literal_float::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal::a2(_t0);
    }

    std::string literal::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal::a3* literal::a3::parse() {
        push_stack();
        literal_integer *_t0 = literal_integer::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal::a3(_t0);
    }

    std::string literal::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal::a4* literal::a4::parse() {
        push_stack();
        literal_sizeof *_t0 = literal_sizeof::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal::a4(_t0);
    }

    std::string literal::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal::a5* literal::a5::parse() {
        push_stack();
        literal_char *_t0 = literal_char::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal::a5(_t0);
    }

    std::string literal::a5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal::a6* literal::a6::parse() {
        push_stack();
        literal_string *_t0 = literal_string::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal::a6(_t0);
    }

    std::string literal::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal::a7* literal::a7::parse() {
        push_stack();
        literal_syscall *_t0 = literal_syscall::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal::a7(_t0);
    }

    std::string literal::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal* literal::parse() {
        if(auto x = literal::a0::parse()) return new literal(x);
        if(auto x = literal::a1::parse()) return new literal(x);
        if(auto x = literal::a2::parse()) return new literal(x);
        if(auto x = literal::a3::parse()) return new literal(x);
        if(auto x = literal::a4::parse()) return new literal(x);
        if(auto x = literal::a5::parse()) return new literal(x);
        if(auto x = literal::a6::parse()) return new literal(x);
        if(auto x = literal::a7::parse()) return new literal(x);
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
        assert(false);
    }

    member_variable_declaration* member_variable_declaration::parse() {
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        std::string _t3 = next_chars(1);
        if(_t3 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new member_variable_declaration(_t0, _t1, _t2, _t3);
    }

    std::string member_variable_declaration::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3;
        return ans;
    }

    constructor_definition* constructor_definition::parse() {
        push_stack();
        base_type *_t0 = base_type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "(") {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        parameter_list *_t4 = parameter_list::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::string _t6 = next_chars(1);
        if(_t6 != ")") {pop_stack(); return nullptr;}
        rm_stack();
        return new constructor_definition(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
    }

    std::string constructor_definition::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6;
        return ans;
    }

    constructor* constructor::parse() {
        push_stack();
        constructor_definition *_t0 = constructor_definition::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        compound_statement *_t2 = compound_statement::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new constructor(_t0, _t1, _t2);
    }

    std::string constructor::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    constructor_call* constructor_call::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != "new") {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        type *_t2 = type::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        std::string _t4 = next_chars(1);
        if(_t4 != "(") {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        argument_list *_t6 = argument_list::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        std::string _t8 = next_chars(1);
        if(_t8 != ")") {pop_stack(); return nullptr;}
        rm_stack();
        return new constructor_call(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
    }

    std::string constructor_call::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4;
        ans += t5->to_string();
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8;
        return ans;
    }

    destructor* destructor::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "~") {pop_stack(); return nullptr;}
        base_type *_t1 = base_type::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        std::string _t3 = next_chars(2);
        if(_t3 != "()") {pop_stack(); return nullptr;}
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        compound_statement *_t5 = compound_statement::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new destructor(_t0, _t1, _t2, _t3, _t4, _t5);
    }

    std::string destructor::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3;
        ans += t4->to_string();
        ans += t5->to_string();
        return ans;
    }

    struct_definition::a0::b0::c0* struct_definition::a0::b0::c0::parse() {
        push_stack();
        member_variable_declaration *_t0 = member_variable_declaration::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new struct_definition::a0::b0::c0(_t0);
    }

    std::string struct_definition::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    struct_definition::a0::b0::c1* struct_definition::a0::b0::c1::parse() {
        push_stack();
        function *_t0 = function::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new struct_definition::a0::b0::c1(_t0);
    }

    std::string struct_definition::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    struct_definition::a0::b0::c2* struct_definition::a0::b0::c2::parse() {
        push_stack();
        constructor *_t0 = constructor::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new struct_definition::a0::b0::c2(_t0);
    }

    std::string struct_definition::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    struct_definition::a0::b0::c3* struct_definition::a0::b0::c3::parse() {
        push_stack();
        destructor *_t0 = destructor::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new struct_definition::a0::b0::c3(_t0);
    }

    std::string struct_definition::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    struct_definition::a0::b0* struct_definition::a0::b0::parse() {
        if(auto x = struct_definition::a0::b0::c0::parse()) return new struct_definition::a0::b0(x);
        if(auto x = struct_definition::a0::b0::c1::parse()) return new struct_definition::a0::b0(x);
        if(auto x = struct_definition::a0::b0::c2::parse()) return new struct_definition::a0::b0(x);
        if(auto x = struct_definition::a0::b0::c3::parse()) return new struct_definition::a0::b0(x);
        return nullptr;
    }

    std::string struct_definition::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        assert(false);
    }

    struct_definition::a0* struct_definition::a0::parse() {
        push_stack();
        struct_definition::a0::b0 *_t0 = struct_definition::a0::b0::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new struct_definition::a0(_t0, _t1);
    }

    std::string struct_definition::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    struct_definition* struct_definition::parse() {
        push_stack();
        std::string _t0 = next_chars(6);
        if(_t0 != "struct") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        base_type *_t2 = base_type::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        std::string _t4 = next_chars(1);
        if(_t4 != "{") {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::vector<struct_definition::a0*> _t6;
        while(true) {
            struct_definition::a0 *tmp = struct_definition::a0::parse();
            if(tmp == nullptr) break;
            _t6.push_back(tmp);
        }
        std::string _t7 = next_chars(1);
        if(_t7 != "}") {pop_stack(); return nullptr;}
        rm_stack();
        return new struct_definition(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7);
    }

    std::string struct_definition::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4;
        ans += t5->to_string();
        for(int i = 0; i < t6.size(); i++) ans += t6[i]->to_string();
        ans += t7;
        return ans;
    }

    templated_struct_definition* templated_struct_definition::parse() {
        push_stack();
        template_header *_t0 = template_header::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        struct_definition *_t2 = struct_definition::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new templated_struct_definition(_t0, _t1, _t2);
    }

    std::string templated_struct_definition::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    expr_primary::a0* expr_primary::a0::parse() {
        push_stack();
        literal *_t0 = literal::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_primary::a0(_t0);
    }

    std::string expr_primary::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    expr_primary::a1* expr_primary::a1::parse() {
        push_stack();
        constructor_call *_t0 = constructor_call::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_primary::a1(_t0);
    }

    std::string expr_primary::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    expr_primary::a2* expr_primary::a2::parse() {
        push_stack();
        function_call *_t0 = function_call::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_primary::a2(_t0);
    }

    std::string expr_primary::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    expr_primary::a3* expr_primary::a3::parse() {
        push_stack();
        identifier *_t0 = identifier::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_primary::a3(_t0);
    }

    std::string expr_primary::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    expr_primary::a4* expr_primary::a4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "(") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        expression *_t2 = expression::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        std::string _t4 = next_chars(1);
        if(_t4 != ")") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_primary::a4(_t0, _t1, _t2, _t3, _t4);
    }

    std::string expr_primary::a4::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4;
        return ans;
    }

    expr_primary* expr_primary::parse() {
        if(auto x = expr_primary::a0::parse()) return new expr_primary(x);
        if(auto x = expr_primary::a1::parse()) return new expr_primary(x);
        if(auto x = expr_primary::a2::parse()) return new expr_primary(x);
        if(auto x = expr_primary::a3::parse()) return new expr_primary(x);
        if(auto x = expr_primary::a4::parse()) return new expr_primary(x);
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

    expr_postfix::a0::b0::c0* expr_postfix::a0::b0::c0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "[") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        expression *_t2 = expression::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        std::string _t4 = next_chars(1);
        if(_t4 != "]") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_postfix::a0::b0::c0(_t0, _t1, _t2, _t3, _t4);
    }

    std::string expr_postfix::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4;
        return ans;
    }

    expr_postfix::a0::b0::c1* expr_postfix::a0::b0::c1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ".") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        function_call *_t2 = function_call::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_postfix::a0::b0::c1(_t0, _t1, _t2);
    }

    std::string expr_postfix::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    expr_postfix::a0::b0::c2* expr_postfix::a0::b0::c2::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "->") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        function_call *_t2 = function_call::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_postfix::a0::b0::c2(_t0, _t1, _t2);
    }

    std::string expr_postfix::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    expr_postfix::a0::b0::c3* expr_postfix::a0::b0::c3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ".") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_postfix::a0::b0::c3(_t0, _t1, _t2);
    }

    std::string expr_postfix::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    expr_postfix::a0::b0::c4* expr_postfix::a0::b0::c4::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "->") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_postfix::a0::b0::c4(_t0, _t1, _t2);
    }

    std::string expr_postfix::a0::b0::c4::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    expr_postfix::a0::b0::c5* expr_postfix::a0::b0::c5::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "++") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_postfix::a0::b0::c5(_t0);
    }

    std::string expr_postfix::a0::b0::c5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_postfix::a0::b0::c6* expr_postfix::a0::b0::c6::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "--") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_postfix::a0::b0::c6(_t0);
    }

    std::string expr_postfix::a0::b0::c6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_postfix::a0::b0* expr_postfix::a0::b0::parse() {
        if(auto x = expr_postfix::a0::b0::c0::parse()) return new expr_postfix::a0::b0(x);
        if(auto x = expr_postfix::a0::b0::c1::parse()) return new expr_postfix::a0::b0(x);
        if(auto x = expr_postfix::a0::b0::c2::parse()) return new expr_postfix::a0::b0(x);
        if(auto x = expr_postfix::a0::b0::c3::parse()) return new expr_postfix::a0::b0(x);
        if(auto x = expr_postfix::a0::b0::c4::parse()) return new expr_postfix::a0::b0(x);
        if(auto x = expr_postfix::a0::b0::c5::parse()) return new expr_postfix::a0::b0(x);
        if(auto x = expr_postfix::a0::b0::c6::parse()) return new expr_postfix::a0::b0(x);
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
        assert(false);
    }

    expr_postfix::a0* expr_postfix::a0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expr_postfix::a0::b0 *_t1 = expr_postfix::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_postfix::a0(_t0, _t1);
    }

    std::string expr_postfix::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    expr_postfix* expr_postfix::parse() {
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
        return new expr_postfix(_t0, _t1);
    }

    std::string expr_postfix::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_unary::a0::b0::c0* expr_unary::a0::b0::c0::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "++") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0::b0::c0(_t0);
    }

    std::string expr_unary::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_unary::a0::b0::c1* expr_unary::a0::b0::c1::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "--") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0::b0::c1(_t0);
    }

    std::string expr_unary::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_unary::a0::b0::c2* expr_unary::a0::b0::c2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "+") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0::b0::c2(_t0);
    }

    std::string expr_unary::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_unary::a0::b0::c3* expr_unary::a0::b0::c3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "-") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0::b0::c3(_t0);
    }

    std::string expr_unary::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_unary::a0::b0::c4* expr_unary::a0::b0::c4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "~") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0::b0::c4(_t0);
    }

    std::string expr_unary::a0::b0::c4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_unary::a0::b0::c5* expr_unary::a0::b0::c5::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "!") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0::b0::c5(_t0);
    }

    std::string expr_unary::a0::b0::c5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_unary::a0::b0::c6* expr_unary::a0::b0::c6::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "*") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0::b0::c6(_t0);
    }

    std::string expr_unary::a0::b0::c6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_unary::a0::b0::c7* expr_unary::a0::b0::c7::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "@") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0::b0::c7(_t0);
    }

    std::string expr_unary::a0::b0::c7::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_unary::a0::b0::c8* expr_unary::a0::b0::c8::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "$") {pop_stack(); return nullptr;}
        type *_t1 = type::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0::b0::c8(_t0, _t1);
    }

    std::string expr_unary::a0::b0::c8::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        return ans;
    }

    expr_unary::a0::b0* expr_unary::a0::b0::parse() {
        if(auto x = expr_unary::a0::b0::c0::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c1::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c2::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c3::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c4::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c5::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c6::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c7::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c8::parse()) return new expr_unary::a0::b0(x);
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

    expr_unary::a0* expr_unary::a0::parse() {
        push_stack();
        expr_unary::a0::b0 *_t0 = expr_unary::a0::b0::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        expr_unary *_t2 = expr_unary::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a0(_t0, _t1, _t2);
    }

    std::string expr_unary::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    expr_unary::a1* expr_unary::a1::parse() {
        push_stack();
        expr_postfix *_t0 = expr_postfix::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_unary::a1(_t0);
    }

    std::string expr_unary::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    expr_unary* expr_unary::parse() {
        if(auto x = expr_unary::a0::parse()) return new expr_unary(x);
        if(auto x = expr_unary::a1::parse()) return new expr_unary(x);
        return nullptr;
    }

    std::string expr_unary::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        assert(false);
    }

    expr_multiplicative::a0::b0::c0* expr_multiplicative::a0::b0::c0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "*") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_multiplicative::a0::b0::c0(_t0);
    }

    std::string expr_multiplicative::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_multiplicative::a0::b0::c1* expr_multiplicative::a0::b0::c1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "/") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_multiplicative::a0::b0::c1(_t0);
    }

    std::string expr_multiplicative::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_multiplicative::a0::b0::c2* expr_multiplicative::a0::b0::c2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "%") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_multiplicative::a0::b0::c2(_t0);
    }

    std::string expr_multiplicative::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_multiplicative::a0::b0* expr_multiplicative::a0::b0::parse() {
        if(auto x = expr_multiplicative::a0::b0::c0::parse()) return new expr_multiplicative::a0::b0(x);
        if(auto x = expr_multiplicative::a0::b0::c1::parse()) return new expr_multiplicative::a0::b0(x);
        if(auto x = expr_multiplicative::a0::b0::c2::parse()) return new expr_multiplicative::a0::b0(x);
        return nullptr;
    }

    std::string expr_multiplicative::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        assert(false);
    }

    expr_multiplicative::a0* expr_multiplicative::a0::parse() {
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
        return new expr_multiplicative::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_multiplicative::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_multiplicative* expr_multiplicative::parse() {
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
        return new expr_multiplicative(_t0, _t1);
    }

    std::string expr_multiplicative::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_additive::a0::b0::c0* expr_additive::a0::b0::c0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "+") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_additive::a0::b0::c0(_t0);
    }

    std::string expr_additive::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_additive::a0::b0::c1* expr_additive::a0::b0::c1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "-") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_additive::a0::b0::c1(_t0);
    }

    std::string expr_additive::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_additive::a0::b0* expr_additive::a0::b0::parse() {
        if(auto x = expr_additive::a0::b0::c0::parse()) return new expr_additive::a0::b0(x);
        if(auto x = expr_additive::a0::b0::c1::parse()) return new expr_additive::a0::b0(x);
        return nullptr;
    }

    std::string expr_additive::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        assert(false);
    }

    expr_additive::a0* expr_additive::a0::parse() {
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
        return new expr_additive::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_additive::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_additive* expr_additive::parse() {
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
        return new expr_additive(_t0, _t1);
    }

    std::string expr_additive::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_shift::a0::b0::c0* expr_shift::a0::b0::c0::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "<<") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_shift::a0::b0::c0(_t0);
    }

    std::string expr_shift::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_shift::a0::b0::c1* expr_shift::a0::b0::c1::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != ">>") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_shift::a0::b0::c1(_t0);
    }

    std::string expr_shift::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_shift::a0::b0* expr_shift::a0::b0::parse() {
        if(auto x = expr_shift::a0::b0::c0::parse()) return new expr_shift::a0::b0(x);
        if(auto x = expr_shift::a0::b0::c1::parse()) return new expr_shift::a0::b0(x);
        return nullptr;
    }

    std::string expr_shift::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        assert(false);
    }

    expr_shift::a0* expr_shift::a0::parse() {
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
        return new expr_shift::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_shift::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_shift* expr_shift::parse() {
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
        return new expr_shift(_t0, _t1);
    }

    std::string expr_shift::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_relational::a0::b0::c0* expr_relational::a0::b0::c0::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "<=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_relational::a0::b0::c0(_t0);
    }

    std::string expr_relational::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_relational::a0::b0::c1* expr_relational::a0::b0::c1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "<") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_relational::a0::b0::c1(_t0);
    }

    std::string expr_relational::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_relational::a0::b0::c2* expr_relational::a0::b0::c2::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != ">=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_relational::a0::b0::c2(_t0);
    }

    std::string expr_relational::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_relational::a0::b0::c3* expr_relational::a0::b0::c3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ">") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_relational::a0::b0::c3(_t0);
    }

    std::string expr_relational::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_relational::a0::b0* expr_relational::a0::b0::parse() {
        if(auto x = expr_relational::a0::b0::c0::parse()) return new expr_relational::a0::b0(x);
        if(auto x = expr_relational::a0::b0::c1::parse()) return new expr_relational::a0::b0(x);
        if(auto x = expr_relational::a0::b0::c2::parse()) return new expr_relational::a0::b0(x);
        if(auto x = expr_relational::a0::b0::c3::parse()) return new expr_relational::a0::b0(x);
        return nullptr;
    }

    std::string expr_relational::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        if(is_c2) return t2->to_string();
        if(is_c3) return t3->to_string();
        assert(false);
    }

    expr_relational::a0* expr_relational::a0::parse() {
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
        return new expr_relational::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_relational::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_relational* expr_relational::parse() {
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
        return new expr_relational(_t0, _t1);
    }

    std::string expr_relational::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_equality::a0::b0::c0* expr_equality::a0::b0::c0::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "==") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_equality::a0::b0::c0(_t0);
    }

    std::string expr_equality::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_equality::a0::b0::c1* expr_equality::a0::b0::c1::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "!=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_equality::a0::b0::c1(_t0);
    }

    std::string expr_equality::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_equality::a0::b0* expr_equality::a0::b0::parse() {
        if(auto x = expr_equality::a0::b0::c0::parse()) return new expr_equality::a0::b0(x);
        if(auto x = expr_equality::a0::b0::c1::parse()) return new expr_equality::a0::b0(x);
        return nullptr;
    }

    std::string expr_equality::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
        assert(false);
    }

    expr_equality::a0* expr_equality::a0::parse() {
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
        return new expr_equality::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_equality::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_equality* expr_equality::parse() {
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
        return new expr_equality(_t0, _t1);
    }

    std::string expr_equality::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_bit_and::a0* expr_bit_and::a0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(1);
        if(_t1 != "&") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_equality *_t3 = expr_equality::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_bit_and::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_bit_and::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_bit_and* expr_bit_and::parse() {
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
        return new expr_bit_and(_t0, _t1);
    }

    std::string expr_bit_and::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_bit_xor::a0* expr_bit_xor::a0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(1);
        if(_t1 != "^") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_bit_and *_t3 = expr_bit_and::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_bit_xor::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_bit_xor::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_bit_xor* expr_bit_xor::parse() {
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
        return new expr_bit_xor(_t0, _t1);
    }

    std::string expr_bit_xor::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_bit_or::a0* expr_bit_or::a0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(1);
        if(_t1 != "|") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_bit_xor *_t3 = expr_bit_xor::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_bit_or::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_bit_or::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_bit_or* expr_bit_or::parse() {
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
        return new expr_bit_or(_t0, _t1);
    }

    std::string expr_bit_or::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_logical_and::a0* expr_logical_and::a0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(2);
        if(_t1 != "&&") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_bit_or *_t3 = expr_bit_or::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_logical_and::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_logical_and::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_logical_and* expr_logical_and::parse() {
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
        return new expr_logical_and(_t0, _t1);
    }

    std::string expr_logical_and::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_logical_or::a0* expr_logical_or::a0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(2);
        if(_t1 != "||") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expr_logical_and *_t3 = expr_logical_and::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_logical_or::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_logical_or::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_logical_or* expr_logical_or::parse() {
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
        return new expr_logical_or(_t0, _t1);
    }

    std::string expr_logical_or::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expr_assignment::a0::b0::c0* expr_assignment::a0::b0::c0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c0(_t0);
    }

    std::string expr_assignment::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c1* expr_assignment::a0::b0::c1::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "+=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c1(_t0);
    }

    std::string expr_assignment::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c2* expr_assignment::a0::b0::c2::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "-=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c2(_t0);
    }

    std::string expr_assignment::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c3* expr_assignment::a0::b0::c3::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "*=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c3(_t0);
    }

    std::string expr_assignment::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c4* expr_assignment::a0::b0::c4::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "/=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c4(_t0);
    }

    std::string expr_assignment::a0::b0::c4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c5* expr_assignment::a0::b0::c5::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "%=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c5(_t0);
    }

    std::string expr_assignment::a0::b0::c5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c6* expr_assignment::a0::b0::c6::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != "<<=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c6(_t0);
    }

    std::string expr_assignment::a0::b0::c6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c7* expr_assignment::a0::b0::c7::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != ">>=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c7(_t0);
    }

    std::string expr_assignment::a0::b0::c7::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c8* expr_assignment::a0::b0::c8::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "&=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c8(_t0);
    }

    std::string expr_assignment::a0::b0::c8::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c9* expr_assignment::a0::b0::c9::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "^=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c9(_t0);
    }

    std::string expr_assignment::a0::b0::c9::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0::c10* expr_assignment::a0::b0::c10::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "|=") {pop_stack(); return nullptr;}
        rm_stack();
        return new expr_assignment::a0::b0::c10(_t0);
    }

    std::string expr_assignment::a0::b0::c10::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    expr_assignment::a0::b0* expr_assignment::a0::b0::parse() {
        if(auto x = expr_assignment::a0::b0::c0::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c1::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c2::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c3::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c4::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c5::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c6::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c7::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c8::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c9::parse()) return new expr_assignment::a0::b0(x);
        if(auto x = expr_assignment::a0::b0::c10::parse()) return new expr_assignment::a0::b0(x);
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
        assert(false);
    }

    expr_assignment::a0* expr_assignment::a0::parse() {
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
        return new expr_assignment::a0(_t0, _t1, _t2, _t3);
    }

    std::string expr_assignment::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    expr_assignment* expr_assignment::parse() {
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
        return new expr_assignment(_t0, _t1);
    }

    std::string expr_assignment::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    expression* expression::parse() {
        push_stack();
        expr_assignment *_t0 = expr_assignment::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new expression(_t0);
    }

    std::string expression::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    overloadable_operator::a0* overloadable_operator::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != "++x") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a0(_t0);
    }

    std::string overloadable_operator::a0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a1* overloadable_operator::a1::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != "--x") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a1(_t0);
    }

    std::string overloadable_operator::a1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a2* overloadable_operator::a2::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != "x++") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a2(_t0);
    }

    std::string overloadable_operator::a2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a3* overloadable_operator::a3::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != "x--") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a3(_t0);
    }

    std::string overloadable_operator::a3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a4* overloadable_operator::a4::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "*x") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a4(_t0);
    }

    std::string overloadable_operator::a4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a5* overloadable_operator::a5::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "+=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a5(_t0);
    }

    std::string overloadable_operator::a5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a6* overloadable_operator::a6::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "-=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a6(_t0);
    }

    std::string overloadable_operator::a6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a7* overloadable_operator::a7::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "*=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a7(_t0);
    }

    std::string overloadable_operator::a7::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a8* overloadable_operator::a8::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "/=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a8(_t0);
    }

    std::string overloadable_operator::a8::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a9* overloadable_operator::a9::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "%=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a9(_t0);
    }

    std::string overloadable_operator::a9::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a10* overloadable_operator::a10::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "&=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a10(_t0);
    }

    std::string overloadable_operator::a10::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a11* overloadable_operator::a11::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "|=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a11(_t0);
    }

    std::string overloadable_operator::a11::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a12* overloadable_operator::a12::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "^=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a12(_t0);
    }

    std::string overloadable_operator::a12::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a13* overloadable_operator::a13::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != "<<=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a13(_t0);
    }

    std::string overloadable_operator::a13::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a14* overloadable_operator::a14::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != ">>=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a14(_t0);
    }

    std::string overloadable_operator::a14::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a15* overloadable_operator::a15::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "+") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a15(_t0);
    }

    std::string overloadable_operator::a15::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a16* overloadable_operator::a16::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "-") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a16(_t0);
    }

    std::string overloadable_operator::a16::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a17* overloadable_operator::a17::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "*") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a17(_t0);
    }

    std::string overloadable_operator::a17::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a18* overloadable_operator::a18::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "/") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a18(_t0);
    }

    std::string overloadable_operator::a18::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a19* overloadable_operator::a19::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "%") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a19(_t0);
    }

    std::string overloadable_operator::a19::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a20* overloadable_operator::a20::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "&") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a20(_t0);
    }

    std::string overloadable_operator::a20::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a21* overloadable_operator::a21::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "|") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a21(_t0);
    }

    std::string overloadable_operator::a21::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a22* overloadable_operator::a22::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "^") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a22(_t0);
    }

    std::string overloadable_operator::a22::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a23* overloadable_operator::a23::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "<<") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a23(_t0);
    }

    std::string overloadable_operator::a23::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a24* overloadable_operator::a24::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != ">>") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a24(_t0);
    }

    std::string overloadable_operator::a24::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a25* overloadable_operator::a25::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "==") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a25(_t0);
    }

    std::string overloadable_operator::a25::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a26* overloadable_operator::a26::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "!=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a26(_t0);
    }

    std::string overloadable_operator::a26::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a27* overloadable_operator::a27::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "<") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a27(_t0);
    }

    std::string overloadable_operator::a27::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a28* overloadable_operator::a28::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "<=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a28(_t0);
    }

    std::string overloadable_operator::a28::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a29* overloadable_operator::a29::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ">") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a29(_t0);
    }

    std::string overloadable_operator::a29::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a30* overloadable_operator::a30::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != ">=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a30(_t0);
    }

    std::string overloadable_operator::a30::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a31* overloadable_operator::a31::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "[]") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a31(_t0);
    }

    std::string overloadable_operator::a31::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator::a32* overloadable_operator::a32::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "=") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a32(_t0);
    }

    std::string overloadable_operator::a32::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    overloadable_operator* overloadable_operator::parse() {
        if(auto x = overloadable_operator::a0::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a1::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a2::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a3::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a4::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a5::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a6::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a7::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a8::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a9::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a10::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a11::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a12::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a13::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a14::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a15::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a16::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a17::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a18::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a19::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a20::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a21::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a22::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a23::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a24::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a25::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a26::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a27::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a28::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a29::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a30::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a31::parse()) return new overloadable_operator(x);
        if(auto x = overloadable_operator::a32::parse()) return new overloadable_operator(x);
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

    overload_definition* overload_definition::parse() {
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(8);
        if(_t2 != "operator") {pop_stack(); return nullptr;}
        overloadable_operator *_t3 = overloadable_operator::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        std::string _t5 = next_chars(1);
        if(_t5 != "(") {pop_stack(); return nullptr;}
        ows *_t6 = ows::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        parameter_list *_t7 = parameter_list::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        ows *_t8 = ows::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        std::string _t9 = next_chars(1);
        if(_t9 != ")") {pop_stack(); return nullptr;}
        rm_stack();
        return new overload_definition(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
    }

    std::string overload_definition::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5;
        ans += t6->to_string();
        ans += t7->to_string();
        ans += t8->to_string();
        ans += t9;
        return ans;
    }

    overload* overload::parse() {
        push_stack();
        overload_definition *_t0 = overload_definition::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        compound_statement *_t2 = compound_statement::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new overload(_t0, _t1, _t2);
    }

    std::string overload::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    templated_overload* templated_overload::parse() {
        push_stack();
        template_header *_t0 = template_header::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        overload *_t2 = overload::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new templated_overload(_t0, _t1, _t2);
    }

    std::string templated_overload::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    alpha::a0* alpha::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "A") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a0(_t0);
    }

    std::string alpha::a0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a1* alpha::a1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "B") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a1(_t0);
    }

    std::string alpha::a1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a2* alpha::a2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "C") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a2(_t0);
    }

    std::string alpha::a2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a3* alpha::a3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "D") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a3(_t0);
    }

    std::string alpha::a3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a4* alpha::a4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "E") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a4(_t0);
    }

    std::string alpha::a4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a5* alpha::a5::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "F") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a5(_t0);
    }

    std::string alpha::a5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a6* alpha::a6::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "G") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a6(_t0);
    }

    std::string alpha::a6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a7* alpha::a7::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "H") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a7(_t0);
    }

    std::string alpha::a7::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a8* alpha::a8::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "I") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a8(_t0);
    }

    std::string alpha::a8::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a9* alpha::a9::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "J") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a9(_t0);
    }

    std::string alpha::a9::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a10* alpha::a10::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "K") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a10(_t0);
    }

    std::string alpha::a10::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a11* alpha::a11::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "L") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a11(_t0);
    }

    std::string alpha::a11::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a12* alpha::a12::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "M") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a12(_t0);
    }

    std::string alpha::a12::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a13* alpha::a13::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "N") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a13(_t0);
    }

    std::string alpha::a13::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a14* alpha::a14::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "O") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a14(_t0);
    }

    std::string alpha::a14::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a15* alpha::a15::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "P") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a15(_t0);
    }

    std::string alpha::a15::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a16* alpha::a16::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "Q") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a16(_t0);
    }

    std::string alpha::a16::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a17* alpha::a17::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "R") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a17(_t0);
    }

    std::string alpha::a17::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a18* alpha::a18::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "S") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a18(_t0);
    }

    std::string alpha::a18::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a19* alpha::a19::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "T") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a19(_t0);
    }

    std::string alpha::a19::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a20* alpha::a20::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "U") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a20(_t0);
    }

    std::string alpha::a20::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a21* alpha::a21::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "V") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a21(_t0);
    }

    std::string alpha::a21::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a22* alpha::a22::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "W") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a22(_t0);
    }

    std::string alpha::a22::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a23* alpha::a23::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "X") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a23(_t0);
    }

    std::string alpha::a23::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a24* alpha::a24::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "Y") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a24(_t0);
    }

    std::string alpha::a24::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a25* alpha::a25::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "Z") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a25(_t0);
    }

    std::string alpha::a25::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a26* alpha::a26::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "a") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a26(_t0);
    }

    std::string alpha::a26::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a27* alpha::a27::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "b") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a27(_t0);
    }

    std::string alpha::a27::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a28* alpha::a28::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "c") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a28(_t0);
    }

    std::string alpha::a28::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a29* alpha::a29::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "d") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a29(_t0);
    }

    std::string alpha::a29::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a30* alpha::a30::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "e") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a30(_t0);
    }

    std::string alpha::a30::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a31* alpha::a31::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "f") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a31(_t0);
    }

    std::string alpha::a31::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a32* alpha::a32::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "g") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a32(_t0);
    }

    std::string alpha::a32::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a33* alpha::a33::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "h") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a33(_t0);
    }

    std::string alpha::a33::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a34* alpha::a34::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "i") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a34(_t0);
    }

    std::string alpha::a34::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a35* alpha::a35::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "j") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a35(_t0);
    }

    std::string alpha::a35::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a36* alpha::a36::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "k") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a36(_t0);
    }

    std::string alpha::a36::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a37* alpha::a37::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "l") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a37(_t0);
    }

    std::string alpha::a37::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a38* alpha::a38::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "m") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a38(_t0);
    }

    std::string alpha::a38::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a39* alpha::a39::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "n") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a39(_t0);
    }

    std::string alpha::a39::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a40* alpha::a40::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "o") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a40(_t0);
    }

    std::string alpha::a40::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a41* alpha::a41::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "p") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a41(_t0);
    }

    std::string alpha::a41::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a42* alpha::a42::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "q") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a42(_t0);
    }

    std::string alpha::a42::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a43* alpha::a43::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "r") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a43(_t0);
    }

    std::string alpha::a43::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a44* alpha::a44::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "s") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a44(_t0);
    }

    std::string alpha::a44::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a45* alpha::a45::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "t") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a45(_t0);
    }

    std::string alpha::a45::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a46* alpha::a46::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "u") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a46(_t0);
    }

    std::string alpha::a46::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a47* alpha::a47::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "v") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a47(_t0);
    }

    std::string alpha::a47::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a48* alpha::a48::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "w") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a48(_t0);
    }

    std::string alpha::a48::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a49* alpha::a49::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "x") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a49(_t0);
    }

    std::string alpha::a49::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a50* alpha::a50::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "y") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a50(_t0);
    }

    std::string alpha::a50::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha::a51* alpha::a51::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "z") {pop_stack(); return nullptr;}
        rm_stack();
        return new alpha::a51(_t0);
    }

    std::string alpha::a51::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    alpha* alpha::parse() {
        if(auto x = alpha::a0::parse()) return new alpha(x);
        if(auto x = alpha::a1::parse()) return new alpha(x);
        if(auto x = alpha::a2::parse()) return new alpha(x);
        if(auto x = alpha::a3::parse()) return new alpha(x);
        if(auto x = alpha::a4::parse()) return new alpha(x);
        if(auto x = alpha::a5::parse()) return new alpha(x);
        if(auto x = alpha::a6::parse()) return new alpha(x);
        if(auto x = alpha::a7::parse()) return new alpha(x);
        if(auto x = alpha::a8::parse()) return new alpha(x);
        if(auto x = alpha::a9::parse()) return new alpha(x);
        if(auto x = alpha::a10::parse()) return new alpha(x);
        if(auto x = alpha::a11::parse()) return new alpha(x);
        if(auto x = alpha::a12::parse()) return new alpha(x);
        if(auto x = alpha::a13::parse()) return new alpha(x);
        if(auto x = alpha::a14::parse()) return new alpha(x);
        if(auto x = alpha::a15::parse()) return new alpha(x);
        if(auto x = alpha::a16::parse()) return new alpha(x);
        if(auto x = alpha::a17::parse()) return new alpha(x);
        if(auto x = alpha::a18::parse()) return new alpha(x);
        if(auto x = alpha::a19::parse()) return new alpha(x);
        if(auto x = alpha::a20::parse()) return new alpha(x);
        if(auto x = alpha::a21::parse()) return new alpha(x);
        if(auto x = alpha::a22::parse()) return new alpha(x);
        if(auto x = alpha::a23::parse()) return new alpha(x);
        if(auto x = alpha::a24::parse()) return new alpha(x);
        if(auto x = alpha::a25::parse()) return new alpha(x);
        if(auto x = alpha::a26::parse()) return new alpha(x);
        if(auto x = alpha::a27::parse()) return new alpha(x);
        if(auto x = alpha::a28::parse()) return new alpha(x);
        if(auto x = alpha::a29::parse()) return new alpha(x);
        if(auto x = alpha::a30::parse()) return new alpha(x);
        if(auto x = alpha::a31::parse()) return new alpha(x);
        if(auto x = alpha::a32::parse()) return new alpha(x);
        if(auto x = alpha::a33::parse()) return new alpha(x);
        if(auto x = alpha::a34::parse()) return new alpha(x);
        if(auto x = alpha::a35::parse()) return new alpha(x);
        if(auto x = alpha::a36::parse()) return new alpha(x);
        if(auto x = alpha::a37::parse()) return new alpha(x);
        if(auto x = alpha::a38::parse()) return new alpha(x);
        if(auto x = alpha::a39::parse()) return new alpha(x);
        if(auto x = alpha::a40::parse()) return new alpha(x);
        if(auto x = alpha::a41::parse()) return new alpha(x);
        if(auto x = alpha::a42::parse()) return new alpha(x);
        if(auto x = alpha::a43::parse()) return new alpha(x);
        if(auto x = alpha::a44::parse()) return new alpha(x);
        if(auto x = alpha::a45::parse()) return new alpha(x);
        if(auto x = alpha::a46::parse()) return new alpha(x);
        if(auto x = alpha::a47::parse()) return new alpha(x);
        if(auto x = alpha::a48::parse()) return new alpha(x);
        if(auto x = alpha::a49::parse()) return new alpha(x);
        if(auto x = alpha::a50::parse()) return new alpha(x);
        if(auto x = alpha::a51::parse()) return new alpha(x);
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

    digit::a0* digit::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "0") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a0(_t0);
    }

    std::string digit::a0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit::a1* digit::a1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "1") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a1(_t0);
    }

    std::string digit::a1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit::a2* digit::a2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "2") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a2(_t0);
    }

    std::string digit::a2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit::a3* digit::a3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "3") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a3(_t0);
    }

    std::string digit::a3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit::a4* digit::a4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "4") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a4(_t0);
    }

    std::string digit::a4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit::a5* digit::a5::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "5") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a5(_t0);
    }

    std::string digit::a5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit::a6* digit::a6::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "6") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a6(_t0);
    }

    std::string digit::a6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit::a7* digit::a7::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "7") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a7(_t0);
    }

    std::string digit::a7::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit::a8* digit::a8::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "8") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a8(_t0);
    }

    std::string digit::a8::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit::a9* digit::a9::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "9") {pop_stack(); return nullptr;}
        rm_stack();
        return new digit::a9(_t0);
    }

    std::string digit::a9::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    digit* digit::parse() {
        if(auto x = digit::a0::parse()) return new digit(x);
        if(auto x = digit::a1::parse()) return new digit(x);
        if(auto x = digit::a2::parse()) return new digit(x);
        if(auto x = digit::a3::parse()) return new digit(x);
        if(auto x = digit::a4::parse()) return new digit(x);
        if(auto x = digit::a5::parse()) return new digit(x);
        if(auto x = digit::a6::parse()) return new digit(x);
        if(auto x = digit::a7::parse()) return new digit(x);
        if(auto x = digit::a8::parse()) return new digit(x);
        if(auto x = digit::a9::parse()) return new digit(x);
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

    escape::a0::b0* escape::a0::b0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "n") {pop_stack(); return nullptr;}
        rm_stack();
        return new escape::a0::b0(_t0);
    }

    std::string escape::a0::b0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    escape::a0::b1* escape::a0::b1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "t") {pop_stack(); return nullptr;}
        rm_stack();
        return new escape::a0::b1(_t0);
    }

    std::string escape::a0::b1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    escape::a0::b2* escape::a0::b2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "r") {pop_stack(); return nullptr;}
        rm_stack();
        return new escape::a0::b2(_t0);
    }

    std::string escape::a0::b2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    escape::a0::b3* escape::a0::b3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "f") {pop_stack(); return nullptr;}
        rm_stack();
        return new escape::a0::b3(_t0);
    }

    std::string escape::a0::b3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    escape::a0::b4* escape::a0::b4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "b") {pop_stack(); return nullptr;}
        rm_stack();
        return new escape::a0::b4(_t0);
    }

    std::string escape::a0::b4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    escape::a0::b5* escape::a0::b5::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\"") {pop_stack(); return nullptr;}
        rm_stack();
        return new escape::a0::b5(_t0);
    }

    std::string escape::a0::b5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    escape::a0::b6* escape::a0::b6::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\\") {pop_stack(); return nullptr;}
        rm_stack();
        return new escape::a0::b6(_t0);
    }

    std::string escape::a0::b6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    escape::a0::b7* escape::a0::b7::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "'") {pop_stack(); return nullptr;}
        rm_stack();
        return new escape::a0::b7(_t0);
    }

    std::string escape::a0::b7::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    escape::a0::b8* escape::a0::b8::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "0") {pop_stack(); return nullptr;}
        rm_stack();
        return new escape::a0::b8(_t0);
    }

    std::string escape::a0::b8::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    escape::a0* escape::a0::parse() {
        if(auto x = escape::a0::b0::parse()) return new escape::a0(x);
        if(auto x = escape::a0::b1::parse()) return new escape::a0(x);
        if(auto x = escape::a0::b2::parse()) return new escape::a0(x);
        if(auto x = escape::a0::b3::parse()) return new escape::a0(x);
        if(auto x = escape::a0::b4::parse()) return new escape::a0(x);
        if(auto x = escape::a0::b5::parse()) return new escape::a0(x);
        if(auto x = escape::a0::b6::parse()) return new escape::a0(x);
        if(auto x = escape::a0::b7::parse()) return new escape::a0(x);
        if(auto x = escape::a0::b8::parse()) return new escape::a0(x);
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

    escape* escape::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\\") {pop_stack(); return nullptr;}
        escape::a0 *_t1 = escape::a0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new escape(_t0, _t1);
    }

    std::string escape::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        return ans;
    }

    symbol::a0* symbol::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "[") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a0(_t0);
    }

    std::string symbol::a0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a1* symbol::a1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "]") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a1(_t0);
    }

    std::string symbol::a1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a2* symbol::a2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "{") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a2(_t0);
    }

    std::string symbol::a2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a3* symbol::a3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "}") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a3(_t0);
    }

    std::string symbol::a3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a4* symbol::a4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "(") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a4(_t0);
    }

    std::string symbol::a4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a5* symbol::a5::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ")") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a5(_t0);
    }

    std::string symbol::a5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a6* symbol::a6::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "<") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a6(_t0);
    }

    std::string symbol::a6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a7* symbol::a7::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ">") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a7(_t0);
    }

    std::string symbol::a7::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a8* symbol::a8::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "=") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a8(_t0);
    }

    std::string symbol::a8::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a9* symbol::a9::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "|") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a9(_t0);
    }

    std::string symbol::a9::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a10* symbol::a10::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ".") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a10(_t0);
    }

    std::string symbol::a10::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a11* symbol::a11::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ",") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a11(_t0);
    }

    std::string symbol::a11::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a12* symbol::a12::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a12(_t0);
    }

    std::string symbol::a12::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a13* symbol::a13::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "-") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a13(_t0);
    }

    std::string symbol::a13::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a14* symbol::a14::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "+") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a14(_t0);
    }

    std::string symbol::a14::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a15* symbol::a15::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "_") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a15(_t0);
    }

    std::string symbol::a15::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a16* symbol::a16::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "*") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a16(_t0);
    }

    std::string symbol::a16::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a17* symbol::a17::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "?") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a17(_t0);
    }

    std::string symbol::a17::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a18* symbol::a18::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != ":") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a18(_t0);
    }

    std::string symbol::a18::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a19* symbol::a19::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "!") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a19(_t0);
    }

    std::string symbol::a19::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a20* symbol::a20::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "@") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a20(_t0);
    }

    std::string symbol::a20::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a21* symbol::a21::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "#") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a21(_t0);
    }

    std::string symbol::a21::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a22* symbol::a22::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "$") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a22(_t0);
    }

    std::string symbol::a22::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a23* symbol::a23::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "%") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a23(_t0);
    }

    std::string symbol::a23::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a24* symbol::a24::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "^") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a24(_t0);
    }

    std::string symbol::a24::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a25* symbol::a25::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "&") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a25(_t0);
    }

    std::string symbol::a25::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a26* symbol::a26::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "/") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a26(_t0);
    }

    std::string symbol::a26::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a27* symbol::a27::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "~") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a27(_t0);
    }

    std::string symbol::a27::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol::a28* symbol::a28::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "`") {pop_stack(); return nullptr;}
        rm_stack();
        return new symbol::a28(_t0);
    }

    std::string symbol::a28::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    symbol* symbol::parse() {
        if(auto x = symbol::a0::parse()) return new symbol(x);
        if(auto x = symbol::a1::parse()) return new symbol(x);
        if(auto x = symbol::a2::parse()) return new symbol(x);
        if(auto x = symbol::a3::parse()) return new symbol(x);
        if(auto x = symbol::a4::parse()) return new symbol(x);
        if(auto x = symbol::a5::parse()) return new symbol(x);
        if(auto x = symbol::a6::parse()) return new symbol(x);
        if(auto x = symbol::a7::parse()) return new symbol(x);
        if(auto x = symbol::a8::parse()) return new symbol(x);
        if(auto x = symbol::a9::parse()) return new symbol(x);
        if(auto x = symbol::a10::parse()) return new symbol(x);
        if(auto x = symbol::a11::parse()) return new symbol(x);
        if(auto x = symbol::a12::parse()) return new symbol(x);
        if(auto x = symbol::a13::parse()) return new symbol(x);
        if(auto x = symbol::a14::parse()) return new symbol(x);
        if(auto x = symbol::a15::parse()) return new symbol(x);
        if(auto x = symbol::a16::parse()) return new symbol(x);
        if(auto x = symbol::a17::parse()) return new symbol(x);
        if(auto x = symbol::a18::parse()) return new symbol(x);
        if(auto x = symbol::a19::parse()) return new symbol(x);
        if(auto x = symbol::a20::parse()) return new symbol(x);
        if(auto x = symbol::a21::parse()) return new symbol(x);
        if(auto x = symbol::a22::parse()) return new symbol(x);
        if(auto x = symbol::a23::parse()) return new symbol(x);
        if(auto x = symbol::a24::parse()) return new symbol(x);
        if(auto x = symbol::a25::parse()) return new symbol(x);
        if(auto x = symbol::a26::parse()) return new symbol(x);
        if(auto x = symbol::a27::parse()) return new symbol(x);
        if(auto x = symbol::a28::parse()) return new symbol(x);
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

    line_comment::a0::b0* line_comment::a0::b0::parse() {
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new line_comment::a0::b0(_t0);
    }

    std::string line_comment::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    line_comment::a0::b1* line_comment::a0::b1::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new line_comment::a0::b1(_t0);
    }

    std::string line_comment::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    line_comment::a0::b2* line_comment::a0::b2::parse() {
        push_stack();
        symbol *_t0 = symbol::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new line_comment::a0::b2(_t0);
    }

    std::string line_comment::a0::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    line_comment::a0::b3* line_comment::a0::b3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\"") {pop_stack(); return nullptr;}
        rm_stack();
        return new line_comment::a0::b3(_t0);
    }

    std::string line_comment::a0::b3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    line_comment::a0::b4* line_comment::a0::b4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "'") {pop_stack(); return nullptr;}
        rm_stack();
        return new line_comment::a0::b4(_t0);
    }

    std::string line_comment::a0::b4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    line_comment::a0::b5* line_comment::a0::b5::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != " ") {pop_stack(); return nullptr;}
        rm_stack();
        return new line_comment::a0::b5(_t0);
    }

    std::string line_comment::a0::b5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    line_comment::a0::b6* line_comment::a0::b6::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\\") {pop_stack(); return nullptr;}
        rm_stack();
        return new line_comment::a0::b6(_t0);
    }

    std::string line_comment::a0::b6::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    line_comment::a0* line_comment::a0::parse() {
        if(auto x = line_comment::a0::b0::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b1::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b2::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b3::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b4::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b5::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b6::parse()) return new line_comment::a0(x);
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

    line_comment* line_comment::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "//") {pop_stack(); return nullptr;}
        std::vector<line_comment::a0*> _t1;
        while(true) {
            line_comment::a0 *tmp = line_comment::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        std::string _t2 = next_chars(1);
        if(_t2 != "\n") {pop_stack(); return nullptr;}
        rm_stack();
        return new line_comment(_t0, _t1, _t2);
    }

    std::string line_comment::to_string() {
        std::string ans = "";
        ans += t0;
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2;
        return ans;
    }

    multiline_comment::a0::b0* multiline_comment::a0::b0::parse() {
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new multiline_comment::a0::b0(_t0);
    }

    std::string multiline_comment::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    multiline_comment::a0::b1* multiline_comment::a0::b1::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new multiline_comment::a0::b1(_t0);
    }

    std::string multiline_comment::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    multiline_comment::a0::b2* multiline_comment::a0::b2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != " ") {pop_stack(); return nullptr;}
        rm_stack();
        return new multiline_comment::a0::b2(_t0);
    }

    std::string multiline_comment::a0::b2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    multiline_comment::a0::b3* multiline_comment::a0::b3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\n") {pop_stack(); return nullptr;}
        rm_stack();
        return new multiline_comment::a0::b3(_t0);
    }

    std::string multiline_comment::a0::b3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    multiline_comment::a0* multiline_comment::a0::parse() {
        if(auto x = multiline_comment::a0::b0::parse()) return new multiline_comment::a0(x);
        if(auto x = multiline_comment::a0::b1::parse()) return new multiline_comment::a0(x);
        if(auto x = multiline_comment::a0::b2::parse()) return new multiline_comment::a0(x);
        if(auto x = multiline_comment::a0::b3::parse()) return new multiline_comment::a0(x);
        return nullptr;
    }

    std::string multiline_comment::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        assert(false);
    }

    multiline_comment* multiline_comment::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "/*") {pop_stack(); return nullptr;}
        std::vector<multiline_comment::a0*> _t1;
        while(true) {
            multiline_comment::a0 *tmp = multiline_comment::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        std::string _t2 = next_chars(2);
        if(_t2 != "*/") {pop_stack(); return nullptr;}
        rm_stack();
        return new multiline_comment(_t0, _t1, _t2);
    }

    std::string multiline_comment::to_string() {
        std::string ans = "";
        ans += t0;
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        ans += t2;
        return ans;
    }

    ws::a0* ws::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != " ") {pop_stack(); return nullptr;}
        rm_stack();
        return new ws::a0(_t0);
    }

    std::string ws::a0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    ws::a1* ws::a1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\n") {pop_stack(); return nullptr;}
        rm_stack();
        return new ws::a1(_t0);
    }

    std::string ws::a1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    ws::a2* ws::a2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\t") {pop_stack(); return nullptr;}
        rm_stack();
        return new ws::a2(_t0);
    }

    std::string ws::a2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    ws::a3* ws::a3::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\r") {pop_stack(); return nullptr;}
        rm_stack();
        return new ws::a3(_t0);
    }

    std::string ws::a3::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    ws::a4* ws::a4::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\f") {pop_stack(); return nullptr;}
        rm_stack();
        return new ws::a4(_t0);
    }

    std::string ws::a4::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    ws::a5* ws::a5::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "\b") {pop_stack(); return nullptr;}
        rm_stack();
        return new ws::a5(_t0);
    }

    std::string ws::a5::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    ws::a6* ws::a6::parse() {
        push_stack();
        line_comment *_t0 = line_comment::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new ws::a6(_t0);
    }

    std::string ws::a6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    ws::a7* ws::a7::parse() {
        push_stack();
        multiline_comment *_t0 = multiline_comment::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new ws::a7(_t0);
    }

    std::string ws::a7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    ws* ws::parse() {
        if(auto x = ws::a0::parse()) return new ws(x);
        if(auto x = ws::a1::parse()) return new ws(x);
        if(auto x = ws::a2::parse()) return new ws(x);
        if(auto x = ws::a3::parse()) return new ws(x);
        if(auto x = ws::a4::parse()) return new ws(x);
        if(auto x = ws::a5::parse()) return new ws(x);
        if(auto x = ws::a6::parse()) return new ws(x);
        if(auto x = ws::a7::parse()) return new ws(x);
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

    rws::a0* rws::a0::parse() {
        push_stack();
        ws *_t0 = ws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new rws::a0(_t0);
    }

    std::string rws::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    rws* rws::parse() {
        push_stack();
        std::vector<rws::a0*> _t0;
        while(true) {
            rws::a0 *tmp = rws::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        if(_t0.size() == 0) {pop_stack(); return nullptr;}
        rm_stack();
        return new rws(_t0);
    }

    std::string rws::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        return ans;
    }

    ows::a0* ows::a0::parse() {
        push_stack();
        ws *_t0 = ws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new ows::a0(_t0);
    }

    std::string ows::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    ows* ows::parse() {
        push_stack();
        std::vector<ows::a0*> _t0;
        while(true) {
            ows::a0 *tmp = ows::a0::parse();
            if(tmp == nullptr) break;
            _t0.push_back(tmp);
        }
        rm_stack();
        return new ows(_t0);
    }

    std::string ows::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        return ans;
    }

    base_type::a0::b0* base_type::a0::b0::parse() {
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new base_type::a0::b0(_t0);
    }

    std::string base_type::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    base_type::a0::b1* base_type::a0::b1::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new base_type::a0::b1(_t0);
    }

    std::string base_type::a0::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    base_type::a0::b2* base_type::a0::b2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "_") {pop_stack(); return nullptr;}
        rm_stack();
        return new base_type::a0::b2(_t0);
    }

    std::string base_type::a0::b2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    base_type::a0* base_type::a0::parse() {
        if(auto x = base_type::a0::b0::parse()) return new base_type::a0(x);
        if(auto x = base_type::a0::b1::parse()) return new base_type::a0(x);
        if(auto x = base_type::a0::b2::parse()) return new base_type::a0(x);
        return nullptr;
    }

    std::string base_type::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        assert(false);
    }

    base_type* base_type::parse() {
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
        return new base_type(_t0, _t1);
    }

    std::string base_type::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    templated_type::a0::b0* templated_type::a0::b0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(1);
        if(_t1 != ",") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        templated_type *_t3 = templated_type::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new templated_type::a0::b0(_t0, _t1, _t2, _t3);
    }

    std::string templated_type::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    templated_type::a0* templated_type::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "<") {pop_stack(); return nullptr;}
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
        std::string _t5 = next_chars(1);
        if(_t5 != ">") {pop_stack(); return nullptr;}
        rm_stack();
        return new templated_type::a0(_t0, _t1, _t2, _t3, _t4, _t5);
    }

    std::string templated_type::a0::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        for(int i = 0; i < t3.size(); i++) ans += t3[i]->to_string();
        ans += t4->to_string();
        ans += t5;
        return ans;
    }

    templated_type::a1::b0* templated_type::a1::b0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "*") {pop_stack(); return nullptr;}
        rm_stack();
        return new templated_type::a1::b0(_t0);
    }

    std::string templated_type::a1::b0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    templated_type::a1::b1* templated_type::a1::b1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "[") {pop_stack(); return nullptr;}
        literal_integer *_t1 = literal_integer::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "]") {pop_stack(); return nullptr;}
        rm_stack();
        return new templated_type::a1::b1(_t0, _t1, _t2);
    }

    std::string templated_type::a1::b1::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        return ans;
    }

    templated_type::a1* templated_type::a1::parse() {
        if(auto x = templated_type::a1::b0::parse()) return new templated_type::a1(x);
        if(auto x = templated_type::a1::b1::parse()) return new templated_type::a1(x);
        return nullptr;
    }

    std::string templated_type::a1::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    templated_type* templated_type::parse() {
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
        return new templated_type(_t0, _t1, _t2);
    }

    std::string templated_type::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        if(t1 != nullptr) ans += t1->to_string();
        for(int i = 0; i < t2.size(); i++) ans += t2[i]->to_string();
        return ans;
    }

    type::a0* type::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "&") {pop_stack(); return nullptr;}
        rm_stack();
        return new type::a0(_t0);
    }

    std::string type::a0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    type* type::parse() {
        push_stack();
        templated_type *_t0 = templated_type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        type::a0 *_t1 = type::a0::parse();
        rm_stack();
        return new type(_t0, _t1);
    }

    std::string type::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        if(t1 != nullptr) ans += t1->to_string();
        return ans;
    }

    template_header::a0* template_header::a0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(1);
        if(_t1 != ",") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        base_type *_t3 = base_type::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new template_header::a0(_t0, _t1, _t2, _t3);
    }

    std::string template_header::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    template_header* template_header::parse() {
        push_stack();
        std::string _t0 = next_chars(8);
        if(_t0 != "template") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "<") {pop_stack(); return nullptr;}
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
        std::string _t7 = next_chars(1);
        if(_t7 != ">") {pop_stack(); return nullptr;}
        rm_stack();
        return new template_header(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7);
    }

    std::string template_header::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        ans += t4->to_string();
        for(int i = 0; i < t5.size(); i++) ans += t5[i]->to_string();
        ans += t6->to_string();
        ans += t7;
        return ans;
    }

    identifier::a0::b0* identifier::a0::b0::parse() {
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new identifier::a0::b0(_t0);
    }

    std::string identifier::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    identifier::a0::b1* identifier::a0::b1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "_") {pop_stack(); return nullptr;}
        rm_stack();
        return new identifier::a0::b1(_t0);
    }

    std::string identifier::a0::b1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    identifier::a0* identifier::a0::parse() {
        if(auto x = identifier::a0::b0::parse()) return new identifier::a0(x);
        if(auto x = identifier::a0::b1::parse()) return new identifier::a0(x);
        return nullptr;
    }

    std::string identifier::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    identifier::a1::b0* identifier::a1::b0::parse() {
        push_stack();
        alpha *_t0 = alpha::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new identifier::a1::b0(_t0);
    }

    std::string identifier::a1::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    identifier::a1::b1* identifier::a1::b1::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new identifier::a1::b1(_t0);
    }

    std::string identifier::a1::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    identifier::a1::b2* identifier::a1::b2::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "_") {pop_stack(); return nullptr;}
        rm_stack();
        return new identifier::a1::b2(_t0);
    }

    std::string identifier::a1::b2::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    identifier::a1* identifier::a1::parse() {
        if(auto x = identifier::a1::b0::parse()) return new identifier::a1(x);
        if(auto x = identifier::a1::b1::parse()) return new identifier::a1(x);
        if(auto x = identifier::a1::b2::parse()) return new identifier::a1(x);
        return nullptr;
    }

    std::string identifier::a1::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        assert(false);
    }

    identifier* identifier::parse() {
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
        return new identifier(_t0, _t1);
    }

    std::string identifier::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    declaration::a0* declaration::a0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(1);
        if(_t1 != "=") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expression *_t3 = expression::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new declaration::a0(_t0, _t1, _t2, _t3);
    }

    std::string declaration::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    declaration* declaration::parse() {
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        declaration::a0 *_t3 = declaration::a0::parse();
        rm_stack();
        return new declaration(_t0, _t1, _t2, _t3);
    }

    std::string declaration::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        if(t3 != nullptr) ans += t3->to_string();
        return ans;
    }

    parameter* parameter::parse() {
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        identifier *_t2 = identifier::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new parameter(_t0, _t1, _t2);
    }

    std::string parameter::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        return ans;
    }

    parameter_list::a0::b0* parameter_list::a0::b0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(1);
        if(_t1 != ",") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        parameter *_t3 = parameter::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new parameter_list::a0::b0(_t0, _t1, _t2, _t3);
    }

    std::string parameter_list::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    parameter_list::a0* parameter_list::a0::parse() {
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
        return new parameter_list::a0(_t0, _t1);
    }

    std::string parameter_list::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    parameter_list* parameter_list::parse() {
        push_stack();
        parameter_list::a0 *_t0 = parameter_list::a0::parse();
        rm_stack();
        return new parameter_list(_t0);
    }

    std::string parameter_list::to_string() {
        std::string ans = "";
        if(t0 != nullptr) ans += t0->to_string();
        return ans;
    }

    argument_list::a0::b0* argument_list::a0::b0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(1);
        if(_t1 != ",") {pop_stack(); return nullptr;}
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        expression *_t3 = expression::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new argument_list::a0::b0(_t0, _t1, _t2, _t3);
    }

    std::string argument_list::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    argument_list::a0* argument_list::a0::parse() {
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
        return new argument_list::a0(_t0, _t1);
    }

    std::string argument_list::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
        return ans;
    }

    argument_list* argument_list::parse() {
        push_stack();
        argument_list::a0 *_t0 = argument_list::a0::parse();
        rm_stack();
        return new argument_list(_t0);
    }

    std::string argument_list::to_string() {
        std::string ans = "";
        if(t0 != nullptr) ans += t0->to_string();
        return ans;
    }

    statement::a0* statement::a0::parse() {
        push_stack();
        simple_statement *_t0 = simple_statement::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new statement::a0(_t0);
    }

    std::string statement::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    statement::a1* statement::a1::parse() {
        push_stack();
        control_statement *_t0 = control_statement::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new statement::a1(_t0);
    }

    std::string statement::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    statement::a2* statement::a2::parse() {
        push_stack();
        compound_statement *_t0 = compound_statement::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new statement::a2(_t0);
    }

    std::string statement::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    statement* statement::parse() {
        if(auto x = statement::a0::parse()) return new statement(x);
        if(auto x = statement::a1::parse()) return new statement(x);
        if(auto x = statement::a2::parse()) return new statement(x);
        return nullptr;
    }

    std::string statement::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        assert(false);
    }

    simple_statement::a0::b0* simple_statement::a0::b0::parse() {
        push_stack();
        rws *_t0 = rws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        expression *_t1 = expression::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new simple_statement::a0::b0(_t0, _t1);
    }

    std::string simple_statement::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    simple_statement::a0* simple_statement::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(6);
        if(_t0 != "return") {pop_stack(); return nullptr;}
        simple_statement::a0::b0 *_t1 = simple_statement::a0::b0::parse();
        ows *_t2 = ows::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        std::string _t3 = next_chars(1);
        if(_t3 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new simple_statement::a0(_t0, _t1, _t2, _t3);
    }

    std::string simple_statement::a0::to_string() {
        std::string ans = "";
        ans += t0;
        if(t1 != nullptr) ans += t1->to_string();
        ans += t2->to_string();
        ans += t3;
        return ans;
    }

    simple_statement::a1* simple_statement::a1::parse() {
        push_stack();
        std::string _t0 = next_chars(5);
        if(_t0 != "break") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new simple_statement::a1(_t0, _t1, _t2);
    }

    std::string simple_statement::a1::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        return ans;
    }

    simple_statement::a2* simple_statement::a2::parse() {
        push_stack();
        std::string _t0 = next_chars(8);
        if(_t0 != "continue") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new simple_statement::a2(_t0, _t1, _t2);
    }

    std::string simple_statement::a2::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        return ans;
    }

    simple_statement::a3* simple_statement::a3::parse() {
        push_stack();
        declaration *_t0 = declaration::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new simple_statement::a3(_t0, _t1, _t2);
    }

    std::string simple_statement::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2;
        return ans;
    }

    simple_statement::a4* simple_statement::a4::parse() {
        push_stack();
        expression *_t0 = expression::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new simple_statement::a4(_t0, _t1, _t2);
    }

    std::string simple_statement::a4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2;
        return ans;
    }

    simple_statement::a5* simple_statement::a5::parse() {
        push_stack();
        std::string _t0 = next_chars(4);
        if(_t0 != "asm!") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "(") {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        literal_string *_t4 = literal_string::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::string _t6 = next_chars(1);
        if(_t6 != ")") {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        std::string _t8 = next_chars(1);
        if(_t8 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new simple_statement::a5(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
    }

    std::string simple_statement::a5::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6;
        ans += t7->to_string();
        ans += t8;
        return ans;
    }

    simple_statement* simple_statement::parse() {
        if(auto x = simple_statement::a0::parse()) return new simple_statement(x);
        if(auto x = simple_statement::a1::parse()) return new simple_statement(x);
        if(auto x = simple_statement::a2::parse()) return new simple_statement(x);
        if(auto x = simple_statement::a3::parse()) return new simple_statement(x);
        if(auto x = simple_statement::a4::parse()) return new simple_statement(x);
        if(auto x = simple_statement::a5::parse()) return new simple_statement(x);
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

    control_statement::a0::b0* control_statement::a0::b0::parse() {
        push_stack();
        rws *_t0 = rws::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::string _t1 = next_chars(4);
        if(_t1 != "else") {pop_stack(); return nullptr;}
        rws *_t2 = rws::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        statement *_t3 = statement::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new control_statement::a0::b0(_t0, _t1, _t2, _t3);
    }

    std::string control_statement::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1;
        ans += t2->to_string();
        ans += t3->to_string();
        return ans;
    }

    control_statement::a0* control_statement::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(2);
        if(_t0 != "if") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "(") {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        expression *_t4 = expression::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::string _t6 = next_chars(1);
        if(_t6 != ")") {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        statement *_t8 = statement::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        control_statement::a0::b0 *_t9 = control_statement::a0::b0::parse();
        rm_stack();
        return new control_statement::a0(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
    }

    std::string control_statement::a0::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6;
        ans += t7->to_string();
        ans += t8->to_string();
        if(t9 != nullptr) ans += t9->to_string();
        return ans;
    }

    control_statement::a1* control_statement::a1::parse() {
        push_stack();
        std::string _t0 = next_chars(5);
        if(_t0 != "while") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "(") {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        expression *_t4 = expression::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::string _t6 = next_chars(1);
        if(_t6 != ")") {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        statement *_t8 = statement::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new control_statement::a1(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
    }

    std::string control_statement::a1::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        ans += t4->to_string();
        ans += t5->to_string();
        ans += t6;
        ans += t7->to_string();
        ans += t8->to_string();
        return ans;
    }

    control_statement::a2::b0* control_statement::a2::b0::parse() {
        push_stack();
        declaration *_t0 = declaration::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new control_statement::a2::b0(_t0);
    }

    std::string control_statement::a2::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    control_statement::a2::b1* control_statement::a2::b1::parse() {
        push_stack();
        expression *_t0 = expression::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new control_statement::a2::b1(_t0);
    }

    std::string control_statement::a2::b1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    control_statement::a2::b2* control_statement::a2::b2::parse() {
        push_stack();
        expression *_t0 = expression::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new control_statement::a2::b2(_t0);
    }

    std::string control_statement::a2::b2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    control_statement::a2* control_statement::a2::parse() {
        push_stack();
        std::string _t0 = next_chars(3);
        if(_t0 != "for") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != "(") {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        control_statement::a2::b0 *_t4 = control_statement::a2::b0::parse();
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        std::string _t6 = next_chars(1);
        if(_t6 != ";") {pop_stack(); return nullptr;}
        ows *_t7 = ows::parse();
        if(_t7 == nullptr) {pop_stack(); return nullptr;}
        control_statement::a2::b1 *_t8 = control_statement::a2::b1::parse();
        ows *_t9 = ows::parse();
        if(_t9 == nullptr) {pop_stack(); return nullptr;}
        std::string _t10 = next_chars(1);
        if(_t10 != ";") {pop_stack(); return nullptr;}
        ows *_t11 = ows::parse();
        if(_t11 == nullptr) {pop_stack(); return nullptr;}
        control_statement::a2::b2 *_t12 = control_statement::a2::b2::parse();
        ows *_t13 = ows::parse();
        if(_t13 == nullptr) {pop_stack(); return nullptr;}
        std::string _t14 = next_chars(1);
        if(_t14 != ")") {pop_stack(); return nullptr;}
        ows *_t15 = ows::parse();
        if(_t15 == nullptr) {pop_stack(); return nullptr;}
        statement *_t16 = statement::parse();
        if(_t16 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new control_statement::a2(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10, _t11, _t12, _t13, _t14, _t15, _t16);
    }

    std::string control_statement::a2::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        ans += t3->to_string();
        if(t4 != nullptr) ans += t4->to_string();
        ans += t5->to_string();
        ans += t6;
        ans += t7->to_string();
        if(t8 != nullptr) ans += t8->to_string();
        ans += t9->to_string();
        ans += t10;
        ans += t11->to_string();
        if(t12 != nullptr) ans += t12->to_string();
        ans += t13->to_string();
        ans += t14;
        ans += t15->to_string();
        ans += t16->to_string();
        return ans;
    }

    control_statement* control_statement::parse() {
        if(auto x = control_statement::a0::parse()) return new control_statement(x);
        if(auto x = control_statement::a1::parse()) return new control_statement(x);
        if(auto x = control_statement::a2::parse()) return new control_statement(x);
        return nullptr;
    }

    std::string control_statement::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        assert(false);
    }

    compound_statement::a0* compound_statement::a0::parse() {
        push_stack();
        statement *_t0 = statement::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new compound_statement::a0(_t0, _t1);
    }

    std::string compound_statement::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    compound_statement* compound_statement::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "{") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::vector<compound_statement::a0*> _t2;
        while(true) {
            compound_statement::a0 *tmp = compound_statement::a0::parse();
            if(tmp == nullptr) break;
            _t2.push_back(tmp);
        }
        std::string _t3 = next_chars(1);
        if(_t3 != "}") {pop_stack(); return nullptr;}
        rm_stack();
        return new compound_statement(_t0, _t1, _t2, _t3);
    }

    std::string compound_statement::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        for(int i = 0; i < t2.size(); i++) ans += t2[i]->to_string();
        ans += t3;
        return ans;
    }

    include::a0::b0* include::a0::b0::parse() {
        push_stack();
        literal_string *_t0 = literal_string::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new include::a0::b0(_t0);
    }

    std::string include::a0::b0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    include::a0::b1* include::a0::b1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "<") {pop_stack(); return nullptr;}
        identifier *_t1 = identifier::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != ">") {pop_stack(); return nullptr;}
        rm_stack();
        return new include::a0::b1(_t0, _t1, _t2);
    }

    std::string include::a0::b1::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2;
        return ans;
    }

    include::a0* include::a0::parse() {
        if(auto x = include::a0::b0::parse()) return new include::a0(x);
        if(auto x = include::a0::b1::parse()) return new include::a0(x);
        return nullptr;
    }

    std::string include::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    include* include::parse() {
        push_stack();
        std::string _t0 = next_chars(8);
        if(_t0 != "#include") {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        include::a0 *_t2 = include::a0::parse();
        if(_t2 == nullptr) {pop_stack(); return nullptr;}
        ows *_t3 = ows::parse();
        if(_t3 == nullptr) {pop_stack(); return nullptr;}
        std::string _t4 = next_chars(1);
        if(_t4 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new include(_t0, _t1, _t2, _t3, _t4);
    }

    std::string include::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4;
        return ans;
    }

    global_declaration::a0* global_declaration::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "-") {pop_stack(); return nullptr;}
        rm_stack();
        return new global_declaration::a0(_t0);
    }

    std::string global_declaration::a0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    global_declaration::a1* global_declaration::a1::parse() {
        push_stack();
        digit *_t0 = digit::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new global_declaration::a1(_t0);
    }

    std::string global_declaration::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    global_declaration::a2* global_declaration::a2::parse() {
        push_stack();
        std::string _t0 = next_chars(6);
        if(_t0 != "extern") {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new global_declaration::a2(_t0, _t1);
    }

    std::string global_declaration::a2::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        return ans;
    }

    global_declaration* global_declaration::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "[") {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        global_declaration::a0 *_t2 = global_declaration::a0::parse();
        std::vector<global_declaration::a1*> _t3;
        while(true) {
            global_declaration::a1 *tmp = global_declaration::a1::parse();
            if(tmp == nullptr) break;
            _t3.push_back(tmp);
        }
        if(_t3.size() == 0) {pop_stack(); return nullptr;}
        ows *_t4 = ows::parse();
        if(_t4 == nullptr) {pop_stack(); return nullptr;}
        std::string _t5 = next_chars(1);
        if(_t5 != "]") {pop_stack(); return nullptr;}
        ows *_t6 = ows::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        global_declaration::a2 *_t7 = global_declaration::a2::parse();
        declaration *_t8 = declaration::parse();
        if(_t8 == nullptr) {pop_stack(); return nullptr;}
        ows *_t9 = ows::parse();
        if(_t9 == nullptr) {pop_stack(); return nullptr;}
        std::string _t10 = next_chars(1);
        if(_t10 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new global_declaration(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10);
    }

    std::string global_declaration::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        if(t2 != nullptr) ans += t2->to_string();
        for(int i = 0; i < t3.size(); i++) ans += t3[i]->to_string();
        ans += t4->to_string();
        ans += t5;
        ans += t6->to_string();
        if(t7 != nullptr) ans += t7->to_string();
        ans += t8->to_string();
        ans += t9->to_string();
        ans += t10;
        return ans;
    }

    program::a0::b0::c0* program::a0::b0::c0::parse() {
        push_stack();
        function *_t0 = function::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new program::a0::b0::c0(_t0);
    }

    std::string program::a0::b0::c0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    program::a0::b0::c1* program::a0::b0::c1::parse() {
        push_stack();
        struct_definition *_t0 = struct_definition::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new program::a0::b0::c1(_t0);
    }

    std::string program::a0::b0::c1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    program::a0::b0::c2* program::a0::b0::c2::parse() {
        push_stack();
        templated_function *_t0 = templated_function::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new program::a0::b0::c2(_t0);
    }

    std::string program::a0::b0::c2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    program::a0::b0::c3* program::a0::b0::c3::parse() {
        push_stack();
        templated_struct_definition *_t0 = templated_struct_definition::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new program::a0::b0::c3(_t0);
    }

    std::string program::a0::b0::c3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    program::a0::b0::c4* program::a0::b0::c4::parse() {
        push_stack();
        overload *_t0 = overload::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new program::a0::b0::c4(_t0);
    }

    std::string program::a0::b0::c4::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    program::a0::b0::c5* program::a0::b0::c5::parse() {
        push_stack();
        templated_overload *_t0 = templated_overload::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new program::a0::b0::c5(_t0);
    }

    std::string program::a0::b0::c5::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    program::a0::b0::c6* program::a0::b0::c6::parse() {
        push_stack();
        include *_t0 = include::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new program::a0::b0::c6(_t0);
    }

    std::string program::a0::b0::c6::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    program::a0::b0::c7* program::a0::b0::c7::parse() {
        push_stack();
        global_declaration *_t0 = global_declaration::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new program::a0::b0::c7(_t0);
    }

    std::string program::a0::b0::c7::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    program::a0::b0* program::a0::b0::parse() {
        if(auto x = program::a0::b0::c0::parse()) return new program::a0::b0(x);
        if(auto x = program::a0::b0::c1::parse()) return new program::a0::b0(x);
        if(auto x = program::a0::b0::c2::parse()) return new program::a0::b0(x);
        if(auto x = program::a0::b0::c3::parse()) return new program::a0::b0(x);
        if(auto x = program::a0::b0::c4::parse()) return new program::a0::b0(x);
        if(auto x = program::a0::b0::c5::parse()) return new program::a0::b0(x);
        if(auto x = program::a0::b0::c6::parse()) return new program::a0::b0(x);
        if(auto x = program::a0::b0::c7::parse()) return new program::a0::b0(x);
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
        assert(false);
    }

    program::a0* program::a0::parse() {
        push_stack();
        ows *_t0 = ows::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        program::a0::b0 *_t1 = program::a0::b0::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new program::a0(_t0, _t1);
    }

    std::string program::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        return ans;
    }

    program* program::parse() {
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
        return new program(_t0, _t1);
    }

    std::string program::to_string() {
        std::string ans = "";
        for(int i = 0; i < t0.size(); i++) ans += t0[i]->to_string();
        ans += t1->to_string();
        return ans;
    }

};
