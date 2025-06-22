#include "Statement.h"
#include "Expression.h"
#include "Declaration.h"
#include "Function.h"
#include "Type.h"
#include "Identifier.h"
#include "FunctionSignature.h"
#include "utils.h"

// -- CONSTRUCTOR --
DeclarationStatement::DeclarationStatement(Declaration *_declaration) {
    declaration = _declaration;
}

ExpressionStatement::ExpressionStatement(Expression *_expr) {
    expr = _expr;
}

ReturnStatement::ReturnStatement(Expression* expr) {
    if(expr == nullptr) opt_expr = std::nullopt;
    else opt_expr = expr;
}

IfStatement::IfStatement(std::vector<Expression*> _exprs, std::vector<Statement*> _statements, Statement *_else_statement) {
    exprs = _exprs;
    statements = _statements;
    if(_else_statement == nullptr) else_statement = std::nullopt;
    else else_statement = _else_statement;
    assert(statements.size() >= 0);
    assert(exprs.size() == statements.size());
}

WhileStatement::WhileStatement(Expression *_expr, Statement *_statement) {
    expr = _expr;
    statement = _statement;
}

ForStatement::ForStatement(Declaration *_declaration, Expression *_expr1, Expression *_expr2, Statement *_statement) {
    if(_declaration == nullptr) declaration = std::nullopt;
    else declaration = _declaration;
    if(_expr1 == nullptr) expr1 = std::nullopt;
    else expr1 = _expr1;
    if(_expr2 == nullptr) expr2 = std::nullopt;
    else expr2 = _expr2;
    statement = _statement;
}

CompoundStatement::CompoundStatement(std::vector<Statement*> _statements) {
    statements = _statements;
}

// -- CONVERT --
Statement* Statement::convert(parser::statement *s) {
    if(s->is_a0) {  //simple statement
        return SimpleStatement::convert(s->t0->t0);
    }
    else if(s->is_a1) { //control statement
        return ControlStatement::convert(s->t1->t0);
    }
    else if(s->is_a2) { //compound statement
        return CompoundStatement::convert(s->t2->t0);
    }
    else assert(false);
}

SimpleStatement* SimpleStatement::convert(parser::simple_statement *s) {
    if(s->is_a0) { //return 
        Expression *expr = nullptr;
        if(s->t0->t1 != nullptr) {
            //non-void return
            expr = Expression::convert(s->t0->t1->t1);
        }
        return new ReturnStatement(expr);
    }
    else if(s->is_a1) {  //declaration
        Declaration *declaration = Declaration::convert(s->t1->t0);
        return new DeclarationStatement(declaration);
    }
    else if(s->is_a2) { //expression
        Expression *expr = Expression::convert(s->t2->t0);
        return new ExpressionStatement(expr);
    }
    else assert(false);
}

ControlStatement* ControlStatement::convert(parser::control_statement *s) {
    if(s->is_a0) {  //if statement
        std::vector<Statement*> statements;
        std::vector<Expression*> exprs;
        Statement *else_statement = nullptr;
        parser::control_statement *ptr = s;
        while(true) {
            assert(ptr->is_a0);
            exprs.push_back(Expression::convert(ptr->t0->t4));
            statements.push_back(Statement::convert(ptr->t0->t8));
            if(ptr->t0->t9 == nullptr) break;   //else doesn't exist
            Statement* tmp_stmt = Statement::convert(ptr->t0->t9->t3);
            if(!ptr->t0->t9->t3->is_a1) {  //statement isn't control statement
                else_statement = Statement::convert(ptr->t0->t9->t3);
                break;
            }
            parser::control_statement *nxt_ptr = ptr->t0->t9->t3->t1->t0;
            if(!nxt_ptr->is_a0) {   //statement isn't if statement
                else_statement = Statement::convert(ptr->t0->t9->t3);
                break;
            }
            ptr = nxt_ptr;
        }
        return new IfStatement(exprs, statements, else_statement);
    }
    else if(s->is_a1){  //while statement
        Expression *expr = Expression::convert(s->t1->t4);
        Statement *statement = Statement::convert(s->t1->t8);
        return new WhileStatement(expr, statement);
    }
    else if(s->is_a2) { //for statement
        Declaration *declaration = nullptr;
        if(s->t2->t4 != nullptr) declaration = Declaration::convert(s->t2->t4->t0);
        Expression *expr1 = nullptr;
        if(s->t2->t8 != nullptr) expr1 = Expression::convert(s->t2->t8->t0);
        Expression *expr2 = nullptr;
        if(s->t2->t12 != nullptr) expr2 = Expression::convert(s->t2->t12->t0);
        Statement *statement = Statement::convert(s->t2->t16);
        return new ForStatement(declaration, expr1, expr2, statement);
    }
    else assert(false);
}

CompoundStatement* CompoundStatement::convert(parser::compound_statement *s) {
    std::vector<Statement*> statements;
    std::vector<parser::compound_statement::a0*> slist = s->t2;
    for(int i = 0; i < slist.size(); i++){
        statements.push_back(Statement::convert(slist[i]->t0));
    }
    return new CompoundStatement(statements);
}

// -- IS ALWAYS RETURNING --
bool DeclarationStatement::is_always_returning() {
    return false;
}

bool ExpressionStatement::is_always_returning() {
    return false;
}

bool ReturnStatement::is_always_returning() {
    return true;
}

bool IfStatement::is_always_returning() {
    //every statement must return 
    for(int i = 0; i < statements.size(); i++){
        if(!statements[i]->is_always_returning()) return false;
    }
    if(else_statement.has_value() && !else_statement.value()->is_always_returning()) return false;
    return true;
}

bool WhileStatement::is_always_returning() {
    return statement->is_always_returning();
}

bool ForStatement::is_always_returning() {
    return statement->is_always_returning();
}

bool CompoundStatement::is_always_returning() {
    //for now, don't worry about unreachable code
    for(int i = 0; i < statements.size(); i++){
        if(statements[i]->is_always_returning()) return true;
    }
    return false;
}

// -- IS WELL FORMED --
bool DeclarationStatement::is_well_formed() {
    // - is the declaration well formed?
    if(!declaration->is_well_formed()) {
        std::cout << "Declaration not well formed\n";
        return false;
    }
    return true;
}

bool ExpressionStatement::is_well_formed() {
    // - does the expression resolve to a type?
    Type *t = expr->resolve_type();
    if(t == nullptr) {
        std::cout << "Expression does not resolve to type\n";
        return false;
    }

    // - does the type actually exist?
    if(!is_type_declared(t)) {
        std::cout << "Type " << t->to_string() << " does not exist\n";
        return false;
    }

    expr->emit_asm();

    return true;
}

bool ReturnStatement::is_well_formed() {
    // - if we are not in a function right now (constructor), make sure this is a void return
    if(enclosing_function == nullptr) {
        if(opt_expr.has_value()) {
            return false;
        }
        return true;
    }

    // - does the expression resolve to a type?
    Type *et = nullptr, *ft = enclosing_function->type;
    if(opt_expr.has_value()) {
        Expression *expr = opt_expr.value();
        et = expr->resolve_type();
    }
    else et = new BaseType("void");
    if(et == nullptr) {
        std::cout << "Return expression does not resolve to type\n";
        return false;
    }
    // - are we trying to return something when the function is returning void?
    if(*ft == BaseType("void") && *et != BaseType("void")) {
        std::cout << "Non-void return expression in void function\n";
        return false;
    }
    
    //see if we need to return something
    if(*ft != BaseType("void")) {
        assert(opt_expr.has_value());
        Expression *expr = opt_expr.value();
        Identifier *vid = new Identifier(create_new_tmp_variable_name());
        push_declaration_stack();

        // - can the expression return type be assigned to the return type of enclosing function?
        Variable *v = emit_initialize_variable(ft, vid, expr);
        if(v == nullptr) {
            std::cout << "Return expression cannot be cast to function return type, " << et->to_string() << " -> " << ft->to_string() << "\n";
            return false;
        }

        //put value of declared variable into %rax
        fout << indent() << "mov " << v->stack_offset << "(%rbp), %rax\n";

        //clean up temp variable
        pop_declaration_stack();
    }

    //clean up local variables
    if(declared_variables.size() != 0) {
        fout << indent() << "add $" << -local_offset << ", %rsp\n"; //should not be managed by local_offset
    }

    if(FunctionSignature(new Identifier("main"), {}) == *(enclosing_function->resolve_function_signature())) {
        //function is main, use exit syscall instead
        fout << indent() << "push %rax\n";  //should not be managed by local_offset
        fout << indent() << "call sys_exit\n";
    }
    else {
        //return from function
        fout << indent() << "pop %rbp\n";   //should not be managed by local_offset
        fout << indent() << "ret\n";
    }

    return true;
}

bool IfStatement::is_well_formed() {
    // - do all expressions resolve to type 'int'?
    for(int i = 0; i < exprs.size(); i++){
        Type *t = exprs[i]->resolve_type();
        if(t == nullptr || *t != BaseType("int")) {
            return false;
        }
    }

    if(asm_debug) fout << indent() << "# if statement start\n";
    std::vector<std::string> labels;
    for(int i = 0; i < statements.size(); i++){
        labels.push_back(create_new_label());
    }
    std::string else_label = "no-label";
    if(else_statement.has_value()) else_label = create_new_label();
    std::string end_if_label = create_new_label();

    //check each expression one by one to see where we should jump to
    for(int i = 0; i < exprs.size(); i++){
        exprs[i]->emit_asm();
        fout << indent() << "cmp $0, %rax\n";
        fout << indent() << "jne " << labels[i] << "\n";
    }
    if(else_statement.has_value()) fout << indent() << "jmp " << else_label << "\n";
    else fout << indent() << "jmp " << end_if_label << "\n";

    // - are all of the statements well formed?
    for(int i = 0; i < statements.size(); i++){
        fout << labels[i] << ":\n";
        if(!statements[i]->is_well_formed()) {
            return false;
        }
        fout << indent() << "jmp " << end_if_label << "\n";
    }

    // - is else statement well formed?
    if(else_statement.has_value()) {
        fout << else_label << ":\n";
        if(!else_statement.value()->is_well_formed()) {
            return false;
        }
        fout << indent() << "jmp " << end_if_label << "\n";
    }

    fout << end_if_label << ":\n";
    if(asm_debug) fout << indent() << "# if statement end\n";

    return true;
}

bool WhileStatement::is_well_formed() {
    // - does the expression resolve to type 'int'?
    Type *t = expr->resolve_type();
    if(t == nullptr || *t != BaseType("int")) {
        return false;
    }

    if(asm_debug) fout << indent() << "# while loop start\n";

    std::string loop_start_label = create_new_label();
    std::string loop_end_label = create_new_label();
    fout << loop_start_label << ":\n";

    //check loop condition
    expr->emit_asm();
    fout << indent() << "cmp $0, %rax\n";
    fout << indent() << "je " << loop_end_label << "\n";

    // - is the statement well formed?
    if(!statement->is_well_formed()) {
        return false;
    }

    //jump to start of loop
    fout << indent() << "jmp " << loop_start_label << "\n";

    //end of loop
    fout << loop_end_label << ":\n";

    if(asm_debug) fout << indent() << "# while loop end\n";

    return true;
}

bool ForStatement::is_well_formed() {
    push_declaration_stack();

    if(asm_debug) fout << indent() << "# for loop start\n";

    // - is declaration well formed?   
    if(declaration.has_value()) {
        if(!declaration.value()->is_well_formed()) {
            return false;
        }
    }

    // - does the conditional expression resolve to type 'int'?
    if(expr1.has_value()) {
        Type *t = expr1.value()->resolve_type();
        if(t == nullptr || *t != BaseType("int")) {
            std::cout << "For loop conditional expression must resolve to type int\n";
            return false;
        }
    }
    // - is the assignment expression well formed?
    if(expr2.has_value()) {
        if(expr2.value()->resolve_type() == nullptr) {
            std::cout << "For loop assignment expression not well formed\n";
            return false;
        }
    }

    //start of loop
    std::string loop_start_label = create_new_label();
    std::string loop_end_label = create_new_label();
    fout << loop_start_label << ":\n";

    //check loop condition
    if(expr1.has_value()) {
        expr1.value()->emit_asm();
        fout << indent() << "cmp $0, %rax\n";
        fout << indent() << "je " << loop_end_label << "\n";
    }

    // - is the statement well formed?
    if(!statement->is_well_formed()) {
        return false;
    }

    //loop variable assignment
    if(expr2.has_value()) expr2.value()->emit_asm();

    //jump to start of loop
    fout << indent() << "jmp " << loop_start_label << "\n";

    //end of loop
    fout << loop_end_label << ":\n";

    if(asm_debug) fout << indent() << "# for loop end\n";

    pop_declaration_stack();
    return true;
}

bool CompoundStatement::is_well_formed() {
    push_declaration_stack();

    // - are all statements within well formed?
    for(int i = 0; i < statements.size(); i++){
        if(!statements[i]->is_well_formed()) {
            return false;
        }
    }

    pop_declaration_stack();
    return true;
}
