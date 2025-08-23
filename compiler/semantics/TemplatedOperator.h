#pragma once
#include "../parser/parser.h"
#include <optional>

struct TemplateHeader;
struct Operator;
struct OperatorCall;
struct OperatorSignature;
struct TemplateMapping;
struct Expression;

struct TemplatedOperator {
    TemplateHeader *header;
    Operator *op;   //bruh, operator is reserved
    std::vector<Operator*> generated_operators;

    TemplatedOperator(TemplateHeader *_header, Operator *_op);

    static TemplatedOperator* convert(parser::templated_overload *o);

    bool is_well_formed();
    TemplateMapping* calc_mapping(OperatorSignature *os);
    TemplateMapping* calc_mapping(OperatorCall *oc);
    Operator* gen_operator(OperatorCall *oc);

    bool replace_templated_types(TemplateMapping *mapping);
};