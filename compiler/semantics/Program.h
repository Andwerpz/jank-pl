#pragma once
#include "../parser/parser.h"
#include <vector>

struct StructDefinition;
struct Function;
struct Overload;

struct TemplatedStructDefinition;
struct TemplatedFunction;
struct TemplatedOverload;

struct Program {
    std::vector<StructDefinition*> structs;
    std::vector<Function*> functions;
    std::vector<Overload*> overloads;

    std::vector<TemplatedStructDefinition*> templated_structs;
    std::vector<TemplatedFunction*> templated_functions;
    std::vector<TemplatedOverload*> templated_overloads;

    Program(std::vector<StructDefinition*> _structs, std::vector<Function*> _functions, std::vector<TemplatedStructDefinition*> _templated_structs, std::vector<TemplatedFunction*> _templated_functions, std::vector<Overload*> overloads, std::vector<TemplatedOverload*> templated_overloads);

    static Program* convert(parser::program *p);
    bool is_well_formed();
};