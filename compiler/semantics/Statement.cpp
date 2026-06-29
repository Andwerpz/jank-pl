#include "Statement.h"
#include "Expression.h"
#include "Declaration.h"
#include "Function.h"
#include "Type.h"
#include "Identifier.h"
#include "FunctionSignature.h"
#include "utils.h"
#include "primitives.h"
#include "Literal.h"
#include "InlineASMAccess.h"
#include "Operator.h"

// -- CONVERT CONSTRUCTOR --
Statement::Statement(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

SimpleStatement::SimpleStatement(parser::token *tok) : Statement(tok) {
    // do nothing
}

DeclarationStatement::DeclarationStatement(parser::token *tok) : SimpleStatement(tok) {
    // do nothing
}

ExpressionStatement::ExpressionStatement(parser::token *tok) : SimpleStatement(tok) {
    // do nothing
}

ReturnStatement::ReturnStatement(parser::token *tok) : SimpleStatement(tok) {
    // do nothing
}

InlineASMStatement::InlineASMStatement(parser::token *tok) : SimpleStatement(tok) {
    // do nothing
}

BreakStatement::BreakStatement(parser::token *tok) : SimpleStatement(tok) {
    // do nothing
}

ContinueStatement::ContinueStatement(parser::token *tok) : SimpleStatement(tok) {
    // do nothing
}

ControlStatement::ControlStatement(parser::token *tok) : Statement(tok) {
    // do nothing
}

IfStatement::IfStatement(parser::token *tok) : ControlStatement(tok) {
    // do nothing
}

WhileStatement::WhileStatement(parser::token *tok) : ControlStatement(tok) {
    // do nothing
}

ForStatement::ForStatement(parser::token *tok) : ControlStatement(tok) {
    // do nothing
}

CompoundStatement::CompoundStatement(parser::token *tok) : Statement(tok) {
    // do nothing
}

// -- COPY CONSTRUCTOR --
Statement::Statement(const Statement& other) : ASTNode(other) {
    // do nothing
}

SimpleStatement::SimpleStatement(const SimpleStatement& other) : Statement(other) {
    // do nothing
}

DeclarationStatement::DeclarationStatement(const DeclarationStatement& other) : SimpleStatement(other) {
    declaration = other.declaration->make_copy();
}

ExpressionStatement::ExpressionStatement(const ExpressionStatement& other) : SimpleStatement(other) {
    expr = other.expr->make_copy();
}

ReturnStatement::ReturnStatement(const ReturnStatement& other) : SimpleStatement(other) {
    opt_expr = std::nullopt;
    if(other.opt_expr.has_value()) opt_expr = other.opt_expr.value()->make_copy();
}

InlineASMStatement::InlineASMStatement(const InlineASMStatement& other) : SimpleStatement(other) {
    for(int i = 0; i < other.tokens.size(); i++) {
        const std::variant<std::string, InlineASMAccess*>& tok = other.tokens[i];
        if(std::holds_alternative<std::string>(tok)) {
            std::string str = std::get<std::string>(tok);
            tokens.push_back(str);
        }   
        else if(std::holds_alternative<InlineASMAccess*>(tok)) {
            InlineASMAccess* ia = std::get<InlineASMAccess*>(tok);
            tokens.push_back(ia->make_copy());
        }
        else assert(false);
    }
}

BreakStatement::BreakStatement(const BreakStatement& other) : SimpleStatement(other) {
    // do nothing
}

ContinueStatement::ContinueStatement(const ContinueStatement& other) : SimpleStatement(other) {
    // do nothing
}

ControlStatement::ControlStatement(const ControlStatement& other) : Statement(other) {
    // do nothing
}

IfStatement::IfStatement(const IfStatement& other) : ControlStatement(other) {
    expr = other.expr->make_copy();
    statement = other.statement->make_copy();
    else_statement = std::nullopt;
    if(other.else_statement.has_value()) else_statement = other.else_statement.value()->make_copy();
}

WhileStatement::WhileStatement(const WhileStatement& other) : ControlStatement(other) {
    expr = other.expr->make_copy();
    statement = other.statement->make_copy();
}

ForStatement::ForStatement(const ForStatement& other) : ControlStatement(other) {
    declaration = std::nullopt;
    if(other.declaration.has_value()) declaration = other.declaration.value()->make_copy();
    expr1 = std::nullopt;
    if(other.expr1.has_value()) expr1 = other.expr1.value()->make_copy();
    expr2 = std::nullopt;
    if(other.expr2.has_value()) expr2 = other.expr2.value()->make_copy();
    statement = other.statement->make_copy();
}

CompoundStatement::CompoundStatement(const CompoundStatement& other) : Statement(other) {
    for(int i = 0; i < other.statements.size(); i++) {
        statements.push_back(other.statements[i]->make_copy());
    }
}

// -- SYNTHESIS CONSTRUCTOR --
Statement::Statement() : ASTNode() {
    // do nothing
}

SimpleStatement::SimpleStatement() : Statement() {
    // do nothing
}

DeclarationStatement::DeclarationStatement(Declaration *_declaration) : SimpleStatement() {
    declaration = _declaration;
}

ExpressionStatement::ExpressionStatement(Expression *_expr) : SimpleStatement() {
    expr = _expr;
}

ReturnStatement::ReturnStatement(std::optional<Expression*> expr) : SimpleStatement() {
    opt_expr = expr;
    if(opt_expr.has_value()) assert(opt_expr.value() != nullptr);
}

InlineASMStatement::InlineASMStatement(std::vector<std::variant<std::string, InlineASMAccess*>> _tokens) : SimpleStatement() {
    tokens = _tokens;
}

BreakStatement::BreakStatement() : SimpleStatement() {
    // do nothing
}

ContinueStatement::ContinueStatement() : SimpleStatement() {
    // do nothing
}

ControlStatement::ControlStatement() : Statement() {
    // do nothing
}

IfStatement::IfStatement(Expression *_expr, Statement *_statement, std::optional<Statement*> _else_statement) : ControlStatement() {
    expr = _expr;
    statement = _statement;
    else_statement = _else_statement;
    assert(expr != nullptr);
    assert(statement != nullptr);
    if(else_statement.has_value()) assert(else_statement.value() != nullptr);
}

WhileStatement::WhileStatement(Expression *_expr, Statement *_statement) : ControlStatement() {
    expr = _expr;
    statement = _statement;
}

ForStatement::ForStatement(
    std::optional<Declaration*> _declaration, 
    std::optional<Expression*> _expr1, 
    std::optional<Expression*> _expr2, 
    Statement *_statement
) : ControlStatement() {
    declaration = _declaration;
    expr1 = _expr1;
    expr2 = _expr2;
    statement = _statement;
    if(declaration != std::nullopt) assert(declaration.value() != nullptr);
    if(expr1 != std::nullopt) assert(expr1.value() != nullptr);
    if(expr2 != std::nullopt) assert(expr2.value() != nullptr);
    assert(statement != nullptr);
}

CompoundStatement::CompoundStatement(std::vector<Statement*> _statements) : Statement() {
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
        ReturnStatement* result = new ReturnStatement(s);
        result->opt_expr = std::nullopt;
        if(s->t0->t1.has_value()) {
            //non-void return
            result->opt_expr = Expression::convert(s->t0->t1.value()->t1);
        }
        return result;
    }
    else if(s->is_a1) { //break 
        return new BreakStatement(s);
    }
    else if(s->is_a2) { //continue
        return new ContinueStatement(s);
    }
    else if(s->is_a3) {  //declaration
        DeclarationStatement* result = new DeclarationStatement(s);
        result->declaration = Declaration::convert(s->t3->t0);
        return result;
    }
    else if(s->is_a4) { //expression
        ExpressionStatement* result = new ExpressionStatement(s);
        result->expr = Expression::convert(s->t4->t0);
        return result;
    }
    else if(s->is_a5) { //inline asm
        InlineASMStatement* result = new InlineASMStatement(s);
        parser::inline_asm_string *str = s->t5->t0->t3;
        for(int i = 0; i < str->t1.size(); i++){
            if(str->t1[i]->is_b0) {         //inline access
                result->tokens.push_back(InlineASMAccess::convert(str->t1[i]->t0->t0));
            }
            else if(str->t1[i]->is_b1) {    //string
                result->tokens.push_back(str->t1[i]->t1->to_string());
            }
            else assert(false);
        }
        return result;
    }
    else assert(false);
}

ControlStatement* ControlStatement::convert(parser::control_statement *s) {
    if(s->is_a0) {  //if statement
        IfStatement* result = new IfStatement(s);
        result->expr = Expression::convert(s->t0->t4);
        result->statement = Statement::convert(s->t0->t8);
        result->else_statement = std::nullopt;
        if(s->t0->t9.has_value()) {
            result->else_statement = Statement::convert(s->t0->t9.value()->t3);
        }
        return result;
    }
    else if(s->is_a1){  //while statement
        WhileStatement* result = new WhileStatement(s);
        result->expr = Expression::convert(s->t1->t4);
        result->statement = Statement::convert(s->t1->t8);
        return result;
    }
    else if(s->is_a2) { //for statement
        ForStatement* result = new ForStatement(s);
        result->declaration = std::nullopt;
        if(s->t2->t4.has_value()) result->declaration = Declaration::convert(s->t2->t4.value()->t0);
        result->expr1 = std::nullopt;
        if(s->t2->t8.has_value()) result->expr1 = Expression::convert(s->t2->t8.value()->t0);
        result->expr2 = std::nullopt;
        if(s->t2->t12.has_value()) result->expr2 = Expression::convert(s->t2->t12.value()->t0);
        result->statement = Statement::convert(s->t2->t16);
        return result;
    }
    else assert(false);
}

CompoundStatement* CompoundStatement::convert(parser::compound_statement *s) {
    CompoundStatement* result = new CompoundStatement(s);
    std::vector<parser::compound_statement::a0*> slist = s->t2;
    for(int i = 0; i < slist.size(); i++){
        assert(slist[i]->t0->is_c0);
        result->statements.push_back(Statement::convert(slist[i]->t0->t0->t0));
    }
    return result;
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

bool InlineASMStatement::is_always_returning() {
    //not really accurate, but undefined behaviour is up to the user
    return false;
}

bool BreakStatement::is_always_returning() {
    return false;
}

bool ContinueStatement::is_always_returning() {
    return false;
}

bool IfStatement::is_always_returning() {
	//if we don't have else, then we're not guaranteed to go into any branch
	if(!else_statement.has_value()) return false;
	
    //every statement must return 
    // TODO need to check if we're guaranteed to go into some if statement
    if(!statement->is_always_returning()) return false;
    if(!else_statement.value()->is_always_returning()) return false;
	
    return true;
}

bool WhileStatement::is_always_returning() {
    return false;	//TODO need to check if we actually enter the loop
}

bool ForStatement::is_always_returning() {
    return false;	//TODO need to check if we actually enter the loop
}

bool CompoundStatement::is_always_returning() {
    //for now, don't worry about unreachable code
    for(int i = 0; i < statements.size(); i++){
        //if this is a break or continue statement, the rest of this block doesn't matter
        if(dynamic_cast<BreakStatement*>(statements[i])) return false;
        if(dynamic_cast<ContinueStatement*>(statements[i])) return false;

        //otherwise, see if it's always returning. 
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
        std::cout << "Expression does not resolve to type : " << expr->to_string() << "\n";
        return false;
    }

    // - does the type actually exist?
    if(!is_type_declared(t)) {
        std::cout << "Type " << t->to_string() << " does not exist\n";
        return false;
    }

    expr->emit_asm(true);

    return true;
}

bool ReturnStatement::is_well_formed() {
    //list of variables we shouldn't clean after returning 
    // (either we returned them, or the variable is 'this' and we have an enclosing type)
    std::vector<Identifier*> escape;

    //need to escape 'this' in struct member functions
    if(enclosing_type.has_value()) {
        escape.push_back(new Identifier("this"));
    }

    // - see if we need to return
    assert(enclosing_return_type != nullptr);
    if(enclosing_return_type->equals(primitives::_void)) {
        if(opt_expr.has_value()) {
            std::cout << "Non-void return on a void return type function\n";
            return false;
        }
    }
    else {
        //otherwise, need to see if we need to return 
        // - does a return expression exist?
        if(!opt_expr.has_value()) {
            std::cout << "Void return on a non-void return type function\n";
            return false;
        }

        // - does the expression resolve to a type?
        Expression *expr = opt_expr.value();
        assert(expr != nullptr);
        Type *et = opt_expr.value()->resolve_type();
        Type *ft = enclosing_return_type->make_copy();
        if(et == nullptr) {
            std::cout << "Return expression does not resolve to type\n";
            return false;
        }

        //if we're directly returning a variable from this function's execution context
        //we can just return it directly without copying, just make sure to not destruct it
        {
            if(!et->equals(ft)) {
                //return type doesn't match function type, probably need some overload
                goto default_return;
            }

            ExprPrimary* ep = dynamic_cast<ExprPrimary*>(expr->expr_node);
            if(ep == nullptr) {
                //expression is a complex statement
                goto default_return;
            }
            
            if(!std::holds_alternative<Identifier*>(ep->val)) {
                //primary expression isn't just a variable
                goto default_return;
            }

            Identifier *vid = std::get<Identifier*>(ep->val);
            Variable *v = get_variable(vid);
            assert(v != nullptr);
            if(v->is_global) {
                //have to make copy of global variable when returning
                goto default_return;
            }

            //found a local variable return
            fout << indent() << "mov " << v->addr << ", %rax\n";
            escape.push_back(vid);

            goto return_done;
        }
        
        default_return: {
            //create temp stack frame for temp variable
            push_declaration_stack();

            // - can the expression return type be assigned to the return type of enclosing function?
            Identifier *vid = new Identifier(create_new_tmp_variable_name());
            Variable *v = emit_initialize_stack_variable(ft, vid, expr);
            if(v == nullptr) {
                std::cout << "Return expression cannot be cast to function return type, " << et->to_string() << " -> " << ft->to_string() << "\n";
                return false;
            }

            //put value of declared variable into %rax
            fout << indent() << "mov " << v->addr << ", %rax\n";

            //clean up temp variable (but don't dealloc)
            pop_declaration_stack(false);
        }
        return_done: {}
    }

    // - do cleanup and return from function. 
    //clean up local variables + function arguments
    for(int i = (int) declaration_stack.size() - 1; i >= 0; i--) {
        emit_cleanup_declaration_stack_layer(i, escape);
    }

    //reset %rsp
    if(local_offset != 0) {
        fout << indent() << "add $" << -local_offset << ", %rsp\n"; //should not be managed by local_offset
    }

    //return from function
    fout << indent() << "pop %rbp\n";   //should not be managed by local_offset
    fout << indent() << "ret\n";

    return true;
}

bool InlineASMStatement::is_well_formed() {
    //we should only have one InlineASMAccess
    int access_cnt = 0;
    std::string asm_str = "";
    for(int i = 0; i < tokens.size(); i++){
        if(std::holds_alternative<std::string>(tokens[i])) {
            std::string tok = std::get<std::string>(tokens[i]);
            asm_str += tok;
        }
        else if(std::holds_alternative<InlineASMAccess*>(tokens[i])) {
            if(access_cnt != 0) {
                std::cout << "Cannot have more than 1 inline access\n";
                return false;
            }

            InlineASMAccess *access = std::get<InlineASMAccess*>(tokens[i]);
            if(!access->is_well_formed()) {
                std::cout << "InlineASMAccess not well formed\n";
                return false;
            }
            access->emit_asm();
            asm_str += access->get_addr();
        }
        else assert(false);
    }

    //let the assembler decide
    fout << indent() << asm_str << "\n";
    return true;
}

bool BreakStatement::is_well_formed() {
    // - are we inside a loop?
    if(loop_stack.size() == 0) {
        std::cout << "Break statement must be inside a loop\n";
        return false;
    }

    //grab the loop context
    LoopContext *lc = *(loop_stack.rbegin());
    assert(lc != nullptr);

    //clean up local variables
    int dl = lc->declaration_layer;
    int tot_sz = 0;
    for(int i = (int) declaration_stack.size() - 1; i > dl; i--) {
        //free heap structs
        emit_cleanup_declaration_stack_layer(i);

        tot_sz += declaration_stack[i].size() * 8;
    }
    fout << indent() << "add $" << tot_sz << ", %rsp\n";    //should not be managed by local_offset

    //jump to loop end label
    fout << indent() << "jmp " << lc->end_label << "\n";

    return true;
}

bool ContinueStatement::is_well_formed() {
    // - are we inside a loop?
    if(loop_stack.size() == 0) {
        std::cout << "Continue statement must be inside a loop\n";
        return false;
    }

    //grab the loop context
    LoopContext *lc = *(loop_stack.rbegin());
    assert(lc != nullptr);

    //clean up local variables
    int dl = lc->declaration_layer;
    int tot_sz = 0;
    for(int i = (int) declaration_stack.size() - 1; i > dl; i--) {
        //free heap structs
        emit_cleanup_declaration_stack_layer(i);

        tot_sz += declaration_stack[i].size() * 8;
    }
    fout << indent() << "add $" << tot_sz << ", %rsp\n";    //should not be managed by local_offset

    //jump to loop assignment label
    fout << indent() << "jmp " << lc->assignment_label << "\n";

    return true;
}

bool IfStatement::is_well_formed() {
    // - does the expression resolve to nonvoid?
    Type *t = expr->resolve_type();
    if(t == nullptr || t->equals(primitives::_void)) {
        std::cout << "If statement expression must resolve to non-void type\n";
        return false;
    }

    //create labels
    if(asm_debug) fout << indent() << "# if statement start\n";
    std::string if_label = create_new_label();
    std::string else_label = "no-label";
    if(else_statement.has_value()) else_label = create_new_label();
    std::string end_if_label = create_new_label();

    //check if we should jump into if statement
    expr->emit_asm(true);
    fout << indent() << "cmp $0, %rax\n";
    fout << indent() << "jne " << if_label << "\n";
    if(else_statement.has_value()) fout << indent() << "jmp " << else_label << "\n";
    else fout << indent() << "jmp " << end_if_label << "\n";

    // - is if statement well formed?
    //each statement should implicitly introduce a scope
    fout << if_label << ":\n";
    push_declaration_stack();
    if(!statement->is_well_formed()) {
        return false;
    }
    pop_declaration_stack();
    fout << indent() << "jmp " << end_if_label << "\n";

    // - is else statement well formed?
    if(else_statement.has_value()) {
        fout << else_label << ":\n";
        push_declaration_stack();
        if(!else_statement.value()->is_well_formed()) {
            return false;
        }
        pop_declaration_stack();
        fout << indent() << "jmp " << end_if_label << "\n";
    }

    fout << end_if_label << ":\n";
    if(asm_debug) fout << indent() << "# if statement end\n";

    return true;
}

bool WhileStatement::is_well_formed() {
    // - does the expression resolve to nonvoid?
    Type *t = expr->resolve_type();
    if(t == nullptr || t->equals(primitives::_void)) {
        std::cout << "While loop expression must resolve to nonvoid type\n";
        return false;
    }

    if(asm_debug) fout << indent() << "# while loop start\n";

    std::string loop_start_label = create_new_label();
    std::string loop_assignment_label = create_new_label();
    std::string loop_end_label = create_new_label();
    push_loop_stack(loop_start_label, loop_assignment_label, loop_end_label);
    fout << loop_start_label << ":\n";

    //check loop condition
    expr->emit_asm(true);
    fout << indent() << "cmp $0, %rax\n";
    fout << indent() << "je " << loop_end_label << "\n";

    // - is the statement well formed?
    push_declaration_stack();
    if(!statement->is_well_formed()) {
        return false;
    }
    pop_declaration_stack();

    //loop variable assignment (just for continue)
    fout << loop_assignment_label << ":\n";

    //jump to start of loop
    fout << indent() << "jmp " << loop_start_label << "\n";

    //end of loop
    pop_loop_stack(loop_start_label, loop_assignment_label, loop_end_label);
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

    // - does the conditional expression resolve to nonvoid?
    if(expr1.has_value()) {
        Type *t = expr1.value()->resolve_type();
        if(t == nullptr || t->equals(primitives::_void)) {
            std::cout << "For loop conditional expression must resolve to type\n";
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
    std::string loop_assignment_label = create_new_label();
    std::string loop_end_label = create_new_label();
    push_loop_stack(loop_start_label, loop_assignment_label, loop_end_label);
    fout << loop_start_label << ":\n";

    //check loop condition
    if(expr1.has_value()) {
        expr1.value()->emit_asm(true);
        fout << indent() << "cmp $0, %rax\n";
        fout << indent() << "je " << loop_end_label << "\n";
    }

    // - is the statement well formed?
    push_declaration_stack();
    if(!statement->is_well_formed()) {
        return false;
    }
    pop_declaration_stack();

    //loop variable assignment
    fout << loop_assignment_label << ":\n";
    if(expr2.has_value()) expr2.value()->emit_asm(true);

    //jump to start of loop
    fout << indent() << "jmp " << loop_start_label << "\n";

    //end of loop
    pop_loop_stack(loop_start_label, loop_assignment_label, loop_end_label);
    fout << loop_end_label << ":\n";

    if(asm_debug) fout << indent() << "# for loop end\n";

    pop_declaration_stack();
    return true;
}

bool CompoundStatement::is_well_formed() {
    // - are all statements within well formed?
    push_declaration_stack();
    for(int i = 0; i < statements.size(); i++){
        if(!statements[i]->is_well_formed()) {
            return false;
        }
    }
    pop_declaration_stack();

    return true;
}

// -- MAKE COPY --
Statement* DeclarationStatement::make_copy() {
    return new DeclarationStatement(*this);
}

Statement* ExpressionStatement::make_copy() {
    return new ExpressionStatement(*this);
}

Statement* ReturnStatement::make_copy() {
    return new ReturnStatement(*this);
}

Statement* InlineASMStatement::make_copy() {
    return new InlineASMStatement(*this);
}

Statement* BreakStatement::make_copy(){
    return new BreakStatement(*this);
}

Statement* ContinueStatement::make_copy() {
    return new ContinueStatement(*this);
}

Statement* IfStatement::make_copy() {
    return new IfStatement(*this);
}

Statement* WhileStatement::make_copy() {
    return new WhileStatement(*this);
}

Statement* ForStatement::make_copy() {
    return new ForStatement(*this);
}

Statement* CompoundStatement::make_copy() {
    return new CompoundStatement(*this);
}

// -- REPLACE TEMPLATED TYPES --
bool DeclarationStatement::replace_templated_types(TemplateMapping *mapping) {
    if(!declaration->replace_templated_types(mapping)) return false;
    return true;
}

bool ExpressionStatement::replace_templated_types(TemplateMapping *mapping) {
    if(!expr->replace_templated_types(mapping)) return false;
    return true;
}

bool ReturnStatement::replace_templated_types(TemplateMapping *mapping) {
    if(opt_expr.has_value()) {
        Expression *expr = opt_expr.value();
        if(!expr->replace_templated_types(mapping)) return false;
        opt_expr = expr;
    }
    return true;
}

bool InlineASMStatement::replace_templated_types(TemplateMapping *mapping) {
    return true;    //do nothing
}

bool BreakStatement::replace_templated_types(TemplateMapping *mapping) {
    return true;    //do nothing
}

bool ContinueStatement::replace_templated_types(TemplateMapping *mapping) {
    return true;    //do nothing
}

bool IfStatement::replace_templated_types(TemplateMapping *mapping) {
    if(!expr->replace_templated_types(mapping)) return false;
    if(!statement->replace_templated_types(mapping)) return false;
    if(else_statement.has_value()) {
        if(!else_statement.value()->replace_templated_types(mapping)) return false;
    }
    return true;
}

bool WhileStatement::replace_templated_types(TemplateMapping *mapping) {
    if(!expr->replace_templated_types(mapping)) return false;
    if(!statement->replace_templated_types(mapping)) return false;
    return true;
}

bool ForStatement::replace_templated_types(TemplateMapping *mapping) {
    if(declaration.has_value() && !declaration.value()->replace_templated_types(mapping)) return false;
    if(expr1.has_value() && !expr1.value()->replace_templated_types(mapping)) return false;
    if(expr2.has_value() && !expr2.value()->replace_templated_types(mapping)) return false;
    if(!statement->replace_templated_types(mapping)) return false;
    return true;
}

bool CompoundStatement::replace_templated_types(TemplateMapping *mapping) {
    for(int i = 0; i < statements.size(); i++){
        if(!statements[i]->replace_templated_types(mapping)) return false;
    }
    return true;
}

// -- LOOK FOR TEMPLATES --
bool DeclarationStatement::look_for_templates() {
    return declaration->look_for_templates();
}

bool ExpressionStatement::look_for_templates() {
    return expr->look_for_templates();
}

bool ReturnStatement::look_for_templates() {
    if(opt_expr.has_value()) if(!opt_expr.value()->look_for_templates()) return false;
    return true;
}

bool InlineASMStatement::look_for_templates() {
    return true;    //do nothing
}

bool BreakStatement::look_for_templates() {
    return true;    //do nothing
}

bool ContinueStatement::look_for_templates() {
    return true;    //do nothing
}

bool IfStatement::look_for_templates() {
    if(!expr->look_for_templates()) return false;
    if(!statement->look_for_templates()) return false;
    if(else_statement.has_value()) {
        if(!else_statement.value()->look_for_templates()) return false;
    }
    return true;
}

bool WhileStatement::look_for_templates() {
    if(!expr->look_for_templates()) return false;
    if(!statement->look_for_templates()) return false;
    return true;
}

bool ForStatement::look_for_templates() {
    if(declaration.has_value()) if(!declaration.value()->look_for_templates()) return false;
    if(expr1.has_value()) if(!expr1.value()->look_for_templates()) return false;
    if(expr2.has_value()) if(!expr2.value()->look_for_templates()) return false;
    if(!statement->look_for_templates()) return false;
    return true;
}

bool CompoundStatement::look_for_templates() {
    for(int i = 0; i < statements.size(); i++) if(!statements[i]->look_for_templates()) return false;
    return true;
}

// -- TO STRING --
std::string DeclarationStatement::to_string() {
    return declaration->to_string() + ";";
}

std::string ExpressionStatement::to_string() {
    return expr->to_string() + ";";
}

std::string ReturnStatement::to_string() {
    std::string ret = "return";
    if(opt_expr.has_value()) ret += " " + opt_expr.value()->to_string();
    ret += ";";
    return ret;
}

std::string InlineASMStatement::to_string() {
    std::string asm_str = "asm!(\"";
    for(int i = 0; i < tokens.size(); i++){
        if(std::holds_alternative<std::string>(tokens[i])) {
            std::string tok = std::get<std::string>(tokens[i]);
            asm_str += tok;
        }
        else if(std::holds_alternative<InlineASMAccess*>(tokens[i])) {
            InlineASMAccess *access = std::get<InlineASMAccess*>(tokens[i]);
            asm_str += access->to_string();
        }
        else assert(false);
    }
    asm_str += "\");";
    return asm_str;
}

std::string BreakStatement::to_string() {
    return "break;";
}

std::string ContinueStatement::to_string() {
    return "continue;";
}

std::string IfStatement::to_string() {
    std::string ret = "";
    ret += "if(";
    ret += expr->to_string();
    ret += ") ";
    ret += statement->to_string();
    if(else_statement.has_value()) {
        ret += "\n";
        ret += "else ";
        ret += else_statement.value()->to_string();
    }
    return ret;
}

std::string WhileStatement::to_string() {
    std::string ret = "";
    ret += "while(";
    ret += expr->to_string();
    ret += ") ";
    ret += statement->to_string();
    return ret;
}

std::string ForStatement::to_string() {
    std::string ret = "";
    ret += "for(";
    if(declaration.has_value()) ret += declaration.value()->to_string();
    ret += "; ";
    if(expr1.has_value()) ret += expr1.value()->to_string();
    ret += "; ";
    if(expr2.has_value()) ret += expr2.value()->to_string();
    ret += ") ";
    ret += statement->to_string();
    return ret;
}

std::string CompoundStatement::to_string() {
    std::string ret = "";
    ret += "{\n";
    for(int i = 0; i < statements.size(); i++){
        ret += statements[i]->to_string();
        ret += "\n";
    }
    ret += "}";
    return ret;
}
