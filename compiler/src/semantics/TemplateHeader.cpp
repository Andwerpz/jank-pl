#include "TemplateHeader.h"
#include "Type.h"

TemplateHeader::TemplateHeader(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

TemplateHeader::TemplateHeader(const TemplateHeader& other) : ASTNode(other) {
    for(int i = 0; i < other.types.size(); i++) {
        types.push_back(dynamic_cast<BaseType*>(other.types[i]->make_copy()));
    }
}

TemplateHeader::TemplateHeader(std::vector<BaseType*> _types) : ASTNode() {
    types = _types;
}

TemplateHeader* TemplateHeader::convert(parser::template_header *h) {
    TemplateHeader* result = new TemplateHeader(h);
    result->types.push_back(BaseType::convert(h->t4));
    for(int i = 0; i < h->t5.size(); i++){
        result->types.push_back(BaseType::convert(h->t5[i]->t3));
    }
    return result;
}

std::string TemplateHeader::to_string() {
    std::string res = "";
    res += "template";
    res += "<";
    for(int i = 0; i < types.size(); i++) {
        res += types[i]->to_string();
        if(i + 1 != types.size()) res += ", ";
    }
    res += ">";
    return res;
}

TemplateHeader* TemplateHeader::make_copy() {
    return new TemplateHeader(*this);
}