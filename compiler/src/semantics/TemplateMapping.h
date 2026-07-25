#pragma once
#include <vector>
#include <string>

struct Type;
struct BaseType;

struct TemplateMapping {
    // first is always of type BaseType, is mapping 'key'
    // second can be an arbitrary Type, is mapping 'value'
    std::vector<std::pair<Type*, Type*>> mapping;

    TemplateMapping();
    TemplateMapping(std::vector<std::pair<Type*, Type*>> _mapping);
    
    Type* find_mapped_type(Type *template_type);
    bool add_mapping(Type *template_type, Type *mapped_type);
    bool merge_with_mapping(TemplateMapping *other);
    std::string to_string();
    void find_all_basetypes(std::vector<BaseType*> &out);
};