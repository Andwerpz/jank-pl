#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct TemplateHeader;
struct Function;
struct FunctionCall;
struct OperatorSignature;
struct OperatorOverload;
struct TemplateMapping;
struct Type;
struct CompilationContext;

struct TemplatedFunction : public ASTNode {
    TemplateHeader *header;
    Function *function;
    std::vector<Function*> generated_functions;

    TemplatedFunction(parser::token *tok);
    TemplatedFunction(const TemplatedFunction& other);
    TemplatedFunction(TemplateHeader* _header, Function *_function);

    static TemplatedFunction* convert(parser::templated_function *f);
    bool is_well_formed(CompilationContext* ctx);
    bool is_main();
    TemplateMapping* calc_mapping(std::vector<Type*> arg_types);
    TemplateMapping* calc_mapping(CompilationContext *ctx, FunctionCall *fc);
    Function* gen_function(CompilationContext *ctx, FunctionCall *fc);
    bool replace_templated_types(TemplateMapping *mapping);
    TemplatedFunction* make_copy();
};