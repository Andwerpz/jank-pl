#pragma once
#include "../parser/parser.h"

struct Include {
    bool is_library_include;
    std::string path;

    Include(std::string _path, bool _is_library_include);

    static Include* convert(parser::include *inc);
};