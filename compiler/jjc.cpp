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
#include <set>
#include <iomanip>

#include "semantics/Program.h"
#include "semantics/StructDefinition.h"
#include "semantics/Function.h"
#include "semantics/Identifier.h"
#include "semantics/Type.h"
#include "semantics/Statement.h"
#include "semantics/FunctionSignature.h"
#include "semantics/Parameter.h"
#include "semantics/Constructor.h"
#include "semantics/ConstructorSignature.h"
#include "semantics/Include.h"

#include "semantics/utils.h"

#include <unistd.h>  
#include <sys/wait.h>
#include <cstring>
#include <cstdio>  
#include <limits.h>
#include <libgen.h>
#include <sys/stat.h>

bool print_timing_info = false;

std::string compiler_dir;
std::string cwd_dir;

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

std::vector<std::string> str_split(std::string s, char sep) {
    std::vector<std::string> ret;
    int l = 0;
    for(int i = 0; i < s.size(); i++) {
        if(s[i] == sep) {
            ret.push_back(s.substr(l, i - l));
            l = i + 1;
        }
    }
    ret.push_back(s.substr(l, s.size() - l));
    return ret;
}

std::string extract_filename(std::string path) {
    return *(str_split(path, '/').rbegin());
}

std::string extract_folder_path(std::string path) {
    for(int i = (int) path.size() - 1; i >= 0; i--){
        if(path[i] == '/') return path.substr(0, i + 1);
    }
    //there is no '/', just return "./"
    return "./";
}

std::string extract_stem(std::string filename) {
    for(int i = filename.size() - 1; i >= 0; i--) {
        if(filename[i] == '.') return filename.substr(0, i);
    }
    return filename;
}   

std::string extract_ext(std::string filename) {
    for(int i = filename.size() - 1; i >= 0; i--){
        if(filename[i] == '.') return filename.substr(i);
    }
    return "";
}

std::string cwd_rel_to_absolute(std::string path){
    if(path[0] == '/') return path;
    return cwd_dir + "/" + path;
}

std::string libj_to_absolute(std::string name) {
    return compiler_dir + "/libj/" + name + ".jank";
}

int gen_asm(std::string src_path, char tmp_filename[]) {
    std::cout << "--- GATHERING FILES ---" << std::endl;
    std::string program_str = "";
    Program *program = new Program();
    ld parse_duration;
    {
        ld parse_start_time = current_time_seconds();

        std::queue<std::string> to_parse;
        std::set<std::string> parsed_paths;

        //include default libraries
        std::vector<std::string> default_includes = {
            "memory",
            "error",
            "defs",
        };  
        if(kernel_mode) {
            //malloc should be implemented by the kernel itself
        }
        else {
            default_includes.push_back("syscall");
            default_includes.push_back("malloc");
        }
        for(std::string s : default_includes) {
            std::string npath = libj_to_absolute(s);
            to_parse.push(npath);
            parsed_paths.insert(npath);
        }

        //base source file
        to_parse.push(cwd_rel_to_absolute(src_path));
        parsed_paths.insert(cwd_rel_to_absolute(src_path));

        while(to_parse.size() != 0){
            std::string cpath = to_parse.front();
            to_parse.pop();
            std::cout << cpath << std::endl;

            std::string code = read_file(cpath);
            program_str += code;

            parser::set_s(code);
            parser::program *pp = parser::program::parse();
            if(!parser::check_finished_parsing(true)) {
                std::cout << "SYNTAX ERROR\n";
                return 1;
            }

            Program *np = Program::convert(pp);
            program->add_all(np);

            //grab all includes
            for(int i = 0; i < np->includes.size(); i++){
                Include *inc = np->includes[i];
                std::string npath;
                if(inc->is_library_include) npath = compiler_dir + "/libj/" + inc->path + ".jank";
                else npath = extract_folder_path(cpath) + inc->path;

                //check if we already parsed
                if(parsed_paths.count(npath)) continue;

                parsed_paths.insert(npath);
                to_parse.push(npath);
            }
        }

        parse_duration = current_time_seconds() - parse_start_time;
    }   

    std::cout << "--- CHECK PROGRAM SEMANTICS ---" << std::endl;
    ld semantics_duration;
    {
        ld semantics_start_time = current_time_seconds();

        fout = std::ofstream(tmp_filename);
        if(!program->is_well_formed()) {
            std::cout << "Program not well formed\n";
            fout.close();
            return 1;
        }
        std::cout << "Program is well formed\n";
        fout.close();

        semantics_duration = current_time_seconds() - semantics_start_time;
        
    }

    if(print_timing_info) {
        std::cout << "--- TIMING INFO ---" << "\n";
        print_duration_stats();
        
        std::cout << "--- TIMING OVERALL ---" << "\n";
        std::cout << std::fixed << std::setprecision(3) << "Parse Duration : " << parse_duration << "\n";
        std::cout << std::fixed << std::setprecision(3) << "Semantics Duration : " << semantics_duration << "\n";
    }

    return 0;
}

int assemble(char src_path[], char res_path[]) {
    pid_t pid = fork();
    if(pid == 0) {
        execlp(
            "gcc", "gcc", 
            "-g",                           //debug metadata
            "-x", "assembler",              //gcc expects .s files to be assembly, tell it that all files are assembly
            "-nostartfiles", "-nostdlib",   //tell gcc that we're not compiling C assembly
            "-m64",                         //64 bit mode?
            src_path,
            "-o", res_path,
            (char*) NULL
        );
        perror("execlp gcc failed");
        return 1;
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                std::cout << "Terminated by signal " << sig << " (" << strsignal(sig) << ")" << std::endl;
            }
            std::cout << "Assembling Error" << std::endl;
            return 1;
        }
    }
    return 0; 
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "USAGE : <filepath>\n";
        std::cout << "-S : generate assembly instead of executable\n";
        std::cout << "-o <out_filepath>\n";
        std::cout << "-k : kernel mode\n";
        std::cout << "-ad : assembly debug mode (prints some helpful (?) comments in the generated assembly)\n";
        return 1;
    }
    int argptr = 1;
    std::string filepath = read_cstr(argv[argptr ++]);
    std::string dst_dir = "a.out";

    //figure out compiler absolute directory
    {
        // /proc/self/exe is symlink to currently running binary
        // so readlink should give the absolute path no matter how this was invoked
        char exe_path[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
        if (len != -1) {
            exe_path[len] = '\0';
            compiler_dir = dirname(exe_path);
            std::cout << "COMPILER DIR : " << compiler_dir << "\n";
        }
        else {
            std::cout << "Could not find jjc path\n";
            return 1;
        }
    }

    //figure out current working directory
    {
        char cwd_path[PATH_MAX];
        if (getcwd(cwd_path, sizeof(cwd_path)) != NULL) {
            cwd_dir = std::string(cwd_path);
            std::cout << "CWD : " << cwd_dir << "\n";
        }
        else {
            std::cout << "Could not find cwd\n";
            return 1;
        }
    }

    //read in arguments
    bool return_asm = false;
    while(argptr < argc) {
        std::string arg(argv[argptr ++]);
        if(arg == "-S") {
            return_asm = true;
        }
        else if(arg == "-o") {
            if(argptr >= argc) {
                std::cout << "Missing output directory after -o\n";
                return 1;
            }
            dst_dir = std::string(argv[argptr ++]);
        }
        else if(arg == "-k") {
            kernel_mode = true;
        }
        else if(arg == "-ad") {
            asm_debug = true;
        }
        else if(arg == "-time") {
            print_timing_info = true;
        }
        else {
            std::cout << "Unrecognized commandline argument : " << arg << "\n";
            return 1;
        }
    }

    char asm_file[] = "jjc_asmXXXXXX";
    int fd = mkstemp(asm_file);
    if(fd == -1) {
        perror("mkstemp failed");
        return 1;
    }
    close(fd);

    pid_t pid = fork();
    if(pid == 0) {
        exit(gen_asm(filepath, asm_file));
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                std::cout << "Terminated by signal " << sig << " (" << strsignal(sig) << ")" << std::endl;
            }
            std::cout << "Compilation Error\n";
            std::remove(asm_file);
            return 1;
        }
    }

    if(return_asm) {
        std::ifstream src(asm_file);
        std::ofstream dst(dst_dir);
        if(!src || !dst) {
            std::cout << "Failed to copy over from " << asm_file << " to " << dst_dir << "\n";
            std::remove(asm_file);
            return 1;
        }
        dst << src.rdbuf();
        std::remove(asm_file);
        return 0;
    }

    //gen_asm success, time to assemble
    char exe_file[] = "jjc_exeXXXXXX";
    fd = mkstemp(exe_file);
    if(fd == -1) {
        perror("mkstemp failed");
        std::remove(asm_file);
        return 1;
    }
    close(fd);
    pid = fork();
    if(pid == 0){
        exit(assemble(asm_file, exe_file));
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            std::cout << "Assembling Error\n";
            std::remove(asm_file);
            std::remove(exe_file);
            return 1;
        }
    }

    //gen exe done, remove asm file
    std::remove(asm_file);

    //move exe file to output
    {
        std::ifstream src(exe_file);
        std::ofstream dst(dst_dir);
        if(!src || !dst) {
            std::cout << "Failed to copy over from " << exe_file << " to " << dst_dir << "\n";
            std::remove(exe_file);
            return 1;
        }
        dst << src.rdbuf();

        //give file execute permissions
        chmod(dst_dir.c_str(), 0755);
    }
    
    //delete exe file
    std::remove(exe_file);
    
    return 0;
}


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


Seems like expression evaluation is actually going to be a relatively expensive portion of the compiler. I optimized it
enough such that it can run fine if you have 20 nested function calls, which should be good enough for the vast majority
of cases. Next feature is probably going to be templating, but I want to first write some testing utilities to automatically
run the tests that I've been writing up until now. I also want to write some tests that won't compile. 


Some thoughts on templating: Either we can write the templated functions to resolve the type at runtime, or do the far
easier option which is to generate every version of templated type during compile time, then treat them as different types. 

Yes, we are going to treat templates as compile time macros. We'll support templating for structs, functions, and overloads. 
Before checking anything else, we look through the entire AST for templated struct instantiations, templated function calls, 
and templated overload usages. Then, we generate the templated instances of those usages. We keep doing this until there 
are no more undefined template usages in the code. Finally, we compile the resulting AST as if there are no templates. 

to resolve function templates, it requires being able to resolve the types of the arguments. To resolve the types of the arguments,
we need to generate all the types in the arguments, and add all their functions and member variable mappings into the controller. 
Adding all the functions and member variables just means generating any templated structs within the generated struct. 

I'm just concerned about circular dependencies between the structs. For example, what if we have something like this:

template <T>
struct A {
    B<T>* foo;

    T do_calc(T arg) {
        return foo->do_calc(new T());
    }
}

template <T>
struct B {
    A<T>* foo;

    T do_calc(T arg) {
        return foo->do_calc(new T());
    }
}

Can we generate all possible structs without generating any functions at all? I don't think so, consider make_pair(T, U). We'll need to 
generate the pair from the return value. And then we can have make_triple(make_pair(T, U), V), triple<T, U, V> is completely obscured by
the templated function calls. But eventually if we go deep enough, there will always be a function call that contains purely non-templated
function calls (?).

the concern is that what if there is a function call that doesn't contain any more templated function calls, but does contain references to 
a templated type that then contains a function call back to itself? Like:

template<T>
struct A {
    A() {
        foo(this);
    }
}

template<T>
void foo(A<T> arg) {

}

foo(new A<int>())

We want to resolve the type of (new A<int>()), but we first need to generate struct A<int>. But generating struct A<int> and then checking it 
will then run into foo(A<int>), which is infinite recursion. 

Perhaps when running into structs, I should only initialize them (make sure their member variables and function parameters are instantiated). 
Then, push their member functions and constructors to be checked later. This way when I'm actually checking a function body and I need to 
instantiate a type to resolve a function call, I can just do a 'shallow' instantiation and still collect all member variables and function
signatures, and this 'shallow' instantiation will not trigger any rounds of 'deep' instantiation. 

ok, realized that we need knowledge of variable types to do function call type resolution. So instead I've worked this out:

 - add_type() will take struct definition, register its type, add all functions / constructors, resolve any templated types
   in the struct member variables and functions and constructors. Will also make sure templated types are well formed
 - get_called_function() should first scan through all templated functions, and generate them. same goes for finding overloads. 
 - look_for_templates() will not resolve function calls, it will just do pattern matching on TemplatedType. 
 - create a new function can_initialize_struct(). This will take the place of the circular dependency checker in Program. 
   should call this before we emit_initialize_struct(). 

ok, it works... kind of. Currently, I'm being waay too loose on checking for ambiguous functions generated when resolving
templated functions. Also, OperatorOverload should not be a child class of Function, it should be its own thing, like 
Constructor. Then, I can implement templated overloads much like templated functions.

is it sufficient to check that a function call cannot map to more than 1 templated function? I just want to avoid the situation
where two different templated functions generate the same signature. I'll try to prove it:

Suppose there exists two templated functions A, B, that both map to some function signature S. This means there is some assignment
of the templated types of A and B such that they both result in the same function signature. Since the resulting function signature
is exactly equal to the argument types in the case of templating, there must exist some function call such that it matches with
both A and B. 

Suppose there is some function call that matches with both A and B. It's trivial to see that A and B can map to the same function
signature. 

Now, the question is should I raise an error only when there exists some function that maps to two templated functions, or
should I check for it preemptively? How would I even check for it? Probably checking for it preemtpively is the better option. 

The problem I need to solve is called unification, there exists a nice algorithm called 'Robinson's Unification Algorithm'
I should do pairwise checks for every templated function and overload to make sure they can't resolve to the same signature. 
I should also check templated functions and overloads against their untemplated counterparts to make sure the templated versions
can't resolve to an untemplated version. 

Also, should probably change the type grammar to only allow references on the outermost layer, and move OperatorSignature outside
of utils. One thing that I'm struggling with is whether or not to treat Overload like an operator, or like a function, as currently
it's sitting somewhere in between, Overload is more like a function, while OverloadCall is more like an operator. 

So, I still need to implement more generous function call resolution with partial ordering of the function definitions. 

fix function call resolution
- want to be able to have both 'void foo(i32 a)' and 'void foo(i32& a)'. Then, foo(0) would resolve to the first one and foo(x) would
    resolve to the second one. 
- ok, let's not worry about references for now, the bigger issue is templating. For each function call, I want to find a unique
    templated function to match to. 
- define S(A) as the set of function calls that a function A can match to. 
- suppose we have two functions A and B. I define A < B if S(A) \subset S(B). Then for a function call F, gather all functions A_i
    such that F \in S(A_i). Then, we have a unique function if there exists some A_i where A_i < A_j for all other j. 
- when doing template comparisons, we remove reference wrappers, that is T and T& are identical when doing this. 
- should probably not make them identical... what if a function call has an r-value argument? Then we might find some minimal function, but
    it doesn't work because the parameter is an l-value
- ok, we first strip all references off of the top layer. Then, have some utility that takes a function signature and some
    arguments and lets us know if the method is callable using those arguments. 
- so to find a called function, a minimal function must exist for the call, and that minimal function must be callable by the 
    given arguments. 
- what if we just make it so that no two functions can share the same name ... no, that would be way too restrictive
- ok, have template specialization implemented, however still am not handling references properly. 
- T maps to anything T& can map to. T& is strictly more restrictive. T& only handles l-values, T handles both l-values and r-values
- what about when generating the mappings, we convert any l-value arguments into T& in the arg_types list. Then, T in the fc can map to both T& and T, however
  T& can only map to T&. This is done as pre-check before we strip all references. 
- to ensure we don't accidentally duplicate functions, we can't just shove all generated functions in a pool, need to do stricter partial ordering stuff
- or maybe we can prove that we can use the current method
- OKOK FIXED YAYY (still need to fix overload resolution)

ideas to optimize compilation times
 - an expression node should only resolve to one type. Perhaps just cache the result whenever we generate it. 
   - need to watch out when using expressions abnormally. Expressions can resolve to different types if we take the
     identifiers out of context. 
   - we then also need to ensure we make copies of everything. 

some miscellaneous features:
 - pointer arithmetic
 - array literals
 - better semantic error messages. 
   - would be nice if on failure, could print out the relevant code or smth. 
   - how would we even do that? perhaps take advantage of the 'to_string()'?
   - would need to store line information along with everything D:
 - default struct member variable declarations
   - this would be implemented in emit_initialize_struct(), right after we call each member variable's default constructor
 - multi declarations, we can separate multiple variable declarations on the same line with commas
 - expression comma operator
   - will need to fix function/constructor call grammar to not interfere with commas separating function call arguments. 
   - and eventually also multi declarations as well
 - implement const
 - have some reserved keywords (break, continue, sizeof)
   - these should be kept seperately? check basetypes, identifiers against keywords?
 - goto statement
 - make id_to_type() return a bool so that it doesn't fail an assert when a variable doesn't exist
 - extension to inline assembly: have a way to print out the address of any local (or global) variable. 
 - templated function calls? like hash<T>(T a)? as an alternative to automated resolution
 - think about how to handle user defined typecasts (and typecasts in general). Perhaps typecasting
   shouldn't be treated the same as other operators. The input type has to exactly match, and the
   output type cannot be a reference type. 
 - think about if I want auto type conversions on function calls, overloads, assignments, etc. Seems like it will make writing code much easier
 - namespaces
   - maybe for now, make it so that all references to functions must be fully qualified. That is, namespaces
     act as part of the identifier. 

implemented misc features:
 - replace type grammar with this:
base_type = alpha , { alpha | digit | "_" } ;
templated_type = base_type , [ "<" , ows , templated_type , { ows , "," , ows , templated_type } , ows , ">" ] , { "*" }
type = templated_type , [ "&" ] ;
   I don't want reference types contained within templates, or pointed to. Reference should be the outer layer of a type. 
   Probably should enforce this semantically. 
   So really, a type doesn't contain references, but we can have a reference to a type...
   Actually, I don't see why we can't have type vector<int&>. After all, references are simply syntactic sugar for
   pointers that get auto-dereferenced. But to make things simpler, I should probably prohibit this behaviour. 
 - primitive constructors
 - cast operator
 - reference operator
 - change int, float into i64 - i8, u64 - u8, f32.
 - floats
 - importing other files
 - anything that takes in a truth value should be able to accept anything. Just check if the register is 0.
 - inline assembly
 - continue, break (loop control statements)
   - keep a stack to maintain the nearest exit label of the loop we're in
   - also need to keep track of how many local variables we need to deallocate
   - these should be control statements, so they shouldn't have to deallocate any temp stuff
 - global variables
   - the order in which we initialize global variables should not matter. 
   - we can achieve this by not registering global variables as variables until we initialize all of them. 
   - but we might want some global variables to depend on others, like 'cout' might depend on 'STDOUT'
   - we can introduce 'tiers'. We initialize all globals of one tier before moving onto the next one. 
     so now, the ordering in which we initialize global variables within a tier should not matter. 
 - add a 'syscall()' builtin expression primary that can take in a bunch of arguments and emit code to do the syscall. 
   - will have to think about how to record the return value tho
   - perhaps can pass in a type as the last argument and that will be the assumed return type?
   - 'syscall(<syscall number>, <return type>, <arg list>)'
   - we won't do any checking on the syscall number and argument types. That's the user's issue. 
 - after implementing free, add struct destructors. 
   - currently, I assume that cleaning up the local variable declaration stack does not affect any
     of the registers. This will no longer be true after implementing struct destructors.
   - examples where I use this assumption are in FunctionCall::emit_asm() and SyscallLiteral::emit_asm() 
     - perhaps can make it so that pop_declaration_stack() specifically saves %rax, %rcx. This is enough for 
       expression evaluation to still work. 
   - variables should get destructed when they go out of scope. Should modify pop_declaration_stack() to do this. 
   - will also have to think about how to free temp variables. 
     - primitives (i32, f32, etc.) are simple and don't need destruction
     - overloaded operators and constructor calls are already wrapped in function calls during the elaboration phase
     - therefore, any temporary structs produced within subexpressions will have been bound to named temporaries or returned 
       from an overload/constructor, which means the only destructible temporary I need to explicitly handle is the final 
       value on the right-hand side of an assignment if it is an rvalue struct
     - actually, I don't even need to specifically handle these temp variables at all, as in the '=' operator, I bind 
       the temp variable to a named temporary reference, so if I just handle cleaning up named variables then I should be fine. 
   - use mmap and munmap syscalls to implement malloc and free. 
   - okok, now we just have to clean up unused r-values from expressions.
     - i changed the default assignment operator to instead return an l-value. So now, if I see any leftover
       r-values, I can confidently just clean them up. 
 - array type. Array types should not be primitive. We can treat them like structs, they'll have layouts, 
   constructors, destructors. 
   - T[<int>] a = new T[<int>]();
   - so the size of the array is part of the type information. 
   - This also means we can only have fixed size arrays
   - array layouts, constructors, destructors, should be added when we lookup if the type exists. 
     - we should always lookup if a type exists before using it right??
     - or maybe, we can add array layouts, constructors, destructors on the fly. We just have to make sure
       their base type exists.
     - maybe can hijack find_templates() and use that to also generate array types?
   - ok, made array struct layouts lazily generated when calling get_struct_layout()
 - default declarations, so the expression portion of a declaration should be optional
 - syntax error reporting, keep track of the deepest parse.
   - also have some pretty printing when parse fails pointing to exact deepest parse position 
 - make string literals point to rodata instead of allocating more memory every time. 
   - should maybe consider implementing const? so a string literal would be of type const u8*
   - instead of string literals emitting their current initialization, I'll just have them load a pointer onto the stack
     and load into the controller that we need to allocate that string. Then after everything has been emitted, put the
     .data section and actually create the strings. 
 - autogenerated default and copy constructors, destructors. Just initialize everything to 0 and copy the struct memory blindly
 - 'kernel mode'
   - in the kernel, we don't have access to malloc, free, sys_exit(). Should replace these calls with other stuff.
   - can't use rip relative addressing
   - maybe should emit some alignment code?
 - unsigned integer literals in different bases
   - hex literals, like 0x00FFFFFF
   - binary literals, like 0b000101
 - extern, ability to define variables read directly from memory
   - extern in C allows to reference some variable that can be defined anywhere else. 
   - in my case, I just want to use extern to access variables that are defined using labels in linked assembly. 
   - a label represents an address, if you use extern, you simply assume that the given address is defined elsewhere and
     use it as if it's defined. 
   - should probably give some error if the extern label conflicts with any existing label. (have a bank of used labels)
   - should probably give a warning if the extern label uses my label naming scheme (not too sure about this one)
   - an extern variable should always be a pointer? so 'extern T* x;'
   - externs are going to be put into the controller before globals?
   - no, externs are just going to have different memory addresses compared to their stack declared counterparts. 
     otherwise, they'll behave exactly the same. 
 - function pointers, so we can pass in interrupt handlers and stuff. 
   - function pointer type : 'fn<i32(i32, i32)>'
 - construct-in-place operator ':='
 - manual destructor calls (very hardcoded and jank (holy moly jank!!!) right now)
 - better global variable dependency stuff
   - ability to declare nodes, can bind variables to nodes. 
   - nodes can depend on other nodes
   - '#global_node <id> [<id_list>]'
   - force the user to initialize all their nodes to avoid typos
   - allow the user to not bind a global to a node, these globals will be initialized last
   - maybe have a label that is guaranteed to initialize first? '__GLOBAL_FIRST__'. It's illegal to make it 
     depend on anything else. 
 - typedefs. Just have them be pretty much resolved template variables
   - 'typedef <type> <basetype>'
   - I just need to check typedefs against all existing basetypes right? can't reuse a basetype
   - in C++, typedefs can depend on eachother. Either have to make the same system with global variables, or just say they can't depend on eachother. 
   - since typedefs are relatively self contained, we can do dynamic typedef resolution. For each typedef, we
     figure out what typedefs it depends on. Then, topological sort all the typedefs and then turn them all into 
     <defined_type> -> <type> mappings. Finally, just do a lil replace templated types on the entire program. 
   - we'll need to do this step after we register all the structs so we can tell what's an existing type 
   - typedef types should be basetypes. 
 - function export modifier. Makes it so that generated label equals function id

Struct member functions should be called with 'this' as a pointer to the target struct. 
The actual location of the struct is hard to control relative to %rbp. This makes it so I can 
allocate the actual struct wherever I want, and still be able to call constructors and 
member functions. 

So ConstructorCall::emit_asm() and FunctionCall::emit_asm() (that have target struct) require that
%rax have the target struct's address. We also don't need special handling to access member variables
anymore. 

ok, made it so that struct heap memory is contiguous now, and we also are keeping the old pointer model 
for struct expression related stuff. The idea is that on the stack, struct value types (T a) and pointer 
types (T* a) are exactly identical. So when I dereference T*, I actually just do nothing. 

I'm running into an issue where when I assign some struct to another struct, I can't just malloc more 
memory and point the assigned struct to the newly generated struct, the assigned struct needs to be generated
in the place where it was originally. So on assignment, we use the non-allocating constructor call to
first overwrite the old struct, and then pass in the mem pointer to the copy constructor. 

Ok, rewrote struct memory layouts so that they're now contiguous on the heap. Also, arrays of structs will 
also be contiguous. It's just structs on the stack will be represented by pointers, so T and T* are identical on the stack.
(by data, not semantically). 


WRITE MORE TESTS!!


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