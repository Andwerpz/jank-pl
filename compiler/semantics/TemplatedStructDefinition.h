#pragma once
#include "../parser/parser.h"

struct TemplateHeader;
struct StructDefinition;
struct TemplatedType;
struct TemplateMapping;

struct TemplatedStructDefinition {  
    TemplateHeader* header;
    StructDefinition* struct_def;

    TemplatedStructDefinition(TemplateHeader *_header, StructDefinition *_struct_def);

    static TemplatedStructDefinition* convert(parser::templated_struct_definition *s);

    bool is_well_formed();
    TemplateMapping* calc_mapping(TemplatedType *type);
    StructDefinition* gen_struct_def(TemplatedType* type);

    bool replace_templated_types(TemplateMapping *mapping);
};