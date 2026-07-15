// Date Generated : 07-14-2026 23:22:38
#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <optional>
#include <map>

namespace parser {
    struct parse_context;
    struct token;
    struct error;

    void set_s(std::string& ns);
    bool check_finished_parsing(bool prettyprint);
    parse_context get_ctx();
    std::vector<error*> get_errors();
    void set_gen_errors(bool b);

    struct parse_context {
        int ptr;        //where we are in the string
        int line;       //how many lines we are in the string
        int col;        //what column are we in the current line
    };

    struct token {
        // set during parse phase
        parse_context start_ctx;    // inclusive
        parse_context end_ctx;      // exclusive

        // set during postprocess phase
        std::string token_type;
        std::vector<token*> token_children;
        
        virtual void postprocess() = 0;
        virtual std::string to_string() = 0;
    };
    
    struct terminal : public token {
        std::string val;
        terminal(std::string _val) {
            val = _val;
        }
        static terminal* parse(std::string val);
        std::string to_string() override;
        void postprocess() override;
    };

    struct error : public token {
        std::string val;
    };
    

    struct value;
    struct basic_string_char;
    struct basic_string;
    struct nonzero_digit;
    struct unsigned_integer;
    struct integer;
    struct boolean;
    struct value_list;
    struct array;
    struct alpha;
    struct digit;
    struct escape;
    struct symbol;
    struct comment;
    struct ws;
    struct hws;
    struct bare_key;
    struct quoted_key;
    struct key;
    struct table;
    struct key_value;
    struct statement;
    struct toml;

    // value = basic_string | integer | boolean | array ;
    struct value : public token {
        struct a0 : public token {
            basic_string *t0;
            a0(basic_string *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            integer *t0;
            a1(integer *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a2 : public token {
            boolean *t0;
            a2(boolean *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a3 : public token {
            array *t0;
            a3(array *_t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        bool is_a2 = false;
        a2 *t2;
        bool is_a3 = false;
        a3 *t3;
        value(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        value(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        value(a2 *_t2) {
            is_a2 = true;
            t2 = _t2;
        }
        value(a3 *_t3) {
            is_a3 = true;
            t3 = _t3;
        }
        static value* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // basic_string_char = alpha | digit | symbol | escape | " " | "'" ;
    struct basic_string_char : public token {
        struct a0 : public token {
            alpha *t0;
            a0(alpha *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            digit *t0;
            a1(digit *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a2 : public token {
            symbol *t0;
            a2(symbol *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a3 : public token {
            escape *t0;
            a3(escape *_t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a4 : public token {
            terminal *t0;
            a4(terminal *_t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a5 : public token {
            terminal *t0;
            a5(terminal *_t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string() override;
            void postprocess() override;
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
        basic_string_char(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        basic_string_char(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        basic_string_char(a2 *_t2) {
            is_a2 = true;
            t2 = _t2;
        }
        basic_string_char(a3 *_t3) {
            is_a3 = true;
            t3 = _t3;
        }
        basic_string_char(a4 *_t4) {
            is_a4 = true;
            t4 = _t4;
        }
        basic_string_char(a5 *_t5) {
            is_a5 = true;
            t5 = _t5;
        }
        static basic_string_char* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // basic_string = "\"" , { basic_string_char } , "\"" ;
    struct basic_string : public token {
        struct a0 : public token {
            basic_string_char *t0;
            a0(basic_string_char *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        terminal *t0;
        std::vector<a0*> t1;
        terminal *t2;
        basic_string(terminal *_t0, std::vector<a0*> _t1, terminal *_t2) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
        }
        static basic_string* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // nonzero_digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
    struct nonzero_digit : public token {
        struct a0 : public token {
            terminal *t0;
            a0(terminal *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            terminal *t0;
            a1(terminal *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a2 : public token {
            terminal *t0;
            a2(terminal *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a3 : public token {
            terminal *t0;
            a3(terminal *_t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a4 : public token {
            terminal *t0;
            a4(terminal *_t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a5 : public token {
            terminal *t0;
            a5(terminal *_t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a6 : public token {
            terminal *t0;
            a6(terminal *_t0) {
                t0 = _t0;
            }
            static a6* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a7 : public token {
            terminal *t0;
            a7(terminal *_t0) {
                t0 = _t0;
            }
            static a7* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a8 : public token {
            terminal *t0;
            a8(terminal *_t0) {
                t0 = _t0;
            }
            static a8* parse();
            std::string to_string() override;
            void postprocess() override;
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
        nonzero_digit(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        nonzero_digit(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        nonzero_digit(a2 *_t2) {
            is_a2 = true;
            t2 = _t2;
        }
        nonzero_digit(a3 *_t3) {
            is_a3 = true;
            t3 = _t3;
        }
        nonzero_digit(a4 *_t4) {
            is_a4 = true;
            t4 = _t4;
        }
        nonzero_digit(a5 *_t5) {
            is_a5 = true;
            t5 = _t5;
        }
        nonzero_digit(a6 *_t6) {
            is_a6 = true;
            t6 = _t6;
        }
        nonzero_digit(a7 *_t7) {
            is_a7 = true;
            t7 = _t7;
        }
        nonzero_digit(a8 *_t8) {
            is_a8 = true;
            t8 = _t8;
        }
        static nonzero_digit* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // unsigned_integer = "0" | nonzero_digit , { [ "_" ] , digit } ;
    struct unsigned_integer : public token {
        struct a0 : public token {
            terminal *t0;
            a0(terminal *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            struct b0 : public token {
                struct c0 : public token {
                    terminal *t0;
                    c0(terminal *_t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                std::optional<c0*> t0;
                digit *t1;
                b0(std::optional<c0*> _t0, digit *_t1) {
                    t0 = _t0;
                    t1 = _t1;
                }
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            nonzero_digit *t0;
            std::vector<b0*> t1;
            a1(nonzero_digit *_t0, std::vector<b0*> _t1) {
                t0 = _t0;
                t1 = _t1;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        unsigned_integer(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        unsigned_integer(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        static unsigned_integer* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // integer = [ "+" | "-" ] , unsigned_integer ;
    struct integer : public token {
        struct a0 : public token {
            struct b0 : public token {
                terminal *t0;
                b0(terminal *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b1 : public token {
                terminal *t0;
                b1(terminal *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string() override;
                void postprocess() override;
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
            std::string to_string() override;
            void postprocess() override;
        };
        std::optional<a0*> t0;
        unsigned_integer *t1;
        integer(std::optional<a0*> _t0, unsigned_integer *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static integer* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // boolean = "true" | "false" ;
    struct boolean : public token {
        struct a0 : public token {
            terminal *t0;
            a0(terminal *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            terminal *t0;
            a1(terminal *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        boolean(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        boolean(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        static boolean* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // value_list = value , ws , { "," , ws , value } , [ ws , "," ] ;
    struct value_list : public token {
        struct a0 : public token {
            terminal *t0;
            ws *t1;
            value *t2;
            a0(terminal *_t0, ws *_t1, value *_t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            ws *t0;
            terminal *t1;
            a1(ws *_t0, terminal *_t1) {
                t0 = _t0;
                t1 = _t1;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        value *t0;
        ws *t1;
        std::vector<a0*> t2;
        std::optional<a1*> t3;
        value_list(value *_t0, ws *_t1, std::vector<a0*> _t2, std::optional<a1*> _t3) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
        }
        static value_list* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // array = "[" , ws , value_list , ws , "]" ;
    struct array : public token {
        terminal *t0;
        ws *t1;
        value_list *t2;
        ws *t3;
        terminal *t4;
        array(terminal *_t0, ws *_t1, value_list *_t2, ws *_t3, terminal *_t4) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
        }
        static array* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // alpha = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" ;
    struct alpha : public token {
        struct a0 : public token {
            terminal *t0;
            a0(terminal *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            terminal *t0;
            a1(terminal *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a2 : public token {
            terminal *t0;
            a2(terminal *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a3 : public token {
            terminal *t0;
            a3(terminal *_t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a4 : public token {
            terminal *t0;
            a4(terminal *_t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a5 : public token {
            terminal *t0;
            a5(terminal *_t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a6 : public token {
            terminal *t0;
            a6(terminal *_t0) {
                t0 = _t0;
            }
            static a6* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a7 : public token {
            terminal *t0;
            a7(terminal *_t0) {
                t0 = _t0;
            }
            static a7* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a8 : public token {
            terminal *t0;
            a8(terminal *_t0) {
                t0 = _t0;
            }
            static a8* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a9 : public token {
            terminal *t0;
            a9(terminal *_t0) {
                t0 = _t0;
            }
            static a9* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a10 : public token {
            terminal *t0;
            a10(terminal *_t0) {
                t0 = _t0;
            }
            static a10* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a11 : public token {
            terminal *t0;
            a11(terminal *_t0) {
                t0 = _t0;
            }
            static a11* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a12 : public token {
            terminal *t0;
            a12(terminal *_t0) {
                t0 = _t0;
            }
            static a12* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a13 : public token {
            terminal *t0;
            a13(terminal *_t0) {
                t0 = _t0;
            }
            static a13* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a14 : public token {
            terminal *t0;
            a14(terminal *_t0) {
                t0 = _t0;
            }
            static a14* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a15 : public token {
            terminal *t0;
            a15(terminal *_t0) {
                t0 = _t0;
            }
            static a15* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a16 : public token {
            terminal *t0;
            a16(terminal *_t0) {
                t0 = _t0;
            }
            static a16* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a17 : public token {
            terminal *t0;
            a17(terminal *_t0) {
                t0 = _t0;
            }
            static a17* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a18 : public token {
            terminal *t0;
            a18(terminal *_t0) {
                t0 = _t0;
            }
            static a18* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a19 : public token {
            terminal *t0;
            a19(terminal *_t0) {
                t0 = _t0;
            }
            static a19* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a20 : public token {
            terminal *t0;
            a20(terminal *_t0) {
                t0 = _t0;
            }
            static a20* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a21 : public token {
            terminal *t0;
            a21(terminal *_t0) {
                t0 = _t0;
            }
            static a21* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a22 : public token {
            terminal *t0;
            a22(terminal *_t0) {
                t0 = _t0;
            }
            static a22* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a23 : public token {
            terminal *t0;
            a23(terminal *_t0) {
                t0 = _t0;
            }
            static a23* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a24 : public token {
            terminal *t0;
            a24(terminal *_t0) {
                t0 = _t0;
            }
            static a24* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a25 : public token {
            terminal *t0;
            a25(terminal *_t0) {
                t0 = _t0;
            }
            static a25* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a26 : public token {
            terminal *t0;
            a26(terminal *_t0) {
                t0 = _t0;
            }
            static a26* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a27 : public token {
            terminal *t0;
            a27(terminal *_t0) {
                t0 = _t0;
            }
            static a27* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a28 : public token {
            terminal *t0;
            a28(terminal *_t0) {
                t0 = _t0;
            }
            static a28* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a29 : public token {
            terminal *t0;
            a29(terminal *_t0) {
                t0 = _t0;
            }
            static a29* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a30 : public token {
            terminal *t0;
            a30(terminal *_t0) {
                t0 = _t0;
            }
            static a30* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a31 : public token {
            terminal *t0;
            a31(terminal *_t0) {
                t0 = _t0;
            }
            static a31* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a32 : public token {
            terminal *t0;
            a32(terminal *_t0) {
                t0 = _t0;
            }
            static a32* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a33 : public token {
            terminal *t0;
            a33(terminal *_t0) {
                t0 = _t0;
            }
            static a33* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a34 : public token {
            terminal *t0;
            a34(terminal *_t0) {
                t0 = _t0;
            }
            static a34* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a35 : public token {
            terminal *t0;
            a35(terminal *_t0) {
                t0 = _t0;
            }
            static a35* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a36 : public token {
            terminal *t0;
            a36(terminal *_t0) {
                t0 = _t0;
            }
            static a36* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a37 : public token {
            terminal *t0;
            a37(terminal *_t0) {
                t0 = _t0;
            }
            static a37* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a38 : public token {
            terminal *t0;
            a38(terminal *_t0) {
                t0 = _t0;
            }
            static a38* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a39 : public token {
            terminal *t0;
            a39(terminal *_t0) {
                t0 = _t0;
            }
            static a39* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a40 : public token {
            terminal *t0;
            a40(terminal *_t0) {
                t0 = _t0;
            }
            static a40* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a41 : public token {
            terminal *t0;
            a41(terminal *_t0) {
                t0 = _t0;
            }
            static a41* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a42 : public token {
            terminal *t0;
            a42(terminal *_t0) {
                t0 = _t0;
            }
            static a42* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a43 : public token {
            terminal *t0;
            a43(terminal *_t0) {
                t0 = _t0;
            }
            static a43* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a44 : public token {
            terminal *t0;
            a44(terminal *_t0) {
                t0 = _t0;
            }
            static a44* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a45 : public token {
            terminal *t0;
            a45(terminal *_t0) {
                t0 = _t0;
            }
            static a45* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a46 : public token {
            terminal *t0;
            a46(terminal *_t0) {
                t0 = _t0;
            }
            static a46* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a47 : public token {
            terminal *t0;
            a47(terminal *_t0) {
                t0 = _t0;
            }
            static a47* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a48 : public token {
            terminal *t0;
            a48(terminal *_t0) {
                t0 = _t0;
            }
            static a48* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a49 : public token {
            terminal *t0;
            a49(terminal *_t0) {
                t0 = _t0;
            }
            static a49* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a50 : public token {
            terminal *t0;
            a50(terminal *_t0) {
                t0 = _t0;
            }
            static a50* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a51 : public token {
            terminal *t0;
            a51(terminal *_t0) {
                t0 = _t0;
            }
            static a51* parse();
            std::string to_string() override;
            void postprocess() override;
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
        std::string to_string() override;
        void postprocess() override;
    };

    // digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
    struct digit : public token {
        struct a0 : public token {
            terminal *t0;
            a0(terminal *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            terminal *t0;
            a1(terminal *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a2 : public token {
            terminal *t0;
            a2(terminal *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a3 : public token {
            terminal *t0;
            a3(terminal *_t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a4 : public token {
            terminal *t0;
            a4(terminal *_t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a5 : public token {
            terminal *t0;
            a5(terminal *_t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a6 : public token {
            terminal *t0;
            a6(terminal *_t0) {
                t0 = _t0;
            }
            static a6* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a7 : public token {
            terminal *t0;
            a7(terminal *_t0) {
                t0 = _t0;
            }
            static a7* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a8 : public token {
            terminal *t0;
            a8(terminal *_t0) {
                t0 = _t0;
            }
            static a8* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a9 : public token {
            terminal *t0;
            a9(terminal *_t0) {
                t0 = _t0;
            }
            static a9* parse();
            std::string to_string() override;
            void postprocess() override;
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
        std::string to_string() override;
        void postprocess() override;
    };

    // escape = "\\" , ( "n" | "t" | "r" | "f" | "b" | "\"" | "\\" ) ;
    struct escape : public token {
        struct a0 : public token {
            struct b0 : public token {
                terminal *t0;
                b0(terminal *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b1 : public token {
                terminal *t0;
                b1(terminal *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b2 : public token {
                terminal *t0;
                b2(terminal *_t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b3 : public token {
                terminal *t0;
                b3(terminal *_t0) {
                    t0 = _t0;
                }
                static b3* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b4 : public token {
                terminal *t0;
                b4(terminal *_t0) {
                    t0 = _t0;
                }
                static b4* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b5 : public token {
                terminal *t0;
                b5(terminal *_t0) {
                    t0 = _t0;
                }
                static b5* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b6 : public token {
                terminal *t0;
                b6(terminal *_t0) {
                    t0 = _t0;
                }
                static b6* parse();
                std::string to_string() override;
                void postprocess() override;
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
            std::string to_string() override;
            void postprocess() override;
        };
        terminal *t0;
        a0 *t1;
        escape(terminal *_t0, a0 *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static escape* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // symbol = "[" | "]" | "{" | "}" | "(" | ")" | "<" | ">" | "=" | "|" | "." | "," | ";" | "-" | "+" | "_" | "*" | "?" | ":" | "!" | "@" | "$" | "%" | "^" | "&" | "/" | "~" | "`" ;
    struct symbol : public token {
        struct a0 : public token {
            terminal *t0;
            a0(terminal *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            terminal *t0;
            a1(terminal *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a2 : public token {
            terminal *t0;
            a2(terminal *_t0) {
                t0 = _t0;
            }
            static a2* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a3 : public token {
            terminal *t0;
            a3(terminal *_t0) {
                t0 = _t0;
            }
            static a3* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a4 : public token {
            terminal *t0;
            a4(terminal *_t0) {
                t0 = _t0;
            }
            static a4* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a5 : public token {
            terminal *t0;
            a5(terminal *_t0) {
                t0 = _t0;
            }
            static a5* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a6 : public token {
            terminal *t0;
            a6(terminal *_t0) {
                t0 = _t0;
            }
            static a6* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a7 : public token {
            terminal *t0;
            a7(terminal *_t0) {
                t0 = _t0;
            }
            static a7* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a8 : public token {
            terminal *t0;
            a8(terminal *_t0) {
                t0 = _t0;
            }
            static a8* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a9 : public token {
            terminal *t0;
            a9(terminal *_t0) {
                t0 = _t0;
            }
            static a9* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a10 : public token {
            terminal *t0;
            a10(terminal *_t0) {
                t0 = _t0;
            }
            static a10* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a11 : public token {
            terminal *t0;
            a11(terminal *_t0) {
                t0 = _t0;
            }
            static a11* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a12 : public token {
            terminal *t0;
            a12(terminal *_t0) {
                t0 = _t0;
            }
            static a12* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a13 : public token {
            terminal *t0;
            a13(terminal *_t0) {
                t0 = _t0;
            }
            static a13* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a14 : public token {
            terminal *t0;
            a14(terminal *_t0) {
                t0 = _t0;
            }
            static a14* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a15 : public token {
            terminal *t0;
            a15(terminal *_t0) {
                t0 = _t0;
            }
            static a15* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a16 : public token {
            terminal *t0;
            a16(terminal *_t0) {
                t0 = _t0;
            }
            static a16* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a17 : public token {
            terminal *t0;
            a17(terminal *_t0) {
                t0 = _t0;
            }
            static a17* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a18 : public token {
            terminal *t0;
            a18(terminal *_t0) {
                t0 = _t0;
            }
            static a18* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a19 : public token {
            terminal *t0;
            a19(terminal *_t0) {
                t0 = _t0;
            }
            static a19* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a20 : public token {
            terminal *t0;
            a20(terminal *_t0) {
                t0 = _t0;
            }
            static a20* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a21 : public token {
            terminal *t0;
            a21(terminal *_t0) {
                t0 = _t0;
            }
            static a21* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a22 : public token {
            terminal *t0;
            a22(terminal *_t0) {
                t0 = _t0;
            }
            static a22* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a23 : public token {
            terminal *t0;
            a23(terminal *_t0) {
                t0 = _t0;
            }
            static a23* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a24 : public token {
            terminal *t0;
            a24(terminal *_t0) {
                t0 = _t0;
            }
            static a24* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a25 : public token {
            terminal *t0;
            a25(terminal *_t0) {
                t0 = _t0;
            }
            static a25* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a26 : public token {
            terminal *t0;
            a26(terminal *_t0) {
                t0 = _t0;
            }
            static a26* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a27 : public token {
            terminal *t0;
            a27(terminal *_t0) {
                t0 = _t0;
            }
            static a27* parse();
            std::string to_string() override;
            void postprocess() override;
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
        static symbol* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // comment = "#" , { alpha | digit | symbol | "#" | "\"" | "'" | " " | "\\" | "\t" } ;
    struct comment : public token {
        struct a0 : public token {
            struct b0 : public token {
                alpha *t0;
                b0(alpha *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b1 : public token {
                digit *t0;
                b1(digit *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b2 : public token {
                symbol *t0;
                b2(symbol *_t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b3 : public token {
                terminal *t0;
                b3(terminal *_t0) {
                    t0 = _t0;
                }
                static b3* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b4 : public token {
                terminal *t0;
                b4(terminal *_t0) {
                    t0 = _t0;
                }
                static b4* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b5 : public token {
                terminal *t0;
                b5(terminal *_t0) {
                    t0 = _t0;
                }
                static b5* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b6 : public token {
                terminal *t0;
                b6(terminal *_t0) {
                    t0 = _t0;
                }
                static b6* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b7 : public token {
                terminal *t0;
                b7(terminal *_t0) {
                    t0 = _t0;
                }
                static b7* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b8 : public token {
                terminal *t0;
                b8(terminal *_t0) {
                    t0 = _t0;
                }
                static b8* parse();
                std::string to_string() override;
                void postprocess() override;
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
            std::string to_string() override;
            void postprocess() override;
        };
        terminal *t0;
        std::vector<a0*> t1;
        comment(terminal *_t0, std::vector<a0*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static comment* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // ws = { " " | "\n" | "\t" | "\r" | comment } ;
    struct ws : public token {
        struct a0 : public token {
            struct b0 : public token {
                terminal *t0;
                b0(terminal *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b1 : public token {
                terminal *t0;
                b1(terminal *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b2 : public token {
                terminal *t0;
                b2(terminal *_t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b3 : public token {
                terminal *t0;
                b3(terminal *_t0) {
                    t0 = _t0;
                }
                static b3* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b4 : public token {
                comment *t0;
                b4(comment *_t0) {
                    t0 = _t0;
                }
                static b4* parse();
                std::string to_string() override;
                void postprocess() override;
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
            std::string to_string() override;
            void postprocess() override;
        };
        std::vector<a0*> t0;
        ws(std::vector<a0*> _t0) {
            t0 = _t0;
        }
        static ws* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // hws = { " " | "\t" } ;
    struct hws : public token {
        struct a0 : public token {
            struct b0 : public token {
                terminal *t0;
                b0(terminal *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b1 : public token {
                terminal *t0;
                b1(terminal *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string() override;
                void postprocess() override;
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
            std::string to_string() override;
            void postprocess() override;
        };
        std::vector<a0*> t0;
        hws(std::vector<a0*> _t0) {
            t0 = _t0;
        }
        static hws* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // bare_key = < alpha | digit | "_" | "-" > ;
    struct bare_key : public token {
        struct a0 : public token {
            struct b0 : public token {
                alpha *t0;
                b0(alpha *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b1 : public token {
                digit *t0;
                b1(digit *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b2 : public token {
                terminal *t0;
                b2(terminal *_t0) {
                    t0 = _t0;
                }
                static b2* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b3 : public token {
                terminal *t0;
                b3(terminal *_t0) {
                    t0 = _t0;
                }
                static b3* parse();
                std::string to_string() override;
                void postprocess() override;
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
            std::string to_string() override;
            void postprocess() override;
        };
        std::vector<a0*> t0;
        bare_key(std::vector<a0*> _t0) {
            t0 = _t0;
        }
        static bare_key* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // quoted_key = "\"" , { alpha | digit | symbol | "'" | " " | "#" } , "\"" | "'" , { alpha | digit | symbol | "\"" | " " | "#" } , "'" ;
    struct quoted_key : public token {
        struct a0 : public token {
            struct b0 : public token {
                struct c0 : public token {
                    alpha *t0;
                    c0(alpha *_t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c1 : public token {
                    digit *t0;
                    c1(digit *_t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c2 : public token {
                    symbol *t0;
                    c2(symbol *_t0) {
                        t0 = _t0;
                    }
                    static c2* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c3 : public token {
                    terminal *t0;
                    c3(terminal *_t0) {
                        t0 = _t0;
                    }
                    static c3* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c4 : public token {
                    terminal *t0;
                    c4(terminal *_t0) {
                        t0 = _t0;
                    }
                    static c4* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c5 : public token {
                    terminal *t0;
                    c5(terminal *_t0) {
                        t0 = _t0;
                    }
                    static c5* parse();
                    std::string to_string() override;
                    void postprocess() override;
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
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            terminal *t0;
            std::vector<b0*> t1;
            terminal *t2;
            a0(terminal *_t0, std::vector<b0*> _t1, terminal *_t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            struct b0 : public token {
                struct c0 : public token {
                    alpha *t0;
                    c0(alpha *_t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c1 : public token {
                    digit *t0;
                    c1(digit *_t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c2 : public token {
                    symbol *t0;
                    c2(symbol *_t0) {
                        t0 = _t0;
                    }
                    static c2* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c3 : public token {
                    terminal *t0;
                    c3(terminal *_t0) {
                        t0 = _t0;
                    }
                    static c3* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c4 : public token {
                    terminal *t0;
                    c4(terminal *_t0) {
                        t0 = _t0;
                    }
                    static c4* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c5 : public token {
                    terminal *t0;
                    c5(terminal *_t0) {
                        t0 = _t0;
                    }
                    static c5* parse();
                    std::string to_string() override;
                    void postprocess() override;
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
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            terminal *t0;
            std::vector<b0*> t1;
            terminal *t2;
            a1(terminal *_t0, std::vector<b0*> _t1, terminal *_t2) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        quoted_key(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        quoted_key(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        static quoted_key* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // key = ( quoted_key | bare_key ) , { hws , "." , hws , ( quoted_key | bare_key ) } ;
    struct key : public token {
        struct a0 : public token {
            struct b0 : public token {
                quoted_key *t0;
                b0(quoted_key *_t0) {
                    t0 = _t0;
                }
                static b0* parse();
                std::string to_string() override;
                void postprocess() override;
            };
            struct b1 : public token {
                bare_key *t0;
                b1(bare_key *_t0) {
                    t0 = _t0;
                }
                static b1* parse();
                std::string to_string() override;
                void postprocess() override;
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
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            struct b0 : public token {
                struct c0 : public token {
                    quoted_key *t0;
                    c0(quoted_key *_t0) {
                        t0 = _t0;
                    }
                    static c0* parse();
                    std::string to_string() override;
                    void postprocess() override;
                };
                struct c1 : public token {
                    bare_key *t0;
                    c1(bare_key *_t0) {
                        t0 = _t0;
                    }
                    static c1* parse();
                    std::string to_string() override;
                    void postprocess() override;
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
                std::string to_string() override;
                void postprocess() override;
            };
            hws *t0;
            terminal *t1;
            hws *t2;
            b0 *t3;
            a1(hws *_t0, terminal *_t1, hws *_t2, b0 *_t3) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        a0 *t0;
        std::vector<a1*> t1;
        key(a0 *_t0, std::vector<a1*> _t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static key* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // table = "[" , hws , key , hws , "]" | "[[" , hws , key , hws , "]]" ;
    struct table : public token {
        struct a0 : public token {
            terminal *t0;
            hws *t1;
            key *t2;
            hws *t3;
            terminal *t4;
            a0(terminal *_t0, hws *_t1, key *_t2, hws *_t3, terminal *_t4) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
                t4 = _t4;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            terminal *t0;
            hws *t1;
            key *t2;
            hws *t3;
            terminal *t4;
            a1(terminal *_t0, hws *_t1, key *_t2, hws *_t3, terminal *_t4) {
                t0 = _t0;
                t1 = _t1;
                t2 = _t2;
                t3 = _t3;
                t4 = _t4;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        table(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        table(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        static table* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // key_value = key , hws , "=" , hws , value ;
    struct key_value : public token {
        key *t0;
        hws *t1;
        terminal *t2;
        hws *t3;
        value *t4;
        key_value(key *_t0, hws *_t1, terminal *_t2, hws *_t3, value *_t4) {
            t0 = _t0;
            t1 = _t1;
            t2 = _t2;
            t3 = _t3;
            t4 = _t4;
        }
        static key_value* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // statement = key_value | table ;
    struct statement : public token {
        struct a0 : public token {
            key_value *t0;
            a0(key_value *_t0) {
                t0 = _t0;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        struct a1 : public token {
            table *t0;
            a1(table *_t0) {
                t0 = _t0;
            }
            static a1* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        bool is_a0 = false;
        a0 *t0;
        bool is_a1 = false;
        a1 *t1;
        statement(a0 *_t0) {
            is_a0 = true;
            t0 = _t0;
        }
        statement(a1 *_t1) {
            is_a1 = true;
            t1 = _t1;
        }
        static statement* parse();
        std::string to_string() override;
        void postprocess() override;
    };

    // toml = { ws , statement } , ws ;
    struct toml : public token {
        struct a0 : public token {
            ws *t0;
            statement *t1;
            a0(ws *_t0, statement *_t1) {
                t0 = _t0;
                t1 = _t1;
            }
            static a0* parse();
            std::string to_string() override;
            void postprocess() override;
        };
        std::vector<a0*> t0;
        ws *t1;
        toml(std::vector<a0*> _t0, ws *_t1) {
            t0 = _t0;
            t1 = _t1;
        }
        static toml* parse();
        std::string to_string() override;
        void postprocess() override;
    };

};
