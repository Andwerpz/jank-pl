#pragma once
#include <vector>

struct Type;

struct TemplateMapping {
    std::vector<std::pair<Type*, Type*>> mapping;

    TemplateMapping();
    TemplateMapping(std::vector<std::pair<Type*, Type*>> _mapping);
    
    Type* find_mapped_type(Type *template_type);
    bool add_mapping(Type *template_type, Type *mapped_type);
};