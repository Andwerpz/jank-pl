#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct TemplateHeader;
struct StructDefinition;
struct TemplatedType;
struct TemplateMapping;
struct CompilationContext;

struct TemplatedStructDefinition : public ASTNode {  
    TemplateHeader* header;
    StructDefinition* struct_def;

    TemplatedStructDefinition(parser::token *tok);
    TemplatedStructDefinition(const TemplatedStructDefinition& other);
    TemplatedStructDefinition(TemplateHeader *_header, StructDefinition *_struct_def);

    static TemplatedStructDefinition* convert(parser::templated_struct_definition *s);
    bool is_well_formed(CompilationContext* ctx);
    TemplateMapping* calc_mapping(TemplatedType *type);
    StructDefinition* gen_struct_def(CompilationContext *ctx, TemplatedType* type);
    bool replace_templated_types(TemplateMapping *mapping);
    TemplatedStructDefinition* make_copy() override;
};