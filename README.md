<h1><img src="janky.png" alt="Logo" style="height: 1em; vertical-align: middle;"> jank</h1>

**jank** is an experimental programming language built from scratch in C++. Currently, the compiler just emits x86‑64 assembly (AT&T syntax) and uses gcc to generate the executables. 

In addition to the language and compiler, the project aims to include a fully custom assembler, allowing complete control over the compilation toolchain - from source code to raw machine code.

Features include:

- Recursive-descent parser
- AST-based semantic analysis
- Structs, functions, operator overloads
- Templating as macros
- Manual memory layout and stack management
- SSE-based float operations
- Basic import system and global variables
- Rudimentary inline assembly
- C/C++ style typedefs

## Setup 

Tested on WSL.

Make sure you have `gcc` and `make` installed. Go into the `compiler` directory and run `make all`. It should create `jjc.exe`. If you want to add it to PATH, you can run `make install`. 

To test your installation, go into `testing` and run `make all`. Then, run the generated executable: `./main.exe run-tests`. 


