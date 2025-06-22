#include "TemplatedStructDefinition.h"
#include "Type.h"
#include "TemplateHeader.h"
#include "TemplateMapping.h"
#include "utils.h"
#include "StructDefinition.h"

TemplatedStructDefinition::TemplatedStructDefinition(TemplateHeader *_header, StructDefinition *_struct_def) {
    header = _header;
    struct_def = _struct_def;
}

TemplatedStructDefinition* TemplatedStructDefinition::convert(parser::templated_struct_definition *s) {
    TemplateHeader *header = TemplateHeader::convert(s->t0);
    StructDefinition *struct_def = StructDefinition::convert(s->t2);
    return new TemplatedStructDefinition(header, struct_def);
}

bool TemplatedStructDefinition::is_well_formed() {
    // - are all of the templated basetypes not declared?
    for(int i = 0; i < header->types.size(); i++){
        if(is_basetype_declared(header->types[i])) {
            std::cout << "Template basetype " << header->types[i]->to_string() << " already declared\n";
            return false;
        }
    }

    // - are there any duplicate template types?
    for(int i = 0; i < header->types.size(); i++){
        for(int j = 0; j < i; j++){
            if(header->types[i]->equals(header->types[j])) {
                std::cout << "Duplicate template basetype : " << header->types[i]->to_string() << "\n";
                return false;
            }
        }
    }

    return true;
}

TemplateMapping* TemplatedStructDefinition::calc_mapping(TemplatedType *type) {
    TemplateMapping *mapping = new TemplateMapping();
    // - does the base type match up?
    if(!this->struct_def->type->equals(type->base_type)) {
        return nullptr;
    }

    // - does the amount of template types match up?
    if(type->template_types.size() != header->types.size()) {
        return nullptr;
    }

    //a mapping should always exist
    for(int i = 0; i < header->types.size(); i++){
        assert(mapping->add_mapping(header->types[i], type->template_types[i]));
    }
    assert(mapping->mapping.size() == header->types.size());

    return mapping;
}   

StructDefinition* TemplatedStructDefinition::gen_struct_def(TemplatedType* type) {
    // - is templated type well formed?
    if(!is_templated_type_well_formed(type)) {
        return nullptr;
    }

    // - is there a mapping?
    TemplateMapping *mapping = this->calc_mapping(type);
    if(mapping == nullptr) return nullptr;

    // - replace struct basetype with templated version
    assert(mapping->add_mapping(this->struct_def->type, type));

    //try to construct
    StructDefinition *n_struct_def = this->struct_def->make_copy();
    if(!n_struct_def->replace_templated_types(mapping)) return nullptr;
    return n_struct_def;
}

