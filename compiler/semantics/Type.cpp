#include "Type.h"
#include "utils.h"
#include "TemplateMapping.h"
#include "TemplateHeader.h"
#include "StructLayout.h"
#include "Literal.h"
#include "Function.h"
#include "FunctionSignature.h"
#include "Parameter.h"

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

ArrayType:: ArrayType(Type *_type, int _amt) {
    assert(_type != nullptr);
    type = _type;
    amt = _amt;
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

FunctionPointerType::FunctionPointerType(Type *_return_type, std::vector<Type*> _param_types) {
    return_type = _return_type;
    param_types = _param_types;
    assert(return_type != nullptr);
    for(int i = 0; i < param_types.size(); i++) assert(param_types[i] != nullptr);
}

// -- CALC SIZE --
int BaseType::calc_size() {
    if(name == "i64" || name == "u64") return 8;
    else if(name == "i32" || name == "u32" || name == "f32") return 4;
    else if(name == "i16" || name == "u16") return 2;
    else if(name == "i8" || name == "u8") return 1;
    else {
        StructLayout *sl = get_struct_layout(this);
        if(sl == nullptr){ 
            std::cout << "Could not find struct layout of BaseType : " << to_string() << std::endl;
            assert(false);
        }
        return sl->get_size();
    }
}

int PointerType::calc_size() {
    return 8;
}

int ArrayType::calc_size() {
    return type->calc_size() * amt;
}

int ReferenceType::calc_size() {
    return 8;
}

int TemplatedType::calc_size() {
    StructLayout *sl = get_struct_layout(this);
    if(sl == nullptr){ 
        std::cout << "Could not find struct layout of TemplatedType : " << to_string() << std::endl;
        assert(false);
    }
    return sl->get_size();
}

int FunctionPointerType::calc_size() {
    return 8;
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

bool ArrayType::equals(const Type *other) const {
    if(auto x = dynamic_cast<const ArrayType*>(other)) return *type == *(x->type) && amt == x->amt;
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

bool FunctionPointerType::equals(const Type *other) const {
    if(auto x = dynamic_cast<const FunctionPointerType*>(other)) {
        if(!return_type->equals(x->return_type)) return false;
        for(int i = 0; i < param_types.size(); i++) if(!param_types[i]->equals(x->param_types[i])) return false;
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

size_t ArrayType::hash() const {
    size_t hash = type->hash();
    hash_combine(hash, (size_t) amt);
    return hash;
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

size_t FunctionPointerType::hash() const {
    size_t hash = 0;
    hash_combine(hash, return_type->hash());
    for(int i = 0; i < param_types.size(); i++) hash_combine(hash, param_types[i]->hash());
    return hash;
}

// -- TO STRING --
std::string BaseType::to_string() {
    return name;
}

std::string PointerType::to_string() {
    return type->to_string() + "*";
}

std::string ArrayType::to_string() {
    return type->to_string() + "[" + std::to_string(amt) + "]";
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

std::string FunctionPointerType::to_string() {
    std::string res = "";
    res += "#fn";
    res += "<";
    res += return_type->to_string();
    res += "(";
    for(int i = 0; i < param_types.size(); i++){
        res += param_types[i]->to_string();
        if(i + 1 != param_types.size()) res += ", ";
    }
    res += ")>";
    return res;
}

// -- MAKE COPY --
Type* BaseType::make_copy() {
    return new BaseType(name);
}

Type* PointerType::make_copy() {
    return new PointerType(type->make_copy());
}

Type* ArrayType::make_copy() {
    return new ArrayType(type->make_copy(), amt);
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

Type* FunctionPointerType::make_copy() {
    std::vector<Type*> _param_types;
    for(int i = 0; i < param_types.size(); i++) _param_types.push_back(param_types[i]->make_copy());
    return new FunctionPointerType(return_type->make_copy(), _param_types);
}

// -- CONVERT --
Type* Type::convert(parser::templated_type *t) {
    Type *res = BaseType::convert(t->t0);
    if(t->t1.has_value()) {  //template types
        std::vector<Type*> template_types;
        template_types.push_back(Type::convert(t->t1.value()->t2));
        for(int i = 0; i < t->t1.value()->t3.size(); i++){
            template_types.push_back(Type::convert(t->t1.value()->t3[i]->t3));
        }
        res = new TemplatedType(dynamic_cast<BaseType*>(res), template_types);
    }
    for(int i = 0; i < t->t2.size(); i++){
        if(t->t2[i]->is_b0) {   //pointer
            res = new PointerType(res);
        }
        else if(t->t2[i]->is_b1) {  //array
            IntegerLiteral *ilit = IntegerLiteral::convert(t->t2[i]->t1->t1);
            int amt = ilit->val;
            res = new ArrayType(res, amt);
        }
        else assert(false);
    }
    return res;
}

Type* Type::convert(parser::type *t) {
    Type *res = nullptr;
    if(t->t0->is_b0) {  //function pointer type
        res = FunctionPointerType::convert(t->t0->t0->t0);
    }
    else if(t->t0->is_b1) { //templated type
        res = Type::convert(t->t0->t1->t0);
    }
    else assert(false);
    assert(res != nullptr);
    if(t->t1.has_value()) {
        res = new ReferenceType(res);
    }
    return res;
}

BaseType* BaseType::convert(parser::base_type *t) {
    return new BaseType(t->to_string());
}

FunctionPointerType* FunctionPointerType::convert(parser::function_pointer_type *t) {
    Type *return_type = Type::convert(t->t3);
    std::vector<Type*> param_types = convert_type_list(t->t7);
    return new FunctionPointerType(return_type, param_types);
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

bool ArrayType::replace_templated_types(TemplateMapping *mapping) {
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

bool FunctionPointerType::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(return_type)) return_type = x;
    else if(!return_type->replace_templated_types(mapping)) return false;
    for(int i = 0; i < param_types.size(); i++){
        if(auto x = mapping->find_mapped_type(param_types[i])) param_types[i] = x;
        else if(!param_types[i]->replace_templated_types(mapping)) return false;
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

bool ArrayType::look_for_templates() {
    if(!type->look_for_templates()) return false;

    //hijacking look_for_templates() to also find array types
    if(!create_arraytype(this)) return false;

    return true;
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

bool FunctionPointerType::look_for_templates() {
    if(!return_type->look_for_templates()) return false;
    for(int i = 0; i < param_types.size(); i++){
        if(!param_types[i]->look_for_templates()) return false;
    }
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

TemplateMapping* ArrayType::generate_mapping(Type *_t, TemplateHeader *header) {
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
        if(dynamic_cast<ArrayType*>(_t) == nullptr) return nullptr;
        ArrayType *t = dynamic_cast<ArrayType*>(_t);
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

        // - exclude matches like T<int, int>, (you can't template the base type of a templated type)
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

TemplateMapping* FunctionPointerType::generate_mapping(Type *_t, TemplateHeader *header) {
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
        if(dynamic_cast<FunctionPointerType*>(_t) == nullptr) return nullptr;
        FunctionPointerType *t = dynamic_cast<FunctionPointerType*>(_t);

        // - number of param types must match
        if(this->param_types.size() != t->param_types.size()) {
            return nullptr;
        }

        TemplateMapping *mapping = new TemplateMapping();

        // - find templates from return type
        {
            TemplateMapping *nm = this->return_type->generate_mapping(t->return_type, header);
            if(nm == nullptr) return nullptr;
            if(!mapping->merge_with_mapping(nm)) return nullptr;
        }

        // - find templates from parameter types
        for(int i = 0; i < this->param_types.size(); i++){
            TemplateMapping *nm = this->param_types[i]->generate_mapping(t->param_types[i], header);
            if(nm == nullptr) return nullptr;
            if(!mapping->merge_with_mapping(nm)) return nullptr;
        }
        
        return mapping;
    }
}

// -- FIND ALL BASETYPES --
void BaseType::find_all_basetypes(std::vector<BaseType*> &out) {
    for(int i = 0; i < out.size(); i++){
        if(out[i]->equals(this)) return;
    }
    out.push_back(this);
}

void PointerType::find_all_basetypes(std::vector<BaseType*> &out) {
    type->find_all_basetypes(out);
}

void ArrayType::find_all_basetypes(std::vector<BaseType*> &out) {
    type->find_all_basetypes(out);
}

void ReferenceType::find_all_basetypes(std::vector<BaseType*> &out) {
    type->find_all_basetypes(out);
}

void TemplatedType::find_all_basetypes(std::vector<BaseType*> &out) {
    base_type->find_all_basetypes(out);
    for(int i = 0; i < template_types.size(); i++) template_types[i]->find_all_basetypes(out);
}

void FunctionPointerType::find_all_basetypes(std::vector<BaseType*> &out) {
    return_type->find_all_basetypes(out);
    for(int i = 0; i < param_types.size(); i++) param_types[i]->find_all_basetypes(out);
}

