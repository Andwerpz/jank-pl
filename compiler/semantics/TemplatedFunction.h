#pragma once
#include "../parser/parser.h"

struct TemplateHeader;
struct Function;
struct FunctionCall;
struct OperatorSignature;
struct OperatorOverload;

struct TemplatedFunction {
    TemplateHeader *header;
    Function *function;

    TemplatedFunction(TemplateHeader *_header, Function *_function);

    static TemplatedFunction* convert(parser::templated_function *f);

    bool is_match(FunctionCall *fc);
    Function* gen_function(FunctionCall *fc);

    bool is_match(OperatorSignature *os);
    OperatorOverload* gen_overload(OperatorSignature *os);
};