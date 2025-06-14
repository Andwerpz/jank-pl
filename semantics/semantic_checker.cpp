#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <optional>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include "jank_parser.cpp"

/*
Takes in a .jank file, generates AST using jank_parser, then makes sure the program is well formed 
 - ensure every variable used is declared
 - do type checking for operators and assignments
 - verify function signatures are obeyed when calling them
 - check for existence of return statements in non-void functions. 

If the program is well formed, will spit out equivalent assembly code

For now, I will only support int as a valid type
To support a binary operator, you have to provide what types it takes in as well as results in, as well as 
some inline assembly code to actually perform the operation. 
logical && and || are a special case as I need to implement short circuiting, which can't be inlined as it
requires jumps. 
The two arguments of binary operators are assumed to be %rax, %rbx, and the result should get stored into %rax

will have two functions for checking, resolve_type() and is_well_formed(). 
resolve_type() returns nullptr if not well formed
these functions will only work with the proper context from declared_functions and declared_variables.
is_well_formed() will also emit asm.

Expression::emit_asm() and all of its subclasses will generate some assembly that will transparently compute
the result and put it into %rax

Literal::emit_asm() will generate some asm that transparently initializes the literal and puts the value
into %rax

to access local variables, do <offset>(%rbp) and lookup the offset in the declared_variables array. 
to use subroutine local variables, do <offset>(%rsp), and do your own book-keeping. All asm generated
within a subroutine should do everything transparently on the stack, except for output values of course. 

Registers are caller saved, there is no guarantee on transparency with those. These saves should happen not 
only before function calls, but before every subroutine. 

Anything that can enclose a DeclarationStatement cannot make use of the stack to save stuff, as variable
declaration needs to happen when the next offset position on the stack is available. 

ReturnStatement should clean up the current stack frame and return. 

FunctionCallStatement does not need to save any registers, but it does need to gather all arguments and 
pass them in via the stack. For now, function calls will behave like Java function calls, where everything
is pass by value.

For the purpose of looking up type conversions, any PointerType will be translated into BaseType("int")

Any type t where sizeof(t) <= 8, it's going to be stored directly on the stack. If sizeof(t) > 8, we'll store a 
pointer to t on the stack. When passing anything by reference, we'll have to keep track of it in Variable whether
or not it was passed by reference, and we'll store a pointer to the stack address instead of the actual
value / pointer. When accessing / modifying the passed by reference variable, we'll just dereference it first.

As long as we're consistent with how we treat types that are of size <= and > 8, then passing by reference 
shouldn't be an issue; it should affect all types the same. 

maybe we should include void as an actual type, but apply some restrictions:
 - variables cannot be of type void, Declaration has to check that the type isn't BaseType("void")
 - arguments cannot be of type void, so Functions have to check that all of their parameters are not of type BaseType("void")
Then, this allows void* to automatically be supported as a type. We can then add a type conversion dictionary that provides
conversions between types without operators, and every pointer should be able to be converted into void* and void* should be able to 
be converted into every pointer. These (pointer -> void*) and (void* -> pointer) conversions can be hardcoded as a special case 
in the conversion finder for now. 

To support pointer arithmetic, we should also hardcode some cases into the binary operator conversion finder. 
 - when subtracting two pointers of same type T*, it returns BaseType("int") equal to the byte offset between the two pointers divided
   by sizeof(T)
 - when adding or subtracting an int I to a pointer T*, it returns type T* incremented or decremented by I * sizeof(T)

just a note on type conversions. Suppose I have `A foo = expr`, where expr resolves to B. For now, I'll only look for direct conversions from
B -> A. So for example if there exists a conversion from B -> C and C -> A, I won't use it for now. 

ok, implemented casting to and from void* for declarations and assignments. Where else do we need this? How can we make it so that I don't
have to consider every case? Maybe there aren't too many cases?
 - when calling functions, arguments may have to be cast? what if it's ambiguous which function you're calling due to this?
 - I'll force the user to be clear about which function they're calling by making them cast their arguments if it's ambiguous. 

 - STRUCTS
okokok, onto implementing user defined types. Every user defined type will be a struct. The struct definition will be very 
C++ like, except there is no public/private keyword. Just declaring struct member variables and functions. 
Immediately, we can derive the memory layout from the order of struct member variables. Then, we can figure out the offset
of any member variable within the struct by taking the sum of sizes of members before it. 

I'll need a better way of storing stuff and keeping track of where everything is on the stack. For every type, should have 
a method of initializing it to an arbitrary memory location. Then, if we happen to want to place it on the stack, we can simply
increment the stack pointer. Where should this method be stored? In Type? Maybe I'll store it like I store type conversions. 
It'll be like a constructor. 

constructor should take in whatever arguments are needed to initialize the struct + memory address of where the struct should
start. Result should be initialized struct at the specified memory location. constructors should be generated as a function
that you can call. 'primitive' types should not get constructors as functions, instead they are caught as special cases. 
for now, 'primitive' types are int, char, and any PointerType. 

all constructors should return the thing they're constructing.

right now, only 
A foo = A();
would make sense. However, this requires both a constructor and copy constructor to be implemented. Perhaps for now, 
just require that both are implemented. We can do more shorthand and special case checking later. 

struct A {
    int a;
    int b;
    A A(int _a, int _b) {
        a = _a;
        b = _b;
    }
    A A(A other) {
        a = other.a;
        b = other.b;
    }
}

so a struct is a collection of declarations and functions. Specific functions are marked as special (constructor, 
copy constructor, default constructor) depending on their function signature. In all struct functions, all of the struct
member variables are passed in as arguments to the function. 

might also have to rework Expression. Currently, they all leave their result in %rax, which is fine for primitive types
but doesn't work for larger types. If we store a pointer to the actual values, it could work, but it could also introduce
more bugs and be less consistent as I'd need to specially check for each type whether or not it's on the stack or heap. 

orrr, could make this much easier and say that non-primitive structs are pointers that point to the actual memory location. 
so the struct IS the pointer and its destination. So before doing anything, will first need to check if the variable we're
dealing with is primitive or not. Also, when passing structs into functions, will only need to pass the pointer portion around.

note that pointers to structs will need to point to the pointer portion. This shouldn't be an issue as after the compiler 
does its magic, we should be able to essentially treat the pointer portion as the actual value. 


ok, structs are mostly done, just have to implement some other features
 - make it prohibited to assign to 'this' within a member function
 - when passing in stuff as an argument, should make a copy. Unless it's a reference. 


what's the next feature? operator overloading for structs? I/O? templating? 
should probably work on incorporating assignments into the expression structure. This means figuring out l-values and r-values. 

ok, assignments are in the expression structure, however I've ran into an issue enforcing that copy
constructors must be used on user defined struct assignment. The issue is invoking the function call, I don't want to have to 
rewrite all the constructor logic, so my idea is to encode the function call explicitly into the expression tree structure.
I'll call this step the 'elaboration' step for now. This should happen sometime after type resolution, but before code generation. 

one issue is deciding how exactly we're going to implement this elaboration step. Ideally, I want to avoid another full scan of the
program tree, but that means combining it with either the type resolution or code generation stage. If we do it during the type resolution 
stage, it isn't really enforced that it has to be done, or it could potentially be done multiple times. If it gets done during code generation, 
at least it's enforced that it should be done exactly once, however I feel rather uncomfortable changing up the expression tree structure right
as we're generating asm. 

We can also apply this elaboration step to auto casting when assigning. (also should figure out how exactly casts are going to work 
for user defined structs. I'm assuming they're going to be struct member functions that I call, so maybe can just turn all non-primitive
casts into function invocations.)


ok, elaboration step all done, also reworked expression syntax structure to make it easier, now each operation is one ExprNode, and
instead of 15 types of ExprNode in order of precedence, I assume that precedence is taken care of by the parser and I only have 4 types. 
This also makes elaboration really easy as I no longer have any restrictions on what ExprNodes can contain what other nodes. 

maybe can work on operator overloading next. Will have to figure out references first tho...
ok, two options:
 - treat reference as its own type, kind of like pointer
 - treat reference as just shorthand for pointers, with automatic dereferencing. 
I'm really leaning towards just treating it as a shorthand for pointers. 

ok, references as syntactic sugar for pointers are implemented. When delcaring a reference, if you assign it an l-value, it 'becomes' that value. 
otherwise if you assign it an r-value, a new instance of the referenced type is created, and the r-value value is assigned to it. 

the only issue is getting function lookup to work with it. We need some way to look up functions of 'best match'. This function lookup should
be triggered whenever we call get_function(FunctionSignature)

also need to rewrite FunctionCall::emit_asm() to use the variable declaring logic instead of just copying over the values. 

ok, so using the variable declaration in FunctionCall is problematic because function calls can happen inside of expressions, which violates
the assumption of Declaration that the current offset from %rbp to %rsp is equal to local_var_offset. There are some fixes:
 - copy over the assignment logic into FunctionCall, and don't initialize a new variable
 - keep track of the current %rsp - %rbp offset and use that offset to declare variables. This is probably the more robust solution, as then I 
   can declare temp variables whenever I want, so long as I make sure to remove them from the stack. 
    - should probably have some helper functions to push/pop from the stack and add/sub from %rsp.

maybe can also make a description of whatever is in which slot of the stack. 

ok, now we're keeping track of the current local offset and we can declare temp variables whenever we like. We just need
to make sure that one declaration_stack 'layer' of variables are all contiguous within stack memory. 
perhaps should also add 'layer' descriptions? 

some other features related to references that can be implemented:
 - detecting dangling references (perhaps this should be left for the user to debug)

its not a big issue right now, but I'll have to think more about the difference between emit_asm() and is_well_formed(). 
The original intention was to have two passes, once to check for semantic consistency, and the next to generate the code, but
it seems like the two passes have kind of melded together into one. 


ok, next is operator overloading. Since all my structs have all public member variables, every overload will just be a 
free function. So for example, dot product could be implemented like

int operator*(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

should also implement a more robust type conversion lookup system like the one for function calls. 

what operators do we want to be overloadable?
 - arithmetic binary operators : + - * / % & | ^ << >> 
 - comparison operators : == != < <= > >=
 - pre/post increment/decrement : ++x --x x++ x--
 - indexing : x[]
 - assignment : =
 - referencing, dereferencing : &x *x
 - casting : (type) x

we don't really have to assign any meaning to each operator (that's up to the user). Whenever we see some operator,
we first just check for an overload, then if we can't find an overload we just default to normal behaviour. 

overload lookup will be very similar to function lookup. I'm going to use whether or not we can declare A as B
as the ground truth for whether or not we can convert A into B. This is going to be used everywhere for implicit conversions
like function calls, return statements, overloads. Except for the casting operator, for that one I'll only accept 
exact matches. First, we don't want casts to have any weird conversions happening, and second, the assignment operator
uses casts to figure out whether or not it can assign, so allowing implicit conversions for the casting operator 
will result in infinite recursion. 

I'll enforce that you cannot define operator overloads within a struct. Any function defined within a struct has
to be a member function of that struct. 

When converting overloads, we'll add them to the functions list as normal to be checked. Perhaps we should add another
stage to the semantic checking, something like gather struct and function defs. Maybe gather_defs()? This should be
applicable to Program, Struct, (and eventually Namespace). Then when actually checking, we maintain the set of stuff
that can be refered to without any qualification. 

This should only be a consideration when implementing namespaces though, as right now we don't have to make any 
qualified references to anything. 

To evaluate and emit expressions, we'll look up operator overloads first, then we'll fallback onto default behaviour. 
When we elaborate, I want to turn all overloads into function calls, so when we emit we don't have to change the logic
and we can reuse the function call logic. 

ok, I've decided that I should respect the reference assignment rules. Namely, when initializing a reference, A&, we
can only assign to it a lvalue of type A (maybe it's a little stricter). Probably the easiest way to respect this is
to modify get_called_function() and is_declarable() to accept expressions instead of function signatures. This way I can
query to see whether something is a l-value or not, as well as its type. 

this change will make implementing builtin operators like '++' easier, as I can just insert them into the conversion map
and say they require type int&, and in the case of ++x, returns int&. Note that any operator that returns a reference type
should be classified as returning an l-value. 


struct nesting / namespaces:
only base types are allowed to be nested inside of other types
only base types are allowed to contain other types
essentially, only base types are allowed to participate in this nesting. (until I figure out templating)
when we're parsing code, we always keep track of what type we're currently in. The semantically correct minimum 
qualified typename depends on context. Suppose that we're currently in type A, and we want to refer to type B. 
 - if A == B, we can just do B
 - if the parent of B is A, we can just do B
 - if B is an ancestor of A, we need to give the fully qualified type of B
 - if B is a child of A, we can give the relative type from A
 - otherwise, we need to give the fully qualified type

if we have the restriction that types cannot be named the same as their ancestors, we can simply enforce that 
B must be qualified at least to some common ancestor of A and B. So it suffices to check all ancestors of A to see
where the B chain starts, and as all the ancestors of A must be unique, there is only one possible starting point. 

to resolve overloads, look at all of the types in the overload, and find the namespaces the types are defined in. 
Then, we can look in any namespace that contains any of the types mentioned to find overloads.


TODO
 - make sure that int main() is never called
 - make actual C++ style pass by copy and reference. 
 - figure out how to let the user access heap memory (arrays and stuff)
   - maybe implement pointers? so int* x is an int pointer, and x stores some heap memory address
   - don't just use raw ints for pointers so we can have char* s, and indexing into s will increment differently than indexing into x
   - need a way to compute the size of any type. For now, can hardcode all primitive types. 
   - sizeof() should be evaluated on the semantic checking level? any occurrence of sizeof() should turn into an integer literal
   - for now, just hardcode a sizeof() lookup table. 
   - all pointers will be 8 byte integers. 
 - implement better calc_size() for Type. Currently, it's just a bunch of special cases
 - right now, we can't have two functions with the same name as they'll generate the same label name in asm. Make it so 
   that all functions generate some kind of unique function label like "F<counter>"

CODE GENERATION
 - store all variables on the stack. This is much less performant, but much easier to implement
 - when going into function, have a 'stack barrier' and then define the positions of all the local variables
   relative to that stack barrier (negative offset? stack grows down??)
 - can also use stack to store temporary variables in calculations. 
 - really only using registers as temp storage. 
 - when exiting from a function, also store the return value on the stack. 
 - when storing stuff, I need to know the size in bytes of each type. Should probably figure out a way to
   determine that. Or maybe, I'll just store memory pointers on the stack. 

 - if we want to store big structs in the heap, will have to implement malloc :/
 - perhaps can start with a simple implementation: we just never reuse the space

 - primitives will be pass by copy (they'll be stored on the stack) while user defined structs will be 
   pass by reference (we'll store a pointer to them on the stack)

 - before a function is called, caller needs to put new base pointer, and put in the parameters into the stack.
   then, when the function is actually entered, callee will treat these as variables. 

 - the caller must save any registers they wish to maintain
 - the caller must pass the arguments onto the stack. 
 - the callee is responsible for setting up and returning the base pointer
*/

static void hash_combine(size_t& seed, size_t value) {
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// -- STRUCT DEFS --
struct Literal;
struct IntegerLiteral;
struct SizeofLiteral;
struct CharLiteral;
struct StringLiteral;
struct Identifier;
struct Type;
struct BaseType;
struct PointerType;
struct ExprNode;
struct ExprPrimary;
struct ExprBinary;
struct ExprPrefix;
struct ExprPostfix;
struct Expression;
struct Declaration;
struct Statement;
struct SimpleStatement;
struct DeclarationStatement;
struct ExpressionStatement;
struct ReturnStatement;
struct ControlStatement;
struct IfStatement;
struct WhileStatement;
struct ForStatement;
struct CompoundStatement;
struct Function;
struct FunctionSignature;
struct FunctionCall;
struct StructDefinition;
struct Program;

struct Variable;
struct OperatorSignature;
struct OperatorImplementation;
struct BuiltinOperator;
struct FunctionOperator;
struct StructLayout;

struct Literal {
    static Literal* convert(parser::literal *n);
    virtual Type* resolve_type() = 0;
    virtual void emit_asm() = 0;    //some inline assembly to initialize this literal into %rax
};

struct IntegerLiteral : public Literal {
    int val;
    IntegerLiteral(int _val) {
        val = _val;
    }
    Type* resolve_type() override;
    void emit_asm() override;
};

struct SizeofLiteral : public Literal {
    Type *type;
    SizeofLiteral(Type *_type) {
        type = _type;
    }
    Type* resolve_type() override;
    void emit_asm() override;
};

struct CharLiteral : public Literal {
    char val;
    CharLiteral(char _val) {
        val = _val;
    }
    Type* resolve_type() override;
    void emit_asm() override;
};

struct StringLiteral : public Literal {
    std::string val;
    StringLiteral(std::string _val) {
        val = _val;
    }
    Type* resolve_type() override;
    void emit_asm() override;
};

struct Identifier {
    std::string name;
    Identifier(std::string _name) {
        name = _name;
    }
    static Identifier* convert(parser::identifier *i);

    size_t hash() const {
        return std::hash<std::string>()(name);
    }
    bool operator==(const Identifier& other) const {
        return name == other.name;
    }
    bool operator!=(const Identifier& other) const {
        return !(*this == other);
    }
};

struct Type {
    static Type* convert(parser::type *t);
    virtual int calc_size() = 0;

    virtual bool equals(const Type *other) const = 0;
    bool operator==(const Type& other) const {return equals(&other);}
    bool operator!=(const Type& other) const {return !equals(&other);}
    virtual size_t hash() const = 0;
    virtual std::string to_string() = 0;
};

struct BaseType : public Type {
    std::string name;
    BaseType(std::string _name) {
        name = _name;
    }
    static BaseType* convert(parser::base_type *t);

    int calc_size() override {
        if(name == "int") return 8;
        else if(name == "char") return 1;
        else return 8;
    }

    bool equals(const Type *other) const override {
        if(auto x = dynamic_cast<const BaseType*>(other)) return name == x->name;
        return false;
    }

    size_t hash() const override {
        return std::hash<std::string>()(name) ^ 0x9e3779b9;
    }

    std::string to_string() {
        return name;
    }
};  

struct PointerType : public Type {
    Type *type;
    PointerType(Type *_type) {
        assert(_type != nullptr);
        type = _type;
    }

    int calc_size() override {
        return 8;
    }

    bool equals(const Type *other) const override {
        if(auto x = dynamic_cast<const PointerType*>(other)) return *type == *(x->type);
        return false;
    }

    size_t hash() const override {
        return type->hash() ^ 0x13952424;
    }

    std::string to_string() {
        return type->to_string() + "*";
    }
};

struct ReferenceType : public Type {
    Type *type;
    ReferenceType(Type *_type) {
        assert(_type != nullptr);
        type = _type;
    }

    int calc_size() override {
        return 8;
    }

    bool equals(const Type *other) const override {
        if(auto x = dynamic_cast<const ReferenceType*>(other)) return *type == *(x->type);
        return false;
    }

    size_t hash() const override {
        return type->hash() ^ 0xdeadbeef;
    }

    std::string to_string() {
        return type->to_string() + "&";
    }
};

struct ExprNode {
    static ExprNode* copy(ExprNode *other);

    static ExprNode* convert(parser::expr_primary *e);
    static ExprNode* convert(parser::expr_postfix *e);
    static ExprNode* convert(parser::expr_unary *e);
    static ExprNode* convert(parser::expr_multiplicative *e);
    static ExprNode* convert(parser::expr_additive *e);
    static ExprNode* convert(parser::expr_shift *e);
    static ExprNode* convert(parser::expr_relational *e);
    static ExprNode* convert(parser::expr_equality *e);
    static ExprNode* convert(parser::expr_bit_and *e);
    static ExprNode* convert(parser::expr_bit_xor *e);
    static ExprNode* convert(parser::expr_bit_or *e);
    static ExprNode* convert(parser::expr_logical_and *e);
    static ExprNode* convert(parser::expr_logical_or *e);
    static ExprNode* convert(parser::expr_assignment *e);

    virtual Type* resolve_type() = 0;   // - no modify
    virtual bool is_lvalue() = 0;       // - no modify
    virtual void elaborate() = 0;       // - can modify, but should not change return value of resolve_type(), is_lvalue()
    virtual void emit_asm() = 0;        // - no modify
    virtual std::string to_string() = 0;
};

//Type* is just a placeholder for a variable of that type. It's just used for type conversion purposes. 
//if there is a Type* and it tries to emit_asm(), it will assert(false). 
struct ExprPrimary : ExprNode {
    using val_t = std::variant<FunctionCall*, Identifier*, Literal*, Expression*, Type*>;
    val_t val;
    ExprPrimary(val_t _val) {
        val = _val;
    }
    Type* resolve_type() override;
    bool is_lvalue() override;
    void elaborate() override;
    void emit_asm() override;
    std::string to_string() override;
};

struct ExprBinary : ExprNode {
    using op_t = std::variant<std::string>;
    ExprNode *left;
    op_t op;
    ExprNode *right;
    ExprBinary(ExprNode *_left, op_t _op, ExprNode *_right) {
        left = _left;
        op = _op;
        right = _right;
        assert(left != nullptr);
        assert(right != nullptr);
    }
    Type* resolve_type() override;
    bool is_lvalue() override;
    void elaborate() override;
    void emit_asm() override;
    std::string to_string() override;
};

struct ExprPrefix : ExprNode {
    using op_t = std::variant<std::string>;
    op_t op;
    ExprNode *right;
    ExprPrefix(op_t _op, ExprNode *_right) {
        op = _op;
        right = _right;
        assert(right != nullptr);
    }
    Type* resolve_type() override;
    bool is_lvalue() override;
    void elaborate() override;
    void emit_asm() override;
    std::string to_string() override;
};

struct ExprPostfix : ExprNode {
    using op_t = std::variant<Expression*, std::pair<std::string, FunctionCall*>, std::pair<std::string, Identifier*>, std::string>;
    ExprNode *left;
    op_t op;
    ExprPostfix(ExprNode *_left, op_t _op) {
        left = _left;
        op = _op;
        assert(left != nullptr);
    }
    Type* resolve_type() override;
    bool is_lvalue() override;
    void elaborate() override;
    void emit_asm() override;
    std::string to_string() override;
};

struct Expression {
    ExprNode *expr_node;
    Expression(ExprNode *_expr_node) {
        expr_node = _expr_node;
    }
    static Expression* convert(parser::expression *e);
    Type* resolve_type();
    bool is_lvalue();
    void elaborate();
    void emit_asm();
    std::string to_string();
};

struct Declaration {
    Type *type;
    Identifier *id;
    Expression *expr;
    Declaration(Type *_type, Identifier *_id, Expression *_expr) {
        type = _type;
        id = _id;
        expr = _expr;
    }
    static Declaration* convert(parser::declaration *d);
    bool is_well_formed();
};

struct Statement {
    static Statement* convert(parser::statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
};

struct SimpleStatement : public Statement {
    static SimpleStatement* convert(parser::simple_statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
};

struct DeclarationStatement : public SimpleStatement {
    Declaration *declaration;
    DeclarationStatement(Declaration *_declaration) {
        declaration = _declaration;
    }
    bool is_well_formed() override;
    bool is_always_returning() override;
};

struct ExpressionStatement : public SimpleStatement {
    Expression *expr;
    ExpressionStatement(Expression *_expr) {
        expr = _expr;
    }
    bool is_well_formed() override;
    bool is_always_returning() override;
};

struct ReturnStatement : public SimpleStatement {
    std::optional<Expression*> opt_expr;
    ReturnStatement(Expression* expr) {
        if(expr == nullptr) opt_expr = std::nullopt;
        else opt_expr = expr;
    }
    bool is_well_formed() override;
    bool is_always_returning() override;
};

struct ControlStatement : public Statement {
    static ControlStatement* convert(parser::control_statement *s);
    virtual bool is_well_formed() = 0;
    virtual bool is_always_returning() = 0;
};

struct IfStatement : public ControlStatement {
    std::vector<Expression*> exprs;
    std::vector<Statement*> statements;
    std::optional<Statement*> else_statement;
    IfStatement(std::vector<Expression*> _exprs, std::vector<Statement*> _statements, Statement *_else_statement) {
        exprs = _exprs;
        statements = _statements;
        if(_else_statement == nullptr) else_statement = std::nullopt;
        else else_statement = _else_statement;
        assert(statements.size() >= 0);
        assert(exprs.size() == statements.size());
    }
    bool is_well_formed() override;
    bool is_always_returning() override;
};

struct WhileStatement : public ControlStatement {
    Expression *expr;
    Statement *statement;
    WhileStatement(Expression *_expr, Statement *_statement) {
        expr = _expr;
        statement = _statement;
    }
    bool is_well_formed() override;
    bool is_always_returning() override;
};

struct ForStatement : public ControlStatement {
    std::optional<Declaration*> declaration;
    std::optional<Expression*> expr1;
    std::optional<Expression*> expr2;
    Statement *statement;
    ForStatement(Declaration *_declaration, Expression *_expr1, Expression *_expr2, Statement *_statement) {
        if(_declaration == nullptr) declaration = std::nullopt;
        else declaration = _declaration;
        if(_expr1 == nullptr) expr1 = std::nullopt;
        else expr1 = _expr1;
        if(_expr2 == nullptr) expr2 = std::nullopt;
        else expr2 = _expr2;
        statement = _statement;
    }
    bool is_well_formed() override;
    bool is_always_returning() override;
};  

struct CompoundStatement : public Statement {
    std::vector<Statement*> statements;
    CompoundStatement(std::vector<Statement*> _statements) {
        statements = _statements;
    }
    static CompoundStatement* convert(parser::compound_statement *s);
    bool is_well_formed();
    bool is_always_returning();
};

//enclosing type, name, and input types. Does not include return type
struct FunctionSignature {
    std::optional<Type*> enclosing_type;
    Identifier *id;
    std::vector<Type*> input_types;

    //global functions
    FunctionSignature(Identifier *_id, std::vector<Type*> _input_types) {   
        enclosing_type = std::nullopt;
        id = _id;
        input_types = _input_types;
    }

    //struct member functions
    FunctionSignature(Type *_enclosing_type, Identifier *_id, std::vector<Type*> _input_types) {    
        assert(_enclosing_type != nullptr);
        enclosing_type = _enclosing_type;
        id = _id;
        input_types = _input_types;
    }

    size_t hash() const {
        size_t h = 0;
        if(enclosing_type.has_value()) hash_combine(h, enclosing_type.value()->hash());
        else hash_combine(h, 0);
        hash_combine(h, std::hash<std::string>()(id->name));
        for(int i = 0; i < input_types.size(); i++) {
            hash_combine(h, input_types[i]->hash());
        }
        return h;
    }
    
    bool operator==(const FunctionSignature& other) const {
        if(enclosing_type.has_value() != other.enclosing_type.has_value()) return false;
        if(enclosing_type.has_value() && *(enclosing_type.value()) != *(other.enclosing_type.value())) return false;
        if(*id != *(other.id)) return false;
        if(input_types.size() != other.input_types.size()) return false;
        for(int i = 0; i < input_types.size(); i++){
            if(*(input_types[i]) != *(other.input_types[i])) return false;
        }
        return true;
    }
    bool operator!=(const FunctionSignature& other) const {
        return !(*this == other);
    }

    std::string to_string() {
        std::string ans = "";
        if(enclosing_type.has_value()) ans += enclosing_type.value()->to_string() + "::";
        ans += id->name;
        ans += "(";
        for(int i = 0; i < input_types.size(); i++) {
            ans += input_types[i]->to_string();
            if(i + 1 != input_types.size()) ans += ", ";
        }
        ans += ")";
        return ans;
    }
};

struct Function {
    struct Parameter {
        Type *type;
        Identifier *id;
        Parameter(Type *_type, Identifier *_id) {
            type = _type;
            id = _id;
        }
        static Parameter* convert(parser::parameter *p);
    };
    
    std::optional<Type*> enclosing_type;
    Type *type;
    Identifier *id;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;

    Function(std::optional<Type*> _enclosing_type, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
        enclosing_type = _enclosing_type;
        id = _id;
        type = _type;
        parameters = _parameters;
        body = _body;

        assert(type != nullptr);
        assert(body != nullptr);
        assert(id != nullptr);
    }

    //use when inputting pre-defined asm functions
    Function(Type *_type, Identifier *_id, std::vector<Type*> input_types) {
        enclosing_type = std::nullopt;
        id = _id;
        type = _type;
        for(int i = 0; i < input_types.size(); i++){
            parameters.push_back(new Parameter(input_types[i], new Identifier("v" + std::to_string(i))));
        }
        body = nullptr;

        assert(type != nullptr);
        assert(id != nullptr);
    }

    virtual ~Function() = default;

    static Function* convert(parser::function *f);
    bool is_well_formed();
    FunctionSignature* resolve_function_signature() const;

    //just checks if the function signature matches
    bool operator==(const Function& other) const {
        return *(resolve_function_signature()) == *(other.resolve_function_signature());
    }
    bool operator!=(const Function& other) const {
        return !(*this == other);
    }
};

struct OperatorOverload : public Function {
    std::string op;
    OperatorOverload(std::string _op, std::optional<Type*> _enclosing_type, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body) : Function(_enclosing_type, _type, _id, _parameters, _body) {
        op = _op;
    }

    OperatorSignature* resolve_operator_signature() const;
};

struct FunctionCall {
    std::optional<Type*> target_type;
    Identifier *id;
    std::vector<Expression*> argument_list;
    FunctionCall(Identifier *_id, std::vector<Expression*> _argument_list) {
        target_type = std::nullopt;
        id = _id;
        argument_list = _argument_list;
    }
    FunctionCall(Type *_target_type, Identifier *_id, std::vector<Expression*> _argument_list) {
        target_type = _target_type;
        id = _id;
        argument_list = _argument_list;
    }
    static FunctionCall* convert(parser::function_call *f);
    Function* get_called_function();
    Type* resolve_type();
    void emit_asm();
};

struct StructDefinition {
    struct MemberVariable {
        Type *type;
        Identifier *id;
        MemberVariable(Type *_type, Identifier *_id) {
            type = _type;
            id = _id;
        }
        static MemberVariable* convert(parser::member_variable_declaration *mvd);
    };

    Type *type;
    std::vector<MemberVariable*> member_variables;
    std::vector<Function*> functions;
    StructDefinition(Type *_type, std::vector<MemberVariable*> _member_variables, std::vector<Function*> _functions) {
        type = _type;
        member_variables = _member_variables;
        functions = _functions;
    }
    static StructDefinition* convert(parser::struct_definition *s);
    bool is_well_formed();
};

struct Program {
    std::vector<StructDefinition*> structs;
    std::vector<Function*> functions;
    Program(std::vector<StructDefinition*> _structs, std::vector<Function*> _functions) {
        structs = _structs;
        functions = _functions;
    }
    static Program* convert(parser::program *p);
    bool is_well_formed();
};

// -- SEMANTIC ANALYSIS + CODE GENERATION CONTROLLER --
std::string indent();
char escape_to_char(parser::escape *e);
std::string create_new_label();
void emit_retrieve_array(int sz);
void emit_address_array(int sz);
void emit_write_array(int sz);
void emit_mem_retrieve(int sz);
void emit_mem_store(int sz);
bool is_declarable(Type *a, Expression* expr);       //does the declaration A foo = expr work?
OperatorImplementation* find_typecast_implementation(Type *from, Type *to);
OperatorImplementation* find_operator_implementation(std::optional<Expression*> left, std::string op, std::optional<Expression*> right);
OperatorImplementation* find_operator_implementation(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right)
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
Function* get_called_function(std::optional<Type*> enclosing_type, Identifier *id, vector<Expression*> args);   //get function that matches, given you can convert each parameter
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
void emit_push(std::string reg);
void emit_pop(std::string reg);
void emit_add_rsp(int amt);
void emit_sub_rsp(int amt);
bool add_operator_implementation(OperatorSignature *os, OperatorImplementation *oi);
bool add_operator_implementation(OperatorOverload *f);
void remove_operator_implementation(OperatorSignature *os, OperatorImplementation *oi);
void remove_operator_implementation(OperatorOverload *f);

struct Variable {
    Type *type;
    Identifier *id;
    int stack_offset;  
    Variable(Type *_type, Identifier *_id) {
        id = _id;
        type = _type;
    }
};

struct OperatorSignature {
    std::optional<Type*> left;
    std::string op;
    std::optional<Type*> right;
    OperatorSignature(Type *_left, std::string _op, Type *_right) { //binary operator or indexing
        assert(_left != nullptr);
        assert(_right != nullptr);
        left = _left;
        op = _op;
        right = _right;
    }
    OperatorSignature(std::string _op, Type *_right) {  //prefix operator
        assert(_right != nullptr);
        left = std::nullopt;
        op = _op;
        right = _right;
    }
    OperatorSignature(Type *_left, std::string _op) {   //postfix operator
        assert(_left != nullptr);
        left = _left;
        op = _op;
        right = std::nullopt;
    }
    OperatorSignature(Type *from, Type *to) {   //casting
        left = from;
        op = "(cast)";
        right = to;
    }

    bool operator==(const OperatorSignature& other) const {
        if(left.has_value() != other.left.has_value()) return false;
        if(left.has_value() && *(left.value()) != *(other.left.value())) return false;
        if(right.has_value() != other.right.has_value()) return false;
        if(right.has_value() && *(right.value()) != *(other.right.value())) return false;
        return op == other.op;
    }

    bool operator!=(const OperatorSignature& other) const {
        return !(*this == other);
    }
};  

namespace std {
    template<>
    struct hash<OperatorSignature*> {
        size_t operator()(const OperatorSignature *key) const {
            size_t seed = 0;
            if(key->left.has_value()) hash_combine(seed, key->left.value()->hash());
            else hash_combine(seed, 0);
            if(key->right.has_value()) hash_combine(seed, key->right.value()->hash());
            else hash_combine(seed, 0);
            hash_combine(seed, std::hash<std::string>()(key->op));
            return seed;
        }
    };
    template<>
    struct equal_to<OperatorSignature*> {
        bool operator()(const OperatorSignature* lhs, const OperatorSignature* rhs) const {
            if (lhs == nullptr || rhs == nullptr) return lhs == rhs;
            return *lhs == *rhs;
        }
    };

    template<>
    struct hash<Type*> {
        size_t operator()(const Type *type) const {
            if(type == nullptr) return 0;
            return type->hash();
        }
    };
    template<>
    struct equal_to<Type*> {
        bool operator()(const Type* lhs, const Type* rhs) const {
            if (lhs == nullptr || rhs == nullptr) return lhs == rhs;
            return lhs->equals(rhs);
        }
    };

    template<>
    struct hash<FunctionSignature*> {
        size_t operator()(const FunctionSignature *fs) const {
            if(fs == nullptr) return 0;
            return fs->hash();
        }
    };
    template<> 
    struct equal_to<FunctionSignature*> {
        bool operator()(const FunctionSignature *lhs, const FunctionSignature *rhs) const { 
            if(lhs == nullptr || rhs == nullptr) return lhs == rhs;
            return *lhs == *rhs;
        }
    };

    template<>
    struct hash<Identifier*> {
        size_t operator()(const Identifier *id) const {
            if(id == nullptr) return 0;
            return id->hash();
        }
    };
    template<>
    struct equal_to<Identifier*> {
        bool operator()(const Identifier *lhs, const Identifier *rhs) const {
            if(lhs == nullptr || rhs == nullptr) return lhs == rhs;
            return *lhs == *rhs;
        }
    };
}

struct OperatorImplementation {
    Type* res_type;

    OperatorImplementation(Type *_res_type) {
        assert(_res_type != nullptr);
        res_type = _res_type;
    }

    virtual ~OperatorImplementation() = default;

    Type* get_res_type() {
        return this->res_type;
    }
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
    BuiltinOperator(Type *_res_type, std::vector<std::string> _instructions) : OperatorImplementation(_res_type) {
        instructions = _instructions;
    }

    void emit_asm();
};

struct FunctionOperator : public OperatorImplementation {
    OperatorOverload *function;
    FunctionOperator(OperatorOverload *_function) : OperatorImplementation((assert(_function != nullptr), _function->type)) {
        function = _function;
    }
};

//holds information on where all the stuff is supposed to go within the heap portion of the struct
// - for every identifier, stores the relative offset within the struct
// - also holds the total size of the heap portion in bytes
struct StructLayout {
    std::vector<StructDefinition::MemberVariable*> member_variables;
    std::unordered_map<Identifier*, int> offset_map;
    int size;
    StructLayout(std::vector<StructDefinition::MemberVariable*> _member_variables, std::unordered_map<Identifier*, int> _offset_map, int _size) {
        member_variables = _member_variables;
        offset_map = _offset_map;
        size = _size;
        assert(member_variables.size() == offset_map.size());
    }

    int get_offset(Identifier *id) {
        assert(offset_map.count(id));
        return offset_map[id];
    }

    Type* get_type(Identifier *id) {
        for(int i = 0; i < member_variables.size(); i++){
            if(*id == *(member_variables[i]->id)) {
                return member_variables[i]->type;
            }
        }
        return nullptr;
    }

    //total size of 'main data' portion of heap struct
    int get_size() {
        return size;
    }

    //offset from start of heap struct to pointer on heap
    int get_ptr_offset() {
        return size;
    }
};

std::ofstream fout("test.asm");

Function* enclosing_function;
std::vector<Type*> declared_types;
std::unordered_set<Type*> primitive_base_types;
std::unordered_map<Type*, StructLayout*> struct_layout_map;
std::vector<Function*> declared_functions;
std::unordered_map<FunctionSignature*, std::string> function_label_map;
std::vector<Variable*> declared_variables;
std::stack<std::vector<Variable*>> declaration_stack;   //every 'layer' of the declaration stack should be contiguous on the stack
std::unordered_map<OperatorSignature*, OperatorImplementation*> conversion_map;

int local_offset;   //tracks the value %rsp - %rbp

//descriptions of whatever is on the stack. 
//to push anything, you need to provide a description
//to pop anything, you need to provide a description, and it will only work if the descriptions match
std::vector<std::string> stack_desc;    

int label_counter;
int tmp_variable_counter;

//adds some helpful (?) comments in the generated asm. 
bool asm_debug = true;

void reset_controller() {
    // - reset semantic controller
    enclosing_function = nullptr;
    declared_types.clear();
    primitive_base_types.clear();
    struct_layout_map.clear();

    declared_functions.clear();
    function_label_map.clear();
    
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
        "inc %rax",
        "incq (%rcx)",
    });
    conversion_map[new OperatorSignature("--", new ReferenceType(p_int))] = new BuiltinOperator(new ReferenceType(p_int), {
        "dec %rax",
        "decq (%rcx)",
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
bool is_declarable(Type *A, Expression *expr) {
    assert(A != nullptr);
    assert(expr != nullptr);

    Type *et = expr->resolve_type();
    bool is_lvalue = expr->is_lvalue();
    if(et == nullptr) return false;

    // if we are assigning to a reference, we must match the type exactly and use a l-value
    if(dynamic_cast<ReferenceType*>(A) != nullptr) {
        if(is_lvalue && *(dynamic_cast<ReferenceType*>(A)->type) == *et) {
            return true;
        }
        return false;
    }

    // otherwise, can look for type conversions
    Expression *expr = new Expression(new ExprBinary(new ExprPrimary(A), "=", new ExprPrimary(expr)));
    if(expr->resolve_type() != nullptr) {
        return true;
    }
    return false;
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
    
    std::vector<OperatorImplementation*> viable;
    for(auto i = conversion_map.begin(); i != conversion_map.end(); i++){
        OperatorSignature *nkey = i->first;
        //does the operator match
        if(key->op != nkey->op) continue;
        //is the left argument convertible
        if(left.has_value() != nkey->left.has_value()) continue;
        if(left.has_value() && !is_declarable(left.value(), nkey->left.value())) continue;
        //is the right argument convertible
        if(key->right.has_value() != nkey->right.has_value()) continue;
        if(key->right.has_value() && !is_declarable(key->right.value(), nkey->right.value())) continue;
        viable.push_back(i->second);
    }
    if(viable.size() != 1) return nullptr;
    return viable[0];
}

OperatorImplementation* find_operator_implementation(std::optional<ExprNode*> left, std::string op, std::optional<ExprNode*> right) {
    if(left.has_value()) assert(left.value() != nullptr);
    if(right.has_value()) assert(right.value() != nullptr);
    std::optional<Expression*> _left = left.has_value() ? new Expression(left.value()) : std::nullopt;
    std::optional<Expression*> _right = right.has_value() ? new Expression(right.value()) : std::nullopt;
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

// - has enclosing type
// - return type equals the enclosing type
// - name of function equals type name
bool is_function_constructor(const Function *f) {
    // FunctionSignature *fs = f->resolve_function_signature();
    return f->enclosing_type.has_value() && *(f->type) == *(f->enclosing_type.value()) && f->id->name == f->type->to_string();
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
        if(*fs == *(declared_functions[i]->resolve_function_signature())) {
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
Function* get_called_function(std::optional<Type*> enclosing_type, Identifier *id, vector<Expression*> args) {
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
        for(int j = 0; j < fs->input_types.size(); j++) {
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
        std::cout << "No matching function for signature : " << fs->to_string() << "\n";
        return nullptr;
    }
    else if(viable.size() > 1) {
        std::cout << "Ambiguous function call : " << fs->to_string() << "\n";
        return nullptr;
    }
    return viable[0];
}

std::string get_function_label(FunctionSignature *fs) {
    assert(function_label_map.count(fs));
    return function_label_map[fs];
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
    std::cout << "ADD FUNCTION : "<< fs->to_string() << std::endl;
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
        StructDefinition::MemberVariable *mv = sl->member_variables[i];
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
void emit_dereference(Type *t) {
    assert(t != nullptr);
    assert(dynamic_cast<ReferenceType*>(t) != nullptr);

    t = dynamic_cast<ReferenceType*>(t)->type;
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

// -- STRUCT FUNCTIONS --
Literal* Literal::convert(parser::literal *l) {
    if(l->is_a0) {  //integer literal
        parser::literal_integer *lit = l->t0->t0;
        return new IntegerLiteral(stoi(lit->to_string()));
    }
    else if(l->is_a1) { //sizeof literal
        parser::literal_sizeof *lit = l->t1->t0;
        Type *t = Type::convert(lit->t4);
        return new SizeofLiteral(t);
    }
    else if(l->is_a2) { //char literal
        char val;
        parser::literal_char *lit = l->t2->t0;
        parser::literal_char::a0 *c = lit->t1;
        if(c->is_b2) {    //escape
            parser::escape *e = c->t2->t0;  
            val = escape_to_char(e);
        }
        else {  //not escape
            val = c->to_string()[0];
        }
        return new CharLiteral(val);
    }
    else if(l->is_a3) { //string literal    
        parser::literal_string *lit = l->t3->t0;
        std::vector<parser::literal_string::a0*> chars = lit->t1;
        std::string val(chars.size(), ' ');
        for(int i = 0; i < chars.size(); i++){
            parser::literal_string::a0* c = chars[i];
            char cchar;
            if(c->is_b2) {  //escape
                parser::escape *e = c->t2->t0;
                cchar = escape_to_char(e);
            }
            else {
                cchar = c->to_string()[0];
            }
            val[i] = cchar;
        }
        return new StringLiteral(val);
    }   
    else assert(false);    
}

Identifier* Identifier::convert(parser::identifier *i) {
    return new Identifier(i->to_string());
}

Type* Type::convert(parser::type *t) {
    Type *res = BaseType::convert(t->t0);
    for(int i = 0; i < t->t1.size(); i++){
        std::string suf = t->t1[i]->to_string();
        if(suf == "*") res = new PointerType(res);
        else if(suf == "&") res = new ReferenceType(res);
        else assert(false);
    }
    return res;
}

BaseType* BaseType::convert(parser::base_type *t) {
    return new BaseType(t->to_string());
}

ExprNode* ExprNode::convert(parser::expr_primary *e) {
    ExprPrimary::val_t val;
    if(e->is_a0) {
        val = Literal::convert(e->t0->t0);
    }
    else if(e->is_a1) {
        val = FunctionCall::convert(e->t1->t0);
    }
    else if(e->is_a2) {
        val = Identifier::convert(e->t2->t0);
    }
    else if(e->is_a3) {
        val = Expression::convert(e->t3->t2);
    }
    else assert(false);
    return new ExprPrimary(val);
}

ExprNode* ExprNode::convert(parser::expr_postfix *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        if(e->t1[i]->t1->is_c0) {       //indexing
            Expression *expr = Expression::convert(e->t1[i]->t1->t0->t2);
            left = new ExprPostfix(left, expr);
        }
        else if(e->t1[i]->t1->is_c1) {  //call member function
            std::string op = ".";
            FunctionCall *fc = FunctionCall::convert(e->t1[i]->t1->t1->t2);
            left = new ExprPostfix(left, std::make_pair(op, fc));
        }
        else if(e->t1[i]->t1->is_c2) {  //dereference, call member function
            std::string op = "->";
            FunctionCall *fc = FunctionCall::convert(e->t1[i]->t1->t2->t2);
            left = new ExprPostfix(left, std::make_pair(op, fc));
        }
        else if(e->t1[i]->t1->is_c3) {  //access member variable
            std::string op = ".";
            Identifier *id = Identifier::convert(e->t1[i]->t1->t3->t2);
            left = new ExprPostfix(left, std::make_pair(op, id));
        }
        else if(e->t1[i]->t1->is_c4) {  //dereference, access member variable
            std::string op = "->";
            Identifier *id = Identifier::convert(e->t1[i]->t1->t4->t2);
            left = new ExprPostfix(left, std::make_pair(op, id));
        }
        else if(e->t1[i]->t1->is_c5) {  //postfix increment
            std::string op = "++";
            left = new ExprPostfix(left, op);
        }
        else if(e->t1[i]->t1->is_c6) {  //postfix decrement
            std::string op = "--";
            left = new ExprPostfix(left, op);
        }
        else assert(false);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_unary *e) {
    if(e->is_a0) {  //unary operator + unary expression
        std::string op = e->t0->t0->to_string();
        ExprNode *right = ExprNode::convert(e->t0->t2);
        return new ExprPrefix(op, right);
    }
    else if(e->is_a1) { //postfix expression
        return ExprNode::convert(e->t1->t0);
    }
    else assert(false);
}

ExprNode* ExprNode::convert(parser::expr_multiplicative *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_additive *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_shift *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_relational *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_equality *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1->to_string();
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_bit_and *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_bit_xor *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_bit_or *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_logical_and *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}   

ExprNode* ExprNode::convert(parser::expr_logical_or *e) {
    ExprNode *left = ExprNode::convert(e->t0);
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = e->t1[i]->t1;
        ExprNode *right = ExprNode::convert(e->t1[i]->t3);
        left = new ExprBinary(left, op, right);
    }
    return left;
}

ExprNode* ExprNode::convert(parser::expr_assignment *e) {
    std::vector<ExprNode*> nodes;
    nodes.push_back(ExprNode::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++) nodes.push_back(ExprNode::convert(e->t1[i]->t3));

    ExprNode *right = nodes[nodes.size() - 1];
    for(int i = (int) nodes.size() - 2; i >= 0; i--){
        std::string op = e->t1[i]->t1->to_string();
        if(op.size() != 1) {    //handle update + assignment operators
            std::string nop = op.substr(0, op.size() - 1);
            right = new ExprBinary(nodes[i], nop, ExprNode::copy(right));
            op = "=";
        }
        right = new ExprBinary(nodes[i], op, right);
    }
    return right;
}

Expression* Expression::convert(parser::expression *e) {
    ExprNode *expr_node = ExprNode::convert(e->t0);
    return new Expression(expr_node);
}

Declaration* Declaration::convert(parser::declaration *d) {
    Type *type = Type::convert(d->t0);
    Identifier *name = Identifier::convert(d->t2);
    Expression *expr = Expression::convert(d->t6);
    return new Declaration(type, name, expr);
}

Statement* Statement::convert(parser::statement *s) {
    if(s->is_a0) {  //simple statement
        return SimpleStatement::convert(s->t0->t0);
    }
    else if(s->is_a1) { //control statement
        return ControlStatement::convert(s->t1->t0);
    }
    else if(s->is_a2) { //compound statement
        return CompoundStatement::convert(s->t2->t0);
    }
    else assert(false);
}

SimpleStatement* SimpleStatement::convert(parser::simple_statement *s) {
    if(s->is_a0) { //return 
        Expression *expr = nullptr;
        if(s->t0->t1 != nullptr) {
            //non-void return
            expr = Expression::convert(s->t0->t1->t1);
        }
        return new ReturnStatement(expr);
    }
    else if(s->is_a1) {  //declaration
        Declaration *declaration = Declaration::convert(s->t1->t0);
        return new DeclarationStatement(declaration);
    }
    else if(s->is_a2) { //expression
        Expression *expr = Expression::convert(s->t2->t0);
        return new ExpressionStatement(expr);
    }
    else assert(false);
}

ControlStatement* ControlStatement::convert(parser::control_statement *s) {
    if(s->is_a0) {  //if statement
        std::vector<Statement*> statements;
        std::vector<Expression*> exprs;
        Statement *else_statement = nullptr;
        parser::control_statement *ptr = s;
        while(true) {
            assert(ptr->is_a0);
            exprs.push_back(Expression::convert(ptr->t0->t4));
            statements.push_back(Statement::convert(ptr->t0->t8));
            if(ptr->t0->t9 == nullptr) break;   //else doesn't exist
            Statement* tmp_stmt = Statement::convert(ptr->t0->t9->t3);
            if(!ptr->t0->t9->t3->is_a1) {  //statement isn't control statement
                else_statement = Statement::convert(ptr->t0->t9->t3);
                break;
            }
            parser::control_statement *nxt_ptr = ptr->t0->t9->t3->t1->t0;
            if(!nxt_ptr->is_a0) {   //statement isn't if statement
                else_statement = Statement::convert(ptr->t0->t9->t3);
                break;
            }
            ptr = nxt_ptr;
        }
        return new IfStatement(exprs, statements, else_statement);
    }
    else if(s->is_a1){  //while statement
        Expression *expr = Expression::convert(s->t1->t4);
        Statement *statement = Statement::convert(s->t1->t8);
        return new WhileStatement(expr, statement);
    }
    else if(s->is_a2) { //for statement
        Declaration *declaration = nullptr;
        if(s->t2->t4 != nullptr) declaration = Declaration::convert(s->t2->t4->t0);
        Expression *expr1 = nullptr;
        if(s->t2->t8 != nullptr) expr1 = Expression::convert(s->t2->t8->t0);
        Expression *expr2 = nullptr;
        if(s->t2->t12 != nullptr) expr2 = Expression::convert(s->t2->t12->t0);
        Statement *statement = Statement::convert(s->t2->t16);
        return new ForStatement(declaration, expr1, expr2, statement);
    }
    else assert(false);
}

CompoundStatement* CompoundStatement::convert(parser::compound_statement *s) {
    std::vector<Statement*> statements;
    std::vector<parser::compound_statement::a0*> slist = s->t2;
    for(int i = 0; i < slist.size(); i++){
        statements.push_back(Statement::convert(slist[i]->t0));
    }
    return new CompoundStatement(statements);
}

Function::Parameter* Function::Parameter::convert(parser::parameter *p) {
    Type *type = Type::convert(p->t0);
    Identifier* name = Identifier::convert(p->t2);
    return new Parameter(type, name);
}

Function* Function::convert(parser::function *f) {
    parser::function_definition *def = f->t0;
    parser::parameter_list *pl = def->t6;
    Type *type = Type::convert(def->t0);
    Identifier *name = new Identifier(def->t2->to_string());
    std::vector<Function::Parameter*> parameters;
    if(pl->t0 != nullptr) {
        parameters.push_back(Function::Parameter::convert(pl->t0->t0));
        std::vector<parser::parameter_list::a0::b0*> tmp = pl->t0->t1;
        for(int i = 0; i < tmp.size(); i++){
            parameters.push_back(Function::Parameter::convert(tmp[i]->t3));
        }
    }
    CompoundStatement *body = CompoundStatement::convert(f->t2);
    if(def->t2->is_a1) {
        //operator overload
        std::string op = def->t2->t1->t0->t1->to_string();
        return new OperatorOverload(op, std::nullopt, type, name, parameters, body);
    }
    else {
        //regular function
        return new Function(std::nullopt, type, name, parameters, body);
    }
}

FunctionCall* FunctionCall::convert(parser::function_call *f) {
    Identifier *fname = Identifier::convert(f->t0);
    std::vector<Expression*> argument_list;
    parser::argument_list *arglist = f->t4;
    if(arglist->t0 != nullptr) {
        argument_list.push_back(Expression::convert(arglist->t0->t0));
        for(int i = 0; i < arglist->t0->t1.size(); i++){
            argument_list.push_back(Expression::convert(arglist->t0->t1[i]->t3));
        }
    }
    return new FunctionCall(fname, argument_list);
}

StructDefinition::MemberVariable* StructDefinition::MemberVariable::convert(parser::member_variable_declaration *mvd) {
    Type *type = Type::convert(mvd->t0);
    Identifier *id = Identifier::convert(mvd->t2);
    return new StructDefinition::MemberVariable(type, id);
}

StructDefinition* StructDefinition::convert(parser::struct_definition *s) {
    Type *type = BaseType::convert(s->t2);
    std::vector<StructDefinition::MemberVariable*> member_variables;
    std::vector<Function*> functions;
    for(int i = 0; i < s->t6.size(); i++){
        if(s->t6[i]->t0->is_c0) {   //member variable declaration
            member_variables.push_back(StructDefinition::MemberVariable::convert(s->t6[i]->t0->t0->t0));
        }
        else if(s->t6[i]->t0->is_c1) {  //function
            Function *f = Function::convert(s->t6[i]->t0->t1->t0);
            f->enclosing_type = type;
            functions.push_back(f);
        }
        else assert(false);
    }
    return new StructDefinition(type, member_variables, functions);
}

Program* Program::convert(parser::program *p) {
    std::vector<StructDefinition*> structs;
    std::vector<Function*> functions;
    for(int i = 0; i < p->t0.size(); i++){
        if(p->t0[i]->t1->is_c0) {   //function
            functions.push_back(Function::convert(p->t0[i]->t1->t0->t0));
        }
        else if(p->t0[i]->t1->is_c1) {  //struct definition
            structs.push_back(StructDefinition::convert(p->t0[i]->t1->t1->t0));
        }
        else assert(false);
    }
    return new Program(structs, functions);
}

Type* IntegerLiteral::resolve_type() {
    return new BaseType("int");
}

Type* SizeofLiteral::resolve_type() {
    return new BaseType("int");
}

Type* CharLiteral::resolve_type() {
    return new BaseType("char");
}

Type* StringLiteral::resolve_type() {
    return new PointerType(new BaseType("char"));
}

std::string ExprPrimary::to_string() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        return fc->resolve_function_signature()->to_string();
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        return id->name;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return "<lit>";
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return "(" + e->to_string() + ")";
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *t = std::get<Type*>(val);
        return t->to_string();
    }
    else assert(false);
}

std::string ExprBinary::to_string() {
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        return left->to_string() + " " + str_op + " " + right->to_string();
    }
    else assert(false);
}

std::string ExprPrefix::to_string() {
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        return str_op + right->to_string();
    }
    else assert(false);
}

std::string ExprPostfix::to_string() {
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        return left->to_string() + "[" + expr->to_string() + "]";
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        return left->to_string() + p.first + p.second->resolve_function_signature()->to_string();
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        return left->to_string() + p.first + p.second->name;
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        std::string str_op = std::get<std::string>(op);
        return left->to_string() + str_op;
    }
    else assert(false);
}

std::string Expression::to_string() {
    return expr_node->to_string();
}

bool ExprPrimary::is_lvalue() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *fc = std::get<FunctionCall*>(val);
        Type *t = fc->resolve_type();
        if(t == nullptr) return false;
        //if return type is a reference, it gets auto-dereferenced. 
        if(dynamic_cast<ReferenceType*>(t) != nullptr) return true;
        return false;
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        return true;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        return false;
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->is_lvalue();
    }
    else if(std::holds_alternative<Type*>(val)) {
        return true;
    }
    else assert(false);
}

bool ExprBinary::is_lvalue() {
    Type *lt = left->resolve_type(), *rt = right->resolve_type();
    if(lt == nullptr || rt == nullptr) return false;
    //only overloads that return a reference can return a l-value
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        Type *otype = find_resulting_type(left, str_op, right);
        if(otype != nullptr) {
            if(dynamic_cast<ReferenceType*>(otype) != nullptr) {
                return true;
            }
        }
    }   
    else assert(false);
    return false;
}

bool ExprPrefix::is_lvalue() {
    Type *rt = right->resolve_type();
    if(!rt->is_lvalue()) return false;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);


        if(str_op == "++" || str_op == "--" || str_op == "*") {
            return true;
        }
    }
    return false; 
}

bool ExprPostfix::is_lvalue() {
    Type *lt = left->resolve_type();
    if(lt == nullptr) return false;
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        return true;
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {    //function call
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        FunctionCall *fc = p.second;  

        //dereference
        if(p.first == "->") {
            if(dynamic_cast<PointerType*>(lt) == nullptr) {
                return false;
            }
            lt = dynamic_cast<PointerType*>(lt)->type;
        }

        //member function call
        fc = new FunctionCall(lt, fc->id, fc->argument_list);
        Type *nt = fc->resolve_type();
        if(fc->resolve_type() == nullptr) {
            return false;
        }

        //if the resulting type is a reference, this is an l-value
        if(dynamic_cast<ReferenceType*>(nt)) return true;
        return false;
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {    //member variable access
        return true;
    }
    else if(std::holds_alternative<std::string>(op)) {  //postfix increment / decrement
        return false;
    }
    else assert(false);
}

bool Expression::is_lvalue() {
    return expr_node->is_lvalue();
}

Type* ExprPrimary::resolve_type() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *f = std::get<FunctionCall*>(val);
        Type *res = f->resolve_type();
        if(res == nullptr) {
            std::cout << "Function call does not resolve to type : " << f->resolve_function_signature()->to_string() << "\n";
            return nullptr;
        }

        //if this is a reference, automatically dereference it
        if(dynamic_cast<ReferenceType*>(res)) {
            res = dynamic_cast<ReferenceType*>(res)->type;
        }
        return res;
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        Type *res = find_variable_type(id);
        if(res == nullptr) {
            std::cout << "Unable to find variable : " << id->name << "\n";
            return nullptr;
        }

        //if this is a reference, automatically dereference it
        if(dynamic_cast<ReferenceType*>(res)) {
            res = dynamic_cast<ReferenceType*>(res)->type;
        }
        return res;
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return l->resolve_type();
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->resolve_type();
    }
    else if(std::holds_alternative<Type*>(val)) {
        Type *res = std::get<Type*>(val);

        //if this is a reference, automatically dereference it
        if(dynamic_cast<ReferenceType*>(res)) {
            res = dynamic_cast<ReferenceType*>(res)->type;
        }
        return res;
    }
    else assert(false);
}

Type* ExprBinary::resolve_type() {
    Type *lt = left->resolve_type(), *rt = right->resolve_type();
    if(lt == nullptr || rt == nullptr) return nullptr;    
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //try to find overload / builtin
        Type *otype = find_resulting_type(left, str_op, right);
        if(otype != nullptr) {
            return otype
        }

        //default behaviour
        if(str_op == "=") {
            if(!left->is_lvalue()) {
                std::cout << "Cannot assign to r-value\n";
                return nullptr;
            }
            OperatorImplementation *oe = find_typecast_implementation(rt, lt);
            if(oe == nullptr) {
                std::cout << "No direct conversion from " << rt->to_string() << " to " << lt->to_string() << "\n";
                return nullptr;
            }
            return res;
        }

        std::cout << "Binary operator " << lt->to_string() << " " << str_op << " " << rt->to_string() << " does not exist\n";
        return nullptr;
    }   
    else assert(false);
}

Type* ExprPrefix::resolve_type() {
    Type *rt = right->resolve_type();
    if(rt == nullptr) return nullptr;
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //try to find overload / builtin
        Type *otype = find_resulting_type(std::nullopt, str_op, right);
        if(otype != nullptr) {
            return otype;
        }

        //default behaviour
        if(str_op == "*") {
            Type *res;
            if(dynamic_cast<PointerType*>(rt)) res = dynamic_cast<PointerType*>(rt)->type;
            else if(dynamic_cast<ReferenceType*>(rt)) res = dynamic_cast<ReferenceType*>(rt)->type;
            else {
                std::cout << "Cannot dereference non-reference type " << rt->to_string() << "\n";
                return nullptr;
            }
            return res;
        }

        std::cout << "Prefix operator " << str_op << " " << rt->to_string() << " does not exist\n";
        return nullptr;
    }
    else assert(false);
}

Type* ExprPostfix::resolve_type() {
    Type *lt = left->resolve_type();
    if(lt == nullptr) return nullptr;
    if(std::holds_alternative<Expression*>(op)) {   //indexing
        Expression *expr = std::get<Expression*>(op);
        Type *et = expr->resolve_type();
        if(et == nullptr) return nullptr;

        //try to find overload
        Type *otype = find_resulting_type(left, "[]", expr->expr_node);
        if(otype != nullptr) {
            return otype;
        }

        //default behaviour
        if(*et != BaseType("int")) {
            std::cout << "Builtin indexing expression must resolve to int\n";
            return nullptr;
        }
        if(dynamic_cast<PointerType*>(lt) == nullptr) {
            std::cout << "Can't index into non-pointer type " << lt->to_string() << "\n";
            return nullptr;
        }
        Type *nt = dynamic_cast<PointerType*>(lt)->type;
        return nt;
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        FunctionCall *fc = p.second;  

        //dereference
        if(p.first == "->") {
            if(dynamic_cast<PointerType*>(lt) == nullptr) {
                std::cout << "Trying to dereference non-pointer type " << lt->to_string() << "\n";
                return nullptr;
            }
            lt = dynamic_cast<PointerType*>(lt)->type;
        }

        //member function call
        fc = new FunctionCall(lt, fc->id, fc->argument_list);
        Type *nt = fc->resolve_type();
        if(fc->resolve_type() == nullptr) {
            std::cout << "Failed to resolve type of function call\n";
            return nullptr;
        }

        //if resulting type is a reference, should dereference
        if(dynamic_cast<ReferenceType*>(nt)) {
            nt = dynamic_cast<ReferenceType*>(nt)->type;
        }

        return nt;
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        Identifier *id = p.second;
        
        //dereference
        if(p.first == "->") {
            if(dynamic_cast<PointerType*>(lt) == nullptr) {
                std::cout << "Trying to dereference non-pointer type " << lt->to_string() << "\n";
                return nullptr;
            }
            lt = dynamic_cast<PointerType*>(lt)->type;
        }

        //member variable access
        StructLayout *sl = get_struct_layout(lt);
        if(sl == nullptr) {
            std::cout << "Unable to find StructLayout for " << lt->to_string() << "\n";
            return nullptr;
        }
        if(sl->get_type(id) == nullptr) {
            std::cout << "Unknown member variable identifier \"" << id->name << "\" for type " << lt->to_string() << "\n";
            return nullptr;
        }
        Type *nt = sl->get_type(id);

        //if resulting type is a reference, should dereference
        if(dynamic_cast<ReferenceType*>(nt)) {
            nt = dynamic_cast<ReferenceType*>(nt)->type;
        }

        return nt;
    }
    else if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);

        //try to find overload / builtin
        Type *res = find_resulting_type(left, str_op, std::nullopt);
        if(res == nullptr) {
            std::cout << "Postfix operator " << lt->to_string() << " " << str_op << " does not exist\n";
            return nullptr;
        }
        return res;
    }
    else assert(false);
}

Type* Expression::resolve_type() {
    return expr_node->resolve_type();
}

Type* FunctionCall::resolve_type() {
    Function *f = this->get_called_function();
    if(f == nullptr) {
        std::cout << "Cannot resolve function call : " << fs->to_string() << "\n";
        return nullptr;
    }
    return f->type;
}

Function* FunctionCall::get_called_function() {
    return get_called_function(this->target_type, this->id, this->argument_list);
}

FunctionSignature* Function::resolve_function_signature() const {
    std::vector<Type*> input_types;
    for(int i = 0; i < parameters.size(); i++) input_types.push_back(parameters[i]->type);
    if(enclosing_type.has_value() && !is_function_constructor(this)) return new FunctionSignature(enclosing_type.value(), id, input_types);
    return new FunctionSignature(id, input_types);
}

OperatorSignature* OperatorOverload::resolve_operator_signature() const {
    if( op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || 
        op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>" ||
        op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || 
        op == ">=" || op == "=") {
        //binary operator
        if(this->parameters.size() != 2) return nullptr;
        Type *left = this->parameters[0]->type, *right = this->parameters[1]->type;
        return new OperatorSignature(left, op, right);
    }
    else if(op == "++x" || op == "--x" || op == "*x" || op == "(cast)") {
        //prefix operator
        if(this->parameters.size() != 1) return nullptr;
        Type *right = this->parameters[0]->type;
        if(op == "++x") return new OperatorSignature("++", right);
        else if(op == "--x") return new OperatorSignature("--", right);
        else if(op == "*x") return new OperatorSignature("*x", right);
        else if(op == "(cast)") return new OperatorSignature(right, this->type);
        else assert(false);
    }
    else if(op == "x++" || op == "x--") {
        //postfix operator
        if(this->parameters.size() != 1) return nullptr;
        Type *left = this->parameters[0]->type;
        if(op == "x++") return new OperatorSignature(left, "++");
        else if(op == "x--") return new OperatorSignature(left, "--");
        else assert(false);
    }
    else if(op == "[]") {
        //indexing
        if(this->parameters.size() != 2) return nullptr;
        Type *left = this->parameters[0]->type, *et = this->parameters[1]->type;
        return new OperatorSignature(left, "[]", et)
    }
    else assert(false);
}

bool DeclarationStatement::is_always_returning() {
    return false;
}

bool ExpressionStatement::is_always_returning() {
    return false;
}

bool ReturnStatement::is_always_returning() {
    return true;
}

bool IfStatement::is_always_returning() {
    //every statement must return 
    for(int i = 0; i < statements.size(); i++){
        if(!statements[i]->is_always_returning()) return false;
    }
    if(else_statement.has_value() && !else_statement.value()->is_always_returning()) return false;
    return true;
}

bool WhileStatement::is_always_returning() {
    return statement->is_always_returning();
}

bool ForStatement::is_always_returning() {
    return statement->is_always_returning();
}

bool CompoundStatement::is_always_returning() {
    //for now, don't worry about unreachable code
    for(int i = 0; i < statements.size(); i++){
        if(statements[i]->is_always_returning()) return true;
    }
    return false;
}

void BuiltinOperator::emit_asm() {
    for(int i = 0; i < instructions.size(); i++){
        fout << indent() << instructions[i] << "\n";
    }
}

void IntegerLiteral::emit_asm() {
    fout << indent() << "mov $" << val << ", %rax\n";
}

void SizeofLiteral::emit_asm() {
    int sz = type->calc_size();
    fout << indent() << "mov $" << sz << ", %rax\n";
}

void CharLiteral::emit_asm() {
    fout << indent() << "movb $" << (int) val << ", %al\n";
}

void StringLiteral::emit_asm() {
    //allocate memory
    emit_malloc(val.size() + 1);

    //save start of string
    emit_push("%rax", "StringLiteral::emit_asm() : string start");

    //string index pointer to %rbx
    fout << indent() << "mov %rax, %rbx\n";

    //populate memory
    for(int i = 0; i < val.size(); i++){
        fout << indent() << "movb $" << (int) val[i] << ", %al\n";
        emit_mem_store(1);
        fout << indent() << "inc %rbx\n";
    }
    fout << indent() << "movb $0, %al\n";
    emit_mem_store(1);

    //retrieve start of string
    emit_pop("%rax", "StringLiteral::emit_asm() : string start");
}

ExprNode* ExprNode::copy(ExprNode *other) {
    if(auto e = dynamic_cast<ExprPrimary*>(other)) {
        return new ExprPrimary(e->val);
    }
    else if(auto e = dynamic_cast<ExprBinary*>(other)) {
        return new ExprBinary(ExprNode::copy(e->left), e->op, ExprNode::copy(e->right));
    }
    else if(auto e = dynamic_cast<ExprPrefix*>(other)) {
        return new ExprPrefix(e->op, ExprNode::copy(e->right));
    }
    else if(auto e = dynamic_cast<ExprPostfix*>(other)) {
        return new ExprPostfix(ExprNode::copy(e->left), e->op);
    }
    else assert(false);
}

void ExprPrimary::elaborate() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        //do nothing
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        //do nothing
    }
    else if(std::holds_alternative<Literal*>(val)) {
        //do nothing
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *expr = std::get<Expression*>(val);
        expr->elaborate();
    }
    else if(std::holds_alternative<Type*>(val)) {
        //do nothing
    }
    else assert(false);
}

void ExprBinary::elaborate() {
    left->elaborate();
    right->elaborate();

    //wrap assignment of non-primitive l-values in copy constructor call
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        if(str_op == "=") {
            Type *rt = right->resolve_type();
            if(!is_type_primitive(rt) && right->is_lvalue()) {
                FunctionCall *copy = new FunctionCall(new Identifier(rt->to_string()), {new Expression(right)});
                right = new ExprPrimary(copy);
            }
        }
    }
}

void ExprPrefix::elaborate() {
    //when I implement casting, should turn casts into function calls
    right->elaborate();
}

void ExprPostfix::elaborate() { 
    left->elaborate();
}

void Expression::elaborate() {
    expr_node->elaborate();
}

void ExprPrimary::emit_asm() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *f = std::get<FunctionCall*>(val);
        f->emit_asm();

        Type *ft = f->resolve_type();
        assert(ft != nullptr);
        
        //if this is a reference, dereference it
        if(dynamic_cast<ReferenceType*>(ft)) {
            emit_dereference(ft);
        }
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        Variable *v = get_variable(id);
        assert(v != nullptr);

        fout << indent() << "mov " << v->stack_offset << "(%rbp), %rax\n";  //value
        fout << indent() << "lea " << v->stack_offset << "(%rbp), %rcx\n";  //address

        //if this is a reference, dereference it
        Type *vt = v->type;
        if(dynamic_cast<ReferenceType*>(vt)) {
            emit_dereference(vt);
        }
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        l->emit_asm();
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        e->emit_asm();
    }
    else if(std::holds_alternative<Type*>(val)) {
        assert(false);  //Type* is just for checking if an expression resolves to something. 
    }
    else assert(false);
}

void ExprBinary::emit_asm() {
    Type *lt = left->resolve_type(), *rt = right->resolve_type();
    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        if(str_op == "||") {    //logical or
            std::string label = create_new_label();
            left->emit_asm();

            //see if we should short circuit
            fout << indent() << "cmp $0, %rax\n";
            fout << indent() << "jne " << label << "\n";

            //if we don't short circuit, evaluate right branch and merge
            {
                emit_push("%rax", "ExprBinary::emit_asm() : || save left");
                right->emit_asm();
                fout << indent() << "mov %rax, %rbx\n";
                emit_pop("%rax", "ExprBinary::emit_asm() : || save left");

                OperatorImplementation *oe = find_operator_implementation(lt, str_op, rt);
                assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
                dynamic_cast<BuiltinOperator*>(oe)->emit_asm();
            }

            fout << label << ":\n";
        }
        else if(str_op == "&&") {   //logical and
            std::string label = create_new_label();
            left->emit_asm();

            //see if we should short circuit
            fout << indent() << "cmp $0, %rax\n";
            fout << indent() << "je " << label << "\n";

            //if we don't short circuit, evaluate right branch and merge
            {
                emit_push("%rax", "ExprBinary::emit_asm() : && save left");
                right->emit_asm();
                fout << indent() << "mov %rax, %rbx\n";
                emit_pop("%rax", "ExprBinary::emit_asm() : && save left");

                OperatorImplementation *oe = find_operator_implementation(lt, str_op, rt);
                assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
                dynamic_cast<BuiltinOperator*>(oe)->emit_asm();
            }

            fout << label << ":\n";
        }
        else if(str_op == "=") {
            //eval right r-value
            right->emit_asm();

            //cast right into left
            OperatorImplementation *oe = find_typecast_implementation(rt, lt);
            assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
            dynamic_cast<BuiltinOperator*>(oe)->emit_asm();

            //save value
            emit_push("%rax", "ExprBinary::emit_asm() : = save left");

            //evaluate left l-value
            left->emit_asm();

            //move value into mem location
            int sz = lt->calc_size();
            fout << indent() << "mov %rcx, %rbx\n";
            emit_pop("%rax", "ExprBinary::emit_asm() : = save left");
            emit_mem_store(sz);
        }
        else {
            left->emit_asm();
            emit_push("%rax", "ExprBinary::emit_asm() : save left");
            right->emit_asm();
            fout << indent() << "mov %rax, %rbx\n";
            emit_pop("%rax", "ExprBinary::emit_asm() : save left");
            
            OperatorImplementation *oe = find_operator_implementation(lt, str_op, rt);
            assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
            dynamic_cast<BuiltinOperator*>(oe)->emit_asm();
        }
    }
    else assert(false);
}

void ExprPrefix::emit_asm() {
    Type *rt = right->resolve_type();
    right->emit_asm();

    if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        assert(str_op != "(cast)"); //these should be handled seperately
        if(str_op == "*"){ 
            if(dynamic_cast<PointerType*>(rt)) rt = dynamic_cast<PointerType*>(rt)->type;
            else if(dynamic_cast<ReferenceType*>(rt)) rt = dynamic_cast<ReferenceType*>(rt)->type;
            else assert(false);
            
            int sz = rt->calc_size();
            fout << indent() << "mov %rax, %rcx\n";
            emit_mem_retrieve(sz); 
        }
        else {
            OperatorImplementation *oe = find_operator_implementation(std::nullopt, str_op, rt);
            assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
            dynamic_cast<BuiltinOperator*>(oe)->emit_asm();
        }
    }
    else assert(false);
}

void ExprPostfix::emit_asm() {
    Type *lt = left->resolve_type();

    //evaluate left expr
    left->emit_asm();

    if(std::holds_alternative<Expression*>(op)) {   //indexing
        //t must be PointerType
        assert(dynamic_cast<PointerType*>(lt) != nullptr);
        lt = (dynamic_cast<PointerType*>(lt))->type;

        //save %rax, %rcx to stack
        emit_push("%rax", "ExprPostfix::emit_asm() : [] %rax 1");
        emit_push("%rcx", "ExprPostfix::emit_asm() : [] %rcx 1");

        //evaluate expression
        Expression *expr = std::get<Expression*>(op);
        assert(expr != nullptr);
        expr->emit_asm();

        //move expression value to %rbx
        fout << indent() << "mov %rax, %rbx\n";

        //return old %rax, %rcx
        emit_pop("%rcx", "ExprPostfix::emit_asm() : [] %rcx 1");
        emit_pop("%rax", "ExprPostfix::emit_asm() : [] %rax 1");

        //save array start
        emit_push("%rax", "ExprPostfix::emit_asm() : [] %rax 2");

        //find sizeof struct
        int sz = lt->calc_size();

        //compute array element addr
        emit_address_array(sz);
        fout << indent() << "mov %rax, %rcx\n";

        //return array start
        emit_pop("%rax", "ExprPostfix::emit_asm() : [] %rax 2");

        //retrieve array element data
        emit_retrieve_array(sz);
    }
    else if(std::holds_alternative<std::pair<std::string, FunctionCall*>>(op)) {
        std::pair<std::string, FunctionCall*> p = std::get<std::pair<std::string, FunctionCall*>>(op);
        FunctionCall *fc = p.second;

        //dereference
        if(p.first == "->") {
            fout << indent() << "mov %rax, %rcx\n";
            fout << indent() << "movq (%rax), %rax\n";

            assert(dynamic_cast<PointerType*>(lt) != nullptr);
            lt = dynamic_cast<PointerType*>(lt)->type;
            assert(lt != nullptr);
        }

        //member function call
        fc = new FunctionCall(lt, fc->id, fc->argument_list);
        Function *f = fc->get_called_function();
        assert(f != nullptr);

        //this is no longer l-value, so don't have to maintain %rcx
        //reference to type is in %rax, so emit function call
        fc->emit_asm();

        //if the return value is a reference, auto-dereference it
        Type *ft = f->type;
        if(dynamic_cast<ReferenceType*>(ft)) {
            emit_dereference(ft);
        }
    }
    else if(std::holds_alternative<std::pair<std::string, Identifier*>>(op)) {
        std::pair<std::string, Identifier*> p = std::get<std::pair<std::string, Identifier*>>(op);
        Identifier *id = p.second;

        //dereference
        if(p.first == "->") {
            fout << indent() << "mov %rax, %rcx\n";
            fout << indent() << "movq (%rax), %rax\n";

            assert(dynamic_cast<PointerType*>(lt) != nullptr);
            lt = dynamic_cast<PointerType*>(lt)->type;
            assert(lt != nullptr);
        }

        //member variable access
        StructLayout *sl = get_struct_layout(lt);
        assert(sl != nullptr);
        assert(sl->get_type(id) != nullptr);

        int offset = sl->get_offset(id);
        int sz = lt->calc_size();
        fout << indent() << "lea " << offset << "(%rax), %rcx\n";
        fout << indent() << "lea " << offset << "(%rax), %rax\n";
        emit_mem_retrieve(sz);  //now, %rax = member variable, %rcx = address of member variable

        //if the return value is a reference, auto-dereference it
        Type *vt = sl->get_type(id);
        if(dynamic_cast<ReferenceType*>(vt)) {
            emit_dereference(vt);
        }
    }
    else if(std::holds_alternative<std::string>(op)) {
        std::string str_op = std::get<std::string>(op);
        OperatorImplementation *oe = find_operator_implementation(lt, str_op, std::nullopt);
        assert(dynamic_cast<BuiltinOperator*>(oe) != nullptr);
        dynamic_cast<BuiltinOperator*>(oe)->emit_asm();
    }
    else assert(false);
}

void Expression::emit_asm() {
    expr_node->elaborate();
    expr_node->emit_asm();
}

void FunctionCall::emit_asm() {
    if(asm_debug) fout << indent() << "# calling function : " << id->name << "\n";
    
    //find original function
    FunctionSignature *fs = resolve_function_signature();
    Function *f = this->get_called_function();
    assert(f != nullptr);
    bool is_constructor = is_function_constructor(f);

    //if it is a constructor, create a new instance of the object
    if(is_constructor) {  
        Type *t = f->type;
        emit_initialize_struct(t);

        //save reference to type as return value
        emit_push("%rbx", "FunctionCall::emit_asm() : constructor");

        //put reference into %rax
        fout << indent() << "mov %rbx, %rax\n";
    }
    
    //if is member function, pass in target struct as an argument
    //expects the target struct to be in %rax
    if(target_type.has_value() || is_constructor) {  //member function
        emit_push("%rax", "FunctionCall::emit_asm() : target struct");
    }

    //create temp variables for all arguments
    push_declaration_stack();
    assert(f->parameters.size() == argument_list.size());
    for(int i = 0; i < argument_list.size(); i++){
        //identifiers cannot begin with digits, so this shouldn't collide with anything else
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Variable *v = emit_initialize_variable(f->parameters[i]->type, id, argument_list[i]);
        assert(v != nullptr);
    }

    //call function
    std::string label = get_function_label(f->resolve_function_signature());
    fout << indent() << "call " << label << "\n";

    //clean up argument temp variables
    pop_declaration_stack();

    //clean up target struct argument
    if(target_type.has_value() || is_constructor) {
        emit_add_rsp(8, "FunctionCall::emit_asm() : target struct");
    }

    //if is a constructor, should return reference to type
    if(is_constructor) {
        emit_pop("%rax", "FunctionCall::emit_asm() : constructor");
    }
}

bool Declaration::is_well_formed() {
    if(asm_debug) fout << indent() << "# initialize local variable : " << type->to_string() << " " << id->name << "\n";
    Variable *v = emit_initialize_variable(type, id, expr);
    if(asm_debug) fout << indent() << "# done initialize local variable : " << type->to_string() << " " << id->name << "\n";

    if(v == nullptr) {
        return false;
    }

    return true;
}

bool DeclarationStatement::is_well_formed() {
    // - is the declaration well formed?
    if(!declaration->is_well_formed()) {
        std::cout << "Declaration not well formed\n";
        return false;
    }
    return true;
}

bool ExpressionStatement::is_well_formed() {
    // - does the expression resolve to a type?
    Type *t = expr->resolve_type();
    if(t == nullptr) {
        std::cout << "Expression does not resolve to type\n";
        return false;
    }

    // - does the type actually exist?
    if(!is_type_declared(t)) {
        std::cout << "Type " << t->to_string() << " does not exist\n";
        return false;
    }

    expr->emit_asm();

    return true;
}

bool ReturnStatement::is_well_formed() {
    // - does the expression resolve to a type?
    Type *et = nullptr, *ft = enclosing_function->type;
    if(opt_expr.has_value()) {
        Expression *expr = opt_expr.value();
        et = expr->resolve_type();
    }
    else et = new BaseType("void");
    if(et == nullptr) {
        std::cout << "Return expression does not resolve to type\n";
        return false;
    }
    // - are we trying to return something when the function is returning void?
    if(*ft == BaseType("void") && *et != BaseType("void")) {
        std::cout << "Non-void return expression in void function\n";
        return false;
    }
    
    //see if we need to return something
    if(*ft != BaseType("void")) {
        assert(opt_expr.has_value());
        Expression *expr = opt_expr.value();
        Identifier *vid = new Identifier(create_new_tmp_variable_name());
        push_declaration_stack();

        // - can the expression return type be assigned to the return type of enclosing function?
        Variable *v = emit_initialize_variable(ft, vid, expr);
        if(v == nullptr) {
            std::cout << "Return expression cannot be cast to function return type, " << et->to_string() << " -> " << ft->to_string() << "\n";
            return false;
        }

        //put value of declared variable into %rax
        fout << indent() << "mov " << v->stack_offset << "(%rbp), %rax\n";

        //clean up temp variable
        pop_declaration_stack();
    }

    //clean up local variables
    if(declared_variables.size() != 0) {
        fout << indent() << "add $" << -local_offset << ", %rsp\n"; //should not be managed by local_offset
    }

    if(FunctionSignature(new Identifier("main"), {}) == *(enclosing_function->resolve_function_signature())) {
        //function is main, use exit syscall instead
        fout << indent() << "push %rax\n";  //should not be managed by local_offset
        fout << indent() << "call sys_exit\n";
    }
    else {
        //return from function
        fout << indent() << "pop %rbp\n";   //should not be managed by local_offset
        fout << indent() << "ret\n";
    }

    return true;
}

bool IfStatement::is_well_formed() {
    // - do all expressions resolve to type 'int'?
    for(int i = 0; i < exprs.size(); i++){
        Type *t = exprs[i]->resolve_type();
        if(t == nullptr || *t != BaseType("int")) {
            return false;
        }
    }

    if(asm_debug) fout << indent() << "# if statement start\n";
    std::vector<std::string> labels;
    for(int i = 0; i < statements.size(); i++){
        labels.push_back(create_new_label());
    }
    std::string else_label = "no-label";
    if(else_statement.has_value()) else_label = create_new_label();
    std::string end_if_label = create_new_label();

    //check each expression one by one to see where we should jump to
    for(int i = 0; i < exprs.size(); i++){
        exprs[i]->emit_asm();
        fout << indent() << "cmp $0, %rax\n";
        fout << indent() << "jne " << labels[i] << "\n";
    }
    if(else_statement.has_value()) fout << indent() << "jmp " << else_label << "\n";
    else fout << indent() << "jmp " << end_if_label << "\n";

    // - are all of the statements well formed?
    for(int i = 0; i < statements.size(); i++){
        fout << labels[i] << ":\n";
        if(!statements[i]->is_well_formed()) {
            return false;
        }
        fout << indent() << "jmp " << end_if_label << "\n";
    }

    // - is else statement well formed?
    if(else_statement.has_value()) {
        fout << else_label << ":\n";
        if(!else_statement.value()->is_well_formed()) {
            return false;
        }
        fout << indent() << "jmp " << end_if_label << "\n";
    }

    fout << end_if_label << ":\n";
    if(asm_debug) fout << indent() << "# if statement end\n";

    return true;
}

bool WhileStatement::is_well_formed() {
    // - does the expression resolve to type 'int'?
    Type *t = expr->resolve_type();
    if(t == nullptr || *t != BaseType("int")) {
        return false;
    }

    if(asm_debug) fout << indent() << "# while loop start\n";

    std::string loop_start_label = create_new_label();
    std::string loop_end_label = create_new_label();
    fout << loop_start_label << ":\n";

    //check loop condition
    expr->emit_asm();
    fout << indent() << "cmp $0, %rax\n";
    fout << indent() << "je " << loop_end_label << "\n";

    // - is the statement well formed?
    if(!statement->is_well_formed()) {
        return false;
    }

    //jump to start of loop
    fout << indent() << "jmp " << loop_start_label << "\n";

    //end of loop
    fout << loop_end_label << ":\n";

    if(asm_debug) fout << indent() << "# while loop end\n";

    return true;
}

bool ForStatement::is_well_formed() {
    push_declaration_stack();

    if(asm_debug) fout << indent() << "# for loop start\n";

    // - is declaration well formed?   
    if(declaration.has_value()) {
        if(!declaration.value()->is_well_formed()) {
            return false;
        }
    }

    // - does the conditional expression resolve to type 'int'?
    if(expr1.has_value()) {
        Type *t = expr1.value()->resolve_type();
        if(t == nullptr || *t != BaseType("int")) {
            std::cout << "For loop conditional expression must resolve to type int\n";
            return false;
        }
    }
    // - is the assignment expression well formed?
    if(expr2.has_value()) {
        if(expr2.value()->resolve_type() == nullptr) {
            std::cout << "For loop assignment expression not well formed\n";
            return false;
        }
    }

    //start of loop
    std::string loop_start_label = create_new_label();
    std::string loop_end_label = create_new_label();
    fout << loop_start_label << ":\n";

    //check loop condition
    if(expr1.has_value()) {
        expr1.value()->emit_asm();
        fout << indent() << "cmp $0, %rax\n";
        fout << indent() << "je " << loop_end_label << "\n";
    }

    // - is the statement well formed?
    if(!statement->is_well_formed()) {
        return false;
    }

    //loop variable assignment
    if(expr2.has_value()) expr2.value()->emit_asm();

    //jump to start of loop
    fout << indent() << "jmp " << loop_start_label << "\n";

    //end of loop
    fout << loop_end_label << ":\n";

    if(asm_debug) fout << indent() << "# for loop end\n";

    pop_declaration_stack();
    return true;
}

bool CompoundStatement::is_well_formed() {
    push_declaration_stack();

    // - are all statements within well formed?
    for(int i = 0; i < statements.size(); i++){
        if(!statements[i]->is_well_formed()) {
            return false;
        }
    }

    pop_declaration_stack();
    return true;
}

bool Function::is_well_formed() {
    FunctionSignature *fs = resolve_function_signature();

    if(body == nullptr) {
        std::cout << "SKIPPING SYS FUNCTION : " << fs->to_string() << std::endl;
        return true;
    }
    std::cout << "CHECKING FUNCTION : " << fs->to_string() << std::endl;

    push_declaration_stack();

    bool is_main = FunctionSignature(new Identifier("main"), {}) == *(fs);
    bool is_constructor = is_function_constructor(this);

    // - if this function name is exactly equal to a type, make sure that it's a constructor
    if(is_type_declared(new BaseType(id->name)) && !is_function_constructor(this)) {
        std::cout << "Invalid function name : " << id->name << "\n";
        return false;
    }

    //print function label
    //if function signature is main(), substitute main for _start
    if(asm_debug) {
        fout << "# " << fs->to_string() << "\n";
    }
    if(is_main) {
        fout << ".global _start\n";
        fout << "_start:\n";
    }
    else {
        std::string label = get_function_label(fs);
        fout << label << ":\n";
    }

    //setup function stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";
    
    for(int i = 0; i < parameters.size(); i++){
        // - does parameter correspond to existing type?
        if(!is_type_declared(parameters[i]->type)) {
            std::cout << "Undeclared type : " << parameters[i]->type->to_string() << "\n";
            return false;
        }
        // - is parameter type not void?
        if(*(parameters[i]->type) == BaseType("void")) {
            std::cout << "Parameter can't have type void\n";
            return false;
        }
    }

    // - is return type of function existing?
    if(!is_type_declared(type)) {
        std::cout << "Function undeclared return type : " << type->to_string() << " " << id->name << "\n";
        return false;
    }
    
    //if has enclosing type, register self as variable (Type* this)
    local_offset = 8 + 8 * parameters.size();
    if(enclosing_type.has_value()) {
        //adjust local offset for 'extra variable'
        local_offset += 8;

        //register self as variable (Type this)
        Type *vt = enclosing_type.value();
        Identifier *vid = new Identifier("this");
        Variable* v = add_variable(vt, vid);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << vt << " " << vid << "\n";
            return false;
        }
        v->stack_offset = local_offset;
        local_offset -= 8;
    }
    for(int i = 0; i < parameters.size(); i++){
        Variable* v = add_variable(parameters[i]->type, parameters[i]->id);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << parameters[i]->type->to_string() << " " << parameters[i]->id->name << "\n";
            return false;
        }
        v->stack_offset = local_offset;
        local_offset -= 8;
    }

    //set local offset equal to %rsp
    local_offset = 0;

    //check that the local stack is empty so far
    assert(stack_desc.size() == 0);

    // - make sure body is well formed
    if(!body->is_well_formed()) {
        std::cout << "Function body not well formed\n";
        return false;
    }
    
    // - if type is not void, check for existence of return statement as last reachable statement
    // - constructors also don't have to have return statements, they're treated as void functions
    if(*type != BaseType("void") && !is_constructor) {
        // from any point of runnable code, we expect that it should be able to reach a return statement. 
        // this effectively means that the last statement should always be a return statement, regardless of 
        // the rest of the code, as the statement right before it needs to be able to return. 

        // once we introduce conditionals, this logic is going to change. For example
        // if(<condition>) {}
        // else {}
        // should not have to be followed with a return statement if there is a return statement in 
        // the <if> and <else> blocks

        // define a compound statement to be 'always returning' if when code execution reaches it, it's guaranteed
        // to return, regardless of whatever values might be in the variables. 

        // In order for a compound statement to be 'always returning', it just have to have some statement inside of it that is
        // 'always returning'. so we just need to check if the function body is 'always returning'. 

        // Note that if there is a statement before the last that is always returning, then any statement after
        // it is unreachable code, in which case we should print some warnings. 
        if(!body->is_always_returning()) {
            std::cout << "Non-void function doesn't always return\n";
            return false;
        }
    }
    else {
        //add trailing return for void functions
        fout << indent() << "pop %rbp\n";   //should not be managed by local_offset
        fout << indent() << "ret\n";
    }

    fout << "\n";

    //unregister parameters as variables
    pop_declaration_stack();

    //local stack should be empty before returning
    assert(stack_desc.size() == 0);

    return true;
}

bool StructDefinition::is_well_formed() {
    // - do all member variable types exist?
    for(int i = 0; i < member_variables.size(); i++){
        Type *vt = member_variables[i]->type;
        if(!is_type_declared(vt)) {
            std::cout << "Member variable type does not exist : " << vt->to_string() << "\n";
            return false;
        }
    }
    // - are there any duplicate member variable identifiers?
    for(int i = 0; i < member_variables.size(); i++){
        Identifier *id = member_variables[i]->id;
        for(int j = i + 1; j < member_variables.size(); j++){
            if(*id == *(member_variables[j]->id)) {
                std::cout << "Duplicate member variable identifier : " << id->name << "\n";
                return false;
            }
        }
    }
    // - are there any duplicate function definitions?
    // - are all of the functions not overloads?
    //add all functions to global list to check later
    for(int i = 0; i < functions.size(); i++){
        Function *f = functions[i];
        if(dynamic_cast<OperatorOverload*>(f) != nullptr) {
            std::cout << "Cannot declare operator overload within struct\n";
            return false;
        }
        if(!add_function(f)) {
            std::cout << "Failed to add struct member function : " << f->resolve_function_signature()->to_string() << "\n";
            return false;
        } 
    }
    // - is there a default constructor?
    {
        FunctionSignature *fid = new FunctionSignature(new Identifier(type->to_string()), {});
        if(!is_function_declared(fid)) {
            std::cout << "Default constructor for " << type->to_string() << " not defined\n";
            return false;
        }
    }
    // - is there a copy constructor?
    {
        FunctionSignature *fid = new FunctionSignature(new Identifier(type->to_string()), {type});
        if(!is_function_declared(fid)) {
            std::cout << "Copy constructor for " << type->to_string() << " not defined\n";
            return false;
        }
    }

    //construct StructLayout
    std::unordered_map<Identifier*, int> offset_map;
    int size = 0;
    for(int i = 0; i < member_variables.size(); i++){
        MemberVariable *mv = member_variables[i];
        offset_map.insert({mv->id, size});
        size += mv->type->calc_size();
    }

    // - make sure we can actually add StructLayout into the controller
    StructLayout *sl = new StructLayout(member_variables, offset_map, size);
    if(!add_struct_layout(type, sl)) {
        std::cout << "Failed to add StructLayout for " << type->to_string() << "\n";
        return false;
    }

    return true;
}

bool Program::is_well_formed() {
    // - set up semantic checker controller
    reset_controller();
    std::cout << "DONE INIT CONTROLLER" << std::endl;

    fout << ".section .text\n";

    //for all structs, register them as types
    for(int i = 0; i < structs.size(); i++) {
        if(!add_type(structs[i]->type)) {
            std::cout << "Failed to add type : " << structs[i]->type->to_string() << "\n";
            return false;
        }
    }
    std::cout << "DONE REGISTER STRUCTS AS TYPES" << std::endl;

    // - are there circular dependencies in the struct member variables?
    //build struct graph, do topological sort on the graph.
    std::vector<int> top_ord(0);
    {
        int n = structs.size();
        std::unordered_map<Type*, int> indmp;
        for(int i = 0; i < n; i++) indmp[structs[i]->type] = i;
        std::vector<std::vector<int>> c(n);
        std::vector<int> indeg(n, 0);
        for(int i = 0; i < n; i++){
            StructDefinition *sd = structs[i];
            for(int j = 0; j < sd->member_variables.size(); j++){
                Type *vt = sd->member_variables[j]->type;
                if(indmp.count(vt)) {
                    int x = indmp[vt];
                    c[i].push_back(x);
                    indeg[x] ++;
                }
            }
        }
        std::queue<int> q;
        for(int i = 0; i < n; i++){
            if(indeg[i] == 0) q.push(i);
        }
        while(q.size() != 0){
            int cur = q.front();
            q.pop();
            top_ord.push_back(cur);
            for(int x : c[cur]) {
                indeg[x] --;
                if(indeg[x] == 0) q.push(x);
            }
        }
    }
    if(top_ord.size() != structs.size()) {
        std::cout << "There exists a circular dependency in the struct graph\n";
        return false;
    }
    std::cout << "DONE CHECK CIRCULAR DEPENDENCIES" << std::endl;

    // - are all structs well formed?
    //do this in topological order
    for(int i = 0; i < structs.size(); i++){
        StructDefinition *s = structs[top_ord[i]];
        if(!s->is_well_formed()) {
            std::cout << "Struct not well formed : " << s->type->to_string() << "\n";
            return false;
        }
    }

    // - are there any duplicate global function definitions?
    for(int i = 0; i < functions.size(); i++) {
        if(!add_function(functions[i])) {
            std::cout << "Failed to add global function : " << functions[i]->id->name << "\n";
            return false;
        }
    }

    // - there must be a global function with function signature 'int main()'
    {
        FunctionSignature *main_fs = new FunctionSignature(new Identifier("main"), {});
        Function *f = get_function(main_fs);
        if(f == nullptr) {
            std::cout << "Missing main function\n";
            return false;
        }
        if(*(f->type) != BaseType("int")) {
            std::cout << "main has wrong return type (must be int)\n";
            return false;
        }
    }

    // - make sure every function is well formed
    for(int i = 0; i < declared_functions.size(); i++){
        enclosing_function = declared_functions[i];

        if(!declared_functions[i]->is_well_formed()) {
            std::cout << "Function not well formed : " << declared_functions[i]->resolve_function_signature()->to_string() << "\n";
            return false;
        }
    }

    assert(declaration_stack.size() == 0);
    assert(declared_variables.size() == 0);

    return true;
}

std::string read_file(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::ostringstream buffer;
    buffer << file.rdbuf(); 
    return buffer.str();     
}

std::string read_cstr(char* s) {
    std::string ans = "";
    int ptr = 0;
    while(s[ptr] != '\0') {
        ans.push_back(s[ptr ++]);
    }
    return ans;
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "USAGE : <filename>\n";
        std::cout << "<filename> must end with \".jank\"\n";
        return 1;
    }
    int argptr = 1;
    std::string filename = read_cstr(argv[argptr ++]);
    assert(filename.size() >= 5 && filename.substr(filename.size() - 5) == ".jank");

    Program *program = nullptr;
    {
        std::cout << "CHECKING SYNTAX" << std::endl;
        std::string code = read_file(filename);
        parser::s = code;
        parser::ptr = 0;
        parser::program *p = parser::program::parse();
        if(parser::ptr != code.size()) {
            std::cout << "SYNTAX ERROR\n";
            return 1;
        }
        std::cout << "SYNTAX PASS" << std::endl;
        std::cout << "CONVERTING ..." << std::endl;
        program = Program::convert(p);
    }

    std::cout << "--- STRUCT DEFINITIONS ---" << std::endl;
    for(int i = 0; i < program->structs.size(); i++){
        StructDefinition *sd = program->structs[i];
        std::cout << "NAME : " << sd->type->to_string() << std::endl;
        std::cout << "MEMBER VARIABLES : \n";
        for(int j = 0; j < sd->member_variables.size(); j++) {
            StructDefinition::MemberVariable *mv = sd->member_variables[j];
            std::cout << mv->type->to_string() << " " << mv->id->name << "\n";
        }
        std::cout << "MEMBER FUNCTIONS : \n";
        for(int j = 0; j < sd->functions.size(); j++){
            std::cout << sd->functions[j]->resolve_function_signature()->to_string() << "\n";
        }
        std::cout << "\n";
    }
   
    std::cout << "--- GLOBAL FUNCTION DEFINITIONS ---" << std::endl;
    for(int i = 0; i < program->functions.size(); i++){
        std::cout << "NAME : " << program->functions[i]->id->name << ", TYPE : " << program->functions[i]->type->to_string() << ", PARAMS :\n";
        for(int j = 0; j < program->functions[i]->parameters.size(); j++){
            Function::Parameter *param = program->functions[i]->parameters[j];
            std::cout << param->type->to_string() << " " << param->id->name << "\n";
        }
        std::cout << "NR STATEMENTS : " << program->functions[i]->body->statements.size() << std::endl;
        std::cout << "\n";
    }

    std::cout << "CHECK PROGRAM SEMANTICS" << std::endl;
    if(!program->is_well_formed()) {
        std::cout << "Program not well formed\n";
        return 1;
    }
    std::cout << "Program is well formed\n";
    
    return 0;
}