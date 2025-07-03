#pragma once
#include "../parser/parser.h"
#include <vector>

struct StructDefinition;
struct Function;
struct Overload;

struct TemplatedStructDefinition;
struct TemplatedFunction;
struct TemplatedOverload;

struct Include;

struct Program {
    std::vector<StructDefinition*> structs;
    std::vector<Function*> functions;
    std::vector<Overload*> overloads;

    std::vector<TemplatedStructDefinition*> templated_structs;
    std::vector<TemplatedFunction*> templated_functions;
    std::vector<TemplatedOverload*> templated_overloads;

    std::vector<Include*> includes;

    Program(); 
    Program(std::vector<StructDefinition*> _structs, std::vector<Function*> _functions, std::vector<TemplatedStructDefinition*> _templated_structs, std::vector<TemplatedFunction*> _templated_functions, std::vector<Overload*> overloads, std::vector<TemplatedOverload*> templated_overloads, std::vector<Include*> includes);

    void add_all(Program *other);
    static Program* convert(parser::program *p);
    bool is_well_formed();
};