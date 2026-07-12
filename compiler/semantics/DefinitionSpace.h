#pragma once
#include <vector>
#include <optional>
#include <string>
#include <unordered_map>

#include "utils.h"
#include "CompilationContext.h"

struct TemplatedFunction;
struct TemplatedOperator;
struct TemplatedStructDefinition;
struct Function;
struct Operator;
struct StructDefinition;
struct Constructor;
struct Destructor;
struct GlobalDeclaration;
struct Typedef;
struct Type;

struct Include;
struct GlobalNode;

struct StructLayout;

struct FunctionSignature;
struct OperatorSignature;
struct ConstructorSignature;

struct FunctionCall;
struct OperatorCall;
struct ConstructorCall;
struct DestructorCall;

struct Expression;
struct ExprNode;

struct CompilationContext;

//determines what definitions are available from an imported file
enum class Visibility {
    All                 = 0,    // only used when querying
    Public              = 1,    // visible to this file and all imports
    Private             = 2,    // only visible inside this file
};

enum class Origin {
    All                 = 0,    // only used when querying
    Source              = 1,    // this declaration came from code that was written by the user
    Generated           = 2,    // this declaration was somehow synthesized by the compiler 
    // TODO consider if we need to further do something like:
    // TemplateInstantiation
    // Synthesized
};

// or maybe 'DeclarationList'?
// container to manage declaration visibility and origin
template <typename T>
struct DeclarationSet {
    struct Bucket {
        std::vector<T*> all_decls;
        std::vector<T*> public_decls;
        std::vector<T*> private_decls;

        void add(T* decl, Visibility vis);
        const std::vector<T*>& get(Visibility vis) const;
        const std::vector<T*>& all() const;
    };

    Bucket all_decls;
    Bucket source_decls;
    Bucket generated_decls;

    void add(T* decl, Visibility vis, Origin orig);
    const std::vector<T*>& get(Visibility vis, Origin orig) const;
    const std::vector<T*>& all() const;
};

//initialization state of definition space
enum class DefinitionSpaceState {
    Unparsed                = 0,
    Parsed                  = 1,
    IncludesParsed          = 2,
    TypedefsResolved        = 3,
    DeclarationsResolved    = 4,
    IncludesResolved        = 5,
};

// TODO figure out whether or not we want to keep generating labels in here
// - arguments for generating labels here:
//   - this class is responsible for keeping track of all instantiated declarations, so 
//     all instantiated declarations do have to go here, and we can just generate a label whenever one is added. 
// - arguments for moving label generation elsewhere:
//   - having label generation as a byproduct of adding a function to a definition space is kinda uncomfortable

// represents all definitions tied to a single file
//   there is also one DefinitionSpace for everything that should be defined globally, 
//   stuff like primitive types and operators defined on primitive types
// definitions each have some 'visibility' level to determine where they can be accessed
// definitions can also include ones generated as a result of compilation (template instantiations)
//   template instantiations should be added as private definitions 
struct DefinitionSpace {
public:
    DefinitionSpace();                                                  // builtin
    DefinitionSpace(std::string filepath);                              // project files + stdlib

    // adding declarations
    // - returns true on success, false on failure
    bool add_templated_function(TemplatedFunction* x, Visibility vis, Origin orig);
    bool add_templated_operator(TemplatedOperator* x, Visibility vis, Origin orig);
    bool add_templated_struct(TemplatedStructDefinition* x, Visibility vis, Origin orig);
    bool add_function(Function* x, Visibility vis, Origin orig);
    bool add_operator(Operator* x, Visibility vis, Origin orig);
    bool add_struct(StructDefinition* x, Visibility vis, Origin orig);
    bool add_constructor(Constructor* x, Visibility vis, Origin orig);
    bool add_destructor(Destructor* x, Visibility vis, Origin orig);
    bool add_global_variable(GlobalDeclaration* x, Visibility vis, Origin orig);
    bool add_typedef(Typedef* x, Visibility vis, Origin orig);
    bool add_type(Type* x, Visibility vis, Origin orig);
    bool add_basetype(BaseType* x, Visibility vis, Origin orig);

    // retrieving declarations
    const std::vector<TemplatedFunction*>& get_templated_functions(Visibility vis, Origin orig = Origin::All);
    const std::vector<TemplatedOperator*>& get_templated_operators(Visibility vis, Origin orig = Origin::All);
    const std::vector<TemplatedStructDefinition*>& get_templated_structs(Visibility vis, Origin orig = Origin::All);
    const std::vector<Operator*>& get_operators(Visibility vis, Origin orig = Origin::All);
    const std::vector<Function*>& get_functions(Visibility vis, Origin orig = Origin::All);
    const std::vector<StructDefinition*>& get_structs(Visibility vis, Origin orig = Origin::All);
    const std::vector<Constructor*>& get_constructors(Visibility vis, Origin orig = Origin::All);
    const std::vector<Destructor*>& get_destructors(Visibility vis, Origin orig = Origin::All);
    const std::vector<Typedef*>& get_typedefs(Visibility vis, Origin orig = Origin::All);
    const std::vector<GlobalDeclaration*>& get_global_variables(Visibility vis, Origin orig = Origin::All);
    const std::vector<Type*>& get_types(Visibility vis, Origin orig = Origin::All);
    const std::vector<BaseType*>& get_basetypes(Visibility vis, Origin orig = Origin::All);

    // misc
    void set_label_prefix(std::string _label_prefix);
    std::string get_filepath() const;
    void add_include(Include* i);
    const std::vector<Include*>& get_includes();
    const std::vector<DefinitionSpace*>& get_included_definition_spaces();
    void add_global_node(GlobalNode* gn);
    const std::vector<GlobalNode*>& get_global_nodes();
    CompilationContext* create_compilation_context();

    // self explanatory :D
    // parses the code
    bool ensure_parsed();

    // just a wrapper to call the highest level of initialization 
    bool ensure_ready();                                

private:
    // state
    DefinitionSpaceState state;

    // declarations
    DeclarationSet<TemplatedFunction> templated_functions;
    DeclarationSet<TemplatedOperator> templated_operators;
    DeclarationSet<TemplatedStructDefinition> templated_structs;
    DeclarationSet<Operator> operators;
    DeclarationSet<Function> functions;
    DeclarationSet<StructDefinition> structs;
    DeclarationSet<Constructor> constructors;
    DeclarationSet<Destructor> destructors;
    DeclarationSet<Typedef> typedefs;
    DeclarationSet<GlobalDeclaration> global_variables;
    DeclarationSet<Type> types;
    DeclarationSet<BaseType> basetypes;

    // misc
    bool is_builtin;
    std::string filepath;
    std::string label_prefix;
    std::vector<DefinitionSpace*> included_definition_spaces;
    std::vector<std::string> include_filepaths;
    std::vector<Include*> includes;
    std::vector<GlobalNode*> global_nodes;

    // ensures all direct includes have been discovered and parsed
    bool ensure_includes_parsed();

    // resolve typedefs
    // - need to gather all public typedefs from imports
    bool ensure_typedefs_resolved();

    // ensure set of defined basetypes in this and all imported definition spaces are unique
    // construct basetype -> filepath map
    // register instantiated declarations
    // well formedness checks on instantiated structs
    // well formedness checks on templated declarations
    // look for templates 
    // - in general, we need to resolve all templates in anything that can effect compilation 
    //   of all other instantiated declarations. 
    // - for example, we need to resolve templates for global variables as these can be used
    //   within a function or operator body, and we need to know the global variable type. 
    // - for comparison, we don't need to resolve templates for an instantiated function body here
    //   as that only effects its own compilation. 
    // - TODO think about whether or not we need to resolve templates in function signatures. 
    bool ensure_declarations_resolved();

    // ensures all imported DefinitionSpaces are locally resolved
    bool ensure_includes_resolved(); 

    // these check for matches against templated declarations
    // can find a match if we can establish a bijection to another declaration
    bool is_templated_function_declared(TemplatedFunction* x) const;
    bool is_templated_operator_declared(TemplatedOperator* x) const;
    bool is_templated_struct_declared(TemplatedStructDefinition* x) const;

    // these check for exact matches against all instantiated declarations
    bool is_function_declared(FunctionSignature* fs) const;
    bool is_operator_declared(OperatorSignature* os) const;
    bool is_constructor_declared(ConstructorSignature* cs) const;
    bool is_destructor_declared(Type* t) const;
    bool is_global_variable_declared(GlobalDeclaration* gd) const;
    bool is_type_declared(Type* t) const;
    bool is_basetype_declared(BaseType* t) const;
};