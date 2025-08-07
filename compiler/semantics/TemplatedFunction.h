#pragma once
#include "../parser/parser.h"

struct TemplateHeader;
struct Function;
struct FunctionCall;
struct OperatorSignature;
struct OperatorOverload;
struct TemplateMapping;

struct TemplatedFunction {
    TemplateHeader *header;
    Function *function;

    TemplatedFunction(TemplateHeader *_header, Function *_function);

    static TemplatedFunction* convert(parser::templated_function *f);

    bool is_well_formed();
    TemplateMapping* calc_mapping(FunctionCall *fc);
    Function* gen_function(FunctionCall *fc);

    bool replace_templated_types(TemplateMapping *mapping);
};