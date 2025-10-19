#pragma once
#include "../parser/parser.h"
#include <optional>
#include <vector>
#include <variant>

struct Declaration;
struct Expression;
struct TemplateMapping;
struct InlineASMAccess;

struct Statement {
    static Statement* convert(parser::statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
    virtual Statement* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates() = 0;
    virtual std::string to_string() = 0;
};

struct SimpleStatement : public Statement {
    static SimpleStatement* convert(parser::simple_statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
    virtual Statement* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates() = 0;
    virtual std::string to_string() = 0;
};

struct DeclarationStatement : public SimpleStatement {
    Declaration *declaration;
    DeclarationStatement(Declaration *_declaration);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    std::string to_string() override;
};

struct ExpressionStatement : public SimpleStatement {
    Expression *expr;
    ExpressionStatement(Expression *_expr);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    std::string to_string() override;
};

struct ReturnStatement : public SimpleStatement {
    std::optional<Expression*> opt_expr;
    ReturnStatement(Expression* expr);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    std::string to_string() override;
};

struct InlineASMStatement : public SimpleStatement {
    std::vector<std::variant<std::string, InlineASMAccess*>> tokens;
    InlineASMStatement(std::vector<std::variant<std::string, InlineASMAccess*>> _tokens);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    std::string to_string() override;
};

struct BreakStatement : public SimpleStatement {
    BreakStatement();
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    std::string to_string() override;
};

struct ContinueStatement : public SimpleStatement {
    ContinueStatement();
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    std::string to_string() override;
};

struct ControlStatement : public Statement {
    static ControlStatement* convert(parser::control_statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
    virtual Statement* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates() = 0;
    virtual std::string to_string() = 0;
};

struct IfStatement : public ControlStatement {
    Expression *expr;
    Statement *statement;
    std::optional<Statement*> else_statement;
    IfStatement(Expression *expr, Statement *statement, std::optional<Statement*> else_statement);
    bool is_well_formed() override;
    bool is_always_returning() override;
    Statement* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    std::string to_string() override;
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
    std::string to_string() override;
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
    std::string to_string() override;
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
    std::string to_string() override;
};