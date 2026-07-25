#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct Include : public ASTNode {
    bool is_library_include;
    std::optional<std::string> package;
    std::string path;
    
    Include(parser::token* tok);
    Include(const Include& other);
    Include(bool _is_library_include, std::optional<std::string> _package, std::string _path);

    static Include* convert(parser::include* inc);
    Include* make_copy() override;
    bool equals(const Include* other) const;
    std::string to_string() const;
};