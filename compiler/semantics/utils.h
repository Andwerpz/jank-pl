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
struct Destructor;
struct DestructorCall;
struct ArrayType;
struct Parameter;
struct OperatorSignature;
struct Operator;
struct OperatorCall;
struct TemplatedOperator;
struct OperatorOverload;

struct Variable;
struct OperatorImplementation;
struct BuiltinOperator;
struct FunctionOperator;
struct StructLayout;
struct LoopContext;

// -- GENERAL UTILS --
typedef long double ld;
long double current_time_seconds();
void add_duration_stat(std::string name, ld dur);
void print_duration_stats();

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
    bool is_extern; //only global variables should be extern
    Type *type;
    Identifier *id;
    std::string addr;   //should hold something like "-8(%rbp)" or "L99(%rip)"
    Variable(bool _is_extern, Type *_type, Identifier *_id, std::string addr);
};

//used by break and continue to know where to jump to and how many things to cleanup
struct LoopContext {
    std::string start_label;
    std::string assignment_label;
    std::string end_label;
    int declaration_layer;
    LoopContext(std::string _start_label, std::string assignment_label, std::string _end_label, int _declaration_layer);
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
bool is_operator_declared(OperatorSignature *os);
bool is_variable_declared(Identifier *id);
bool is_constructor_declared(ConstructorSignature *cs);
bool is_destructor_declared(Type *t);
Function* get_function(FunctionSignature *fs);  
Function* get_called_function(FunctionCall *fc);
Operator* get_operator(OperatorSignature *os);
Operator* get_called_operator(OperatorCall *oc);
Operator* get_called_operator(std::optional<Expression*> left, std::string op, std::optional<Expression*> right);
Operator* get_called_operator(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right);
Operator* get_called_typecast(Type *from, Type *to);
Constructor* get_called_constructor(ConstructorCall *cc);
Destructor* get_called_destructor(DestructorCall *dc);
std::string get_function_label(FunctionSignature *fs);
std::string get_constructor_label(ConstructorSignature *cs);
std::string get_destructor_label(Type *t);
std::string get_operator_label(OperatorSignature *os);
Variable* get_variable(Identifier *id);
bool add_struct_type(StructDefinition *sd);
bool add_primitive_basetype(BaseType *t);   //these don't constitute all the primitive types, you can have derived primitive types (like pointer)
bool add_basetype(BaseType *t);
bool add_arraytype(ArrayType *t);
bool add_templated_struct(TemplatedStructDefinition *t);
bool add_templated_function(TemplatedFunction *f);  
bool add_templated_operator(TemplatedOperator *o);
bool create_templated_type(TemplatedType *t); 
bool create_arraytype(ArrayType *t);
bool add_function(Function *f);
bool add_operator(Operator *o);
bool add_builtin_operator(BuiltinOperator *o);
bool add_constructor(Constructor *c);
bool add_destructor(Destructor *d);
Variable* add_stack_variable(Type *t, Identifier *id);
Variable* add_global_variable(Type *t, Identifier *id, bool is_extern);
Variable* add_variable(Type *t, Identifier *id, std::string addr_str, bool is_global = false, bool is_extern = false);
void remove_function(Function *f);
void remove_variable(Identifier *id);
void remove_constructor(Constructor *c);
void remove_destructor(Destructor *d);
void push_declaration_stack();
void emit_destructor_call(Type *t, bool should_dealloc = true);
void emit_cleanup_declaration_stack_layer(int layer_ind);
void pop_declaration_stack(bool do_free = true);
void push_loop_stack(std::string start_label, std::string assignment_label, std::string end_label);   //call these when the loop variables are on the top of the declaration stack
void pop_loop_stack(std::string start_label, std::string assignment_label, std::string end_label);
bool construct_struct_layout(Type *t);
StructLayout* get_struct_layout(Type *t);   
StructDefinition* get_struct_definition(Type *t);   
void emit_initialize_primitive(Type *t);
void emit_initialize_struct(Type *t);
Variable* emit_initialize_stack_variable(Type *vt, Identifier *id, std::optional<Expression*> expr);
Variable* emit_initialize_global_variable(Type* vt, Identifier *id, std::optional<Expression*> expr, bool is_extern);
Variable* emit_initialize_variable(Type* vt, Identifier *id, std::optional<Expression*> expr, std::string addr_str, bool is_global = false, bool is_extern = false);
void emit_dereference(Type *t);
void dump_stack_desc();
void emit_push(std::string reg, std::string desc);
void emit_pop(std::string reg, std::string desc);
void emit_add_rsp(int amt, std::string desc);
void emit_add_rsp(int amt, std::vector<std::string> desc_list);
void emit_sub_rsp(int amt, std::string desc);
void emit_malloc(int sz_bytes);
void emit_free(int sz_bytes);
bool add_string_literal(std::string str);
std::string get_string_literal_label(std::string str);
void emit_data_section();

// -- PARSE UTILS --
std::vector<Type*> convert_type_list(parser::type_list *t);
std::vector<Parameter*> convert_parameter_list(parser::parameter_list *t);
std::vector<Expression*> convert_argument_list(parser::argument_list *t);
std::vector<Identifier*> convert_identifier_list(parser::identifier_list *t);

// -- CONTROLLER --
//should probably move this stuff into its own file

inline std::ofstream fout;

//have these here to be visible. 
inline Function* enclosing_function;
inline OperatorOverload* enclosing_overload;
inline std::vector<Function*> declared_functions;
inline std::vector<StructDefinition*> declared_structs;
inline std::vector<Operator*> declared_operators;
inline std::vector<Constructor*> declared_constructors;
inline std::vector<Destructor*> declared_destructors;
inline std::vector<Variable*> declared_variables;
inline std::vector<std::vector<Variable*>> declaration_stack;   //every 'layer' of the declaration stack should be contiguous on the stack
inline std::vector<LoopContext*> loop_stack;

//add some helpful (?) comments in the generated asm. 
inline bool asm_debug = false;

//does some various changes to support a kernel
// - doesn't import syscall by default
// - doesn't call sys_exit() when returning out of main
inline bool kernel_mode = false;

//add some helpful (?) prints in the compiler
inline bool debug = false;

//descriptions of whatever is on the stack. 
//to push anything, you need to provide a description
//to pop anything, you need to provide a description, and it will only work if the descriptions match
inline std::vector<std::string> stack_desc;  

inline std::string global_init_label;
inline int local_offset;   //tracks the value %rsp - %rbp

void reset_controller();

