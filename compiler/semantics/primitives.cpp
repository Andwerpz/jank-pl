#include "primitives.h"
#include "utils.h"
#include "Type.h"
#include <iostream>
#include <string>
#include <map>

namespace primitives {

    void add_int_operators(Type *p_int, int sz_bytes, bool is_signed) {
        std::string rax, rbx, mov, inc, dec, movs, movz;
        if(sz_bytes == 1) {
            rax = "%al";
            rbx = "%bl";
            mov = "movb";
            inc = "incb";
            dec = "decb";
            movs = "movsbq";
            movz = "movzbq";
        }
        else if(sz_bytes == 2) {
            rax = "%ax";
            rbx = "%bx";
            mov = "movw";
            inc = "incw";
            dec = "decw";
            movs = "movswq";
            movz = "movzwq";
        }
        else if(sz_bytes == 4) {
            rax = "%eax";
            rbx = "%ebx";
            mov = "movl";
            inc = "incl";
            dec = "decl";
            movs = "movslq";
            movz = "movzlq";
        }
        else if(sz_bytes == 8) {
            rax = "%rax";
            rbx = "%rbx";
            mov = "movq";
            inc = "incq";
            dec = "decq";
        }
        else {
            std::cout << "Bad int operator size : " << sz_bytes << "\n";
            assert(false);
        }

        //prefix operators
        add_operator_implementation(new OperatorSignature("+", p_int), new BuiltinOperator(p_int, {}));
        add_operator_implementation(new OperatorSignature("-", p_int), new BuiltinOperator(p_int, {"neg " + rax}));
        add_operator_implementation(new OperatorSignature("~", p_int), new BuiltinOperator(p_int, {"not " + rax}));
        add_operator_implementation(new OperatorSignature("!", p_int), new BuiltinOperator(p_int, {
            "test " + rax + ", " + rax,
            "sete %al",
            "movzx %al, %rax",
        }));

        //pre and post increment / decrement
        add_operator_implementation(new OperatorSignature("++", new ReferenceType(p_int)), new BuiltinOperator(new ReferenceType(p_int), {
            inc + " (%rcx)",
            "mov %rcx, %rax",
        }));
        add_operator_implementation(new OperatorSignature("--", new ReferenceType(p_int)), new BuiltinOperator(new ReferenceType(p_int), {
            dec + " (%rcx)",
            "mov %rcx, %rax",
        }));
        add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "++"), new BuiltinOperator(p_int, {inc + " (%rcx)"}));
        add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "--"), new BuiltinOperator(p_int, {dec + " (%rcx)"}));

        // binary +, -
        add_operator_implementation(new OperatorSignature(p_int, "+", p_int), new BuiltinOperator(p_int, {"add " + rbx + ", " + rax}));
        add_operator_implementation(new OperatorSignature(p_int, "-", p_int), new BuiltinOperator(p_int, {"sub " + rbx + ", " + rax}));
        add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "+=", p_int), new BuiltinOperator(p_int, {
            "add " + rbx + ", " + rax,
            mov + " " + rax + ", (%rcx)",
        }));
        add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "-=", p_int), new BuiltinOperator(p_int, {
            "sub " + rbx + ", " + rax,
            mov + " " + rax + ", (%rcx)",
        }));
        
        //binary * / %
        if(is_signed) {
            add_operator_implementation(new OperatorSignature(p_int, "*", p_int), new BuiltinOperator(p_int, {"imul " + rbx + ", " + rax}));
            add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "*=", p_int), new BuiltinOperator(p_int, {
                "imul " + rbx + ", " + rax,
                mov + " " + rax + ", (%rcx)",
            }));
            if(sz_bytes < 8) {
                add_operator_implementation(new OperatorSignature(p_int, "/", p_int), new BuiltinOperator(p_int, {
                    movs + " " + rax + ", %rax",
                    movs + " " + rbx + ", %rbx",
                    "cqo",
                    "idiv %rbx",
                    movz + " " + rax + ", %rax",
                }));
                add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "/=", p_int), new BuiltinOperator(p_int, {
                    movs + " " + rax + ", %rax",
                    movs + " " + rbx + ", %rbx",
                    "cqo",
                    "idiv %rbx",
                    movz + " " + rax + ", %rax",
                    mov + " " + rax + ", (%rcx)",
                }));
                add_operator_implementation(new OperatorSignature(p_int, "%", p_int), new BuiltinOperator(p_int, {
                    movs + " " + rax + ", %rax",
                    movs + " " + rbx + ", %rbx",
                    "cqo",
                    "idiv %rbx",
                    "mov %rdx, %rax",
                    movz + " " + rax + ", %rax",
                }));
                add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "%=", p_int), new BuiltinOperator(p_int, {
                    movs + " " + rax + ", %rax",
                    movs + " " + rbx + ", %rbx",
                    "cqo",
                    "idiv %rbx",
                    "mov %rdx, %rax",
                    movz + " " + rax + ", %rax",
                    mov + " " + rax + ", (%rcx)",
                }));
            }
            else if(sz_bytes == 8) {
                add_operator_implementation(new OperatorSignature(p_int, "/", p_int), new BuiltinOperator(p_int, {
                    "cqo",
                    "idiv %rbx",
                }));
                add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "/=", p_int), new BuiltinOperator(p_int, {
                    "cqo",
                    "idiv %rbx",
                    mov + " " + rax + ", (%rcx)",
                }));
                add_operator_implementation(new OperatorSignature(p_int, "%", p_int), new BuiltinOperator(p_int, {
                    "cqo",
                    "idiv %rbx",
                    "mov %rdx, %rax",
                }));
                add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "%=", p_int), new BuiltinOperator(p_int, {
                    "cqo",
                    "idiv %rbx",
                    "mov %rdx, %rax",
                    mov + " " + rax + ", (%rcx)",
                }));
            }
            else assert(false);
        }
        else {
            add_operator_implementation(new OperatorSignature(p_int, "*", p_int), new BuiltinOperator(p_int, {"mul " + rbx + ", " + rax}));
            add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "*=", p_int), new BuiltinOperator(p_int, {
                "mul " + rbx + ", " + rax,
                mov + " " + rax + ", (%rcx)",
            }));
            if(sz_bytes < 8) {
                add_operator_implementation(new OperatorSignature(p_int, "/", p_int), new BuiltinOperator(p_int, {
                    movs + " " + rax + ", %rax",
                    movs + " " + rbx + ", %rbx",
                    "cqo",
                    "div %rbx",
                    movz + " " + rax + ", %rax",
                }));
                add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "/=", p_int), new BuiltinOperator(p_int, {
                    movs + " " + rax + ", %rax",
                    movs + " " + rbx + ", %rbx",
                    "cqo",
                    "div %rbx",
                    movz + " " + rax + ", %rax",
                    mov + " " + rax + ", (%rcx)",
                }));
                add_operator_implementation(new OperatorSignature(p_int, "%", p_int), new BuiltinOperator(p_int, {
                    movs + " " + rax + ", %rax",
                    movs + " " + rbx + ", %rbx",
                    "cqo",
                    "div %rbx",
                    "mov %rdx, %rax",
                    movz + " " + rax + ", %rax",
                }));
                add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "%=", p_int), new BuiltinOperator(p_int, {
                    movs + " " + rax + ", %rax",
                    movs + " " + rbx + ", %rbx",
                    "cqo",
                    "div %rbx",
                    "mov %rdx, %rax",
                    movz + " " + rax + ", %rax",
                    mov + " " + rax + ", (%rcx)",
                }));
            }
            else if(sz_bytes == 8) {
                add_operator_implementation(new OperatorSignature(p_int, "/", p_int), new BuiltinOperator(p_int, {
                    "cqo",
                    "div %rbx",
                }));
                add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "/=", p_int), new BuiltinOperator(p_int, {
                    "cqo",
                    "div %rbx",
                    mov + " " + rax + ", (%rcx)",
                }));
                add_operator_implementation(new OperatorSignature(p_int, "%", p_int), new BuiltinOperator(p_int, {
                    "cqo",
                    "div %rbx",
                    "mov %rdx, %rax",
                }));
                add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "%=", p_int), new BuiltinOperator(p_int, {
                    "cqo",
                    "div %rbx",
                    "mov %rdx, %rax",
                    mov + " " + rax + ", (%rcx)",
                }));
            }
            else assert(false);
        }

        //just do bitwise operations on the whole register. 
        add_operator_implementation(new OperatorSignature(p_int, "&", p_int), new BuiltinOperator(p_int, {"and %rbx, %rax"}));
        add_operator_implementation(new OperatorSignature(p_int, "^", p_int), new BuiltinOperator(p_int, {"xor %rbx, %rax"}));
        add_operator_implementation(new OperatorSignature(p_int, "|", p_int), new BuiltinOperator(p_int, {"or %rbx, %rax"}));
        add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "&=", p_int), new BuiltinOperator(p_int, {
            "and %rbx, %rax",
            mov + " " + rax + ", (%rcx)",
        }));
        add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "^=", p_int), new BuiltinOperator(p_int, {
            "xor %rbx, %rax",
            mov + " " + rax + ", (%rcx)",
        }));
        add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "|=", p_int), new BuiltinOperator(p_int, {
            "or %rbx, %rax",
            mov + " " + rax + ", (%rcx)",
        }));

        add_operator_implementation(new OperatorSignature(p_int, "&&", p_int), new BuiltinOperator(i64, {
            "test " + rax + ", " + rbx,
            "setne %al",
            "movzx %al, %rax",
            "test " + rbx + ", " + rbx,
            "setne %bl",
            "movzx %bl, %rbx",
            "and %rbx, %rax",
        }));    
        add_operator_implementation(new OperatorSignature(p_int, "||", p_int), new BuiltinOperator(i64, {
            "test " + rax + ", " + rbx,
            "setne %al",
            "movzx %al, %rax",
            "test " + rbx + ", " + rbx,
            "setne %bl",
            "movzx %bl, %rbx",
            "or %rbx, %rax",
        }));  

        add_operator_implementation(new OperatorSignature(p_int, "==", p_int), new BuiltinOperator(i64, {
            "cmp " + rbx + ", " + rax,
            "sete %al",
            "movzx %al, %rax",
        }));  
        add_operator_implementation(new OperatorSignature(p_int, "!=", p_int), new BuiltinOperator(i64, {
            "cmp " + rbx + ", " + rax,
            "setne %al",
            "movzx %al, %rax",
        }));  

        //for comparison, it depends on the interpretation of the integers. 
        if(is_signed) {
            add_operator_implementation(new OperatorSignature(p_int, "<", p_int), new BuiltinOperator(i64, {
                "cmp " + rbx + ", " + rax,
                "setl %al",
                "movzx %al, %rax"
            }));  
            add_operator_implementation(new OperatorSignature(p_int, ">", p_int), new BuiltinOperator(i64, {
                "cmp " + rbx + ", " + rax,
                "setg %al",
                "movzx %al, %rax"
            }));
            add_operator_implementation(new OperatorSignature(p_int, "<=", p_int), new BuiltinOperator(i64, {
                "cmp " + rbx + ", " + rax,
                "setle %al",
                "movzx %al, %rax"
            }));
            add_operator_implementation(new OperatorSignature(p_int, ">=", p_int), new BuiltinOperator(i64, {
                "cmp " + rbx + ", " + rax,
                "setge %al",
                "movzx %al, %rax"
            }));
        }
        else {
            add_operator_implementation(new OperatorSignature(p_int, "<", p_int), new BuiltinOperator(i64, {
                "cmp " + rbx + ", " + rax,
                "setb %al",
                "movzx %al, %rax"
            }));  
            add_operator_implementation(new OperatorSignature(p_int, ">", p_int), new BuiltinOperator(i64, {
                "cmp " + rbx + ", " + rax,
                "seta %al",
                "movzx %al, %rax"
            }));
            add_operator_implementation(new OperatorSignature(p_int, "<=", p_int), new BuiltinOperator(i64, {
                "cmp " + rbx + ", " + rax,
                "setbe %al",
                "movzx %al, %rax"
            }));
            add_operator_implementation(new OperatorSignature(p_int, ">=", p_int), new BuiltinOperator(i64, {
                "cmp " + rbx + ", " + rax,
                "setae %al",
                "movzx %al, %rax"
            }));
        }
        
        //also need to do signed aware behaviour for shifting
        if(is_signed) {
            add_operator_implementation(new OperatorSignature(p_int, "<<", p_int), new BuiltinOperator(p_int, {
                "push %rcx",
                "mov %rbx, %rcx",
                "sal %cl, " + rax,
                "pop %rcx",
            }));
            add_operator_implementation(new OperatorSignature(p_int, ">>", p_int), new BuiltinOperator(p_int, {
                "push %rcx",
                "mov %rbx, %rcx",
                "sar %cl, " + rax,
                "pop %rcx",
            }));
            add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "<<=", p_int), new BuiltinOperator(p_int, {
                "push %rcx",
                "mov %rbx, %rcx",
                "sal %cl, " + rax,
                "pop %rcx",
                mov + " " + rax + ", (%rcx)",
            }));
            add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), ">>=", p_int), new BuiltinOperator(p_int, {
                "push %rcx",
                "mov %rbx, %rcx",
                "sar %cl, " + rax,
                "pop %rcx",
                mov + " " + rax + ", (%rcx)",
            }));
        }
        else {
            add_operator_implementation(new OperatorSignature(p_int, "<<", p_int), new BuiltinOperator(p_int, {
                "push %rcx",
                "mov %rbx, %rcx",
                "shl %cl, " + rax,
                "pop %rcx",
            }));
            add_operator_implementation(new OperatorSignature(p_int, ">>", p_int), new BuiltinOperator(p_int, {
                "push %rcx",
                "mov %rbx, %rcx",
                "shr %cl, " + rax,
                "pop %rcx",
            }));
            add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), "<<=", p_int), new BuiltinOperator(p_int, {
                "push %rcx",
                "mov %rbx, %rcx",
                "shl %cl, " + rax,
                "pop %rcx",
                mov + " " + rax + ", (%rcx)",
            }));
            add_operator_implementation(new OperatorSignature(new ReferenceType(p_int), ">>=", p_int), new BuiltinOperator(p_int, {
                "push %rcx",
                "mov %rbx, %rcx",
                "shr %cl, " + rax,
                "pop %rcx",
                mov + " " + rax + ", (%rcx)",
            }));
        }
    }

    void init_int_primitives() {
        i8 = new BaseType("i8");
        i16 = new BaseType("i16");
        i32 = new BaseType("i32");
        i64 = new BaseType("i64");

        u8 = new BaseType("u8");
        u16 = new BaseType("u16");
        u32 = new BaseType("u32");
        u64 = new BaseType("u64");

        add_primitive_basetype(i8);
        add_primitive_basetype(i16);
        add_primitive_basetype(i32);
        add_primitive_basetype(i64);

        add_primitive_basetype(u8);
        add_primitive_basetype(u16);
        add_primitive_basetype(u32);
        add_primitive_basetype(u64);

        add_int_operators(i8, 1, true);
        add_int_operators(i16, 2, true);
        add_int_operators(i32, 4, true);
        add_int_operators(i64, 8, true);

        add_int_operators(u8, 1, false);
        add_int_operators(u16, 2, false);
        add_int_operators(u32, 4, false);
        add_int_operators(u64, 8, false);

        //every int type should be able to convert to every other int type by truncating or 0-extending. 
        std::vector<std::pair<Type*, int>> ints = {
            {i8, 1},
            {i16, 2},
            {i32, 4},
            {i64, 8},
        };
        std::vector<std::pair<Type*, int>> uints = {
            {u8, 1},
            {u16, 2},
            {u32, 4},
            {u64, 8},
        };

        std::map<int, std::string> sz_to_reg;
        sz_to_reg[1] = "%al";
        sz_to_reg[2] = "%ax";
        sz_to_reg[4] = "%eax";
        sz_to_reg[8] = "%rax";

        std::map<int, std::string> sz_to_suf;
        sz_to_suf[1] = "b";
        sz_to_suf[2] = "w";
        sz_to_suf[4] = "l";
        sz_to_suf[8] = "q";

        //in the case where the type gets larger, the general rule seems to be
        // - if the source is signed, we sign extend
        // - if the source is unsigned, we zero extend

        // signed int to signed int
        for(int i = 0; i < ints.size(); i++){
            for(int j = 0; j < ints.size(); j++){
                if(i == j) continue;
                Type *ta = ints[i].first, *tb = ints[j].first;
                int sza = ints[i].second, szb = ints[j].second;
                std::string ra = sz_to_reg[sza], rb = sz_to_reg[szb];
                if(sza < szb) {
                    //need to sign extend
                    std::string mov = "movs" + sz_to_suf[sza] + sz_to_suf[szb];
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {mov + " " + ra + ", " + rb}));
                }
                else if(sza > szb) {
                    //need to truncate
                    std::string mov = "movz" + sz_to_suf[szb] + "q";
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {mov + " " + rb + ", %rax\n"}));
                }
                else assert(false);
            }
        }

        // unsigned int to unsigned int
        for(int i = 0; i < uints.size(); i++){
            for(int j = 0; j < uints.size(); j++){
                if(i == j) continue;
                Type *ta = uints[i].first, *tb = uints[j].first;
                int sza = uints[i].second, szb = uints[j].second;
                std::string ra = sz_to_reg[sza], rb = sz_to_reg[szb];
                if(sza < szb) {
                    //need to zero extend
                    std::string mov = "movz" + sz_to_suf[sza] + sz_to_suf[szb];
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {mov + " " + ra + ", " + rb}));
                }
                else if(sza > szb) {
                    //need to truncate
                    std::string mov = "movz" + sz_to_suf[szb] + "q";
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {mov + " " + rb + ", %rax\n"}));
                }
                else assert(false);
            }
        }

        // signed int to unsigned int
        for(int i = 0; i < ints.size(); i++){
            for(int j = 0; j < uints.size(); j++){
                Type *ta = ints[i].first, *tb = uints[j].first;
                int sza = ints[i].second, szb = uints[j].second;
                std::string ra = sz_to_reg[sza], rb = sz_to_reg[szb];
                if(sza < szb) {
                    //sign extend
                    std::string mov = "movs" + sz_to_suf[sza] + sz_to_suf[szb];
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {mov + " " + ra + ", " + rb}));
                }
                else if(sza > szb) {
                    //truncate
                    std::string mov = "movz" + sz_to_suf[szb] + "q";
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {mov + " " + rb + ", %rax\n"}));
                }
                else {
                    //do nothing
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {}));
                }
            }
        }

        // unsigned int to signed int 
        for(int i = 0; i < uints.size(); i++){
            for(int j = 0; j < ints.size(); j++){
                Type *ta = uints[i].first, *tb = ints[j].first;
                int sza = uints[i].second, szb = ints[j].second;
                std::string ra = sz_to_reg[sza], rb = sz_to_reg[szb];
                if(sza < szb) {
                    //need to zero extend
                    std::string mov = "movz" + sz_to_suf[sza] + sz_to_suf[szb];
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {mov + " " + ra + ", " + rb}));
                }
                else if(sza > szb) {
                    //need to truncate
                    std::string mov = "movz" + sz_to_suf[szb] + "q";
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {mov + " " + rb + ", %rax\n"}));
                }
                else {
                    //do nothing
                    add_operator_implementation(new OperatorSignature(ta, tb), new BuiltinOperator(tb, {}));
                }
            }
        }

    }

    void init_float_primitives() {
        f32 = new BaseType("f32");
        add_primitive_basetype(f32);

        //soo, designers of intel syntax must have been trolling
        //movss <dest> <src>
        //addss <src> <dest>

        //use 'movd <src> <dest>' when moving to and from general purpose registers

        // +x -x
        add_operator_implementation(new OperatorSignature("+", f32), new BuiltinOperator(f32, {}));
        add_operator_implementation(new OperatorSignature("-", f32), new BuiltinOperator(f32, {
            "xor $0x80000000, %eax",    //flip the sign bit
        }));

        // + - * / += -= *= /= 
        add_operator_implementation(new OperatorSignature(f32, "+", f32), new BuiltinOperator(f32, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "addss %xmm1, %xmm0",
            "movd %xmm0, %eax",
        }));
        add_operator_implementation(new OperatorSignature(f32, "-", f32), new BuiltinOperator(f32, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "subss %xmm1, %xmm0",
            "movd %xmm0, %eax",
        }));
        add_operator_implementation(new OperatorSignature(f32, "*", f32), new BuiltinOperator(f32, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "mulss %xmm1, %xmm0",
            "movd %xmm0, %eax",
        }));
        add_operator_implementation(new OperatorSignature(f32, "/", f32), new BuiltinOperator(f32, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "divss %xmm1, %xmm0",
            "movd %xmm0, %eax",
        }));
        add_operator_implementation(new OperatorSignature(new ReferenceType(f32), "+=", f32), new BuiltinOperator(f32, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "addss %xmm1, %xmm0",
            "movd %xmm0, %eax",
            "movl %eax, (%rcx)",
        }));
        add_operator_implementation(new OperatorSignature(new ReferenceType(f32), "-=", f32), new BuiltinOperator(f32, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "subss %xmm1, %xmm0",
            "movd %xmm0, %eax",
            "movl %eax, (%rcx)",
        }));
        add_operator_implementation(new OperatorSignature(new ReferenceType(f32), "*=", f32), new BuiltinOperator(f32, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "mulss %xmm1, %xmm0",
            "movd %xmm0, %eax",
            "movl %eax, (%rcx)",
        }));
        add_operator_implementation(new OperatorSignature(new ReferenceType(f32), "/=", f32), new BuiltinOperator(f32, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "divss %xmm1, %xmm0",
            "movd %xmm0, %eax",
            "movl %eax, (%rcx)",
        }));

        // == < > <= >= 
        add_operator_implementation(new OperatorSignature(f32, "==", f32), new BuiltinOperator(i64, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "ucomiss %xmm1, %xmm0",
            "sete %al",
            "movzx %al, %rax",
        }));
        add_operator_implementation(new OperatorSignature(f32, "<", f32), new BuiltinOperator(i64, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "ucomiss %xmm1, %xmm0",
            "setb %al",
            "movzx %al, %rax",
        }));
        add_operator_implementation(new OperatorSignature(f32, ">", f32), new BuiltinOperator(i64, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "ucomiss %xmm1, %xmm0",
            "seta %al",
            "movzx %al, %rax",
        }));
        add_operator_implementation(new OperatorSignature(f32, "<=", f32), new BuiltinOperator(i64, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "ucomiss %xmm1, %xmm0",
            "setbe %al",
            "movzx %al, %rax",
        }));
        add_operator_implementation(new OperatorSignature(f32, ">=", f32), new BuiltinOperator(i64, {
            "movd %eax, %xmm0",
            "movd %ebx, %xmm1",
            "ucomiss %xmm1, %xmm0",
            "setae %al",
            "movzx %al, %rax",
        }));

        //convert float into signed integer
        // cvttss2si : ConVerT with Truncation Scalar Single precision floating point 2 Signed Integer
        // cvtsi2ss : ConVerT Scalar Signed Integer 2 Scalar Single precision floating point
        add_operator_implementation(new OperatorSignature(f32, i8), new BuiltinOperator(i8, {
            "movd %eax, %xmm0",
            "cvtsi2ss %xmm0, %eax",
            "movzx %al, %rax",
        }));
        add_operator_implementation(new OperatorSignature(f32, i16), new BuiltinOperator(i16, {
            "movd %eax, %xmm0",
            "cvttss2si %xmm0, %eax",
            "movzx %ax, %rax",
        }));
        add_operator_implementation(new OperatorSignature(f32, i32), new BuiltinOperator(i32, {
            "movd %eax, %xmm0",
            "cvttss2si %xmm0, %eax",
        }));
        add_operator_implementation(new OperatorSignature(f32, i64), new BuiltinOperator(i64, {
            "movd %eax, %xmm0",
            "cvttss2si %xmm0, %rax",
        }));

        //convert signed integer into float
        add_operator_implementation(new OperatorSignature(i8, f32), new BuiltinOperator(f32, {
            "movsbl %al, %eax",
            "cvtsi2ss %eax, %xmm0",
            "movd %xmm0, %eax",
        }));
        add_operator_implementation(new OperatorSignature(i16, f32), new BuiltinOperator(f32, {
            "movswl %ax, %eax",
            "cvtsi2ss %eax, %xmm0",
            "movd %xmm0, %eax",
        }));
        add_operator_implementation(new OperatorSignature(i32, f32), new BuiltinOperator(f32, {
            "cvtsi2ss %eax, %xmm0",
            "movd %xmm0, %eax",
        }));
        add_operator_implementation(new OperatorSignature(i64, f32), new BuiltinOperator(f32, {
            "cvtsi2ss %rax, %xmm0",
            "movd %xmm0, %eax",
        }));

        //convert unsigned integer into float?
    }
    
    void init_primitives() {
        _void = new BaseType("void");
        add_primitive_basetype(_void);

        init_int_primitives();
        init_float_primitives();
    }
}
