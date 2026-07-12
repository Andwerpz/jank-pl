#include "Expression.h"

#include "Literal.h"
#include "FunctionCall.h"
#include "Identifier.h"
#include "Type.h"
#include "utils.h"
#include "Function.h"
#include "ConstructorCall.h"
#include "TemplateMapping.h"
#include "OperatorCall.h"
#include "StructLayout.h"
#include "primitives.h"
#include "DestructorCall.h"
#include "Destructor.h"
#include "Operator.h"
#include "CompilationContext.h"
#include "DefinitionSpace.h"

// -- CONVERT CONSTRUCTOR --
ExprNode::ExprNode(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

ExprPrimary::ExprPrimary(parser::token *tok) : ExprNode(tok) {
    // do nothing
}

ExprBinary::ExprBinary(parser::token *tok) : ExprNode(tok) {
    // do nothing
}

ExprPrefix::ExprPrefix(parser::token *tok) : ExprNode(tok) {
    // do nothing
}

ExprPostfix::ExprPostfix(parser::token *tok) : ExprNode(tok) {
    // do nothing
}

Expression::Expression(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

// -- COPY CONSTRUCTOR --
ExprNode::ExprNode(const ExprNode& other) : ASTNode(other) {
    // do nothing
}

ExprPrimary::ExprPrimary(const ExprPrimary& other) : ExprNode(other) {
    if(std::holds_alternative<FunctionCall*>(other.val)) {
        FunctionCall *fc = std::get<FunctionCall*>(other.val);
        val = fc->make_copy();
    }
    else if(std::holds_alternative<ConstructorCall*>(other.val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(other.val);
        val = c->make_copy();
    }
    else if(std::holds_alternative<OperatorCall*>(other.val)) {
        OperatorCall *o = std::get<OperatorCall*>(other.val);
        val = o->make_copy();
    }
    else if(std::holds_alternative<Identifier*>(other.val)) {
        Identifier *id = std::get<Identifier*>(other.val);
        val = id->make_copy();
    }
    else if(std::holds_alternative<Literal*>(other.val)) {
        Literal *l = std::get<Literal*>(other.val);
        val = l->make_copy();
    }
    else if(std::holds_alternative<Expression*>(other.val)) {
        Expression *e = std::get<Expression*>(other.val);
        val = e->make_copy();
    }
    else if(std::holds_alternative<Type*>(other.val)) {
        Type *t = std::get<Type*>(other.val);
        val = t->make_copy();
    }
    else assert(false);
}

ExprBinary::ExprBinary(const ExprBinary& other) : ExprNode(other) {
    left = other.left->make_copy();
    if(std::holds_alternative<std::string>(other.op)) {
        std::string o = std::get<std::string>(other.op);
        op = o;
    }
    else assert(false);
    right = other.right->make_copy();
}

ExprPrefix::ExprPrefix(const ExprPrefix& other) : ExprNode(other) {
    if(std::holds_alternative<std::string>(other.op)) {
        std::string str_op = std::get<std::string>(other.op);
        op = str_op;
    }
    else if(std::holds_alternative<Type*>(other.op)) {
        Type *cast_t = std::get<Type*>(other.op);
        op = cast_t->make_copy();
    }
    else assert(false);
    right = other.right->make_copy();
}

ExprPostfix::ExprPostfix(const ExprPostfix& other) : ExprNode(other) {
    left = other.left->make_copy();
    if(std::holds_alternative<Expression*>(other.op)) {   //indexing
        Expression *expr = std::get<Expression*>(other.op);
        op = expr->make_copy();
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(other.op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(other.op);
        op = std::make_pair(p.first, p.second->make_copy());
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(other.op)) {    //member variable access
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(other.op);
        op = std::make_pair(p.first, p.second->make_copy());
    }
    else if(std::holds_alternative<std::string>(other.op)) {  //postfix increment / decrement
        std::string str_op = std::get<std::string>(other.op);
        op = str_op;
    }   
    else if(std::holds_alternative<std::vector<Expression*>>(other.op)) {
        const std::vector<Expression*>& argument_list = std::get<std::vector<Expression*>>(other.op);
        std::vector<Expression*> _argument_list;
        for(int i = 0; i < argument_list.size(); i++) _argument_list.push_back(argument_list[i]->make_copy());
        op = _argument_list;
    }
    else assert(false);
}

Expression::Expression(const Expression& other) : ASTNode(other) {
    has_elaborated = other.has_elaborated;
    expr_node = other.expr_node->make_copy();
}

// -- SYNTHESIS CONSTRUCTOR --
ExprNode::ExprNode() : ASTNode() {
    // do nothing
}

ExprPrimary::ExprPrimary(val_t _val) : ExprNode() {
    val = _val;
}

ExprBinary::ExprBinary(ExprNode *_left, op_t _op, ExprNode *_right) : ExprNode() {
    left = _left;
    op = _op;
    right = _right;
    assert(left != nullptr);
    assert(right != nullptr);
}

ExprPrefix::ExprPrefix(op_t _op, ExprNode *_right) : ExprNode() {
    op = _op;
    right = _right;
    assert(right != nullptr);
}

ExprPostfix::ExprPostfix(ExprNode *_left, op_t _op) : ExprNode() {
    left = _left;
    op = _op;
    assert(left != nullptr);
}

Expression::Expression(ExprNode *_expr_node) : ASTNode() {
    has_elaborated = false;
    expr_node = _expr_node;
}

// -- CONVERT --
ExprNode* ExprNode::convert(parser::expr_primary *e) {
    ExprPrimary* result = new ExprPrimary(e);
    if(e->is_a0) {
        result->val = Literal::convert(e->t0->t0);
    }
    else if(e->is_a1) {
        result->val = ConstructorCall::convert(e->t1->t0);
    }
    else if(e->is_a2) {
        result->val = FunctionCall::convert(e->t2->t0);
    }
    else if(e->is_a3) {
        result->val = Identifier::convert(e->t3->t0);
    }
    else if(e->is_a4) {
        result->val = Expression::convert(e->t4->t2);
    }
    else assert(false);
    return result;
}

ExprNode* ExprNode::convert(parser::expr_postfix *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprPostfix* _result = new ExprPostfix(e->t1[i]->t1);
        if(e->t1[i]->t1->is_c0) {       //indexing
            _result->left = result;
            _result->op = Expression::convert(e->t1[i]->t1->t0->t2);
        }
        else if(e->t1[i]->t1->is_c1) {  //call member function
            std::string op = ".";
            FunctionCall *fc = FunctionCall::convert(e->t1[i]->t1->t1->t2);
            _result->left = result;
            _result->op = std::make_pair(op, fc);
        }
        else if(e->t1[i]->t1->is_c2) {  //dereference, call member function
            std::string op = "->";
            FunctionCall *fc = FunctionCall::convert(e->t1[i]->t1->t2->t2);
            _result->left = result;
            _result->op = std::make_pair(op, fc);
        }
        else if(e->t1[i]->t1->is_c3) {  //access member variable
            std::string op = ".";
            Identifier *id = Identifier::convert(e->t1[i]->t1->t3->t2);
            _result->left = result;
            _result->op = std::make_pair(op, id);
        }
        else if(e->t1[i]->t1->is_c4) {  //dereference, access member variable
            std::string op = "->";
            Identifier *id = Identifier::convert(e->t1[i]->t1->t4->t2);
            _result->left = result;
            _result->op = std::make_pair(op, id);
        }
        else if(e->t1[i]->t1->is_c5) {  //postfix increment
            std::string op = "++";
            _result->left = result;
            _result->op = op;
        }
        else if(e->t1[i]->t1->is_c6) {  //postfix decrement
            std::string op = "--";
            _result->left = result;
            _result->op = op;
        }
        else if(e->t1[i]->t1->is_c7) {  //function pointer call
            std::vector<Expression*> argument_list = convert_argument_list(e->t1[i]->t1->t7->t1);
            _result->left = result;
            _result->op = argument_list;
        }
        else if(e->t1[i]->t1->is_c8) {  //destructor call
            std::string op = ".~()";
            _result->left = result;
            _result->op = op;
        }
        else if(e->t1[i]->t1->is_c9) {  //dereference, destructor call
            std::string op = "->~()";
            _result->left = result;
            _result->op = op;
        }       
        else assert(false);
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_unary *e) {
    if(e->is_a0) {      //unary operator + unary expression
        ExprPrefix* result = new ExprPrefix(e);
        if(e->t0->t0->is_c8) {  //casting
            result->op = Type::convert(e->t0->t0->t8->t1);
        }
        else {                  //arithmetic operator
            result->op = e->t0->t0->to_string();
        }
        result->right = ExprNode::convert(e->t0->t2);
        return result;
    }
    else if(e->is_a1) { //postfix expression
        return ExprNode::convert(e->t1->t0);
    }
    else assert(false);
}

ExprNode* ExprNode::convert(parser::expr_multiplicative *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_additive *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_shift *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_relational *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_equality *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_bit_and *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_bit_xor *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_bit_or *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_logical_and *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}   

ExprNode* ExprNode::convert(parser::expr_logical_or *e) {
    ExprNode* result = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        _result->left = result;
        _result->op = op;
        _result->right = right;
        result = _result;
    }
    return result;
}

ExprNode* ExprNode::convert(parser::expr_assignment *e) {
    //assignment is right to left associative
    std::vector<ExprNode*> nodes;
    nodes.push_back(ExprNode::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++) nodes.push_back(ExprNode::convert(e->t1[i]->t3));

    ExprNode* result = nodes[nodes.size() - 1];
    for(int i = (int) nodes.size() - 2; i >= 0; i--){
        ExprBinary* _result = new ExprBinary(e->t1[i]);
        std::string op = e->t1[i]->t1->to_string();
        _result->left = nodes[i];
        _result->op = op;
        _result->right = result;
        result = _result;
    }
    return result;
}

Expression* Expression::convert(parser::expression *e) {
    Expression* result = new Expression(e);
    result->has_elaborated = false;
    result->expr_node = ExprNode::convert(e->t0);
    return result;
}

// -- RESOLVE_TYPE --
Type* ExprNode::resolve_type(CompilationContext *ctx) {
    if(resolve_type_cache.has_value()) return resolve_type_cache.value();
    resolve_type_cache = this->_resolve_type(ctx);
    return resolve_type_cache.value();
}

Type* ExprPrimary::_resolve_type(CompilationContext *ctx) {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *f = std::get<FunctionCall*>(val);
        Type *res = f->resolve_type(ctx);
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
        Type *res = c->resolve_type(ctx);
        if(res == nullptr){
            std::cout << "Constructor call does not resolve to type : " << c->to_string() << "\n";
            return nullptr;
        }
        return res;
    }
    else if(std::holds_alternative<OperatorCall*>(val)) {
        OperatorCall *o = std::get<OperatorCall*>(val);
        Type *res = o->resolve_type(ctx);
        if(res == nullptr){
            std::cout << "Overload does not resolve to type : " << o->to_string() << "\n";
            return nullptr;
        }

        //if this is a reference, automatically dereference it
        if(dynamic_cast<ReferenceType*>(res)) {
            res = dynamic_cast<ReferenceType*>(res)->type;
        }
        return res;
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        Type *res = get_variable_type(id);
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
        return l->resolve_type(ctx);
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->resolve_type(ctx);
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

Type* ExprBinary::_resolve_type(CompilationContext *ctx) {
    Type *lt = left->resolve_type(ctx), *rt = right->resolve_type(ctx);
    if(lt == nullptr || rt == nullptr) return nullptr;    
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //try to find overload / builtin
        Type *otype = ctx->find_resulting_type(left, str_op, right);
        if(otype != nullptr) {
            //auto dereference reference type
            if(auto x = dynamic_cast<ReferenceType*>(otype)) {
                otype = x->type;
            }
            return otype;
        }

        //default behaviour
        if(str_op == "=") {
            if(!left->is_lvalue(ctx)) {
                if(debug) std::cout << "Cannot assign to r-value\n";
                return nullptr;
            }
            if(!lt->equals(rt)) {
                if(debug) std::cout << "Cannot assign " << lt->to_string() << " to " << rt->to_string() << "\n";
                return nullptr;
            }
            return lt;
        }

        std::cout << "Binary operator " << lt->to_string() << " " << str_op << " " << rt->to_string() << " does not exist\n";
        return nullptr;
    }   
    else assert(false);
}

Type* ExprPrefix::_resolve_type(CompilationContext *ctx) {
    Type *rt = right->resolve_type(ctx);
    if(rt == nullptr) return nullptr;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //try to find overload / builtin
        Type *otype = ctx->find_resulting_type(std::nullopt, str_op, right);
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
        else if(str_op == "@") {
            //right must be an l-value in order to reference
            if(!right->is_lvalue(ctx)) {
                std::cout << "Cannot create reference of r-value : " << right->to_string() << "\n";
                return nullptr;
            }

            //should be able to always reference
            return new PointerType(rt);
        }

        std::cout << "Prefix operator " << str_op << " " << rt->to_string() << " does not exist\n";
        return nullptr;
    }
    else if(std::holds_alternative<Type*>(op)) {
        Type *cast_t = std::get<Type*>(op);

        //try to find cast operator implementation
        Operator *o = ctx->get_called_typecast(rt, cast_t);
        if(o == nullptr) {
            std::cout << "Cannot cast from " << rt->to_string() << " to " << cast_t->to_string() << "\n";
            return nullptr;
        }

        return cast_t;
    }
    else assert(false);
}

Type* ExprPostfix::_resolve_type(CompilationContext *ctx) {
    Type *lt = left->resolve_type(ctx);
    if(lt == nullptr) return nullptr;
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        Type *et = expr->resolve_type(ctx);
        if(et == nullptr) return nullptr;

        //try to find overload
        Type *otype = ctx->find_resulting_type(left, "[]", expr->expr_node);
        if(otype != nullptr) {
            //auto dereference reference type
            if(auto x = dynamic_cast<ReferenceType*>(otype)) {
                otype = x->type;
            }
            return otype;
        }

        //default behaviour
        if(ctx->get_called_typecast(et, primitives::u64) == nullptr) {
            std::cout << "Builtin indexing expression must be convertible to u64\n";
            return nullptr;
        }
        if(auto nt = dynamic_cast<PointerType*>(lt)) {
            return nt->type;
        }
        else if(auto nt = dynamic_cast<ArrayType*>(lt)) {
            return nt->type;
        }
        else {
            std::cout << "Can't index into non-pointer and non-array type " << lt->to_string() << "\n";
            return nullptr;
        }
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

        //make sure this isn't primitive
        if(is_type_primitive(lt)) {
            std::cout << "Trying to access member variable of primitive type : " << lt->to_string() << "\n";
            return nullptr;
        }

        //member function call
        fc = new FunctionCall(lt, fc->id, fc->argument_list);
        Type *nt = fc->resolve_type(ctx);
        if(fc->resolve_type(ctx) == nullptr) {
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

        //make sure this isn't primitive
        if(is_type_primitive(lt)) {
            std::cout << "Trying to access member variable of primitive type : " << lt->to_string() << "\n";
            return nullptr;
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

        //manual destructor calls
        if(str_op == ".~()" || str_op == "->~()") {

            //dereference
            if(str_op == "->~()") {
                if(dynamic_cast<PointerType*>(lt) == nullptr) {
                    std::cout << "Trying to dereference non-pointer type " << lt->to_string() << "\n";
                    return nullptr;
                }
                lt = dynamic_cast<PointerType*>(lt)->type;
            }

            return primitives::_void->make_copy();
        }

        //try to find overload / builtin
        Type *otype = ctx->find_resulting_type(left, str_op, std::nullopt);
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
    else if(std::holds_alternative<std::vector<Expression*>>(op)) {
        std::vector<Expression*> argument_list = std::get<std::vector<Expression*>>(op);

        // - is type of left a function pointer?
        FunctionPointerType *fpt = dynamic_cast<FunctionPointerType*>(lt);
        if(fpt == nullptr) {
            std::cout << "Trying to do function pointer call on non function pointer : " << lt->to_string() << "\n";
            return nullptr;
        }

        // - do the amount of arguments match?
        if(argument_list.size() != fpt->param_types.size()) {
            std::cout << "Function pointer call argument count doesn't match with param count\n";
            return nullptr;
        }

        // - can the arguments actually call the function?
        for(int i = 0; i < argument_list.size(); i++){
            if(!ctx->is_declarable(fpt->param_types[i], argument_list[i])){
                std::cout << "Cannot declare function pointer argument : " << fpt->param_types[i]->to_string() << " with expression " << argument_list[i]->to_string() << "\n";
                return nullptr;
            }
        }
        
        return fpt->return_type->make_copy();
    }
    else assert(false);
}

Type* Expression::resolve_type(CompilationContext *ctx) {
    return expr_node->resolve_type(ctx);
}

// -- IS_LVALUE --
bool ExprNode::is_lvalue(CompilationContext *ctx) {
    if(is_lvalue_cache.has_value()) return is_lvalue_cache.value();
    is_lvalue_cache = this->_is_lvalue(ctx);
    return is_lvalue_cache.value();
}

bool ExprPrimary::_is_lvalue(CompilationContext *ctx) {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        Type *t = fc->resolve_type(ctx);
        if(t == nullptr) return false;
        //if return type is a reference, it gets auto-dereferenced. 
        if(dynamic_cast<ReferenceType*>(t) != nullptr) return true;
        return false;
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        //if we construct-in-place, this returns a l-value, otherwise r-value
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        if(c->cip_expr.has_value()) return true;
        else return false;
    }
    else if(std::holds_alternative<OperatorCall*>(val)) {
        OperatorCall *o = std::get<OperatorCall*>(val);
        Type *t = o->resolve_type(ctx);
        if(t == nullptr) return false;
        //if return type is reference, auto-dereference
        if(dynamic_cast<ReferenceType*>(t) != nullptr) return true;
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
        return e->is_lvalue(ctx);
    }
    else if(std::holds_alternative<Type*>(val)) {
        //if type is reference, this is an l-value
        Type *t = std::get<Type*>(val);
        return dynamic_cast<ReferenceType*>(t);
    }
    else assert(false);
}

bool ExprBinary::_is_lvalue(CompilationContext *ctx) {
    Type *lt = left->resolve_type(ctx), *rt = right->resolve_type(ctx);
    if(lt == nullptr || rt == nullptr) return false;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //find overload / builtin
        Type *otype = ctx->find_resulting_type(left, str_op, right);
        if(otype != nullptr) {
            return dynamic_cast<ReferenceType*>(otype) != nullptr;
        }
        
        //default behaviour
        return true;
    }   
    else assert(false);
}

bool ExprPrefix::_is_lvalue(CompilationContext *ctx) {
    Type *rt = right->resolve_type(ctx);
    if(!right->is_lvalue(ctx)) return false;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //find overload / builtin
        Type *otype = ctx->find_resulting_type(std::nullopt, str_op, right);
        if(otype != nullptr) {
            return dynamic_cast<ReferenceType*>(otype) != nullptr;
        }

        // default behaviour
        if(str_op == "*") {
            return true;
        }

        return false;
    }
    else if(std::holds_alternative<Type*>(op)) {
        Type *cast_t = std::get<Type*>(op);
        //results from casts are always r-values
        return false;
    }
    else assert(false);
}

bool ExprPostfix::_is_lvalue(CompilationContext *ctx) {
    Type *lt = left->resolve_type(ctx);
    if(lt == nullptr) return false;
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        Type *et = expr->resolve_type(ctx);
        if(et == nullptr) return false;
        
        //find overload
        Type *otype = ctx->find_resulting_type(left, "[]", expr->expr_node);
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
        Type *nt = fc->resolve_type(ctx);
        if(fc->resolve_type(ctx) == nullptr) {
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

        //manual destructor calls
        if(str_op == ".~()" || str_op == "->~()") {
            return false;
        }

        //find overload
        Type *otype = ctx->find_resulting_type(left, str_op, std::nullopt);
        if(otype != nullptr) {
            return dynamic_cast<ReferenceType*>(otype) != nullptr;
        }

        //default behaviour
        return false;
    }
    else if(std::holds_alternative<std::vector<Expression*>>(op)) { //function pointer call
        std::vector<Expression*> argument_list = std::get<std::vector<Expression*>>(op);
        FunctionPointerType *fpt = dynamic_cast<FunctionPointerType*>(fpt);
        if(fpt == nullptr) return false;
        return dynamic_cast<ReferenceType*>(fpt->return_type);
    }
    else assert(false);
}

bool Expression::is_lvalue(CompilationContext *ctx) {
    return expr_node->is_lvalue(ctx);
}

// -- ELABORATE --
void ExprPrimary::elaborate(CompilationContext *ctx, ExprNode*& self) {
    if(std::holds_alternative<FunctionCall*>(val)) {
        //do nothing
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        //do nothing
    }
    else if(std::holds_alternative<OperatorCall*>(val)) {
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
        expr->elaborate(ctx);
    }
    else if(std::holds_alternative<Type*>(val)) {
        //do nothing
    }
    else assert(false);
}

void ExprBinary::elaborate(CompilationContext *ctx, ExprNode*& self) {
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //convert overloads into overload calls
        Operator *o = ctx->get_called_operator(left, str_op, right);
        if(dynamic_cast<OperatorOverload*>(o) != nullptr) {
            OperatorCall *oc = new OperatorCall(left, str_op, right);
            self = new ExprPrimary(oc);
            return;
        }
    }
    else assert(false);

    //if we haven't replaced ourself, elaborate children
    left->elaborate(ctx, left);
    right->elaborate(ctx, right);
}

void ExprPrefix::elaborate(CompilationContext *ctx, ExprNode*& self) {
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //convert overloads into overload calls
        Operator *o = ctx->get_called_operator(std::nullopt, str_op, right);
        if(dynamic_cast<OperatorOverload*>(o) != nullptr) {
            OperatorCall *oc = new OperatorCall(std::nullopt, str_op, right);
            self = new ExprPrimary(oc);
            return;
        }
    }
    else if(std::holds_alternative<Type*>(op)) {
        Type *cast_t = std::get<Type*>(op);
        Type *rt = right->resolve_type(ctx);

        //for now, assume that all typecasts are builtin
        Operator *oe = ctx->get_called_typecast(rt, cast_t);
        assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
    }
    else assert(false);

    //if we haven't replaced ourself, elaborate children
    right->elaborate(ctx, right);
}

void ExprPostfix::elaborate(CompilationContext *ctx, ExprNode*& self) { 
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        assert(expr->resolve_type(ctx) != nullptr);

        //convert overloads into overload calls
        Operator *o = ctx->get_called_operator(left, "[]", expr->expr_node);
        if(dynamic_cast<OperatorOverload*>(o) != nullptr) {
            OperatorCall *oc = new OperatorCall(left, "[]", expr->expr_node);
            self = new ExprPrimary(oc);
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

        //convert overloads into overload calls
        Operator *o = ctx->get_called_operator(left, str_op, std::nullopt);
        if(dynamic_cast<OperatorOverload*>(o) != nullptr) {
            OperatorCall *oc = new OperatorCall(left, str_op, std::nullopt);
            self = new ExprPrimary(oc);
            return;
        }
    }
    else if(std::holds_alternative<std::vector<Expression*>>(op)) {
        //do nothing
    }
    else assert(false);

    //if we haven't replaced ourself, elaborate children
    left->elaborate(ctx, left);
}

void Expression::elaborate(CompilationContext *ctx) {
    if(has_elaborated) {
        if(debug) std::cout << "ALREADY ELABORATED : " << to_string() << std::endl;
        assert(false);
    }
    if(debug) std::cout << "ELABORATING : " << to_string() << std::endl;
    has_elaborated = true;
    expr_node->elaborate(ctx, expr_node);
    if(debug) std::cout << "DONE ELABORATING : " << to_string() << std::endl;
}

// -- EMIT_ASM -- 
void ExprPrimary::emit_asm(CompilationContext *ctx) {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *f = std::get<FunctionCall*>(val);
        f->emit_asm(ctx);

        Type *ft = f->resolve_type(ctx);
        assert(ft != nullptr);
        
        //if this is a reference, dereference it
        if(dynamic_cast<ReferenceType*>(ft)) {
            emit_dereference(ft);
        }
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        c->emit_asm(ctx, false);
    }
    else if(std::holds_alternative<OperatorCall*>(val)) {
        OperatorCall *o = std::get<OperatorCall*>(val);
        o->emit_asm(ctx);

        Type *ot = o->resolve_type(ctx);
        assert(ot != nullptr);

        //if this is a reference, dereference it
        if(dynamic_cast<ReferenceType*>(ot)) {
            emit_dereference(ot);
        }
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        Variable *v = get_variable(id);
        assert(v != nullptr);

        if(asm_debug) fout << indent() << "# load variable " << id->name << "\n";
        fout << indent() << "mov " << v->addr << ", %rax\n";  //value

        //address. 
        //For structs, they are supposed to be their main memory, so their address is the same as their value
        //since struct T* is the same as T, T& is also the same as T (it's just we auto dereference T&)
        if(is_type_primitive(v->type)) {
            fout << indent() << "lea " << v->addr << ", %rcx\n";  //address
        }
        else {
            fout << indent() << "mov " << v->addr << ", %rcx\n";  //address
        }

        //if this is a reference, dereference it
        Type *vt = v->type;
        if(dynamic_cast<ReferenceType*>(vt)) {
            emit_dereference(vt);
        }
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        l->emit_asm(ctx);
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        e->expr_node->emit_asm(ctx);   //bypass elaboration
    }
    else if(std::holds_alternative<Type*>(val)) {
        assert(false);  //Type* is just for checking if an expression resolves to something. 
    }
    else assert(false);
}

void ExprBinary::emit_asm(CompilationContext *ctx) {
    Type *lt = left->resolve_type(ctx), *rt = right->resolve_type(ctx);
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        if(str_op == "||") {    //logical or
            std::string label = create_new_label();
            left->emit_asm(ctx);

            //see if we should short circuit
            fout << indent() << "cmp $0, %rax\n";
            fout << indent() << "jne " << label << "\n";

            //if we don't short circuit, evaluate right branch and merge
            {
                emit_push("%rax", "ExprBinary::emit_asm() : || save left");
                right->emit_asm(ctx);
                fout << indent() << "mov %rax, %rbx\n";
                emit_pop("%rax", "ExprBinary::emit_asm() : || save left");

                Operator *oe = ctx->get_called_operator(left, str_op, right);
                assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
                dynamic_cast<BuiltinOperator*>(oe)->emit_asm();
            }

            fout << label << ":\n";
        }
        else if(str_op == "&&") {   //logical and
            std::string label = create_new_label();
            left->emit_asm(ctx);

            //see if we should short circuit
            fout << indent() << "cmp $0, %rax\n";
            fout << indent() << "je " << label << "\n";

            //if we don't short circuit, evaluate right branch and merge
            {
                emit_push("%rax", "ExprBinary::emit_asm() : && save left");
                right->emit_asm(ctx);
                fout << indent() << "mov %rax, %rbx\n";
                emit_pop("%rax", "ExprBinary::emit_asm() : && save left");

                Operator *oe = ctx->get_called_operator(left, str_op, right);
                assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
                dynamic_cast<BuiltinOperator*>(oe)->emit_asm();
            }

            fout << label << ":\n";
        }
        else if(str_op == "=") {
            //this should be handled during elaboration
            assert(lt->equals(rt));

            if(is_type_primitive(lt)) {
                //eval right r-value
                right->emit_asm(ctx);

                //save value
                emit_push("%rax", "ExprBinary::emit_asm() : = save right");

                //evaluate left l-value
                left->emit_asm(ctx);

                //move right value into left mem location
                int sz = lt->calc_size();
                fout << indent() << "mov %rcx, %rbx\n";
                emit_pop("%rax", "ExprBinary::emit_asm() : = save right");
                emit_mem_store(sz);
            }
            else {
                //generate right struct
                right->emit_asm(ctx);

                //create temp struct reference variable 
                push_declaration_stack();
                if(asm_debug) fout << indent() << "# expression struct assignment tmp variable\n";
                Identifier *id = new Identifier(create_new_tmp_variable_name());
                emit_push("%rax", id->name);    //push pointer to right struct on stack
                Variable *v = add_stack_variable(new ReferenceType(rt), id);

                //generate left struct. %rax should now hold struct mem location
                left->emit_asm(ctx);
                
                //destruct left struct without dealloccing
                emit_push("%rax", "ExprBinary::emit_asm() : save left addr before destruct");
                emit_destructor_call(ctx, lt, false);
                emit_pop("%rax", "ExprBinary::emit_asm() : save left addr before destruct");

                //use copy constructor to overwrite left struct mem location
                ConstructorCall *cc = new ConstructorCall(std::nullopt, lt, {new Expression(new ExprPrimary(id))});
                assert(cc->resolve_called_constructor(ctx) != nullptr);
                cc->emit_asm(ctx, true);    //%rax should now hold struct mem location

                //if the right struct is not an lvalue, deallocate it
                //This is not handled by pop_declaration_stack() as the temp variable is of reference type
                if(!right->is_lvalue(ctx)){
                    //save left mem addr
                    emit_push("%rax", "ExprBinary::emit_asm() : save struct addr during right dealloc");
                    
                    //put addr to struct in %rax
                    fout << indent() << "movq " << v->addr << ", %rax\n";

                    //call destructor
                    emit_destructor_call(ctx, rt, true);
                    
                    //retrieve left mem addr
                    emit_pop("%rax", "ExprBinary::emit_asm() : save struct addr during right dealloc");
                }

                //clean up temp variables
                pop_declaration_stack(ctx);

                //%rax should hold address, copy it over to %rcx
                fout << indent() << "mov %rax, %rcx\n";
            }

            //the value of left after the assignment should be in %rax
            //the address of left should be in %rcx
        }
        else {
            right->emit_asm(ctx);
            emit_push("%rax", "ExprBinary::emit_asm() : save right");
            left->emit_asm(ctx);
            emit_pop("%rbx", "ExprBinary::emit_asm() : save right");
            
            Operator *o = ctx->get_called_operator(left, str_op, right);
            assert(dynamic_cast<BuiltinOperator*>(o) != nullptr);
            dynamic_cast<BuiltinOperator*>(o)->emit_asm();

            //if this is a reference, dereference it
            Type *res = o->type;
            if(dynamic_cast<ReferenceType*>(res) != nullptr) {
                emit_dereference(res);
            }
        }
    }
    else assert(false);
}

void ExprPrefix::emit_asm(CompilationContext *ctx) {
    Type *rt = right->resolve_type(ctx);
    right->emit_asm(ctx);

    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        if(str_op == "*"){ 
            assert(dynamic_cast<PointerType*>(rt) != nullptr);
            emit_dereference(rt);
            rt = dynamic_cast<PointerType*>(rt)->type;
            assert(rt != nullptr);
        }
        else if(str_op == "@") {
            assert(right->is_lvalue(ctx));
            if(is_type_primitive(rt)) {
                //only replace the value if it's a primitive type. 
                //If it's a struct, then it's already equivalent to a pointer. 
                fout << indent() << "mov %rcx, %rax\n";
            }
        }
        else {
            Operator *o = ctx->get_called_operator(std::nullopt, str_op, right);
            assert(dynamic_cast<BuiltinOperator*>(o) != nullptr);
            dynamic_cast<BuiltinOperator*>(o)->emit_asm();

            //if this is a reference, dereference it
            Type *res = o->type;
            if(dynamic_cast<ReferenceType*>(res) != nullptr) {
                emit_dereference(res);
            }
        }
    }
    else if(std::holds_alternative<Type*>(op)) {
        Type *cast_t = std::get<Type*>(op);

        //for now, assume all typecasts are builtin
        Operator *oi = ctx->get_called_typecast(rt, cast_t);
        assert(dynamic_cast<BuiltinOperator*>(oi) != nullptr);
        dynamic_cast<BuiltinOperator*>(oi)->emit_asm();
    }
    else assert(false);
}

void ExprPostfix::emit_asm(CompilationContext *ctx) {
    Type *lt = left->resolve_type(ctx);
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        //evaluate left expr
        left->emit_asm(ctx);

        //t must be PointerType or ArrayType
        if(dynamic_cast<PointerType*>(lt)) {
            lt = dynamic_cast<PointerType*>(lt)->type;
        }
        else if(dynamic_cast<ArrayType*>(lt)) {
            lt = dynamic_cast<ArrayType*>(lt)->type;
        }
        else assert(false);

        //save %rax, %rcx to stack
        emit_push("%rax", "ExprPostfix::emit_asm() : [] %rax 1");
        emit_push("%rcx", "ExprPostfix::emit_asm() : [] %rcx 1");

        //evaluate expression
        Expression *expr = std::get<Expression*>(op);
        Type *et = expr->resolve_type(ctx);
        assert(expr != nullptr);
        expr->emit_asm(ctx);

        //convert et to u64
        BuiltinOperator *cast_op = dynamic_cast<BuiltinOperator*>(ctx->get_called_typecast(et, primitives::u64));
        assert(cast_op != nullptr);
        cast_op->emit_asm();

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

        if(is_type_primitive(lt)) {
            //retrieve array element data
            emit_retrieve_array(sz);
        }
        else {
            //just return lvalue pointer to data
            fout << indent() << "mov %rcx, %rax\n";
        }
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        FunctionCall *fc = p.second;

        //evaluate left expr
        left->emit_asm(ctx);

        //dereference
        if(p.first == "->") {
            assert(dynamic_cast<PointerType*>(lt) != nullptr);
            emit_dereference(lt);
            lt = dynamic_cast<PointerType*>(lt)->type;
            assert(lt != nullptr);
        }

        //member function call
        fc = new FunctionCall(lt, fc->id, fc->argument_list);
        Function *f = fc->resolve_called_function(ctx);
        assert(f != nullptr);

        //this is no longer l-value, so don't have to maintain %rcx
        //reference to type is in %rax, so emit function call
        fc->emit_asm(ctx);

        //if the return value is a reference, auto-dereference it
        Type *ft = f->type;
        if(dynamic_cast<ReferenceType*>(ft)) {
            emit_dereference(ft);
        }
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        Identifier *id = p.second;

        //evaluate left expr
        left->emit_asm(ctx);

        //dereference
        if(p.first == "->") {
            if(asm_debug) fout << indent() << "# dereference via ->\n";
            assert(dynamic_cast<PointerType*>(lt) != nullptr);
            emit_dereference(lt);
            lt = dynamic_cast<PointerType*>(lt)->type;
            assert(lt != nullptr);
            if(asm_debug) fout << indent() << "# done dereference via ->\n";
        }

        //member variable access
        StructLayout *sl = get_struct_layout(lt);
        assert(sl != nullptr);
        assert(sl->get_type(id) != nullptr);

        Type *vt = sl->get_type(id);
        int offset = sl->get_offset(id);
        if(asm_debug) fout << indent() << "# accessing member variable " << id->name << ", offset : " << offset << "\n";

        fout << indent() << "lea " << offset << "(%rax), %rcx\n";
        fout << indent() << "lea " << offset << "(%rax), %rax\n";

        //only load the actual value into the register if the type is primitive
        if(is_type_primitive(vt)) { 
            emit_mem_retrieve(vt->calc_size());  //now, %rax = member variable, %rcx = address of member variable
        }   

        if(asm_debug) fout << indent() << "# done accessing member variable " << id->name << "\n";

    }
    else if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //evaluate left expr
        left->emit_asm(ctx);

        //manual destructor calls
        if(str_op == ".~()" || str_op == "->~()") {
            //dereference
            if(str_op == "->~()") {
                if(asm_debug) fout << indent() << "# dereference via ->\n";
                assert(dynamic_cast<PointerType*>(lt) != nullptr);
                emit_dereference(lt);
                lt = dynamic_cast<PointerType*>(lt)->type;
                assert(lt != nullptr);
                if(asm_debug) fout << indent() << "# done dereference via ->\n";
            }   
            
            //if this is not primitive, destruct without deallocing
            if(!is_type_primitive(lt)) {
                emit_destructor_call(ctx, lt, false);
            }
        }
        else {
            //evaluate operator
            Operator *o = ctx->get_called_operator(left, str_op, std::nullopt);
            assert(dynamic_cast<BuiltinOperator*>(o) != nullptr);
            dynamic_cast<BuiltinOperator*>(o)->emit_asm();

            //if this is a reference, dereference it
            Type *res = o->type;
            if(dynamic_cast<ReferenceType*>(res) != nullptr) {
                emit_dereference(res);
            }
        }
    }
    else if(std::holds_alternative<std::vector<Expression*>>(op)) {
        std::vector<Expression*> argument_list = std::get<std::vector<Expression*>>(op);

        if(asm_debug) fout << indent() << "# calling function pointer : " << lt->to_string() << "\n";

        FunctionPointerType *vt = dynamic_cast<FunctionPointerType*>(lt);
        assert(vt != nullptr);

        //create temp variables for arguments
        push_declaration_stack();
        assert(argument_list.size() == vt->param_types.size());
        for(int i = 0; i < argument_list.size(); i++){
            if(asm_debug) fout << indent() << "# function pointer call member variable : " << vt->param_types[i]->to_string() << "\n";
            Identifier *id = new Identifier(create_new_tmp_variable_name());
            Variable *v = emit_initialize_stack_variable(ctx, vt->param_types[i], id, argument_list[i]);
            assert(v != nullptr);
        }

        //evaluate left expr
        left->emit_asm(ctx);

        //call function pointer
        fout << indent() << "call *%rax\n"; //*%rax denotes an indirect call, otherwise %rax would be interpreted as a label

        //cleanup argument temp variables, function will deallocate them
        pop_declaration_stack(ctx, false);

        if(asm_debug) fout << indent() << "# done calling function pointer : " << lt->to_string() << "\n";
    }
    else assert(false);
}

void Expression::emit_asm(CompilationContext *ctx, bool should_dealloc) {
    elaborate(ctx);
    expr_node->emit_asm(ctx);

    //dealloc unused r-value struct
    if(should_dealloc) {
        Type *et = resolve_type(ctx);
        assert(et != nullptr);
        if(!is_type_primitive(et) && !is_lvalue(ctx)) {
            //call destructor
            emit_destructor_call(ctx, et, true);
        }
    }
}

// -- TO STRING --
std::string ExprPrimary::to_string() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        return fc->to_string();
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        return c->to_string();
    }
    else if(std::holds_alternative<OperatorCall*>(val)) {
        OperatorCall *o = std::get<OperatorCall*>(val);
        return o->to_string();
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        return id->name;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return l->to_string();
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
    else if(std::holds_alternative<Type*>(op)) {
        Type *cast_t = std::get<Type*>(op);
        return "$" + cast_t->to_string() + " " + right->to_string();
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
    else if(std::holds_alternative<std::vector<Expression*>>(op)) {
        std::vector<Expression*> argument_list = std::get<std::vector<Expression*>>(op);
        std::string ret = left->to_string() + "#(";
        for(int i = 0; i < argument_list.size(); i++){
            ret += argument_list[i]->to_string();
            if(i + 1 != argument_list.size()) ret += ", ";
        }
        ret += ")";
        return ret;
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
    else if(std::holds_alternative<OperatorCall*>(val)) {
        OperatorCall *o = std::get<OperatorCall*>(val);
        return o->hash();
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
    else if(std::holds_alternative<Type*>(op)) {
        Type *cast_t = std::get<Type*>(op);
        hash_combine(hash, cast_t->hash());
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
    else if(std::holds_alternative<std::vector<Expression*>>(op)) {
        std::vector<Expression*> argument_list = std::get<std::vector<Expression*>>(op);
        for(int i = 0; i < argument_list.size(); i++){
            hash_combine(hash, argument_list[i]->hash());
        }
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
    else if(std::holds_alternative<OperatorCall*>(val)) {
        OperatorCall *o = std::get<OperatorCall*>(val);
        OperatorCall *oo = std::get<OperatorCall*>(other->val);
        return o->equals(oo);
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
    else if(std::holds_alternative<Type*>(op)) {
        Type *cast_t = std::get<Type*>(op);
        Type *ocast_t = std::get<Type*>(other->op);
        return cast_t->equals(ocast_t);
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
    else if(std::holds_alternative<std::vector<Expression*>>(op)) {
        std::vector<Expression*> argument_list = std::get<std::vector<Expression*>>(op);
        std::vector<Expression*> oargument_list = std::get<std::vector<Expression*>>(other->op);
        if(argument_list.size() != oargument_list.size()) return false;
        for(int i = 0; i < argument_list.size(); i++){
            if(!argument_list[i]->equals(oargument_list[i])) return false;
        }
        return true;
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
    else if(std::holds_alternative<OperatorCall*>(val)) {
        OperatorCall *o = std::get<OperatorCall*>(val);
        
        //turn both arguments to type
        if(o->left.has_value()) o->left.value()->id_to_type();
        if(o->right.has_value()) o->right.value()->id_to_type();
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);   

        //retrieve identifier type and replace
        Type *t = get_variable_type(id);
        if(t == nullptr) {
            std::cout << "Could not find type of variable : " << id->name << "\n";
            assert(false);
        }

        //we use reference types as standin for l-value, so if this isn't a reference type, turn it into one
        if(dynamic_cast<ReferenceType*>(t) == nullptr) {
            t = new ReferenceType(t);
        }

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
    else if(std::holds_alternative<std::vector<Expression*>>(op)) {
        std::vector<Expression*> argument_list = std::get<std::vector<Expression*>>(op);
        for(int i = 0; i < argument_list.size(); i++){
            argument_list[i]->id_to_type();
        }
    }
    else assert(false);
}

void Expression::id_to_type() {
    expr_node->id_to_type();
}

// -- COPY --
ExprNode* ExprPrimary::make_copy() {
    return new ExprPrimary(*this);
}

ExprNode* ExprBinary::make_copy() {
    return new ExprBinary(*this);
}

ExprNode* ExprPrefix::make_copy() {
    return new ExprPrefix(*this);
}

ExprNode* ExprPostfix::make_copy() {
    return new ExprPostfix(*this);
}

Expression* Expression::make_copy() {
    return new Expression(*this);
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
    else if(std::holds_alternative<OperatorCall*>(val)) {
        OperatorCall *o = std::get<OperatorCall*>(val);
        return o->replace_templated_types(mapping);
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        //do nothing 
        return true;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return l->replace_templated_types(mapping);
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->replace_templated_types(mapping);
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *t = std::get<Type*>(val);
        if(auto x = mapping->find_mapped_type(t)) t = x;
        else if(!t->replace_templated_types(mapping)) return false;
        val = t;
        return true;
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
    else if(std::holds_alternative<Type*>(op)) {
        Type *cast_t = std::get<Type*>(op);
        if(auto x = mapping->find_mapped_type(cast_t)) cast_t = x;
        else if(!cast_t->replace_templated_types(mapping)) return false;
        op = cast_t;
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
    else if(std::holds_alternative<std::vector<Expression*>>(op)) {
        std::vector<Expression*> argument_list = std::get<std::vector<Expression*>>(op);
        for(int i = 0; i < argument_list.size(); i++){
            if(!argument_list[i]->replace_templated_types(mapping)) return false;
        }
    }
    else assert(false);
    return true;
}

bool Expression::replace_templated_types(TemplateMapping *mapping) {
    return expr_node->replace_templated_types(mapping);
}

// -- LOOK FOR TEMPLATES --
bool ExprPrimary::look_for_templates(CompilationContext *ctx) {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        if(!fc->look_for_templates(ctx)) return false;
    }
    else if(std::holds_alternative<ConstructorCall*>(val)) {
        ConstructorCall *c = std::get<ConstructorCall*>(val);
        if(!c->look_for_templates(ctx)) return false;
    }
    else if(std::holds_alternative<OperatorCall*>(val)) {
        OperatorCall *o = std::get<OperatorCall*>(val);
        if(!o->look_for_templates(ctx)) return false;
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        // do nothing
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        // do nothing
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        if(!e->look_for_templates(ctx)) return false;
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *t = std::get<Type*>(val);
        if(!t->look_for_templates(ctx)) return false;
    }
    else assert(false);
    return true;
}

bool ExprBinary::look_for_templates(CompilationContext *ctx) {
    if(!left->look_for_templates(ctx)) return false;
    if(!right->look_for_templates(ctx)) return false;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        // do nothing
    }
    else assert(false);
    return true;
}

bool ExprPrefix::look_for_templates(CompilationContext *ctx) {
    if(!right->look_for_templates(ctx)) return false;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        // do nothing
    }
    else if(std::holds_alternative<Type*>(op)) {
        Type *cast_t = std::get<Type*>(op);
        if(!cast_t->look_for_templates(ctx)) return false;
    }
    else assert(false);
    return true;
}

bool ExprPostfix::look_for_templates(CompilationContext *ctx) {
    if(!left->look_for_templates(ctx)) return false;
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        if(!expr->look_for_templates(ctx)) return false;
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        if(!p.second->look_for_templates(ctx)) return false;
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        // do nothing
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        std::string str_op = std::get<std::string>(op);
        // do nothing
    }
    else if(std::holds_alternative<std::vector<Expression*>>(op)) {
        std::vector<Expression*> argument_list = std::get<std::vector<Expression*>>(op);
        for(int i = 0; i < argument_list.size(); i++){
            if(!argument_list[i]->look_for_templates(ctx)) return false;
        }
    }
    else assert(false);
    return true;
}

bool Expression::look_for_templates(CompilationContext *ctx) {
    return expr_node->look_for_templates(ctx);
}