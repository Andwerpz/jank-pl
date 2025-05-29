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
struct Expression;
struct Declaration;
struct Assignment;
struct Statement;
struct SimpleStatement;
struct DeclarationStatement;
struct FunctionCallStatement;
struct ReturnStatement;
struct AssignmentStatement;
struct ControlStatement;
struct IfStatement;
struct WhileStatement;
struct ForStatement;
struct CompoundStatement;
struct Function;
struct FunctionSignature;
struct FunctionCall;
struct Program;

struct Variable;

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

    bool operator==(const Identifier& other) {
        return name == other.name;
    }
    bool operator!=(const Identifier& other) {
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
        assert(false);  
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
        type = _type;
    }
    static PointerType* convert(parser::pointer_type *t);

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

struct Expression {
    struct Primary {
        using val_t = std::variant<FunctionCall*, Identifier*, Literal*, Expression*>;
        val_t val;
        Primary(val_t _val) {
            val = _val;
        }
        static Primary* convert(parser::expr_primary *e); 
        Type* resolve_type();
        void emit_asm();
    };

    struct Postfix {
        using val_op = std::variant<Expression*>;
        Primary* term;
        std::vector<val_op> ops;
        Postfix(Primary *_term, std::vector<val_op> _ops) { 
            term = _term;
            ops = _ops;
        }
        static Postfix* convert(parser::expr_postfix *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct Unary {
        std::string op;
        std::variant<Postfix*, Unary*> val;
        Unary(std::string _op, Unary *_val) {
            assert(_val != nullptr);
            op = _op;
            val = _val;
        }
        Unary(Postfix *_val) {
            assert(_val != nullptr);
            val = _val;
        }
        static Unary* convert(parser::expr_unary *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct Multiplicative {
        std::vector<std::string> ops;
        std::vector<Unary*> terms;
        Multiplicative(std::vector<std::string> _ops, std::vector<Unary*> _terms) {
            ops = _ops;
            terms = _terms;
        }
        static Multiplicative* convert(parser::expr_multiplicative *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct Additive {
        std::vector<std::string> ops;
        std::vector<Multiplicative*> terms;
        Additive(std::vector<std::string> _ops, std::vector<Multiplicative*> _terms) {
            ops = _ops;
            terms = _terms;
        }
        static Additive* convert(parser::expr_additive *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct Shift {
        std::vector<std::string> ops;
        std::vector<Additive*> terms;
        Shift(std::vector<std::string> _ops, std::vector<Additive*> _terms) {
            ops = _ops;
            terms = _terms;
        }
        static Shift* convert(parser::expr_shift *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct Relational {
        std::vector<std::string> ops;
        std::vector<Shift*> terms;
        Relational(std::vector<std::string> _ops, std::vector<Shift*> _terms) {
            ops = _ops;
            terms = _terms;
        }
        static Relational* convert(parser::expr_relational *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct Equality {
        std::vector<std::string> ops;
        std::vector<Relational*> terms;
        Equality(std::vector<std::string> _ops, std::vector<Relational*> _terms) {
            ops = _ops;
            terms = _terms;
        }
        static Equality* convert(parser::expr_equality *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct BitAnd {
        std::vector<std::string> ops;
        std::vector<Equality*> terms;
        BitAnd(std::vector<std::string> _ops, std::vector<Equality*> _terms) {
            ops = _ops;
            terms = _terms;
        }
        static BitAnd* convert(parser::expr_bit_and *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct BitXor {
        std::vector<std::string> ops;
        std::vector<BitAnd*> terms;
        BitXor(std::vector<std::string> _ops, std::vector<BitAnd*> _terms) {
            ops = _ops;
            terms = _terms;
        }
        static BitXor* convert(parser::expr_bit_xor *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct BitOr {
        std::vector<std::string> ops;
        std::vector<BitXor*> terms;
        BitOr(std::vector<std::string> _ops, std::vector<BitXor*> _terms) {
            ops = _ops;
            terms = _terms;
        }
        static BitOr* convert(parser::expr_bit_or *e);
        Type* resolve_type();
        void emit_asm();
    };

    struct LogicalAnd {
        std::vector<std::string> ops;
        std::vector<BitOr*> terms;
        LogicalAnd(std::vector<std::string> _ops, std::vector<BitOr*> _terms) {
            ops = _ops;
            terms = _terms;
        }
        static LogicalAnd* convert(parser::expr_logical_and* e);
        Type* resolve_type();
        void emit_asm();
    };

    std::vector<std::string> ops;
    std::vector<LogicalAnd*> terms;
    Expression(std::vector<std::string> _ops, std::vector<LogicalAnd*> _terms) {
        ops = _ops;
        terms = _terms;
    }
    static Expression* convert(parser::expression *e);
    Type* resolve_type();
    void emit_asm();
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
    void emit_asm();
};

struct Assignment {
    Identifier *id;
    std::vector<Expression*> index_expr;
    Expression *expr;
    Assignment(Identifier *_id, std::vector<Expression*> _index_expr, Expression *_expr) {
        id = _id;
        index_expr = _index_expr;
        expr = _expr;
    }
    static Assignment* convert(parser::assignment *a);
    bool is_well_formed();
    void emit_asm();
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

struct FunctionCallStatement : public SimpleStatement {
    FunctionCall *function_call;
    FunctionCallStatement(FunctionCall *_function_call) {
        function_call = _function_call;
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

struct AssignmentStatement : public SimpleStatement {
    Assignment *assignment;
    AssignmentStatement(Assignment *a) {
        assignment = a;
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
    std::optional<Expression*> expr;
    std::optional<Assignment*> assignment;
    Statement *statement;
    ForStatement(Declaration *_declaration, Expression *_expr, Assignment *_assignment, Statement *_statement) {
        if(_declaration == nullptr) declaration = std::nullopt;
        else declaration = _declaration;
        if(_expr == nullptr) expr = std::nullopt;
        else expr = _expr;
        if(_assignment == nullptr) assignment = std::nullopt;
        else assignment = _assignment;
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

//name and input types. Does not include return type
struct FunctionSignature {
    Identifier *id;
    std::vector<Type*> input_types;
    FunctionSignature(Identifier *_id, std::vector<Type*> _input_types) {
        id = _id;
        input_types = _input_types;
    }
    
    bool operator==(FunctionSignature& other) {
        if(*id != *(other.id)) return false;
        if(input_types.size() != other.input_types.size()) return false;
        for(int i = 0; i < input_types.size(); i++){
            if(*(input_types[i]) != *(other.input_types[i])) return false;
        }
        return true;
    }
    bool operator!=(FunctionSignature& other) {
        return !(*this == other);
    }

    std::string to_string() {
        std::string ans = "";
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

    Identifier *id;
    Type *type;
    FunctionSignature *fs;
    std::vector<Parameter*> parameters;
    CompoundStatement *body;
    Function(Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body) {
        std::vector<Type*> input_types;
        for(int i = 0; i < _parameters.size(); i++) input_types.push_back(_parameters[i]->type);
        fs = new FunctionSignature(_id, input_types);
        id = _id;
        type = _type;
        parameters = _parameters;
        body = _body;
    }   

    //use when inputting pre-defined asm functions
    Function(Type *_type, Identifier *_id, std::vector<Type*> input_types) {
        fs = new FunctionSignature(_id, input_types);
        id = _id;
        type = _type;
    }

    static Function* convert(parser::function *f);
    bool is_well_formed();

    //just checks if the function signature matches
    bool operator==(Function& other) {
        return *fs == *(other.fs);
    }
    bool operator!=(Function& other) {
        return !(*this == other);
    }
};

struct FunctionCall {
    Identifier *id;
    std::vector<Expression*> argument_list;
    FunctionCall(Identifier *_id, std::vector<Expression*> _argument_list) {
        id = _id;
        argument_list = _argument_list;
    }
    static FunctionCall* convert(parser::function_call *f);
    Type* resolve_type();
    FunctionSignature* resolve_function_signature();
    void emit_asm();
};

//right now, a program is just a collection of functions. 
//Later, we'll introduce user defined types (structs)
struct Program {
    std::vector<Function*> functions;
    Program(std::vector<Function*> _functions) {
        functions = _functions;
    }
    static Program* convert(parser::program *p);
    bool is_well_formed();
};

// -- SEMANTIC ANALYSIS + CODE GENERATION CONTROLLER --
std::string indent() {
    return "    ";  //4 spaces
}

struct Variable {
    Type *type;
    Identifier *id;
    int stack_offset;
    Variable(Type *_type, Identifier *_id) {
        id = _id;
        type = _type;
    }
};

std::ofstream fout("test.asm");

Function* enclosing_function;
std::vector<Type*> declared_types;
std::vector<Function*> declared_functions;
std::vector<Variable*> declared_variables;
std::stack<std::vector<Variable*>> declaration_stack;

int local_var_stack_offset;
int label_counter;

//adds some helpful (?) comments in the generated asm. 
bool asm_debug = true;

std::string create_new_label() {
    std::string ans = "L" + std::to_string(label_counter++);
    return ans;
}

//expects %rax = array start, %rbx = array index
//will put return value into %rax
void emit_retrieve_array(int sz) {
    if(sz == 1) fout << indent() << "movb (%rax, %rbx, 1), %al\n";
    else if(sz == 2) fout << indent() << "movw (%rax, %rbx, 2), %ax\n";
    else if(sz == 4) fout << indent() << "movl (%rax, %rbx, 4), %eax\n";
    else if(sz == 8) fout << indent() << "movq (%rax, %rbx, 8), %rax\n";
    else assert(false);  //TODO : figure out what happens when struct size is > 8 bytes. 
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

//expects %rax = value, %rbx = mem address
//stores %rax into (%rbx)
void emit_mem_store(int sz) {
    if(sz == 1) fout << indent() << "movb %al, (%rbx)\n";
    else if(sz == 2) fout << indent() << "movw %ax, (%rbx)\n";
    else if(sz == 4) fout << indent() << "movl %eax, (%rbx)\n";
    else if(sz == 8) fout << indent() << "movq %rax, (%rbx)\n";
    else assert(false);
}

struct TypeConversionKey {
    std::optional<Type*> left;
    std::string op;
    std::optional<Type*> right;
    TypeConversionKey(Type *_left, std::string _op, Type *_right) { //binary operator
        assert(_left != nullptr);
        assert(_right != nullptr);
        left = _left;
        op = _op;
        right = _right;
    }
    TypeConversionKey(std::string _op, Type *_right) {  //left unary operator
        assert(_right != nullptr);
        left = std::nullopt;
        op = _op;
        right = _right;
    }
    TypeConversionKey(Type *_left, std::string _op) {   //right unary operator
        assert(_left != nullptr);
        left = _left;
        op = _op;
        right = std::nullopt;
    }
    TypeConversionKey(Type *from, Type *to) {   //casting
        left = from;
        op = "cast";
        right = to;
    }

    bool operator==(const TypeConversionKey& other) const {
        if(left.has_value() != other.left.has_value()) return false;
        if(left.has_value() && *(left.value()) != *(other.left.value())) return false;
        if(right.has_value() != other.right.has_value()) return false;
        if(right.has_value() && *(right.value()) != *(other.right.value())) return false;
        return op == other.op;
    }
};  

namespace std {
    template<>
    struct hash<TypeConversionKey> {
        size_t operator()(const TypeConversionKey& key) const {
            size_t seed = 0;
            if(key.left.has_value()) hash_combine(seed, key.left.value()->hash());
            else hash_combine(seed, 0);
            if(key.right.has_value()) hash_combine(seed, key.right.value()->hash());
            else hash_combine(seed, 0);
            hash_combine(seed, std::hash<std::string>()(key.op));
            return seed;
        }
    private:
        // Helper function to combine hashes
        static void hash_combine(size_t& seed, size_t value) {
            seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    };
}

//for binary operators, assumes left is in %rax and right is in %rbx
//for left unary operators, assumes right is in %rax
//for right unary operators, assumes left is in %rax
//for casting, assumes left is in %rax
//for the indexing operator specifically, '[]', the result of the expression is in %rbx
//will place the answer into %rax
struct TypeConversion {
    Type* res_type;
    std::vector<std::string> instructions;
    TypeConversion() {
        res_type = nullptr;
    }
    TypeConversion(Type *_res_type, std::vector<std::string> _instructions) {
        res_type = _res_type;
        instructions = _instructions;
    }

    void emit_asm();
};

std::unordered_map<TypeConversionKey, TypeConversion> conversion_map;

Type* find_resulting_type(Type* left, std::string op, Type* right) {
    if(left == nullptr || right == nullptr) return nullptr;
    assert(op.size() != 0);
    if(dynamic_cast<PointerType*>(left)) left = new BaseType("int");
    if(dynamic_cast<PointerType*>(right)) right = new BaseType("int");
    TypeConversionKey key = {left, op, right};
    if(!conversion_map.count(key)) {
        std::cout << "Invalid type conversion : " << left->to_string() << " " << op << " " << right->to_string() << "\n";
        return nullptr;
    }
    return conversion_map[key].res_type;
}

TypeConversion* find_type_conversion(Type* left, std::string op, Type* right) {
    if(left == nullptr || right == nullptr) return nullptr;
    assert(op.size() != 0);
    if(dynamic_cast<PointerType*>(left)) left = new BaseType("int");
    if(dynamic_cast<PointerType*>(right)) right = new BaseType("int");
    TypeConversionKey key = {left, op, right};
    if(!conversion_map.count(key)) {
        std::cout << "Invalid type conversion : " << left->to_string() << " " << op << " " << right->to_string() << "\n";
        return nullptr;
    }
    return &(conversion_map[key]);
}

Type* find_resulting_type(std::string op, Type* right) {
    if(right == nullptr) return nullptr;
    assert(op.size() != 0);
    if(dynamic_cast<PointerType*>(right)) right = new BaseType("int");
    TypeConversionKey key = {op, right};
    if(!conversion_map.count(key)) {
        std::cout << "Invalid type conversion : " << op << " " << right->to_string() << "\n";
        return nullptr;
    }
    return conversion_map[key].res_type;
}

TypeConversion* find_type_conversion(std::string op, Type* right) {
    if(right == nullptr) return nullptr;
    assert(op.size() != 0);
    if(dynamic_cast<PointerType*>(right)) right = new BaseType("int");
    TypeConversionKey key = {op, right};
    if(!conversion_map.count(key)) {
        std::cout << "Invalid type conversion : " << op << " " << right->to_string() << "\n";
        return nullptr;
    }
    return &(conversion_map[key]);
}

//TODO : haven't implemented type conversion lookup for right unary operators

Type* find_resulting_type(Type* from, Type* to) {
    if(from == nullptr || to == nullptr) return nullptr;

    //special cases
    // - from and to are the same type
    if(*from == *to) {
        return to;
    }
    Type* voidptr_t = new PointerType(new BaseType("void"));
    // - from is a pointer, to is void*
    if(dynamic_cast<PointerType*>(from) != nullptr && *to == *voidptr_t) {
        return to;
    }
    // - from is void*, to is a pointer
    if(*from == *voidptr_t && dynamic_cast<PointerType*>(to) != nullptr) {
        return to;
    }

    TypeConversionKey key = {from, to};
    if(!conversion_map.count(key)) {
        std::cout << "Invalid type conversion : (" << to->to_string() << ") " << from->to_string() << "\n";
        return nullptr;
    }
    return conversion_map[key].res_type;
}

TypeConversion* find_type_conversion(Type* from, Type* to) {
    if(from == nullptr || to == nullptr) return nullptr;

    //special cases
    // - from and to are the same type
    if(*from == *to) {
        return new TypeConversion(to, {});    //do nothing
    }
    Type* voidptr_t = new PointerType(new BaseType("void"));
    // - from is a pointer, to is void*
    if(dynamic_cast<PointerType*>(from) != nullptr && *to == *voidptr_t) {
        return new TypeConversion(to, {});    //do nothing
    }
    // - from is void*, to is a pointer
    if(*from == *voidptr_t && dynamic_cast<PointerType*>(to) != nullptr) {
        return new TypeConversion(to, {});    //do nothing
    }
    
    TypeConversionKey key = {from, to};
    if(!conversion_map.count(key)) {
        std::cout << "Invalid type conversion : (" << to->to_string() << ") " << from->to_string() << "\n";
        return nullptr;
    }
    return &(conversion_map[key]);
}

Type* find_variable_type(Identifier *id) {
    std::cout << "FIND VARIABLE TYPE : " << id->name << std::endl;
    for(int i = 0; i < declared_variables.size(); i++){
        if(*(declared_variables[i]->id) == *id) return declared_variables[i]->type;
    }
    return nullptr;
}

Type* find_function_type(FunctionSignature *fs) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(*(declared_functions[i]->fs) == *fs) return declared_functions[i]->type;
    }
    return nullptr;
}

bool is_type_declared(Type *t) {
    if(auto x = dynamic_cast<PointerType*>(t)) return is_type_declared(x->type);
    for(int i = 0; i < declared_types.size(); i++){
        if(*(declared_types[i]) == *t) return true;
    }
    return false;
}

bool is_function_declared(FunctionSignature *fs) {
    for(int i = 0; i < declared_functions.size(); i++){
        if(*fs == *(declared_functions[i]->fs)) {
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
    std::cout << "GET FUNCTION : " << fs->to_string() << "\n";
    for(int i = 0; i < declared_functions.size(); i++){
        if(*fs == *(declared_functions[i]->fs)) {
            return declared_functions[i];
        }
    }
    return nullptr;
}

Variable* get_variable(Identifier *id) {
    std::cout << "GET VARIABLE : " << id->name << "\n";
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

Variable* add_variable(Type *t, Identifier *id) {
    assert(t != nullptr && id != nullptr);
    assert(declaration_stack.size() != 0);
    std::cout << "ADDING VARIABLE : " << id->name << std::endl;
    if(is_identifier_used(id)) return nullptr;
    Variable *v = new Variable(t, id);
    declared_variables.push_back(v);
    declaration_stack.top().push_back(v);
    return v;
}

void remove_variable(Identifier *id) {
    assert(id != nullptr);
    std::cout << "REMOVING VARIABLE : " << id->name << std::endl;
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
    local_var_stack_offset += top.size() * 8;
    declaration_stack.pop();
    for(int i = 0; i < top.size(); i++){
        remove_variable(top[i]->id);
    }
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
    if(t->is_a0) {
        return PointerType::convert(t->t0->t0);
    }
    else if(t->is_a1) {
        return BaseType::convert(t->t1->t0);
    }
    else assert(false);
}

BaseType* BaseType::convert(parser::base_type *t) {
    return new BaseType(t->to_string());
}

PointerType* PointerType::convert(parser::pointer_type *t) {
    Type *type = BaseType::convert(t->t0);
    for(int i = 0; i < t->t1.size() - 1; i++){
        type = new PointerType(type);
    }
    return new PointerType(type);
}

Expression::Primary* Expression::Primary::convert(parser::expr_primary *e) {
    Expression::Primary::val_t val;
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
    return new Expression::Primary(val);
}

Expression::Postfix* Expression::Postfix::convert(parser::expr_postfix *e) {
    Expression::Primary *term = Expression::Primary::convert(e->t0);
    std::vector<val_op> ops;
    for(int i = 0; i < e->t1.size(); i++){
        std::string op = "[]";
        Expression* expr = Expression::convert(e->t1[i]->t3);
        ops.push_back(expr);
    }
    return new Expression::Postfix(term, ops);
}

Expression::Unary* Expression::Unary::convert(parser::expr_unary *e) {
    if(e->is_a0) {  //unary operator + unary expression
        std::string op = e->t0->t0->to_string();
        Expression::Unary *u = Expression::Unary::convert(e->t0->t2);
        return new Expression::Unary(op, u);
    }
    else if(e->is_a1) { //postfix expression
        Expression::Postfix *p = Expression::Postfix::convert(e->t1->t0);
        return new Expression::Unary(p);
    }
    else assert(false);
}

Expression::Multiplicative* Expression::Multiplicative::convert(parser::expr_multiplicative *e) {
    std::vector<std::string> ops;
    std::vector<Expression::Unary*> terms;
    terms.push_back(Expression::Unary::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1->to_string());
        terms.push_back(Expression::Unary::convert(e->t1[i]->t3));
    }
    return new Expression::Multiplicative(ops, terms);
}

Expression::Additive* Expression::Additive::convert(parser::expr_additive *e) {
    std::vector<std::string> ops;
    std::vector<Expression::Multiplicative*> terms;
    terms.push_back(Expression::Multiplicative::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1->to_string());
        terms.push_back(Expression::Multiplicative::convert(e->t1[i]->t3));
    }
    return new Expression::Additive(ops, terms);
}

Expression::Shift* Expression::Shift::convert(parser::expr_shift *e) {
    std::vector<std::string> ops;
    std::vector<Expression::Additive*> terms;
    terms.push_back(Expression::Additive::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1->to_string());
        terms.push_back(Expression::Additive::convert(e->t1[i]->t3));
    }
    return new Expression::Shift(ops, terms);
}

Expression::Relational* Expression::Relational::convert(parser::expr_relational *e) {
    std::vector<std::string> ops;
    std::vector<Expression::Shift*> terms;
    terms.push_back(Expression::Shift::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1->to_string());
        terms.push_back(Expression::Shift::convert(e->t1[i]->t3));
    }
    return new Expression::Relational(ops, terms);
}

Expression::Equality* Expression::Equality::convert(parser::expr_equality *e) {
    std::vector<std::string> ops;
    std::vector<Expression::Relational*> terms;
    terms.push_back(Expression::Relational::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1->to_string());
        terms.push_back(Expression::Relational::convert(e->t1[i]->t3));
    }
    return new Expression::Equality(ops, terms);
}

Expression::BitAnd* Expression::BitAnd::convert(parser::expr_bit_and *e) {
    std::vector<std::string> ops;
    std::vector<Expression::Equality*> terms;
    terms.push_back(Expression::Equality::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1);
        terms.push_back(Expression::Equality::convert(e->t1[i]->t3));
    }
    return new Expression::BitAnd(ops, terms);
}

Expression::BitXor* Expression::BitXor::convert(parser::expr_bit_xor *e) {
    std::vector<std::string> ops;
    std::vector<Expression::BitAnd*> terms;
    terms.push_back(Expression::BitAnd::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1);
        terms.push_back(Expression::BitAnd::convert(e->t1[i]->t3));
    }
    return new Expression::BitXor(ops, terms);
}

Expression::BitOr* Expression::BitOr::convert(parser::expr_bit_or *e) {
    std::vector<std::string> ops;
    std::vector<Expression::BitXor*> terms;
    terms.push_back(Expression::BitXor::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1);
        terms.push_back(Expression::BitXor::convert(e->t1[i]->t3));
    }
    return new Expression::BitOr(ops, terms);
}

Expression::LogicalAnd* Expression::LogicalAnd::convert(parser::expr_logical_and *e) {
    std::vector<std::string> ops;
    std::vector<Expression::BitOr*> terms;
    terms.push_back(Expression::BitOr::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1);
        terms.push_back(Expression::BitOr::convert(e->t1[i]->t3));
    }
    return new Expression::LogicalAnd(ops, terms);
}

Expression* Expression::convert(parser::expression *e) {
    std::vector<std::string> ops;
    std::vector<Expression::LogicalAnd*> terms;
    terms.push_back(Expression::LogicalAnd::convert(e->t0));
    for(int i = 0; i < e->t1.size(); i++){
        ops.push_back(e->t1[i]->t1);
        terms.push_back(Expression::LogicalAnd::convert(e->t1[i]->t3));
    }
    return new Expression(ops, terms);
}

Declaration* Declaration::convert(parser::declaration *d) {
    Type *type = Type::convert(d->t0);
    Identifier *name = Identifier::convert(d->t2);
    Expression *expr = Expression::convert(d->t6);
    return new Declaration(type, name, expr);
}

Assignment* Assignment::convert(parser::assignment *a) {
    Identifier *id = Identifier::convert(a->t0);
    std::vector<Expression*> index_expr;
    for(int i = 0; i < a->t1.size(); i++){
        index_expr.push_back(Expression::convert(a->t1[i]->t3));
    }
    Expression *expr = Expression::convert(a->t5);
    return new Assignment(id, index_expr, expr);
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
    if(s->is_a0) {  //declaration
        Declaration *declaration = Declaration::convert(s->t0->t0);
        return new DeclarationStatement(declaration);
    }
    else if(s->is_a1) { //function call
        FunctionCall *function_call = FunctionCall::convert(s->t1->t0);
        return new FunctionCallStatement(function_call);
    }
    else if(s->is_a2) { //return 
        Expression *expr = nullptr;
        if(s->t2->t1 != nullptr) {
            //non-void return
            expr = Expression::convert(s->t2->t1->t1);
        }
        return new ReturnStatement(expr);
    }
    else if(s->is_a3) { //assignment
        Assignment *assignment = Assignment::convert(s->t3->t0);
        return new AssignmentStatement(assignment);
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
        Expression *expr = nullptr;
        if(s->t2->t8 != nullptr) expr = Expression::convert(s->t2->t8->t0);
        Assignment *assignment = nullptr;
        if(s->t2->t12 != nullptr) assignment = Assignment::convert(s->t2->t12->t0);
        Statement *statement = Statement::convert(s->t2->t16);
        return new ForStatement(declaration, expr, assignment, statement);
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
    Identifier *name = Identifier::convert(def->t2);
    std::vector<Function::Parameter*> parameters;
    if(pl->t0 != nullptr) {
        parameters.push_back(Function::Parameter::convert(pl->t0->t0));
        std::vector<parser::parameter_list::a0::b0*> tmp = pl->t0->t1;
        for(int i = 0; i < tmp.size(); i++){
            parameters.push_back(Function::Parameter::convert(tmp[i]->t3));
        }
    }
    CompoundStatement *body = CompoundStatement::convert(f->t2);
    return new Function(type, name, parameters, body);
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

Program* Program::convert(parser::program *p) {
    std::vector<Function*> functions;
    for(int i = 0; i < p->t0.size(); i++){
        functions.push_back(Function::convert(p->t0[i]->t1));
    }
    return new Program(functions);
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

Type* Expression::Primary::resolve_type() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *f = std::get<FunctionCall*>(val);
        return f->resolve_type();
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        return find_variable_type(id);
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        return l->resolve_type();
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        return e->resolve_type();
    }
    else assert(false);
}

Type* Expression::Postfix::resolve_type() {
    Type *t = term->resolve_type();
    for(int i = 0; i < ops.size(); i++){
        val_op op = ops[i];
        if(std::holds_alternative<Expression*>(op)) {   //indexing
            Expression *expr = std::get<Expression*>(op);
            Type *et = expr->resolve_type();
            if(et == nullptr) return nullptr;
            if(*et != BaseType("int")) return nullptr;  //can't index without int
            if(auto x = dynamic_cast<PointerType*>(t)) {
                t = x->type;
            }
            else return nullptr;    //can't index into non-pointer
        }
        else assert(false);
    }
    return t;
}

Type* Expression::Unary::resolve_type() {
    if(std::holds_alternative<Postfix*>(val)) {
        Expression::Postfix *p = std::get<Expression::Postfix*>(val);
        return p->resolve_type();
    }
    else if(std::holds_alternative<Unary*>(val)) {
        Expression::Unary *u = std::get<Expression::Unary*>(val);
        return find_resulting_type(op, u->resolve_type());
    }
    else assert(false);
}

Type* Expression::Multiplicative::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* Expression::Additive::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* Expression::Shift::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* Expression::Relational::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* Expression::Equality::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* Expression::BitAnd::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* Expression::BitXor::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* Expression::BitOr::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* Expression::LogicalAnd::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* Expression::resolve_type() {
    Type *left = terms[0]->resolve_type();
    for(int i = 1; i < terms.size(); i++){
        left = find_resulting_type(left, ops[i - 1], terms[i]->resolve_type());
    }
    return left;
}

Type* FunctionCall::resolve_type() {
    FunctionSignature *fs = resolve_function_signature();
    if(fs == nullptr) {
        return nullptr;
    }
    Function *f = get_function(fs);
    if(f == nullptr) {
        return nullptr;
    }
    return f->type;
}

FunctionSignature* FunctionCall::resolve_function_signature() {
    //for each argument, figure out its type
    std::vector<Type*> types;
    for(int i = 0; i < argument_list.size(); i++){
        Type *t = argument_list[i]->resolve_type();
        if(t == nullptr) return nullptr;
        types.push_back(t);
    }
    return new FunctionSignature(id, types);
}

bool DeclarationStatement::is_always_returning() {
    return false;
}

bool FunctionCallStatement::is_always_returning() {
    return false;
}

bool ReturnStatement::is_always_returning() {
    return true;
}

bool AssignmentStatement::is_always_returning() {
    return false;
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

void TypeConversion::emit_asm() {
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
    fout << indent() << "mov $" << val.size() + 1 << ", %rax\n";
    fout << indent() << "push %rax\n";
    fout << indent() << "call malloc\n";
    fout << indent() << "add $8, %rsp\n";

    //save start of string
    fout << indent() << "push %rax\n";

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
    fout << indent() << "pop %rax\n";
}

void Expression::Primary::emit_asm() {
    if(std::holds_alternative<FunctionCall*>(val)) {
        FunctionCall *f = std::get<FunctionCall*>(val);
        f->emit_asm();
    }
    else if(std::holds_alternative<Identifier*>(val)) {
        Identifier *id = std::get<Identifier*>(val);
        Variable *v = get_variable(id);
        assert(v != nullptr);
        fout << indent() << "mov " << v->stack_offset << "(%rbp), %rax\n";
    }
    else if(std::holds_alternative<Literal*>(val)) {
        Literal *l = std::get<Literal*>(val);
        l->emit_asm();
    }
    else if(std::holds_alternative<Expression*>(val)) {
        Expression *e = std::get<Expression*>(val);
        e->emit_asm();
    }
    else assert(false);
}

void Expression::Postfix::emit_asm() {
    Type *t = term->resolve_type();
    term->emit_asm();   
    for(int i = 0; i < ops.size(); i++){
        val_op op = ops[i];
        if(std::holds_alternative<Expression*>(op)) {   //indexing
            //t must be PointerType
            assert(dynamic_cast<PointerType*>(t) != nullptr);
            t = (dynamic_cast<PointerType*>(t))->type;

            //save %rax to stack
            fout << indent() << "push %rax\n";

            //evaluate expression
            Expression *expr = std::get<Expression*>(op);
            assert(expr != nullptr);
            expr->emit_asm();

            //move expression result to %rbx
            fout << indent() << "mov %rax, %rbx\n";

            //return old %rax
            fout << indent() << "pop %rax\n";

            //find sizeof struct
            int sz = t->calc_size();

            //move data
            emit_retrieve_array(sz);
        }
        else assert(false);
    }
}

void Expression::Unary::emit_asm() {
    if(std::holds_alternative<Postfix*>(val)) {
        Expression::Postfix *p = std::get<Expression::Postfix*>(val);
        p->emit_asm();
    }
    else if(std::holds_alternative<Unary*>(val)) {
        Expression::Unary *u = std::get<Expression::Unary*>(val);
        u->emit_asm();
        TypeConversion *tc = find_type_conversion(op, u->resolve_type());
        tc->emit_asm();
    }
    else assert(false);
}

void Expression::Multiplicative::emit_asm() {
    terms[0]->emit_asm();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";
        
        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
}

void Expression::Additive::emit_asm() {
    terms[0]->emit_asm();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";
        
        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
}

void Expression::Shift::emit_asm() {
    terms[0]->emit_asm();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";
        
        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
}

void Expression::Relational::emit_asm() {
    terms[0]->emit_asm();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";
        
        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
}

void Expression::Equality::emit_asm() {
    terms[0]->emit_asm();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";
        
        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
}

void Expression::BitAnd::emit_asm() {
    terms[0]->emit_asm();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";
        
        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
}

void Expression::BitXor::emit_asm() {
    terms[0]->emit_asm();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";
        
        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
}

void Expression::BitOr::emit_asm() {
    terms[0]->emit_asm();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";
        
        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
}

void Expression::LogicalAnd::emit_asm() {
    terms[0]->emit_asm();
    std::string label = "no-label";
    if(terms.size() > 1) label = create_new_label();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "cmp $0, %rax\n";
        fout << indent() << "je " << label << "\n";

        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";

        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
    if(terms.size() > 1) fout << label << ":\n";
}

void Expression::emit_asm() {   //logical or
    terms[0]->emit_asm();
    std::string label = "no-label";
    if(terms.size() > 1) label = create_new_label();
    for(int i = 1; i < terms.size(); i++){
        fout << indent() << "cmp $0, %rax\n";
        fout << indent() << "jne " << label << "\n";

        fout << indent() << "push %rax\n";
        terms[i]->emit_asm();
        fout << indent() << "mov %rax, %rbx\n";
        fout << indent() << "pop %rax\n";

        TypeConversion *tc = find_type_conversion(terms[i - 1]->resolve_type(), ops[i - 1], terms[i]->resolve_type());
        tc->emit_asm();
    }
    if(terms.size() > 1) fout << label << ":\n";
}

void FunctionCall::emit_asm() {
    if(asm_debug) fout << indent() << "# calling function : " << id->name << "\n";

    //gather arguments and push them to the stack. 
    for(int i = 0; i < argument_list.size(); i++){
        Expression *e = argument_list[i];
        e->emit_asm();
        fout << indent() << "push %rax\n";
    }

    //call function
    fout << indent() << "call " << id->name << "\n";

    //clean up arguments
    fout << indent() << "add $" << 8 * argument_list.size() << ", %rsp\n";
}

void Declaration::emit_asm() {
    Variable *v = get_variable(id);
    assert(v != nullptr);
    v->stack_offset = local_var_stack_offset;
    local_var_stack_offset -= 8;

    if(asm_debug) fout << indent() << "# initialize local variable : " << type->to_string() << " " << id->name << "\n";

    //evaluate expression
    expr->emit_asm();

    //cast expression type to variable type
    Type *et = expr->resolve_type(), *vt = v->type;
    TypeConversion *tc = find_type_conversion(et, vt);
    assert(tc != nullptr);
    tc->emit_asm();

    //push variable to stack
    fout << indent() << "push %rax\n";

    if(asm_debug) fout << indent() << "# done initialize local variable : " << type->to_string() << " " << id->name << "\n";
}

void Assignment::emit_asm() {
    Variable *v = get_variable(id);
    assert(v != nullptr);

    if(asm_debug) fout << indent() << "# assignment start\n";

    //evaluate expression
    expr->emit_asm();
    
    //cast expression type to variable type
    Type *et = expr->resolve_type(), *vt = v->type;
    for(int i = 0; i < index_expr.size(); i++){
        PointerType *pt = dynamic_cast<PointerType*>(vt);
        assert(pt != nullptr);
        vt = pt->type;
    }
    TypeConversion *tc = find_type_conversion(et, vt);
    assert(tc != nullptr);
    tc->emit_asm();

    if(index_expr.size() == 0){
        //store into stack
        fout << indent() << "mov %rax, " << v->stack_offset << "(%rbp)\n";
    }
    else {
        //push expression value onto stack
        fout << indent() << "push %rax\n";

        //find index to place value into
        fout << indent() << "lea " << v->stack_offset << "(%rbp), %rax\n";  //%rax points at the stack
        Type *t = v->type;
        for(int i = 0; i < index_expr.size(); i++){
            //compute index
            fout << indent() << "push %rax\n";
            index_expr[i]->emit_asm();
            fout << indent() << "mov %rax, %rbx\n";
            fout << indent() << "pop %rax\n";

            PointerType *pt = dynamic_cast<PointerType*>(t);
            assert(pt != nullptr);
            int sz = pt->type->calc_size();
            fout << indent() << "mov (%rax), %rax\n";   //%rax now points at the beginning of the array
            fout << indent() << "lea (%rax, %rbx, " << sz << "), %rax\n";
            t = pt->type;
        }
        fout << indent() << "mov %rax, %rbx\n";

        //store value
        fout << indent() << "pop %rax\n";   //expression value
        emit_mem_store(t->calc_size());
    }

    if(asm_debug) fout << indent() << "# assignment end\n";
}

bool Declaration::is_well_formed() {
    // - is the type being used declared?
    if(!is_type_declared(type)) {
        std::cout << "Declaration using undeclared type : " << type->to_string() << "\n";
        return false;
    }
    // - make sure type is not void
    if(*type == BaseType("void")) {
        std::cout << "Cannot declare variable with void type\n";
        return false;
    }
    // - does the expression resolve to a type?
    Type *expr_type = expr->resolve_type();
    if(expr_type == nullptr || !is_type_declared(expr_type)) {
        std::cout << "Declaration expression does not resolve to existing type\n";
        return false;
    }
    // - can expr_type be casted to type?
    if(find_type_conversion(expr_type, type) == nullptr) {
        std::cout << "Declaration type mismatch: cannot cast type " << expr_type->to_string() << " to " << type->to_string() << "\n";
        return false;
    }
    // - is the identifier being used already taken?
    Variable *v = add_variable(type, id);
    if(v == nullptr) {
        std::cout << "Unable to add variable : " << type->to_string() << " " << id->name << "\n";
        return false;
    }
    return true;
}

bool Assignment::is_well_formed() {
    // - does the expression resolve to a type?
    Type *et = expr->resolve_type();
    if(et == nullptr) {
        std::cout << "Assignment expression does not resolve to type\n";
        return false;
    }
    // - does the type the expression resolves to exist?
    if(!is_type_declared(et)) {
        std::cout << "Assignment expression does not resolve to existing type\n";
        return false;
    }
    // - does the variable exist?
    Variable *v = get_variable(id);
    if(v == nullptr) {
        std::cout << "Assignment variable does not exist : " << id->name << "\n";
        return false;
    }
    // - do all of the indexing expressions resolve to int?
    for(int i = 0; i < index_expr.size(); i++){
        if(*(index_expr[i]->resolve_type()) != BaseType("int")) {
            std::cout << "Indexing expression must resolve to int\n";
            return false;
        }
    }
    // - can we actually index into this variable?
    Type *vt = v->type;
    for(int i = 0; i < index_expr.size(); i++){
        PointerType *pt = dynamic_cast<PointerType*>(vt);
        if(pt == nullptr) {
            std::cout << "Can't index into non-pointer\n";
            return false;
        }
        vt = pt->type;
    }
    // - can the expression type be casted to the variable type?
    if(find_type_conversion(et, vt) == nullptr) {
        std::cout << "Assignment type mismatch: cannot cast " << et->to_string() << " to " << vt->to_string() << "\n";
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

    declaration->emit_asm();

    return true;
}

bool FunctionCallStatement::is_well_formed() {
    // - does the function call resolve properly to a type?
    FunctionSignature *fs = function_call->resolve_function_signature();
    if(fs == nullptr) {
        std::cout << "Unable to resolve function signature\n";
        return false;
    }
    // - is the function we're referencing actually declared?
    if(!is_function_declared(fs)) {
        std::cout << "Reference to undeclared function : " << fs->id << "\n";
        return false;
    }

    function_call->emit_asm();

    return true;
}

bool ReturnStatement::is_well_formed() {
    // - does the expression resolve to a type?
    Type *t = nullptr;
    if(opt_expr.has_value()) {
        Expression *expr = opt_expr.value();
        t = expr->resolve_type();
    }
    else t = new BaseType("void");
    if(t == nullptr) {
        std::cout << "Return expression does not resolve to type\n";
        return false;
    }
    // - does the expression return type match the return type of the enclosing function?
    if(*t != *(enclosing_function->type)) {
        std::cout << "Return type does not match enclosing function\n";
        return false;
    }

    //put return value into %rax
    if(*t != BaseType("void")) {
        assert(opt_expr.has_value());
        opt_expr.value()->emit_asm();
    }

    //clean up local variables
    if(declared_variables.size() != 0) {
        fout << indent() << "add $" << declaration_stack.top().size() * 8 << ", %rsp\n";
    }

    if(FunctionSignature(new Identifier("main"), {}) == *(enclosing_function->fs)) {
        //function is main, use exit syscall instead
        fout << indent() << "push %rax\n";
        fout << indent() << "call sys_exit\n";
    }
    else {
        //return from function
        fout << indent() << "pop %rbp\n";
        fout << indent() << "ret\n";
    }

    return true;
}

bool AssignmentStatement::is_well_formed() {
    // - is the assignment well formed?
    if(!assignment->is_well_formed()) {
        std::cout << "Assignment not well formed\n";
        return false;
    }

    assignment->emit_asm();

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

    // - is the declaration well formed? 
    if(declaration.has_value()) {
        if(!declaration.value()->is_well_formed()) {
            return false;
        }
    }
    // - does the expression resolve to type 'int'?
    if(expr.has_value()) {
        Type *t = expr.value()->resolve_type();
        if(t == nullptr || *t != BaseType("int")) {
            return false;
        }
    }
    // - is the assignment well formed?
    if(assignment.has_value()) {
        if(!assignment.value()->is_well_formed()) {
            return false;
        }
    }

    //declare loop variable
    if(declaration.has_value()) declaration.value()->emit_asm();

    if(asm_debug) fout << indent() << "# for loop start\n";

    //start of loop
    std::string loop_start_label = create_new_label();
    std::string loop_end_label = create_new_label();
    fout << loop_start_label << ":\n";

    //check loop condition
    if(expr.has_value()) {
        expr.value()->emit_asm();
        fout << indent() << "cmp $0, %rax\n";
        fout << indent() << "je " << loop_end_label << "\n";
    }

    // - is the statement well formed?
    if(!statement->is_well_formed()) {
        return false;
    }

    //loop variable assignment
    if(assignment.has_value()) assignment.value()->emit_asm();

    //jump to start of loop
    fout << indent() << "jmp " << loop_start_label << "\n";

    //end of loop
    fout << loop_end_label << ":\n";

    //remove local variables from stack
    if(declaration_stack.top().size() != 0) {
        fout << indent() << "add $" << declaration_stack.top().size() * 8 << ", %rsp\n";
    }

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

    //remove local variables from stack
    if(declaration_stack.top().size() != 0) {
        fout << indent() << "add $" << declaration_stack.top().size() * 8 << ", %rsp\n";
    }

    pop_declaration_stack();
    return true;
}

bool Function::is_well_formed() {
    push_declaration_stack();

    bool is_main = FunctionSignature(new Identifier("main"), {}) == *fs;

    //print function label
    //if function signature is main(), substitute main for _start
    if(is_main) {
        fout << ".global _start\n";
        fout << "_start:\n";
    }
    else {
        fout << ".global " << id->name << "\n";
        fout << id->name << ":\n";
    }

    //setup function stack frame
    fout << indent() << "push %rbp\n";
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

    //register parameters as variables
    local_var_stack_offset = 8 + 8 * parameters.size();
    for(int i = 0; i < parameters.size(); i++){
        Variable* v = add_variable(parameters[i]->type, parameters[i]->id);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << parameters[i]->type->to_string() << " " << parameters[i]->id->name << "\n";
            return false;
        }
        v->stack_offset = local_var_stack_offset;
        local_var_stack_offset -= 8;
    }
    local_var_stack_offset = -8;

    // - make sure body is well formed
    if(!body->is_well_formed()) {
        std::cout << "Function body not well formed\n";
        return false;
    }
    
    // - if type is not void, check for existence of return statement as last reachable statement
    if(*type != BaseType("void")) {
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
        fout << indent() << "pop %rbp\n";
        fout << indent() << "ret\n";
    }

    fout << "\n";

    //unregister parameters as variables
    pop_declaration_stack();

    return true;
}

bool Program::is_well_formed() {
    // - set up semantic checker controller
    {
        conversion_map.clear();
        enclosing_function = nullptr;
        declared_functions.clear();
        declared_types.clear();
        declared_variables.clear();
        while(declaration_stack.size()) declaration_stack.pop();
        label_counter = 0;

        // - sys functions
        declared_functions.push_back(new Function(new PointerType(new BaseType("void")), new Identifier("malloc"), {new BaseType("int")}));
        declared_functions.push_back(new Function(new PointerType(new BaseType("char")), new Identifier("int_to_string"), {new BaseType("int")}));
        declared_functions.push_back(new Function(new BaseType("void"), new Identifier("puts"), {new PointerType(new BaseType("char"))}));
        declared_functions.push_back(new Function(new BaseType("void"), new Identifier("puts_endl"), {new PointerType(new BaseType("char"))}));
        declared_functions.push_back(new Function(new BaseType("void"), new Identifier("puti"), {new BaseType("int")}));
        declared_functions.push_back(new Function(new BaseType("void"), new Identifier("puti_endl"), {new BaseType("int")}));

        // - primitive types
        Type *p_int = new BaseType("int");
        declared_types.push_back(p_int);
        Type *p_char = new BaseType("char");
        declared_types.push_back(p_char);
        Type *p_void = new BaseType("void");
        declared_types.push_back(p_void);

        // - populate conversion map
        conversion_map[{"+", p_int}] = {p_int, {}};
        conversion_map[{"-", p_int}] = {p_int, {"neg %rax"}};
        conversion_map[{"~", p_int}] = {p_int, {"not %rax"}};
        conversion_map[{"!", p_int}] = {p_int, {
            "test %rax, %rax",
            "sete %al",
            "movzx %al, %rax",
        }};
        conversion_map[{p_int, "+", p_int}] = {p_int, {"add %rbx, %rax"}};
        conversion_map[{p_int, "-", p_int}] = {p_int, {"sub %rbx, %rax"}};
        conversion_map[{p_int, "*", p_int}] = {p_int, {"imul %rbx, %rax"}};
        conversion_map[{p_int, "/", p_int}] = {p_int, {
            "cqo",
            "idiv %rbx",
        }};
        conversion_map[{p_int, "%", p_int}] = {p_int, {
            "cqo",
            "idiv %rbx",
            "mov %rdx, %rax",
        }};
        conversion_map[{p_int, "&", p_int}] = {p_int, {"and %rbx, %rax"}};
        conversion_map[{p_int, "^", p_int}] = {p_int, {"xor %rbx, %rax"}};
        conversion_map[{p_int, "|", p_int}] = {p_int, {"or %rbx, %rax"}};
        conversion_map[{p_int, "&&", p_int}] = {p_int, {
            "test %rax, %rax\n",
            "setne %al\n",
            "movzx %al, %rax\n",
            "test %rbx, %rbx\n",
            "setne %bl\n",
            "movzx %bl, %rbx\n",
            "and %rbx, %rax\n",
        }};
        conversion_map[{p_int, "||", p_int}] = {p_int, {
            "test %rax, %rax\n",
            "setne %al\n",
            "movzx %al, %rax\n",
            "test %rbx, %rbx\n",
            "setne %bl\n",
            "movzx %bl, %rbx\n",
            "or %rbx, %rax\n",
        }};
        conversion_map[{p_int, "==", p_int}] = {p_int, {
            "cmp %rbx, %rax",
            "sete %al",
            "movzx %al, %rax",
        }};
        conversion_map[{p_int, "!=", p_int}] = {p_int, {
            "cmp %rbx, %rax",
            "setne %al",
            "movzx %al, %rax",
        }};
        conversion_map[{p_int, "<", p_int}] = {p_int, {
            "cmp %rbx, %rax",
            "setl %al",
            "movzx %al, %rax",
        }};
        conversion_map[{p_int, ">", p_int}] = {p_int, {
            "cmp %rbx, %rax",
            "setg %al",
            "movzx %al, %rax",
        }};
        conversion_map[{p_int, "<=", p_int}] = {p_int, {
            "cmp %rbx, %rax",
            "setle %al",
            "movzx %al, %rax",
        }};
        conversion_map[{p_int, ">=", p_int}] = {p_int, {
            "cmp %rbx, %rax",
            "setge %al",
            "movzx %al, %rax",
        }};
        conversion_map[{p_int, "<<", p_int}] = {p_int, {
            "push %rcx",
            "mov %rbx, %rcx",
            "sal %cl, %rax",
            "pop %rcx",
        }};
        conversion_map[{p_int, ">>", p_int}] = {p_int, {
            "push %rcx",
            "mov %rbx, %rcx",
            "sar %cl, %rax",
            "pop %rcx",
        }};
    }
    

    push_declaration_stack();

    fout << ".section .text\n";

    // - are there any duplicate function definitions?
    for(int i = 0; i < functions.size(); i++){
        for(int j = i + 1; j < functions.size(); j++) {
            if(*(functions[i]) == *(functions[j])) {
                std::cout << "Duplicate function definitions : " << functions[i]->id->name << "\n";
                return false;
            }
        }
    }
    for(int i = 0; i < functions.size(); i++) {
        declared_functions.push_back(functions[i]);
    }

    // - there must be a function with function signature 'int main()'
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
    for(int i = 0; i < functions.size(); i++){
        enclosing_function = functions[i];
        if(!functions[i]->is_well_formed()) {
            std::cout << "Function not well formed : " << functions[i]->id->name << "\n";
            return false;
        }
    }

    pop_declaration_stack();

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
   
    std::cout << "FUNCTION DEFINITIONS : " << std::endl;
    for(int i = 0; i < program->functions.size(); i++){
        std::cout << "NAME : " << program->functions[i]->id->name << ", TYPE : " << program->functions[i]->type->to_string() << ", PARAMS :\n";
        for(int j = 0; j < program->functions[i]->parameters.size(); j++){
            Function::Parameter *param = program->functions[i]->parameters[j];
            std::cout << param->type->to_string() << " " << param->id->name << "\n";
        }
        std::cout << "NR STATEMENTS : " << program->functions[i]->body->statements.size() << std::endl;
    }

    if(!program->is_well_formed()) {
        std::cout << "Program not well formed\n";
        return 1;
    }
    std::cout << "Program is well formed\n";
    
    return 0;
}