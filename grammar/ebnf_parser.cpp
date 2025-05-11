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

struct letter;
struct digit;
struct symbol;
struct escape;
struct identifier;
struct comment_char;
struct comment;
struct ws;
struct rws;
struct ows;
struct terminal_char;
struct terminal;
struct term;
struct concatenation;
struct alternation;
struct rule;
struct grammar;

// letter = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" | "_" ;
struct letter {
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
    struct a52 {
        std::string t0;
        a52(std::string _t0) {
            t0 = _t0;
        }
        static a52* parse();
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
    bool is_a52 = false;
    a52 *t52;
    letter(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    letter(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    letter(a2 *_t2) {
        is_a2 = true;
        t2 = _t2;
    }
    letter(a3 *_t3) {
        is_a3 = true;
        t3 = _t3;
    }
    letter(a4 *_t4) {
        is_a4 = true;
        t4 = _t4;
    }
    letter(a5 *_t5) {
        is_a5 = true;
        t5 = _t5;
    }
    letter(a6 *_t6) {
        is_a6 = true;
        t6 = _t6;
    }
    letter(a7 *_t7) {
        is_a7 = true;
        t7 = _t7;
    }
    letter(a8 *_t8) {
        is_a8 = true;
        t8 = _t8;
    }
    letter(a9 *_t9) {
        is_a9 = true;
        t9 = _t9;
    }
    letter(a10 *_t10) {
        is_a10 = true;
        t10 = _t10;
    }
    letter(a11 *_t11) {
        is_a11 = true;
        t11 = _t11;
    }
    letter(a12 *_t12) {
        is_a12 = true;
        t12 = _t12;
    }
    letter(a13 *_t13) {
        is_a13 = true;
        t13 = _t13;
    }
    letter(a14 *_t14) {
        is_a14 = true;
        t14 = _t14;
    }
    letter(a15 *_t15) {
        is_a15 = true;
        t15 = _t15;
    }
    letter(a16 *_t16) {
        is_a16 = true;
        t16 = _t16;
    }
    letter(a17 *_t17) {
        is_a17 = true;
        t17 = _t17;
    }
    letter(a18 *_t18) {
        is_a18 = true;
        t18 = _t18;
    }
    letter(a19 *_t19) {
        is_a19 = true;
        t19 = _t19;
    }
    letter(a20 *_t20) {
        is_a20 = true;
        t20 = _t20;
    }
    letter(a21 *_t21) {
        is_a21 = true;
        t21 = _t21;
    }
    letter(a22 *_t22) {
        is_a22 = true;
        t22 = _t22;
    }
    letter(a23 *_t23) {
        is_a23 = true;
        t23 = _t23;
    }
    letter(a24 *_t24) {
        is_a24 = true;
        t24 = _t24;
    }
    letter(a25 *_t25) {
        is_a25 = true;
        t25 = _t25;
    }
    letter(a26 *_t26) {
        is_a26 = true;
        t26 = _t26;
    }
    letter(a27 *_t27) {
        is_a27 = true;
        t27 = _t27;
    }
    letter(a28 *_t28) {
        is_a28 = true;
        t28 = _t28;
    }
    letter(a29 *_t29) {
        is_a29 = true;
        t29 = _t29;
    }
    letter(a30 *_t30) {
        is_a30 = true;
        t30 = _t30;
    }
    letter(a31 *_t31) {
        is_a31 = true;
        t31 = _t31;
    }
    letter(a32 *_t32) {
        is_a32 = true;
        t32 = _t32;
    }
    letter(a33 *_t33) {
        is_a33 = true;
        t33 = _t33;
    }
    letter(a34 *_t34) {
        is_a34 = true;
        t34 = _t34;
    }
    letter(a35 *_t35) {
        is_a35 = true;
        t35 = _t35;
    }
    letter(a36 *_t36) {
        is_a36 = true;
        t36 = _t36;
    }
    letter(a37 *_t37) {
        is_a37 = true;
        t37 = _t37;
    }
    letter(a38 *_t38) {
        is_a38 = true;
        t38 = _t38;
    }
    letter(a39 *_t39) {
        is_a39 = true;
        t39 = _t39;
    }
    letter(a40 *_t40) {
        is_a40 = true;
        t40 = _t40;
    }
    letter(a41 *_t41) {
        is_a41 = true;
        t41 = _t41;
    }
    letter(a42 *_t42) {
        is_a42 = true;
        t42 = _t42;
    }
    letter(a43 *_t43) {
        is_a43 = true;
        t43 = _t43;
    }
    letter(a44 *_t44) {
        is_a44 = true;
        t44 = _t44;
    }
    letter(a45 *_t45) {
        is_a45 = true;
        t45 = _t45;
    }
    letter(a46 *_t46) {
        is_a46 = true;
        t46 = _t46;
    }
    letter(a47 *_t47) {
        is_a47 = true;
        t47 = _t47;
    }
    letter(a48 *_t48) {
        is_a48 = true;
        t48 = _t48;
    }
    letter(a49 *_t49) {
        is_a49 = true;
        t49 = _t49;
    }
    letter(a50 *_t50) {
        is_a50 = true;
        t50 = _t50;
    }
    letter(a51 *_t51) {
        is_a51 = true;
        t51 = _t51;
    }
    letter(a52 *_t52) {
        is_a52 = true;
        t52 = _t52;
    }
    static letter* parse();
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

// symbol = "[" | "]" | "{" | "}" | "(" | ")" | "<" | ">" | "'" | "=" | "|" | "." | "," | ";" | "-" | "+" | "*" | "?" | ":" | "!" | "@" | "#" | "$" | "%" | "^" | "&" | "/" | "~" | "`" ;
struct symbol {
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
    symbol(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    symbol(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    symbol(a2 *_t2) {
        is_a2 = true;
        t2 = _t2;
    }
    symbol(a3 *_t3) {
        is_a3 = true;
        t3 = _t3;
    }
    symbol(a4 *_t4) {
        is_a4 = true;
        t4 = _t4;
    }
    symbol(a5 *_t5) {
        is_a5 = true;
        t5 = _t5;
    }
    symbol(a6 *_t6) {
        is_a6 = true;
        t6 = _t6;
    }
    symbol(a7 *_t7) {
        is_a7 = true;
        t7 = _t7;
    }
    symbol(a8 *_t8) {
        is_a8 = true;
        t8 = _t8;
    }
    symbol(a9 *_t9) {
        is_a9 = true;
        t9 = _t9;
    }
    symbol(a10 *_t10) {
        is_a10 = true;
        t10 = _t10;
    }
    symbol(a11 *_t11) {
        is_a11 = true;
        t11 = _t11;
    }
    symbol(a12 *_t12) {
        is_a12 = true;
        t12 = _t12;
    }
    symbol(a13 *_t13) {
        is_a13 = true;
        t13 = _t13;
    }
    symbol(a14 *_t14) {
        is_a14 = true;
        t14 = _t14;
    }
    symbol(a15 *_t15) {
        is_a15 = true;
        t15 = _t15;
    }
    symbol(a16 *_t16) {
        is_a16 = true;
        t16 = _t16;
    }
    symbol(a17 *_t17) {
        is_a17 = true;
        t17 = _t17;
    }
    symbol(a18 *_t18) {
        is_a18 = true;
        t18 = _t18;
    }
    symbol(a19 *_t19) {
        is_a19 = true;
        t19 = _t19;
    }
    symbol(a20 *_t20) {
        is_a20 = true;
        t20 = _t20;
    }
    symbol(a21 *_t21) {
        is_a21 = true;
        t21 = _t21;
    }
    symbol(a22 *_t22) {
        is_a22 = true;
        t22 = _t22;
    }
    symbol(a23 *_t23) {
        is_a23 = true;
        t23 = _t23;
    }
    symbol(a24 *_t24) {
        is_a24 = true;
        t24 = _t24;
    }
    symbol(a25 *_t25) {
        is_a25 = true;
        t25 = _t25;
    }
    symbol(a26 *_t26) {
        is_a26 = true;
        t26 = _t26;
    }
    symbol(a27 *_t27) {
        is_a27 = true;
        t27 = _t27;
    }
    symbol(a28 *_t28) {
        is_a28 = true;
        t28 = _t28;
    }
    static symbol* parse();
};

// escape = "\\" , ( "n" | "t" | "r" | "f" | "b" | "\"" | "\\" ) ;
struct escape {
    struct a0 {
        struct b0 {
            std::string t0;
            b0(std::string _t0) {
                t0 = _t0;
            }
            static b0* parse();
        };
        struct b1 {
            std::string t0;
            b1(std::string _t0) {
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
        struct b3 {
            std::string t0;
            b3(std::string _t0) {
                t0 = _t0;
            }
            static b3* parse();
        };
        struct b4 {
            std::string t0;
            b4(std::string _t0) {
                t0 = _t0;
            }
            static b4* parse();
        };
        struct b5 {
            std::string t0;
            b5(std::string _t0) {
                t0 = _t0;
            }
            static b5* parse();
        };
        struct b6 {
            std::string t0;
            b6(std::string _t0) {
                t0 = _t0;
            }
            static b6* parse();
        };
        bool is_b0 = false;
        b0 *t0;
        bool is_b1 = false;
        b1 *t1;
        bool is_b2 = false;
        b2 *t2;
        bool is_b3 = false;
        b3 *t3;
        bool is_b4 = false;
        b4 *t4;
        bool is_b5 = false;
        b5 *t5;
        bool is_b6 = false;
        b6 *t6;
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
        a0(b3 *_t3) {
            is_b3 = true;
            t3 = _t3;
        }
        a0(b4 *_t4) {
            is_b4 = true;
            t4 = _t4;
        }
        a0(b5 *_t5) {
            is_b5 = true;
            t5 = _t5;
        }
        a0(b6 *_t6) {
            is_b6 = true;
            t6 = _t6;
        }
        static a0* parse();
    };
    std::string t0;
    a0 *t1;
    escape(std::string _t0, a0 *_t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static escape* parse();
};

// identifier = letter , { letter } ;
struct identifier {
    struct a0 {
        letter *t0;
        a0(letter *_t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    letter *t0;
    std::vector<a0*> t1;
    identifier(letter *_t0, std::vector<a0*> _t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static identifier* parse();
};

// comment_char = letter | digit | ws | "[" | "]" | "{" | "}" | "(" | ")" | "<" | ">" | "'" | "=" | "|" | "." | "," | ";" | "-" | "+" | "?" | ":" | "!" | "@" | "#" | "$" | "%" | "^" | "&" | "/" | "~" | "`" | "\\" | "\"" ;
struct comment_char {
    struct a0 {
        letter *t0;
        a0(letter *_t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    struct a1 {
        digit *t0;
        a1(digit *_t0) {
            t0 = _t0;
        }
        static a1* parse();
    };
    struct a2 {
        ws *t0;
        a2(ws *_t0) {
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
    comment_char(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    comment_char(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    comment_char(a2 *_t2) {
        is_a2 = true;
        t2 = _t2;
    }
    comment_char(a3 *_t3) {
        is_a3 = true;
        t3 = _t3;
    }
    comment_char(a4 *_t4) {
        is_a4 = true;
        t4 = _t4;
    }
    comment_char(a5 *_t5) {
        is_a5 = true;
        t5 = _t5;
    }
    comment_char(a6 *_t6) {
        is_a6 = true;
        t6 = _t6;
    }
    comment_char(a7 *_t7) {
        is_a7 = true;
        t7 = _t7;
    }
    comment_char(a8 *_t8) {
        is_a8 = true;
        t8 = _t8;
    }
    comment_char(a9 *_t9) {
        is_a9 = true;
        t9 = _t9;
    }
    comment_char(a10 *_t10) {
        is_a10 = true;
        t10 = _t10;
    }
    comment_char(a11 *_t11) {
        is_a11 = true;
        t11 = _t11;
    }
    comment_char(a12 *_t12) {
        is_a12 = true;
        t12 = _t12;
    }
    comment_char(a13 *_t13) {
        is_a13 = true;
        t13 = _t13;
    }
    comment_char(a14 *_t14) {
        is_a14 = true;
        t14 = _t14;
    }
    comment_char(a15 *_t15) {
        is_a15 = true;
        t15 = _t15;
    }
    comment_char(a16 *_t16) {
        is_a16 = true;
        t16 = _t16;
    }
    comment_char(a17 *_t17) {
        is_a17 = true;
        t17 = _t17;
    }
    comment_char(a18 *_t18) {
        is_a18 = true;
        t18 = _t18;
    }
    comment_char(a19 *_t19) {
        is_a19 = true;
        t19 = _t19;
    }
    comment_char(a20 *_t20) {
        is_a20 = true;
        t20 = _t20;
    }
    comment_char(a21 *_t21) {
        is_a21 = true;
        t21 = _t21;
    }
    comment_char(a22 *_t22) {
        is_a22 = true;
        t22 = _t22;
    }
    comment_char(a23 *_t23) {
        is_a23 = true;
        t23 = _t23;
    }
    comment_char(a24 *_t24) {
        is_a24 = true;
        t24 = _t24;
    }
    comment_char(a25 *_t25) {
        is_a25 = true;
        t25 = _t25;
    }
    comment_char(a26 *_t26) {
        is_a26 = true;
        t26 = _t26;
    }
    comment_char(a27 *_t27) {
        is_a27 = true;
        t27 = _t27;
    }
    comment_char(a28 *_t28) {
        is_a28 = true;
        t28 = _t28;
    }
    comment_char(a29 *_t29) {
        is_a29 = true;
        t29 = _t29;
    }
    comment_char(a30 *_t30) {
        is_a30 = true;
        t30 = _t30;
    }
    comment_char(a31 *_t31) {
        is_a31 = true;
        t31 = _t31;
    }
    comment_char(a32 *_t32) {
        is_a32 = true;
        t32 = _t32;
    }
    static comment_char* parse();
};

// comment = "(*" , { comment_char } , "*)" ;
struct comment {
    struct a0 {
        comment_char *t0;
        a0(comment_char *_t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    std::string t0;
    std::vector<a0*> t1;
    std::string t2;
    comment(std::string _t0, std::vector<a0*> _t1, std::string _t2) {
        t0 = _t0;
        t1 = _t1;
        t2 = _t2;
    }
    static comment* parse();
};

// ws = " " | "\n" | "\t" | "\r" | "\f" | "\b" | comment ;
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
    struct a6 {
        comment *t0;
        a6(comment *_t0) {
            t0 = _t0;
        }
        static a6* parse();
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
    ws(a6 *_t6) {
        is_a6 = true;
        t6 = _t6;
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

// terminal_char = letter | digit | symbol | escape | " " ;
struct terminal_char {
    struct a0 {
        letter *t0;
        a0(letter *_t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    struct a1 {
        digit *t0;
        a1(digit *_t0) {
            t0 = _t0;
        }
        static a1* parse();
    };
    struct a2 {
        symbol *t0;
        a2(symbol *_t0) {
            t0 = _t0;
        }
        static a2* parse();
    };
    struct a3 {
        escape *t0;
        a3(escape *_t0) {
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
    terminal_char(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    terminal_char(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    terminal_char(a2 *_t2) {
        is_a2 = true;
        t2 = _t2;
    }
    terminal_char(a3 *_t3) {
        is_a3 = true;
        t3 = _t3;
    }
    terminal_char(a4 *_t4) {
        is_a4 = true;
        t4 = _t4;
    }
    static terminal_char* parse();
};

// terminal = "\"" , terminal_char , { terminal_char } , "\"" ;
struct terminal {
    struct a0 {
        terminal_char *t0;
        a0(terminal_char *_t0) {
            t0 = _t0;
        }
        static a0* parse();
    };
    std::string t0;
    terminal_char *t1;
    std::vector<a0*> t2;
    std::string t3;
    terminal(std::string _t0, terminal_char *_t1, std::vector<a0*> _t2, std::string _t3) {
        t0 = _t0;
        t1 = _t1;
        t2 = _t2;
        t3 = _t3;
    }
    static terminal* parse();
};

// term = "(" , rws , alternation , rws , ")" | "[" , rws , alternation , rws , "]" | "{" , rws , alternation , rws , "}" | terminal | identifier ;
struct term {
    struct a0 {
        std::string t0;
        rws *t1;
        alternation *t2;
        rws *t3;
        std::string t4;
        a0(std::string _t0, rws *_t1, alternation *_t2, rws *_t3, std::string _t4) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
        }
        static a0* parse();
    };
    struct a1 {
        std::string t0;
        rws *t1;
        alternation *t2;
        rws *t3;
        std::string t4;
        a1(std::string _t0, rws *_t1, alternation *_t2, rws *_t3, std::string _t4) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
        }
        static a1* parse();
    };
    struct a2 {
        std::string t0;
        rws *t1;
        alternation *t2;
        rws *t3;
        std::string t4;
        a2(std::string _t0, rws *_t1, alternation *_t2, rws *_t3, std::string _t4) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
        }
        static a2* parse();
    };
    struct a3 {
        terminal *t0;
        a3(terminal *_t0) {
            t0 = _t0;
        }
        static a3* parse();
    };
    struct a4 {
        identifier *t0;
        a4(identifier *_t0) {
            t0 = _t0;
        }
        static a4* parse();
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
    term(a0 *_t0) {
        is_a0 = true;
        t0 = _t0;
    }
    term(a1 *_t1) {
        is_a1 = true;
        t1 = _t1;
    }
    term(a2 *_t2) {
        is_a2 = true;
        t2 = _t2;
    }
    term(a3 *_t3) {
        is_a3 = true;
        t3 = _t3;
    }
    term(a4 *_t4) {
        is_a4 = true;
        t4 = _t4;
    }
    static term* parse();
};

// concatenation = term , { rws , "," , rws , term } ;
struct concatenation {
    struct a0 {
        rws *t0;
        std::string t1;
        rws *t2;
        term *t3;
        a0(rws *_t0, std::string _t1, rws *_t2, term *_t3) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
        }
        static a0* parse();
    };
    term *t0;
    std::vector<a0*> t1;
    concatenation(term *_t0, std::vector<a0*> _t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static concatenation* parse();
};

// alternation = concatenation , { rws , "|" , rws , concatenation } ;
struct alternation {
    struct a0 {
        rws *t0;
        std::string t1;
        rws *t2;
        concatenation *t3;
        a0(rws *_t0, std::string _t1, rws *_t2, concatenation *_t3) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
        }
        static a0* parse();
    };
    concatenation *t0;
    std::vector<a0*> t1;
    alternation(concatenation *_t0, std::vector<a0*> _t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static alternation* parse();
};

// rule = identifier , rws , "=" , rws , alternation , rws , ";" ;
struct rule {
    identifier *t0;
    rws *t1;
    std::string t2;
    rws *t3;
    alternation *t4;
    rws *t5;
    std::string t6;
    rule(identifier *_t0, rws *_t1, std::string _t2, rws *_t3, alternation *_t4, rws *_t5, std::string _t6) {
        t0 = _t0;
        t1 = _t1;
        t2 = _t2;
        t3 = _t3;
        t4 = _t4;
        t5 = _t5;
        t6 = _t6;
    }
    static rule* parse();
};

// grammar = { ows , rule } , ows ;
struct grammar {
    struct a0 {
        ows *t0;
        rule *t1;
        a0(ows *_t0, rule *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static a0* parse();
    };
    std::vector<a0*> t0;
    ows *t1;
    grammar(std::vector<a0*> _t0, ows *_t1) {
        t0 = _t0;
        t1 = _t1;
    }
    static grammar* parse();
};

letter::a0* letter::a0::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "A") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a0(_t0);
}

letter::a1* letter::a1::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "B") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a1(_t0);
}

letter::a2* letter::a2::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "C") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a2(_t0);
}

letter::a3* letter::a3::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "D") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a3(_t0);
}

letter::a4* letter::a4::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "E") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a4(_t0);
}

letter::a5* letter::a5::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "F") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a5(_t0);
}

letter::a6* letter::a6::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "G") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a6(_t0);
}

letter::a7* letter::a7::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "H") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a7(_t0);
}

letter::a8* letter::a8::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "I") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a8(_t0);
}

letter::a9* letter::a9::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "J") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a9(_t0);
}

letter::a10* letter::a10::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "K") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a10(_t0);
}

letter::a11* letter::a11::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "L") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a11(_t0);
}

letter::a12* letter::a12::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "M") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a12(_t0);
}

letter::a13* letter::a13::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "N") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a13(_t0);
}

letter::a14* letter::a14::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "O") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a14(_t0);
}

letter::a15* letter::a15::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "P") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a15(_t0);
}

letter::a16* letter::a16::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "Q") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a16(_t0);
}

letter::a17* letter::a17::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "R") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a17(_t0);
}

letter::a18* letter::a18::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "S") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a18(_t0);
}

letter::a19* letter::a19::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "T") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a19(_t0);
}

letter::a20* letter::a20::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "U") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a20(_t0);
}

letter::a21* letter::a21::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "V") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a21(_t0);
}

letter::a22* letter::a22::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "W") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a22(_t0);
}

letter::a23* letter::a23::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "X") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a23(_t0);
}

letter::a24* letter::a24::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "Y") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a24(_t0);
}

letter::a25* letter::a25::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "Z") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a25(_t0);
}

letter::a26* letter::a26::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "a") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a26(_t0);
}

letter::a27* letter::a27::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "b") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a27(_t0);
}

letter::a28* letter::a28::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "c") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a28(_t0);
}

letter::a29* letter::a29::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "d") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a29(_t0);
}

letter::a30* letter::a30::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "e") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a30(_t0);
}

letter::a31* letter::a31::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "f") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a31(_t0);
}

letter::a32* letter::a32::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "g") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a32(_t0);
}

letter::a33* letter::a33::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "h") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a33(_t0);
}

letter::a34* letter::a34::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "i") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a34(_t0);
}

letter::a35* letter::a35::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "j") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a35(_t0);
}

letter::a36* letter::a36::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "k") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a36(_t0);
}

letter::a37* letter::a37::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "l") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a37(_t0);
}

letter::a38* letter::a38::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "m") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a38(_t0);
}

letter::a39* letter::a39::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "n") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a39(_t0);
}

letter::a40* letter::a40::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "o") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a40(_t0);
}

letter::a41* letter::a41::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "p") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a41(_t0);
}

letter::a42* letter::a42::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "q") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a42(_t0);
}

letter::a43* letter::a43::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "r") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a43(_t0);
}

letter::a44* letter::a44::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "s") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a44(_t0);
}

letter::a45* letter::a45::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "t") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a45(_t0);
}

letter::a46* letter::a46::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "u") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a46(_t0);
}

letter::a47* letter::a47::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "v") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a47(_t0);
}

letter::a48* letter::a48::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "w") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a48(_t0);
}

letter::a49* letter::a49::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "x") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a49(_t0);
}

letter::a50* letter::a50::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "y") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a50(_t0);
}

letter::a51* letter::a51::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "z") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a51(_t0);
}

letter::a52* letter::a52::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "_") {pop_stack(); return nullptr;}
    rm_stack();
    return new letter::a52(_t0);
}

letter* letter::parse() {
    if(auto x = letter::a0::parse()) return new letter(x);
    if(auto x = letter::a1::parse()) return new letter(x);
    if(auto x = letter::a2::parse()) return new letter(x);
    if(auto x = letter::a3::parse()) return new letter(x);
    if(auto x = letter::a4::parse()) return new letter(x);
    if(auto x = letter::a5::parse()) return new letter(x);
    if(auto x = letter::a6::parse()) return new letter(x);
    if(auto x = letter::a7::parse()) return new letter(x);
    if(auto x = letter::a8::parse()) return new letter(x);
    if(auto x = letter::a9::parse()) return new letter(x);
    if(auto x = letter::a10::parse()) return new letter(x);
    if(auto x = letter::a11::parse()) return new letter(x);
    if(auto x = letter::a12::parse()) return new letter(x);
    if(auto x = letter::a13::parse()) return new letter(x);
    if(auto x = letter::a14::parse()) return new letter(x);
    if(auto x = letter::a15::parse()) return new letter(x);
    if(auto x = letter::a16::parse()) return new letter(x);
    if(auto x = letter::a17::parse()) return new letter(x);
    if(auto x = letter::a18::parse()) return new letter(x);
    if(auto x = letter::a19::parse()) return new letter(x);
    if(auto x = letter::a20::parse()) return new letter(x);
    if(auto x = letter::a21::parse()) return new letter(x);
    if(auto x = letter::a22::parse()) return new letter(x);
    if(auto x = letter::a23::parse()) return new letter(x);
    if(auto x = letter::a24::parse()) return new letter(x);
    if(auto x = letter::a25::parse()) return new letter(x);
    if(auto x = letter::a26::parse()) return new letter(x);
    if(auto x = letter::a27::parse()) return new letter(x);
    if(auto x = letter::a28::parse()) return new letter(x);
    if(auto x = letter::a29::parse()) return new letter(x);
    if(auto x = letter::a30::parse()) return new letter(x);
    if(auto x = letter::a31::parse()) return new letter(x);
    if(auto x = letter::a32::parse()) return new letter(x);
    if(auto x = letter::a33::parse()) return new letter(x);
    if(auto x = letter::a34::parse()) return new letter(x);
    if(auto x = letter::a35::parse()) return new letter(x);
    if(auto x = letter::a36::parse()) return new letter(x);
    if(auto x = letter::a37::parse()) return new letter(x);
    if(auto x = letter::a38::parse()) return new letter(x);
    if(auto x = letter::a39::parse()) return new letter(x);
    if(auto x = letter::a40::parse()) return new letter(x);
    if(auto x = letter::a41::parse()) return new letter(x);
    if(auto x = letter::a42::parse()) return new letter(x);
    if(auto x = letter::a43::parse()) return new letter(x);
    if(auto x = letter::a44::parse()) return new letter(x);
    if(auto x = letter::a45::parse()) return new letter(x);
    if(auto x = letter::a46::parse()) return new letter(x);
    if(auto x = letter::a47::parse()) return new letter(x);
    if(auto x = letter::a48::parse()) return new letter(x);
    if(auto x = letter::a49::parse()) return new letter(x);
    if(auto x = letter::a50::parse()) return new letter(x);
    if(auto x = letter::a51::parse()) return new letter(x);
    if(auto x = letter::a52::parse()) return new letter(x);
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

symbol::a0* symbol::a0::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "[") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a0(_t0);
}

symbol::a1* symbol::a1::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "]") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a1(_t0);
}

symbol::a2* symbol::a2::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "{") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a2(_t0);
}

symbol::a3* symbol::a3::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "}") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a3(_t0);
}

symbol::a4* symbol::a4::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "(") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a4(_t0);
}

symbol::a5* symbol::a5::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ")") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a5(_t0);
}

symbol::a6* symbol::a6::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "<") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a6(_t0);
}

symbol::a7* symbol::a7::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ">") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a7(_t0);
}

symbol::a8* symbol::a8::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "'") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a8(_t0);
}

symbol::a9* symbol::a9::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "=") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a9(_t0);
}

symbol::a10* symbol::a10::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "|") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a10(_t0);
}

symbol::a11* symbol::a11::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ".") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a11(_t0);
}

symbol::a12* symbol::a12::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ",") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a12(_t0);
}

symbol::a13* symbol::a13::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ";") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a13(_t0);
}

symbol::a14* symbol::a14::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "-") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a14(_t0);
}

symbol::a15* symbol::a15::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "+") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a15(_t0);
}

symbol::a16* symbol::a16::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "*") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a16(_t0);
}

symbol::a17* symbol::a17::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "?") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a17(_t0);
}

symbol::a18* symbol::a18::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ":") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a18(_t0);
}

symbol::a19* symbol::a19::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "!") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a19(_t0);
}

symbol::a20* symbol::a20::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "@") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a20(_t0);
}

symbol::a21* symbol::a21::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "#") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a21(_t0);
}

symbol::a22* symbol::a22::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "$") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a22(_t0);
}

symbol::a23* symbol::a23::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "%") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a23(_t0);
}

symbol::a24* symbol::a24::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "^") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a24(_t0);
}

symbol::a25* symbol::a25::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "&") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a25(_t0);
}

symbol::a26* symbol::a26::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "/") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a26(_t0);
}

symbol::a27* symbol::a27::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "~") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a27(_t0);
}

symbol::a28* symbol::a28::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "`") {pop_stack(); return nullptr;}
    rm_stack();
    return new symbol::a28(_t0);
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

escape::a0::b0* escape::a0::b0::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "n") {pop_stack(); return nullptr;}
    rm_stack();
    return new escape::a0::b0(_t0);
}

escape::a0::b1* escape::a0::b1::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "t") {pop_stack(); return nullptr;}
    rm_stack();
    return new escape::a0::b1(_t0);
}

escape::a0::b2* escape::a0::b2::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "r") {pop_stack(); return nullptr;}
    rm_stack();
    return new escape::a0::b2(_t0);
}

escape::a0::b3* escape::a0::b3::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "f") {pop_stack(); return nullptr;}
    rm_stack();
    return new escape::a0::b3(_t0);
}

escape::a0::b4* escape::a0::b4::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "b") {pop_stack(); return nullptr;}
    rm_stack();
    return new escape::a0::b4(_t0);
}

escape::a0::b5* escape::a0::b5::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\"") {pop_stack(); return nullptr;}
    rm_stack();
    return new escape::a0::b5(_t0);
}

escape::a0::b6* escape::a0::b6::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\\") {pop_stack(); return nullptr;}
    rm_stack();
    return new escape::a0::b6(_t0);
}

escape::a0* escape::a0::parse() {
    if(auto x = escape::a0::b0::parse()) return new escape::a0(x);
    if(auto x = escape::a0::b1::parse()) return new escape::a0(x);
    if(auto x = escape::a0::b2::parse()) return new escape::a0(x);
    if(auto x = escape::a0::b3::parse()) return new escape::a0(x);
    if(auto x = escape::a0::b4::parse()) return new escape::a0(x);
    if(auto x = escape::a0::b5::parse()) return new escape::a0(x);
    if(auto x = escape::a0::b6::parse()) return new escape::a0(x);
    return nullptr;
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

identifier::a0* identifier::a0::parse() {
    push_stack();
    letter *_t0 = letter::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new identifier::a0(_t0);
}

identifier* identifier::parse() {
    push_stack();
    letter *_t0 = letter::parse();
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

comment_char::a0* comment_char::a0::parse() {
    push_stack();
    letter *_t0 = letter::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a0(_t0);
}

comment_char::a1* comment_char::a1::parse() {
    push_stack();
    digit *_t0 = digit::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a1(_t0);
}

comment_char::a2* comment_char::a2::parse() {
    push_stack();
    ws *_t0 = ws::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a2(_t0);
}

comment_char::a3* comment_char::a3::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "[") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a3(_t0);
}

comment_char::a4* comment_char::a4::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "]") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a4(_t0);
}

comment_char::a5* comment_char::a5::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "{") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a5(_t0);
}

comment_char::a6* comment_char::a6::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "}") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a6(_t0);
}

comment_char::a7* comment_char::a7::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "(") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a7(_t0);
}

comment_char::a8* comment_char::a8::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ")") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a8(_t0);
}

comment_char::a9* comment_char::a9::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "<") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a9(_t0);
}

comment_char::a10* comment_char::a10::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ">") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a10(_t0);
}

comment_char::a11* comment_char::a11::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "'") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a11(_t0);
}

comment_char::a12* comment_char::a12::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "=") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a12(_t0);
}

comment_char::a13* comment_char::a13::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "|") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a13(_t0);
}

comment_char::a14* comment_char::a14::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ".") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a14(_t0);
}

comment_char::a15* comment_char::a15::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ",") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a15(_t0);
}

comment_char::a16* comment_char::a16::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ";") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a16(_t0);
}

comment_char::a17* comment_char::a17::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "-") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a17(_t0);
}

comment_char::a18* comment_char::a18::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "+") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a18(_t0);
}

comment_char::a19* comment_char::a19::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "?") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a19(_t0);
}

comment_char::a20* comment_char::a20::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != ":") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a20(_t0);
}

comment_char::a21* comment_char::a21::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "!") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a21(_t0);
}

comment_char::a22* comment_char::a22::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "@") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a22(_t0);
}

comment_char::a23* comment_char::a23::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "#") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a23(_t0);
}

comment_char::a24* comment_char::a24::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "$") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a24(_t0);
}

comment_char::a25* comment_char::a25::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "%") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a25(_t0);
}

comment_char::a26* comment_char::a26::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "^") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a26(_t0);
}

comment_char::a27* comment_char::a27::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "&") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a27(_t0);
}

comment_char::a28* comment_char::a28::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "/") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a28(_t0);
}

comment_char::a29* comment_char::a29::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "~") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a29(_t0);
}

comment_char::a30* comment_char::a30::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "`") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a30(_t0);
}

comment_char::a31* comment_char::a31::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\\") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a31(_t0);
}

comment_char::a32* comment_char::a32::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\"") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment_char::a32(_t0);
}

comment_char* comment_char::parse() {
    if(auto x = comment_char::a0::parse()) return new comment_char(x);
    if(auto x = comment_char::a1::parse()) return new comment_char(x);
    if(auto x = comment_char::a2::parse()) return new comment_char(x);
    if(auto x = comment_char::a3::parse()) return new comment_char(x);
    if(auto x = comment_char::a4::parse()) return new comment_char(x);
    if(auto x = comment_char::a5::parse()) return new comment_char(x);
    if(auto x = comment_char::a6::parse()) return new comment_char(x);
    if(auto x = comment_char::a7::parse()) return new comment_char(x);
    if(auto x = comment_char::a8::parse()) return new comment_char(x);
    if(auto x = comment_char::a9::parse()) return new comment_char(x);
    if(auto x = comment_char::a10::parse()) return new comment_char(x);
    if(auto x = comment_char::a11::parse()) return new comment_char(x);
    if(auto x = comment_char::a12::parse()) return new comment_char(x);
    if(auto x = comment_char::a13::parse()) return new comment_char(x);
    if(auto x = comment_char::a14::parse()) return new comment_char(x);
    if(auto x = comment_char::a15::parse()) return new comment_char(x);
    if(auto x = comment_char::a16::parse()) return new comment_char(x);
    if(auto x = comment_char::a17::parse()) return new comment_char(x);
    if(auto x = comment_char::a18::parse()) return new comment_char(x);
    if(auto x = comment_char::a19::parse()) return new comment_char(x);
    if(auto x = comment_char::a20::parse()) return new comment_char(x);
    if(auto x = comment_char::a21::parse()) return new comment_char(x);
    if(auto x = comment_char::a22::parse()) return new comment_char(x);
    if(auto x = comment_char::a23::parse()) return new comment_char(x);
    if(auto x = comment_char::a24::parse()) return new comment_char(x);
    if(auto x = comment_char::a25::parse()) return new comment_char(x);
    if(auto x = comment_char::a26::parse()) return new comment_char(x);
    if(auto x = comment_char::a27::parse()) return new comment_char(x);
    if(auto x = comment_char::a28::parse()) return new comment_char(x);
    if(auto x = comment_char::a29::parse()) return new comment_char(x);
    if(auto x = comment_char::a30::parse()) return new comment_char(x);
    if(auto x = comment_char::a31::parse()) return new comment_char(x);
    if(auto x = comment_char::a32::parse()) return new comment_char(x);
    return nullptr;
}

comment::a0* comment::a0::parse() {
    push_stack();
    comment_char *_t0 = comment_char::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new comment::a0(_t0);
}

comment* comment::parse() {
    push_stack();
    std::string _t0 = next_chars(2);
    if(_t0 != "(*") {pop_stack(); return nullptr;}
    std::vector<comment::a0*> _t1;
    while(true) {
        comment::a0 *tmp = comment::a0::parse();
        if(tmp == nullptr) break;
        _t1.push_back(tmp);
    }
    std::string _t2 = next_chars(2);
    if(_t2 != "*)") {pop_stack(); return nullptr;}
    rm_stack();
    return new comment(_t0, _t1, _t2);
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

ws::a6* ws::a6::parse() {
    push_stack();
    comment *_t0 = comment::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new ws::a6(_t0);
}

ws* ws::parse() {
    if(auto x = ws::a0::parse()) return new ws(x);
    if(auto x = ws::a1::parse()) return new ws(x);
    if(auto x = ws::a2::parse()) return new ws(x);
    if(auto x = ws::a3::parse()) return new ws(x);
    if(auto x = ws::a4::parse()) return new ws(x);
    if(auto x = ws::a5::parse()) return new ws(x);
    if(auto x = ws::a6::parse()) return new ws(x);
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

terminal_char::a0* terminal_char::a0::parse() {
    push_stack();
    letter *_t0 = letter::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new terminal_char::a0(_t0);
}

terminal_char::a1* terminal_char::a1::parse() {
    push_stack();
    digit *_t0 = digit::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new terminal_char::a1(_t0);
}

terminal_char::a2* terminal_char::a2::parse() {
    push_stack();
    symbol *_t0 = symbol::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new terminal_char::a2(_t0);
}

terminal_char::a3* terminal_char::a3::parse() {
    push_stack();
    escape *_t0 = escape::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new terminal_char::a3(_t0);
}

terminal_char::a4* terminal_char::a4::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != " ") {pop_stack(); return nullptr;}
    rm_stack();
    return new terminal_char::a4(_t0);
}

terminal_char* terminal_char::parse() {
    if(auto x = terminal_char::a0::parse()) return new terminal_char(x);
    if(auto x = terminal_char::a1::parse()) return new terminal_char(x);
    if(auto x = terminal_char::a2::parse()) return new terminal_char(x);
    if(auto x = terminal_char::a3::parse()) return new terminal_char(x);
    if(auto x = terminal_char::a4::parse()) return new terminal_char(x);
    return nullptr;
}

terminal::a0* terminal::a0::parse() {
    push_stack();
    terminal_char *_t0 = terminal_char::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new terminal::a0(_t0);
}

terminal* terminal::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "\"") {pop_stack(); return nullptr;}
    terminal_char *_t1 = terminal_char::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    std::vector<terminal::a0*> _t2;
    while(true) {
        terminal::a0 *tmp = terminal::a0::parse();
        if(tmp == nullptr) break;
        _t2.push_back(tmp);
    }
    std::string _t3 = next_chars(1);
    if(_t3 != "\"") {pop_stack(); return nullptr;}
    rm_stack();
    return new terminal(_t0, _t1, _t2, _t3);
}

term::a0* term::a0::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "(") {pop_stack(); return nullptr;}
    rws *_t1 = rws::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    alternation *_t2 = alternation::parse();
    if(_t2 == nullptr) {pop_stack(); return nullptr;}
    rws *_t3 = rws::parse();
    if(_t3 == nullptr) {pop_stack(); return nullptr;}
    std::string _t4 = next_chars(1);
    if(_t4 != ")") {pop_stack(); return nullptr;}
    rm_stack();
    return new term::a0(_t0, _t1, _t2, _t3, _t4);
}

term::a1* term::a1::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "[") {pop_stack(); return nullptr;}
    rws *_t1 = rws::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    alternation *_t2 = alternation::parse();
    if(_t2 == nullptr) {pop_stack(); return nullptr;}
    rws *_t3 = rws::parse();
    if(_t3 == nullptr) {pop_stack(); return nullptr;}
    std::string _t4 = next_chars(1);
    if(_t4 != "]") {pop_stack(); return nullptr;}
    rm_stack();
    return new term::a1(_t0, _t1, _t2, _t3, _t4);
}

term::a2* term::a2::parse() {
    push_stack();
    std::string _t0 = next_chars(1);
    if(_t0 != "{") {pop_stack(); return nullptr;}
    rws *_t1 = rws::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    alternation *_t2 = alternation::parse();
    if(_t2 == nullptr) {pop_stack(); return nullptr;}
    rws *_t3 = rws::parse();
    if(_t3 == nullptr) {pop_stack(); return nullptr;}
    std::string _t4 = next_chars(1);
    if(_t4 != "}") {pop_stack(); return nullptr;}
    rm_stack();
    return new term::a2(_t0, _t1, _t2, _t3, _t4);
}

term::a3* term::a3::parse() {
    push_stack();
    terminal *_t0 = terminal::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new term::a3(_t0);
}

term::a4* term::a4::parse() {
    push_stack();
    identifier *_t0 = identifier::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new term::a4(_t0);
}

term* term::parse() {
    if(auto x = term::a0::parse()) return new term(x);
    if(auto x = term::a1::parse()) return new term(x);
    if(auto x = term::a2::parse()) return new term(x);
    if(auto x = term::a3::parse()) return new term(x);
    if(auto x = term::a4::parse()) return new term(x);
    return nullptr;
}

concatenation::a0* concatenation::a0::parse() {
    push_stack();
    rws *_t0 = rws::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    std::string _t1 = next_chars(1);
    if(_t1 != ",") {pop_stack(); return nullptr;}
    rws *_t2 = rws::parse();
    if(_t2 == nullptr) {pop_stack(); return nullptr;}
    term *_t3 = term::parse();
    if(_t3 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new concatenation::a0(_t0, _t1, _t2, _t3);
}

concatenation* concatenation::parse() {
    push_stack();
    term *_t0 = term::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    std::vector<concatenation::a0*> _t1;
    while(true) {
        concatenation::a0 *tmp = concatenation::a0::parse();
        if(tmp == nullptr) break;
        _t1.push_back(tmp);
    }
    rm_stack();
    return new concatenation(_t0, _t1);
}

alternation::a0* alternation::a0::parse() {
    push_stack();
    rws *_t0 = rws::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    std::string _t1 = next_chars(1);
    if(_t1 != "|") {pop_stack(); return nullptr;}
    rws *_t2 = rws::parse();
    if(_t2 == nullptr) {pop_stack(); return nullptr;}
    concatenation *_t3 = concatenation::parse();
    if(_t3 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new alternation::a0(_t0, _t1, _t2, _t3);
}

alternation* alternation::parse() {
    push_stack();
    concatenation *_t0 = concatenation::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    std::vector<alternation::a0*> _t1;
    while(true) {
        alternation::a0 *tmp = alternation::a0::parse();
        if(tmp == nullptr) break;
        _t1.push_back(tmp);
    }
    rm_stack();
    return new alternation(_t0, _t1);
}

rule* rule::parse() {
    push_stack();
    identifier *_t0 = identifier::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rws *_t1 = rws::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    std::string _t2 = next_chars(1);
    if(_t2 != "=") {pop_stack(); return nullptr;}
    rws *_t3 = rws::parse();
    if(_t3 == nullptr) {pop_stack(); return nullptr;}
    alternation *_t4 = alternation::parse();
    if(_t4 == nullptr) {pop_stack(); return nullptr;}
    rws *_t5 = rws::parse();
    if(_t5 == nullptr) {pop_stack(); return nullptr;}
    std::string _t6 = next_chars(1);
    if(_t6 != ";") {pop_stack(); return nullptr;}
    rm_stack();
    return new rule(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
}

grammar::a0* grammar::a0::parse() {
    push_stack();
    ows *_t0 = ows::parse();
    if(_t0 == nullptr) {pop_stack(); return nullptr;}
    rule *_t1 = rule::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new grammar::a0(_t0, _t1);
}

grammar* grammar::parse() {
    push_stack();
    std::vector<grammar::a0*> _t0;
    while(true) {
        grammar::a0 *tmp = grammar::a0::parse();
        if(tmp == nullptr) break;
        _t0.push_back(tmp);
    }
    ows *_t1 = ows::parse();
    if(_t1 == nullptr) {pop_stack(); return nullptr;}
    rm_stack();
    return new grammar(_t0, _t1);
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
    grammar *x = grammar::parse();
    assert(ptr_stack.size() == 0);
    if(x == nullptr) {std::cout << "FAILED\n"; return 0;}
    std::cout << "SUCCESS\n";
    std::cout << x->t0.size() << "\n";
    return 0;
}