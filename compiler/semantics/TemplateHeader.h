#pragma once
#include "../parser/parser.h"
#include <vector>

struct Type;
struct BaseType;

struct TemplateHeader {
    std::vector<BaseType*> types;
    TemplateHeader(std::vector<BaseType*> _types);

    static TemplateHeader* convert(parser::template_header *h);
    std::string to_string();
    TemplateHeader* make_copy();
};