#pragma once
#include "../parser/parser.h"
#include <vector>

struct Type;

struct TemplateHeader {
    std::vector<Type*> types;
    TemplateHeader(std::vector<Type*> _types);
};