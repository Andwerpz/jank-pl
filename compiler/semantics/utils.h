#pragma once
#include <string>
#include <optional>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../parser/parser.h"

struct ExprNode;
struct Expression;
struct Type;
struct BaseType;
struct PointerType;
struct ReferenceType;
struct Identifier;
struct FunctionSignature;
struct Function;
struct OperatorOverload;
struct StructDefinition;
struct MemberVariable;
struct FunctionCall;

struct Variable;
struct OperatorSignature;
struct OperatorImplementation;
struct BuiltinOperator;
struct FunctionOperator;
struct StructLayout;

// -- HASHING STRUCTS --
namespace {
    struct TypeHash;
    struct TypeEquals;

    struct IdentifierHash;
    struct IdentifierEquals;

    struct FunctionSignatureHash;
    struct FunctionSignatureEquals;

    struct OperatorSignatureHash;
    struct OperatorSignatureEquals;
}

// -- UTIL STRUCTS --
struct Variable {
    Type *type;
    Identifier *id;
    int stack_offset;  
    Variable(Type *_type, Identifier *_id);
};

struct OperatorSignature {
    std::optional<Type*> left;
    std::string op;
    std::optional<Type*> right;
    OperatorSignature(Type *_left, std::string _op, Type *_right);
    OperatorSignature(std::string _op, Type *_right);
    OperatorSignature(Type *_left, std::string _op);
    OperatorSignature(Type *from, Type *to);

    size_t hash() const;
    bool equals(const OperatorSignature* other) const;
    bool operator==(const OperatorSignature& other) const;
    bool operator!=(const OperatorSignature& other) const;
};  

struct OperatorImplementation {
    Type* res_type;

    OperatorImplementation(Type *_res_type);

    virtual ~OperatorImplementation() = default;
    Type* get_res_type();
};

//for non-binary l-value operations, %rcx is where the address is
//for binary operators, assumes left is in %rax and right is in %rbx
//for left unary operators, assumes right is in %rax
//for right unary operators, assumes left is in %rax
//for casting, assumes the input is in %rax
//for the indexing operator specifically, '[]', the result of the expression is in %rbx
//will place the answer into %rax
struct BuiltinOperator : public OperatorImplementation {
    std::vector<std::string> instructions;
    BuiltinOperator(Type *_res_type, std::vector<std::string> _instructions);
    void emit_asm();
};

struct FunctionOperator : public OperatorImplementation {
    OperatorOverload *function;
    FunctionOperator(OperatorOverload *_function);
};

//holds information on where all the stuff is supposed to go within the heap portion of the struct
// - for every identifier, stores the relative offset within the struct
// - also holds the total size of the heap portion in bytes
struct StructLayout {
    std::vector<MemberVariable*> member_variables;
    std::unordered_map<std::string, int> offset_map;
    int size;

    StructLayout(std::vector<MemberVariable*> _member_variables, std::unordered_map<std::string, int> _offset_map, int _size);

    int get_offset(Identifier *id);
    Type* get_type(Identifier *id);
    int get_size();         //total size of 'main data' portion of heap struct
    int get_ptr_offset();   //offset from start of heap struct to pointer on heap
};

// -- UTIL FNDEFS --
void hash_combine(size_t& seed, size_t value);
std::string indent();
char escape_to_char(parser::escape *e);
std::string create_new_label();
std::string create_new_tmp_variable_name();
void emit_retrieve_array(int sz);
void emit_address_array(int sz);
void emit_write_array(int sz);
void emit_mem_retrieve(int sz);
void emit_mem_store(int sz);
bool is_declarable(Type *a, Expression* expr);       //does the declaration A foo = expr work?
OperatorImplementation* find_typecast_implementation(Type *from, Type *to);
OperatorImplementation* find_operator_implementation(std::optional<Expression*> left, std::string op, std::optional<Expression*> right);
OperatorImplementation* find_operator_implementation(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right);
Type* find_resulting_type(std::optional<Expression*> left, std::string op, std::optional<Expression*> right);
Type* find_resulting_type(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right);
Type* find_variable_type(Identifier *id);
Type* find_function_type(FunctionSignature *fs);
bool is_function_constructor(const Function *f);
bool is_type_declared(Type *t);
bool is_type_primitive(Type *t);
bool is_function_declared(FunctionSignature *fs);
bool is_variable_declared(Identifier *id);
Function* get_function(FunctionSignature *fs);  
Function* get_called_function(FunctionCall *fc);
std::string get_function_label(FunctionSignature *fs);
Variable* get_variable(Identifier *id);
bool is_identifier_used(Identifier *id);
bool add_type(Type *t);
bool add_primitive_type(Type *t);
bool add_function(Function *f);
bool add_sys_function(Function *f);
Variable* add_variable(Type *t, Identifier *id);
void remove_function(Function *f);
void remove_variable(Identifier *id);
void push_declaration_stack();
void pop_declaration_stack();
StructLayout* get_struct_layout(Type *t);
bool add_struct_layout(Type *t, StructLayout *sl);
void emit_initialize_primitive(Type *t);
void emit_initialize_struct(Type *t);
Variable* emit_initialize_variable(Type* vt, Identifier *id, Expression *expr);
void emit_dereference(Type *t);
void emit_push(std::string reg, std::string desc);
void emit_pop(std::string reg, std::string desc);
void emit_add_rsp(int amt, std::string desc);
void emit_add_rsp(int amt, std::vector<std::string> desc_list);
void emit_sub_rsp(int amt, std::string desc);
bool add_operator_implementation(OperatorSignature *os, OperatorImplementation *oi);
bool add_operator_implementation(OperatorOverload *f);
void remove_operator_implementation(OperatorSignature *os, OperatorImplementation *oi);
void remove_operator_implementation(OperatorOverload *f);
void emit_malloc(int sz_bytes);

// -- CONTROLLER --
//should probably move this stuff into its own file

inline std::ofstream fout;

//have these here to be visible. 
inline Function* enclosing_function;
inline std::vector<Function*> declared_functions;
inline std::vector<Variable*> declared_variables;
inline std::stack<std::vector<Variable*>> declaration_stack;   //every 'layer' of the declaration stack should be contiguous on the stack

//add some helpful (?) comments in the generated asm. 
inline bool asm_debug = true;

//descriptions of whatever is on the stack. 
//to push anything, you need to provide a description
//to pop anything, you need to provide a description, and it will only work if the descriptions match
inline std::vector<std::string> stack_desc;  

inline int local_offset;   //tracks the value %rsp - %rbp

void reset_controller();

