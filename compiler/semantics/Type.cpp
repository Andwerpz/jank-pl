#include "Type.h"
#include "utils.h"
#include "TemplateMapping.h"
#include "TemplateHeader.h"

Type* Type::remove_reference() {
    Type *ret = this->make_copy();
    if(dynamic_cast<ReferenceType*>(ret)) ret = dynamic_cast<ReferenceType*>(ret)->type;
    return ret;
}

bool Type::operator==(const Type& other) const {
    return this->equals(&other);
}

bool Type::operator!=(const Type& other) const {
    return !this->equals(&other);
}

// -- CONSTRUCTOR --
BaseType::BaseType(std::string _name) {
    name = _name;
}

PointerType::PointerType(Type *_type) {
    assert(_type != nullptr);
    type = _type;
}

ReferenceType::ReferenceType(Type *_type) {
    assert(_type != nullptr);
    type = _type;
}

TemplatedType::TemplatedType(BaseType *_base_type, std::vector<Type*> _template_types) {
    assert(_base_type != nullptr);
    assert(_template_types.size() != 0);
    for(int i = 0; i < _template_types.size(); i++) assert(_template_types[i] != nullptr);
    base_type = _base_type;
    template_types = _template_types;
}

// -- EQUALS --
bool BaseType::equals(const Type *other) const {
    if(auto x = dynamic_cast<const BaseType*>(other)) return name == x->name;
    return false;
}

bool PointerType::equals(const Type *other) const {
    if(auto x = dynamic_cast<const PointerType*>(other)) return *type == *(x->type);
    return false;
}

bool ReferenceType::equals(const Type *other) const {
    if(auto x = dynamic_cast<const ReferenceType*>(other)) return *type == *(x->type);
    return false;
}

bool TemplatedType::equals(const Type *other) const {
    if(auto x = dynamic_cast<const TemplatedType*>(other)) {
        if(!base_type->equals(x->base_type)) return false;
        if(template_types.size() != x->template_types.size()) return false;
        for(int i = 0; i < template_types.size(); i++) if(!template_types[i]->equals(x->template_types[i])) return false;
        return true;
    }
    return false;
}

// -- HASH --
size_t BaseType::hash() const {
    return std::hash<std::string>()(name) ^ 0x9e3779b9;
}

size_t PointerType::hash() const {
    return type->hash() ^ 0x13952424;
}

size_t ReferenceType::hash() const {
    return type->hash() ^ 0xdeadbeef;
}

size_t TemplatedType::hash() const {
    size_t hash = 0;
    hash_combine(hash, base_type->hash());
    for(int i = 0; i < template_types.size(); i++) hash_combine(hash, template_types[i]->hash());
    return hash;
}

// -- TO STRING --
std::string BaseType::to_string() {
    return name;
}

std::string PointerType::to_string() {
    return type->to_string() + "*";
}

std::string ReferenceType::to_string() {
    return type->to_string() + "&";
}

std::string TemplatedType::to_string() {
    std::string res = "";
    res += base_type->to_string();
    res += "<";
    for(int i = 0; i < template_types.size(); i++) {
        res += template_types[i]->to_string();
        if(i + 1 != template_types.size()) res += ", ";
    }
    res += ">";
    return res;
}

// -- MAKE COPY --
Type* BaseType::make_copy() {
    return new BaseType(name);
}

Type* PointerType::make_copy() {
    return new PointerType(type->make_copy());
}

Type* ReferenceType::make_copy() {
    return new ReferenceType(type->make_copy());
}

Type* TemplatedType::make_copy() {
    std::vector<Type*> template_types_copy;
    for(int i = 0; i < template_types.size(); i++) {
        template_types_copy.push_back(template_types[i]->make_copy());
    }
    return new TemplatedType(dynamic_cast<BaseType*>(base_type->make_copy()), template_types_copy);
}

// -- CONVERT --
Type* Type::convert(parser::templated_type *t) {
    Type *res = BaseType::convert(t->t0);
    if(t->t1 != nullptr) {  //template types
        std::vector<Type*> template_types;
        template_types.push_back(Type::convert(t->t1->t2));
        for(int i = 0; i < t->t1->t3.size(); i++){
            template_types.push_back(Type::convert(t->t1->t3[i]->t3));
        }
        res = new TemplatedType(dynamic_cast<BaseType*>(res), template_types);
    }
    for(int i = 0; i < t->t2.size(); i++){
        res = new PointerType(res);
    }
    return res;
}

Type* Type::convert(parser::type *t) {
    Type *res = Type::convert(t->t0);
    if(t->t1 != nullptr) {
        res = new ReferenceType(res);
    }
    return res;
}

BaseType* BaseType::convert(parser::base_type *t) {
    return new BaseType(t->to_string());
}

// -- REPLACE TEMPLATED TYPES --
bool BaseType::replace_templated_types(TemplateMapping *mapping) {
    // do nothing
    return true;
}

bool PointerType::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) {type = x; return true;}
    else return type->replace_templated_types(mapping);
}

bool ReferenceType::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) {type = x; return true;}
    else return type->replace_templated_types(mapping);
}

bool TemplatedType::replace_templated_types(TemplateMapping *mapping) {
    //right now, I'm not supporting template template parameters
    //template <template <typename> class T>
    if(mapping->find_mapped_type(base_type)) {
        std::cout << "Currently not supporting template template parameters\n";
        return false;
    }

    for(int i = 0; i < template_types.size(); i++){
        if(auto x = mapping->find_mapped_type(template_types[i])) template_types[i] = x;
        else {
            if(!template_types[i]->replace_templated_types(mapping)) return false;
        }
    }
    return true;
}

// -- LOOK FOR TEMPLATES --
bool BaseType::look_for_templates() {
    // do nothing
    return true;
}

bool PointerType::look_for_templates() {
    return type->look_for_templates();
}

bool ReferenceType::look_for_templates(){
    return type->look_for_templates();
}

bool TemplatedType::look_for_templates() {
    if(!base_type->look_for_templates()) return false;
    for(int i = 0; i < template_types.size(); i++){
        if(!template_types[i]->look_for_templates()) return false;
    }

    //aha! found one! 
    if(!create_templated_type(this)) return false;
    
    return true;
}

// -- GENERATE MAPPING --
TemplateMapping* BaseType::generate_mapping(Type *_t, TemplateHeader *header) {
    if(dynamic_cast<BaseType*>(_t) == nullptr) return nullptr;
    BaseType *t = dynamic_cast<BaseType*>(_t);
    // - is t a templated type?
    for(int i = 0; i < header->types.size(); i++){
        if(header->types[i]->equals(t)) {
            TemplateMapping *mapping = new TemplateMapping();
            mapping->add_mapping(header->types[i], this->make_copy());
            return mapping;
        }
    }
    // - is t equal to this type?
    if(this->equals(t)) {
        return new TemplateMapping();
    }
    return nullptr;
}

TemplateMapping* PointerType::generate_mapping(Type *_t, TemplateHeader *header) {
    if(dynamic_cast<BaseType*>(_t)) {
        BaseType *t = dynamic_cast<BaseType*>(_t);
        // - is t a templated type?
        for(int i = 0; i < header->types.size(); i++){
            if(header->types[i]->equals(t)) {
                TemplateMapping *mapping = new TemplateMapping();
                mapping->add_mapping(header->types[i], this->make_copy());
                return mapping;
            }
        }
        return nullptr;
    }
    else {
        if(dynamic_cast<PointerType*>(_t) == nullptr) return nullptr;
        PointerType *t = dynamic_cast<PointerType*>(_t);
        return this->type->generate_mapping(t->type, header);
    }
}

TemplateMapping* ReferenceType::generate_mapping(Type *_t, TemplateHeader *header) {
    assert(false);
}

TemplateMapping* TemplatedType::generate_mapping(Type *_t, TemplateHeader *header) {
    if(dynamic_cast<BaseType*>(_t)) {
        BaseType *t = dynamic_cast<BaseType*>(_t);
        // - is t a templated type?
        for(int i = 0; i < header->types.size(); i++){
            if(header->types[i]->equals(t)) {
                TemplateMapping *mapping = new TemplateMapping();
                mapping->add_mapping(header->types[i], this->make_copy());
                return mapping;
            }
        }
        return nullptr;
    }
    else {
        if(dynamic_cast<TemplatedType*>(_t) == nullptr) return nullptr;
        TemplatedType *t = dynamic_cast<TemplatedType*>(_t);

        // - exclude matches like T<int, int>
        if(!this->base_type->equals(t->base_type)) return nullptr;

        // - number of template types must match
        if(this->template_types.size() != t->template_types.size()) return nullptr;

        // - find matches in all template types
        TemplateMapping *mapping = new TemplateMapping();
        for(int i = 0; i < this->template_types.size(); i++){
            TemplateMapping *nm = this->template_types[i]->generate_mapping(t->template_types[i], header);
            if(nm == nullptr) return nullptr;
            if(!mapping->merge_with_mapping(nm)) return nullptr;
        }
        return mapping;
    }
}