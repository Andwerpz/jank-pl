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
struct Constructor;
struct ConstructorSignature;
struct ConstructorCall;
struct TemplatedType;
struct TemplatedStructDefinition;
struct Program;
struct TemplatedFunction;
struct Overload;
struct TemplatedOverload;
struct Destructor;
struct DestructorCall;

struct Variable;
struct OperatorSignature;
struct OperatorImplementation;
struct BuiltinOperator;
struct FunctionOperator;
struct StructLayout;
struct LoopContext;

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
    std::string addr;   //should hold something like "-8(%rbp)" or "24(%r15)"
    Variable(Type *_type, Identifier *_id);
};

//used by break and continue to know where to jump to and how many things to cleanup
struct LoopContext {
    std::string start_label;
    std::string assignment_label;
    std::string end_label;
    int declaration_layer;
    LoopContext(std::string _start_label, std::string assignment_label, std::string _end_label, int _declaration_layer);
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
    std::string to_string();
};  

struct OperatorImplementation {
    Type* res_type;

    OperatorImplementation(Type *_res_type);

    virtual ~OperatorImplementation() = default;
    Type* get_res_type();
};

//when returning a l-value, %rcx is where the mem address should go
//for binary operators, assumes left is in %rax, right in %rcx
//for left unary operators, assumes right is in %rax
//for right unary operators, assumes left is in %rax
//for casting, assumes the input is in %rax
//for the indexing operator specifically, '[]', the result of the expression is in %rbx
//will place the answer into %rax, address in %rcx
struct BuiltinOperator : public OperatorImplementation {
    std::vector<std::string> instructions;
    BuiltinOperator(Type *_res_type, std::vector<std::string> _instructions);
    void emit_asm();
};

struct OverloadedOperator : public OperatorImplementation {
    Overload *overload;
    OverloadedOperator(Overload *_overload);
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
bool is_type_declared(Type *t);
bool is_basetype_declared(BaseType *t);
bool is_templated_struct_declared(TemplatedStructDefinition *t);
bool is_templated_type_well_formed(TemplatedType *t);
bool is_type_primitive(Type *t);
bool is_function_declared(FunctionSignature *fs);
bool is_sys_function(FunctionSignature *fs);
bool is_variable_declared(Identifier *id);
bool is_constructor_declared(ConstructorSignature *cs);
bool is_destructor_declared(Type *t);
Function* get_function(FunctionSignature *fs);  
Function* get_called_function(FunctionCall *fc);
Constructor* get_called_constructor(ConstructorCall *cc);
Destructor* get_called_destructor(DestructorCall *dc);
std::string get_function_label(FunctionSignature *fs);
std::string get_constructor_label(ConstructorSignature *cs);
std::string get_destructor_label(Type *t);
std::string get_overload_label(OperatorSignature *os);
Variable* get_variable(Identifier *id);
bool add_struct_type(StructDefinition *sd);
bool add_primitive_basetype(BaseType *t);   //these don't constitute all the primitive types, you can have derived primitive types (like pointer)
bool add_basetype(BaseType *t);
bool add_templated_struct(TemplatedStructDefinition *t);
bool add_templated_function(TemplatedFunction *f);  //TODO add unification checking 
bool add_templated_overload(TemplatedOverload *o);
bool create_templated_type(TemplatedType *t); 
bool add_function(Function *f);
bool add_sys_function(Function *f);
bool add_constructor(Constructor *c);
bool add_destructor(Destructor *d);
Variable* add_variable(Type *t, Identifier *id, bool is_global = false);
void remove_function(Function *f);
void remove_variable(Identifier *id);
void remove_constructor(Constructor *c);
void remove_destructor(Destructor *d);
void push_declaration_stack();
void emit_cleanup_declaration_stack_layer(int layer_ind);
void pop_declaration_stack(bool do_free = true);
void push_loop_stack(std::string start_label, std::string assignment_label, std::string end_label);   //call these when the loop variables are on the top of the declaration stack
void pop_loop_stack(std::string start_label, std::string assignment_label, std::string end_label);
bool construct_struct_layout(Type *t);
StructLayout* get_struct_layout(Type *t);   
StructDefinition* get_struct_definition(Type *t);
void emit_initialize_primitive(Type *t);
void emit_initialize_struct(Type *t);
Variable* emit_initialize_stack_variable(Type *vt, Identifier *id, Expression *expr);
Variable* emit_initialize_variable(Type* vt, Identifier *id, Expression *expr, std::string addr_str, bool is_global = false);
void emit_dereference(Type *t);
void dump_stack_desc();
void emit_push(std::string reg, std::string desc);
void emit_pop(std::string reg, std::string desc);
void emit_add_rsp(int amt, std::string desc);
void emit_add_rsp(int amt, std::vector<std::string> desc_list);
void emit_sub_rsp(int amt, std::string desc);
bool add_operator_implementation(OperatorSignature *os, OperatorImplementation *oi);
bool add_operator_implementation(Overload *o);
void remove_operator_implementation(OperatorSignature *os, OperatorImplementation *oi);
void remove_operator_implementation(Overload *o);
void emit_malloc(int sz_bytes);
void emit_free(int sz_bytes);

// -- CONTROLLER --
//should probably move this stuff into its own file

inline std::ofstream fout;

//have these here to be visible. 
inline Function* enclosing_function;
inline Overload* enclosing_overload;
inline Program* enclosing_program;
inline std::vector<Function*> declared_functions;
inline std::vector<StructDefinition*> declared_structs;
inline std::vector<Overload*> declared_overloads;
inline std::vector<Constructor*> declared_constructors;
inline std::vector<Destructor*> declared_destructors;
inline std::vector<Variable*> declared_variables;
inline std::vector<std::vector<Variable*>> declaration_stack;   //every 'layer' of the declaration stack should be contiguous on the stack
inline std::vector<LoopContext*> loop_stack;

//add some helpful (?) comments in the generated asm. 
inline bool asm_debug = true;

//descriptions of whatever is on the stack. 
//to push anything, you need to provide a description
//to pop anything, you need to provide a description, and it will only work if the descriptions match
inline std::vector<std::string> stack_desc;  

inline std::string global_init_label;
inline int local_offset;   //tracks the value %rsp - %rbp

void reset_controller();

