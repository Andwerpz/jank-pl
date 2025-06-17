
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
    virtual size_t hash() = 0;
    virtual bool equals(Literal *other) = 0;
    virtual Literal* make_copy() = 0;
};

struct IntegerLiteral : public Literal {
    int val;
    IntegerLiteral(int _val);
    
    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
};

struct SizeofLiteral : public Literal {
    Type *type;
    SizeofLiteral(Type *_type);

    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
};

struct CharLiteral : public Literal {
    char val;
    CharLiteral(char _val);

    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
};

struct StringLiteral : public Literal {
    std::string val;
    StringLiteral(std::string _val);

    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
};

