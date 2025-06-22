#include "TemplateMapping.h"
#include "Type.h"

TemplateMapping::TemplateMapping() {
    mapping = std::vector<std::pair<Type*, Type*>>(0);
}

TemplateMapping::TemplateMapping(std::vector<std::pair<Type*, Type*>> _mapping) {
    mapping = _mapping;
}

Type* TemplateMapping::find_mapped_type(Type *template_type) {
    for(int i = 0; i < mapping.size(); i++){
        if(mapping[i].first->equals(template_type)) {
            return mapping[i].second->make_copy();
        }
    }
    return nullptr;
}

bool TemplateMapping::add_mapping(Type *template_type, Type *mapped_type) {
    //check if already exists
    if(this->find_mapped_type(template_type) != nullptr) {
        return false;
    }
    //insert mapping
    mapping.push_back(std::make_pair(template_type->make_copy(), mapped_type->make_copy()));
    return true;
}

