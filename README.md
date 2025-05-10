# Jank Programming Language

## Description
I love Dylan Jank

## Abstract
Write a programming language. 🤯

## Goal
Andrew Li says: Learn how programming languages work under the hood, the programming language we create doesn't have to be very useful or good, it can be a clone of C. I just want to know how to make one.

### Restrictions
- No writing an interpreted language. More specifically, we must write our own compiler, and in the end produce executable machine code

### Overview
 - Lexical Analysis
   - Recognizing keywords, operators, constants, etc.
 - Parsing
   - Understanding the stream of tokens, putting them into some sort of structure (like a parse tree)
 - Semantic Analysis
   - Understanding the meaning of the parse tree. This is so that we can check for type consistency and other stuff like that.
 - Optimization
   - Implementing transparent optimizations to the code. For this project, this phase can be ignored. 
 - Machine Code Generation
   - Generating bytecode from our chosen instruction set. 

### Short Term Tasks:
 - figure out how to rigorously define a grammar. 
 - decide on the 'theme' for the language. Is it going to be a clone of C, or a performance improvement to Python? 
   - Maybe we do Java, but add operator overloading
 - pick a target OS and instruction set for the language (can one OS run on different processors with different instruction sets? Perhaps should revisit this later with more knowledge)
 - define a grammar for the language.  
 - (Important!!) decide on a name for the language
   - If we do the Java improvement, maybe Jank is a good name? (holy moly dylan janky??)

## Logs
### 5/8/2025
Ok, we're going to make a C clone targeted towards x86 Linux. I'm thinking that I just use EBNF to define my grammar. 

Hmm, seems like writing an EBNF parser is a project in of itself. I'll have to create a parser generator for EBNF, and then use the output to parse my stuff.

app.jank -> app.asm -> app.o
ASM syntax will be in [AT&T](https://en.wikipedia.org/wiki/X86_assembly_language#Syntax)

Assembler will convert ASM to [ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)

`gcc -nostartfiles -no-pie -o bello.o bello.s`

### 5/9/2025
Seems like making a greedy EBNF parser is the easiest way to go. This means that my grammar will be restricted to being unambiguous, but that is perfectly fine. 