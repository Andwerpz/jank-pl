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

#include "semantics/Program.h"
#include "semantics/StructDefinition.h"
#include "semantics/Function.h"
#include "semantics/Identifier.h"
#include "semantics/Type.h"
#include "semantics/Statement.h"
#include "semantics/FunctionSignature.h"

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
        parser::set_s(code);
        parser::program *p = parser::program::parse();
        if(!parser::check_finished_parsing()) {
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
            MemberVariable *mv = sd->member_variables[j];
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