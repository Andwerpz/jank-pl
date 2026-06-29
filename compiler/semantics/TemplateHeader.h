#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"
#include <vector>

struct Type;
struct BaseType;

struct TemplateHeader : public ASTNode {
    std::vector<BaseType*> types;

    TemplateHeader(parser::token *tok);
    TemplateHeader(const TemplateHeader& other);
    TemplateHeader(std::vector<BaseType*> _types);

    static TemplateHeader* convert(parser::template_header *h);
    std::string to_string();
    TemplateHeader* make_copy();
};