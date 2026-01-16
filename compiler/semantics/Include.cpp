#include "Include.h"
#include "Literal.h"
#include "Identifier.h"

Include::Include(std::string _path, bool _is_library_include) {
    path = _path;
    is_library_include = _is_library_include;
}   

Include* Include::convert(parser::include *inc) {
    if(inc->t2->is_b0) {
        std::string path = inc->t2->t0->to_string();
        assert(path.size() >= 2);
        assert(path[0] == '"' && path[path.size() - 1] == '"');
        return new Include(path.substr(1, path.size() - 2), false);
    }
    else if(inc->t2->is_b1){
        std::string path = inc->t2->t1->to_string();
        assert(path.size() >= 2);
        assert(path[0] == '<' && path[path.size() - 1] == '>');
        return new Include(path.substr(1, path.size() - 2), true);
    }
    else assert(false);
}