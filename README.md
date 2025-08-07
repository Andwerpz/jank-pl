<h1><img src="janky.png" alt="Logo" style="height: 1em; vertical-align: middle;"> jank</h1>

**jank** is an experimental systems programming language built from scratch in C++, designed for low-level development with C-like performance and control. The compiler emits x86‑64 assembly and uses GCC for linking. Future plans include a custom assembler for complete toolchain control.

## Features

- **C/C++ style syntax** with structs, functions, and operator overloads
- **Templates and generics** with compile-time expansion (`template<T>`)
- **Manual memory management** with explicit allocation/deallocation
- **Inline assembly** support with `asm!()` syntax
- **Function pointers** and references (`fn<T(U,V)>`, `&`, `*`)
- **Comprehensive type system** including primitives, arrays, and custom types
- **Type aliases** with C-style typedefs
- **Global dependency management** with `#global_node` system
- **Standard library** with containers (`vector`, `string`), I/O, and math functions
- **Limited error reporting** (work in progress)

## Setup 

Tested on WSL. Make sure you have `gcc` and `make` installed. 

```bash
cd compiler && make all      # Creates jjc.exe
make install                 # adds jjc to PATH
cd ../testing && make all    # Build test runner
./main.exe run-tests         # Verify installation
``` 

## Usage

```bash
# Compile a .jank file
jjc source.jank

# Generate assembly output
jjc source.jank -S -o output.s

# Kernel mode compilation
jjc source.jank -k

# Debug mode with helpful(?) comments in assembly
jjc source.jank -S -ad
```

## Examples

See the `testing/tests/` directory for examples covering (hopefully) all language features. 

## Projects

- **Wolf3D** - A raycasting demo showcasing graphics programming in jank (`wolf3D/`)
- **jank-os** - A learning operating system built with jank ([GitHub](https://github.com/Andwerpz/jank-os))

