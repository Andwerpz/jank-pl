#include "Statement.h"
#include "Expression.h"
#include "Declaration.h"
#include "Function.h"
#include "Type.h"
#include "Identifier.h"
#include "FunctionSignature.h"
#include "utils.h"
#include "Overload.h"
#include "primitives.h"
#include "Literal.h"

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

ASMStatement::ASMStatement(std::string _asm_str) {
    asm_str = _asm_str;
}

BreakStatement::BreakStatement() {
    //do nothing
}

ContinueStatement::ContinueStatement() {
    //do nothing
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
    else if(s->is_a1) { //break 
        return new BreakStatement();
    }
    else if(s->is_a2) { //continue
        return new ContinueStatement();
    }
    else if(s->is_a3) {  //declaration
        Declaration *declaration = Declaration::convert(s->t3->t0);
        return new DeclarationStatement(declaration);
    }
    else if(s->is_a4) { //expression
        Expression *expr = Expression::convert(s->t4->t0);
        return new ExpressionStatement(expr);
    }
    else if(s->is_a5) { //inline asm
        std::string asm_str = StringLiteral::convert(s->t5->t4)->val;
        return new ASMStatement(asm_str);
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

bool ASMStatement::is_always_returning() {
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
        std::cout << "Expression does not resolve to type\n";
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
    // - if we are not in a function or overload right now (constructor / destructor), make sure this is a void return
    if(enclosing_function == nullptr && enclosing_overload == nullptr) {
        if(opt_expr.has_value()) {
            std::cout << "Constructor / Destructor return cannot be non-void\n";
            return false;
        }
        return true;
    }

    // - does the expression resolve to a type?
    Type *et = nullptr, *ft = nullptr;
    if(enclosing_function != nullptr) ft = enclosing_function->type;
    else if(enclosing_overload != nullptr) ft = enclosing_overload->type;
    else assert(false);

    if(opt_expr.has_value()) {
        Expression *expr = opt_expr.value();
        et = expr->resolve_type();
    }
    else et = primitives::_void->make_copy();
    if(et == nullptr) {
        std::cout << "Return expression does not resolve to type\n";
        return false;
    }
    // - are we trying to return something when the function is returning void?
    if(ft->equals(primitives::_void) && !et->equals(primitives::_void)) {
        std::cout << "Non-void return expression in void function\n";
        return false;
    }
    
    //see if we need to return something
    if(!ft->equals(primitives::_void)) {
        assert(opt_expr.has_value());
        Expression *expr = opt_expr.value();
        Identifier *vid = new Identifier(create_new_tmp_variable_name());
        push_declaration_stack();

        // - can the expression return type be assigned to the return type of enclosing function?
        Variable *v = emit_initialize_stack_variable(ft, vid, expr);
        if(v == nullptr) {
            std::cout << "Return expression cannot be cast to function return type, " << et->to_string() << " -> " << ft->to_string() << "\n";
            return false;
        }

        //put value of declared variable into %rax
        fout << indent() << "mov " << v->addr << ", %rax\n";

        //clean up temp variable
        pop_declaration_stack(false);
    }

    //clean up local variables
    for(int i = (int) declaration_stack.size() - 1; i >= 1; i--) {  //free heap structs
        emit_cleanup_declaration_stack_layer(i);
    }
    if(local_offset != 0) { //reset %rsp
        fout << indent() << "add $" << -local_offset << ", %rsp\n"; //should not be managed by local_offset
    }

    //special case for exiting out of main
    if(enclosing_function != nullptr && FunctionSignature(new Identifier("main"), {}) == *(enclosing_function->resolve_function_signature())) {
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

bool ASMStatement::is_well_formed() {
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

    //jump to loop assignment label
    fout << indent() << "jmp " << lc->assignment_label << "\n";

    return true;
}

bool IfStatement::is_well_formed() {
    // - do all expressions resolve to nonvoid?
    for(int i = 0; i < exprs.size(); i++){
        Type *t = exprs[i]->resolve_type();
        if(t == nullptr || t->equals(primitives::_void)) {
            std::cout << "If statement expression must resolve to type\n";
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
        exprs[i]->emit_asm(true);
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
    if(!statement->is_well_formed()) {
        return false;
    }

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
    if(!statement->is_well_formed()) {
        return false;
    }

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

// -- MAKE COPY --
Statement* DeclarationStatement::make_copy() {
    return new DeclarationStatement(declaration->make_copy());
}

Statement* ExpressionStatement::make_copy() {
    return new ExpressionStatement(expr->make_copy());
}

Statement* ReturnStatement::make_copy() {
    Expression *_expr = nullptr;
    if(opt_expr.has_value()) _expr = opt_expr.value()->make_copy();
    return new ReturnStatement(_expr);
}

Statement* ASMStatement::make_copy() {
    return new ASMStatement(asm_str);
}

Statement* BreakStatement::make_copy(){
    return new BreakStatement();
}

Statement* ContinueStatement::make_copy() {
    return new ContinueStatement();
}

Statement* IfStatement::make_copy() {
    std::vector<Expression*> _exprs;
    std::vector<Statement*> _statements;
    Statement* _else_statement = nullptr;
    for(int i = 0; i < exprs.size(); i++){
        _exprs.push_back(exprs[i]->make_copy());
    }
    for(int i = 0; i < statements.size(); i++){
        _statements.push_back(statements[i]->make_copy());
    }
    if(else_statement.has_value()) _else_statement = else_statement.value()->make_copy();
    return new IfStatement(_exprs, _statements, _else_statement);
}

Statement* WhileStatement::make_copy() {
    return new WhileStatement(expr->make_copy(), statement->make_copy());
}

Statement* ForStatement::make_copy() {
    Declaration *_declaration = nullptr;
    Expression *_expr1 = nullptr;
    Expression *_expr2 = nullptr;
    Statement *_statement = statement->make_copy();
    if(declaration.has_value()) _declaration = declaration.value()->make_copy();
    if(expr1.has_value()) _expr1 = expr1.value()->make_copy();
    if(expr2.has_value()) _expr2 = expr2.value()->make_copy();
    return new ForStatement(_declaration, _expr1, _expr2, _statement);
}

Statement* CompoundStatement::make_copy() {
    std::vector<Statement*> _statements;
    for(int i = 0; i < statements.size(); i++){
        _statements.push_back(statements[i]->make_copy());
    }
    return new CompoundStatement(_statements);
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

bool ASMStatement::replace_templated_types(TemplateMapping *mapping) {
    return true;    //do nothing
}

bool BreakStatement::replace_templated_types(TemplateMapping *mapping) {
    return true;    //do nothing
}

bool ContinueStatement::replace_templated_types(TemplateMapping *mapping) {
    return true;    //do nothing
}

bool IfStatement::replace_templated_types(TemplateMapping *mapping) {
    for(int i = 0; i < exprs.size(); i++){
        if(!exprs[i]->replace_templated_types(mapping)) return false;
    }
    for(int i = 0; i < statements.size(); i++){
        if(!statements[i]->replace_templated_types(mapping)) return false;
    }
    if(else_statement.has_value() && !else_statement.value()->replace_templated_types(mapping)) return false;
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

bool ASMStatement::look_for_templates() {
    return true;    //do nothing
}

bool BreakStatement::look_for_templates() {
    return true;    //do nothing
}

bool ContinueStatement::look_for_templates() {
    return true;    //do nothing
}

bool IfStatement::look_for_templates() {
    for(int i = 0; i < exprs.size(); i++) if(!exprs[i]->look_for_templates()) return false;
    for(int i = 0; i < statements.size(); i++) if(!statements[i]->look_for_templates()) return false;
    if(else_statement.has_value()) if(!else_statement.value()->look_for_templates()) return false;
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

std::string ASMStatement::to_string() {
    return "asm!(\"" + asm_str + "\");";
}

std::string BreakStatement::to_string() {
    return "break;";
}

std::string ContinueStatement::to_string() {
    return "continue;";
}

std::string IfStatement::to_string() {
    std::string ret = "";
    for(int i = 0; i < exprs.size(); i++){
        if(i == 0) ret += "if(";
        else ret += "else if(";
        ret += exprs[i]->to_string();
        ret += ") ";
        ret += statements[i]->to_string();
        if(i + 1 != exprs.size()) ret += "\n";
    }
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
