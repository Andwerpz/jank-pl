#include <chrono>
#include <algorithm>
#include <iomanip>
#include <functional>

#include "utils.h"
#include "Type.h"
#include "Function.h"
#include "Expression.h"
#include "Identifier.h"
#include "FunctionSignature.h"
#include "FunctionCall.h"
#include "StructDefinition.h"
#include "Constructor.h"
#include "ConstructorSignature.h"
#include "ConstructorCall.h"
#include "TemplatedStructDefinition.h"
#include "Program.h"
#include "TemplatedFunction.h"
#include "StructLayout.h"
#include "primitives.h"
#include "Destructor.h"
#include "DestructorCall.h"
#include "Statement.h"
#include "Literal.h"
#include "Parameter.h"
#include "Declaration.h"
#include "TemplateHeader.h"
#include "OperatorSignature.h"
#include "Operator.h"
#include "TemplatedOperator.h"
#include "OperatorCall.h"

ld current_time_seconds() {
    using namespace std::chrono;
    auto now = system_clock::now().time_since_epoch();
    return duration_cast<duration<ld>>(now).count();
}

std::vector<std::pair<std::string, ld>> duration_stats;
void add_duration_stat(std::string name, ld dur) {
    duration_stats.push_back({name, dur});
}

void print_duration_stats() {
    std::sort(duration_stats.begin(), duration_stats.end(), [](std::pair<std::string, ld> a, std::pair<std::string, ld> b) -> bool {
        return a.second < b.second;
    });
    for(int i = 0; i < duration_stats.size(); i++){
        std::cout << duration_stats[i].first << " : " << std::fixed << std::setprecision(3) << duration_stats[i].second << "\n";
    }
}

Variable::Variable(bool _is_global, bool _is_extern, Type *_type, Identifier *_id, std::string _addr) {
    is_global = _is_global;
    is_extern = _is_extern;
    id = _id;
    type = _type;
    addr = _addr;
}

LoopContext::LoopContext(std::string _start_label, std::string _assignment_label, std::string _end_label, int _declaration_layer) {
    start_label = _start_label;
    assignment_label = _assignment_label;
    end_label = _end_label;
    declaration_layer = _declaration_layer;
}

//define hash and eq structs in anon
namespace {
    struct TypeHash {
        size_t operator()(Type* t) const {
            return t ? t->hash() : 0;
        }
    };
    struct TypeEquals {
        bool operator()(Type* lhs, Type* rhs) const {
            return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
        }
    };

    struct IdentifierHash {
        size_t operator()(Identifier* id) const {
            return id ? id->hash() : 0;
        }
    };
    struct IdentifierEquals {
        bool operator()(Identifier *lhs, Identifier *rhs) const {
            return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
        }
    };

    struct FunctionSignatureHash {
        size_t operator()(FunctionSignature *fc) const {
            return fc ? fc->hash() : 0;
        }
    };
    struct FunctionSignatureEquals {
        bool operator()(FunctionSignature* lhs, FunctionSignature *rhs) const {
            return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
        }
    };

    struct ConstructorSignatureHash {
        size_t operator()(ConstructorSignature *cs) const {
            return cs ? cs->hash() : 0;
        }
    };
    struct ConstructorSignatureEquals {
        bool operator()(ConstructorSignature *lhs, ConstructorSignature *rhs) const {
            return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
        }
    };

    struct OperatorSignatureHash {
        size_t operator()(OperatorSignature *os) const {
            return os ? os->hash() : 0;
        }
    };
    struct OperatorSignatureEquals {
        bool operator()(OperatorSignature* lhs, OperatorSignature *rhs) const {
            return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
        }
    };

    struct DeclarableHash {
        size_t operator()(std::pair<Type*, Expression*> p) const {
            size_t hash = 0;
            if(p.first) hash_combine(hash, p.first->hash());
            else hash_combine(hash, 0);
            if(p.second) hash_combine(hash, p.second->hash());
            else hash_combine(hash, 0);
            return hash;
        }
    };
    struct DeclarableEquals {
        bool operator()(std::pair<Type*, Expression*> lhs, std::pair<Type*, Expression*> rhs) const {
            if((lhs.first == nullptr) ^ (rhs.first == nullptr)) return false;
            if((lhs.second == nullptr) ^ (rhs.second == nullptr)) return false;
            if(lhs.first && !lhs.first->equals(rhs.first)) return false;
            if(lhs.second && !lhs.second->equals(rhs.second)) return false;
            return true;
        }
    };
}

std::vector<Type*> declared_types;
std::vector<TemplatedStructDefinition*> declared_templated_structs;
std::vector<TemplatedFunction*> declared_templated_functions;
std::vector<TemplatedOperator*> declared_templated_operators;
std::vector<BaseType*> declared_basetypes;
std::unordered_set<Type*, TypeHash, TypeEquals> primitive_base_types;
std::vector<std::pair<Type*, StructLayout*>> struct_layout_map;
std::unordered_map<FunctionSignature*, std::string, FunctionSignatureHash, FunctionSignatureEquals> function_label_map;
std::unordered_map<ConstructorSignature*, std::string, ConstructorSignatureHash, ConstructorSignatureEquals> constructor_label_map;
std::unordered_map<Type*, std::string, TypeHash, TypeEquals> destructor_label_map;
std::unordered_map<OperatorSignature*, std::string, OperatorSignatureHash, OperatorSignatureEquals> operator_label_map;
std::unordered_map<std::string, std::string> string_literal_label_map;
std::vector<Variable*> declared_global_variables;

int label_counter;
int tmp_variable_counter;

void reset_controller() {
    enclosing_type = std::nullopt;
    enclosing_return_type = nullptr;

    declared_types.clear();
    declared_basetypes.clear();
    declared_structs.clear();
    declared_templated_structs.clear();
    declared_templated_functions.clear();
    declared_templated_operators.clear();
    primitive_base_types.clear();
    struct_layout_map.clear();

    declared_functions.clear();
    function_label_map.clear();

    declared_operators.clear();
    operator_label_map.clear();

    declared_constructors.clear();
    constructor_label_map.clear();

    declared_destructors.clear();
    destructor_label_map.clear();
    
    declared_variables.clear();
    declared_global_variables.clear();
    while(declaration_stack.size()) declaration_stack.pop_back();

    local_offset = 0;
    stack_desc.clear();

    label_counter = 0;
    tmp_variable_counter = 0;

    primitives::init_primitives();
}

void hash_combine(size_t& seed, size_t value) {
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

std::string indent() {
    return "    ";  //4 spaces
}

std::string create_new_label() {
    std::string ans = "L" + std::to_string(label_counter++);
    return ans;
}

std::string create_new_tmp_variable_name() {
    std::string ans = "TMP:" + std::to_string(tmp_variable_counter ++);
    return ans;
}

//"n" | "t" | "r" | "f" | "b" | "\"" | "\\" | "'" | "0"
char escape_to_char(parser::escape *e) {
    char val;
    char eid = e->to_string()[1];
    if(eid == 'n') val = '\n';
    else if(eid == 't') val = '\t';
    else if(eid == 'r') val = '\r';
    else if(eid == 'f') val = '\f';
    else if(eid == 'b') val = '\b';
    else if(eid == '\"') val = '\"';
    else if(eid == '\\') val = '\\';
    else if(eid == '\'') val = '\'';
    else if(eid == '0') val = '\0';
    else assert(false);
    return val;
}

void dump_stack_desc() {
    for(int i = 0; i < stack_desc.size(); i++){
        std::cout << stack_desc[i] << std::endl;
    }
}

void emit_push(std::string reg, std::string desc){
    stack_desc.push_back(desc);
    fout << indent() << "push " << reg << (asm_debug? "    # " + desc : "") <<"\n";
    local_offset -= 8;
}

void emit_pop(std::string reg, std::string desc){
    if(stack_desc.size() == 0) {
        std::cout << "Stack empty : " << desc << std::endl;
        assert(false);
    }
    if(stack_desc[stack_desc.size() - 1] != desc) {
        std::cout << "Stack desc doesn't match : " << desc << std::endl;
        dump_stack_desc();
        assert(false);
    }
    stack_desc.pop_back();

    fout << indent() << "pop " << reg << (asm_debug? "    # " + desc : "") <<"\n";
    local_offset += 8;
}

void emit_add_rsp(int amt, std::string desc){
    assert(amt % 8 == 0);
    assert(amt >= 0);
    for(int i = 0; i < amt / 8; i++) {
        if(stack_desc.size() == 0) {
            std::cout << "Stack empty : " << desc << std::endl;
            assert(false);
        }
        if(stack_desc[stack_desc.size() - 1] != desc) {
            std::cout << "Stack desc doesn't match : " << desc << std::endl;
            dump_stack_desc();
            assert(false);
        }
        stack_desc.pop_back();
    }

    if(asm_debug) fout << indent() << "# " << desc << "\n";
    fout << indent() << "add $" << amt << ", %rsp\n";
    local_offset += amt;
}

void emit_add_rsp(int amt, std::vector<std::string> desc_list) {
    assert(amt % 8 == 0);
    assert(amt >= 0);
    assert(amt / 8 == desc_list.size());

    if(desc_list.size() > stack_desc.size()) {
        std::cout << "Not enough elements in stack : multi emit add" << std::endl;
        dump_stack_desc();
        assert(false);
    }

    for(int i = 0; i < amt / 8; i++) {
        if(stack_desc[stack_desc.size() - 1] != desc_list[desc_list.size() - 1]) {
            std::cout << "Stack desc doesn't match : " << desc_list[desc_list.size() - 1] << std::endl;
            dump_stack_desc();
            assert(false);
        }
        stack_desc.pop_back();
        desc_list.pop_back();
    }

    fout << indent() << "add $" << amt << ", %rsp\n";
    local_offset += amt;
}

void emit_sub_rsp(int amt, std::string desc) {
    assert(amt % 8 == 0);
    assert(amt >= 0);
    for(int i = 0; i < amt / 8; i++){
        stack_desc.push_back(desc);
    }

    if(asm_debug) fout << indent() << "# " << desc << "\n";
    fout << indent() << "sub $" << amt << ", %rsp\n";
    local_offset -= amt;
}

bool is_type_primitive(Type *t) {
    if(primitive_base_types.count(t)) return true;          //is it explicitly mentioned?
    if(dynamic_cast<PointerType*>(t)) return true;          //is it a pointer?
    if(dynamic_cast<ReferenceType*>(t)) return true;        //is it a reference?
    if(dynamic_cast<FunctionPointerType*>(t)) return true;  //is it a function pointer?
    return false;
}

//expects %rax = array start, %rbx = array index
//will put return value into %rax
//always zero extends %rax
void emit_retrieve_array(int sz) {
    if(sz == 1) fout << indent() << "movzbl (%rax, %rbx, 1), %eax\n";
    else if(sz == 2) fout << indent() << "movzwl (%rax, %rbx, 2), %eax\n";
    else if(sz == 4) fout << indent() << "movl (%rax, %rbx, 4), %eax\n";
    else if(sz == 8) fout << indent() << "movq (%rax, %rbx, 8), %rax\n";
    else {
        std::cout << "Cannot retrieve from array with element of size : " << sz << std::endl;
        assert(false);
    }
}

//expects %rax = array start, %rbx = array index
//will put address of element into %rax
void emit_address_array(int sz) {
    if(sz == 1) fout << indent() << "lea (%rax, %rbx, 1), %rax\n";
    else if(sz == 2) fout << indent() << "lea (%rax, %rbx, 2), %rax\n";
    else if(sz == 4) fout << indent() << "lea (%rax, %rbx, 4), %rax\n";
    else if(sz == 8) fout << indent() << "lea (%rax, %rbx, 8), %rax\n";
    else {
        emit_push("%rax", "emit_address_array() : save array start");
        fout << indent() << "mov $" << sz << ", %rax\n";
        fout << indent() << "imul %rax, %rbx\n";
        emit_pop("%rax", "emit_address_array() : save array start");
        fout << indent() << "add %rbx, %rax\n";
    }
}

//expects %rax = array start, %rbx = array index, %rcx = value
//writes %rcx (or whatever portion of %rcx) into array
void emit_write_array(int sz) {
    if(sz == 1) fout << indent() << "movb %cl, (%rax, %rbx, 1)\n";
    else if(sz == 2) fout << indent() << "movw %cx, (%rax, %rbx, 2)\n";
    else if(sz == 4) fout << indent() << "movl %ecx, (%rax, %rbx, 4)\n";
    else if(sz == 8) fout << indent() << "movq %rcx, (%rax, %rbx, 8)\n";
    else {
        std::cout << "Cannot write array with element of size : " << sz << std::endl;
        assert(false);
    }
}

//expects %rax = address
//will put return value into %rax
//always zero extends %rax if the amount is less than 8 bytes
void emit_mem_retrieve(int sz) {
    if(sz == 1) fout << indent() << "movzbq (%rax), %rax\n";
    else if(sz == 2) fout << indent() << "movzwq (%rax), %rax\n";
    else if(sz == 4) fout << indent() << "movl (%rax), %eax\n";
    else if(sz == 8) fout << indent() << "movq (%rax), %rax\n";
    else {
        std::cout << "Cannot retrieve element of size : " << sz << std::endl;
        assert(false);
    }   
}

//expects %rax = value, %rbx = mem address
//stores %rax into (%rbx)
void emit_mem_store(int sz) {
    if(sz == 1) fout << indent() << "movb %al, (%rbx)\n";
    else if(sz == 2) fout << indent() << "movw %ax, (%rbx)\n";
    else if(sz == 4) fout << indent() << "movl %eax, (%rbx)\n";
    else if(sz == 8) fout << indent() << "movq %rax, (%rbx)\n";
    else {
        std::cout << "Cannot store element of size : " << sz << std::endl;
        assert(false);
    }
}

//returns true if we can create a new variable of type A with the given expression
//to avoid exponential runtime on nested function calls, I cache the results from previous calls. 
//this function should be a pure function (once we transform Expression into some standard form). 
//Observe that the resulting type of Expression is dependent on the types of the Identifiers within it. 
//So if we replace all the Identifiers within the expression with their corresponding type, we can use
//this as a key to cache. 
//It's a good assumption that the resulting type of some function will not be dependent on the value of 
//the variables, so this should work for now. 
int is_declarable_cntr;
std::unordered_map<std::pair<Type*, Expression*>, bool, DeclarableHash, DeclarableEquals> is_declarable_cache;
bool is_declarable(Type *A, Expression *expr) {
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
        Type *et = expr->resolve_type();
        bool is_lvalue = expr->is_lvalue();
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

Type* find_resulting_type(std::optional<Expression*> left, std::string op, std::optional<Expression*> right) {
    Operator *o = get_called_operator(left, op, right);
    if(o == nullptr) return nullptr;
    return o->type;
}

Type* find_resulting_type(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right) {
    Operator *o = get_called_operator(left, op, right);
    if(o == nullptr) return nullptr;
    return o->type;
}

Type* find_variable_type(Identifier *id) {
    for(int i = 0; i < declared_variables.size(); i++){
        if(*(declared_variables[i]->id) == *id) return declared_variables[i]->type;
    }
    return nullptr;
}

Type* find_function_type(FunctionSignature *fs) {
    assert(fs != nullptr);
    for(int i = 0; i < declared_functions.size(); i++){
        if(*(declared_functions[i]->resolve_function_signature()) == *fs) return declared_functions[i]->type;
    }
    return nullptr;
}

bool is_type_declared(Type *t) {
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
    for(int i = 0; i < declared_types.size(); i++){
        if(*(declared_types[i]) == *t) return true;
    }
    return false;
}

bool is_basetype_declared(BaseType* t) {
    assert(t != nullptr);
    for(int i = 0; i < declared_basetypes.size(); i++){
        if(t->equals(declared_basetypes[i])) return true;
    }
    return false;
}

//just checks the basetype against all other declared templated struct defs
bool is_templated_struct_declared(TemplatedStructDefinition *t) {
    assert(t != nullptr);
    for(int i = 0; i < declared_templated_structs.size(); i++){
        if(t->struct_def->type->equals(declared_templated_structs[i]->struct_def->type)) return true;
    }
    return false;
}

//just checks to make sure that it's composed of declared base types. 
//also, none of the template types can be ReferenceType
bool is_templated_type_well_formed(TemplatedType *t) {
    assert(t != nullptr);
    if(!is_basetype_declared(t->base_type)) return false;
    for(int i = 0; i < t->template_types.size(); i++){
        Type *nt = t->template_types[i];
        assert(nt != nullptr);
        while(true) {
            if(dynamic_cast<ReferenceType*>(nt)) return false;
            else if(auto x = dynamic_cast<PointerType*>(nt)) {
                nt = x->type;
            }
            else if(auto x = dynamic_cast<BaseType*>(nt)) {
                if(!is_basetype_declared(x)) return false;
                break;
            }
            else if(auto x = dynamic_cast<TemplatedType*>(nt)) {
                if(!is_templated_type_well_formed(x)) return false;
                break;
            }
            else if(auto x = dynamic_cast<ArrayType*>(nt)) {
                nt = x->type;
            }
            else assert(false);
        }
    }
    return true;
}

bool is_function_declared(FunctionSignature *fs) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(fs->equals(declared_functions[i]->resolve_function_signature())) {
            return true;
        }
    }
    return false;
}

bool is_operator_declared(OperatorSignature *os) {
    for(int i = 0; i < declared_operators.size(); i++){
        if(os->equals(declared_operators[i]->resolve_operator_signature())) {
            return true;
        }
    }
    return false;
}

bool is_variable_declared(Identifier *id) {
    for(int i = 0; i < declared_variables.size(); i++){
        if(*id == *(declared_variables[i]->id)) {
            return true;
        }
    }
    return false;
}

bool is_constructor_declared(ConstructorSignature *cs) {
    assert(cs != nullptr);
    for(int i = 0; i < declared_constructors.size(); i++){
        if(cs->equals(declared_constructors[i]->resolve_constructor_signature())) {
            return true;
        }
    }
    return false;
}

bool is_destructor_declared(Type *t) {
    assert(t != nullptr);
    for(int i = 0; i < declared_destructors.size(); i++){
        if(t->equals(declared_destructors[i]->type)) {
            return true;
        }
    }
    return false;
}

Function* get_function(FunctionSignature *fs) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(*fs == *(declared_functions[i]->resolve_function_signature())) {
            return declared_functions[i];
        }
    }
    return nullptr;
}

//finds all viable functions given some function call
// - If there is exactly one, returns that one
// - If there are multiple or zero, returns nullptr
Function* get_called_function(FunctionCall *fc) {
    assert(fc != nullptr);
    //we convert all l-values into references. 
    //within templated function, we handle reference logic. (T cannot be assigned to T&)
    //Due to how the partial ordering works, this results in decent reference semantics. 
    
    // - gather all templated functions that work
    std::vector<TemplatedFunction*> viable;
    for(int i = 0; i < declared_templated_functions.size(); i++){
        TemplatedFunction *tf = declared_templated_functions[i];
        if(tf->calc_mapping(fc)) {
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

    // - generate function
    assert(best.size() == 1);
    Function *f = best[0]->gen_function(fc);
    assert(f != nullptr);
    assert(f->is_valid_call(fc));

    return f;
}

//just turns the signature into a corresponding call 
Function* get_called_function(FunctionSignature *fs) {
    std::vector<Expression*> args;
    for(int i = 0; i < fs->input_types.size(); i++) {
        args.push_back(new Expression(new ExprPrimary(fs->input_types[i]->make_copy())));
    }
    FunctionCall *fc = new FunctionCall(std::nullopt, fs->id->make_copy(), args);
    return get_called_function(fc);
}

//this doesn't handle the cast operator, there are some special rules with that one. 
Operator* get_called_operator(OperatorCall *oc) {
    // std::cout << "GET CALLED OPERATOR : " << oc->to_string() << "\n";
    //some sanity checks
    //one of left or right has to have a value
    assert(oc->left.has_value() || oc->right.has_value());
    //check that the values are not nullptr
    if(oc->left.has_value()) assert(oc->left.value() != nullptr);
    if(oc->right.has_value()) assert(oc->right.value() != nullptr);
    //this should not be the casting operator
    assert(oc->op != "$");

    // - gather all templated operators that work
    std::vector<TemplatedOperator*> viable;
    for(int i = 0; i < declared_templated_operators.size(); i++){
        TemplatedOperator *to = declared_templated_operators[i];
        if(to->calc_mapping(oc)) {
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

    // - generate operator
    assert(best.size() == 1);
    Operator *o = best[0]->gen_operator(oc);
    assert(o != nullptr);
    assert(o->is_valid_call(oc));

    // std::cout << "FOUND OPERATOR : " << o->resolve_operator_signature()->to_string() << "\n";

    return o;
}

Operator* get_called_operator(std::optional<Expression*> _left, std::string op, std::optional<Expression*> _right) {
    std::optional<ExprNode*> left = std::nullopt, right = std::nullopt;
    if(_left.has_value()) left = _left.value()->expr_node;
    if(_right.has_value()) right = _right.value()->expr_node;
    OperatorCall *oc = new OperatorCall(left, op, right);
    return get_called_operator(oc);
}

Operator* get_called_operator(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right) {
    OperatorCall *oc = new OperatorCall(left, op, right);
    return get_called_operator(oc);
}

//requires that the types exactly match
Operator* get_called_typecast(Type *from, Type *to) {
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
    std::vector<Operator*> viable;
    OperatorSignature *key = new OperatorSignature(from, to);
    for(int i = 0; i < declared_operators.size(); i++){
        if(key->equals(declared_operators[i]->resolve_operator_signature())) {
            viable.push_back(declared_operators[i]);
        }
    }
    if(viable.size() != 1) return nullptr;
    return viable[0];
}

//pretty much exactly the same as get_called_function
//has some special logic with primitive 
Constructor* get_called_constructor(ConstructorCall *cc) {
    assert(cc != nullptr);
    Type *type = cc->type;
    std::vector<Expression*> args = cc->argument_list;
    assert(type != nullptr);

    //see if we're doing a primitive default constructor
    if(is_type_primitive(cc->type) && cc->argument_list.size() == 0) {
        Constructor *c = new PrimitiveConstructor(cc->type, false);
        if(!is_constructor_declared(c->resolve_constructor_signature())) add_constructor(c);
        return c;
    }

    //see if we're doing a primitive copy constructor
    if(is_type_primitive(cc->type) && cc->argument_list.size() == 1 && is_declarable(cc->type, cc->argument_list[0])) {
        Constructor *c = new PrimitiveConstructor(cc->type, true);
        if(!is_constructor_declared(c->resolve_constructor_signature())) add_constructor(c);
        return c;
    }
    
    std::vector<Constructor*> viable;
    for(int i = 0; i < declared_constructors.size(); i++){
        ConstructorSignature *ncc = declared_constructors[i]->resolve_constructor_signature();

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
        viable.push_back(declared_constructors[i]);
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

//just look for a destructor of the same type
Destructor* get_called_destructor(DestructorCall *dc) {
    assert(dc != nullptr);

    for(int i = 0; i < declared_destructors.size(); i++){
        if(dc->type->equals(declared_destructors[i]->type)) {
            return declared_destructors[i];
        }
    }

    std::cout << "Unable to find destructor for type : " << dc->type->to_string() << "\n";
    return nullptr;
}

std::string get_function_label(FunctionSignature *fs) {
    assert(fs != nullptr);
    if(!function_label_map.count(fs)) {
        std::cout << "Unable to find label for : " << fs->to_string() << "\n";
        assert(false);
    }
    return function_label_map[fs];
}

std::string get_constructor_label(ConstructorSignature *cs) {
    assert(cs != nullptr);
    assert(constructor_label_map.count(cs));
    return constructor_label_map[cs];
}

std::string get_destructor_label(Type *t) {
    assert(t != nullptr);
    assert(destructor_label_map.count(t));
    return destructor_label_map[t];
}

std::string get_operator_label(OperatorSignature *os) {
    assert(os != nullptr);
    assert(operator_label_map.count(os));
    return operator_label_map[os];
}

Variable* get_variable(Identifier *id) {
    for(int i = 0; i < declared_variables.size(); i++){
        if(*id == *(declared_variables[i]->id)) {
            return declared_variables[i];
        }
    }
    return nullptr;
}

//declaring a new concrete (no template variables) version of a struct
//if the struct doesn't have any templates, it may already be declared
bool add_struct_type(StructDefinition *sd) {
    assert(sd != nullptr);
    Type *t = sd->type;
    if(!is_type_declared(t)) {
        assert(!(dynamic_cast<BaseType*>(t)));  //type should not be a BaseType
        declared_types.push_back(t);
    }
    declared_structs.push_back(sd);

    //add all functions and constructors
    for(int i = 0; i < sd->functions.size(); i++){
        TemplatedFunction *f = sd->functions[i];
        if(!add_templated_function(f)) {
            std::cout << "Failed to add struct member function : " << f->function->resolve_function_signature()->to_string() << "\n";
            return false;
        } 
    }
    for(int i = 0; i < sd->constructors.size(); i++) {
        Constructor *c = sd->constructors[i];
        if(!add_constructor(c)) {
            std::cout << "Failed to add struct constructor : " << c->resolve_constructor_signature()->to_string() << "\n";
            return false;
        }
    }

    //add destructor 
    for(int i = 0; i < sd->destructors.size(); i++){
        if(!add_destructor(sd->destructors[i])) {
            std::cout << "Failed to add destructor : " << t->to_string() << "\n";
            return false;
        }
    }

    //default default constructor
    add_constructor(new StructConstructor(t->make_copy(), {}, new CompoundStatement({})));

    //default copy constructor
    // - for each field, just does default copy constructor
    {
        Identifier *oid = new Identifier("other");
        Identifier *tid = new Identifier("this");

        std::vector<Statement*> statements;
        for(int i = 0; i < sd->member_variables.size(); i++){
            Identifier *mvi = sd->member_variables[i]->id;
            statements.push_back(
                new ExpressionStatement(new Expression(new ExprBinary(
                    new ExprPostfix(
                        new ExprPrimary(tid),
                        std::make_pair(".", mvi)
                    ),
                    "=",
                    new ExprPostfix(
                        new ExprPrimary(oid),
                        std::make_pair(".", mvi)
                    )
                )))
            );
        }

        CompoundStatement *body = new CompoundStatement(statements);
        add_constructor(new StructConstructor(
            t->make_copy(), 
            {new Parameter(new ReferenceType(t->make_copy()), oid->make_copy())}, 
            body
        ));
    }

    //default destructor
    add_destructor(new Destructor(t->make_copy(), new CompoundStatement({})));

    //resolve all templates in function signature and member variables
    if(!sd->look_for_templates()) {
        std::cout << "Unable to resolve all templates in " << t->to_string() << "\n";
        return false;
    }

    std::cout << "ADD TYPE : " << t->to_string() << "\n";

    return true;
}

bool add_primitive_basetype(BaseType *t) {
    assert(t != nullptr);
    if(is_type_declared(t)) return false;
    if(!add_basetype(t)) return false;
    declared_types.push_back(t);
    primitive_base_types.insert(t);
    return true;
}

bool add_basetype(BaseType *t) {
    assert(t != nullptr);
    if(is_basetype_declared(t)) return false;
    declared_types.push_back(t);
    declared_basetypes.push_back(t);
    return true;
}

bool add_arraytype(ArrayType *t) {
    assert(t != nullptr);
    if(is_type_declared(t)) return false;
    declared_types.push_back(t);
    return true;
}

bool add_templated_struct(TemplatedStructDefinition *t) {
    assert(t != nullptr);
    if(is_templated_struct_declared(t)) return false;
    declared_templated_structs.push_back(t);
    std::cout << "ADD TEMPLATED STRUCT : " << t->struct_def->type->to_string() << "\n";
    return true;
}

bool add_templated_function(TemplatedFunction *f) {
    assert(f != nullptr);

    //check if this is a duplicate of another templated function
    //do this by seeing if we can draw a bijection between the sets of calls that f and another function can handle
    std::vector<Type*> farg_list;
    for(int i = 0; i < f->function->parameters.size(); i++){
        farg_list.push_back(f->function->parameters[i]->type->make_copy());
    }
    for(int i = 0; i < declared_templated_functions.size(); i++){
        TemplatedFunction *of = declared_templated_functions[i];

        // - do they have the same identifier?
        if(!f->function->id->equals(of->function->id)) {
            continue;
        }

        // - are enclosing types the same?
        if(f->function->enclosing_type.has_value() != of->function->enclosing_type.has_value()) continue;
        if(f->function->enclosing_type.has_value() && !f->function->enclosing_type.value()->equals(of->function->enclosing_type.value())) continue;

        // - can they both map to eachother?
        std::vector<Type*> arg_list;    
        for(int j = 0; j < of->function->parameters.size(); j++){
            arg_list.push_back(of->function->parameters[j]->type->make_copy());
        }
        if(of->calc_mapping(farg_list) == nullptr || f->calc_mapping(arg_list) == nullptr) {
            continue;
        }

        //found bijection
        std::cout << "Duplicate functions : " << f->function->resolve_function_signature()->to_string() << " : " << of->function->resolve_function_signature()->to_string() << "\n";
        return false;
    }

    //ok, good to go
    declared_templated_functions.push_back(f);

    //if this isn't really a templated function (empty header), go ahead and register it as function
    if(f->header->types.size() == 0) {
        add_function(f->function);
        f->generated_functions.push_back(f->function);
    }
    else {
        std::cout << "ADD TEMPLATED FUNCTION : " << f->function->resolve_function_signature()->to_string() << "\n";
    }

    return true;
}

bool add_templated_operator(TemplatedOperator *o) {
    assert(o != nullptr);

    //check if this is a duplicate of another templated operator
    OperatorSignature *os = o->op->resolve_operator_signature();
    for(int i = 0; i < declared_templated_operators.size(); i++){
        TemplatedOperator *oo = declared_templated_operators[i];
        OperatorSignature *oos = oo->op->resolve_operator_signature();

        // - can they both map to eachother?
        if(o->calc_mapping(oos) == nullptr || oo->calc_mapping(os) == nullptr) {
            continue;
        }

        //found bijection
        std::cout << "Duplicate operators : " << os->to_string() << " : " << oos->to_string() << "\n";
        return false;
    }

    //ok, good to go
    declared_templated_operators.push_back(o);

    //if this doesn't have any template variables, register it as an operator
    if(o->header->types.size() == 0) {
        add_operator(o->op);
        o->generated_operators.push_back(o->op);
    }
    else {
        std::cout << "ADD TEMPLATED OPERATOR : " << o->op->resolve_operator_signature()->to_string() << "\n";
    }

    return true;
}   

//look through all declared templated struct defs. If one matches, create it. 
//returns false if it creates a malformed type, or can't create the type
bool create_templated_type(TemplatedType *t) {
    //have we already generated this type?
    if(is_type_declared(t)) return true;
    //try to generate it
    for(int i = 0; i < declared_templated_structs.size(); i++){
        TemplatedStructDefinition *tsd = declared_templated_structs[i];
        StructDefinition *sd = tsd->gen_struct_def(t);
        if(sd != nullptr) {
            assert(sd->type->equals(t));

            //add this type as declared
            if(!add_struct_type(sd)) {
                std::cout << "Unable to add templated type : " << t->to_string() << std::endl;
                return false;
            }

            //make sure this is well formed
            if(!sd->is_well_formed()) {
                std::cout << "Templated type is not well formed : " << t->to_string() << std::endl;
                return false;
            }

            //construct struct layout for this type
            if(!construct_struct_layout(sd->type)) {
                std::cout << "Unable to construct struct layout for templated type : " << t->to_string() << std::endl;
                return false;
            }

            return true;
        }
    }

    //couldn't generate it
    return false;
}

bool create_arraytype(ArrayType *t) {
    //have we already generated this type?
    if(is_type_declared(t)) return true;
    
    //is amt <= 0?
    if(t->amt <= 0) {
        std::cout << "Array type must have amount > 0\n";
        return false;
    }

    //generate type
    //  - default constructor
    //  - copy constructor
    //  - destructor
    //  - struct layout (this is generated lazily)
    if(!add_arraytype(t)) {
        std::cout << "add_arraytype(t) failed : " << t->to_string() << "\n";
        assert(false);
        return false;
    }

    Constructor *default_constructor = new StructConstructor(t, {}, new CompoundStatement({}));

    Identifier *xid = new Identifier("x");
    Identifier *thisid = new Identifier("this");
    Identifier *iid = new Identifier("i");

    Constructor *copy_constructor = new StructConstructor(
        t,
        {new Parameter(new ReferenceType(t->make_copy()), xid->make_copy())},
        new CompoundStatement({
            new ForStatement(
                new Declaration(primitives::i32->make_copy(), iid->make_copy(), new Expression(new ExprPrimary(new IntegerLiteral(0)))),
                new Expression(new ExprBinary(new ExprPrimary(iid->make_copy()), "<", new ExprPrimary(new IntegerLiteral(t->amt)))),
                new Expression(new ExprPostfix(new ExprPrimary(iid->make_copy()), "++")),
                new CompoundStatement({
                    new ExpressionStatement(new Expression(
                        new ExprBinary(
                            new ExprPostfix(new ExprPrimary(thisid), new Expression(new ExprPrimary(iid->make_copy()))),
                            "=",
                            new ExprPostfix(new ExprPrimary(xid), new Expression(new ExprPrimary(iid->make_copy())))
                        )
                    ))
                })
            )
        })
    );

    Destructor *destructor = new Destructor(t, new CompoundStatement({}));

    add_constructor(default_constructor);
    add_constructor(copy_constructor);
    add_destructor(destructor);

    return true;
}

bool add_function(Function *f){
    assert(f != nullptr);
    FunctionSignature *fs = f->resolve_function_signature();
    if(is_function_declared(fs)) return false;
    declared_functions.push_back(f);

    //generate function label
    if(f->is_export) function_label_map.insert({fs, f->id->name});
    else function_label_map.insert({fs, create_new_label()});
    
    std::cout << "ADD FUNCTION : " << fs->to_string() << std::endl;
    return true;
}

bool add_operator(Operator *o) {
    assert(o != nullptr);
    OperatorSignature *os = o->resolve_operator_signature();
    if(is_operator_declared(os)) return false;
    declared_operators.push_back(o);

    //if operator is an overload, generate function label
    if(dynamic_cast<OperatorOverload*>(o)) {
        operator_label_map.insert({os, create_new_label()});
    }

    std::cout << "ADD OPERATOR : " << os->to_string() << std::endl;
    return true;
}

bool add_builtin_operator(BuiltinOperator *o) {
    assert(o != nullptr);

    //just wrap this in a TemplatedOperator with no templating. 
    TemplatedOperator *to = new TemplatedOperator(new TemplateHeader({}), o);

    if(!add_templated_operator(to)) {
        assert(false);
    }

    return true;
}

bool add_constructor(Constructor *c) {
    assert(c != nullptr);
    ConstructorSignature *cs = c->resolve_constructor_signature();
    if(is_constructor_declared(cs)) return false;   
    declared_constructors.push_back(c);
    constructor_label_map.insert({cs, create_new_label()});
    std::cout << "ADD CONSTRUCTOR : " << cs->to_string() << std::endl;
    return true;
}

bool add_destructor(Destructor *d) {
    assert(d != nullptr);
    Type *t = d->type;
    if(is_destructor_declared(t)) return false;
    declared_destructors.push_back(d);
    destructor_label_map.insert({t, create_new_label()});
    std::cout << "ADD DESTRUCTOR : " << t->to_string() << std::endl;
    return true;
}

//takes whatever the current local offset is as the address
Variable* add_stack_variable(Type *t, Identifier *id) {
    return add_variable(t, id, std::to_string(local_offset) + "(%rbp)", false, false);
}

//takes the name as the address
Variable* add_global_variable(Type *t, Identifier *id, bool is_extern) {
    return add_variable(t, id, id->name + "(%rip)", true, is_extern);
}

Variable* add_variable(Type *t, Identifier *id, std::string addr_str, bool is_global, bool is_extern) {
    assert(t != nullptr && id != nullptr);
    if(is_extern) assert(is_global);                        //extern variables must be global
    if(!is_global) assert(declaration_stack.size() != 0);   //stack variables must be declared on an existing declaration stack
    if(is_variable_declared(id)) {
        std::cout << "Cannot redeclare " << t->to_string() << " " << id->name << "\n";
        return nullptr;
    }
    Variable *v = new Variable(is_global, is_extern, t, id, addr_str);
    declared_variables.push_back(v);
    if(!is_global) declaration_stack.rbegin()->push_back(v);
    else declared_global_variables.push_back(v);
    return v;
}

void remove_function(Function *f) {
    assert(f != nullptr);
    int ind = -1;
    for(int i = 0; i < declared_functions.size(); i++){
        if(*f == *(declared_functions[i])) {
            ind = i;
            break;
        }
    }
    assert(ind != -1);
    function_label_map.erase(f->resolve_function_signature());
    declared_functions.erase(declared_functions.begin() + ind);
}

void remove_variable(Identifier *id) {
    assert(id != nullptr);
    int ind = -1;
    for(int i = 0; i < declared_variables.size(); i++){
        if(*id == *(declared_variables[i]->id)) {
            ind = i;
            break;
        }
    }
    assert(ind != -1);
    declared_variables.erase(declared_variables.begin() + ind);
}

void remove_constructor(Constructor *c) {
    assert(c != nullptr);
    int ind = -1;
    for(int i = 0; i < declared_constructors.size(); i++){
        if(c->equals(declared_constructors[i])) {
            ind = i;
            break;
        }
    }
    assert(ind != -1);
    constructor_label_map.erase(c->resolve_constructor_signature());
    declared_constructors.erase(declared_constructors.begin() + ind);
}

void remove_destructor(Destructor *d) {
    assert(d != nullptr);
    int ind = -1;
    for(int i = 0; i < declared_destructors.size(); i++){
        if(d->equals(declared_destructors[i])) {
            ind = i;
            break;
        }
    }
    assert(ind != -1);
    destructor_label_map.erase(d->type);
    declared_constructors.erase(declared_constructors.begin() + ind);
}

void push_declaration_stack() {
    declaration_stack.push_back(std::vector<Variable*>(0));
}

//expects mem addr of the struct to be in %rax
void emit_destructor_call(Type *t, bool should_dealloc) {
    DestructorCall *dc = new DestructorCall(t);
    assert(dc->resolve_type() != nullptr);
    dc->emit_asm(should_dealloc);
}

//just emits frees, does not remove global variables from controller
//destruct global variables in reverse order of declaration
void emit_cleanup_global_variables() {
    for(int i = declared_global_variables.size() - 1; i >= 0; i--) {
        Variable *v = declared_global_variables[i];
        assert(v->is_global);
        Type *t = v->type;
        if(!is_type_primitive(t) && !v->is_extern) {
            //put addr to struct in %rax
            fout << indent() << "movq " << v->addr << ", %rax\n";

            //call destructor
            emit_destructor_call(t, true);
        }
    }
}

//just emits frees, should not actually affect the controller
//this should only be called outside of pop_declaration_stack() in special cases (return, break, continue)
//specifically saves registers %rax, %rcx so that this can be called inside expressions
//specifically ignores variables named in 'ignore' (used in return to let variables escape destruction if we return them)
void emit_cleanup_declaration_stack_layer(int layer_ind, std::vector<Identifier*> ignore) {
    assert(layer_ind >= 0 && layer_ind < declaration_stack.size());
    std::vector<Variable*> layer = declaration_stack[layer_ind];

    //see if there is anything to do
    std::vector<Variable*> to_clean;
    for(int i = 0; i < layer.size(); i++) {
        assert(!layer[i]->is_global);
        Type *t = layer[i]->type;
        Identifier* id = layer[i]->id;
        if(is_type_primitive(t)) continue;
        bool ignored = false;
        for(int j = 0; j < ignore.size(); j++) {
            if(ignore[j]->equals(id)) {
                ignored = true;
                break;
            }
        }
        if(ignored) {
            continue;
        }
        to_clean.push_back(layer[i]);
    }
    if(to_clean.size() == 0) {
        return;
    }
    
    //save %rax, %rcx
    emit_push("%rax", "emit_cleanup_declaration_stack_layer() : save %rax");
    emit_push("%rcx", "emit_cleanup_declaration_stack_layer() : save %rcx");

    //destruct any non-primitive variables
    assert(to_clean.size() != 0);
    for(int i = to_clean.size() - 1; i >= 0; i--){
        assert(!to_clean[i]->is_global);
        Type *t = to_clean[i]->type;
        if(!is_type_primitive(t)) {
            //put addr to struct in %rax
            fout << indent() << "movq " << to_clean[i]->addr << ", %rax\n";

            //call destructor
            emit_destructor_call(t, true);
        }
    }   

    //retrieve %rax, %rcx
    emit_pop("%rcx", "emit_cleanup_declaration_stack_layer() : save %rcx");
    emit_pop("%rax", "emit_cleanup_declaration_stack_layer() : save %rax");
}

void emit_cleanup_declaration_stack_layer(int layer_ind) {
    emit_cleanup_declaration_stack_layer(layer_ind, {});
}

//cleans up one layer of the declaration stack
//destructs any non-primitive variables
//adjusts the stack pointer
//do_free = false should only be the case from 
// - return, we can't free the return value
// - after function return, function itself is freeing function arguments
void pop_declaration_stack(bool do_free) {
    assert(declaration_stack.size() != 0);

    //retrieve layer to be removed
    std::vector<Variable*> top = *(declaration_stack.rbegin());

    //if this is not the function parameter layer, adjust %rsp
    if(declaration_stack.size() > 1) {
        //free heap allocated structs. 
        if(do_free) emit_cleanup_declaration_stack_layer(declaration_stack.size() - 1);

        //adjust %rsp
        std::vector<std::string> desc_list;
        for(int i = 0; i < top.size(); i++){
            assert(!top[i]->is_global);
            desc_list.push_back(top[i]->id->name);
        }
        emit_add_rsp(top.size() * 8, desc_list);
    }

    //unregister variables as declared
    for(int i = 0; i < top.size(); i++){
        assert(!top[i]->is_global);
        remove_variable(top[i]->id);
    }

    //remove layer
    declaration_stack.pop_back();
}

void push_loop_stack(std::string start_label, std::string assignment_label, std::string end_label) {
    assert(declaration_stack.size() != 0);
    int declaration_layer = declaration_stack.size() - 1;
    LoopContext *lc = new LoopContext(start_label, assignment_label, end_label, declaration_layer);
    loop_stack.push_back(lc);
}

void pop_loop_stack(std::string start_label, std::string assignment_label, std::string end_label) {
    assert(loop_stack.size() != 0);
    LoopContext *lc = *(loop_stack.rbegin());
    loop_stack.pop_back();
    assert(lc->start_label == start_label);
    assert(lc->assignment_label == assignment_label);
    assert(lc->end_label == end_label);
}

bool _construct_struct_layout(Type *t, std::vector<Type*> type_stack, int& byte_off) {
    assert(t != nullptr);

    // - is this type declared?
    if(!is_type_declared(t)) {
        std::cout << "Type is not declared : " << t->to_string() << "\n";
        return false;
    }

    // - have we gone too deep?
    int struct_depth_limit = 32;
    if(type_stack.size() > struct_depth_limit) {
        std::cout << "Struct member nesting too deep (depth > " << struct_depth_limit << ")\n";
        return false;
    }

    // - is this initialization infinite recursive?
    // this check really isn't necessary, but it's nice to have some extra info
    for(int i = 0; i < type_stack.size(); i++){
        if(t->equals(type_stack[i])) {
            std::cout << "Infinite recursive struct initialization : " << t->to_string() << " contains itself\n";
            return false;
        }
    }
    
    // - handle primitives seperately
    if(is_type_primitive(t)) {
        byte_off += t->calc_size();
        return true;
    }

    // - see if we've already generated this struct layout
    if(auto *sl = get_struct_layout(t)) {
        byte_off += sl->get_size();
        return true;
    }

    //get struct definition
    StructDefinition *sd = get_struct_definition(t);
    if(sd == nullptr) {
        std::cout << "Could not find struct definition : " << t->to_string() << std::endl;
        assert(false);
    }

    type_stack.push_back(t);

    std::vector<MemberVariable*> member_variables;
    std::vector<std::pair<Identifier*, int>> offset_map;
    int size = 0;
    int old_byte_off = byte_off;

    for(int i = 0; i < sd->member_variables.size(); i++){
        MemberVariable *mv = sd->member_variables[i];
        assert(mv != nullptr);

        // - we should be able to create struct layout for all member variables
        if(!_construct_struct_layout(mv->type, type_stack, byte_off)) {
            return false;
        }

        member_variables.push_back(mv->make_copy());
        offset_map.push_back(std::make_pair(mv->id->make_copy(), size));
        size += byte_off - old_byte_off;
        old_byte_off = byte_off;
    }

    type_stack.pop_back();

    StructLayout *sl = new StructLayout(member_variables, offset_map, size);
    assert(get_struct_layout(t) == nullptr);
    struct_layout_map.push_back({t, sl});
    assert(get_struct_layout(t) != nullptr);

    if(debug) {
        std::cout << "STRUCT LAYOUT : " << t->to_string() << " : " << size << "\n";
        for(int i = 0; i < offset_map.size(); i++){
            std::cout << offset_map[i].first->name << " " << offset_map[i].second << "\n";
        }
    }
    

    return true;
}

//this requires that t have a struct definition. 
bool construct_struct_layout(Type *t) {
    std::vector<Type*> type_stack;
    int byte_off = 0;
    return _construct_struct_layout(t, type_stack, byte_off);
}

StructLayout* get_struct_layout(Type *t) {
    assert(t != nullptr);
    for(int i = 0; i < struct_layout_map.size(); i++){
        if(struct_layout_map[i].first->equals(t)) {
            return struct_layout_map[i].second;
        }
    }

    //catch for array types
    if(auto atype = dynamic_cast<ArrayType*>(t)) {
        StructLayout *nsl = nullptr;
        if(is_type_primitive(atype->type)) {
            //directly generate struct layout
            nsl = new StructLayout({}, {}, atype->type->calc_size() * atype->amt);
        }
        else {
            //generate struct layout for underlying type 
            construct_struct_layout(atype->type);
            StructLayout *sl = get_struct_layout(atype->type);
            if(sl != nullptr) nsl = new StructLayout({}, {}, atype->type->calc_size() * atype->amt);
        }

        if(nsl != nullptr) {
            struct_layout_map.push_back({t, nsl});
            return nsl;
        }
    }

    return nullptr;
}

StructDefinition* get_struct_definition(Type *t) {
    assert(t != nullptr);
    for(int i = 0; i < declared_structs.size(); i++){
        if(t->equals(declared_structs[i]->type)) return declared_structs[i];
    }
    return nullptr;
}

//allocates sz_bytes memory by calling malloc. Resulting address is in %rax
void emit_malloc(int sz_bytes) {
    assert(sz_bytes >= 0);

    FunctionSignature *malloc_signature = new FunctionSignature(new Identifier("malloc"), {primitives::u64});
    std::string malloc_label = get_function_label(malloc_signature);

    fout << indent() << "mov $" << sz_bytes << ", %rax\n";
    emit_push("%rax", "emit_malloc() : malloc arg");
    fout << indent() << "call " << malloc_label << "\n";
    emit_add_rsp(8, "emit_malloc() : malloc arg");
}

//frees sz_bytes memory at address provided by %rax
//resulting free status is in %rax
void emit_free(int sz_bytes) {
    assert(sz_bytes >= 0);

    FunctionSignature *free_signature = new FunctionSignature(new Identifier("free"), {new PointerType(primitives::_void), primitives::u64});
    std::string free_label = get_function_label(free_signature);

    emit_push("%rax", "emit_free() : free addr");
    fout << indent() << "mov $" << sz_bytes << ", %rax\n";
    emit_push("%rax", "emit_free() : free sz_bytes");
    fout << indent() << "call " << free_label << "\n";
    std::vector<std::string> stk_strs = {"emit_free() : free addr", "emit_free() : free sz_bytes"};
    emit_add_rsp(16, stk_strs);
}

//expects memory address in %rax, places primitive into address, returns with memory address in %rax
//expects memory to already have been allocated
void emit_initialize_primitive(Type *t) {
    if(!is_type_primitive(t)) {
        std::cout << "Tried to initialize unrecognized primitive type : " << t->to_string() << std::endl;
        assert(false);
    }

    //every primitive should be 0 initialized
    int sz = t->calc_size();
    if(sz == 1) fout << indent() << "movb $0, (%rax)\n";
    else if(sz == 2) fout << indent() << "movw $0, (%rax)\n";
    else if(sz == 4) fout << indent() << "movl $0, (%rax)\n";
    else if(sz == 8) fout << indent() << "movq $0, (%rax)\n";
    else assert(false);
}

//expects memory address in %rax, initializes struct, returns with memory address in %rax
//expects memory to already have been allocated
void emit_initialize_array(ArrayType *t) {
    if(debug) std::cout << "EMIT INITIALIZE ARRAY : " << t->to_string() << std::endl;
    if(asm_debug) fout << indent() << "# initialize array " << t->to_string() << "\n";

    emit_push("%rax", "emit_initialize_array() :: save original %rax");

    Type *bt = t->type;
    int bt_sz = bt->calc_size();

    for(int i = 0; i < t->amt; i++){
        if(is_type_primitive(bt)) {
            emit_initialize_primitive(bt);
        }
        else {
            //invoke struct default constructor
            emit_push("%rax", "emit_initialize_array() :: save %rax before constructor call");

            //%rax already holds member struct memory address
            ConstructorCall *cc = new ConstructorCall(std::nullopt, bt, {});
            assert(cc->resolve_type()->equals(bt));
            cc->emit_asm(true);

            emit_pop("%rax", "emit_initialize_array() :: save %rax before constructor call");
        }

        //increment %rax
        fout << indent() << "add $" << bt_sz << ", %rax\n";
    }

    emit_pop("%rax", "emit_initialize_array() :: save original %rax");

    if(asm_debug) fout << indent() << "# done initialize array memory " << t->to_string() << "\n";
}

//expects memory address in %rax, initializes struct, returns with memory address in %rax
//expects memory to already have been allocated
void emit_initialize_struct(Type *t) {
    assert(t != nullptr);

    //check if we're actually trying to initialize a primitive
    if(is_type_primitive(t)) {
        emit_initialize_primitive(t);
        return;
    }

    //check if we're trying to initialize an array
    if(auto x = dynamic_cast<ArrayType*>(t)) {
        emit_initialize_array(x);
        return;
    }

    StructLayout *sl = get_struct_layout(t);
    assert(sl != nullptr);

    if(asm_debug) fout << indent() << "# initialize struct " << t->to_string() << "\n";
    
    emit_push("%rax", "emit_initialize_struct() :: save original %rax");

    for(int i = 0; i < sl->member_variables.size(); i++){
        MemberVariable *mv = sl->member_variables[i];
        int mv_size = mv->type->calc_size();
        if(is_type_primitive(mv->type)) {
            emit_initialize_primitive(mv->type);
        }
        else {
            //invoke struct default constructor
            emit_push("%rax", "emit_initialize_struct() :: save %rax before constructor call");

            //%rax already holds member struct memory address
            ConstructorCall *cc = new ConstructorCall(std::nullopt, mv->type->make_copy(), {});
            assert(cc->resolve_type()->equals(mv->type));
            cc->emit_asm(true);

            emit_pop("%rax", "emit_initialize_struct() :: save %rax before constructor call");
        }
        
        //increment %rax
        fout << indent() << "add $" << mv_size << ", %rax\n";
    }

    emit_pop("%rax", "emit_initialize_struct() :: save original %rax");

    if(asm_debug) fout << indent() << "# done initialize struct memory " << t->to_string() << "\n";
}

//allocates the next stack slot and initializes the variable into it. 
Variable* emit_initialize_stack_variable(Type *vt, Identifier *id, std::optional<Expression*> expr) {
    //claim next stack slot
    emit_sub_rsp(8, id->name);
    std::string addr_str = std::to_string(local_offset) + "(%rbp)";

    //try to initialize variable
    Variable *v = emit_initialize_variable(vt, id, expr, addr_str, false, false);

    return v;
}

//initializes a global variable. 
Variable* emit_initialize_global_variable(Type *vt, Identifier *id, std::optional<Expression*> expr, bool is_extern) {
    std::string addr_str = id->name + "(%rip)";
    return emit_initialize_variable(vt, id, expr, addr_str, true, is_extern);
}

//should be logically similar to is_declarable(), except this one emits a variable declaration 
//every variable declaration must use this except for registering parameters at the beginning of function calls
//evaluates the expression and initializes the variable onto the top of the stack
//if for some reason is unable to initialize the variable, returns nullptr
//expects the address at addr_str to be already allocated
Variable* emit_initialize_variable(Type *vt, Identifier *id, std::optional<Expression*> expr, std::string addr_str, bool is_global, bool is_extern) { 
    assert(vt != nullptr);
    assert(id != nullptr);
    if(expr.has_value()) assert(expr.value() != nullptr);

    if(debug) std::cout << "Initialize variable : " << vt->to_string() << " " << id->name << std::endl;

    // - make sure vt is declared
    if(!is_type_declared(vt)) {
        std::cout << "Type " << vt->to_string() << " is not declared\n";
        return nullptr;
    }
    // - make sure vt is not void
    if(vt->equals(primitives::_void)) {
        std::cout << "Cannot initialize a variable of type void\n";
        return nullptr;
    }
    // - is the variable already declared?
    if(is_variable_declared(id)) {
        std::cout << "Cannot redeclare variable : " << vt->to_string() << " " << id->name << "\n";
        return nullptr;
    }
    // - does the expression resolve to a type?
    if(expr.has_value() && expr.value()->resolve_type() == nullptr){
        std::cout << "Expression does not resolve to type\n";
        return nullptr;
    }

    Variable *v = add_variable(vt, id, addr_str, is_global, is_extern);
    assert(v != nullptr);

    if(dynamic_cast<ReferenceType*>(vt) != nullptr) {
        vt = dynamic_cast<ReferenceType*>(vt)->type;
        // - extern variables cannot be of reference type
        if(is_extern) {
            std::cout << "Extern variable cannot be reference type\n";
            return nullptr;
        }
        // - must assign a value when initializing references
        if(!expr.has_value()) {
            std::cout << "Cannot default initialize reference\n";
            return nullptr;
        }
        // - must use l-value when binding references
        if(!expr.value()->is_lvalue()) {
            std::cout << "Cannot assign r-value to reference\n";
            return nullptr;
        }
        // - must bind to reference something of matching type (no conversions) 
        if(*vt != *(expr.value()->resolve_type())) {
            std::cout << "Cannot assign non-matching type to reference\n";
            return nullptr;
        }

        //zero initialize primitive
        fout << indent() << "movq $0, " << addr_str << "\n";

        //evaluate expr
        //%rax = value, %rcx = addr
        expr.value()->emit_asm();

        //save addr into given addr
        fout << indent() << "movq %rcx, " << addr_str << "\n";
    }
    else if(is_type_primitive(vt)) {
        if(!is_extern) {
            //zero initialize variable
            fout << indent() << "movq $0, " << addr_str << "\n";
        }
        if(expr.has_value()) {
            //assign the variable to the value
            Expression *a_expr = new Expression(new ExprBinary(new ExprPrimary(id), "=", expr.value()->expr_node));
            if(a_expr->resolve_type() == nullptr) {
                std::cout << "Cannot assign expression into variable type : " << vt->to_string() << ", " << expr.value()->to_string() << "\n";
                return nullptr;
            }
            a_expr->emit_asm();
        }
    }
    else {
        // - don't call default constructor, then immediately call copy constructor. Should only call one constructor
        // - don't call copy constructor when initializing a variable using a r-value
        //   the variable should instead just become the r-value
        //   make sure to not auto-dealloc the r-value after emitting the expression
        // - if const gets implemented, don't have to copy l-values into const variables, can just pass a reference

        if(expr.has_value()) {
            //if the expression type is the same as the variable type, we can directly assign
            //otherwise we have to create a subexpression to do overload resolution
            Type* et = expr.value()->resolve_type();
            assert(et != nullptr);

            if(!et->equals(vt)) {
                //default construct variable
                ConstructorCall *cc = new ConstructorCall(std::nullopt, vt, {});
                assert(cc->resolve_type()->equals(vt));
                cc->emit_asm(false);
                fout << indent() << "movq %rax, " << addr_str << "\n";

                //evaluate assignment expression
                Expression *a_expr = new Expression(new ExprBinary(new ExprPrimary(id), "=", expr.value()->expr_node));
                if(a_expr->resolve_type() == nullptr) {
                    std::cout << "Cannot assign expression into variable type : " << vt->to_string() << ", " << expr.value()->to_string() << "\n";
                    return nullptr;
                }
                a_expr->emit_asm();
            }
            else if(expr.value()->is_lvalue()) {
                //evaluate expression, make copy
                ConstructorCall *cc = new ConstructorCall(std::nullopt, vt->make_copy(), {expr.value()->make_copy()});
                assert(cc->resolve_type()->equals(vt));
                cc->emit_asm(false);

                //move copy into variable
                fout << indent() << "movq %rax, " << addr_str << "\n";
            }
            else {
                //evaluate expression
                expr.value()->emit_asm(false);  //don't dealloc r-value struct
                
                //move directly into variable
                fout << indent() << "movq %rax, " << addr_str << "\n";
            }
        }
        else if(!is_extern) {
            //default construct variable
            ConstructorCall *cc = new ConstructorCall(std::nullopt, vt, {});
            assert(cc->resolve_type()->equals(vt));
            cc->emit_asm(false);

            //save pointer to addr
            fout << indent() << "movq %rax, " << addr_str << "\n";
        }
    }

    return v;
}

//assumes reference type is in %rax
//returns with value in %rax, address in %rcx
void emit_dereference(Type *rt) {
    assert(rt != nullptr);
    Type *t = nullptr;
    if(auto _t = dynamic_cast<ReferenceType*>(rt)) {
        t = _t->type;
    }
    else if(auto _t = dynamic_cast<PointerType*>(rt)) {
        t = _t->type;
    }
    else {
        std::cout << "Can only emit dereference for pointer or reference type" << std::endl;
        assert(false);
    }
    
    assert(is_type_declared(t));
    if(asm_debug) fout << indent() << "# dereferencing to type " << t->to_string() << "\n";
    fout << indent() << "mov %rax, %rcx\n";
    if(is_type_primitive(t)) {
        //we should only load stuff if the base type is a primitive. 
        int sz = t->calc_size();
        emit_mem_retrieve(sz); 
    }
}

bool add_string_literal(std::string str) {
    if(string_literal_label_map.count(str)) return true;
    string_literal_label_map[str] = create_new_label();
    return true;
}

std::string get_string_literal_label(std::string str) {
    assert(string_literal_label_map.count(str));
    return string_literal_label_map[str];
}

void emit_data_section() {
    fout << ".section .data\n";
    //alloc space for global variables
    for(int i = 0; i < declared_global_variables.size(); i++){
        Variable* v = declared_global_variables[i];
        assert(v->is_global);
        std::string var_addr = v->addr;
        assert(var_addr.size() >= 6);
        std::string suf = var_addr.substr(var_addr.size() - 6);
        assert(suf == "(%rip)");
        std::string label = var_addr.substr(0, var_addr.size() - 6);
        assert(label.size() > 0);

        //if it's extern, it should already be declared somewhere
        if(!v->is_extern) {
            fout << label << ": .quad 0";
            if(asm_debug) fout << " # " << v->type->to_string() << " " << v->id->name;
            fout << "\n";
        }
    }
    fout << "\n";

    fout << ".section .rodata\n";
    //alloc string literals
    for(auto i = string_literal_label_map.begin(); i != string_literal_label_map.end(); i++) {
        std::string val = i->first;
        std::string label = i->second;
        fout << label << ": .string \"" << val << "\"\n";
    }
    fout << "\n";
}


std::vector<Type*> convert_type_list(parser::type_list *t) {
    std::vector<Type*> arr;
    if(t->t0.has_value()) {
        arr.push_back(Type::convert(t->t0.value()->t0));
        for(int i = 0; i < t->t0.value()->t1.size(); i++){
            arr.push_back(Type::convert(t->t0.value()->t1[i]->t3));
        }
    }
    return arr;
}

std::vector<Parameter*> convert_parameter_list(parser::parameter_list *t) {
    std::vector<Parameter*> arr;
    if(t->t0.has_value()) {
        arr.push_back(Parameter::convert(t->t0.value()->t0));
        for(int i = 0; i < t->t0.value()->t1.size(); i++){
            arr.push_back(Parameter::convert(t->t0.value()->t1[i]->t3));
        }
    }
    return arr;
}

std::vector<Expression*> convert_argument_list(parser::argument_list *t) {
    std::vector<Expression*> arr;
    if(t->t0.has_value()) {
        arr.push_back(Expression::convert(t->t0.value()->t0));
        for(int i = 0; i < t->t0.value()->t1.size(); i++){
            arr.push_back(Expression::convert(t->t0.value()->t1[i]->t3));
        }
    }
    return arr;
}

std::vector<Identifier*> convert_identifier_list(parser::identifier_list *t) {
    std::vector<Identifier*> arr;
    if(t->t0.has_value()) {
        arr.push_back(Identifier::convert(t->t0.value()->t0));
        for(int i = 0; i < t->t0.value()->t1.size(); i++){
            arr.push_back(Identifier::convert(t->t0.value()->t1[i]->t3));
        }
    }
    return arr;
}