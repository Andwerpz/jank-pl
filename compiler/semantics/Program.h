#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"
#include <vector>

struct StructDefinition;
struct Function;
struct Overload;

struct TemplatedStructDefinition;
struct TemplatedFunction;
struct TemplatedOperator;

struct Include;

struct GlobalDeclaration;
struct GlobalNode;

struct Typedef;

struct Program : public ASTNode {
    std::vector<TemplatedStructDefinition*> templated_structs;
    std::vector<TemplatedFunction*> templated_functions;
    std::vector<TemplatedOperator*> templated_operators;

    std::vector<Include*> includes;

    std::vector<GlobalDeclaration*> global_declarations;
    std::vector<GlobalNode*> global_nodes;

    std::vector<Typedef*> typedefs;

    Program(parser::token *tok);
    Program(const Program& other);
    Program(
        std::vector<TemplatedStructDefinition*> _templated_structs, 
        std::vector<TemplatedFunction*> _templated_functions, 
        std::vector<TemplatedOperator*> _templated_operators, 
        std::vector<Include*> _includes, 
        std::vector<GlobalDeclaration*> _global_declarations, 
        std::vector<GlobalNode*> _global_nodes, 
        std::vector<Typedef*> _typedefs
    );
    Program();

    static Program* convert(parser::program *p);
    Program* make_copy() override;
};