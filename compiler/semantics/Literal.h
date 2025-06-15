
#pragma once
#include "../parser/parser.h"

struct Type;

struct Literal;
struct IntegerLiteral;
struct SizeofLiteral;
struct CharLiteral;
struct StringLiteral;

struct Literal {
    static Literal* convert(parser::literal *n);
    virtual Type* resolve_type() = 0;
    virtual void emit_asm() = 0;    //some inline assembly to initialize this literal into %rax
};

struct IntegerLiteral : public Literal {
    int val;
    IntegerLiteral(int _val) {
        val = _val;
    }
    Type* resolve_type() override;
    void emit_asm() override;
};

struct SizeofLiteral : public Literal {
    Type *type;
    SizeofLiteral(Type *_type) {
        type = _type;
    }
    Type* resolve_type() override;
    void emit_asm() override;
};

struct CharLiteral : public Literal {
    char val;
    CharLiteral(char _val) {
        val = _val;
    }
    Type* resolve_type() override;
    void emit_asm() override;
};

struct StringLiteral : public Literal {
    std::string val;
    StringLiteral(std::string _val) {
        val = _val;
    }
    Type* resolve_type() override;
    void emit_asm() override;
};