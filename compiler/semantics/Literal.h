
#pragma once
#include "../parser/parser.h"

struct Type;

struct Literal;
struct IntegerLiteral;
struct SizeofLiteral;
struct CharLiteral;
struct StringLiteral;
struct TemplateMapping;
struct Expression;

struct Literal {
    static Literal* convert(parser::literal *n);
    virtual Type* resolve_type() = 0;
    virtual void emit_asm() = 0;    //some inline assembly to initialize this literal into %rax
    virtual size_t hash() = 0;
    virtual bool equals(Literal *other) = 0;
    virtual Literal* make_copy() = 0;
    virtual std::string to_string() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
};

struct FloatLiteral : public Literal {
    float val;
    FloatLiteral(float _val);

    static FloatLiteral* convert(parser::literal_float *l);
    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
    std::string to_string() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
};

struct IntegerLiteral : public Literal {
    int val;
    IntegerLiteral(int _val);
    
    static IntegerLiteral* convert(parser::literal_integer *l);
    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
    std::string to_string() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
};

struct SizeofLiteral : public Literal {
    Type *type;
    SizeofLiteral(Type *_type);
    
    static SizeofLiteral* convert(parser::literal_sizeof *l);
    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
    std::string to_string() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
};

struct CharLiteral : public Literal {
    char val;
    CharLiteral(char _val);

    static CharLiteral* convert(parser::literal_char *l);
    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
    std::string to_string() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
};

struct StringLiteral : public Literal {
    std::string val;
    StringLiteral(std::string _val);

    static StringLiteral* convert(parser::literal_string *l);
    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
    std::string to_string() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
};

struct SyscallLiteral : public Literal {
    int syscall_id;
    std::vector<Expression*> arguments;
    Type *type;
    SyscallLiteral(int _syscall_id, std::vector<Expression*> _arguments, Type *_type);

    static SyscallLiteral* convert(parser::literal_syscall *l);
    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
    std::string to_string() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
};

struct HexLiteral : public Literal {
    std::string hex_str;    //doesn't contain 0x
    HexLiteral(std::string _hex_str);

    static HexLiteral* convert(parser::literal_hex *l);
    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
    std::string to_string() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
};

struct BinaryLiteral : public Literal {
    std::string bin_str;    //doesn't contain 0b
    BinaryLiteral(std::string _bin_str);

    static BinaryLiteral* convert(parser::literal_binary *l);
    Type* resolve_type() override;
    void emit_asm() override;
    size_t hash() override;
    bool equals(Literal *other) override;
    Literal* make_copy() override;
    std::string to_string() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
};