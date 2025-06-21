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

Variable::Variable(Type *_type, Identifier *_id) {
    id = _id;
    type = _type;
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
    op = "(cast)";
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

FunctionOperator::FunctionOperator(OperatorOverload *_function) : OperatorImplementation((assert(_function != nullptr), _function->type)) {
    function = _function;
}

StructLayout::StructLayout(std::vector<MemberVariable*> _member_variables, std::unordered_map<std::string, int> _offset_map, int _size) {
    member_variables = _member_variables;
    offset_map = _offset_map;
    size = _size;
    assert(member_variables.size() == offset_map.size());
}

int StructLayout::get_offset(Identifier *id) {
    assert(offset_map.count(id->name));
    return offset_map[id->name];
}

Type* StructLayout::get_type(Identifier *id) {
    for(int i = 0; i < member_variables.size(); i++){
        if(*id == *(member_variables[i]->id)) {
            return member_variables[i]->type;
        }
    }
    return nullptr;
}

int StructLayout::get_size() {
    return size;
}

int StructLayout::get_ptr_offset() {
    return size;
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
std::unordered_set<Type*, TypeHash, TypeEquals> primitive_base_types;
std::unordered_map<Type*, StructLayout*, TypeHash, TypeEquals> struct_layout_map;
std::unordered_map<FunctionSignature*, std::string, FunctionSignatureHash, FunctionSignatureEquals> function_label_map;
std::unordered_map<ConstructorSignature*, std::string, ConstructorSignatureHash, ConstructorSignatureEquals> constructor_label_map;
std::unordered_map<OperatorSignature*, OperatorImplementation*, OperatorSignatureHash, OperatorSignatureEquals> conversion_map;  

int label_counter;
int tmp_variable_counter;

void reset_controller() {
    // - reset semantic controller
    enclosing_function = nullptr;
    declared_types.clear();
    primitive_base_types.clear();
    struct_layout_map.clear();

    declared_functions.clear();
    function_label_map.clear();

    declared_constructors.clear();
    constructor_label_map.clear();
    
    declared_variables.clear();
    while(declaration_stack.size()) declaration_stack.pop();
    conversion_map.clear();

    local_offset = 0;
    stack_desc.clear();

    label_counter = 0;
    tmp_variable_counter = 0;

    // - primitive types
    Type *p_int = new BaseType("int");
    Type *p_char = new BaseType("char");
    Type *p_void = new BaseType("void");
    add_primitive_type(p_int);
    add_primitive_type(p_char);
    add_primitive_type(p_void);

    // - sys functions
    add_sys_function(new Function(p_void, new Identifier("sys_exit"), {p_int}));

    add_sys_function(new Function(new PointerType(p_void), new Identifier("malloc"), {p_int}));
    add_sys_function(new Function(new PointerType(p_char), new Identifier("int_to_string"), {p_int}));
    add_sys_function(new Function(p_void, new Identifier("puts"), {new PointerType(p_char)}));
    add_sys_function(new Function(p_void, new Identifier("puts_endl"), {new PointerType(p_char)}));
    add_sys_function(new Function(p_void, new Identifier("puti"), {p_int}));
    add_sys_function(new Function(p_void, new Identifier("puti_endl"), {p_int}));

    // - populate conversion map
    conversion_map[new OperatorSignature("+", p_int)] = new BuiltinOperator(p_int, {});
    conversion_map[new OperatorSignature("-", p_int)] = new BuiltinOperator(p_int, {"neg %rax"});
    conversion_map[new OperatorSignature("~", p_int)] = new BuiltinOperator(p_int, {"not %rax"});
    conversion_map[new OperatorSignature("!", p_int)] = new BuiltinOperator(p_int, {
        "test %rax, %rax",
        "sete %al",
        "movzx %al, %rax",
    });
    conversion_map[new OperatorSignature("++", new ReferenceType(p_int))] = new BuiltinOperator(new ReferenceType(p_int), {
        "incq (%rcx)",
        "mov %rcx, %rax",
    });
    conversion_map[new OperatorSignature("--", new ReferenceType(p_int))] = new BuiltinOperator(new ReferenceType(p_int), {
        "decq (%rcx)",
        "mov %rcx, %rax",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "++")] = new BuiltinOperator(p_int, {"incq (%rcx)"});
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "--")] = new BuiltinOperator(p_int, {"decq (%rcx)"});
    conversion_map[new OperatorSignature(p_int, "+", p_int)] = new BuiltinOperator(p_int, {"add %rbx, %rax"});
    conversion_map[new OperatorSignature(p_int, "-", p_int)] = new BuiltinOperator(p_int, {"sub %rbx, %rax"});
    conversion_map[new OperatorSignature(p_int, "*", p_int)] = new BuiltinOperator(p_int, {"imul %rbx, %rax"});
    conversion_map[new OperatorSignature(p_int, "/", p_int)] = new BuiltinOperator(p_int, {
        "cqo",
        "idiv %rbx",
    });
    conversion_map[new OperatorSignature(p_int, "%", p_int)] = new BuiltinOperator(p_int, {
        "cqo",
        "idiv %rbx",
        "mov %rdx, %rax",
    });
    conversion_map[new OperatorSignature(p_int, "&", p_int)] = new BuiltinOperator(p_int, {"and %rbx, %rax"});
    conversion_map[new OperatorSignature(p_int, "^", p_int)] = new BuiltinOperator(p_int, {"xor %rbx, %rax"});
    conversion_map[new OperatorSignature(p_int, "|", p_int)] = new BuiltinOperator(p_int, {"or %rbx, %rax"});
    conversion_map[new OperatorSignature(p_int, "&&", p_int)] = new BuiltinOperator(p_int, {
        "test %rax, %rax",
        "setne %al",
        "movzx %al, %rax",
        "test %rbx, %rbx",
        "setne %bl",
        "movzx %bl, %rbx",
        "and %rbx, %rax",
    });
    conversion_map[new OperatorSignature(p_int, "||", p_int)] = new BuiltinOperator(p_int, {
        "test %rax, %rax",
        "setne %al",
        "movzx %al, %rax",
        "test %rbx, %rbx",
        "setne %bl",
        "movzx %bl, %rbx",
        "or %rbx, %rax",
    });
    conversion_map[new OperatorSignature(p_int, "==", p_int)] = new BuiltinOperator(p_int, {
        "cmp %rbx, %rax",
        "sete %al",
        "movzx %al, %rax",
    });
    conversion_map[new OperatorSignature(p_int, "!=", p_int)] = new BuiltinOperator(p_int, {
        "cmp %rbx, %rax",
        "setne %al",
        "movzx %al, %rax",
    });
    conversion_map[new OperatorSignature(p_int, "<", p_int)] = new BuiltinOperator(p_int, {
        "cmp %rbx, %rax",
        "setl %al",
        "movzx %al, %rax",
    });
    conversion_map[new OperatorSignature(p_int, ">", p_int)] = new BuiltinOperator(p_int, {
        "cmp %rbx, %rax",
        "setg %al",
        "movzx %al, %rax",
    });
    conversion_map[new OperatorSignature(p_int, "<=", p_int)] = new BuiltinOperator(p_int, {
        "cmp %rbx, %rax",
        "setle %al",
        "movzx %al, %rax",
    });
    conversion_map[new OperatorSignature(p_int, ">=", p_int)] = new BuiltinOperator(p_int, {
        "cmp %rbx, %rax",
        "setge %al",
        "movzx %al, %rax",
    });
    conversion_map[new OperatorSignature(p_int, "<<", p_int)] = new BuiltinOperator(p_int, {
        "push %rcx",
        "mov %rbx, %rcx",
        "sal %cl, %rax",
        "pop %rcx",
    });
    conversion_map[new OperatorSignature(p_int, ">>", p_int)] = new BuiltinOperator(p_int, {
        "push %rcx",
        "mov %rbx, %rcx",
        "sar %cl, %rax",
        "pop %rcx",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "+=", p_int)] = new BuiltinOperator(p_int, {
        "add %rbx, %rax",
        "mov %rax, (%rcx)",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "-=", p_int)] = new BuiltinOperator(p_int, {
        "sub %rbx, %rax",
        "mov %rax, (%rcx)",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "*=", p_int)] = new BuiltinOperator(p_int, {
        "imul %rbx, %rax",
        "mov %rax, (%rcx)",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "/=", p_int)] = new BuiltinOperator(p_int, {
        "cqo",
        "idiv %rbx",
        "mov %rax, (%rcx)",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "%=", p_int)] = new BuiltinOperator(p_int, {
        "cqo",
        "idiv %rbx",
        "mov %rdx, %rax",
        "mov %rax, (%rcx)",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "<<=", p_int)] = new BuiltinOperator(p_int, {
        "push %rcx",
        "mov %rbx, %rcx",
        "sal %cl, %rax",
        "pop %rcx",
        "mov %rax, (%rcx)",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), ">>=", p_int)] = new BuiltinOperator(p_int, {
        "push %rcx",
        "mov %rbx, %rcx",
        "sar %cl, %rax",
        "pop %rcx",
        "mov %rax, (%rcx)",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "&=", p_int)] = new BuiltinOperator(p_int, {
        "and %rbx, %rax",
        "mov %rax, (%rcx)",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "^=", p_int)] = new BuiltinOperator(p_int, {
        "xor %rbx, %rax",
        "mov %rax, (%rcx)",
    });
    conversion_map[new OperatorSignature(new ReferenceType(p_int), "|=", p_int)] = new BuiltinOperator(p_int, {
        "or %rbx, %rax",
        "mov %rax, (%rcx)",
    });

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
    fout << indent() << "push " << reg << "\n";
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

    fout << indent() << "pop " << reg << "\n";
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
void emit_retrieve_array(int sz) {
    if(sz == 1) fout << indent() << "movb (%rax, %rbx, 1), %al\n";
    else if(sz == 2) fout << indent() << "movw (%rax, %rbx, 2), %ax\n";
    else if(sz == 4) fout << indent() << "movl (%rax, %rbx, 4), %eax\n";
    else if(sz == 8) fout << indent() << "movq (%rax, %rbx, 8), %rax\n";
    else assert(false);
}

//expects %rax = array start, %rbx = array index
//will put address of element into %rax
void emit_address_array(int sz) {
    if(sz == 1) fout << indent() << "lea (%rax, %rbx, 1), %rax\n";
    else if(sz == 2) fout << indent() << "lea (%rax, %rbx, 2), %rax\n";
    else if(sz == 4) fout << indent() << "lea (%rax, %rbx, 4), %rax\n";
    else if(sz == 8) fout << indent() << "lea (%rax, %rbx, 8), %rax\n";
    else assert(false);
}

//expects %rax = array start, %rbx = array index, %rcx = value
//writes %rcx (or whatever portion of %rcx) into array
void emit_write_array(int sz) {
    if(sz == 1) fout << indent() << "movb %cl, (%rax, %rbx, 1)\n";
    else if(sz == 2) fout << indent() << "movw %cx, (%rax, %rbx, 2)\n";
    else if(sz == 4) fout << indent() << "movl %ecx, (%rax, %rbx, 4)\n";
    else if(sz == 8) fout << indent() << "movq %rcx, (%rax, %rbx, 8)\n";
    else assert(false);
}

//expects %rax = address
//will put return value into %rax
void emit_mem_retrieve(int sz) {
    if(sz == 1) fout << indent() << "movb (%rax), %al\n";
    else if(sz == 2) fout << indent() << "movw (%rax), %ax\n";
    else if(sz == 4) fout << indent() << "movl (%rax), %eax\n";
    else if(sz == 8) fout << indent() << "movq (%rax), %rax\n";
    else assert(false);
}

//expects %rax = value, %rbx = mem address
//stores %rax into (%rbx)
void emit_mem_store(int sz) {
    if(sz == 1) fout << indent() << "movb %al, (%rbx)\n";
    else if(sz == 2) fout << indent() << "movw %ax, (%rbx)\n";
    else if(sz == 4) fout << indent() << "movl %eax, (%rbx)\n";
    else if(sz == 8) fout << indent() << "movq %rax, (%rbx)\n";
    else assert(false);
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
        return new BuiltinOperator(to, {});    //do nothing
    }
    Type* voidptr_t = new PointerType(new BaseType("void"));
    // - from is a pointer, to is void*
    if(dynamic_cast<PointerType*>(from) != nullptr && *to == *voidptr_t) {
        return new BuiltinOperator(to, {});    //do nothing
    }
    // - from is void*, to is a pointer
    if(*from == *voidptr_t && dynamic_cast<PointerType*>(to) != nullptr) {
        return new BuiltinOperator(to, {});    //do nothing
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
    //one of left or right has to have a value
    assert(left.has_value() || right.has_value());
    //check that the values are not nullptr
    if(left.has_value()) assert(left.value() != nullptr);
    if(right.has_value()) assert(right.value() != nullptr);
    //this should not be the casting operator
    assert(op != "(cast)");

    // std::cout << "Find operator implementation : " << op << "\n";
    
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
    if(viable.size() != 1) return nullptr;
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
    for(int i = 0; i < declared_functions.size(); i++){
        if(*(declared_functions[i]->resolve_function_signature()) == *fs) return declared_functions[i]->type;
    }
    return nullptr;
}

bool is_type_declared(Type *t) {
    if(auto x = dynamic_cast<PointerType*>(t)) return is_type_declared(x->type);
    if(auto x = dynamic_cast<ReferenceType*>(t)) return is_type_declared(x->type);
    for(int i = 0; i < declared_types.size(); i++){
        if(*(declared_types[i]) == *t) return true;
    }
    return false;
}

bool is_function_declared(FunctionSignature *fs) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(fs->equals(declared_functions[i]->resolve_function_signature())) {
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
    for(int i = 0; i < declared_constructors.size(); i++){
        if(cs->equals(declared_constructors[i]->resolve_constructor_signature())) {
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
Constructor* get_called_constructor(ConstructorCall *cc) {
    assert(cc != nullptr);
    Type *type = cc->type;
    std::vector<Expression*> args = cc->argument_list;
    assert(type != nullptr);
    
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
        if(!is_viable) break;

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

std::string get_function_label(FunctionSignature *fs) {
    assert(function_label_map.count(fs));
    return function_label_map[fs];
}

std::string get_constructor_label(ConstructorSignature *cs) {
    assert(constructor_label_map.count(cs));
    return constructor_label_map[cs];
}

Variable* get_variable(Identifier *id) {
    for(int i = 0; i < declared_variables.size(); i++){
        if(*id == *(declared_variables[i]->id)) {
            return declared_variables[i];
        }
    }
    return nullptr;
}

//checks against all variables, functions
bool is_identifier_used(Identifier *id) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(*id == *(declared_functions[i]->id)) return true;
    }
    for(int i = 0; i < declared_variables.size(); i++){
        if(*id == *(declared_variables[i]->id)) return true;
    }
    return false;
}

bool add_type(Type *t) {
    assert(t != nullptr);
    if(is_type_declared(t)) return false;
    declared_types.push_back(t);
    std::cout << "ADD TYPE : " << t->to_string() << "\n";
    return true;
}

bool add_primitive_type(Type *t) {
    assert(t != nullptr);
    if(is_type_declared(t)) return false;
    declared_types.push_back(t);
    primitive_base_types.insert(t);
    return true;
}

bool add_function(Function *f){
    assert(f != nullptr);
    FunctionSignature *fs = f->resolve_function_signature();
    if(is_function_declared(fs)) return false;
    if(auto x = dynamic_cast<OperatorOverload*>(f)) {   //if it's an overload, register it
        if(!add_operator_implementation(x)) return false;
    }
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
    function_label_map.insert({fs, f->id->name});
    return true;
}

bool add_constructor(Constructor *c) {
    assert(c != nullptr);
    ConstructorSignature *cs = c->resolve_constructor_signature();
    if(is_constructor_declared(cs)) assert(false);
    declared_constructors.push_back(c);
    constructor_label_map.insert({cs, create_new_label()});
    std::cout << "ADD CONSTRUCTOR : " << cs->to_string() << std::endl;
    return true;
}

Variable* add_variable(Type *t, Identifier *id) {
    assert(t != nullptr && id != nullptr);
    assert(declaration_stack.size() != 0);
    if(is_identifier_used(id)) return nullptr;
    Variable *v = new Variable(t, id);
    declared_variables.push_back(v);
    declaration_stack.top().push_back(v);
    return v;
}

void remove_function(Function *f) {
    assert(f != nullptr);
    if(auto x = dynamic_cast<OperatorOverload*>(f)) {
        remove_operator_implementation(x);
    }
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

void push_declaration_stack() {
    declaration_stack.push(std::vector<Variable*>(0));
}

void pop_declaration_stack() {
    assert(declaration_stack.size() != 0);

    std::vector<Variable*> top = declaration_stack.top();
    declaration_stack.pop();
    if(declaration_stack.size() > 0) {
        //this is not the function parameter layer, adjust %rsp
        std::vector<std::string> desc_list;
        for(int i = 0; i < top.size(); i++){
            desc_list.push_back(top[i]->id->name);
        }
        emit_add_rsp(top.size() * 8, desc_list);
    }
    
    for(int i = 0; i < top.size(); i++){
        remove_variable(top[i]->id);
    }
}

StructLayout* get_struct_layout(Type *t) {
    assert(t != nullptr);
    assert(struct_layout_map.count(t));
    return struct_layout_map[t];
}

bool add_struct_layout(Type *t, StructLayout *sl) {
    assert(t != nullptr);
    assert(sl != nullptr);
    assert(!struct_layout_map.count(t));
    struct_layout_map.insert({t, sl});
    return true;
}

//allocates sz_bytes memory by calling malloc. Resulting address is in %rax
void emit_malloc(int sz_bytes) {
    fout << indent() << "mov $" << sz_bytes << ", %rax\n";
    emit_push("%rax", "emit_malloc() : malloc arg");
    fout << indent() << "call malloc\n";
    emit_add_rsp(8, "emit_malloc() : malloc arg");
}

//expects memory address in %rax, places primitive into address, returns with memory address in %rax
//expects memory to already have been allocated
void emit_initialize_primitive(Type *t) {
    if(*t == BaseType("int")) {
        fout << indent() << "movq $0, (%rax)\n";
    }
    else if(*t == BaseType("char")) {
        fout << indent() << "movb $0, (%rax)\n";
    }
    else if(dynamic_cast<PointerType*>(t) != nullptr) {
        fout << indent() << "movq $0, (%rax)\n";
    }
    else if(dynamic_cast<ReferenceType*>(t) != nullptr) {
        Type *rt = dynamic_cast<ReferenceType*>(t)->type;
        if(is_type_primitive(rt)) {
            //save address to reference 
            emit_push("%rax", "emit_initialize_primitve() : addr to primitive ref");

            //allocate some memory for the referenced type
            int sz = rt->calc_size();
            emit_malloc(sz);

            //initialize primitive at address
            emit_initialize_primitive(rt);

            //write address into reference
            fout << indent() << "mov %rax, %rbx\n";
            emit_pop("%rax", "emit_initialize_primitve() : addr to primitive ref");
            fout << indent() << "movq %rbx, (%rax)\n";
        }
        else {
            //save address to reference 
            emit_push("%rax", "emit_initialize_primitve() : addr to struct ref");

            //initialize struct
            emit_initialize_struct(rt);

            //write heap pointer address into reference
            fout << indent() << "mov %rax, %rbx\n";
            emit_pop("%rax", "emit_initialize_primitve() : addr to struct ref");
            fout << indent() << "movq %rbx, (%rax)\n";
        }
    }
    else {
        std::cout << "Tried to initialize unrecognized primitive type : " << t->to_string() << std::endl;
        assert(false);
    }
}

//allocates new memory on the heap then places the specified struct there
//resulting heap pointer address is in %rax, actual struct address is in %rbx
//just default initializes all member variables
void emit_initialize_struct(Type *t) {
    assert(t != nullptr);
    assert(!is_type_primitive(t));
    StructLayout *sl = get_struct_layout(t);

    if(asm_debug) fout << indent() << "# initialize struct memory " << t->to_string() << "\n";

    //allocate the memory, +8 for the pointer on heap
    emit_malloc(sl->get_size() + 8);

    //save actual struct loc for later
    //%rax is the pointer for the current location in the struct
    emit_push("%rax", "emit_initialize_struct() : struct base ptr");

    //initialize member variables
    for(int i = 0; i < sl->member_variables.size(); i++){
        //initialize member variable
        MemberVariable *mv = sl->member_variables[i];
        int size = mv->type->calc_size();
        if(is_type_primitive(mv->type)) {   //primitive
            emit_initialize_primitive(mv->type);
        }
        else {  //struct
            //invoke default constructor
            emit_push("%rax", "emit_initialize_struct() : struct ptr");
            FunctionCall *fc = new FunctionCall(new Identifier(t->to_string()), {});
            fc->emit_asm();

            //save reference to struct
            fout << indent() << "mov %rax, %rbx\n";
            emit_pop("%rax", "emit_initialize_struct() : struct ptr");
            fout << indent() << "movq %rbx, (%rax)\n";
        }

        //increment pointer
        fout << indent() << "add $" << size << ", %rax\n";
    }

    //initialize struct heap ptr
    emit_initialize_primitive(new PointerType(t));
    emit_pop("%rbx", "emit_initialize_struct() : struct base ptr");
    fout << indent() << "movq %rbx, (%rax)\n";

    if(asm_debug) fout << indent() << "# done initialize struct memory " << t->to_string() << "\n";

    //now, %rax should hold location of heap ptr, %rbx holds location of actual struct start
}

//should be logically similar to is_declarable(), except this one emits a variable declaration 
//every variable declaration must use this except for registering parameters at the beginning of function calls
//evaluates the expression and initializes the variable onto the top of the stack
//if for some reason is unable to initialize the variable, returns nullptr
Variable* emit_initialize_variable(Type *vt, Identifier *id, Expression *expr) { 
    assert(vt != nullptr);
    assert(id != nullptr);
    assert(expr != nullptr);    //might want to later have a version that default declares types

    std::cout << "Initialize variable : " << vt->to_string() << " " << id->name << std::endl;

    // - make sure vt is not void
    if(*vt == BaseType("void")) {
        std::cout << "Cannot initialize a variable of type void\n";
        return nullptr;
    }
    // - is the identifier available?
    if(is_identifier_used(id)) {
        std::cout << "Identifier already used : " << id->name << "\n";
        return nullptr;
    }
    // - does the expression resolve to a type?
    if(expr->resolve_type() == nullptr){
        std::cout << "Expression does not resolve to type\n";
        return nullptr;
    }

    Type *et = expr->resolve_type();
    bool is_lvalue = expr->is_lvalue();
    Variable *v = add_variable(vt, id);
    assert(v != nullptr);

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

        //we want addr
        fout << indent() << "mov %rcx, %rax\n";

        //push to stack
        emit_push("%rax", id->name);

        //set stack offset
        v->stack_offset = local_offset;
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
            //claim next stack address
            emit_sub_rsp(8, id->name);
            fout << indent() << "lea (%rsp), %rax\n";

            //this will initialize the primitive onto the stack
            emit_initialize_primitive(vt);
        }
        else {
            //initialize struct in memory
            emit_initialize_struct(vt);

            //push pointer to the stack
            emit_push("%rbx", id->name);
        }

        //set stack offset
        v->stack_offset = local_offset;

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

//assumes reference address is in %rax
//returns with value in %rax, address in %rcx
void emit_dereference(Type *rt) {
    assert(rt != nullptr);
    assert(dynamic_cast<ReferenceType*>(rt) != nullptr);

    Type *t = dynamic_cast<ReferenceType*>(rt)->type;
    assert(t != nullptr);
    int sz = t->calc_size();
    fout << indent() << "mov %rax, %rcx\n";
    emit_mem_retrieve(sz); 
}

bool add_operator_implementation(OperatorSignature *os, OperatorImplementation *oi){
    if(conversion_map.count(os)) {
        return false;
    }
    conversion_map[os] = oi;
    return true;
}

bool add_operator_implementation(OperatorOverload *f){
    assert(f != nullptr);
    OperatorSignature *os = f->resolve_operator_signature();
    if(os == nullptr) return false;
    FunctionOperator *fo = new FunctionOperator(f);
    return add_operator_implementation(os, fo);
}

void remove_operator_implementation(OperatorSignature *os, OperatorImplementation *oi) {
    assert(conversion_map.count(os));
    conversion_map.erase(os);
}

void remove_operator_implementation(OperatorOverload *f) {
    assert(f != nullptr);
    OperatorSignature *os = f->resolve_operator_signature();
    assert(os != nullptr);
    FunctionOperator *fo = new FunctionOperator(f);
    remove_operator_implementation(os, fo);
}