// Date Generated : 06-14-2025 18:00:50
#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace parser {
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
    
    struct overloadable_operator;
    struct overload_identifier;
    struct function_identifier;
    struct function_definition;
    struct function;
    struct function_call;
    struct literal_sizeof;
    struct literal_integer;
    struct literal_float;
    struct literal_char;
    struct literal_string;
    struct literal;
    struct expr_primary;
    struct expr_postfix;
    struct expr_unary;
    struct expr_multiplicative;
    struct expr_additive;
    struct expr_shift;
    struct expr_relational;
    struct expr_equality;
    struct expr_bit_and;
    struct expr_bit_xor;
    struct expr_bit_or;
    struct expr_logical_and;
    struct expr_logical_or;
    struct expr_assignment;
    struct expression;
    struct member_variable_declaration;
    struct struct_definition;
    struct alpha;
    struct digit;
    struct escape;
    struct symbol;
    struct line_comment;
    struct multiline_comment;
    struct ws;
    struct rws;
    struct ows;
    struct type;
    struct base_type;
    struct identifier;
    struct declaration;
    struct parameter;
    struct parameter_list;
    struct argument_list;
    struct statement;
    struct simple_statement;
    struct control_statement;
    struct compound_statement;
    struct program;

    // overloadable_operator = "+" | "-" | "*" | "/" | "%" | "&" | "|" | "^" | "<<" | ">>" | "==" | "!=" | "<" | "<=" | ">" | ">=" | "++x" | "--x" | "x++" | "x--" | "[]" | "=" | "*x" | "(cast)" ;
    struct overloadable_operator {
        struct a0 {
            std::string t0;
            a0(std::string _t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            std::string t0;
            a1(std::string _t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            std::string t0;
            a2(std::string _t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            std::string t0;
            a3(std::string _t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string();
        };
        struct a4 {
            std::string t0;
            a4(std::string _t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string();
        };
        struct a5 {
            std::string t0;
            a5(std::string _t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string();
        };
        struct a6 {
            std::string t0;
            a6(std::string _t0) {
                t0 = _t0;
            }
            static a6* parse();
            std::string to_string();
        };
        struct a7 {
            std::string t0;
            a7(std::string _t0) {
                t0 = _t0;
            }
            static a7* parse();
            std::string to_string();
        };
        struct a8 {
            std::string t0;
            a8(std::string _t0) {
                t0 = _t0;
            }
            static a8* parse();
            std::string to_string();
        };
        struct a9 {
            std::string t0;
            a9(std::string _t0) {
                t0 = _t0;
            }
            static a9* parse();
            std::string to_string();
        };
        struct a10 {
            std::string t0;
            a10(std::string _t0) {
                t0 = _t0;
            }
            static a10* parse();
            std::string to_string();
        };
        struct a11 {
            std::string t0;
            a11(std::string _t0) {
                t0 = _t0;
            }
            static a11* parse();
            std::string to_string();
        };
        struct a12 {
            std::string t0;
            a12(std::string _t0) {
                t0 = _t0;
            }
            static a12* parse();
            std::string to_string();
        };
        struct a13 {
            std::string t0;
            a13(std::string _t0) {
                t0 = _t0;
            }
            static a13* parse();
            std::string to_string();
        };
        struct a14 {
            std::string t0;
            a14(std::string _t0) {
                t0 = _t0;
            }
            static a14* parse();
            std::string to_string();
        };
        struct a15 {
            std::string t0;
            a15(std::string _t0) {
                t0 = _t0;
            }
            static a15* parse();
            std::string to_string();
        };
        struct a16 {
            std::string t0;
            a16(std::string _t0) {
                t0 = _t0;
            }
            static a16* parse();
            std::string to_string();
        };
        struct a17 {
            std::string t0;
            a17(std::string _t0) {
                t0 = _t0;
            }
            static a17* parse();
            std::string to_string();
        };
        struct a18 {
            std::string t0;
            a18(std::string _t0) {
                t0 = _t0;
            }
            static a18* parse();
            std::string to_string();
        };
        struct a19 {
            std::string t0;
            a19(std::string _t0) {
                t0 = _t0;
            }
            static a19* parse();
            std::string to_string();
        };
        struct a20 {
            std::string t0;
            a20(std::string _t0) {
                t0 = _t0;
            }
            static a20* parse();
            std::string to_string();
        };
        struct a21 {
            std::string t0;
            a21(std::string _t0) {
                t0 = _t0;
            }
            static a21* parse();
            std::string to_string();
        };
        struct a22 {
            std::string t0;
            a22(std::string _t0) {
                t0 = _t0;
            }
            static a22* parse();
            std::string to_string();
        };
        struct a23 {
            std::string t0;
            a23(std::string _t0) {
                t0 = _t0;
            }
            static a23* parse();
            std::string to_string();
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
        overloadable_operator(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        overloadable_operator(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        overloadable_operator(a2 *_t2) {
            is_a2 = true;
            t2 = _t2;
        }
        overloadable_operator(a3 *_t3) {
            is_a3 = true;
            t3 = _t3;
        }
        overloadable_operator(a4 *_t4) {
            is_a4 = true;
            t4 = _t4;
        }
        overloadable_operator(a5 *_t5) {
            is_a5 = true;
            t5 = _t5;
        }
        overloadable_operator(a6 *_t6) {
            is_a6 = true;
            t6 = _t6;
        }
        overloadable_operator(a7 *_t7) {
            is_a7 = true;
            t7 = _t7;
        }
        overloadable_operator(a8 *_t8) {
            is_a8 = true;
            t8 = _t8;
        }
        overloadable_operator(a9 *_t9) {
            is_a9 = true;
            t9 = _t9;
        }
        overloadable_operator(a10 *_t10) {
            is_a10 = true;
            t10 = _t10;
        }
        overloadable_operator(a11 *_t11) {
            is_a11 = true;
            t11 = _t11;
        }
        overloadable_operator(a12 *_t12) {
            is_a12 = true;
            t12 = _t12;
        }
        overloadable_operator(a13 *_t13) {
            is_a13 = true;
            t13 = _t13;
        }
        overloadable_operator(a14 *_t14) {
            is_a14 = true;
            t14 = _t14;
        }
        overloadable_operator(a15 *_t15) {
            is_a15 = true;
            t15 = _t15;
        }
        overloadable_operator(a16 *_t16) {
            is_a16 = true;
            t16 = _t16;
        }
        overloadable_operator(a17 *_t17) {
            is_a17 = true;
            t17 = _t17;
        }
        overloadable_operator(a18 *_t18) {
            is_a18 = true;
            t18 = _t18;
        }
        overloadable_operator(a19 *_t19) {
            is_a19 = true;
            t19 = _t19;
        }
        overloadable_operator(a20 *_t20) {
            is_a20 = true;
            t20 = _t20;
        }
        overloadable_operator(a21 *_t21) {
            is_a21 = true;
            t21 = _t21;
        }
        overloadable_operator(a22 *_t22) {
            is_a22 = true;
            t22 = _t22;
        }
        overloadable_operator(a23 *_t23) {
            is_a23 = true;
            t23 = _t23;
        }
        static overloadable_operator* parse();
        std::string to_string();
    };

    // overload_identifier = "operator" , overloadable_operator ;
    struct overload_identifier {
        std::string t0;
        overloadable_operator *t1;
        overload_identifier(std::string _t0, overloadable_operator *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static overload_identifier* parse();
        std::string to_string();
    };

    // function_identifier = overload_identifier | identifier ;
    struct function_identifier {
        struct a0 {
            overload_identifier *t0;
            a0(overload_identifier *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            identifier *t0;
            a1(identifier *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        function_identifier(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        function_identifier(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        static function_identifier* parse();
        std::string to_string();
    };

    // function_definition = type , rws , function_identifier , ows , "(" , ows , parameter_list , ows , ")" ;
    struct function_definition {
        type *t0;
        rws *t1;
        function_identifier *t2;
        ows *t3;
        std::string t4;
        ows *t5;
        parameter_list *t6;
        ows *t7;
        std::string t8;
        function_definition(type *_t0, rws *_t1, function_identifier *_t2, ows *_t3, std::string _t4, ows *_t5, parameter_list *_t6, ows *_t7, std::string _t8) {
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
        static function_definition* parse();
        std::string to_string();
    };

    // function = function_definition , ows , compound_statement ;
    struct function {
        function_definition *t0;
        ows *t1;
        compound_statement *t2;
        function(function_definition *_t0, ows *_t1, compound_statement *_t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static function* parse();
        std::string to_string();
    };

    // function_call = identifier , ows , "(" , ows , argument_list , ows , ")" ;
    struct function_call {
        identifier *t0;
        ows *t1;
        std::string t2;
        ows *t3;
        argument_list *t4;
        ows *t5;
        std::string t6;
        function_call(identifier *_t0, ows *_t1, std::string _t2, ows *_t3, argument_list *_t4, ows *_t5, std::string _t6) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
            t5 = _t5;
            t6 = _t6;
        }
        static function_call* parse();
        std::string to_string();
    };

    // literal_sizeof = "sizeof" , ows , "(" , ows , type , ows , ")" ;
    struct literal_sizeof {
        std::string t0;
        ows *t1;
        std::string t2;
        ows *t3;
        type *t4;
        ows *t5;
        std::string t6;
        literal_sizeof(std::string _t0, ows *_t1, std::string _t2, ows *_t3, type *_t4, ows *_t5, std::string _t6) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
            t5 = _t5;
            t6 = _t6;
        }
        static literal_sizeof* parse();
        std::string to_string();
    };

    // literal_integer = < digit > ;
    struct literal_integer {
        struct a0 {
            digit *t0;
            a0(digit *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        std::vector<a0*> t0;
        literal_integer(std::vector<a0*> _t0) {
            t0 = _t0;
        }
        static literal_integer* parse();
        std::string to_string();
    };

    // literal_float = < digit > , "." , < digit > ;
    struct literal_float {
        struct a0 {
            digit *t0;
            a0(digit *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            digit *t0;
            a1(digit *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        std::vector<a0*> t0;
        std::string t1;
        std::vector<a1*> t2;
        literal_float(std::vector<a0*> _t0, std::string _t1, std::vector<a1*> _t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static literal_float* parse();
        std::string to_string();
    };

    // literal_char = "'" , ( alpha | digit | escape | symbol | " " ) , "'" ;
    struct literal_char {
        struct a0 {
            struct b0 {
                alpha *t0;
                b0(alpha *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                digit *t0;
                b1(digit *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            struct b2 {
                escape *t0;
                b2(escape *_t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string();
            };
            struct b3 {
                symbol *t0;
                b3(symbol *_t0) {
                    t0 = _t0;
                }
                static b3* parse();
                std::string to_string();
            };
            struct b4 {
                std::string t0;
                b4(std::string _t0) {
                    t0 = _t0;
                }
                static b4* parse();
                std::string to_string();
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
            static a0* parse();
            std::string to_string();
        };
        std::string t0;
        a0 *t1;
        std::string t2;
        literal_char(std::string _t0, a0 *_t1, std::string _t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static literal_char* parse();
        std::string to_string();
    };

    // literal_string = "\"" , { alpha | digit | escape | symbol | " " } , "\"" ;
    struct literal_string {
        struct a0 {
            struct b0 {
                alpha *t0;
                b0(alpha *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                digit *t0;
                b1(digit *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            struct b2 {
                escape *t0;
                b2(escape *_t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string();
            };
            struct b3 {
                symbol *t0;
                b3(symbol *_t0) {
                    t0 = _t0;
                }
                static b3* parse();
                std::string to_string();
            };
            struct b4 {
                std::string t0;
                b4(std::string _t0) {
                    t0 = _t0;
                }
                static b4* parse();
                std::string to_string();
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
            static a0* parse();
            std::string to_string();
        };
        std::string t0;
        std::vector<a0*> t1;
        std::string t2;
        literal_string(std::string _t0, std::vector<a0*> _t1, std::string _t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static literal_string* parse();
        std::string to_string();
    };

    // literal = literal_integer | literal_sizeof | literal_char | literal_string ;
    struct literal {
        struct a0 {
            literal_integer *t0;
            a0(literal_integer *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            literal_sizeof *t0;
            a1(literal_sizeof *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            literal_char *t0;
            a2(literal_char *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            literal_string *t0;
            a3(literal_string *_t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string();
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        bool is_a2 = false;
        a2 *t2;
        bool is_a3 = false;
        a3 *t3;
        literal(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        literal(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        literal(a2 *_t2) {
            is_a2 = true;
            t2 = _t2;
        }
        literal(a3 *_t3) {
            is_a3 = true;
            t3 = _t3;
        }
        static literal* parse();
        std::string to_string();
    };

    // expr_primary = literal | function_call | identifier | "(" , ows , expression , ows , ")" ;
    struct expr_primary {
        struct a0 {
            literal *t0;
            a0(literal *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            function_call *t0;
            a1(function_call *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            identifier *t0;
            a2(identifier *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            std::string t0;
            ows *t1;
            expression *t2;
            ows *t3;
            std::string t4;
            a3(std::string _t0, ows *_t1, expression *_t2, ows *_t3, std::string _t4) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
                t4 = _t4;
            }
            static a3* parse();
            std::string to_string();
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        bool is_a2 = false;
        a2 *t2;
        bool is_a3 = false;
        a3 *t3;
        expr_primary(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        expr_primary(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        expr_primary(a2 *_t2) {
            is_a2 = true;
            t2 = _t2;
        }
        expr_primary(a3 *_t3) {
            is_a3 = true;
            t3 = _t3;
        }
        static expr_primary* parse();
        std::string to_string();
    };

    // expr_postfix = expr_primary , { ows , ( "[" , ows , expression , ows , "]" | "." , ows , function_call | "->" , ows , function_call | "." , ows , identifier | "->" , ows , identifier | "++" | "--" ) } ;
    struct expr_postfix {
        struct a0 {
            struct b0 {
                struct c0 {
                    std::string t0;
                    ows *t1;
                    expression *t2;
                    ows *t3;
                    std::string t4;
                    c0(std::string _t0, ows *_t1, expression *_t2, ows *_t3, std::string _t4) {
                        t0 = _t0;
                        t1 = _t1;
                        t2 = _t2;
                        t3 = _t3;
                        t4 = _t4;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    std::string t0;
                    ows *t1;
                    function_call *t2;
                    c1(std::string _t0, ows *_t1, function_call *_t2) {
                        t0 = _t0;
                        t1 = _t1;
                        t2 = _t2;
                    }
                    static c1* parse();
                    std::string to_string();
                };
                struct c2 {
                    std::string t0;
                    ows *t1;
                    function_call *t2;
                    c2(std::string _t0, ows *_t1, function_call *_t2) {
                        t0 = _t0;
                        t1 = _t1;
                        t2 = _t2;
                    }
                    static c2* parse();
                    std::string to_string();
                };
                struct c3 {
                    std::string t0;
                    ows *t1;
                    identifier *t2;
                    c3(std::string _t0, ows *_t1, identifier *_t2) {
                        t0 = _t0;
                        t1 = _t1;
                        t2 = _t2;
                    }
                    static c3* parse();
                    std::string to_string();
                };
                struct c4 {
                    std::string t0;
                    ows *t1;
                    identifier *t2;
                    c4(std::string _t0, ows *_t1, identifier *_t2) {
                        t0 = _t0;
                        t1 = _t1;
                        t2 = _t2;
                    }
                    static c4* parse();
                    std::string to_string();
                };
                struct c5 {
                    std::string t0;
                    c5(std::string _t0) {
                        t0 = _t0;
                    }
                    static c5* parse();
                    std::string to_string();
                };
                struct c6 {
                    std::string t0;
                    c6(std::string _t0) {
                        t0 = _t0;
                    }
                    static c6* parse();
                    std::string to_string();
                };
                bool is_c0 = false;
                c0 *t0;
                bool is_c1 = false;
                c1 *t1;
                bool is_c2 = false;
                c2 *t2;
                bool is_c3 = false;
                c3 *t3;
                bool is_c4 = false;
                c4 *t4;
                bool is_c5 = false;
                c5 *t5;
                bool is_c6 = false;
                c6 *t6;
                b0(c0 *_t0) {
                    is_c0 = true;
                    t0 = _t0;
                }
                b0(c1 *_t1) {
                    is_c1 = true;
                    t1 = _t1;
                }
                b0(c2 *_t2) {
                    is_c2 = true;
                    t2 = _t2;
                }
                b0(c3 *_t3) {
                    is_c3 = true;
                    t3 = _t3;
                }
                b0(c4 *_t4) {
                    is_c4 = true;
                    t4 = _t4;
                }
                b0(c5 *_t5) {
                    is_c5 = true;
                    t5 = _t5;
                }
                b0(c6 *_t6) {
                    is_c6 = true;
                    t6 = _t6;
                }
                static b0* parse();
                std::string to_string();
            };
            ows *t0;
            b0 *t1;
            a0(ows *_t0, b0 *_t1) {
                t0 = _t0;
                t1 = _t1;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_primary *t0;
        std::vector<a0*> t1;
        expr_postfix(expr_primary *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_postfix* parse();
        std::string to_string();
    };

    // expr_unary = ( "++" | "--" | "+" | "-" | "~" | "!" | "*" ) , ows , expr_unary | expr_postfix ;
    struct expr_unary {
        struct a0 {
            struct b0 {
                struct c0 {
                    std::string t0;
                    c0(std::string _t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    std::string t0;
                    c1(std::string _t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string();
                };
                struct c2 {
                    std::string t0;
                    c2(std::string _t0) {
                        t0 = _t0;
                    }
                    static c2* parse();
                    std::string to_string();
                };
                struct c3 {
                    std::string t0;
                    c3(std::string _t0) {
                        t0 = _t0;
                    }
                    static c3* parse();
                    std::string to_string();
                };
                struct c4 {
                    std::string t0;
                    c4(std::string _t0) {
                        t0 = _t0;
                    }
                    static c4* parse();
                    std::string to_string();
                };
                struct c5 {
                    std::string t0;
                    c5(std::string _t0) {
                        t0 = _t0;
                    }
                    static c5* parse();
                    std::string to_string();
                };
                struct c6 {
                    std::string t0;
                    c6(std::string _t0) {
                        t0 = _t0;
                    }
                    static c6* parse();
                    std::string to_string();
                };
                bool is_c0 = false;
                c0 *t0;
                bool is_c1 = false;
                c1 *t1;
                bool is_c2 = false;
                c2 *t2;
                bool is_c3 = false;
                c3 *t3;
                bool is_c4 = false;
                c4 *t4;
                bool is_c5 = false;
                c5 *t5;
                bool is_c6 = false;
                c6 *t6;
                b0(c0 *_t0) {
                    is_c0 = true;
                    t0 = _t0;
                }
                b0(c1 *_t1) {
                    is_c1 = true;
                    t1 = _t1;
                }
                b0(c2 *_t2) {
                    is_c2 = true;
                    t2 = _t2;
                }
                b0(c3 *_t3) {
                    is_c3 = true;
                    t3 = _t3;
                }
                b0(c4 *_t4) {
                    is_c4 = true;
                    t4 = _t4;
                }
                b0(c5 *_t5) {
                    is_c5 = true;
                    t5 = _t5;
                }
                b0(c6 *_t6) {
                    is_c6 = true;
                    t6 = _t6;
                }
                static b0* parse();
                std::string to_string();
            };
            b0 *t0;
            ows *t1;
            expr_unary *t2;
            a0(b0 *_t0, ows *_t1, expr_unary *_t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            expr_postfix *t0;
            a1(expr_postfix *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        expr_unary(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        expr_unary(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        static expr_unary* parse();
        std::string to_string();
    };

    // expr_multiplicative = expr_unary , { ows , ( "*" | "/" | "%" ) , ows , expr_unary } ;
    struct expr_multiplicative {
        struct a0 {
            struct b0 {
                struct c0 {
                    std::string t0;
                    c0(std::string _t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    std::string t0;
                    c1(std::string _t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string();
                };
                struct c2 {
                    std::string t0;
                    c2(std::string _t0) {
                        t0 = _t0;
                    }
                    static c2* parse();
                    std::string to_string();
                };
                bool is_c0 = false;
                c0 *t0;
                bool is_c1 = false;
                c1 *t1;
                bool is_c2 = false;
                c2 *t2;
                b0(c0 *_t0) {
                    is_c0 = true;
                    t0 = _t0;
                }
                b0(c1 *_t1) {
                    is_c1 = true;
                    t1 = _t1;
                }
                b0(c2 *_t2) {
                    is_c2 = true;
                    t2 = _t2;
                }
                static b0* parse();
                std::string to_string();
            };
            ows *t0;
            b0 *t1;
            ows *t2;
            expr_unary *t3;
            a0(ows *_t0, b0 *_t1, ows *_t2, expr_unary *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_unary *t0;
        std::vector<a0*> t1;
        expr_multiplicative(expr_unary *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_multiplicative* parse();
        std::string to_string();
    };

    // expr_additive = expr_multiplicative , { ows , ( "+" | "-" ) , ows , expr_multiplicative } ;
    struct expr_additive {
        struct a0 {
            struct b0 {
                struct c0 {
                    std::string t0;
                    c0(std::string _t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    std::string t0;
                    c1(std::string _t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string();
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
                std::string to_string();
            };
            ows *t0;
            b0 *t1;
            ows *t2;
            expr_multiplicative *t3;
            a0(ows *_t0, b0 *_t1, ows *_t2, expr_multiplicative *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_multiplicative *t0;
        std::vector<a0*> t1;
        expr_additive(expr_multiplicative *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_additive* parse();
        std::string to_string();
    };

    // expr_shift = expr_additive , { ows , ( "<<" | ">>" ) , ows , expr_additive } ;
    struct expr_shift {
        struct a0 {
            struct b0 {
                struct c0 {
                    std::string t0;
                    c0(std::string _t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    std::string t0;
                    c1(std::string _t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string();
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
                std::string to_string();
            };
            ows *t0;
            b0 *t1;
            ows *t2;
            expr_additive *t3;
            a0(ows *_t0, b0 *_t1, ows *_t2, expr_additive *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_additive *t0;
        std::vector<a0*> t1;
        expr_shift(expr_additive *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_shift* parse();
        std::string to_string();
    };

    // expr_relational = expr_shift , { ows , ( "<=" | "<" | ">=" | ">" ) , ows , expr_shift } ;
    struct expr_relational {
        struct a0 {
            struct b0 {
                struct c0 {
                    std::string t0;
                    c0(std::string _t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    std::string t0;
                    c1(std::string _t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string();
                };
                struct c2 {
                    std::string t0;
                    c2(std::string _t0) {
                        t0 = _t0;
                    }
                    static c2* parse();
                    std::string to_string();
                };
                struct c3 {
                    std::string t0;
                    c3(std::string _t0) {
                        t0 = _t0;
                    }
                    static c3* parse();
                    std::string to_string();
                };
                bool is_c0 = false;
                c0 *t0;
                bool is_c1 = false;
                c1 *t1;
                bool is_c2 = false;
                c2 *t2;
                bool is_c3 = false;
                c3 *t3;
                b0(c0 *_t0) {
                    is_c0 = true;
                    t0 = _t0;
                }
                b0(c1 *_t1) {
                    is_c1 = true;
                    t1 = _t1;
                }
                b0(c2 *_t2) {
                    is_c2 = true;
                    t2 = _t2;
                }
                b0(c3 *_t3) {
                    is_c3 = true;
                    t3 = _t3;
                }
                static b0* parse();
                std::string to_string();
            };
            ows *t0;
            b0 *t1;
            ows *t2;
            expr_shift *t3;
            a0(ows *_t0, b0 *_t1, ows *_t2, expr_shift *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_shift *t0;
        std::vector<a0*> t1;
        expr_relational(expr_shift *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_relational* parse();
        std::string to_string();
    };

    // expr_equality = expr_relational , { ows , ( "==" | "!=" ) , ows , expr_relational } ;
    struct expr_equality {
        struct a0 {
            struct b0 {
                struct c0 {
                    std::string t0;
                    c0(std::string _t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    std::string t0;
                    c1(std::string _t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string();
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
                std::string to_string();
            };
            ows *t0;
            b0 *t1;
            ows *t2;
            expr_relational *t3;
            a0(ows *_t0, b0 *_t1, ows *_t2, expr_relational *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_relational *t0;
        std::vector<a0*> t1;
        expr_equality(expr_relational *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_equality* parse();
        std::string to_string();
    };

    // expr_bit_and = expr_equality , { ows , "&" , ows , expr_equality } ;
    struct expr_bit_and {
        struct a0 {
            ows *t0;
            std::string t1;
            ows *t2;
            expr_equality *t3;
            a0(ows *_t0, std::string _t1, ows *_t2, expr_equality *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_equality *t0;
        std::vector<a0*> t1;
        expr_bit_and(expr_equality *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_bit_and* parse();
        std::string to_string();
    };

    // expr_bit_xor = expr_bit_and , { ows , "^" , ows , expr_bit_and } ;
    struct expr_bit_xor {
        struct a0 {
            ows *t0;
            std::string t1;
            ows *t2;
            expr_bit_and *t3;
            a0(ows *_t0, std::string _t1, ows *_t2, expr_bit_and *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_bit_and *t0;
        std::vector<a0*> t1;
        expr_bit_xor(expr_bit_and *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_bit_xor* parse();
        std::string to_string();
    };

    // expr_bit_or = expr_bit_xor , { ows , "|" , ows , expr_bit_xor } ;
    struct expr_bit_or {
        struct a0 {
            ows *t0;
            std::string t1;
            ows *t2;
            expr_bit_xor *t3;
            a0(ows *_t0, std::string _t1, ows *_t2, expr_bit_xor *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_bit_xor *t0;
        std::vector<a0*> t1;
        expr_bit_or(expr_bit_xor *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_bit_or* parse();
        std::string to_string();
    };

    // expr_logical_and = expr_bit_or , { ows , "&&" , ows , expr_bit_or } ;
    struct expr_logical_and {
        struct a0 {
            ows *t0;
            std::string t1;
            ows *t2;
            expr_bit_or *t3;
            a0(ows *_t0, std::string _t1, ows *_t2, expr_bit_or *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_bit_or *t0;
        std::vector<a0*> t1;
        expr_logical_and(expr_bit_or *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_logical_and* parse();
        std::string to_string();
    };

    // expr_logical_or = expr_logical_and , { ows , "||" , ows , expr_logical_and } ;
    struct expr_logical_or {
        struct a0 {
            ows *t0;
            std::string t1;
            ows *t2;
            expr_logical_and *t3;
            a0(ows *_t0, std::string _t1, ows *_t2, expr_logical_and *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_logical_and *t0;
        std::vector<a0*> t1;
        expr_logical_or(expr_logical_and *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_logical_or* parse();
        std::string to_string();
    };

    // expr_assignment = expr_logical_or , { ows , ( "=" | "+=" | "-=" | "*=" | "/=" | "%=" | "<<=" | ">>=" | "&=" | "^=" | "|=" ) , ows , expr_logical_or } ;
    struct expr_assignment {
        struct a0 {
            struct b0 {
                struct c0 {
                    std::string t0;
                    c0(std::string _t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    std::string t0;
                    c1(std::string _t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string();
                };
                struct c2 {
                    std::string t0;
                    c2(std::string _t0) {
                        t0 = _t0;
                    }
                    static c2* parse();
                    std::string to_string();
                };
                struct c3 {
                    std::string t0;
                    c3(std::string _t0) {
                        t0 = _t0;
                    }
                    static c3* parse();
                    std::string to_string();
                };
                struct c4 {
                    std::string t0;
                    c4(std::string _t0) {
                        t0 = _t0;
                    }
                    static c4* parse();
                    std::string to_string();
                };
                struct c5 {
                    std::string t0;
                    c5(std::string _t0) {
                        t0 = _t0;
                    }
                    static c5* parse();
                    std::string to_string();
                };
                struct c6 {
                    std::string t0;
                    c6(std::string _t0) {
                        t0 = _t0;
                    }
                    static c6* parse();
                    std::string to_string();
                };
                struct c7 {
                    std::string t0;
                    c7(std::string _t0) {
                        t0 = _t0;
                    }
                    static c7* parse();
                    std::string to_string();
                };
                struct c8 {
                    std::string t0;
                    c8(std::string _t0) {
                        t0 = _t0;
                    }
                    static c8* parse();
                    std::string to_string();
                };
                struct c9 {
                    std::string t0;
                    c9(std::string _t0) {
                        t0 = _t0;
                    }
                    static c9* parse();
                    std::string to_string();
                };
                struct c10 {
                    std::string t0;
                    c10(std::string _t0) {
                        t0 = _t0;
                    }
                    static c10* parse();
                    std::string to_string();
                };
                bool is_c0 = false;
                c0 *t0;
                bool is_c1 = false;
                c1 *t1;
                bool is_c2 = false;
                c2 *t2;
                bool is_c3 = false;
                c3 *t3;
                bool is_c4 = false;
                c4 *t4;
                bool is_c5 = false;
                c5 *t5;
                bool is_c6 = false;
                c6 *t6;
                bool is_c7 = false;
                c7 *t7;
                bool is_c8 = false;
                c8 *t8;
                bool is_c9 = false;
                c9 *t9;
                bool is_c10 = false;
                c10 *t10;
                b0(c0 *_t0) {
                    is_c0 = true;
                    t0 = _t0;
                }
                b0(c1 *_t1) {
                    is_c1 = true;
                    t1 = _t1;
                }
                b0(c2 *_t2) {
                    is_c2 = true;
                    t2 = _t2;
                }
                b0(c3 *_t3) {
                    is_c3 = true;
                    t3 = _t3;
                }
                b0(c4 *_t4) {
                    is_c4 = true;
                    t4 = _t4;
                }
                b0(c5 *_t5) {
                    is_c5 = true;
                    t5 = _t5;
                }
                b0(c6 *_t6) {
                    is_c6 = true;
                    t6 = _t6;
                }
                b0(c7 *_t7) {
                    is_c7 = true;
                    t7 = _t7;
                }
                b0(c8 *_t8) {
                    is_c8 = true;
                    t8 = _t8;
                }
                b0(c9 *_t9) {
                    is_c9 = true;
                    t9 = _t9;
                }
                b0(c10 *_t10) {
                    is_c10 = true;
                    t10 = _t10;
                }
                static b0* parse();
                std::string to_string();
            };
            ows *t0;
            b0 *t1;
            ows *t2;
            expr_logical_or *t3;
            a0(ows *_t0, b0 *_t1, ows *_t2, expr_logical_or *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        expr_logical_or *t0;
        std::vector<a0*> t1;
        expr_assignment(expr_logical_or *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static expr_assignment* parse();
        std::string to_string();
    };

    // expression = expr_assignment ;
    struct expression {
        expr_assignment *t0;
        expression(expr_assignment *_t0) {
            t0 = _t0;
        }
        static expression* parse();
        std::string to_string();
    };

    // member_variable_declaration = type , rws , identifier , ";" ;
    struct member_variable_declaration {
        type *t0;
        rws *t1;
        identifier *t2;
        std::string t3;
        member_variable_declaration(type *_t0, rws *_t1, identifier *_t2, std::string _t3) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
        }
        static member_variable_declaration* parse();
        std::string to_string();
    };

    // struct_definition = "struct" , ows , base_type , ows , "{" , ows , { ( member_variable_declaration | function ) , ows } , "}" ;
    struct struct_definition {
        struct a0 {
            struct b0 {
                struct c0 {
                    member_variable_declaration *t0;
                    c0(member_variable_declaration *_t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    function *t0;
                    c1(function *_t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string();
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
                std::string to_string();
            };
            b0 *t0;
            ows *t1;
            a0(b0 *_t0, ows *_t1) {
                t0 = _t0;
                t1 = _t1;
            }
            static a0* parse();
            std::string to_string();
        };
        std::string t0;
        ows *t1;
        base_type *t2;
        ows *t3;
        std::string t4;
        ows *t5;
        std::vector<a0*> t6;
        std::string t7;
        struct_definition(std::string _t0, ows *_t1, base_type *_t2, ows *_t3, std::string _t4, ows *_t5, std::vector<a0*> _t6, std::string _t7) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
            t5 = _t5;
            t6 = _t6;
            t7 = _t7;
        }
        static struct_definition* parse();
        std::string to_string();
    };

    // alpha = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" ;
    struct alpha {
        struct a0 {
            std::string t0;
            a0(std::string _t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            std::string t0;
            a1(std::string _t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            std::string t0;
            a2(std::string _t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            std::string t0;
            a3(std::string _t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string();
        };
        struct a4 {
            std::string t0;
            a4(std::string _t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string();
        };
        struct a5 {
            std::string t0;
            a5(std::string _t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string();
        };
        struct a6 {
            std::string t0;
            a6(std::string _t0) {
                t0 = _t0;
            }
            static a6* parse();
            std::string to_string();
        };
        struct a7 {
            std::string t0;
            a7(std::string _t0) {
                t0 = _t0;
            }
            static a7* parse();
            std::string to_string();
        };
        struct a8 {
            std::string t0;
            a8(std::string _t0) {
                t0 = _t0;
            }
            static a8* parse();
            std::string to_string();
        };
        struct a9 {
            std::string t0;
            a9(std::string _t0) {
                t0 = _t0;
            }
            static a9* parse();
            std::string to_string();
        };
        struct a10 {
            std::string t0;
            a10(std::string _t0) {
                t0 = _t0;
            }
            static a10* parse();
            std::string to_string();
        };
        struct a11 {
            std::string t0;
            a11(std::string _t0) {
                t0 = _t0;
            }
            static a11* parse();
            std::string to_string();
        };
        struct a12 {
            std::string t0;
            a12(std::string _t0) {
                t0 = _t0;
            }
            static a12* parse();
            std::string to_string();
        };
        struct a13 {
            std::string t0;
            a13(std::string _t0) {
                t0 = _t0;
            }
            static a13* parse();
            std::string to_string();
        };
        struct a14 {
            std::string t0;
            a14(std::string _t0) {
                t0 = _t0;
            }
            static a14* parse();
            std::string to_string();
        };
        struct a15 {
            std::string t0;
            a15(std::string _t0) {
                t0 = _t0;
            }
            static a15* parse();
            std::string to_string();
        };
        struct a16 {
            std::string t0;
            a16(std::string _t0) {
                t0 = _t0;
            }
            static a16* parse();
            std::string to_string();
        };
        struct a17 {
            std::string t0;
            a17(std::string _t0) {
                t0 = _t0;
            }
            static a17* parse();
            std::string to_string();
        };
        struct a18 {
            std::string t0;
            a18(std::string _t0) {
                t0 = _t0;
            }
            static a18* parse();
            std::string to_string();
        };
        struct a19 {
            std::string t0;
            a19(std::string _t0) {
                t0 = _t0;
            }
            static a19* parse();
            std::string to_string();
        };
        struct a20 {
            std::string t0;
            a20(std::string _t0) {
                t0 = _t0;
            }
            static a20* parse();
            std::string to_string();
        };
        struct a21 {
            std::string t0;
            a21(std::string _t0) {
                t0 = _t0;
            }
            static a21* parse();
            std::string to_string();
        };
        struct a22 {
            std::string t0;
            a22(std::string _t0) {
                t0 = _t0;
            }
            static a22* parse();
            std::string to_string();
        };
        struct a23 {
            std::string t0;
            a23(std::string _t0) {
                t0 = _t0;
            }
            static a23* parse();
            std::string to_string();
        };
        struct a24 {
            std::string t0;
            a24(std::string _t0) {
                t0 = _t0;
            }
            static a24* parse();
            std::string to_string();
        };
        struct a25 {
            std::string t0;
            a25(std::string _t0) {
                t0 = _t0;
            }
            static a25* parse();
            std::string to_string();
        };
        struct a26 {
            std::string t0;
            a26(std::string _t0) {
                t0 = _t0;
            }
            static a26* parse();
            std::string to_string();
        };
        struct a27 {
            std::string t0;
            a27(std::string _t0) {
                t0 = _t0;
            }
            static a27* parse();
            std::string to_string();
        };
        struct a28 {
            std::string t0;
            a28(std::string _t0) {
                t0 = _t0;
            }
            static a28* parse();
            std::string to_string();
        };
        struct a29 {
            std::string t0;
            a29(std::string _t0) {
                t0 = _t0;
            }
            static a29* parse();
            std::string to_string();
        };
        struct a30 {
            std::string t0;
            a30(std::string _t0) {
                t0 = _t0;
            }
            static a30* parse();
            std::string to_string();
        };
        struct a31 {
            std::string t0;
            a31(std::string _t0) {
                t0 = _t0;
            }
            static a31* parse();
            std::string to_string();
        };
        struct a32 {
            std::string t0;
            a32(std::string _t0) {
                t0 = _t0;
            }
            static a32* parse();
            std::string to_string();
        };
        struct a33 {
            std::string t0;
            a33(std::string _t0) {
                t0 = _t0;
            }
            static a33* parse();
            std::string to_string();
        };
        struct a34 {
            std::string t0;
            a34(std::string _t0) {
                t0 = _t0;
            }
            static a34* parse();
            std::string to_string();
        };
        struct a35 {
            std::string t0;
            a35(std::string _t0) {
                t0 = _t0;
            }
            static a35* parse();
            std::string to_string();
        };
        struct a36 {
            std::string t0;
            a36(std::string _t0) {
                t0 = _t0;
            }
            static a36* parse();
            std::string to_string();
        };
        struct a37 {
            std::string t0;
            a37(std::string _t0) {
                t0 = _t0;
            }
            static a37* parse();
            std::string to_string();
        };
        struct a38 {
            std::string t0;
            a38(std::string _t0) {
                t0 = _t0;
            }
            static a38* parse();
            std::string to_string();
        };
        struct a39 {
            std::string t0;
            a39(std::string _t0) {
                t0 = _t0;
            }
            static a39* parse();
            std::string to_string();
        };
        struct a40 {
            std::string t0;
            a40(std::string _t0) {
                t0 = _t0;
            }
            static a40* parse();
            std::string to_string();
        };
        struct a41 {
            std::string t0;
            a41(std::string _t0) {
                t0 = _t0;
            }
            static a41* parse();
            std::string to_string();
        };
        struct a42 {
            std::string t0;
            a42(std::string _t0) {
                t0 = _t0;
            }
            static a42* parse();
            std::string to_string();
        };
        struct a43 {
            std::string t0;
            a43(std::string _t0) {
                t0 = _t0;
            }
            static a43* parse();
            std::string to_string();
        };
        struct a44 {
            std::string t0;
            a44(std::string _t0) {
                t0 = _t0;
            }
            static a44* parse();
            std::string to_string();
        };
        struct a45 {
            std::string t0;
            a45(std::string _t0) {
                t0 = _t0;
            }
            static a45* parse();
            std::string to_string();
        };
        struct a46 {
            std::string t0;
            a46(std::string _t0) {
                t0 = _t0;
            }
            static a46* parse();
            std::string to_string();
        };
        struct a47 {
            std::string t0;
            a47(std::string _t0) {
                t0 = _t0;
            }
            static a47* parse();
            std::string to_string();
        };
        struct a48 {
            std::string t0;
            a48(std::string _t0) {
                t0 = _t0;
            }
            static a48* parse();
            std::string to_string();
        };
        struct a49 {
            std::string t0;
            a49(std::string _t0) {
                t0 = _t0;
            }
            static a49* parse();
            std::string to_string();
        };
        struct a50 {
            std::string t0;
            a50(std::string _t0) {
                t0 = _t0;
            }
            static a50* parse();
            std::string to_string();
        };
        struct a51 {
            std::string t0;
            a51(std::string _t0) {
                t0 = _t0;
            }
            static a51* parse();
            std::string to_string();
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
        std::string to_string();
    };

    // digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
    struct digit {
        struct a0 {
            std::string t0;
            a0(std::string _t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            std::string t0;
            a1(std::string _t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            std::string t0;
            a2(std::string _t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            std::string t0;
            a3(std::string _t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string();
        };
        struct a4 {
            std::string t0;
            a4(std::string _t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string();
        };
        struct a5 {
            std::string t0;
            a5(std::string _t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string();
        };
        struct a6 {
            std::string t0;
            a6(std::string _t0) {
                t0 = _t0;
            }
            static a6* parse();
            std::string to_string();
        };
        struct a7 {
            std::string t0;
            a7(std::string _t0) {
                t0 = _t0;
            }
            static a7* parse();
            std::string to_string();
        };
        struct a8 {
            std::string t0;
            a8(std::string _t0) {
                t0 = _t0;
            }
            static a8* parse();
            std::string to_string();
        };
        struct a9 {
            std::string t0;
            a9(std::string _t0) {
                t0 = _t0;
            }
            static a9* parse();
            std::string to_string();
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
        std::string to_string();
    };

    // escape = "\\" , ( "n" | "t" | "r" | "f" | "b" | "\"" | "\\" | "'" | "0" ) ;
    struct escape {
        struct a0 {
            struct b0 {
                std::string t0;
                b0(std::string _t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                std::string t0;
                b1(std::string _t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            struct b2 {
                std::string t0;
                b2(std::string _t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string();
            };
            struct b3 {
                std::string t0;
                b3(std::string _t0) {
                    t0 = _t0;
                }
                static b3* parse();
                std::string to_string();
            };
            struct b4 {
                std::string t0;
                b4(std::string _t0) {
                    t0 = _t0;
                }
                static b4* parse();
                std::string to_string();
            };
            struct b5 {
                std::string t0;
                b5(std::string _t0) {
                    t0 = _t0;
                }
                static b5* parse();
                std::string to_string();
            };
            struct b6 {
                std::string t0;
                b6(std::string _t0) {
                    t0 = _t0;
                }
                static b6* parse();
                std::string to_string();
            };
            struct b7 {
                std::string t0;
                b7(std::string _t0) {
                    t0 = _t0;
                }
                static b7* parse();
                std::string to_string();
            };
            struct b8 {
                std::string t0;
                b8(std::string _t0) {
                    t0 = _t0;
                }
                static b8* parse();
                std::string to_string();
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
            bool is_b7 = false;
            b7 *t7;
            bool is_b8 = false;
            b8 *t8;
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
            a0(b7 *_t7) {
                is_b7 = true;
                t7 = _t7;
            }
            a0(b8 *_t8) {
                is_b8 = true;
                t8 = _t8;
            }
            static a0* parse();
            std::string to_string();
        };
        std::string t0;
        a0 *t1;
        escape(std::string _t0, a0 *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static escape* parse();
        std::string to_string();
    };

    // symbol = "[" | "]" | "{" | "}" | "(" | ")" | "<" | ">" | "=" | "|" | "." | "," | ";" | "-" | "+" | "_" | "*" | "?" | ":" | "!" | "@" | "#" | "$" | "%" | "^" | "&" | "/" | "~" | "`" ;
    struct symbol {
        struct a0 {
            std::string t0;
            a0(std::string _t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            std::string t0;
            a1(std::string _t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            std::string t0;
            a2(std::string _t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            std::string t0;
            a3(std::string _t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string();
        };
        struct a4 {
            std::string t0;
            a4(std::string _t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string();
        };
        struct a5 {
            std::string t0;
            a5(std::string _t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string();
        };
        struct a6 {
            std::string t0;
            a6(std::string _t0) {
                t0 = _t0;
            }
            static a6* parse();
            std::string to_string();
        };
        struct a7 {
            std::string t0;
            a7(std::string _t0) {
                t0 = _t0;
            }
            static a7* parse();
            std::string to_string();
        };
        struct a8 {
            std::string t0;
            a8(std::string _t0) {
                t0 = _t0;
            }
            static a8* parse();
            std::string to_string();
        };
        struct a9 {
            std::string t0;
            a9(std::string _t0) {
                t0 = _t0;
            }
            static a9* parse();
            std::string to_string();
        };
        struct a10 {
            std::string t0;
            a10(std::string _t0) {
                t0 = _t0;
            }
            static a10* parse();
            std::string to_string();
        };
        struct a11 {
            std::string t0;
            a11(std::string _t0) {
                t0 = _t0;
            }
            static a11* parse();
            std::string to_string();
        };
        struct a12 {
            std::string t0;
            a12(std::string _t0) {
                t0 = _t0;
            }
            static a12* parse();
            std::string to_string();
        };
        struct a13 {
            std::string t0;
            a13(std::string _t0) {
                t0 = _t0;
            }
            static a13* parse();
            std::string to_string();
        };
        struct a14 {
            std::string t0;
            a14(std::string _t0) {
                t0 = _t0;
            }
            static a14* parse();
            std::string to_string();
        };
        struct a15 {
            std::string t0;
            a15(std::string _t0) {
                t0 = _t0;
            }
            static a15* parse();
            std::string to_string();
        };
        struct a16 {
            std::string t0;
            a16(std::string _t0) {
                t0 = _t0;
            }
            static a16* parse();
            std::string to_string();
        };
        struct a17 {
            std::string t0;
            a17(std::string _t0) {
                t0 = _t0;
            }
            static a17* parse();
            std::string to_string();
        };
        struct a18 {
            std::string t0;
            a18(std::string _t0) {
                t0 = _t0;
            }
            static a18* parse();
            std::string to_string();
        };
        struct a19 {
            std::string t0;
            a19(std::string _t0) {
                t0 = _t0;
            }
            static a19* parse();
            std::string to_string();
        };
        struct a20 {
            std::string t0;
            a20(std::string _t0) {
                t0 = _t0;
            }
            static a20* parse();
            std::string to_string();
        };
        struct a21 {
            std::string t0;
            a21(std::string _t0) {
                t0 = _t0;
            }
            static a21* parse();
            std::string to_string();
        };
        struct a22 {
            std::string t0;
            a22(std::string _t0) {
                t0 = _t0;
            }
            static a22* parse();
            std::string to_string();
        };
        struct a23 {
            std::string t0;
            a23(std::string _t0) {
                t0 = _t0;
            }
            static a23* parse();
            std::string to_string();
        };
        struct a24 {
            std::string t0;
            a24(std::string _t0) {
                t0 = _t0;
            }
            static a24* parse();
            std::string to_string();
        };
        struct a25 {
            std::string t0;
            a25(std::string _t0) {
                t0 = _t0;
            }
            static a25* parse();
            std::string to_string();
        };
        struct a26 {
            std::string t0;
            a26(std::string _t0) {
                t0 = _t0;
            }
            static a26* parse();
            std::string to_string();
        };
        struct a27 {
            std::string t0;
            a27(std::string _t0) {
                t0 = _t0;
            }
            static a27* parse();
            std::string to_string();
        };
        struct a28 {
            std::string t0;
            a28(std::string _t0) {
                t0 = _t0;
            }
            static a28* parse();
            std::string to_string();
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
        std::string to_string();
    };

    // line_comment = "//" , { alpha | digit | symbol | "\"" | "'" | " " } , "\n" ;
    struct line_comment {
        struct a0 {
            struct b0 {
                alpha *t0;
                b0(alpha *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                digit *t0;
                b1(digit *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            struct b2 {
                symbol *t0;
                b2(symbol *_t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string();
            };
            struct b3 {
                std::string t0;
                b3(std::string _t0) {
                    t0 = _t0;
                }
                static b3* parse();
                std::string to_string();
            };
            struct b4 {
                std::string t0;
                b4(std::string _t0) {
                    t0 = _t0;
                }
                static b4* parse();
                std::string to_string();
            };
            struct b5 {
                std::string t0;
                b5(std::string _t0) {
                    t0 = _t0;
                }
                static b5* parse();
                std::string to_string();
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
            static a0* parse();
            std::string to_string();
        };
        std::string t0;
        std::vector<a0*> t1;
        std::string t2;
        line_comment(std::string _t0, std::vector<a0*> _t1, std::string _t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static line_comment* parse();
        std::string to_string();
    };

    // multiline_comment = "/*" , { alpha | digit | " " | "\n" } , "*/" ;
    struct multiline_comment {
        struct a0 {
            struct b0 {
                alpha *t0;
                b0(alpha *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                digit *t0;
                b1(digit *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            struct b2 {
                std::string t0;
                b2(std::string _t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string();
            };
            struct b3 {
                std::string t0;
                b3(std::string _t0) {
                    t0 = _t0;
                }
                static b3* parse();
                std::string to_string();
            };
            bool is_b0 = false;
            b0 *t0;
            bool is_b1 = false;
            b1 *t1;
            bool is_b2 = false;
            b2 *t2;
            bool is_b3 = false;
            b3 *t3;
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
            static a0* parse();
            std::string to_string();
        };
        std::string t0;
        std::vector<a0*> t1;
        std::string t2;
        multiline_comment(std::string _t0, std::vector<a0*> _t1, std::string _t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static multiline_comment* parse();
        std::string to_string();
    };

    // ws = " " | "\n" | "\t" | "\r" | "\f" | "\b" | line_comment | multiline_comment ;
    struct ws {
        struct a0 {
            std::string t0;
            a0(std::string _t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            std::string t0;
            a1(std::string _t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            std::string t0;
            a2(std::string _t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            std::string t0;
            a3(std::string _t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string();
        };
        struct a4 {
            std::string t0;
            a4(std::string _t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string();
        };
        struct a5 {
            std::string t0;
            a5(std::string _t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string();
        };
        struct a6 {
            line_comment *t0;
            a6(line_comment *_t0) {
                t0 = _t0;
            }
            static a6* parse();
            std::string to_string();
        };
        struct a7 {
            multiline_comment *t0;
            a7(multiline_comment *_t0) {
                t0 = _t0;
            }
            static a7* parse();
            std::string to_string();
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
        ws(a7 *_t7) {
            is_a7 = true;
            t7 = _t7;
        }
        static ws* parse();
        std::string to_string();
    };

    // rws = < ws > ;
    struct rws {
        struct a0 {
            ws *t0;
            a0(ws *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        std::vector<a0*> t0;
        rws(std::vector<a0*> _t0) {
            t0 = _t0;
        }
        static rws* parse();
        std::string to_string();
    };

    // ows = { ws } ;
    struct ows {
        struct a0 {
            ws *t0;
            a0(ws *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        std::vector<a0*> t0;
        ows(std::vector<a0*> _t0) {
            t0 = _t0;
        }
        static ows* parse();
        std::string to_string();
    };

    // type = base_type , { "*" | "&" } ;
    struct type {
        struct a0 {
            struct b0 {
                std::string t0;
                b0(std::string _t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                std::string t0;
                b1(std::string _t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            bool is_b0 = false;
            b0 *t0;
            bool is_b1 = false;
            b1 *t1;
            a0(b0 *_t0) {
                is_b0 = true;
                t0 = _t0;
            }
            a0(b1 *_t1) {
                is_b1 = true;
                t1 = _t1;
            }
            static a0* parse();
            std::string to_string();
        };
        base_type *t0;
        std::vector<a0*> t1;
        type(base_type *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static type* parse();
        std::string to_string();
    };

    // base_type = alpha , { alpha | digit | "_" } ;
    struct base_type {
        struct a0 {
            struct b0 {
                alpha *t0;
                b0(alpha *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                digit *t0;
                b1(digit *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            struct b2 {
                std::string t0;
                b2(std::string _t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string();
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
            std::string to_string();
        };
        alpha *t0;
        std::vector<a0*> t1;
        base_type(alpha *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static base_type* parse();
        std::string to_string();
    };

    // identifier = ( alpha | "_" ) , { alpha | digit | "_" } ;
    struct identifier {
        struct a0 {
            struct b0 {
                alpha *t0;
                b0(alpha *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                std::string t0;
                b1(std::string _t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            bool is_b0 = false;
            b0 *t0;
            bool is_b1 = false;
            b1 *t1;
            a0(b0 *_t0) {
                is_b0 = true;
                t0 = _t0;
            }
            a0(b1 *_t1) {
                is_b1 = true;
                t1 = _t1;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            struct b0 {
                alpha *t0;
                b0(alpha *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                digit *t0;
                b1(digit *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            struct b2 {
                std::string t0;
                b2(std::string _t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string();
            };
            bool is_b0 = false;
            b0 *t0;
            bool is_b1 = false;
            b1 *t1;
            bool is_b2 = false;
            b2 *t2;
            a1(b0 *_t0) {
                is_b0 = true;
                t0 = _t0;
            }
            a1(b1 *_t1) {
                is_b1 = true;
                t1 = _t1;
            }
            a1(b2 *_t2) {
                is_b2 = true;
                t2 = _t2;
            }
            static a1* parse();
            std::string to_string();
        };
        a0 *t0;
        std::vector<a1*> t1;
        identifier(a0 *_t0, std::vector<a1*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static identifier* parse();
        std::string to_string();
    };

    // declaration = type , rws , identifier , ows , "=" , ows , expression ;
    struct declaration {
        type *t0;
        rws *t1;
        identifier *t2;
        ows *t3;
        std::string t4;
        ows *t5;
        expression *t6;
        declaration(type *_t0, rws *_t1, identifier *_t2, ows *_t3, std::string _t4, ows *_t5, expression *_t6) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
            t5 = _t5;
            t6 = _t6;
        }
        static declaration* parse();
        std::string to_string();
    };

    // parameter = type , rws , identifier ;
    struct parameter {
        type *t0;
        rws *t1;
        identifier *t2;
        parameter(type *_t0, rws *_t1, identifier *_t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static parameter* parse();
        std::string to_string();
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
                std::string to_string();
            };
            parameter *t0;
            std::vector<b0*> t1;
            a0(parameter *_t0, std::vector<b0*> _t1) {
                t0 = _t0;
                t1 = _t1;
            }
            static a0* parse();
            std::string to_string();
        };
        a0 *t0;
        parameter_list(a0 *_t0) {
            t0 = _t0;
        }
        static parameter_list* parse();
        std::string to_string();
    };

    // argument_list = [ expression , { ows , "," , ows , expression } ] ;
    struct argument_list {
        struct a0 {
            struct b0 {
                ows *t0;
                std::string t1;
                ows *t2;
                expression *t3;
                b0(ows *_t0, std::string _t1, ows *_t2, expression *_t3) {
                    t0 = _t0;
                    t1 = _t1;
                    t2 = _t2;
                    t3 = _t3;
                }
                static b0* parse();
                std::string to_string();
            };
            expression *t0;
            std::vector<b0*> t1;
            a0(expression *_t0, std::vector<b0*> _t1) {
                t0 = _t0;
                t1 = _t1;
            }
            static a0* parse();
            std::string to_string();
        };
        a0 *t0;
        argument_list(a0 *_t0) {
            t0 = _t0;
        }
        static argument_list* parse();
        std::string to_string();
    };

    // statement = simple_statement | control_statement | compound_statement ;
    struct statement {
        struct a0 {
            simple_statement *t0;
            a0(simple_statement *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            control_statement *t0;
            a1(control_statement *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            compound_statement *t0;
            a2(compound_statement *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        bool is_a2 = false;
        a2 *t2;
        statement(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        statement(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        statement(a2 *_t2) {
            is_a2 = true;
            t2 = _t2;
        }
        static statement* parse();
        std::string to_string();
    };

    // simple_statement = "return" , [ rws , expression ] , ows , ";" | declaration , ows , ";" | expression , ows , ";" ;
    struct simple_statement {
        struct a0 {
            struct b0 {
                rws *t0;
                expression *t1;
                b0(rws *_t0, expression *_t1) {
                    t0 = _t0;
                    t1 = _t1;
                }
                static b0* parse();
                std::string to_string();
            };
            std::string t0;
            b0 *t1;
            ows *t2;
            std::string t3;
            a0(std::string _t0, b0 *_t1, ows *_t2, std::string _t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            declaration *t0;
            ows *t1;
            std::string t2;
            a1(declaration *_t0, ows *_t1, std::string _t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            expression *t0;
            ows *t1;
            std::string t2;
            a2(expression *_t0, ows *_t1, std::string _t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a2* parse();
            std::string to_string();
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        bool is_a2 = false;
        a2 *t2;
        simple_statement(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        simple_statement(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        simple_statement(a2 *_t2) {
            is_a2 = true;
            t2 = _t2;
        }
        static simple_statement* parse();
        std::string to_string();
    };

    // control_statement = "if" , ows , "(" , ows , expression , ows , ")" , ows , statement , [ rws , "else" , rws , statement ] | "while" , ows , "(" , ows , expression , ows , ")" , ows , statement | "for" , ows , "(" , ows , [ declaration ] , ows , ";" , ows , [ expression ] , ows , ";" , ows , [ expression ] , ows , ")" , ows , statement ;
    struct control_statement {
        struct a0 {
            struct b0 {
                rws *t0;
                std::string t1;
                rws *t2;
                statement *t3;
                b0(rws *_t0, std::string _t1, rws *_t2, statement *_t3) {
                    t0 = _t0;
                    t1 = _t1;
                    t2 = _t2;
                    t3 = _t3;
                }
                static b0* parse();
                std::string to_string();
            };
            std::string t0;
            ows *t1;
            std::string t2;
            ows *t3;
            expression *t4;
            ows *t5;
            std::string t6;
            ows *t7;
            statement *t8;
            b0 *t9;
            a0(std::string _t0, ows *_t1, std::string _t2, ows *_t3, expression *_t4, ows *_t5, std::string _t6, ows *_t7, statement *_t8, b0 *_t9) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
                t4 = _t4;
                t5 = _t5;
                t6 = _t6;
                t7 = _t7;
                t8 = _t8;
                t9 = _t9;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            std::string t0;
            ows *t1;
            std::string t2;
            ows *t3;
            expression *t4;
            ows *t5;
            std::string t6;
            ows *t7;
            statement *t8;
            a1(std::string _t0, ows *_t1, std::string _t2, ows *_t3, expression *_t4, ows *_t5, std::string _t6, ows *_t7, statement *_t8) {
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
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            struct b0 {
                declaration *t0;
                b0(declaration *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                expression *t0;
                b1(expression *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string();
            };
            struct b2 {
                expression *t0;
                b2(expression *_t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string();
            };
            std::string t0;
            ows *t1;
            std::string t2;
            ows *t3;
            b0 *t4;
            ows *t5;
            std::string t6;
            ows *t7;
            b1 *t8;
            ows *t9;
            std::string t10;
            ows *t11;
            b2 *t12;
            ows *t13;
            std::string t14;
            ows *t15;
            statement *t16;
            a2(std::string _t0, ows *_t1, std::string _t2, ows *_t3, b0 *_t4, ows *_t5, std::string _t6, ows *_t7, b1 *_t8, ows *_t9, std::string _t10, ows *_t11, b2 *_t12, ows *_t13, std::string _t14, ows *_t15, statement *_t16) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
                t4 = _t4;
                t5 = _t5;
                t6 = _t6;
                t7 = _t7;
                t8 = _t8;
                t9 = _t9;
                t10 = _t10;
                t11 = _t11;
                t12 = _t12;
                t13 = _t13;
                t14 = _t14;
                t15 = _t15;
                t16 = _t16;
            }
            static a2* parse();
            std::string to_string();
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        bool is_a2 = false;
        a2 *t2;
        control_statement(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        control_statement(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        control_statement(a2 *_t2) {
            is_a2 = true;
            t2 = _t2;
        }
        static control_statement* parse();
        std::string to_string();
    };

    // compound_statement = "{" , ows , { statement , ows } , "}" ;
    struct compound_statement {
        struct a0 {
            statement *t0;
            ows *t1;
            a0(statement *_t0, ows *_t1) {
                t0 = _t0;
                t1 = _t1;
            }
            static a0* parse();
            std::string to_string();
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
        std::string to_string();
    };

    // program = { ows , ( function | struct_definition ) } , ows ;
    struct program {
        struct a0 {
            struct b0 {
                struct c0 {
                    function *t0;
                    c0(function *_t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string();
                };
                struct c1 {
                    struct_definition *t0;
                    c1(struct_definition *_t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string();
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
                std::string to_string();
            };
            ows *t0;
            b0 *t1;
            a0(ows *_t0, b0 *_t1) {
                t0 = _t0;
                t1 = _t1;
            }
            static a0* parse();
            std::string to_string();
        };
        std::vector<a0*> t0;
        ows *t1;
        program(std::vector<a0*> _t0, ows *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static program* parse();
        std::string to_string();
    };

    overloadable_operator::a0* overloadable_operator::a0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "+") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(1);
        if(_t0 != "-") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(1);
        if(_t0 != "*") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(1);
        if(_t0 != "/") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(1);
        if(_t0 != "%") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(1);
        if(_t0 != "&") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(1);
        if(_t0 != "|") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(1);
        if(_t0 != "^") {pop_stack(); return nullptr;}
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
        if(_t0 != "<<") {pop_stack(); return nullptr;}
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
        if(_t0 != ">>") {pop_stack(); return nullptr;}
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
        if(_t0 != "==") {pop_stack(); return nullptr;}
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
        if(_t0 != "!=") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(1);
        if(_t0 != "<") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(2);
        if(_t0 != "<=") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(1);
        if(_t0 != ">") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(2);
        if(_t0 != ">=") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(3);
        if(_t0 != "++x") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(3);
        if(_t0 != "--x") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(3);
        if(_t0 != "x++") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(3);
        if(_t0 != "x--") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(2);
        if(_t0 != "[]") {pop_stack(); return nullptr;}
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
        if(_t0 != "=") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(2);
        if(_t0 != "*x") {pop_stack(); return nullptr;}
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
        std::string _t0 = next_chars(6);
        if(_t0 != "(cast)") {pop_stack(); return nullptr;}
        rm_stack();
        return new overloadable_operator::a23(_t0);
    }

    std::string overloadable_operator::a23::to_string() {
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
        assert(false);
    }

    overload_identifier* overload_identifier::parse() {
        push_stack();
        std::string _t0 = next_chars(8);
        if(_t0 != "operator") {pop_stack(); return nullptr;}
        overloadable_operator *_t1 = overloadable_operator::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new overload_identifier(_t0, _t1);
    }

    std::string overload_identifier::to_string() {
        std::string ans = "";
        ans += t0;
        ans += t1->to_string();
        return ans;
    }

    function_identifier::a0* function_identifier::a0::parse() {
        push_stack();
        overload_identifier *_t0 = overload_identifier::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new function_identifier::a0(_t0);
    }

    std::string function_identifier::a0::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    function_identifier::a1* function_identifier::a1::parse() {
        push_stack();
        identifier *_t0 = identifier::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new function_identifier::a1(_t0);
    }

    std::string function_identifier::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    function_identifier* function_identifier::parse() {
        if(auto x = function_identifier::a0::parse()) return new function_identifier(x);
        if(auto x = function_identifier::a1::parse()) return new function_identifier(x);
        return nullptr;
    }

    std::string function_identifier::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        assert(false);
    }

    function_definition* function_definition::parse() {
        push_stack();
        type *_t0 = type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rws *_t1 = rws::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        function_identifier *_t2 = function_identifier::parse();
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

    literal::a0* literal::a0::parse() {
        push_stack();
        literal_integer *_t0 = literal_integer::parse();
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
        literal_sizeof *_t0 = literal_sizeof::parse();
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
        literal_char *_t0 = literal_char::parse();
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
        literal_string *_t0 = literal_string::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new literal::a3(_t0);
    }

    std::string literal::a3::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        return ans;
    }

    literal* literal::parse() {
        if(auto x = literal::a0::parse()) return new literal(x);
        if(auto x = literal::a1::parse()) return new literal(x);
        if(auto x = literal::a2::parse()) return new literal(x);
        if(auto x = literal::a3::parse()) return new literal(x);
        return nullptr;
    }

    std::string literal::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
        assert(false);
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
        function_call *_t0 = function_call::parse();
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
        identifier *_t0 = identifier::parse();
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
        return new expr_primary::a3(_t0, _t1, _t2, _t3, _t4);
    }

    std::string expr_primary::a3::to_string() {
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
        return nullptr;
    }

    std::string expr_primary::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
        if(is_a3) return t3->to_string();
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

    expr_unary::a0::b0* expr_unary::a0::b0::parse() {
        if(auto x = expr_unary::a0::b0::c0::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c1::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c2::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c3::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c4::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c5::parse()) return new expr_unary::a0::b0(x);
        if(auto x = expr_unary::a0::b0::c6::parse()) return new expr_unary::a0::b0(x);
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

    struct_definition::a0::b0* struct_definition::a0::b0::parse() {
        if(auto x = struct_definition::a0::b0::c0::parse()) return new struct_definition::a0::b0(x);
        if(auto x = struct_definition::a0::b0::c1::parse()) return new struct_definition::a0::b0(x);
        return nullptr;
    }

    std::string struct_definition::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
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

    line_comment::a0* line_comment::a0::parse() {
        if(auto x = line_comment::a0::b0::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b1::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b2::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b3::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b4::parse()) return new line_comment::a0(x);
        if(auto x = line_comment::a0::b5::parse()) return new line_comment::a0(x);
        return nullptr;
    }

    std::string line_comment::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        if(is_b2) return t2->to_string();
        if(is_b3) return t3->to_string();
        if(is_b4) return t4->to_string();
        if(is_b5) return t5->to_string();
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

    type::a0::b0* type::a0::b0::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "*") {pop_stack(); return nullptr;}
        rm_stack();
        return new type::a0::b0(_t0);
    }

    std::string type::a0::b0::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    type::a0::b1* type::a0::b1::parse() {
        push_stack();
        std::string _t0 = next_chars(1);
        if(_t0 != "&") {pop_stack(); return nullptr;}
        rm_stack();
        return new type::a0::b1(_t0);
    }

    std::string type::a0::b1::to_string() {
        std::string ans = "";
        ans += t0;
        return ans;
    }

    type::a0* type::a0::parse() {
        if(auto x = type::a0::b0::parse()) return new type::a0(x);
        if(auto x = type::a0::b1::parse()) return new type::a0(x);
        return nullptr;
    }

    std::string type::a0::to_string() {
        if(is_b0) return t0->to_string();
        if(is_b1) return t1->to_string();
        assert(false);
    }

    type* type::parse() {
        push_stack();
        base_type *_t0 = base_type::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        std::vector<type::a0*> _t1;
        while(true) {
            type::a0 *tmp = type::a0::parse();
            if(tmp == nullptr) break;
            _t1.push_back(tmp);
        }
        rm_stack();
        return new type(_t0, _t1);
    }

    std::string type::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        for(int i = 0; i < t1.size(); i++) ans += t1[i]->to_string();
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

    declaration* declaration::parse() {
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
        if(_t4 != "=") {pop_stack(); return nullptr;}
        ows *_t5 = ows::parse();
        if(_t5 == nullptr) {pop_stack(); return nullptr;}
        expression *_t6 = expression::parse();
        if(_t6 == nullptr) {pop_stack(); return nullptr;}
        rm_stack();
        return new declaration(_t0, _t1, _t2, _t3, _t4, _t5, _t6);
    }

    std::string declaration::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2->to_string();
        ans += t3->to_string();
        ans += t4;
        ans += t5->to_string();
        ans += t6->to_string();
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
        declaration *_t0 = declaration::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new simple_statement::a1(_t0, _t1, _t2);
    }

    std::string simple_statement::a1::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2;
        return ans;
    }

    simple_statement::a2* simple_statement::a2::parse() {
        push_stack();
        expression *_t0 = expression::parse();
        if(_t0 == nullptr) {pop_stack(); return nullptr;}
        ows *_t1 = ows::parse();
        if(_t1 == nullptr) {pop_stack(); return nullptr;}
        std::string _t2 = next_chars(1);
        if(_t2 != ";") {pop_stack(); return nullptr;}
        rm_stack();
        return new simple_statement::a2(_t0, _t1, _t2);
    }

    std::string simple_statement::a2::to_string() {
        std::string ans = "";
        ans += t0->to_string();
        ans += t1->to_string();
        ans += t2;
        return ans;
    }

    simple_statement* simple_statement::parse() {
        if(auto x = simple_statement::a0::parse()) return new simple_statement(x);
        if(auto x = simple_statement::a1::parse()) return new simple_statement(x);
        if(auto x = simple_statement::a2::parse()) return new simple_statement(x);
        return nullptr;
    }

    std::string simple_statement::to_string() {
        if(is_a0) return t0->to_string();
        if(is_a1) return t1->to_string();
        if(is_a2) return t2->to_string();
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

    program::a0::b0* program::a0::b0::parse() {
        if(auto x = program::a0::b0::c0::parse()) return new program::a0::b0(x);
        if(auto x = program::a0::b0::c1::parse()) return new program::a0::b0(x);
        return nullptr;
    }

    std::string program::a0::b0::to_string() {
        if(is_c0) return t0->to_string();
        if(is_c1) return t1->to_string();
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
