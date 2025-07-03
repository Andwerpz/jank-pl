#include "Include.h"
#include "Literal.h"
#include "Identifier.h"

Include::Include(std::string _path, bool _is_library_include) {
    path = _path;
    is_library_include = _is_library_include;
}   

Include* Include::convert(parser::include *inc) {
    if(inc->t2->is_b0) {
        std::string path = StringLiteral::convert(inc->t2->t0->t0)->val;
        return new Include(path, false);
    }
    else if(inc->t2->is_b1){
        std::string path = Identifier::convert(inc->t2->t1->t1)->name;
        return new Include(path, true);
    }
    else assert(false);
}