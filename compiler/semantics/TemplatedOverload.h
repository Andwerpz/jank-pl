#pragma once
#include "../parser/parser.h"
#include <optional>

struct TemplateHeader;
struct Overload;
struct OverloadCall;
struct OperatorSignature;
struct TemplateMapping;
struct Expression;

struct TemplatedOverload {
    TemplateHeader *header;
    Overload *overload;

    TemplatedOverload(TemplateHeader *_header, Overload *_overload);

    static TemplatedOverload* convert(parser::templated_overload *o);

    bool is_well_formed();
    TemplateMapping* calc_mapping(std::optional<Expression*> left, std::string op, std::optional<Expression*> right);
    Overload* gen_overload(std::optional<Expression*> left, std::string op, std::optional<Expression*> right);
};