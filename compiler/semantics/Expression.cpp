#include "Expression.h"

#include "Literal.h"
#include "FunctionCall.h"
#include "Identifier.h"
#include "Type.h"
#include "utils.h"
#include "Function.h"
#include "ConstructorCall.h"
#include "TemplateMapping.h"

// -- CONSTRUCTOR --
ExprPrimary::ExprPrimary(val_t _val) {
    val = _val;
}

ExprBinary::ExprBinary(ExprNode *_left, op_t _op, ExprNode *_right) {
    left = _left;
    op = _op;
    right = _right;
    assert(left != nullptr);
    assert(right != nullptr);
}

ExprPrefix::ExprPrefix(op_t _op, ExprNode *_right) {
    op = _op;
    right = _right;
    assert(right != nullptr);
}

ExprPostfix::ExprPostfix(ExprNode *_left, op_t _op) {
    left = _left;
    op = _op;
    assert(left != nullptr);
}

Expression::Expression(ExprNode *_expr_node) {
    has_elaborated = false;
    expr_node = _expr_node;
}

// -- CONVERT --
ExprNode* ExprNode::convert(parser::expr_primary *e) {
    ExprPrimary::val_t val;
    if(e->is_a0) {
        val = Literal::convert(e->t0->t0);
    }
    else if(e->is_a1) {
        val = ConstructorCall::convert(e->t1->t0);
    }
    else if(e->is_a2) {
        val = FunctionCall::convert(e->t2->t0);
    }
    else if(e->is_a3) {
        val = Identifier::convert(e->t3->t0);
    }
    else if(e->is_a4) {
        val = Expression::convert(e->t4->t2);
    }
    else assert(false);
    return new ExprPrimary(val);
}

ExprNode* ExprNode::convert(parser::expr_postfix *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        if(e->t1[i]->t1->is_c0) {       //indexing
            Expression *expr = Expression::convert(e->t1[i]->t1->t0->t2);
            left = new ExprPostfix(left, expr);
        }
        else if(e->t1[i]->t1->is_c1) {  //call member function
            std::string op = ".";
            FunctionCall *fc = FunctionCall::convert(e->t1[i]->t1->t1->t2);
            left = new ExprPostfix(left, std::make_pair(op, fc));
        }
        else if(e->t1[i]->t1->is_c2) {  //dereference, call member function
            std::string op = "->";
            FunctionCall *fc = FunctionCall::convert(e->t1[i]->t1->t2->t2);
            left = new ExprPostfix(left, std::make_pair(op, fc));
        }
        else if(e->t1[i]->t1->is_c3) {  //access member variable
            std::string op = ".";
            Identifier *id = Identifier::convert(e->t1[i]->t1->t3->t2);
            left = new ExprPostfix(left, std::make_pair(op, id));
        }
        else if(e->t1[i]->t1->is_c4) {  //dereference, access member variable
            std::string op = "->";
            Identifier *id = Identifier::convert(e->t1[i]->t1->t4->t2);
            left = new ExprPostfix(left, std::make_pair(op, id));
        }
        else if(e->t1[i]->t1->is_c5) {  //postfix increment
            std::string op = "++";
            left = new ExprPostfix(left, op);
        }
        else if(e->t1[i]->t1->is_c6) {  //postfix decrement
            std::string op = "--";
            left = new ExprPostfix(left, op);
        }
        else assert(false);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_unary *e) {
    if(e->is_a0) {  //unary operator + unary expression
        std::string op = e->t0->t0->to_string();
        ExprNode *right = ExprNode::convert(e->t0->t2);
        return new ExprPrefix(op, right);
    }
    else if(e->is_a1) { //postfix expression
        return ExprNode::convert(e->t1->t0);
    }
    else assert(false);
}

ExprNode* ExprNode::convert(parser::expr_multiplicative *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_additive *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_shift *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_relational *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_equality *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_bit_and *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_bit_xor *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_bit_or *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_logical_and *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}   

ExprNode* ExprNode::convert(parser::expr_logical_or *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_assignment *e) {
    std::vector<ExprNode*> nodes;
    nodes.push_back(ExprNode::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++) nodes.push_back(ExprNode::convert(e->t1[i]->t3));

    ExprNode *right = nodes[nodes.size() - 1];
    for(int i = (int) nodes.size() - 2; i >= 0; i--){
        std::string op = e->t1[i]->t1->to_string();
        right = new ExprBinary(nodes[i], op, right);
    }
    return right;
}

Expression* Expression::convert(parser::expression *e) {
    ExprNode *expr_node = ExprNode::convert(e->t0);
    return new Expression(expr_node);
}

// -- RESOLVE_TYPE --
Type* ExprPrimary::resolve_type() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *f = std::get<FunctionCall*>(val);
        Type *res = f->resolve_type();
        if(res == nullptr) {
            std::cout << "Function call does not resolve to type : " << f->to_string() << "\n";
            return nullptr;
        }

        //if this is a reference, automatically dereference it
        if(dynamic_cast<ReferenceType*>(res)) {
            res = dynamic_cast<ReferenceType*>(res)->type;
        }
        return res;
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        Type *res = c->resolve_type();
        if(res == nullptr){
            std::cout << "Constructor call does not resolve to type : " << c->to_string() << "\n";
            return nullptr;
        }
        return res;
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        Type *res = find_variable_type(id);
        if(res == nullptr) {
            std::cout << "Unable to find variable : " << id->name << "\n";
            return nullptr;
        }

        //if this is a reference, automatically dereference it
        if(dynamic_cast<ReferenceType*>(res)) {
            res = dynamic_cast<ReferenceType*>(res)->type;
        }
        return res;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return l->resolve_type();
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->resolve_type();
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *res = std::get<Type*>(val);

        //if this is a reference, automatically dereference it
        if(dynamic_cast<ReferenceType*>(res)) {
            res = dynamic_cast<ReferenceType*>(res)->type;
        }
        return res;
    }
    else assert(false);
}

Type* ExprBinary::resolve_type() {
    Type *lt = left->resolve_type(), *rt = right->resolve_type();
    if(lt == nullptr || rt == nullptr) return nullptr;    
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //try to find overload / builtin
        Type *otype = find_resulting_type(left, str_op, right);
        if(otype != nullptr) {
            //auto dereference reference type
            if(auto x = dynamic_cast<ReferenceType*>(otype)) {
                otype = x->type;
            }
            return otype;
        }

        //default behaviour
        if(str_op == "=") {
            if(!left->is_lvalue()) {
                std::cout << "Cannot assign to r-value\n";
                return nullptr;
            }
            OperatorImplementation *oe = find_typecast_implementation(rt, lt);
            if(oe == nullptr) {
                std::cout << "No direct conversion from " << rt->to_string() << " to " << lt->to_string() << "\n";
                return nullptr;
            }
            return lt;
        }

        std::cout << "Binary operator " << lt->to_string() << " " << str_op << " " << rt->to_string() << " does not exist\n";
        return nullptr;
    }   
    else assert(false);
}

Type* ExprPrefix::resolve_type() {
    Type *rt = right->resolve_type();
    if(rt == nullptr) return nullptr;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //try to find overload / builtin
        Type *otype = find_resulting_type(std::nullopt, str_op, right);
        if(otype != nullptr) {
            //auto dereference reference type
            if(auto x = dynamic_cast<ReferenceType*>(otype)) {
                otype = x->type;
            }
            return otype;
        }

        //default behaviour
        if(str_op == "*") {
            Type *res;
            if(dynamic_cast<PointerType*>(rt)) res = dynamic_cast<PointerType*>(rt)->type;
            else if(dynamic_cast<ReferenceType*>(rt)) res = dynamic_cast<ReferenceType*>(rt)->type;
            else {
                std::cout << "Cannot dereference non-reference type " << rt->to_string() << "\n";
                return nullptr;
            }
            return res;
        }

        std::cout << "Prefix operator " << str_op << " " << rt->to_string() << " does not exist\n";
        return nullptr;
    }
    else assert(false);
}

Type* ExprPostfix::resolve_type() {
    Type *lt = left->resolve_type();
    if(lt == nullptr) return nullptr;
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        Type *et = expr->resolve_type();
        if(et == nullptr) return nullptr;

        //try to find overload
        Type *otype = find_resulting_type(left, "[]", expr->expr_node);
        if(otype != nullptr) {
            //auto dereference reference type
            if(auto x = dynamic_cast<ReferenceType*>(otype)) {
                otype = x->type;
            }
            return otype;
        }

        //default behaviour
        if(*et != BaseType("int")) {
            std::cout << "Builtin indexing expression must resolve to int\n";
            return nullptr;
        }
        if(dynamic_cast<PointerType*>(lt) == nullptr) {
            std::cout << "Can't index into non-pointer type " << lt->to_string() << "\n";
            return nullptr;
        }
        Type *nt = dynamic_cast<PointerType*>(lt)->type;
        return nt;
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        FunctionCall *fc = p.second;  

        //dereference
        if(p.first == "->") {
            if(dynamic_cast<PointerType*>(lt) == nullptr) {
                std::cout << "Trying to dereference non-pointer type " << lt->to_string() << "\n";
                return nullptr;
            }
            lt = dynamic_cast<PointerType*>(lt)->type;
        }

        //member function call
        fc = new FunctionCall(lt, fc->id, fc->argument_list);
        Type *nt = fc->resolve_type();
        if(fc->resolve_type() == nullptr) {
            std::cout << "Failed to resolve type of function call\n";
            return nullptr;
        }

        //if resulting type is a reference, should dereference
        if(dynamic_cast<ReferenceType*>(nt)) {
            nt = dynamic_cast<ReferenceType*>(nt)->type;
        }

        return nt;
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        Identifier *id = p.second;
        
        //dereference
        if(p.first == "->") {
            if(dynamic_cast<PointerType*>(lt) == nullptr) {
                std::cout << "Trying to dereference non-pointer type " << lt->to_string() << "\n";
                return nullptr;
            }
            lt = dynamic_cast<PointerType*>(lt)->type;
        }

        //member variable access
        StructLayout *sl = get_struct_layout(lt);
        if(sl == nullptr) {
            std::cout << "Unable to find StructLayout for " << lt->to_string() << "\n";
            return nullptr;
        }
        if(sl->get_type(id) == nullptr) {
            std::cout << "Unknown member variable identifier \"" << id->name << "\" for type " << lt->to_string() << "\n";
            return nullptr;
        }
        Type *nt = sl->get_type(id);

        //if resulting type is a reference, should dereference
        if(dynamic_cast<ReferenceType*>(nt)) {
            nt = dynamic_cast<ReferenceType*>(nt)->type;
        }

        return nt;
    }
    else if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //try to find overload / builtin
        Type *otype = find_resulting_type(left, str_op, std::nullopt);
        if(otype != nullptr) {
            //auto dereference reference type
            if(auto x = dynamic_cast<ReferenceType*>(otype)) {
                otype = x->type;
            }
            return otype;
        }

        std::cout << "Postfix operator " << lt->to_string() << " " << str_op << " does not exist\n";
        return nullptr;
    }
    else assert(false);
}

Type* Expression::resolve_type() {
    return expr_node->resolve_type();
}

// -- IS_LVALUE --
bool ExprPrimary::is_lvalue() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        Type *t = fc->resolve_type();
        if(t == nullptr) return false;
        //if return type is a reference, it gets auto-dereferenced. 
        if(dynamic_cast<ReferenceType*>(t) != nullptr) return true;
        return false;
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        return false;
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        return true;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        return false;
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->is_lvalue();
    }
    else if(std::holds_alternative<Type*>(val)) {
        return true;
    }
    else assert(false);
}

bool ExprBinary::is_lvalue() {
    Type *lt = left->resolve_type(), *rt = right->resolve_type();
    if(lt == nullptr || rt == nullptr) return false;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //find overload / builtin
        Type *otype = find_resulting_type(left, str_op, right);
        if(otype != nullptr) {
            return dynamic_cast<ReferenceType*>(otype) != nullptr;
        }
        
        //default behaviour
        return false;
    }   
    else assert(false);
}

bool ExprPrefix::is_lvalue() {
    Type *rt = right->resolve_type();
    if(!right->is_lvalue()) return false;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //find overload / builtin
        Type *otype = find_resulting_type(std::nullopt, str_op, right);
        if(otype != nullptr) {
            return dynamic_cast<ReferenceType*>(otype) != nullptr;
        }

        // default behaviour
        if(str_op == "*") {
            return true;
        }

        return false;
    }
    else assert(false);
}

bool ExprPostfix::is_lvalue() {
    Type *lt = left->resolve_type();
    if(lt == nullptr) return false;
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        Type *et = expr->resolve_type();
        if(et == nullptr) return false;
        
        //find overload
        Type *otype = find_resulting_type(left, "[]", expr->expr_node);
        if(otype != nullptr) {
            return dynamic_cast<ReferenceType*>(otype) != nullptr;
        }

        //builtin indexing
        return true;
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        FunctionCall *fc = p.second;  

        //dereference
        if(p.first == "->") {
            if(dynamic_cast<PointerType*>(lt) == nullptr) {
                return false;
            }
            lt = dynamic_cast<PointerType*>(lt)->type;
        }

        //member function call
        fc = new FunctionCall(lt, fc->id, fc->argument_list);
        Type *nt = fc->resolve_type();
        if(fc->resolve_type() == nullptr) {
            return false;
        }

        //if the resulting type is a reference, this is an l-value
        if(dynamic_cast<ReferenceType*>(nt)) return true;
        return false;
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        return true;
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        std::string str_op = std::get<std::string>(op);

        //find overload
        Type *otype = find_resulting_type(left, str_op, std::nullopt);
        if(otype != nullptr) {
            return dynamic_cast<ReferenceType*>(otype) != nullptr;
        }

        //default behaviour
        return false;
    }
    else assert(false);
}

bool Expression::is_lvalue() {
    return expr_node->is_lvalue();
}

// -- ELABORATE --
void ExprPrimary::elaborate(ExprNode*& self) {
    if(std::holds_alternative<FunctionCall*>(val)) {
        //do nothing
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        //do nothing
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        //do nothing
    }
    else if(std::holds_alternative<Literal*>(val)) {
        //do nothing
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *expr = std::get<Expression*>(val);
        expr->elaborate();
    }
    else if(std::holds_alternative<Type*>(val)) {
        //do nothing
    }
    else assert(false);
}

void ExprBinary::elaborate(ExprNode*& self) {
    left->elaborate(left);
    right->elaborate(right);
    
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //convert function overloads into function calls
        OperatorImplementation *oe = find_operator_implementation(left, str_op, right);
        if(auto fo = dynamic_cast<FunctionOperator*>(oe)) {
            OperatorOverload *f = fo->function;
            FunctionCall *fc = new FunctionCall(f->id, {new Expression(left), new Expression(right)});
            self = new ExprPrimary(fc);
            return;
        }

        //wrap assignment of non-primitive l-values in copy constructor call
        if(str_op == "=") {
            Type *rt = right->resolve_type();
            if(!is_type_primitive(rt) && right->is_lvalue()) {
                ConstructorCall *copy = new ConstructorCall(rt->make_copy(), {new Expression(right)});
                right = new ExprPrimary(copy);
            }
        }
    }
}

void ExprPrefix::elaborate(ExprNode*& self) {
    //when I implement casting, should turn casts into function calls
    right->elaborate(right);

    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //convert function overloads into function calls
        OperatorImplementation *oe = find_operator_implementation(std::nullopt, str_op, right);
        if(auto fo = dynamic_cast<FunctionOperator*>(oe)) {
            OperatorOverload *f = fo->function;
            FunctionCall *fc = new FunctionCall(f->id, {new Expression(right)});
            self = new ExprPrimary(fc);
            return;
        }

        return;
    }
    else assert(false);
}

void ExprPostfix::elaborate(ExprNode*& self) { 
    left->elaborate(left);

    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        assert(expr->resolve_type() != nullptr);

        //convert function overloads into function calls
        OperatorImplementation *oe = find_operator_implementation(left, "[]", expr->expr_node);
        if(auto fo = dynamic_cast<FunctionOperator*>(oe)) {
            OperatorOverload *f = fo->function;
            FunctionCall *fc = new FunctionCall(f->id, {new Expression(left), expr});
            self = new ExprPrimary(fc);
            return;
        }
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {
        //do nothing
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {
        //do nothing
    }
    else if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //convert function overloads into function calls
        OperatorImplementation *oe = find_operator_implementation(left, str_op, std::nullopt);
        if(auto fo = dynamic_cast<FunctionOperator*>(oe)) {
            OperatorOverload *f = fo->function;
            FunctionCall *fc = new FunctionCall(f->id, {new Expression(left)});
            self = new ExprPrimary(fc);
            return;
        }
    }
    else assert(false);
}

void Expression::elaborate() {
    if(has_elaborated) {
        std::cout << "ALREADY ELABORATED : " << to_string() << "\n";
        assert(false);
    }
    std::cout << "ELABORATING : " << to_string() << "\n";
    has_elaborated = true;
    expr_node->elaborate(expr_node);
}

// -- EMIT_ASM -- 
void ExprPrimary::emit_asm() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *f = std::get<FunctionCall*>(val);
        f->emit_asm();

        Type *ft = f->resolve_type();
        assert(ft != nullptr);
        
        //if this is a reference, dereference it
        if(dynamic_cast<ReferenceType*>(ft)) {
            emit_dereference(ft);
        }
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        c->emit_asm();
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        Variable *v = get_variable(id);
        assert(v != nullptr);

        fout << indent() << "mov " << v->stack_offset << "(%rbp), %rax\n";  //value
        fout << indent() << "lea " << v->stack_offset << "(%rbp), %rcx\n";  //address

        //if this is a reference, dereference it
        Type *vt = v->type;
        if(dynamic_cast<ReferenceType*>(vt)) {
            emit_dereference(vt);
        }
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        l->emit_asm();
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        e->expr_node->emit_asm();   //bypass elaboration
    }
    else if(std::holds_alternative<Type*>(val)) {
        assert(false);  //Type* is just for checking if an expression resolves to something. 
    }
    else assert(false);
}

void ExprBinary::emit_asm() {
    Type *lt = left->resolve_type(), *rt = right->resolve_type();
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        if(str_op == "||") {    //logical or
            std::string label = create_new_label();
            left->emit_asm();

            //see if we should short circuit
            fout << indent() << "cmp $0, %rax\n";
            fout << indent() << "jne " << label << "\n";

            //if we don't short circuit, evaluate right branch and merge
            {
                emit_push("%rax", "ExprBinary::emit_asm() : || save left");
                right->emit_asm();
                fout << indent() << "mov %rax, %rbx\n";
                emit_pop("%rax", "ExprBinary::emit_asm() : || save left");

                OperatorImplementation *oe = find_operator_implementation(left, str_op, right);
                assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
                dynamic_cast<BuiltinOperator*>(oe)->emit_asm();
            }

            fout << label << ":\n";
        }
        else if(str_op == "&&") {   //logical and
            std::string label = create_new_label();
            left->emit_asm();

            //see if we should short circuit
            fout << indent() << "cmp $0, %rax\n";
            fout << indent() << "je " << label << "\n";

            //if we don't short circuit, evaluate right branch and merge
            {
                emit_push("%rax", "ExprBinary::emit_asm() : && save left");
                right->emit_asm();
                fout << indent() << "mov %rax, %rbx\n";
                emit_pop("%rax", "ExprBinary::emit_asm() : && save left");

                OperatorImplementation *oe = find_operator_implementation(left, str_op, right);
                assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
                dynamic_cast<BuiltinOperator*>(oe)->emit_asm();
            }

            fout << label << ":\n";
        }
        else if(str_op == "=") {
            //eval right r-value
            right->emit_asm();

            //cast right into left
            OperatorImplementation *oe = find_typecast_implementation(rt, lt);
            assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
            dynamic_cast<BuiltinOperator*>(oe)->emit_asm();

            //save value
            emit_push("%rax", "ExprBinary::emit_asm() : = save right");

            //evaluate left l-value
            left->emit_asm();

            //move right value into left mem location
            int sz = lt->calc_size();
            fout << indent() << "mov %rcx, %rbx\n";
            emit_pop("%rax", "ExprBinary::emit_asm() : = save right");
            emit_mem_store(sz);
        }
        else {
            right->emit_asm();
            emit_push("%rax", "ExprBinary::emit_asm() : save right");
            left->emit_asm();
            emit_pop("%rbx", "ExprBinary::emit_asm() : save right");
            
            OperatorImplementation *oe = find_operator_implementation(left, str_op, right);
            assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
            dynamic_cast<BuiltinOperator*>(oe)->emit_asm();

            //if this is a reference, dereference it
            Type *res = oe->res_type;
            if(dynamic_cast<ReferenceType*>(res) != nullptr) {
                emit_dereference(res);
            }
        }
    }
    else assert(false);
}

void ExprPrefix::emit_asm() {
    Type *rt = right->resolve_type();
    right->emit_asm();

    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        assert(str_op != "(cast)"); //these should be handled seperately
        if(str_op == "*"){ 
            if(dynamic_cast<PointerType*>(rt)) rt = dynamic_cast<PointerType*>(rt)->type;
            else if(dynamic_cast<ReferenceType*>(rt)) rt = dynamic_cast<ReferenceType*>(rt)->type;
            else assert(false);
            
            int sz = rt->calc_size();
            fout << indent() << "mov %rax, %rcx\n";
            emit_mem_retrieve(sz); 
        }
        else {
            OperatorImplementation *oe = find_operator_implementation(std::nullopt, str_op, right);
            assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
            dynamic_cast<BuiltinOperator*>(oe)->emit_asm();

            //if this is a reference, dereference it
            Type *res = oe->res_type;
            if(dynamic_cast<ReferenceType*>(res) != nullptr) {
                emit_dereference(res);
            }
        }
    }
    else assert(false);
}

void ExprPostfix::emit_asm() {
    Type *lt = left->resolve_type();

    //evaluate left expr
    left->emit_asm();

    if(std::holds_alternative<Expression*>(op)) {   //indexing
        //t must be PointerType
        assert(dynamic_cast<PointerType*>(lt) != nullptr);
        lt = (dynamic_cast<PointerType*>(lt))->type;

        //save %rax, %rcx to stack
        emit_push("%rax", "ExprPostfix::emit_asm() : [] %rax 1");
        emit_push("%rcx", "ExprPostfix::emit_asm() : [] %rcx 1");

        //evaluate expression
        Expression *expr = std::get<Expression*>(op);
        assert(expr != nullptr);
        expr->emit_asm();

        //move expression value to %rbx
        fout << indent() << "mov %rax, %rbx\n";

        //return old %rax, %rcx
        emit_pop("%rcx", "ExprPostfix::emit_asm() : [] %rcx 1");
        emit_pop("%rax", "ExprPostfix::emit_asm() : [] %rax 1");

        //save array start
        emit_push("%rax", "ExprPostfix::emit_asm() : [] %rax 2");

        //find sizeof struct
        int sz = lt->calc_size();

        //compute array element addr
        emit_address_array(sz);
        fout << indent() << "mov %rax, %rcx\n";

        //return array start
        emit_pop("%rax", "ExprPostfix::emit_asm() : [] %rax 2");

        //retrieve array element data
        emit_retrieve_array(sz);
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        FunctionCall *fc = p.second;

        //dereference
        if(p.first == "->") {
            fout << indent() << "mov %rax, %rcx\n";
            fout << indent() << "movq (%rax), %rax\n";

            assert(dynamic_cast<PointerType*>(lt) != nullptr);
            lt = dynamic_cast<PointerType*>(lt)->type;
            assert(lt != nullptr);
        }

        //member function call
        fc = new FunctionCall(lt, fc->id, fc->argument_list);
        Function *f = fc->resolve_called_function();
        assert(f != nullptr);

        //this is no longer l-value, so don't have to maintain %rcx
        //reference to type is in %rax, so emit function call
        fc->emit_asm();

        //if the return value is a reference, auto-dereference it
        Type *ft = f->type;
        if(dynamic_cast<ReferenceType*>(ft)) {
            emit_dereference(ft);
        }
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        Identifier *id = p.second;

        //dereference
        if(p.first == "->") {
            fout << indent() << "mov %rax, %rcx\n";
            fout << indent() << "movq (%rax), %rax\n";

            assert(dynamic_cast<PointerType*>(lt) != nullptr);
            lt = dynamic_cast<PointerType*>(lt)->type;
            assert(lt != nullptr);
        }

        //member variable access
        StructLayout *sl = get_struct_layout(lt);
        assert(sl != nullptr);
        assert(sl->get_type(id) != nullptr);

        std::cout << "TRY TO GET MEMBER VARIABLE : " << id->name << "\n";

        int offset = sl->get_offset(id);
        int sz = lt->calc_size();
        fout << indent() << "lea " << offset << "(%rax), %rcx\n";
        fout << indent() << "lea " << offset << "(%rax), %rax\n";
        emit_mem_retrieve(sz);  //now, %rax = member variable, %rcx = address of member variable

        //if the return value is a reference, auto-dereference it
        Type *vt = sl->get_type(id);
        if(dynamic_cast<ReferenceType*>(vt)) {
            emit_dereference(vt);
        }
    }
    else if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        OperatorImplementation *oe = find_operator_implementation(left, str_op, std::nullopt);
        assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
        dynamic_cast<BuiltinOperator*>(oe)->emit_asm();

        //if this is a reference, dereference it
        Type *res = oe->res_type;
        if(dynamic_cast<ReferenceType*>(res) != nullptr) {
            emit_dereference(res);
        }
    }
    else assert(false);
}

void Expression::emit_asm() {
    elaborate();
    expr_node->emit_asm();
}

// -- TO_STRING --
std::string ExprPrimary::to_string() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        return fc->to_string();
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        return c->to_string();
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        return id->name;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return "<lit>";
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return "(" + e->to_string() + ")";
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *t = std::get<Type*>(val);
        return t->to_string();
    }
    else assert(false);
}

std::string ExprBinary::to_string() {
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        return left->to_string() + " " + str_op + " " + right->to_string();
    }
    else assert(false);
}

std::string ExprPrefix::to_string() {
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        return str_op + right->to_string();
    }
    else assert(false);
}

std::string ExprPostfix::to_string() {
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        return left->to_string() + "[" + expr->to_string() + "]";
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        return left->to_string() + p.first + p.second->to_string();
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        return left->to_string() + p.first + p.second->name;
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        std::string str_op = std::get<std::string>(op);
        return left->to_string() + str_op;
    }
    else assert(false);
}

std::string Expression::to_string() {
    return expr_node->to_string();
}

// -- HASH --
size_t ExprPrimary::hash() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        return fc->hash();
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        return c->hash();
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        return id->hash();
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return l->hash();
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->hash();
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *t = std::get<Type*>(val);
        return t->hash();
    }
    else assert(false);
}

size_t ExprBinary::hash() {
    size_t hash = 0;
    hash_combine(hash, left->hash());
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        hash_combine(hash, std::hash<std::string>()(str_op));
    }
    else assert(false);
    hash_combine(hash, right->hash());
    return hash;
}

size_t ExprPrefix::hash() {
    size_t hash = 0;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        hash_combine(hash, std::hash<std::string>()(str_op));
    }
    else assert(false);
    hash_combine(hash, right->hash());
    return hash;
}

size_t ExprPostfix::hash() {
    size_t hash = 0;
    hash_combine(hash, left->hash());
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        hash_combine(hash, std::hash<std::string>()("[]"));
        hash_combine(hash, expr->hash());
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        hash_combine(hash, std::hash<std::string>()(p.first));
        hash_combine(hash, p.second->hash());
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        hash_combine(hash, std::hash<std::string>()(p.first));
        hash_combine(hash, p.second->hash());
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        std::string str_op = std::get<std::string>(op);
        hash_combine(hash, std::hash<std::string>()(str_op));
    }
    else assert(false);
    return hash;
}

size_t Expression::hash() {
    return expr_node->hash();
}

// -- EQUALS --
bool ExprPrimary::equals(ExprNode* _other) {
    if(dynamic_cast<ExprPrimary*>(_other) == nullptr) return false;
    ExprPrimary *other = dynamic_cast<ExprPrimary*>(_other);

    if(val.index() != other->val.index()) return false;
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        FunctionCall *ofc = std::get<FunctionCall*>(other->val);
        return fc->equals(ofc);
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        ConstructorCall *oc = std::get<ConstructorCall*>(other->val);
        return c->equals(oc);
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        Identifier *oid = std::get<Identifier*>(other->val);
        return id->equals(oid);
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        Literal *ol = std::get<Literal*>(other->val);
        return l->equals(ol);
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        Expression *oe = std::get<Expression*>(other->val);
        return e->equals(oe);
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *t = std::get<Type*>(val);
        Type *ot = std::get<Type*>(other->val);
        return t->equals(ot);
    }
    else assert(false);
}

bool ExprBinary::equals(ExprNode* _other) {
    if(dynamic_cast<ExprBinary*>(_other) == nullptr) return false;
    ExprBinary *other = dynamic_cast<ExprBinary*>(_other);

    if(!left->equals(other->left)) return false;
    if(!right->equals(other->right)) return false;

    if(op.index() != other->op.index()) return false;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        std::string ostr_op = std::get<std::string>(other->op);
        return str_op == ostr_op;
    }
    else assert(false);
}

bool ExprPrefix::equals(ExprNode* _other) {
    if(dynamic_cast<ExprPrefix*>(_other) == nullptr) return false;
    ExprPrefix *other = dynamic_cast<ExprPrefix*>(_other);

    if(!right->equals(other->right)) return false;

    if(op.index() != other->op.index()) return false;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        std::string ostr_op = std::get<std::string>(other->op);
        return str_op == ostr_op;
    }
    else assert(false);
}

bool ExprPostfix::equals(ExprNode* _other) {
    if(dynamic_cast<ExprPostfix*>(_other) == nullptr) return false;
    ExprPostfix *other = dynamic_cast<ExprPostfix*>(_other);

    if(!left->equals(other->left)) return false;

    if(op.index() != other->op.index()) return false;
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        Expression *oexpr = std::get<Expression*>(other->op);
        return expr->equals(oexpr);
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        std::pair<std::string, FunctionCall*> op = std::get<std::pair<std::string, FunctionCall*>>(other->op);
        return p.first == op.first && p.second->equals(op.second);
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        std::pair<std::string, Identifier*> op = std::get<std::pair<std::string, Identifier*>>(other->op);
        return p.first == op.first && p.second->equals(op.second);
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        std::string str_op = std::get<std::string>(op);
        std::string ostr_op = std::get<std::string>(other->op);
        return str_op == ostr_op;
    }
    else assert(false);
}

bool Expression::equals(Expression* other) {
    return expr_node->equals(other->expr_node);
}

// -- ID TO TYPE --
void ExprPrimary::id_to_type() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        
        //just turn all arguments of the function call to type
        for(int i = 0; i < fc->argument_list.size(); i++){
            fc->argument_list[i]->id_to_type();
        }
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);

        //turn all arguments of function call to type
        for(int i = 0; i < c->argument_list.size(); i++){
            c->argument_list[i]->id_to_type();
        }
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);   

        //retrieve identifier type and replace
        Type *t = find_variable_type(id);
        assert(t != nullptr);
        val = t;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        // do nothing
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        e->id_to_type();
    }
    else if(std::holds_alternative<Type*>(val)) {
        // we good here
    }
    else assert(false);
}

void ExprBinary::id_to_type() {
    left->id_to_type();
    right->id_to_type();
}

void ExprPrefix::id_to_type() {
    right->id_to_type();
}

void ExprPostfix::id_to_type() {
    left->id_to_type();
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        expr->id_to_type();
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        // do nothing
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        // do nothing
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        // do nothing
    }
    else assert(false);
}

void Expression::id_to_type() {
    expr_node->id_to_type();
}

// -- COPY --
ExprNode* ExprPrimary::make_copy() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        return new ExprPrimary(fc->make_copy());
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        return new ExprPrimary(c->make_copy());
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        return new ExprPrimary(id->make_copy());
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return new ExprPrimary(l->make_copy());
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return new ExprPrimary(e->make_copy());
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *t = std::get<Type*>(val);
        return new ExprPrimary(t->make_copy());
    }
    else assert(false);
}

ExprNode* ExprBinary::make_copy() {
    return new ExprBinary(left->make_copy(), op, right->make_copy());
}

ExprNode* ExprPrefix::make_copy() {
    return new ExprPrefix(op, right->make_copy());
}

ExprNode* ExprPostfix::make_copy() {
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        return new ExprPostfix(left->make_copy(), expr->make_copy());
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        return new ExprPostfix(left->make_copy(), std::make_pair(p.first, p.second->make_copy()));
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        return new ExprPostfix(left->make_copy(), std::make_pair(p.first, p.second->make_copy()));
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        std::string str_op = std::get<std::string>(op);
        return new ExprPostfix(left->make_copy(), str_op);
    }   
    else assert(false);
}

Expression* Expression::make_copy() {
    return new Expression(expr_node->make_copy());
}

// -- REPLACE TEMPLATED TYPES --
bool ExprPrimary::replace_templated_types(TemplateMapping *mapping) {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        return fc->replace_templated_types(mapping);
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        return c->replace_templated_types(mapping);
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        //do nothing 
        return true;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        //do nothing
        return true;
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->replace_templated_types(mapping);
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *t = std::get<Type*>(val);
        return t->replace_templated_types(mapping);
    }
    else assert(false);
}

bool ExprBinary::replace_templated_types(TemplateMapping *mapping) {
    if(!left->replace_templated_types(mapping)) return false;
    if(!right->replace_templated_types(mapping)) return false;
    if(std::holds_alternative<std::string>(op)) {
        //do nothing
    }
    else assert(false);
    return true;
}

bool ExprPrefix::replace_templated_types(TemplateMapping *mapping) {
    if(!right->replace_templated_types(mapping)) return false;
    if(std::holds_alternative<std::string>(op)) {
        //do nothing
    }
    else assert(false);
    return true;
}

bool ExprPostfix::replace_templated_types(TemplateMapping *mapping) {
    if(!left->replace_templated_types(mapping)) return false;
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        if(!expr->replace_templated_types(mapping)) return false;
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        if(!p.second->replace_templated_types(mapping)) return false;
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        //do nothing
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        std::string str_op = std::get<std::string>(op);
        //do nothing
    }
    else assert(false);
    return true;
}

bool Expression::replace_templated_types(TemplateMapping *mapping) {
    return expr_node->replace_templated_types(mapping);
}