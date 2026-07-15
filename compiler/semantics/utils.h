#pragma once
#include <string>
#include <optional>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
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
struct DefinitionSpace;
struct CompilationContext;
struct CompilationController;
struct ASTNode;
struct ArrayType;

struct Variable;
struct OperatorImplementation;
struct BuiltinOperator;
struct FunctionOperator;
struct StructLayout;
struct LoopContext;

enum class Visibility;

// -- HASHING STRUCTS --
struct TypeHash {
    size_t operator()(Type* t) const;
};
struct TypeEquals {
    bool operator()(Type* lhs, Type* rhs) const;
};

struct IdentifierHash {
    size_t operator()(Identifier* id) const;
};
struct IdentifierEquals {
    bool operator()(Identifier *lhs, Identifier *rhs) const;
};

struct FunctionSignatureHash {
    size_t operator()(FunctionSignature *fc) const;
};
struct FunctionSignatureEquals {
    bool operator()(FunctionSignature* lhs, FunctionSignature *rhs) const;
};

struct ConstructorSignatureHash {
    size_t operator()(ConstructorSignature *cs) const;
};
struct ConstructorSignatureEquals {
    bool operator()(ConstructorSignature *lhs, ConstructorSignature *rhs) const;
};

struct OperatorSignatureHash {
    size_t operator()(OperatorSignature *os) const;
};
struct OperatorSignatureEquals {
    bool operator()(OperatorSignature* lhs, OperatorSignature *rhs) const;
};

struct DeclarableHash {
    size_t operator()(std::pair<Type*, Expression*> p) const;
};
struct DeclarableEquals {
    bool operator()(std::pair<Type*, Expression*> lhs, std::pair<Type*, Expression*> rhs) const;
};

// -- UTIL STRUCTS --
struct Variable {
    bool is_global;
    bool is_extern;     //only global variables should be extern
    Type *type;
    Identifier *id;
    std::string addr;   //should hold something like "-8(%rbp)" or "L99(%rip)"
    Variable(bool _is_global, bool _is_extern, Type *_type, Identifier *_id, std::string addr);
};

//used by break and continue to know where to jump to and how many things to cleanup
struct LoopContext {
    std::string start_label;
    std::string assignment_label;
    std::string end_label;
    int declaration_layer;
    LoopContext(std::string _start_label, std::string assignment_label, std::string _end_label, int _declaration_layer);
}; 

// -- GENERAL UTILS --
typedef long double ld;
long double current_time_seconds();
void add_duration_stat(std::string name, ld dur);
void print_duration_stats();
void hash_combine(size_t& seed, size_t value);
std::string read_file(const std::string& filename);
std::vector<std::string> str_split(const std::string& s, char sep);
std::string extract_filename(std::string path);             // given path to file, gives you the filename
std::string extract_folder_path(std::string path);          // given path to file, gives you path to folder the file is in
std::string extract_stem(std::string filename);             // given filename 'foo.jank', gives you 'foo'
std::string extract_ext(std::string filename);              // given filename 'foo.jank', gives you 'jank'
std::string normalize_path(std::string path);               // given an absolute filepath, removes all relative moves ("..", ".")
std::string labelize_path(std::string path);                // given an absolute path, normalizes it, then replaces '/' between parts with underscores
std::string cwd_rel_to_absolute(std::string path);          // given path relative to CWD, gives absolute path
std::string libj_to_absolute(std::string name);             // given name of stdlib file, gives absolute path

// -- PARSE UTILS --
char escape_to_char(parser::escape *e);
std::vector<Type*> convert_type_list(parser::type_list *t);
std::vector<Parameter*> convert_parameter_list(parser::parameter_list *t);
std::vector<Expression*> convert_argument_list(parser::argument_list *t);
std::vector<Identifier*> convert_identifier_list(parser::identifier_list *t);

// -- EMIT ASM HELPERS --
std::string indent();
std::string create_new_label();
std::string create_new_tmp_variable_name();
void emit_push(std::string reg, std::string desc);
void emit_pop(std::string reg, std::string desc);
void emit_add_rsp(int amt, std::string desc);
void emit_add_rsp(int amt, std::vector<std::string> desc_list);
void emit_sub_rsp(int amt, std::string desc);

// -- EMIT ASM MEMORY ACCESS --
void emit_retrieve_array(int sz);
void emit_address_array(int sz);
void emit_write_array(int sz);
void emit_mem_retrieve(int sz);
void emit_mem_store(int sz);
void emit_malloc(int sz_bytes);
void emit_free(int sz_bytes);
void emit_dereference(Type *t);

// -- EMIT ASM CLEANUP --
void emit_destructor_call(CompilationContext *ctx, Type *t, bool should_dealloc = true);
void emit_cleanup_struct(CompilationContext* ctx, Type *t);
void emit_cleanup_declaration_stack_layer(CompilationContext *ctx, int layer_ind, std::vector<Identifier*> ignore);
void emit_cleanup_declaration_stack_layer(CompilationContext *ctx, int layer_ind);

// -- EMIT ASM INITIALIZATION --
void emit_initialize_primitive(Type *t);
void emit_initialize_array(CompilationContext *ctx, ArrayType *t);
void emit_initialize_struct(CompilationContext *ctx, Type *t);
Variable* emit_initialize_stack_variable(CompilationContext *ctx, Type *vt, Identifier *id, std::optional<Expression*> expr);
bool emit_initialize_global_variable(CompilationContext *ctx, Type* vt, Identifier *id, std::optional<Expression*> expr, bool is_extern);
bool emit_initialize_variable(CompilationContext *ctx, Type* vt, Identifier *id, std::optional<Expression*> expr, std::string addr_str, bool is_global = false, bool is_extern = false);

// -- LOCAL VARIABLE STATE --
Variable* get_variable(Identifier *id);
Type* get_variable_type(Identifier *id);
Variable* add_stack_variable(Type *t, Identifier *id);
Variable* add_global_variable(Type *t, Identifier *id, bool is_extern);
Variable* add_variable(Type *t, Identifier *id, std::string addr_str, bool is_global = false, bool is_extern = false);
void remove_variable(Identifier *id);
void remove_all_global_variables();         // can only be used when there are only global variables
bool is_variable_declared(Identifier *id);
void push_declaration_stack();
void pop_declaration_stack(CompilationContext *ctx, bool do_free = true);
void dump_stack_desc();

// -- LOOP STATE --
void push_loop_stack(std::string start_label, std::string assignment_label, std::string end_label);
void pop_loop_stack(std::string start_label, std::string assignment_label, std::string end_label);

// -- CONTEXT UTILS --
bool is_type_primitive(Type *t);
DefinitionSpace* get_definition_space(std::string filepath);        // if a definition space doesn't exist, this creates it
DefinitionSpace* get_definition_space(BaseType* t);                 // gets definition space that declares this BaseType
DefinitionSpace* get_definition_space(Type *t);                     // gets definition space that is responsible for this type
bool is_templated_type_well_formed(TemplatedType *t, CompilationContext *ctx);
bool create_templated_type(TemplatedType *t, CompilationContext *ctx);
bool create_array_type(ArrayType* t, CompilationContext *ctx);

//these add declarations to the specified DefinitionSpace and also the work queue
//also does some well formedness checking
//after DefinitionSpace initialization, should be purely using these
bool add_templated_function(DefinitionSpace* ds, TemplatedFunction* x, Visibility vis, CompilationContext* ctx);
bool add_function(DefinitionSpace* ds, Function* x, Visibility vis, CompilationContext* ctx);
bool add_operator(DefinitionSpace* ds, Operator* x, Visibility vis, CompilationContext* ctx);
bool add_struct(DefinitionSpace* ds, StructDefinition* x, Visibility vis, CompilationContext* ctx);
bool add_constructor(DefinitionSpace* ds, Constructor* x, Visibility vis, CompilationContext* ctx);
bool add_destructor(DefinitionSpace* ds, Destructor* x, Visibility vis, CompilationContext* ctx);

// -- GLOBAL UTILS --
StructDefinition* get_struct_definition(Type *t);
TemplatedStructDefinition* get_templated_struct_definition(BaseType *t);
StructLayout* get_struct_layout(Type *t);
std::string get_function_label(FunctionSignature *fs);
std::string get_constructor_label(ConstructorSignature *cs);
std::string get_destructor_label(Type *t);
std::string get_operator_label(OperatorSignature *os);
bool add_string_literal(std::string str);
std::string get_string_literal_label(std::string str);

// -- CONTROLLER --
//important directories
inline std::string compiler_dir;        // directory of compiler executable
inline std::string cwd_dir;
inline std::string stdlib_dir;          // stdlib root

//list of files we parsed source from
//this list is referred to in the ASTNode struct
//the most recent file to be parsed is always the last file in this list
inline std::vector<std::string> source_files;

//the target file should always be parsed first
const int TARGET_SOURCE_FILE = 0;

//output stream to assembly file
inline std::ofstream fout;

//definition spaces
inline DefinitionSpace* builtin_definition_space;                  //holds all builtin definitions
inline std::unordered_map<std::string, DefinitionSpace*> definition_spaces;  //mapping absolute filepath -> definition space
inline std::unordered_map<Type*, DefinitionSpace*, TypeHash, TypeEquals> basetype_to_definition_space;    //maps basetype to what definition space defines it

//queue of instantiated declarations to compile
inline std::queue<std::pair<ASTNode*, CompilationContext*>> work_queue;

//global struct information
inline std::vector<StructDefinition*> struct_definitions;
inline std::vector<std::pair<Type*, StructLayout*>> struct_layouts;    

inline int is_declarable_cntr = 0;
inline std::unordered_map<std::pair<Type*, Expression*>, bool, DeclarableHash, DeclarableEquals> is_declarable_cache;

//global label information
inline std::unordered_map<FunctionSignature*, std::string, FunctionSignatureHash, FunctionSignatureEquals> function_label_map;
inline std::unordered_map<ConstructorSignature*, std::string, ConstructorSignatureHash, ConstructorSignatureEquals> constructor_label_map;
inline std::unordered_map<Type*, std::string, TypeHash, TypeEquals> destructor_label_map;
inline std::unordered_map<OperatorSignature*, std::string, OperatorSignatureHash, OperatorSignatureEquals> operator_label_map;
inline std::unordered_map<std::string, std::string> string_literal_label_map;

//compilation state
inline std::vector<Variable*> declared_variables;               // holds all variables currently in scope
inline std::vector<Variable*> declared_global_variables;        // holds all global variables visible to the current compilation context
inline std::optional<Type*> enclosing_type;                     // if we're compiling a struct member function, this should be set to the type of the struct
inline Type* enclosing_return_type;                             // this should be set to the return type of the thing we're compiling 
inline std::vector<std::vector<Variable*>> declaration_stack;   // every 'layer' of the declaration stack should be contiguous on the stack in memory
inline std::vector<LoopContext*> loop_stack;                    

//add some helpful (?) comments in the generated asm. 
inline bool asm_debug = false;

//does some various changes to support a kernel
// - doesn't import syscall by default
// - doesn't call sys_exit() when returning out of main
inline bool kernel_mode = false;

//add some helpful (?) prints in the compiler
inline bool debug = false;

//toggle to print various performance related information
inline bool print_timing_info = false;

//disables all default includes
inline bool no_default_includes = false;

//only emit driver code
inline bool only_emit_driver = false;

//recursively look for all dependencies of the target file
inline bool recursive_compile = false;

//descriptions of whatever is on the stack. 
//to push anything, you need to provide a description
//to pop anything, you need to provide a description, and it will only work if the descriptions match
inline std::vector<std::string> stack_desc;  

inline int local_offset = 0;   //tracks the value %rsp - %rbp
inline int label_counter = 0;
inline int tmp_variable_counter = 0;

void initialize_controller();                           // should call this once before trying to compile

// these functions do some work and emit assembly via fout
bool compile(std::string target_filepath);              // compiles the target file
bool emit_driver(std::string target_filepath);          // looks for a main() in the target file, emits code to handle initialization and cleanup of program
bool compile_all(std::string target_filepath);          // recursively look for all files that the target depends on and compiles them all

