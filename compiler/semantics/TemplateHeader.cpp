#include "TemplateHeader.h"
#include "Type.h"

TemplateHeader::TemplateHeader(std::vector<BaseType*> _types) {
    types = _types;
}

TemplateHeader* TemplateHeader::convert(parser::template_header *h) {
    std::vector<BaseType*> types;
    types.push_back(BaseType::convert(h->t4));
    for(int i = 0; i < h->t5.size(); i++){
        types.push_back(BaseType::convert(h->t5[i]->t3));
    }
    return new TemplateHeader(types);
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