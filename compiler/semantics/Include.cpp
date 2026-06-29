#include "Include.h"
#include "Literal.h"
#include "Identifier.h"

Include::Include(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Include::Include(const Include& other) : ASTNode(other) {
    is_library_include = other.is_library_include;
    path = other.path;
}

Include::Include(std::string _path, bool _is_library_include) : ASTNode() {
    path = _path;
    is_library_include = _is_library_include;
}   

Include* Include::convert(parser::include *inc) {
    Include* result = new Include(inc);
    if(inc->t2->is_b0) {
        result->is_library_include = false;
        std::string path_str = inc->t2->t0->to_string();
        assert(path_str.size() >= 2);
        assert(path_str[0] == '"' && path_str[path_str.size() - 1] == '"');
        result->path = path_str.substr(1, path_str.size() - 2);
    }
    else if(inc->t2->is_b1){
        result->is_library_include = true;
        std::string path_str = inc->t2->t1->to_string();
        assert(path_str.size() >= 2);
        assert(path_str[0] == '<' && path_str[path_str.size() - 1] == '>');
        result->path = path_str.substr(1, path_str.size() - 2);
    }
    else assert(false);
    return result;
}

Include* Include::make_copy() {
    return new Include(*this);
}