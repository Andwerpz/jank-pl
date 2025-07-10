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
#include "Overload.h"
#include "TemplatedOverload.h"
#include "StructLayout.h"
#include "primitives.h"
#include "Destructor.h"
#include "DestructorCall.h"

Variable::Variable(Type *_type, Identifier *_id) {
    id = _id;
    type = _type;
}

LoopContext::LoopContext(std::string _start_label, std::string _assignment_label, std::string _end_label, int _declaration_layer) {
    start_label = _start_label;
    assignment_label = _assignment_label;
    end_label = _end_label;
    declaration_layer = _declaration_layer;
}

OperatorSignature::OperatorSignature(Type *_left, std::string _op, Type *_right) {
    assert(_left != nullptr);
    assert(_right != nullptr);
    left = _left;
    op = _op;
    right = _right;
}

OperatorSignature::OperatorSignature(std::string _op, Type *_right) {
    assert(_right != nullptr);
    left = std::nullopt;
    op = _op;
    right = _right;
}

OperatorSignature::OperatorSignature(Type *_left, std::string _op) {
    assert(_left != nullptr);
    left = _left;
    op = _op;
    right = std::nullopt;
}

OperatorSignature::OperatorSignature(Type *from, Type *to) {
    left = from;
    op = "$";
    right = to;
}

size_t OperatorSignature::hash() const {
    size_t hash = 0;
    if(left.has_value()) hash_combine(hash, left.value()->hash());
    else hash_combine(hash, 0);
    hash_combine(hash, std::hash<std::string>()(op));
    if(right.has_value()) hash_combine(hash, right.value()->hash());
    else hash_combine(hash, 0);
    return hash;
}

bool OperatorSignature::equals(const OperatorSignature* other) const {
    if(left.has_value() != other->left.has_value()) return false;
    if(left.has_value() && *(left.value()) != *(other->left.value())) return false;
    if(right.has_value() != other->right.has_value()) return false;
    if(right.has_value() && *(right.value()) != *(other->right.value())) return false;
    return op == other->op;
}

bool OperatorSignature::operator==(const OperatorSignature& other) const {
    return this->equals(&other);
}

bool OperatorSignature::operator!=(const OperatorSignature& other) const {
    return !this->equals(&other);
}

std::string OperatorSignature::to_string() {
    std::string res = "";
    if(left.has_value()) res += left.value()->to_string() + " ";
    res += op;
    if(right.has_value()) res += " " + right.value()->to_string();
    return res;
}

OperatorImplementation::OperatorImplementation(Type *_res_type) {
    assert(_res_type != nullptr);
    res_type = _res_type;
}

Type* OperatorImplementation::get_res_type() {
    return this->res_type;
}

BuiltinOperator::BuiltinOperator(Type *_res_type, std::vector<std::string> _instructions) : OperatorImplementation(_res_type) {
    instructions = _instructions;
}

void BuiltinOperator::emit_asm() {
    for(int i = 0; i < instructions.size(); i++){
        fout << indent() << instructions[i] << "\n";
    }
}

OverloadedOperator::OverloadedOperator(Overload *_overload) : OperatorImplementation((assert(_overload != nullptr), _overload->type)) {
    overload = _overload;
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
std::vector<Function*> declared_sys_functions;
std::vector<TemplatedStructDefinition*> declared_templated_structs;
std::vector<TemplatedFunction*> declared_templated_functions;
std::vector<TemplatedOverload*> declared_templated_overloads;
std::vector<BaseType*> declared_basetypes;
std::unordered_set<Type*, TypeHash, TypeEquals> primitive_base_types;
std::vector<std::pair<Type*, StructLayout*>> struct_layout_map;
std::unordered_map<FunctionSignature*, std::string, FunctionSignatureHash, FunctionSignatureEquals> function_label_map;
std::unordered_map<ConstructorSignature*, std::string, ConstructorSignatureHash, ConstructorSignatureEquals> constructor_label_map;
std::unordered_map<Type*, std::string, TypeHash, TypeEquals> destructor_label_map;
std::unordered_map<OperatorSignature*, std::string, OperatorSignatureHash, OperatorSignatureEquals> overload_label_map;
std::unordered_map<OperatorSignature*, OperatorImplementation*, OperatorSignatureHash, OperatorSignatureEquals> conversion_map;  

int label_counter;
int tmp_variable_counter;

void reset_controller() {
    enclosing_function = nullptr;
    enclosing_program = nullptr;
    declared_types.clear();
    declared_basetypes.clear();
    declared_structs.clear();
    declared_overloads.clear();
    declared_functions.clear();
    declared_templated_structs.clear();
    declared_templated_functions.clear();
    declared_templated_overloads.clear();
    declared_sys_functions.clear();
    primitive_base_types.clear();
    struct_layout_map.clear();

    declared_functions.clear();
    function_label_map.clear();

    declared_constructors.clear();
    constructor_label_map.clear();

    declared_destructors.clear();
    destructor_label_map.clear();
    
    declared_variables.clear();
    while(declaration_stack.size()) declaration_stack.pop_back();
    conversion_map.clear();

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
    if(primitive_base_types.count(t)) return true;      //is it explicitly mentioned?
    if(dynamic_cast<PointerType*>(t)) return true;      //is it a pointer?
    if(dynamic_cast<ReferenceType*>(t)) return true;    //is it a reference?
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
    if(sz == 1) fout << indent() << "movzbl (%rax), %al\n";
    else if(sz == 2) fout << indent() << "movzwl (%rax), %ax\n";
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

        // otherwise, can look for type conversions
        Expression *a_expr = new Expression(new ExprBinary(new ExprPrimary(A), "=", new ExprPrimary(expr)));
        if(a_expr->resolve_type() != nullptr) {
            ans = true;
            goto done;
        }
        ans = false;
        goto done;
    }
    done: {}

    is_declarable_cache[pkey] = ans;
    return ans;
}

//requires that the types exactly match
OperatorImplementation* find_typecast_implementation(Type *from, Type *to) {
    //special cases
    // - from and to are the same type
    if(*from == *to) {
        return new BuiltinOperator(to, {});     //do nothing
    }
    Type* voidptr_t = new PointerType(primitives::_void->make_copy());
    // - from is a pointer, to is void*
    if(dynamic_cast<PointerType*>(from) != nullptr && *to == *voidptr_t) {
        return new BuiltinOperator(to, {});     //do nothing
    }
    // - from is void*, to is a pointer
    if(*from == *voidptr_t && dynamic_cast<PointerType*>(to) != nullptr) {
        return new BuiltinOperator(to, {});     //do nothing
    }
    // - from is a pointer, to is a pointer
    if(dynamic_cast<PointerType*>(from) != nullptr && dynamic_cast<PointerType*>(to) != nullptr) {
        return new BuiltinOperator(to, {});     //do nothing
    }
    // - from is a pointer, to is an u64
    if(dynamic_cast<PointerType*>(from) != nullptr && to->equals(primitives::u64)) {
        return new BuiltinOperator(to, {});     //do nothing
    }
    // - from is an u64, to is a pointer
    if(from->equals(primitives::u64) && dynamic_cast<PointerType*>(to) != nullptr) {
        return new BuiltinOperator(to, {});     //do nothing
    }

    //look through all conversions to see if we have an exact match
    std::vector<OperatorImplementation*> viable;
    OperatorSignature *key = new OperatorSignature(from, to);
    for(auto i = conversion_map.begin(); i != conversion_map.end(); i++){
        OperatorSignature *nkey = i->first;
        if(*key == *nkey) viable.push_back(i->second);
    }
    if(viable.size() != 1) return nullptr;
    return viable[0];
}

//looks through all operator implementations and finds all that matches
//same idea as get_function() for all conversions except for direct casting. 
OperatorImplementation* find_operator_implementation(std::optional<Expression*> left, std::string op, std::optional<Expression*> right) {
    //some sanity checks
    //one of left or right has to have a value
    assert(left.has_value() || right.has_value());
    //check that the values are not nullptr
    if(left.has_value()) assert(left.value() != nullptr);
    if(right.has_value()) assert(right.value() != nullptr);
    //this should not be the casting operator
    assert(op != "$");

    //try to construct templated overload
    for(int i = 0; i < declared_templated_overloads.size(); i++){
        Overload *no = declared_templated_overloads[i]->gen_overload(left, op, right);
        if(no == nullptr) {
            continue;
        }
        add_operator_implementation(no);
    }

    std::vector<OperatorImplementation*> viable;
    for(auto i = conversion_map.begin(); i != conversion_map.end(); i++){
        OperatorSignature *nkey = i->first;
        //does the operator match
        if(op != nkey->op) continue;
        //is the left argument convertible
        if(left.has_value() != nkey->left.has_value()) continue;
        if(left.has_value() && !is_declarable(nkey->left.value(), left.value())) continue;
        //is the right argument convertible
        if(right.has_value() != nkey->right.has_value()) continue;
        if(right.has_value() && !is_declarable(nkey->right.value(), right.value())) continue;
        viable.push_back(i->second);
    }
    if(viable.size() != 1) {
        return nullptr;
    }
    return viable[0];
}

OperatorImplementation* find_operator_implementation(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right) {
    if(left.has_value()) assert(left.value() != nullptr);
    if(right.has_value()) assert(right.value() != nullptr);
    std::optional<Expression*> _left = std::nullopt;
    if(left.has_value()) _left = new Expression(left.value());
    std::optional<Expression*> _right = std::nullopt;
    if(right.has_value()) _right = new Expression(right.value());
    return find_operator_implementation(_left, op, _right);
}

Type* find_resulting_type(std::optional<Expression*> left, std::string op, std::optional<Expression*> right) {
    OperatorImplementation *oe = find_operator_implementation(left, op, right);
    if(oe == nullptr) return nullptr;
    return oe->res_type;
}

Type* find_resulting_type(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right) {
    OperatorImplementation *oe = find_operator_implementation(left, op, right);
    if(oe == nullptr) return nullptr;
    return oe->res_type;
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
        if(dynamic_cast<ReferenceType*>(nt)) return false;
        while(dynamic_cast<PointerType*>(nt)) {
            nt = dynamic_cast<PointerType*>(nt)->type;
        }
        if(auto x = dynamic_cast<BaseType*>(nt)) {
            if(!is_basetype_declared(x)) return false;
        }
        else if(auto x = dynamic_cast<TemplatedType*>(nt)) {
            if(!is_templated_type_well_formed(x)) return false;
        }
        else assert(false);
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

bool is_sys_function(FunctionSignature *fs) {
    for(int i = 0; i < declared_sys_functions.size(); i++){
        if(fs->equals(declared_sys_functions[i]->resolve_function_signature())) {
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

//finds all viable functions with parameter conversions
// - If there is exactly one, returns that one
// - If there are multiple or zero, returns nullptr
Function* get_called_function(FunctionCall *fc) {
    assert(fc != nullptr);

    //see if we can make some templated function out of fc
    //TODO check if some function call is ambiguous due to templating. 
    for(int i = 0; i < declared_templated_functions.size(); i++){
        Function* nf = declared_templated_functions[i]->gen_function(fc);
        if(nf == nullptr) {
            continue;
        }

        //ok, we've generated a new function. Add it
        if(!add_function(nf)) {
            //duplicate function signature is bad?
            // std::cout << "Generated duplicate function signature from templated function : " << nf->resolve_function_signature()->to_string() << "\n";
            // return nullptr;
        }
    }

    std::optional<Type*> enclosing_type = fc->target_type;
    Identifier *id = fc->id;
    std::vector<Expression*> args = fc->argument_list;
    if(enclosing_type.has_value()) assert(enclosing_type.value() != nullptr);
    assert(id != nullptr);

    std::vector<Function*> viable;
    for(int i = 0; i < declared_functions.size(); i++){
        FunctionSignature *nfs = declared_functions[i]->resolve_function_signature();

        // - do the identifiers match?
        if(*id != *(nfs->id)) {
            continue;
        }
        // - do the argument counts match?
        if(args.size() != nfs->input_types.size()) {
            continue;
        }
        // - does enclosing_type match? (must match exactly)
        if(enclosing_type.has_value() != nfs->enclosing_type.has_value()) {
            continue;
        }
        if(enclosing_type.has_value() && *(enclosing_type.value()) != *(nfs->enclosing_type.value())) {
            continue;
        }

        // - can all the arguments somehow be converted into nfs?
        bool is_viable = true;
        for(int j = 0; j < args.size(); j++) {
            Type *nt = nfs->input_types[j];
            if(!is_declarable(nt, args[j])) {
                is_viable = false;
            }
        }
        if(!is_viable) continue;

        //all checks passed
        viable.push_back(declared_functions[i]);
    }

    if(viable.size() == 0) {
        std::cout << "No matching function for signature : " << fc->to_string() << "\n";
        return nullptr;
    }
    else if(viable.size() > 1) {
        std::cout << "Ambiguous function call : " << fc->to_string() << "\n";
        return nullptr;
    }
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
    assert(function_label_map.count(fs));
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

std::string get_overload_label(OperatorSignature *os) {
    assert(os != nullptr);
    assert(overload_label_map.count(os));
    return overload_label_map[os];
}

Variable* get_variable(Identifier *id) {
    for(int i = 0; i < declared_variables.size(); i++){
        if(*id == *(declared_variables[i]->id)) {
            return declared_variables[i];
        }
    }
    return nullptr;
}

bool add_struct_type(StructDefinition *sd) {
    assert(sd != nullptr);
    Type *t = sd->type;
    if(is_type_declared(t)) return false;
    declared_types.push_back(t);
    declared_structs.push_back(sd);

    //add all functions and constructors
    for(int i = 0; i < sd->functions.size(); i++){
        Function *f = sd->functions[i];
        if(!add_function(f)) {
            std::cout << "Failed to add struct member function : " << f->resolve_function_signature()->to_string() << "\n";
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
    declared_basetypes.push_back(t);
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
    declared_templated_functions.push_back(f);
    std::cout << "ADD TEMPLATED FUNCTION : " << f->function->resolve_function_signature()->to_string() << "\n";
    return true;
}

bool add_templated_overload(TemplatedOverload *o) {
    assert(o != nullptr);
    declared_templated_overloads.push_back(o);
    std::cout << "ADD TEMPLATED OVERLOAD : " << o->overload->resolve_operator_signature()->to_string() << "\n";
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

bool add_function(Function *f){
    assert(f != nullptr);
    FunctionSignature *fs = f->resolve_function_signature();
    if(is_function_declared(fs)) return false;
    declared_functions.push_back(f);
    function_label_map.insert({fs, create_new_label()});
    std::cout << "ADD FUNCTION : " << fs->to_string() << std::endl;
    return true;
}

bool add_sys_function(Function *f) {
    assert(f != nullptr);
    FunctionSignature *fs = f->resolve_function_signature();
    if(is_function_declared(fs)) assert(false);
    declared_functions.push_back(f);
    declared_sys_functions.push_back(f);
    function_label_map.insert({fs, f->id->name});
    return true;
}

bool add_constructor(Constructor *c) {
    assert(c != nullptr);
    ConstructorSignature *cs = c->resolve_constructor_signature();
    if(is_constructor_declared(cs)) assert(false);
    declared_constructors.push_back(c);
    constructor_label_map.insert({cs, create_new_label()});
    std::cout << "ADD CONSTRUCTOR : " << cs->to_string() << " " << declared_constructors.size() << std::endl;
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

Variable* add_variable(Type *t, Identifier *id, bool is_global) {
    assert(t != nullptr && id != nullptr);
    if(!is_global) assert(declaration_stack.size() != 0);
    if(is_variable_declared(id)) {
        std::cout << "Cannot redeclare " << t->to_string() << " " << id->name << "\n";
        return nullptr;
    }
    Variable *v = new Variable(t, id);
    declared_variables.push_back(v);
    if(!is_global) declaration_stack.rbegin()->push_back(v);
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

//if do_free == true, also frees allocated structs on this layer
//just emits frees, should not actually affect the controller
//this should only be called outside of pop_declaration_stack() in return, break, and continue. 
//specifically saves registers %rax, %rcx
void emit_cleanup_declaration_stack_layer(int layer_ind) {
    assert(layer_ind > 0 && layer_ind < declaration_stack.size());
    std::vector<Variable*> layer = declaration_stack[layer_ind];
    
    //save %rax, %rcx
    emit_push("%rax", "emit_cleanup_declaration_stack_layer() : save %rax");
    emit_push("%rcx", "emit_cleanup_declaration_stack_layer() : save %rcx");

    //destruct any non-primitive variables
    for(int i = 0; i < layer.size(); i++){
        Type *t = layer[i]->type;
        if(!is_type_primitive(t)) {
            //put addr to struct in %rax
            fout << indent() << "movq " << layer[i]->addr << ", %rax\n";

            //call destructor
            emit_destructor_call(t, true);
        }
    }   

    //retrieve %rax, %rcx
    emit_pop("%rcx", "emit_cleanup_declaration_stack_layer() : save %rcx");
    emit_pop("%rax", "emit_cleanup_declaration_stack_layer() : save %rax");
}

//cleans up one layer of the declaration stack
//destructs any non-primitive variables
//do_free = false should only be the case from return. (we can't free the return value)
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
            desc_list.push_back(top[i]->id->name);
        }
        emit_add_rsp(top.size() * 8, desc_list);
    }

    //unregister variables as declared
    for(int i = 0; i < top.size(); i++){
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
    assert(sd != nullptr);

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

    std::cout << "STRUCT LAYOUT : " << t->to_string() << " : " << size << "\n";
    for(int i = 0; i < offset_map.size(); i++){
        std::cout << offset_map[i].first->name << " " << offset_map[i].second << "\n";
    }

    return true;
}

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
void emit_initialize_struct(Type *t) {
    assert(t != nullptr);

    //check if we're actually trying to initialize a primitive
    if(is_type_primitive(t)) {
        emit_initialize_primitive(t);
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
            ConstructorCall *cc = new ConstructorCall(mv->type->make_copy(), {});
            assert(cc->resolve_type()->equals(mv->type));
            cc->emit_asm(false);

            emit_pop("%rax", "emit_initialize_struct() :: save %rax before constructor call");
        }
        
        //increment %rax
        fout << indent() << "add $" << mv_size << ", %rax\n";
    }

    emit_pop("%rax", "emit_initialize_struct() :: save original %rax");

    if(asm_debug) fout << indent() << "# done initialize struct memory " << t->to_string() << "\n";
}

//allocates the next stack slot and initializes the variable into it. 
Variable* emit_initialize_stack_variable(Type *vt, Identifier *id, Expression *expr) {
    //claim next stack slot
    emit_sub_rsp(8, id->name);
    std::string addr_str = std::to_string(local_offset) + "(%rbp)";

    //try to initialize variable
    Variable *v = emit_initialize_variable(vt, id, expr, addr_str);

    return v;
}

//should be logically similar to is_declarable(), except this one emits a variable declaration 
//every variable declaration must use this except for registering parameters at the beginning of function calls
//evaluates the expression and initializes the variable onto the top of the stack
//if for some reason is unable to initialize the variable, returns nullptr
//expects the address at addr_str to be already allocated
Variable* emit_initialize_variable(Type *vt, Identifier *id, Expression *expr, std::string addr_str, bool is_global) { 
    assert(vt != nullptr);
    assert(id != nullptr);
    assert(expr != nullptr);    //might want to later have a version that default declares types

    std::cout << "Initialize variable : " << vt->to_string() << " " << id->name << std::endl;

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
    if(expr->resolve_type() == nullptr){
        std::cout << "Expression does not resolve to type\n";
        return nullptr;
    }

    Type *et = expr->resolve_type();
    bool is_lvalue = expr->is_lvalue();
    Variable *v = add_variable(vt, id, is_global);
    assert(v != nullptr);
    v->addr = addr_str;

    if(dynamic_cast<ReferenceType*>(vt) != nullptr) {
        vt = dynamic_cast<ReferenceType*>(vt)->type;
        // - must use l-value when binding references
        if(!is_lvalue) {
            std::cout << "Cannot assign r-value to reference\n";
            return nullptr;
        }
        // - must bind to reference something of matching type (no conversions) 
        if(*vt != *et) {
            std::cout << "Cannot assign non-matching type to reference\n";
            return nullptr;
        }

        //evaluate expr
        //%rax = value, %rcx = addr
        expr->emit_asm();

        //save addr into given addr
        fout << indent() << "movq %rcx, " << addr_str << "\n";
    }
    else {
        //split declaration into the 'declaration' and 'assignment'
        //int a = b;
        //will turn into
        //int a;
        //a = b;
        //this is so that I don't have to rewrite all that assignment logic

        //'declaration'
        if(is_type_primitive(vt)) {
            //just 0 initialize the primitive
            fout << indent() << "movq $0, " << addr_str << "\n";
        }
        else {
            //call default constructor 
            ConstructorCall *cc = new ConstructorCall(vt, {});
            assert(cc->resolve_type()->equals(vt));
            cc->emit_asm(true);

            //save pointer to addr
            fout << indent() << "movq %rax, " << addr_str << "\n";
        }

        //'assignment'
        //right now, expr = b. We want expr = (a = b)
        Expression *a_expr = new Expression(new ExprBinary(new ExprPrimary(id), "=", expr->expr_node));
        if(a_expr->resolve_type() == nullptr) {
            std::cout << "Cannot cast expression type into variable type\n";
            return nullptr;
        }
        a_expr->emit_asm();
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

bool add_operator_implementation(OperatorSignature *os, OperatorImplementation *oi){
    if(conversion_map.count(os)) {
        return false;
    }
    conversion_map[os] = oi;
    return true;
}

bool add_operator_implementation(Overload *o){
    assert(o != nullptr);
    OperatorSignature *os = o->resolve_operator_signature();
    if(os == nullptr) return false;
    OverloadedOperator *oo = new OverloadedOperator(o);
    if(!add_operator_implementation(os, oo)) return false;
    declared_overloads.push_back(o);
    overload_label_map.insert({os, create_new_label()});
    std::cout << "ADD OVERLOAD : " << o->resolve_operator_signature()->to_string() << std::endl;
    return true;
}

void remove_operator_implementation(OperatorSignature *os, OperatorImplementation *oi) {
    assert(conversion_map.count(os));
    conversion_map.erase(os);
}

void remove_operator_implementation(Overload *o) {
    assert(o != nullptr);
    OperatorSignature *os = o->resolve_operator_signature();
    assert(os != nullptr);
    OverloadedOperator *oo = new OverloadedOperator(o);
    remove_operator_implementation(os, oo);
    {
        int ind = -1;
        for(int i = 0; i < declared_overloads.size(); i++){
            if(o->resolve_operator_signature()->equals(declared_overloads[i]->resolve_operator_signature())) {
                ind = i;
            }
        }   
        assert(ind != -1);
        declared_overloads.erase(declared_overloads.begin() + ind);
    }
    overload_label_map.erase(os);
}