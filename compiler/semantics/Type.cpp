#include "Type.h"
#include "utils.h"
#include "TemplateMapping.h"
#include "TemplateHeader.h"
#include "StructLayout.h"
#include "Literal.h"
#include "Function.h"
#include "FunctionSignature.h"
#include "Parameter.h"
#include "CompilationContext.h"

// -- CONVERT CONSTRUCTOR --
Type::Type(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

BaseType::BaseType(parser::token *tok) : Type(tok) {
    // do nothing
}

PointerType::PointerType(parser::token *tok) : Type(tok) {
    // do nothing
}

ArrayType::ArrayType(parser::token *tok) : Type(tok) {
    // do nothing
}

ReferenceType::ReferenceType(parser::token *tok) : Type(tok) {
    // do nothing
}

TemplatedType::TemplatedType(parser::token *tok) : Type(tok) {
    // do nothing
}

FunctionPointerType::FunctionPointerType(parser::token *tok) : Type(tok) {
    // do nothing
}

// -- COPY CONSTRUCTOR --
Type::Type(const Type& other) : ASTNode(other) {
    // do nothing
}

BaseType::BaseType(const BaseType& other) : Type(other) {
    name = other.name;
}

PointerType::PointerType(const PointerType& other) : Type(other) {
    type = other.type->make_copy();
}

ArrayType::ArrayType(const ArrayType& other) : Type(other) {
    type = other.type->make_copy();
    amt = other.amt;
}

ReferenceType::ReferenceType(const ReferenceType& other) : Type(other) {
    type = other.type->make_copy();
}

TemplatedType::TemplatedType(const TemplatedType& other) : Type(other) {
    base_type = dynamic_cast<BaseType*>(other.base_type->make_copy());
    for(int i = 0; i < other.template_types.size(); i++) {
        template_types.push_back(other.template_types[i]->make_copy());
    }
}

FunctionPointerType::FunctionPointerType(const FunctionPointerType& other) : Type(other) {
    return_type = other.return_type->make_copy();
    for(int i = 0; i < other.param_types.size(); i++) {
        param_types.push_back(other.param_types[i]->make_copy());
    }
}

// -- SYNTHESIS CONSTRUCTOR --
Type::Type() : ASTNode() {
    // do nothing
}

BaseType::BaseType(std::string _name) : Type() {
    name = _name;
}

PointerType::PointerType(Type *_type) : Type() {
    type = _type;
    assert(type != nullptr);
}

ArrayType:: ArrayType(Type *_type, int _amt) : Type() {
    type = _type;
    amt = _amt;
    assert(type != nullptr);
}

ReferenceType::ReferenceType(Type *_type) : Type() {
    type = _type;
    assert(type != nullptr);
}

TemplatedType::TemplatedType(BaseType *_base_type, std::vector<Type*> _template_types) : Type() {
    base_type = _base_type;
    template_types = _template_types;
    assert(base_type != nullptr);
    assert(template_types.size() != 0);
    for(int i = 0; i < template_types.size(); i++) assert(template_types[i] != nullptr);
}

FunctionPointerType::FunctionPointerType(Type *_return_type, std::vector<Type*> _param_types) : Type() {
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
    return new BaseType(*this);
}

Type* PointerType::make_copy() {
    return new PointerType(*this);
}

Type* ArrayType::make_copy() {
    return new ArrayType(*this);
}

Type* ReferenceType::make_copy() {
    return new ReferenceType(*this);
}

Type* TemplatedType::make_copy() {
    return new TemplatedType(*this);
}

Type* FunctionPointerType::make_copy() {
    return new FunctionPointerType(*this);
}

// -- CONVERT --
Type* Type::convert(parser::type *t) {
    Type* result = nullptr;
    if(t->t0->is_b0) {      //function pointer type
        result = FunctionPointerType::convert(t->t0->t0->t0);
    }
    else if(t->t0->is_b1) { //templated type
        result = Type::convert(t->t0->t1->t0);
    }
    else assert(false);
    assert(result != nullptr);
    if(t->t1.has_value()) {
        ReferenceType* _result = new ReferenceType(t);
        _result->type = result;
        result = _result;
    }
    return result;
}

Type* Type::convert(parser::templated_type *t) {
    Type* result = BaseType::convert(t->t0);
    if(t->t1.has_value()) {  //template types
        TemplatedType* _result = new TemplatedType(t);
        _result->base_type = dynamic_cast<BaseType*>(result);
        _result->template_types.push_back(Type::convert(t->t1.value()->t2));
        for(int i = 0; i < t->t1.value()->t3.size(); i++){
            _result->template_types.push_back(Type::convert(t->t1.value()->t3[i]->t3));
        }
        result = _result;
    }
    for(int i = 0; i < t->t2.size(); i++){
        //is this a pointer type?
        if(t->t2[i]->is_b0) { 
            PointerType* _result = new PointerType(t);
            _result->type = result;
            result = _result;
            continue;
        }

        //this is an array type
        //we want our arrays to be nested from right to left, the parser parses them from left to right
        //find the maximal chunk of arrays, reverse and nest them.
        assert(t->t2[i]->is_b1);    //array
        int r = i;
        while(r != t->t2.size() && t->t2[r]->is_b1) r ++;
        for(int j = r - 1; j >= i; j--) {
            ArrayType* _result = new ArrayType(t);
            _result->type = result;
            IntegerLiteral *ilit = IntegerLiteral::convert(t->t2[j]->t1->t1);
            _result->amt = ilit->val;
            result = _result;
        }   
        i = r - 1;
    }
    return result;
}

BaseType* BaseType::convert(parser::base_type *t) {
    BaseType* result = new BaseType(t);
    result->name = t->to_string();
    return result;
}

FunctionPointerType* FunctionPointerType::convert(parser::function_pointer_type *t) {
    FunctionPointerType* result = new FunctionPointerType(t);
    result->return_type = Type::convert(t->t3);
    result->param_types = convert_type_list(t->t7);
    return result;
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
    //essentially, the base type of a templated type cannot be templated
    //in C++, template <template <typename> class T>
    //template<T>
    //struct A {
    //    T<i32> foo;   // this places a restriction on T to be a templated type 
    //    A<i32> bar;   // I auto-replace any instances of the struct basetype within its definition, so this is invalid as well
    //}
    if(mapping->find_mapped_type(base_type)) {
        std::cout << "Currently not supporting template template parameters : " << this->to_string() << "\n";
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
bool BaseType::look_for_templates(CompilationContext *ctx) {
    // do nothing
    return true;
}

bool PointerType::look_for_templates(CompilationContext *ctx) {
    return type->look_for_templates(ctx);
}

bool ArrayType::look_for_templates(CompilationContext *ctx) {
    if(!type->look_for_templates(ctx)) return false;

    //hijacking look_for_templates() to also find array types
    if(!create_array_type(this, ctx)) return false;

    return true;
}

bool ReferenceType::look_for_templates(CompilationContext *ctx){
    return type->look_for_templates(ctx);
}

bool TemplatedType::look_for_templates(CompilationContext *ctx) {
    if(!base_type->look_for_templates(ctx)) return false;
    for(int i = 0; i < template_types.size(); i++){
        if(!template_types[i]->look_for_templates(ctx)) return false;
    }

    //aha! found one! 
    if(!create_templated_type(this, ctx)) return false;
    
    return true;
}

bool FunctionPointerType::look_for_templates(CompilationContext *ctx) {
    if(!return_type->look_for_templates(ctx)) return false;
    for(int i = 0; i < param_types.size(); i++){
        if(!param_types[i]->look_for_templates(ctx)) return false;
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

        // - array sizes must match
        if(this->amt != t->amt) {
            return nullptr;
        }

        return this->type->generate_mapping(t->type, header);
    }
}

TemplateMapping* ReferenceType::generate_mapping(Type *_t, TemplateHeader *header) {
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
        if(dynamic_cast<ReferenceType*>(_t) == nullptr) return nullptr;
        ReferenceType *t = dynamic_cast<ReferenceType*>(_t);
        return this->type->generate_mapping(t->type, header);
    }
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

// -- MISC --
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
