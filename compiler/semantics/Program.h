#pragma once
#include "../parser/parser.h"
#include <vector>

struct StructDefinition;
struct Function;

struct Program {
    std::vector<StructDefinition*> structs;
    std::vector<Function*> functions;

    Program(std::vector<StructDefinition*> _structs, std::vector<Function*> _functions);

    static Program* convert(parser::program *p);
    bool is_well_formed();
};