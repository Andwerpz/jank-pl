// Date Generated : 07-03-2025 20:05:33
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
    void set_s(std::string& ns);
    bool check_finished_parsing();

    struct function_definition;
    struct function;
    struct templated_function;
    struct function_call;
    struct literal_sizeof;
    struct literal_integer;
    struct literal_float;
    struct literal_char;
    struct literal_string;
    struct literal;
    struct member_variable_declaration;
    struct constructor_definition;
    struct constructor;
    struct constructor_call;
    struct struct_definition;
    struct templated_struct_definition;
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
    struct overloadable_operator;
    struct overload_definition;
    struct overload;
    struct templated_overload;
    struct alpha;
    struct digit;
    struct escape;
    struct symbol;
    struct line_comment;
    struct multiline_comment;
    struct ws;
    struct rws;
    struct ows;
    struct base_type;
    struct templated_type;
    struct type;
    struct template_header;
    struct identifier;
    struct declaration;
    struct parameter;
    struct parameter_list;
    struct argument_list;
    struct statement;
    struct simple_statement;
    struct control_statement;
    struct compound_statement;
    struct include;
    struct global_declaration;
    struct program;

    // function_definition = type , rws , identifier , ows , "(" , ows , parameter_list , ows , ")" ;
    struct function_definition {
        type *t0;
        rws *t1;
        identifier *t2;
        ows *t3;
        std::string t4;
        ows *t5;
        parameter_list *t6;
        ows *t7;
        std::string t8;
        function_definition(type *_t0, rws *_t1, identifier *_t2, ows *_t3, std::string _t4, ows *_t5, parameter_list *_t6, ows *_t7, std::string _t8) {
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

    // templated_function = template_header , ows , function ;
    struct templated_function {
        template_header *t0;
        ows *t1;
        function *t2;
        templated_function(template_header *_t0, ows *_t1, function *_t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static templated_function* parse();
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

    // literal = literal_float | literal_integer | literal_sizeof | literal_char | literal_string ;
    struct literal {
        struct a0 {
            literal_float *t0;
            a0(literal_float *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        struct a1 {
            literal_integer *t0;
            a1(literal_integer *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            literal_sizeof *t0;
            a2(literal_sizeof *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            literal_char *t0;
            a3(literal_char *_t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string();
        };
        struct a4 {
            literal_string *t0;
            a4(literal_string *_t0) {
                t0 = _t0;
            }
            static a4* parse();
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
        literal(a4 *_t4) {
            is_a4 = true;
            t4 = _t4;
        }
        static literal* parse();
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

    // constructor_definition = base_type , ows , "(" , ows , parameter_list , ows , ")" ;
    struct constructor_definition {
        base_type *t0;
        ows *t1;
        std::string t2;
        ows *t3;
        parameter_list *t4;
        ows *t5;
        std::string t6;
        constructor_definition(base_type *_t0, ows *_t1, std::string _t2, ows *_t3, parameter_list *_t4, ows *_t5, std::string _t6) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
            t5 = _t5;
            t6 = _t6;
        }
        static constructor_definition* parse();
        std::string to_string();
    };

    // constructor = constructor_definition , ows , compound_statement ;
    struct constructor {
        constructor_definition *t0;
        ows *t1;
        compound_statement *t2;
        constructor(constructor_definition *_t0, ows *_t1, compound_statement *_t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static constructor* parse();
        std::string to_string();
    };

    // constructor_call = "new" , rws , type , ows , "(" , ows , argument_list , ows , ")" ;
    struct constructor_call {
        std::string t0;
        rws *t1;
        type *t2;
        ows *t3;
        std::string t4;
        ows *t5;
        argument_list *t6;
        ows *t7;
        std::string t8;
        constructor_call(std::string _t0, rws *_t1, type *_t2, ows *_t3, std::string _t4, ows *_t5, argument_list *_t6, ows *_t7, std::string _t8) {
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
        static constructor_call* parse();
        std::string to_string();
    };

    // struct_definition = "struct" , ows , base_type , ows , "{" , ows , { ( member_variable_declaration | function | constructor ) , ows } , "}" ;
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
                struct c2 {
                    constructor *t0;
                    c2(constructor *_t0) {
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

    // templated_struct_definition = template_header , ows , struct_definition ;
    struct templated_struct_definition {
        template_header *t0;
        ows *t1;
        struct_definition *t2;
        templated_struct_definition(template_header *_t0, ows *_t1, struct_definition *_t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static templated_struct_definition* parse();
        std::string to_string();
    };

    // expr_primary = literal | constructor_call | function_call | identifier | "(" , ows , expression , ows , ")" ;
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
            constructor_call *t0;
            a1(constructor_call *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            function_call *t0;
            a2(function_call *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            identifier *t0;
            a3(identifier *_t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string();
        };
        struct a4 {
            std::string t0;
            ows *t1;
            expression *t2;
            ows *t3;
            std::string t4;
            a4(std::string _t0, ows *_t1, expression *_t2, ows *_t3, std::string _t4) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
                t4 = _t4;
            }
            static a4* parse();
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
        expr_primary(a4 *_t4) {
            is_a4 = true;
            t4 = _t4;
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

    // expr_unary = ( "++" | "--" | "+" | "-" | "~" | "!" | "*" | "@" | "(" , type , ")" ) , ows , expr_unary | expr_postfix ;
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
                    type *t1;
                    std::string t2;
                    c8(std::string _t0, type *_t1, std::string _t2) {
                        t0 = _t0;
                        t1 = _t1;
                        t2 = _t2;
                    }
                    static c8* parse();
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

    // overloadable_operator = "++x" | "--x" | "x++" | "x--" | "*x" | "+" | "-" | "*" | "/" | "%" | "&" | "|" | "^" | "<<" | ">>" | "==" | "!=" | "<" | "<=" | ">" | ">=" | "[]" | "=" | "(cast)" ;
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

    // overload_definition = type , rws , "operator" , overloadable_operator , ows , "(" , ows , parameter_list , ows , ")" ;
    struct overload_definition {
        type *t0;
        rws *t1;
        std::string t2;
        overloadable_operator *t3;
        ows *t4;
        std::string t5;
        ows *t6;
        parameter_list *t7;
        ows *t8;
        std::string t9;
        overload_definition(type *_t0, rws *_t1, std::string _t2, overloadable_operator *_t3, ows *_t4, std::string _t5, ows *_t6, parameter_list *_t7, ows *_t8, std::string _t9) {
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
        static overload_definition* parse();
        std::string to_string();
    };

    // overload = overload_definition , ows , compound_statement ;
    struct overload {
        overload_definition *t0;
        ows *t1;
        compound_statement *t2;
        overload(overload_definition *_t0, ows *_t1, compound_statement *_t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static overload* parse();
        std::string to_string();
    };

    // templated_overload = template_header , ows , overload ;
    struct templated_overload {
        template_header *t0;
        ows *t1;
        overload *t2;
        templated_overload(template_header *_t0, ows *_t1, overload *_t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static templated_overload* parse();
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

    // templated_type = base_type , [ "<" , ows , templated_type , { ows , "," , ows , templated_type } , ows , ">" ] , { "*" } ;
    struct templated_type {
        struct a0 {
            struct b0 {
                ows *t0;
                std::string t1;
                ows *t2;
                templated_type *t3;
                b0(ows *_t0, std::string _t1, ows *_t2, templated_type *_t3) {
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
            templated_type *t2;
            std::vector<b0*> t3;
            ows *t4;
            std::string t5;
            a0(std::string _t0, ows *_t1, templated_type *_t2, std::vector<b0*> _t3, ows *_t4, std::string _t5) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
                t4 = _t4;
                t5 = _t5;
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
        base_type *t0;
        a0 *t1;
        std::vector<a1*> t2;
        templated_type(base_type *_t0, a0 *_t1, std::vector<a1*> _t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static templated_type* parse();
        std::string to_string();
    };

    // type = templated_type , [ "&" ] ;
    struct type {
        struct a0 {
            std::string t0;
            a0(std::string _t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string();
        };
        templated_type *t0;
        a0 *t1;
        type(templated_type *_t0, a0 *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static type* parse();
        std::string to_string();
    };

    // template_header = "template" , ows , "<" , ows , base_type , { ows , "," , ows , base_type } , ows , ">" ;
    struct template_header {
        struct a0 {
            ows *t0;
            std::string t1;
            ows *t2;
            base_type *t3;
            a0(ows *_t0, std::string _t1, ows *_t2, base_type *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a0* parse();
            std::string to_string();
        };
        std::string t0;
        ows *t1;
        std::string t2;
        ows *t3;
        base_type *t4;
        std::vector<a0*> t5;
        ows *t6;
        std::string t7;
        template_header(std::string _t0, ows *_t1, std::string _t2, ows *_t3, base_type *_t4, std::vector<a0*> _t5, ows *_t6, std::string _t7) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
            t5 = _t5;
            t6 = _t6;
            t7 = _t7;
        }
        static template_header* parse();
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

    // simple_statement = "return" , [ rws , expression ] , ows , ";" | "break" , ows , ";" | "continue" , ows , ";" | declaration , ows , ";" | expression , ows , ";" | "asm!" , ows , "(" , ows , literal_string , ows , ")" , ows , ";" ;
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
            std::string t0;
            ows *t1;
            std::string t2;
            a1(std::string _t0, ows *_t1, std::string _t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a1* parse();
            std::string to_string();
        };
        struct a2 {
            std::string t0;
            ows *t1;
            std::string t2;
            a2(std::string _t0, ows *_t1, std::string _t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a2* parse();
            std::string to_string();
        };
        struct a3 {
            declaration *t0;
            ows *t1;
            std::string t2;
            a3(declaration *_t0, ows *_t1, std::string _t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a3* parse();
            std::string to_string();
        };
        struct a4 {
            expression *t0;
            ows *t1;
            std::string t2;
            a4(expression *_t0, ows *_t1, std::string _t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a4* parse();
            std::string to_string();
        };
        struct a5 {
            std::string t0;
            ows *t1;
            std::string t2;
            ows *t3;
            literal_string *t4;
            ows *t5;
            std::string t6;
            ows *t7;
            std::string t8;
            a5(std::string _t0, ows *_t1, std::string _t2, ows *_t3, literal_string *_t4, ows *_t5, std::string _t6, ows *_t7, std::string _t8) {
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
            static a5* parse();
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
        simple_statement(a3 *_t3) {
            is_a3 = true;
            t3 = _t3;
        }
        simple_statement(a4 *_t4) {
            is_a4 = true;
            t4 = _t4;
        }
        simple_statement(a5 *_t5) {
            is_a5 = true;
            t5 = _t5;
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

    // include = "#include" , rws , ( literal_string | "<" , identifier , ">" ) , ows , ";" ;
    struct include {
        struct a0 {
            struct b0 {
                literal_string *t0;
                b0(literal_string *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string();
            };
            struct b1 {
                std::string t0;
                identifier *t1;
                std::string t2;
                b1(std::string _t0, identifier *_t1, std::string _t2) {
                    t0 = _t0;
                    t1 = _t1;
                    t2 = _t2;
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
        std::string t0;
        rws *t1;
        a0 *t2;
        ows *t3;
        std::string t4;
        include(std::string _t0, rws *_t1, a0 *_t2, ows *_t3, std::string _t4) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
        }
        static include* parse();
        std::string to_string();
    };

    // global_declaration = "[" , ows , [ "-" ] , < digit > , ows , "]" , ows , declaration , ows , ";" ;
    struct global_declaration {
        struct a0 {
            std::string t0;
            a0(std::string _t0) {
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
        std::string t0;
        ows *t1;
        a0 *t2;
        std::vector<a1*> t3;
        ows *t4;
        std::string t5;
        ows *t6;
        declaration *t7;
        ows *t8;
        std::string t9;
        global_declaration(std::string _t0, ows *_t1, a0 *_t2, std::vector<a1*> _t3, ows *_t4, std::string _t5, ows *_t6, declaration *_t7, ows *_t8, std::string _t9) {
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
        static global_declaration* parse();
        std::string to_string();
    };

    // program = { ows , ( function | struct_definition | templated_function | templated_struct_definition | overload | templated_overload | include | global_declaration ) } , ows ;
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
                struct c2 {
                    templated_function *t0;
                    c2(templated_function *_t0) {
                        t0 = _t0;
                    }
                    static c2* parse();
                    std::string to_string();
                };
                struct c3 {
                    templated_struct_definition *t0;
                    c3(templated_struct_definition *_t0) {
                        t0 = _t0;
                    }
                    static c3* parse();
                    std::string to_string();
                };
                struct c4 {
                    overload *t0;
                    c4(overload *_t0) {
                        t0 = _t0;
                    }
                    static c4* parse();
                    std::string to_string();
                };
                struct c5 {
                    templated_overload *t0;
                    c5(templated_overload *_t0) {
                        t0 = _t0;
                    }
                    static c5* parse();
                    std::string to_string();
                };
                struct c6 {
                    include *t0;
                    c6(include *_t0) {
                        t0 = _t0;
                    }
                    static c6* parse();
                    std::string to_string();
                };
                struct c7 {
                    global_declaration *t0;
                    c7(global_declaration *_t0) {
                        t0 = _t0;
                    }
                    static c7* parse();
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

};
