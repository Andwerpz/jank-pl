#pragma once
#include <vector>
#include <optional>
#include <string>
#include <unordered_map>
#include <span>
#include <ranges>

struct DefinitionSpace;

struct TemplatedFunction;
struct TemplatedOperator;
struct TemplatedStructDefinition;

struct Function;
struct Operator;
struct StructDefinition;
struct Constructor;
struct Destructor;

struct StructLayout;
struct GlobalDeclaration;

struct Type;
struct BaseType;

struct FunctionSignature;
struct OperatorSignature;
struct ConstructorSignature;

struct FunctionCall;
struct OperatorCall;
struct ConstructorCall;
struct DestructorCall;

struct Expression;
struct ExprNode;

enum class Visibility;

// full context the compiler needs to compile some piece of code. 
// should be relatively lightweight, this will get copied around somewhat. 
struct CompilationContext {
    struct DefinitionSpaceView {
        DefinitionSpace* space;
        Visibility vis;
    };
    
    // all definition spaces this context has access to
    std::vector<DefinitionSpaceView> definition_spaces;

    CompilationContext();

    // add definition + permission
    // if definition + permission already exists, takes the more permissive permission
    void add_definition_space(DefinitionSpace* space, Visibility vis);

    // makes sure all definition spaces in this context are ready for compilation
    void ensure_ready();

    // resolve result of calling something
    Function* get_called_function(FunctionCall* fc);
    Function* get_called_function(FunctionSignature* fs);
    Operator* get_called_operator(OperatorCall* oc);
    Constructor* get_called_constructor(ConstructorCall* cc);
    Destructor* get_called_destructor(DestructorCall* dc);

    Operator* get_called_operator(std::optional<Expression*> left, std::string op, std::optional<Expression*> right);
    Operator* get_called_operator(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right);
    Operator* get_called_typecast(Type* from, Type* to);

    // expression utilities
    Type* find_resulting_type(std::optional<Expression*> left, std::string op, std::optional<Expression*> right);
    Type* find_resulting_type(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right);
    bool is_declarable(Type* a, Expression* expr);  //does the declaration 'A foo = expr' work?

    // checking is declared
    bool is_type_declared(Type* t) const;
    bool is_basetype_declared(BaseType* t) const;

    // misc
    void dump_context();    // print out what definition spaces are inside this context

private:
    // helpers to aggregate declarations from definition spaces
    // they're private right now because using them externally would mean not having 'auto' as the return type
    // can only use externally if the return type is deduced.
    // implementing them in the header is also not an option as that would require knowing the full layout of DefinitionSpace
    //   actually, I could probably do some jank indirect call to DefinitionSpace
    // writing out their return type would be very inelegant as it's very long. 
    // TODO write a custom iterator that this thing returns. 
    auto get_templated_functions() const;
    auto get_templated_operators() const;
    auto get_templated_structs() const;
    auto get_operators() const;
    auto get_functions() const;
    auto get_structs() const;
    auto get_constructors() const;
    auto get_destructors() const;
    auto get_typedefs() const;
    auto get_global_variables() const;
    auto get_types() const;
    auto get_basetypes() const;
};
