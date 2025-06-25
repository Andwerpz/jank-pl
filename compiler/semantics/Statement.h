#pragma once
#include "../parser/parser.h"
#include <optional>
#include <vector>

struct Declaration;
struct Expression;
struct TemplateMapping;

struct Statement {
    static Statement* convert(parser::statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
    virtual Statement* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates() = 0;
};

struct SimpleStatement : public Statement {
    static SimpleStatement* convert(parser::simple_statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
    virtual Statement* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates() = 0;
};

struct DeclarationStatement : public SimpleStatement {
    Declaration *declaration;
    DeclarationStatement(Declaration *_declaration);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
};

struct ExpressionStatement : public SimpleStatement {
    Expression *expr;
    ExpressionStatement(Expression *_expr);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
};

struct ReturnStatement : public SimpleStatement {
    std::optional<Expression*> opt_expr;
    ReturnStatement(Expression* expr);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
};

struct ControlStatement : public Statement {
    static ControlStatement* convert(parser::control_statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
    virtual Statement* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates() = 0;
};

struct IfStatement : public ControlStatement {
    std::vector<Expression*> exprs;
    std::vector<Statement*> statements;
    std::optional<Statement*> else_statement;
    IfStatement(std::vector<Expression*> _exprs, std::vector<Statement*> _statements, Statement *_else_statement);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
};

struct WhileStatement : public ControlStatement {
    Expression *expr;
    Statement *statement;
    WhileStatement(Expression *_expr, Statement *_statement);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
};

struct ForStatement : public ControlStatement {
    std::optional<Declaration*> declaration;
    std::optional<Expression*> expr1;
    std::optional<Expression*> expr2;
    Statement *statement;
    ForStatement(Declaration *_declaration, Expression *_expr1, Expression *_expr2, Statement *_statement);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
};  

struct CompoundStatement : public Statement {
    std::vector<Statement*> statements;
    CompoundStatement(std::vector<Statement*> _statements);
    static CompoundStatement* convert(parser::compound_statement *s);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
};