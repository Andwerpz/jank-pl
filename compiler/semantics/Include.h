#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct Include : public ASTNode {
    bool is_library_include;
    std::string path;
    
    Include(parser::token* tok);
    Include(const Include& other);
    Include(std::string _path, bool _is_library_include);

    static Include* convert(parser::include* inc);
    Include* make_copy() override;
};