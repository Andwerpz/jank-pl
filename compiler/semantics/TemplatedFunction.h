#pragma once
#include "../parser/parser.h"

struct TemplateHeader;
struct Function;
struct FunctionCall;
struct OperatorSignature;
struct OperatorOverload;
struct TemplateMapping;
struct Type;

struct TemplatedFunction {
    TemplateHeader *header;
    Function *function;
    std::vector<Function*> generated_functions;

    TemplatedFunction(TemplateHeader *_header, Function *_function);

    static TemplatedFunction* convert(parser::templated_function *f);

    bool is_well_formed();
    bool is_main();
    TemplateMapping* calc_mapping(std::vector<Type*> arg_types);
    TemplateMapping* calc_mapping(FunctionCall *fc);
    Function* gen_function(FunctionCall *fc);

    bool replace_templated_types(TemplateMapping *mapping);

    TemplatedFunction* make_copy();
};