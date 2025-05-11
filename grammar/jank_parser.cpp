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

struct alpha;
struct digit;
struct number;
struct ws;
struct rws;
struct ows;
struct identifier;
struct type_identifier;
struct expression;
struct declaration;
struct parameter;
struct parameter_list;
struct statement;
struct compound_statement;
struct function_definition;
struct function;
struct program;

// alpha = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" ;
struct alpha {
    struct a0 {
        std::string t0;
        a0(std::string _t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    struct a1 {
        std::string t0;
        a1(std::string _t0) {
            t0 = _t0;
        }
        static a1* parse();
    };
    struct a2 {
        std::string t0;
        a2(std::string _t0) {
            t0 = _t0;
        }
        static a2* parse();
    };
    struct a3 {
        std::string t0;
        a3(std::string _t0) {
            t0 = _t0;
        }
        static a3* parse();
    };
    struct a4 {
        std::string t0;
        a4(std::string _t0) {
            t0 = _t0;
        }
        static a4* parse();
    };
    struct a5 {
        std::string t0;
        a5(std::string _t0) {
            t0 = _t0;
        }
        static a5* parse();
    };
    struct a6 {
        std::string t0;
        a6(std::string _t0) {
            t0 = _t0;
        }
        static a6* parse();
    };
    struct a7 {
        std::string t0;
        a7(std::string _t0) {
            t0 = _t0;
        }
        static a7* parse();
    };
    struct a8 {
        std::string t0;
        a8(std::string _t0) {
            t0 = _t0;
        }
        static a8* parse();
    };
    struct a9 {
        std::string t0;
        a9(std::string _t0) {
            t0 = _t0;
        }
        static a9* parse();
    };
    struct a10 {
        std::string t0;
        a10(std::string _t0) {
            t0 = _t0;
        }
        static a10* parse();
    };
    struct a11 {
        std::string t0;
        a11(std::string _t0) {
            t0 = _t0;
        }
        static a11* parse();
    };
    struct a12 {
        std::string t0;
        a12(std::string _t0) {
            t0 = _t0;
        }
        static a12* parse();
    };
    struct a13 {
        std::string t0;
        a13(std::string _t0) {
            t0 = _t0;
        }
        static a13* parse();
    };
    struct a14 {
        std::string t0;
        a14(std::string _t0) {
            t0 = _t0;
        }
        static a14* parse();
    };
    struct a15 {
        std::string t0;
        a15(std::string _t0) {
            t0 = _t0;
        }
        static a15* parse();
    };
    struct a16 {
        std::string t0;
        a16(std::string _t0) {
            t0 = _t0;
        }
        static a16* parse();
    };
    struct a17 {
        std::string t0;
        a17(std::string _t0) {
            t0 = _t0;
        }
        static a17* parse();
    };
    struct a18 {
        std::string t0;
        a18(std::string _t0) {
            t0 = _t0;
        }
        static a18* parse();
    };
    struct a19 {
        std::string t0;
        a19(std::string _t0) {
            t0 = _t0;
        }
        static a19* parse();
    };
    struct a20 {
        std::string t0;
        a20(std::string _t0) {
            t0 = _t0;
        }
        static a20* parse();
    };
    struct a21 {
        std::string t0;
        a21(std::string _t0) {
            t0 = _t0;
        }
        static a21* parse();
    };
    struct a22 {
        std::string t0;
        a22(std::string _t0) {
            t0 = _t0;
        }
        static a22* parse();
    };
    struct a23 {
        std::string t0;
        a23(std::string _t0) {
            t0 = _t0;
        }
        static a23* parse();
    };
    struct a24 {
        std::string t0;
        a24(std::string _t0) {
            t0 = _t0;
        }
        static a24* parse();
    };
    struct a25 {
        std::string t0;
        a25(std::string _t0) {
            t0 = _t0;
        }
        static a25* parse();
    };
    struct a26 {
        std::string t0;
        a26(std::string _t0) {
            t0 = _t0;
        }
        static a26* parse();
    };
    struct a27 {
        std::string t0;
        a27(std::string _t0) {
            t0 = _t0;
        }
        static a27* parse();
    };
    struct a28 {
        std::string t0;
        a28(std::string _t0) {
            t0 = _t0;
        }
        static a28* parse();
    };
    struct a29 {
        std::string t0;
        a29(std::string _t0) {
            t0 = _t0;
        }
        static a29* parse();
    };
    struct a30 {
        std::string t0;
        a30(std::string _t0) {
            t0 = _t0;
        }
        static a30* parse();
    };
    struct a31 {
        std::string t0;
        a31(std::string _t0) {
            t0 = _t0;
        }
        static a31* parse();
    };
    struct a32 {
        std::string t0;
        a32(std::string _t0) {
            t0 = _t0;
        }
        static a32* parse();
    };
    struct a33 {
        std::string t0;
        a33(std::string _t0) {
            t0 = _t0;
        }
        static a33* parse();
    };
    struct a34 {
        std::string t0;
        a34(std::string _t0) {
            t0 = _t0;
        }
        static a34* parse();
    };
    struct a35 {
        std::string t0;
        a35(std::string _t0) {
            t0 = _t0;
        }
        static a35* parse();
    };
    struct a36 {
        std::string t0;
        a36(std::string _t0) {
            t0 = _t0;
        }
        static a36* parse();
    };
    struct a37 {
        std::string t0;
        a37(std::string _t0) {
            t0 = _t0;
        }
        static a37* parse();
    };
    struct a38 {
        std::string t0;
        a38(std::string _t0) {
            t0 = _t0;
        }
        static a38* parse();
    };
    struct a39 {
        std::string t0;
        a39(std::string _t0) {
            t0 = _t0;
        }
        static a39* parse();
    };
    struct a40 {
        std::string t0;
        a40(std::string _t0) {
            t0 = _t0;
        }
        static a40* parse();
    };
    struct a41 {
        std::string t0;
        a41(std::string _t0) {
            t0 = _t0;
        }
        static a41* parse();
    };
    struct a42 {
        std::string t0;
        a42(std::string _t0) {
            t0 = _t0;
        }
        static a42* parse();
    };
    struct a43 {
        std::string t0;
        a43(std::string _t0) {
            t0 = _t0;
        }
        static a43* parse();
    };
    struct a44 {
        std::string t0;
        a44(std::string _t0) {
            t0 = _t0;
        }
        static a44* parse();
    };
    struct a45 {
        std::string t0;
        a45(std::string _t0) {
            t0 = _t0;
        }
        static a45* parse();
    };
    struct a46 {
        std::string t0;
        a46(std::string _t0) {
            t0 = _t0;
        }
        static a46* parse();
    };
    struct a47 {
        std::string t0;
        a47(std::string _t0) {
            t0 = _t0;
        }
        static a47* parse();
    };
    struct a48 {
        std::string t0;
        a48(std::string _t0) {
            t0 = _t0;
        }
        static a48* parse();
    };
    struct a49 {
        std::string t0;
        a49(std::string _t0) {
            t0 = _t0;
        }
        static a49* parse();
    };
    struct a50 {
        std::string t0;
        a50(std::string _t0) {
            t0 = _t0;
        }
        static a50* parse();
    };
    struct a51 {
        std::string t0;
        a51(std::string _t0) {
            t0 = _t0;
        }
        static a51* parse();
    };
    bool is_a0 = false;
    a0 *t0;
    bool is_a1 = false;
    a1 *t1;
    bool is_a2 = false;
    a2 *t2;
    bool is_a3 = false;
    a3 *t3;
    bool is_a4 = false;
    a4 *t4;
    bool is_a5 = false;
    a5 *t5;
    bool is_a6 = false;
    a6 *t6;
    bool is_a7 = false;
    a7 *t7;
    bool is_a8 = false;
    a8 *t8;
    bool is_a9 = false;
    a9 *t9;
    bool is_a10 = false;
    a10 *t10;
    bool is_a11 = false;
    a11 *t11;
    bool is_a12 = false;
    a12 *t12;
    bool is_a13 = false;
    a13 *t13;
    bool is_a14 = false;
    a14 *t14;
    bool is_a15 = false;
    a15 *t15;
    bool is_a16 = false;
    a16 *t16;
    bool is_a17 = false;
    a17 *t17;
    bool is_a18 = false;
    a18 *t18;
    bool is_a19 = false;
    a19 *t19;
    bool is_a20 = false;
    a20 *t20;
    bool is_a21 = false;
    a21 *t21;
    bool is_a22 = false;
    a22 *t22;
    bool is_a23 = false;
    a23 *t23;
    bool is_a24 = false;
    a24 *t24;
    bool is_a25 = false;
    a25 *t25;
    bool is_a26 = false;
    a26 *t26;
    bool is_a27 = false;
    a27 *t27;
    bool is_a28 = false;
    a28 *t28;
    bool is_a29 = false;
    a29 *t29;
    bool is_a30 = false;
    a30 *t30;
    bool is_a31 = false;
    a31 *t31;
    bool is_a32 = false;
    a32 *t32;
    bool is_a33 = false;
    a33 *t33;
    bool is_a34 = false;
    a34 *t34;
    bool is_a35 = false;
    a35 *t35;
    bool is_a36 = false;
    a36 *t36;
    bool is_a37 = false;
    a37 *t37;
    bool is_a38 = false;
    a38 *t38;
    bool is_a39 = false;
    a39 *t39;
    bool is_a40 = false;
    a40 *t40;
    bool is_a41 = false;
    a41 *t41;
    bool is_a42 = false;
    a42 *t42;
    bool is_a43 = false;
    a43 *t43;
    bool is_a44 = false;
    a44 *t44;
    bool is_a45 = false;
    a45 *t45;
    bool is_a46 = false;
    a46 *t46;
    bool is_a47 = false;
    a47 *t47;
    bool is_a48 = false;
    a48 *t48;
    bool is_a49 = false;
    a49 *t49;
    bool is_a50 = false;
    a50 *t50;
    bool is_a51 = false;
    a51 *t51;
    alpha(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    alpha(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    alpha(a2 *_t2) {
        is_a2 = true;
        t2 = _t2;
    }
    alpha(a3 *_t3) {
        is_a3 = true;
        t3 = _t3;
    }
    alpha(a4 *_t4) {
        is_a4 = true;
        t4 = _t4;
    }
    alpha(a5 *_t5) {
        is_a5 = true;
        t5 = _t5;
    }
    alpha(a6 *_t6) {
        is_a6 = true;
        t6 = _t6;
    }
    alpha(a7 *_t7) {
        is_a7 = true;
        t7 = _t7;
    }
    alpha(a8 *_t8) {
        is_a8 = true;
        t8 = _t8;
    }
    alpha(a9 *_t9) {
        is_a9 = true;
        t9 = _t9;
    }
    alpha(a10 *_t10) {
        is_a10 = true;
        t10 = _t10;
    }
    alpha(a11 *_t11) {
        is_a11 = true;
        t11 = _t11;
    }
    alpha(a12 *_t12) {
        is_a12 = true;
        t12 = _t12;
    }
    alpha(a13 *_t13) {
        is_a13 = true;
        t13 = _t13;
    }
    alpha(a14 *_t14) {
        is_a14 = true;
        t14 = _t14;
    }
    alpha(a15 *_t15) {
        is_a15 = true;
        t15 = _t15;
    }
    alpha(a16 *_t16) {
        is_a16 = true;
        t16 = _t16;
    }
    alpha(a17 *_t17) {
        is_a17 = true;
        t17 = _t17;
    }
    alpha(a18 *_t18) {
        is_a18 = true;
        t18 = _t18;
    }
    alpha(a19 *_t19) {
        is_a19 = true;
        t19 = _t19;
    }
    alpha(a20 *_t20) {
        is_a20 = true;
        t20 = _t20;
    }
    alpha(a21 *_t21) {
        is_a21 = true;
        t21 = _t21;
    }
    alpha(a22 *_t22) {
        is_a22 = true;
        t22 = _t22;
    }
    alpha(a23 *_t23) {
        is_a23 = true;
        t23 = _t23;
    }
    alpha(a24 *_t24) {
        is_a24 = true;
        t24 = _t24;
    }
    alpha(a25 *_t25) {
        is_a25 = true;
        t25 = _t25;
    }
    alpha(a26 *_t26) {
        is_a26 = true;
        t26 = _t26;
    }
    alpha(a27 *_t27) {
        is_a27 = true;
        t27 = _t27;
    }
    alpha(a28 *_t28) {
        is_a28 = true;
        t28 = _t28;
    }
    alpha(a29 *_t29) {
        is_a29 = true;
        t29 = _t29;
    }
    alpha(a30 *_t30) {
        is_a30 = true;
        t30 = _t30;
    }
    alpha(a31 *_t31) {
        is_a31 = true;
        t31 = _t31;
    }
    alpha(a32 *_t32) {
        is_a32 = true;
        t32 = _t32;
    }
    alpha(a33 *_t33) {
        is_a33 = true;
        t33 = _t33;
    }
    alpha(a34 *_t34) {
        is_a34 = true;
        t34 = _t34;
    }
    alpha(a35 *_t35) {
        is_a35 = true;
        t35 = _t35;
    }
    alpha(a36 *_t36) {
        is_a36 = true;
        t36 = _t36;
    }
    alpha(a37 *_t37) {
        is_a37 = true;
        t37 = _t37;
    }
    alpha(a38 *_t38) {
        is_a38 = true;
        t38 = _t38;
    }
    alpha(a39 *_t39) {
        is_a39 = true;
        t39 = _t39;
    }
    alpha(a40 *_t40) {
        is_a40 = true;
        t40 = _t40;
    }
    alpha(a41 *_t41) {
        is_a41 = true;
        t41 = _t41;
    }
    alpha(a42 *_t42) {
        is_a42 = true;
        t42 = _t42;
    }
    alpha(a43 *_t43) {
        is_a43 = true;
        t43 = _t43;
    }
    alpha(a44 *_t44) {
        is_a44 = true;
        t44 = _t44;
    }
    alpha(a45 *_t45) {
        is_a45 = true;
        t45 = _t45;
    }
    alpha(a46 *_t46) {
        is_a46 = true;
        t46 = _t46;
    }
    alpha(a47 *_t47) {
        is_a47 = true;
        t47 = _t47;
    }
    alpha(a48 *_t48) {
        is_a48 = true;
        t48 = _t48;
    }
    alpha(a49 *_t49) {
        is_a49 = true;
        t49 = _t49;
    }
    alpha(a50 *_t50) {
        is_a50 = true;
        t50 = _t50;
    }
    alpha(a51 *_t51) {
        is_a51 = true;
        t51 = _t51;
    }
    static alpha* parse();
};

// digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
struct digit {
    struct a0 {
        std::string t0;
        a0(std::string _t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    struct a1 {
        std::string t0;
        a1(std::string _t0) {
            t0 = _t0;
        }
        static a1* parse();
    };
    struct a2 {
        std::string t0;
        a2(std::string _t0) {
            t0 = _t0;
        }
        static a2* parse();
    };
    struct a3 {
        std::string t0;
        a3(std::string _t0) {
            t0 = _t0;
        }
        static a3* parse();
    };
    struct a4 {
        std::string t0;
        a4(std::string _t0) {
            t0 = _t0;
        }
        static a4* parse();
    };
    struct a5 {
        std::string t0;
        a5(std::string _t0) {
            t0 = _t0;
        }
        static a5* parse();
    };
    struct a6 {
        std::string t0;
        a6(std::string _t0) {
            t0 = _t0;
        }
        static a6* parse();
    };
    struct a7 {
        std::string t0;
        a7(std::string _t0) {
            t0 = _t0;
        }
        static a7* parse();
    };
    struct a8 {
        std::string t0;
        a8(std::string _t0) {
            t0 = _t0;
        }
        static a8* parse();
    };
    struct a9 {
        std::string t0;
        a9(std::string _t0) {
            t0 = _t0;
        }
        static a9* parse();
    };
    bool is_a0 = false;
    a0 *t0;
    bool is_a1 = false;
    a1 *t1;
    bool is_a2 = false;
    a2 *t2;
    bool is_a3 = false;
    a3 *t3;
    bool is_a4 = false;
    a4 *t4;
    bool is_a5 = false;
    a5 *t5;
    bool is_a6 = false;
    a6 *t6;
    bool is_a7 = false;
    a7 *t7;
    bool is_a8 = false;
    a8 *t8;
    bool is_a9 = false;
    a9 *t9;
    digit(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    digit(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    digit(a2 *_t2) {
        is_a2 = true;
        t2 = _t2;
    }
    digit(a3 *_t3) {
        is_a3 = true;
        t3 = _t3;
    }
    digit(a4 *_t4) {
        is_a4 = true;
        t4 = _t4;
    }
    digit(a5 *_t5) {
        is_a5 = true;
        t5 = _t5;
    }
    digit(a6 *_t6) {
        is_a6 = true;
        t6 = _t6;
    }
    digit(a7 *_t7) {
        is_a7 = true;
        t7 = _t7;
    }
    digit(a8 *_t8) {
        is_a8 = true;
        t8 = _t8;
    }
    digit(a9 *_t9) {
        is_a9 = true;
        t9 = _t9;
    }
    static digit* parse();
};

// number = digit , { digit } ;
struct number {
    struct a0 {
        digit *t0;
        a0(digit *_t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    digit *t0;
    std::vector<a0*> t1;
    number(digit *_t0, std::vector<a0*> _t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static number* parse();
};

// ws = " " | "\n" | "\t" | "\r" | "\f" | "\b" ;
struct ws {
    struct a0 {
        std::string t0;
        a0(std::string _t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    struct a1 {
        std::string t0;
        a1(std::string _t0) {
            t0 = _t0;
        }
        static a1* parse();
    };
    struct a2 {
        std::string t0;
        a2(std::string _t0) {
            t0 = _t0;
        }
        static a2* parse();
    };
    struct a3 {
        std::string t0;
        a3(std::string _t0) {
            t0 = _t0;
        }
        static a3* parse();
    };
    struct a4 {
        std::string t0;
        a4(std::string _t0) {
            t0 = _t0;
        }
        static a4* parse();
    };
    struct a5 {
        std::string t0;
        a5(std::string _t0) {
            t0 = _t0;
        }
        static a5* parse();
    };
    bool is_a0 = false;
    a0 *t0;
    bool is_a1 = false;
    a1 *t1;
    bool is_a2 = false;
    a2 *t2;
    bool is_a3 = false;
    a3 *t3;
    bool is_a4 = false;
    a4 *t4;
    bool is_a5 = false;
    a5 *t5;
    ws(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    ws(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    ws(a2 *_t2) {
        is_a2 = true;
        t2 = _t2;
    }
    ws(a3 *_t3) {
        is_a3 = true;
        t3 = _t3;
    }
    ws(a4 *_t4) {
        is_a4 = true;
        t4 = _t4;
    }
    ws(a5 *_t5) {
        is_a5 = true;
        t5 = _t5;
    }
    static ws* parse();
};

// rws = ws , { ws } ;
struct rws {
    struct a0 {
        ws *t0;
        a0(ws *_t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    ws *t0;
    std::vector<a0*> t1;
    rws(ws *_t0, std::vector<a0*> _t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static rws* parse();
};

// ows = { ws } ;
struct ows {
    struct a0 {
        ws *t0;
        a0(ws *_t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    std::vector<a0*> t0;
    ows(std::vector<a0*> _t0) {
        t0 = _t0;
    }
    static ows* parse();
};

// identifier = alpha , { alpha | digit | "_" } ;
struct identifier {
    struct a0 {
        struct b0 {
            alpha *t0;
            b0(alpha *_t0) {
                t0 = _t0;
            }
            static b0* parse();
        };
        struct b1 {
            digit *t0;
            b1(digit *_t0) {
                t0 = _t0;
            }
            static b1* parse();
        };
        struct b2 {
            std::string t0;
            b2(std::string _t0) {
                t0 = _t0;
            }
            static b2* parse();
        };
        bool is_b0 = false;
        b0 *t0;
        bool is_b1 = false;
        b1 *t1;
        bool is_b2 = false;
        b2 *t2;
        a0(b0 *_t0) {
            is_b0 = true;
            t0 = _t0;
        }
        a0(b1 *_t1) {
            is_b1 = true;
            t1 = _t1;
        }
        a0(b2 *_t2) {
            is_b2 = true;
            t2 = _t2;
        }
        static a0* parse();
    };
    alpha *t0;
    std::vector<a0*> t1;
    identifier(alpha *_t0, std::vector<a0*> _t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static identifier* parse();
};

// type_identifier = "int" | "float" | "void" ;
struct type_identifier {
    struct a0 {
        std::string t0;
        a0(std::string _t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    struct a1 {
        std::string t0;
        a1(std::string _t0) {
            t0 = _t0;
        }
        static a1* parse();
    };
    struct a2 {
        std::string t0;
        a2(std::string _t0) {
            t0 = _t0;
        }
        static a2* parse();
    };
    bool is_a0 = false;
    a0 *t0;
    bool is_a1 = false;
    a1 *t1;
    bool is_a2 = false;
    a2 *t2;
    type_identifier(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    type_identifier(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    type_identifier(a2 *_t2) {
        is_a2 = true;
        t2 = _t2;
    }
    static type_identifier* parse();
};

// expression = identifier | number ;
struct expression {
    struct a0 {
        identifier *t0;
        a0(identifier *_t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    struct a1 {
        number *t0;
        a1(number *_t0) {
            t0 = _t0;
        }
        static a1* parse();
    };
    bool is_a0 = false;
    a0 *t0;
    bool is_a1 = false;
    a1 *t1;
    expression(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    expression(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    static expression* parse();
};

// declaration = type_identifier , rws , identifier , rws , "=" , rws , expression ;
struct declaration {
    type_identifier *t0;
    rws *t1;
    identifier *t2;
    rws *t3;
    std::string t4;
    rws *t5;
    expression *t6;
    declaration(type_identifier *_t0, rws *_t1, identifier *_t2, rws *_t3, std::string _t4, rws *_t5, expression *_t6) {
        t0 = _t0;
        t1 = _t1;
        t2 = _t2;
        t3 = _t3;
        t4 = _t4;
        t5 = _t5;
        t6 = _t6;
    }
    static declaration* parse();
};

// parameter = type_identifier , rws , identifier ;
struct parameter {
    type_identifier *t0;
    rws *t1;
    identifier *t2;
    parameter(type_identifier *_t0, rws *_t1, identifier *_t2) {
        t0 = _t0;
        t1 = _t1;
        t2 = _t2;
    }
    static parameter* parse();
};

// parameter_list = [ parameter , { ows , "," , ows , parameter } ] ;
struct parameter_list {
    struct a0 {
        struct b0 {
            ows *t0;
            std::string t1;
            ows *t2;
            parameter *t3;
            b0(ows *_t0, std::string _t1, ows *_t2, parameter *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static b0* parse();
        };
        parameter *t0;
        std::vector<b0*> t1;
        a0(parameter *_t0, std::vector<b0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static a0* parse();
    };
    a0 *t0;
    parameter_list(a0 *_t0) {
        t0 = _t0;
    }
    static parameter_list* parse();
};

// statement = declaration , ows , ";" ;
struct statement {
    declaration *t0;
    ows *t1;
    std::string t2;
    statement(declaration *_t0, ows *_t1, std::string _t2) {
        t0 = _t0;
        t1 = _t1;
        t2 = _t2;
    }
    static statement* parse();
};

// compound_statement = "{" , ows , { ( statement | compound_statement ) , ows } , "}" ;
struct compound_statement {
    struct a0 {
        struct b0 {
            struct c0 {
                statement *t0;
                c0(statement *_t0) {
                    t0 = _t0;
                }
                static c0* parse();
            };
            struct c1 {
                compound_statement *t0;
                c1(compound_statement *_t0) {
                    t0 = _t0;
                }
                static c1* parse();
            };
            bool is_c0 = false;
            c0 *t0;
            bool is_c1 = false;
            c1 *t1;
            b0(c0 *_t0) {
                is_c0 = true;
                t0 = _t0;
            }
            b0(c1 *_t1) {
                is_c1 = true;
                t1 = _t1;
            }
            static b0* parse();
        };
        b0 *t0;
        ows *t1;
        a0(b0 *_t0, ows *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static a0* parse();
    };
    std::string t0;
    ows *t1;
    std::vector<a0*> t2;
    std::string t3;
    compound_statement(std::string _t0, ows *_t1, std::vector<a0*> _t2, std::string _t3) {
        t0 = _t0;
        t1 = _t1;
        t2 = _t2;
        t3 = _t3;
    }
    static compound_statement* parse();
};

// function_definition = type_identifier , rws , identifier ;
struct function_definition {
    type_identifier *t0;
    rws *t1;
    identifier *t2;
    function_definition(type_identifier *_t0, rws *_t1, identifier *_t2) {
        t0 = _t0;
        t1 = _t1;
        t2 = _t2;
    }
    static function_definition* parse();
};

// function = function_definition , ows , "(" , ows , parameter_list , ows , ")" , ows , compound_statement ;
struct function {
    function_definition *t0;
    ows *t1;
    std::string t2;
    ows *t3;
    parameter_list *t4;
    ows *t5;
    std::string t6;
    ows *t7;
    compound_statement *t8;
    function(function_definition *_t0, ows *_t1, std::string _t2, ows *_t3, parameter_list *_t4, ows *_t5, std::string _t6, ows *_t7, compound_statement *_t8) {
        t0 = _t0;
        t1 = _t1;
        t2 = _t2;
        t3 = _t3;
        t4 = _t4;
        t5 = _t5;
        t6 = _t6;
        t7 = _t7;
        t8 = _t8;
    }
    static function* parse();
};

// program = { ows , function } , ows ;
struct program {
    struct a0 {
        ows *t0;
        function *t1;
        a0(ows *_t0, function *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static a0* parse();
    };
    std::vector<a0*> t0;
    ows *t1;
    program(std::vector<a0*> _t0, ows *_t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static program* parse();
};

alpha::a0* alpha::a0::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "A") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a0(_t0);
}

alpha::a1* alpha::a1::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "B") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a1(_t0);
}

alpha::a2* alpha::a2::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "C") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a2(_t0);
}

alpha::a3* alpha::a3::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "D") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a3(_t0);
}

alpha::a4* alpha::a4::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "E") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a4(_t0);
}

alpha::a5* alpha::a5::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "F") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a5(_t0);
}

alpha::a6* alpha::a6::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "G") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a6(_t0);
}

alpha::a7* alpha::a7::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "H") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a7(_t0);
}

alpha::a8* alpha::a8::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "I") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a8(_t0);
}

alpha::a9* alpha::a9::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "J") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a9(_t0);
}

alpha::a10* alpha::a10::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "K") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a10(_t0);
}

alpha::a11* alpha::a11::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "L") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a11(_t0);
}

alpha::a12* alpha::a12::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "M") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a12(_t0);
}

alpha::a13* alpha::a13::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "N") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a13(_t0);
}

alpha::a14* alpha::a14::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "O") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a14(_t0);
}

alpha::a15* alpha::a15::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "P") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a15(_t0);
}

alpha::a16* alpha::a16::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "Q") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a16(_t0);
}

alpha::a17* alpha::a17::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "R") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a17(_t0);
}

alpha::a18* alpha::a18::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "S") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a18(_t0);
}

alpha::a19* alpha::a19::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "T") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a19(_t0);
}

alpha::a20* alpha::a20::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "U") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a20(_t0);
}

alpha::a21* alpha::a21::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "V") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a21(_t0);
}

alpha::a22* alpha::a22::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "W") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a22(_t0);
}

alpha::a23* alpha::a23::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "X") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a23(_t0);
}

alpha::a24* alpha::a24::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "Y") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a24(_t0);
}

alpha::a25* alpha::a25::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "Z") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a25(_t0);
}

alpha::a26* alpha::a26::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "a") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a26(_t0);
}

alpha::a27* alpha::a27::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "b") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a27(_t0);
}

alpha::a28* alpha::a28::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "c") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a28(_t0);
}

alpha::a29* alpha::a29::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "d") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a29(_t0);
}

alpha::a30* alpha::a30::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "e") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a30(_t0);
}

alpha::a31* alpha::a31::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "f") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a31(_t0);
}

alpha::a32* alpha::a32::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "g") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a32(_t0);
}

alpha::a33* alpha::a33::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "h") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a33(_t0);
}

alpha::a34* alpha::a34::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "i") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a34(_t0);
}

alpha::a35* alpha::a35::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "j") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a35(_t0);
}

alpha::a36* alpha::a36::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "k") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a36(_t0);
}

alpha::a37* alpha::a37::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "l") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a37(_t0);
}

alpha::a38* alpha::a38::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "m") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a38(_t0);
}

alpha::a39* alpha::a39::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "n") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a39(_t0);
}

alpha::a40* alpha::a40::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "o") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a40(_t0);
}

alpha::a41* alpha::a41::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "p") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a41(_t0);
}

alpha::a42* alpha::a42::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "q") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a42(_t0);
}

alpha::a43* alpha::a43::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "r") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a43(_t0);
}

alpha::a44* alpha::a44::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "s") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a44(_t0);
}

alpha::a45* alpha::a45::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "t") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a45(_t0);
}

alpha::a46* alpha::a46::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "u") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a46(_t0);
}

alpha::a47* alpha::a47::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "v") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a47(_t0);
}

alpha::a48* alpha::a48::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "w") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a48(_t0);
}

alpha::a49* alpha::a49::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "x") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a49(_t0);
}

alpha::a50* alpha::a50::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "y") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a50(_t0);
}

alpha::a51* alpha::a51::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "z") {pop_stack(); return nullptr;}
    rm_stack();
    return new alpha::a51(_t0);
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

digit::a0* digit::a0::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "0") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a0(_t0);
}

digit::a1* digit::a1::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "1") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a1(_t0);
}

digit::a2* digit::a2::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "2") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a2(_t0);
}

digit::a3* digit::a3::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "3") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a3(_t0);
}

digit::a4* digit::a4::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "4") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a4(_t0);
}

digit::a5* digit::a5::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "5") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a5(_t0);
}

digit::a6* digit::a6::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "6") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a6(_t0);
}

digit::a7* digit::a7::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "7") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a7(_t0);
}

digit::a8* digit::a8::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "8") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a8(_t0);
}

digit::a9* digit::a9::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "9") {pop_stack(); return nullptr;}
    rm_stack();
    return new digit::a9(_t0);
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

number::a0* number::a0::parse() {
    push_stack();
    digit *_t0 = digit::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new number::a0(_t0);
}

number* number::parse() {
    push_stack();
    digit *_t0 = digit::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    std::vector<number::a0*> _t1;
    while(true) {
        number::a0 *tmp = number::a0::parse();
        if(tmp == nullptr) break;
        _t1.push_back(tmp);
    }
    rm_stack();
    return new number(_t0, _t1);
}

ws::a0* ws::a0::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != " ") {pop_stack(); return nullptr;}
    rm_stack();
    return new ws::a0(_t0);
}

ws::a1* ws::a1::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\n") {pop_stack(); return nullptr;}
    rm_stack();
    return new ws::a1(_t0);
}

ws::a2* ws::a2::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\t") {pop_stack(); return nullptr;}
    rm_stack();
    return new ws::a2(_t0);
}

ws::a3* ws::a3::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\r") {pop_stack(); return nullptr;}
    rm_stack();
    return new ws::a3(_t0);
}

ws::a4* ws::a4::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\f") {pop_stack(); return nullptr;}
    rm_stack();
    return new ws::a4(_t0);
}

ws::a5* ws::a5::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\b") {pop_stack(); return nullptr;}
    rm_stack();
    return new ws::a5(_t0);
}

ws* ws::parse() {
    if(auto x = ws::a0::parse()) return new ws(x);
    if(auto x = ws::a1::parse()) return new ws(x);
    if(auto x = ws::a2::parse()) return new ws(x);
    if(auto x = ws::a3::parse()) return new ws(x);
    if(auto x = ws::a4::parse()) return new ws(x);
    if(auto x = ws::a5::parse()) return new ws(x);
    return nullptr;
}

rws::a0* rws::a0::parse() {
    push_stack();
    ws *_t0 = ws::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new rws::a0(_t0);
}

rws* rws::parse() {
    push_stack();
    ws *_t0 = ws::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    std::vector<rws::a0*> _t1;
    while(true) {
        rws::a0 *tmp = rws::a0::parse();
        if(tmp == nullptr) break;
        _t1.push_back(tmp);
    }
    rm_stack();
    return new rws(_t0, _t1);
}

ows::a0* ows::a0::parse() {
    push_stack();
    ws *_t0 = ws::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new ows::a0(_t0);
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

identifier::a0::b0* identifier::a0::b0::parse() {
    push_stack();
    alpha *_t0 = alpha::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new identifier::a0::b0(_t0);
}

identifier::a0::b1* identifier::a0::b1::parse() {
    push_stack();
    digit *_t0 = digit::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new identifier::a0::b1(_t0);
}

identifier::a0::b2* identifier::a0::b2::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "_") {pop_stack(); return nullptr;}
    rm_stack();
    return new identifier::a0::b2(_t0);
}

identifier::a0* identifier::a0::parse() {
    if(auto x = identifier::a0::b0::parse()) return new identifier::a0(x);
    if(auto x = identifier::a0::b1::parse()) return new identifier::a0(x);
    if(auto x = identifier::a0::b2::parse()) return new identifier::a0(x);
    return nullptr;
}

identifier* identifier::parse() {
    push_stack();
    alpha *_t0 = alpha::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    std::vector<identifier::a0*> _t1;
    while(true) {
        identifier::a0 *tmp = identifier::a0::parse();
        if(tmp == nullptr) break;
        _t1.push_back(tmp);
    }
    rm_stack();
    return new identifier(_t0, _t1);
}

type_identifier::a0* type_identifier::a0::parse() {
    push_stack();
    std::string _t0 = next_chars(3);
    if(_t0 != "int") {pop_stack(); return nullptr;}
    rm_stack();
    return new type_identifier::a0(_t0);
}

type_identifier::a1* type_identifier::a1::parse() {
    push_stack();
    std::string _t0 = next_chars(5);
    if(_t0 != "float") {pop_stack(); return nullptr;}
    rm_stack();
    return new type_identifier::a1(_t0);
}

type_identifier::a2* type_identifier::a2::parse() {
    push_stack();
    std::string _t0 = next_chars(4);
    if(_t0 != "void") {pop_stack(); return nullptr;}
    rm_stack();
    return new type_identifier::a2(_t0);
}

type_identifier* type_identifier::parse() {
    if(auto x = type_identifier::a0::parse()) return new type_identifier(x);
    if(auto x = type_identifier::a1::parse()) return new type_identifier(x);
    if(auto x = type_identifier::a2::parse()) return new type_identifier(x);
    return nullptr;
}

expression::a0* expression::a0::parse() {
    push_stack();
    identifier *_t0 = identifier::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new expression::a0(_t0);
}

expression::a1* expression::a1::parse() {
    push_stack();
    number *_t0 = number::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new expression::a1(_t0);
}

expression* expression::parse() {
    if(auto x = expression::a0::parse()) return new expression(x);
    if(auto x = expression::a1::parse()) return new expression(x);
    return nullptr;
}

declaration* declaration::parse() {
    push_stack();
    type_identifier *_t0 = type_identifier::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rws *_t1 = rws::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    identifier *_t2 = identifier::parse();
    if(_t2 == nullptr) {pop_stack(); return nullptr;}
    rws *_t3 = rws::parse();
    if(_t3 == nullptr) {pop_stack(); return nullptr;}
    std::string _t4 = next_chars(1);
    if(_t4 != "=") {pop_stack(); return nullptr;}
    rws *_t5 = rws::parse();
    if(_t5 == nullptr) {pop_stack(); return nullptr;}
    expression *_t6 = expression::parse();
    if(_t6 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new declaration(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
}

parameter* parameter::parse() {
    push_stack();
    type_identifier *_t0 = type_identifier::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rws *_t1 = rws::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    identifier *_t2 = identifier::parse();
    if(_t2 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new parameter(_t0, _t1, _t2);
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

parameter_list* parameter_list::parse() {
    push_stack();
    parameter_list::a0 *_t0 = parameter_list::a0::parse();
    rm_stack();
    return new parameter_list(_t0);
}

statement* statement::parse() {
    push_stack();
    declaration *_t0 = declaration::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    ows *_t1 = ows::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    std::string _t2 = next_chars(1);
    if(_t2 != ";") {pop_stack(); return nullptr;}
    rm_stack();
    return new statement(_t0, _t1, _t2);
}

compound_statement::a0::b0::c0* compound_statement::a0::b0::c0::parse() {
    push_stack();
    statement *_t0 = statement::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new compound_statement::a0::b0::c0(_t0);
}

compound_statement::a0::b0::c1* compound_statement::a0::b0::c1::parse() {
    push_stack();
    compound_statement *_t0 = compound_statement::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new compound_statement::a0::b0::c1(_t0);
}

compound_statement::a0::b0* compound_statement::a0::b0::parse() {
    if(auto x = compound_statement::a0::b0::c0::parse()) return new compound_statement::a0::b0(x);
    if(auto x = compound_statement::a0::b0::c1::parse()) return new compound_statement::a0::b0(x);
    return nullptr;
}

compound_statement::a0* compound_statement::a0::parse() {
    push_stack();
    compound_statement::a0::b0 *_t0 = compound_statement::a0::b0::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    ows *_t1 = ows::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new compound_statement::a0(_t0, _t1);
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

function_definition* function_definition::parse() {
    push_stack();
    type_identifier *_t0 = type_identifier::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rws *_t1 = rws::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    identifier *_t2 = identifier::parse();
    if(_t2 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new function_definition(_t0, _t1, _t2);
}

function* function::parse() {
    push_stack();
    function_definition *_t0 = function_definition::parse();
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
    ows *_t7 = ows::parse();
    if(_t7 == nullptr) {pop_stack(); return nullptr;}
    compound_statement *_t8 = compound_statement::parse();
    if(_t8 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new function(_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
}

program::a0* program::a0::parse() {
    push_stack();
    ows *_t0 = ows::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    function *_t1 = function::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new program::a0(_t0, _t1);
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
    program *x = program::parse();
    assert(ptr_stack.size() == 0);
    if(x == nullptr) {std::cout << "FAILED\n"; return 0;}
    std::cout << "SUCCESS\n";
    std::cout << x->t0.size() << "\n";
    return 0;
}