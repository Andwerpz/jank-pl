#include "Include.h"
#include "Literal.h"
#include "Identifier.h"

Include::Include(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Include::Include(const Include& other) : ASTNode(other) {
    is_library_include = other.is_library_include;
    package = other.package;
    path = other.path;
}

Include::Include(bool _is_library_include, std::optional<std::string> _package, std::string _path) : ASTNode() {
    is_library_include = _is_library_include;
    package = _package;
    path = _path;

    if(package.has_value()) {
        assert(is_library_include);
    }
}   

Include* Include::convert(parser::include *inc) {
    Include* result = new Include(inc);
    if(inc->t2->is_b0) {
        result->is_library_include = false;
        result->package = std::nullopt;
        parser::include_path_relative* inc_rel = inc->t2->t0->t0;
        result->path = inc_rel->t1->to_string();
    }
    else if(inc->t2->is_b1){
        result->is_library_include = true;
        parser::include_path_lib* inc_lib = inc->t2->t1->t0;
        result->package = std::nullopt;
        if(inc_lib->t1.has_value()) {
            result->package = inc_lib->t1.value()->t0->to_string();
        }
        result->path = inc_lib->t2->to_string();
    }
    else assert(false);
    return result;
}

Include* Include::make_copy() {
    return new Include(*this);
}

bool Include::equals(const Include* other) const {
    return 
        this->is_library_include == other->is_library_include && 
        this->package == other->package &&
        this->path == other->path
    ;   
}

std::string Include::to_string() const {
    std::string ret = "#include ";
    ret += is_library_include? "<" : "\"";
    if(package.has_value()) ret += package.value() + "::";
    ret += path;
    ret += is_library_include? ">" : "\"";
    return ret;
}