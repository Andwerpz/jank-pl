#pragma once
#include "../parser/parser.h"

struct TemplateHeader;
struct TemplateMapping;

struct Type {
    static Type* convert(parser::type *t);
    static Type* convert(parser::templated_type *t);
    
    Type* remove_reference();

    virtual int calc_size() = 0;
    virtual bool equals(const Type *other) const = 0;
    bool operator==(const Type& other) const;
    bool operator!=(const Type& other) const;
    virtual size_t hash() const = 0;
    virtual std::string to_string() = 0;
    virtual Type* make_copy() = 0;
    virtual bool replace_templated_types(TemplateMapping *mapping) = 0;
    virtual bool look_for_templates() = 0;
    virtual TemplateMapping* generate_mapping(Type *t, TemplateHeader *header) = 0;
};

struct BaseType : public Type {
    std::string name;
    BaseType(std::string _name);
    static BaseType* convert(parser::base_type *t);

    int calc_size() override;
    bool equals(const Type *other) const override;
    size_t hash() const override;
    std::string to_string() override;
    Type* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    TemplateMapping* generate_mapping(Type *t, TemplateHeader *header) override;
};  

struct PointerType : public Type {
    Type *type;
    PointerType(Type *_type);

    int calc_size() override;
    bool equals(const Type *other) const override;
    size_t hash() const override;
    std::string to_string() override;
    Type* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    TemplateMapping* generate_mapping(Type *t, TemplateHeader *header) override;
};

struct ReferenceType : public Type {
    Type *type;
    ReferenceType(Type *_type);

    int calc_size() override;
    bool equals(const Type *other) const override;
    size_t hash() const override;
    std::string to_string() override;
    Type* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    TemplateMapping* generate_mapping(Type *t, TemplateHeader *header) override;
};

struct TemplatedType : public Type {
    BaseType *base_type;
    std::vector<Type*> template_types;
    TemplatedType(BaseType *_base_type, std::vector<Type*> _template_types);

    int calc_size() override;
    bool equals(const Type *other) const override;
    size_t hash() const override;
    std::string to_string() override;
    Type* make_copy() override;
    bool replace_templated_types(TemplateMapping *mapping) override;
    bool look_for_templates() override;
    TemplateMapping* generate_mapping(Type *t, TemplateHeader *header) override;
};