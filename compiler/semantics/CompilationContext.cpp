#include "CompilationContext.h"

#include "DefinitionSpace.h"

#include "TemplatedFunction.h"
#include "TemplatedOperator.h"
#include "TemplatedStructDefinition.h"

#include "Function.h"
#include "Operator.h"
#include "StructDefinition.h"
#include "Constructor.h"
#include "Destructor.h"

#include "StructLayout.h"
#include "GlobalDeclaration.h"
#include "Typedef.h"

#include "Type.h"

#include "FunctionSignature.h"
#include "OperatorSignature.h"
#include "ConstructorSignature.h"

#include "FunctionCall.h"
#include "OperatorCall.h"
#include "ConstructorCall.h"
#include "DestructorCall.h"

#include "Expression.h"

#include "utils.h"
#include "primitives.h"

#include "TemplateHeader.h"
#include "TemplateMapping.h"
#include "Statement.h"
#include "Identifier.h"
#include "Parameter.h"
#include "Declaration.h"


CompilationContext::CompilationContext() {
    // do nothing
}

void CompilationContext::add_definition_space(DefinitionSpace* space, Visibility vis) {
    // check if this is the same as a previously added definition space
    for(int i = 0; i < definition_spaces.size(); i++) {
        DefinitionSpace *_space = definition_spaces[i].space;
        if(_space->get_filepath() == space->get_filepath()) {
            // take the more permissive visibility
            definition_spaces[i].vis = std::max(definition_spaces[i].vis, vis);
            return;
        }
    }
    
    // add it in
    definition_spaces.push_back({space, vis});
}

// TODO 
// not all definition spaces have to actually be in the 'ready' state, for some we can 
// just get away with a state like locally resolved. 
void CompilationContext::ensure_ready() {
    for(DefinitionSpaceView dsv : definition_spaces) {
        dsv.space->ensure_ready();
    }
}

// -- HELPERS --
auto CompilationContext::get_templated_functions() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_templated_functions(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_templated_operators() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_templated_operators(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_templated_structs() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_templated_structs(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_operators() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_operators(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_functions() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_functions(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_structs() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_structs(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_constructors() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_constructors(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_destructors() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_destructors(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_typedefs() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_typedefs(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_global_variables() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_global_variables(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_types() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_types(dsv.vis);
    }) | std::views::join;
}

auto CompilationContext::get_basetypes() const {
    return definition_spaces | std::views::transform([](const DefinitionSpaceView& dsv) {
        return dsv.space->get_basetypes(dsv.vis);
    }) | std::views::join;
}

// -- RESOLVE CALL --

//first, finds all viable functions given some function call. Then:
// - If there is exactly one, returns that one
// - If there are multiple or zero, returns nullptr
//during generation, we convert all l-values into references. 
//within templated function, we handle reference logic. (T cannot be assigned to T&)
//Due to how the partial ordering works, this results in decent reference semantics. 
Function* CompilationContext::get_called_function(FunctionCall* fc) {
    std::cout << "CompilationContext::get_called_function() : " << fc->to_string() << std::endl;
    assert(fc != nullptr);
    
    // - gather all templated functions that work
    std::vector<TemplatedFunction*> viable;
    for(TemplatedFunction *tf : get_templated_functions()) {
        if(tf->calc_mapping(this, fc)) {
            viable.push_back(tf);
        }
    }

    // - find 'most specialized' templated function
    //there exists directed edge (A, B) iff A is a superset of B. 
    //we can tell if A is a superset of B if we can generate a template mapping from A using B's templated signature
    //then, we just look for all functions with outdegree of 0
    std::vector<int> outdeg(viable.size(), 0);
    for(int i = 0; i < viable.size(); i++){
        TemplatedFunction *tf = viable[i];
        std::vector<Type*> arg_types(tf->function->parameters.size());
        for(int j = 0; j < tf->function->parameters.size(); j++){
            arg_types[j] = tf->function->parameters[j]->type->make_copy();
        }
        for(int j = 0; j < viable.size(); j++){
            if(i == j) continue;
            if(viable[j]->calc_mapping(arg_types)) {
                outdeg[j] ++;
            }
        }
    }
    std::vector<TemplatedFunction*> best;
    for(int i = 0; i < viable.size(); i++) {
        if(outdeg[i] == 0) best.push_back(viable[i]);
    }
    if(best.size() == 0) {
        std::cout << "No matching function for signature : " << fc->to_string() << "\n";
        return nullptr;
    }
    else if(best.size() > 1) {
        std::cout << "Ambiguous function call : " << fc->to_string() << "\n";
        return nullptr;
    }

    // - see if a previously generated function works
    for(int i = 0; i < best[0]->generated_functions.size(); i++){
        if(best[0]->generated_functions[i]->is_valid_call(this, fc)) {
            return best[0]->generated_functions[i];
        }
    }

    // - generate function
    assert(best.size() == 1);
    Function *f = best[0]->gen_function(this, fc);
    assert(f != nullptr);
    assert(f->is_valid_call(this, fc));

    // - figure out from where the chosen templated function came from
    DefinitionSpace *template_ds = nullptr;
    for(DefinitionSpaceView dsv : definition_spaces) {
        for(TemplatedFunction *tf : dsv.space->get_templated_functions(dsv.vis)) {
            if(tf == best[0]) {
                template_ds = dsv.space;
            }
        }
    }
    assert(template_ds != nullptr);

    // - generate compilation context for this templated function 
    TemplateMapping *tm = best[0]->calc_mapping(this, fc);
    assert(tm != nullptr);
    std::vector<BaseType*> basetypes;
    tm->find_all_basetypes(basetypes);
    if(f->enclosing_type.has_value()) {
        f->enclosing_type.value()->find_all_basetypes(basetypes);
    }
    CompilationContext *ctx = template_ds->create_compilation_context();
    for(BaseType *bt : basetypes) {
        DefinitionSpace *ds = get_definition_space(bt);
        assert(ds != nullptr);
        ctx->add_definition_space(ds, Visibility::Public);
    }

    std::cout << "BASETYPES : " << std::endl;
    for(BaseType *bt : basetypes) {
        std::cout << "    " << bt->to_string() << std::endl;
    }

    // - add generated function to the definition space its templated counterpart comes from
    // TODO properly figure out visibility of generated function
    if(!add_function(template_ds, f, Visibility::Public, ctx)) {
        assert(false);
    }

    return f;
}

// just turns the function signature into the corresponding function call
Function* CompilationContext::get_called_function(FunctionSignature *fs) {
    std::vector<Expression*> args;
    for(int i = 0; i < fs->input_types.size(); i++) {
        args.push_back(new Expression(new ExprPrimary(fs->input_types[i]->make_copy())));
    }
    FunctionCall *fc = new FunctionCall(std::nullopt, fs->id->make_copy(), args);
    return get_called_function(fc);
}

// this doesn't handle the cast operator, there are some special rules with that one
Operator* CompilationContext::get_called_operator(OperatorCall* oc) {
    //first, some sanity checks
    // - one of left or right has to have a value
    assert(oc->left.has_value() || oc->right.has_value());
    // - check that the values are not nullptr
    if(oc->left.has_value()) assert(oc->left.value() != nullptr);
    if(oc->right.has_value()) assert(oc->right.value() != nullptr);
    // - this should not be the casting operator
    assert(oc->op != "$");

    // - gather all templated operators that work
    std::vector<TemplatedOperator*> viable;
    for(TemplatedOperator *to : get_templated_operators()) {
        if(to->calc_mapping(this, oc)) {
            viable.push_back(to);
        }
    }

    // - find 'most specialized' templated operator
    std::vector<int> outdeg(viable.size(), 0);
    for(int i = 0; i < viable.size(); i++){
        OperatorSignature *os = viable[i]->op->resolve_operator_signature();
        for(int j = 0; j < viable.size(); j++){
            if(i == j) continue;
            if(viable[j]->calc_mapping(os) != nullptr) {
                outdeg[j] ++;
            }
        }
    }
    std::vector<TemplatedOperator*> best;
    for(int i = 0; i < viable.size(); i++){
        if(outdeg[i] == 0) {
            best.push_back(viable[i]);
        }
    }
    if(best.size() == 0) {
        return nullptr;
    }
    else if(best.size() > 1) {
        return nullptr;
    }

    // - see if a previously generated operator works
    for(int i = 0; i < best[0]->generated_operators.size(); i++){
        if(best[0]->generated_operators[i]->is_valid_call(this, oc)) {
            return best[0]->generated_operators[i];
        }
    }

    // - generate operator
    assert(best.size() == 1);
    Operator *o = best[0]->gen_operator(this, oc);
    assert(o != nullptr);
    assert(o->is_valid_call(this, oc));

    // - figure out from where the chosen templated operator came from
    DefinitionSpace *template_ds = nullptr;
    for(DefinitionSpaceView dsv : definition_spaces) {
        for(TemplatedOperator *to : dsv.space->get_templated_operators(dsv.vis)) {
            if(to == best[0]) {

                template_ds = dsv.space;
            }
        }
    }
    assert(template_ds != nullptr);

    // - generate compilation context for this instantiated operator 
    std::cout << "BEST DS FOR OPERATOR : " << template_ds->get_filepath() << " : " << o->resolve_operator_signature()->to_string() << std::endl;
    TemplateMapping *tm = best[0]->calc_mapping(this, oc);
    assert(tm != nullptr);
    std::vector<BaseType*> basetypes;
    tm->find_all_basetypes(basetypes);
    CompilationContext *ctx = template_ds->create_compilation_context();
    for(BaseType *bt : basetypes) {
        DefinitionSpace *ds = get_definition_space(bt);
        assert(ds != nullptr);
        ctx->add_definition_space(ds, Visibility::Public);
    }

    // - add generated operator to the definition space its templated counterpart comes from
    // TODO properly figure out visibility of generated operator
    if(!add_operator(template_ds, o, Visibility::Public, ctx)) {
        assert(false);
    }

    return o;
}

// pretty much like resolving a function call, except for special logic regarding primitive constructors. 
Constructor* CompilationContext::get_called_constructor(ConstructorCall* cc) {
    assert(cc != nullptr);
    Type *type = cc->type;
    std::vector<Expression*> args = cc->argument_list;
    assert(type != nullptr);

    //see if we're doing a primitive default constructor
    if(is_type_primitive(cc->type) && cc->argument_list.size() == 0) {
        // add primitive constructor to builtin definition space
        Constructor *c = new PrimitiveConstructor(cc->type, false);
        add_constructor(builtin_definition_space, c, Visibility::Public, this);
        return c;
    }

    //see if we're doing a primitive copy constructor
    if(is_type_primitive(cc->type) && cc->argument_list.size() == 1 && is_declarable(cc->type, cc->argument_list[0])) {
        // add primitive copy constructor to builtin definition space
        Constructor *c = new PrimitiveConstructor(cc->type, true);
        add_constructor(builtin_definition_space, c, Visibility::Public, this);
        return c;
    }
    
    std::vector<Constructor*> viable;
    for(Constructor *c : get_constructors()){
        ConstructorSignature *ncc = c->resolve_constructor_signature();

        //do the types match?
        if(!cc->type->equals(ncc->type)) {
            continue;
        }
        //do the argument counts match?
        if(args.size() != ncc->input_types.size()) {
            continue;
        }

        //can all args be somehow converted into ncc->input_types?
        bool is_viable = true;
        for(int j = 0; j < args.size(); j++){
            Type *nt = ncc->input_types[j];
            if(!is_declarable(nt, args[j])) {
                is_viable = false;
            }
        }
        if(!is_viable) continue;

        //all checks passed
        viable.push_back(c);
    }

    if(viable.size() == 0) {
        std::cout << "No matching constructor for signature : " << cc->to_string() << "\n";
        return nullptr;
    }
    else if(viable.size() > 1){
        std::cout << "Ambiguous function call : " << cc->to_string() << "\n";
        return nullptr;
    }
    return viable[0];
}

// just look for a destructor of the same type
Destructor* CompilationContext::get_called_destructor(DestructorCall* dc) {
    assert(dc != nullptr);

    for(Destructor *d : get_destructors()){
        if(dc->type->equals(d->type)) {
            return d;
        }
    }

    std::cout << "Unable to find destructor for type : " << dc->type->to_string() << "\n";
    return nullptr;
}

Operator* CompilationContext::get_called_operator(std::optional<Expression*> _left, std::string op, std::optional<Expression*> _right) {
    std::optional<ExprNode*> left = std::nullopt, right = std::nullopt;
    if(_left.has_value()) left = _left.value()->expr_node;
    if(_right.has_value()) right = _right.value()->expr_node;
    OperatorCall *oc = new OperatorCall(left, op, right);
    return get_called_operator(oc);
}

Operator* CompilationContext::get_called_operator(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right) {
    OperatorCall *oc = new OperatorCall(left, op, right);
    return get_called_operator(oc);
}

Operator* CompilationContext::get_called_typecast(Type* from, Type* to) {
    //special cases
    // - from and to are the same type
    if(*from == *to) {
        return new BuiltinOperator(to->make_copy(), from->make_copy(), "$", to->make_copy(), {});     //do nothing
    }
    Type* voidptr_t = new PointerType(primitives::_void->make_copy());
    // - from is a pointer, to is void*
    if(dynamic_cast<PointerType*>(from) != nullptr && *to == *voidptr_t) {
        return new BuiltinOperator(to->make_copy(), from->make_copy(), "$", to->make_copy(), {});     //do nothing
    }
    // - from is void*, to is a pointer
    if(*from == *voidptr_t && dynamic_cast<PointerType*>(to) != nullptr) {
        return new BuiltinOperator(to->make_copy(), from->make_copy(), "$", to->make_copy(), {});     //do nothing
    }
    // - from is a pointer, to is a pointer
    if(dynamic_cast<PointerType*>(from) != nullptr && dynamic_cast<PointerType*>(to) != nullptr) {
        return new BuiltinOperator(to->make_copy(), from->make_copy(), "$", to->make_copy(), {});     //do nothing
    }
    // - from is a pointer, to is an u64
    if(dynamic_cast<PointerType*>(from) != nullptr && to->equals(primitives::u64)) {
        return new BuiltinOperator(to->make_copy(), from->make_copy(), "$", to->make_copy(), {});     //do nothing
    }
    // - from is an u64, to is a pointer
    if(from->equals(primitives::u64) && dynamic_cast<PointerType*>(to) != nullptr) {
        return new BuiltinOperator(to->make_copy(), from->make_copy(), "$", to->make_copy(), {});     //do nothing
    }
    // - from is a function pointer, to is an u64
    if(dynamic_cast<FunctionPointerType*>(from) != nullptr && to->equals(primitives::u64)) {
        return new BuiltinOperator(to->make_copy(), from->make_copy(), "$", to->make_copy(), {});     //do nothing
    }

    //look through all conversions to see if we have an exact match
    // TODO consider templated cast operators
    std::vector<Operator*> viable;
    OperatorSignature *key = new OperatorSignature(from, to);
    for(Operator *o : get_operators()){
        if(key->equals(o->resolve_operator_signature())) {
            viable.push_back(o);
        }
    }
    if(viable.size() != 1) return nullptr;
    return viable[0];
}

// -- EXPR UTILITIES --
Type* CompilationContext::find_resulting_type(std::optional<Expression*> left, std::string op, std::optional<Expression*> right) {
    Operator *o = get_called_operator(left, op, right);
    if(o == nullptr) return nullptr;
    return o->type;
}

Type* CompilationContext::find_resulting_type(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right) {
    Operator *o = get_called_operator(left, op, right);
    if(o == nullptr) return nullptr;
    return o->type;
}

//returns true if we can create a new variable of type A with the given expression
//to avoid exponential runtime on nested function calls, I cache the results from previous calls. 
//this function should be a pure function (once we transform Expression into some standard form). 
//Observe that the resulting type of Expression is dependent on the types of the Identifiers within it. 
//So if we replace all the Identifiers within the expression with their corresponding type, we can use
//this as a key to cache. 
//It's a good assumption that the resulting type of some function will not be dependent on the value of 
//the variables, so this should work for now. 
bool CompilationContext::is_declarable(Type *A, Expression *expr) {
    assert(A != nullptr);
    assert(expr != nullptr);

    std::pair<Type*, Expression*> pkey = std::make_pair(A, expr->make_copy());
    pkey.second->id_to_type();
    if(is_declarable_cache.count(pkey)) {
        is_declarable_cntr ++;
        return is_declarable_cache[pkey];
    }

    bool ans = false;
    {
        Type *et = expr->resolve_type(this);
        bool is_lvalue = expr->is_lvalue(this);
        if(et == nullptr) {
            ans = false;
            goto done;
        }

        // if we are assigning to a reference, we must match the type exactly and use a l-value
        if(dynamic_cast<ReferenceType*>(A) != nullptr) {
            if(is_lvalue && *(dynamic_cast<ReferenceType*>(A)->type) == *et) {
                ans = true;
                goto done;
            }
            ans = false;
            goto done;
        }

        // otherwise, we just have to match the type exactly
        if(A->equals(et)) {
            ans = true;
            goto done;
        }
        ans = false;
        goto done;

        // // otherwise, can look for type conversions
        // Expression *a_expr = new Expression(new ExprBinary(new ExprPrimary(new ReferenceType(A)), "=", new ExprPrimary(expr)));
        // if(a_expr->resolve_type() != nullptr) {
        //     ans = true;
        //     goto done;
        // }
        // ans = false;
        // goto done;
    }
    done: {}

    is_declarable_cache[pkey] = ans;
    return ans;
}

// -- CHECKING IS DECLARED --
bool CompilationContext::is_type_declared(Type* t) const {\
    assert(t != nullptr);
    if(auto x = dynamic_cast<PointerType*>(t)) return is_type_declared(x->type);
    if(auto x = dynamic_cast<ReferenceType*>(t)) return is_type_declared(x->type);
    if(auto x = dynamic_cast<FunctionPointerType*>(t)) {
        if(!is_type_declared(x->return_type)) return false;
        for(int i = 0; i < x->param_types.size(); i++){
            if(!is_type_declared(x->param_types[i])) return false;
        }
        return true;
    }
    for(Type *ot : get_types()) {
        if(t->equals(ot)) {
            return true;
        }
    }
    return false;
}

bool CompilationContext::is_basetype_declared(BaseType* t) const {
    for(BaseType *ot : get_basetypes()) {
        if(t->equals(ot)) {
            return true;
        }
    }
    return false;
}


// -- MISC --
void CompilationContext::dump_context() {
    std::cout << "Context Dump : " << std::endl;
    for(DefinitionSpaceView dsv : definition_spaces) {
        std::cout << "    " << dsv.space->get_filepath() << std::endl;
    }
}


