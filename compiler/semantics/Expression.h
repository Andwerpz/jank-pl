#pragma once
#include "../parser/parser.h"
#include <string>
#include <variant>

struct ExprNode;
struct ExprPrimary;
struct ExprBinary;
struct ExprPrefix;
struct ExprPostfix;
struct Expression;
struct FunctionCall;
struct Literal;
struct Type;
struct Identifier;
struct FunctionSignature;
struct OperatorSignature;
struct ConstructorCall;
struct TemplateMapping;

struct ExprNode {
    static ExprNode* convert(parser::expr_primary *e);
    static ExprNode* convert(parser::expr_postfix *e);
    static ExprNode* convert(parser::expr_unary *e);
    static ExprNode* convert(parser::expr_multiplicative *e);
    static ExprNode* convert(parser::expr_additive *e);
    static ExprNode* convert(parser::expr_shift *e);
    static ExprNode* convert(parser::expr_relational *e);
    static ExprNode* convert(parser::expr_equality *e);
    static ExprNode* convert(parser::expr_bit_and *e);
    static ExprNode* convert(parser::expr_bit_xor *e);
    static ExprNode* convert(parser::expr_bit_or *e);
    static ExprNode* convert(parser::expr_logical_and *e);
    static ExprNode* convert(parser::expr_logical_or *e);
    static ExprNode* convert(parser::expr_assignment *e);

    virtual Type* resolve_type() = 0;                   // - no modify
    virtual bool is_lvalue() = 0;                       // - no modify

    // - can modify, but should not change return value of resolve_type(), is_lvalue()
    // - should remove all overloads, after this stage the only operators should be hardcoded or Builtin. 
    //   overloads should turn into ExprPrimary function calls. 
    virtual void elaborate(ExprNode*& self) = 0;        
    virtual void emit_asm() = 0;                        // - no modify
    virtual std::string to_string() = 0;
    virtual size_t hash() = 0;
    virtual bool equals(ExprNode* other) = 0;

    // turns all Identifiers in ExprPrimary to Type. Useful for caching results.
    // If you don't do this, you'll be comparing Identifiers. 
    virtual void id_to_type() = 0;
    virtual ExprNode* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual void look_for_templates() = 0;
};

//Type* is just a placeholder for a variable of that type. It's just used for type conversion purposes. 
//if there is a Type* and it tries to emit_asm(), it will assert(false). 
struct ExprPrimary : ExprNode {
    using val_t = std::variant<FunctionCall*, ConstructorCall*, Identifier*, Literal*, Expression*, Type*>;
    val_t val;
    ExprPrimary(val_t _val);

    Type* resolve_type() override;
    bool is_lvalue() override;
    void elaborate(ExprNode*& self) override;
    void emit_asm() override;
    std::string to_string() override;
    size_t hash() override;
    bool equals(ExprNode* other) override;
    void id_to_type() override;
    ExprNode* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    void look_for_templates() override;
};

struct ExprBinary : ExprNode {
    using op_t = std::variant<std::string>;
    ExprNode *left;
    op_t op;
    ExprNode *right;
    ExprBinary(ExprNode *_left, op_t _op, ExprNode *_right);

    Type* resolve_type() override;
    bool is_lvalue() override;
    void elaborate(ExprNode*& self) override;
    void emit_asm() override;
    std::string to_string() override;
    size_t hash() override;
    bool equals(ExprNode* other) override;
    void id_to_type() override;
    ExprNode* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    void look_for_templates() override;
};

struct ExprPrefix : ExprNode {
    using op_t = std::variant<std::string>;
    op_t op;
    ExprNode *right;
    ExprPrefix(op_t _op, ExprNode *_right);

    Type* resolve_type() override;
    bool is_lvalue() override;
    void elaborate(ExprNode*& self) override;
    void emit_asm() override;
    std::string to_string() override;
    size_t hash() override;
    bool equals(ExprNode* other) override;
    void id_to_type() override;
    ExprNode* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    void look_for_templates() override;
};

struct ExprPostfix : ExprNode {
    using op_t = std::variant<Expression*, std::pair<std::string, FunctionCall*>, std::pair<std::string, Identifier*>, std::string>;
    ExprNode *left;
    op_t op;
    ExprPostfix(ExprNode *_left, op_t _op);

    Type* resolve_type() override;
    bool is_lvalue() override;
    void elaborate(ExprNode*& self) override;
    void emit_asm() override;
    std::string to_string() override;
    size_t hash() override;
    bool equals(ExprNode* other) override;
    void id_to_type() override;
    ExprNode* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    void look_for_templates() override;
};

struct Expression {
    bool has_elaborated;    //make sure we only elaborate once
    ExprNode *expr_node;
    Expression(ExprNode *_expr_node);

    static Expression* convert(parser::expression *e);
    Type* resolve_type();
    bool is_lvalue();
    void elaborate();
    void emit_asm();
    std::string to_string();
    size_t hash();
    bool equals(Expression* other);
    void id_to_type();
    Expression* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    void look_for_templates();
};