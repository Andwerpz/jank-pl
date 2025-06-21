#pragma once
#include "../parser/parser.h"
#include <vector>

struct StructDefinition;
struct Function;

struct TemplatedStructDefinition;
struct TemplatedFunction;

struct Program {
    std::vector<StructDefinition*> structs;
    std::vector<Function*> functions;

    std::vector<TemplatedStructDefinition*> templated_structs;
    std::vector<TemplatedFunction*> templated_functions;

    Program(std::vector<StructDefinition*> _structs, std::vector<Function*> _functions, std::vector<TemplatedStructDefinition*> _templated_structs, std::vector<TemplatedFunction*> _templated_functions);

    static Program* convert(parser::program *p);
    bool is_well_formed();
};