#include "Literal.h"
#include "Type.h"
#include "utils.h"
#include <bit>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cstring>
#include "TemplateMapping.h"
#include "primitives.h"

// -- CONSTRUCTOR --
FloatLiteral::FloatLiteral(float _val) {
    val = _val;
}

IntegerLiteral::IntegerLiteral(int _val) {
    val = _val;
}

SizeofLiteral::SizeofLiteral(Type *_type) {
    type = _type;
}

CharLiteral::CharLiteral(char _val) {
    val = _val;
}

StringLiteral::StringLiteral(std::string _val) {
    val = _val;
}

// -- CONVERT --
Literal* Literal::convert(parser::literal *l) {
    if(l->is_a0) {
        parser::literal_float *lit = l->t0->t0;
        float val = std::stof(lit->to_string());
        return new FloatLiteral(val);
    }
    else if(l->is_a1) {  //integer literal
        parser::literal_integer *lit = l->t1->t0;
        return new IntegerLiteral(stoi(lit->to_string()));
    }
    else if(l->is_a2) { //sizeof literal
        parser::literal_sizeof *lit = l->t2->t0;
        Type *t = Type::convert(lit->t4);
        return new SizeofLiteral(t);
    }
    else if(l->is_a3) { //char literal
        char val;
        parser::literal_char *lit = l->t3->t0;
        parser::literal_char::a0 *c = lit->t1;
        if(c->is_b2) {    //escape
            parser::escape *e = c->t2->t0;  
            val = escape_to_char(e);
        }
        else {  //not escape
            val = c->to_string()[0];
        }
        return new CharLiteral(val);
    }
    else if(l->is_a4) { //string literal    
        parser::literal_string *lit = l->t4->t0;
        std::vector<parser::literal_string::a0*> chars = lit->t1;
        std::string val(chars.size(), ' ');
        for(int i = 0; i < chars.size(); i++){
            parser::literal_string::a0* c = chars[i];
            char cchar;
            if(c->is_b2) {  //escape
                parser::escape *e = c->t2->t0;
                cchar = escape_to_char(e);
            }
            else {
                cchar = c->to_string()[0];
            }
            val[i] = cchar;
        }
        return new StringLiteral(val);
    }   
    else assert(false);    
}

// -- RESOLVE TYPE --
Type* FloatLiteral::resolve_type() {
    return primitives::f32->make_copy();
}

Type* IntegerLiteral::resolve_type() {
    return primitives::i32->make_copy();
}

Type* SizeofLiteral::resolve_type() {
    return primitives::u64->make_copy();
}

Type* CharLiteral::resolve_type() {
    return primitives::u8->make_copy();
}

Type* StringLiteral::resolve_type() {
    return new PointerType(primitives::u8->make_copy());
}

// -- EMIT ASM --
std::string float_to_hex_string(float f) {
    uint32_t bits;
    std::memcpy(&bits, &f, sizeof(bits));
    
    std::stringstream ss;
    ss << "$0x" << std::hex << std::setw(8) << std::setfill('0') << bits;
    return ss.str();
}

void FloatLiteral::emit_asm() {
    fout << indent() << "mov " << float_to_hex_string(val) << ", %eax\n";
}

void IntegerLiteral::emit_asm() {
    fout << indent() << "mov $" << val << ", %rax\n";
}

void SizeofLiteral::emit_asm() {
    int sz = type->calc_size();
    fout << indent() << "mov $" << sz << ", %rax\n";
}

void CharLiteral::emit_asm() {
    fout << indent() << "movb $" << (int) val << ", %al\n";
}

void StringLiteral::emit_asm() {
    //allocate memory
    emit_malloc(val.size() + 1);

    //save start of string
    emit_push("%rax", "StringLiteral::emit_asm() : string start");

    //string index pointer to %rbx
    fout << indent() << "mov %rax, %rbx\n";

    //populate memory
    for(int i = 0; i < val.size(); i++){
        fout << indent() << "movb $" << (int) val[i] << ", %al\n";
        emit_mem_store(1);
        fout << indent() << "inc %rbx\n";
    }
    fout << indent() << "movb $0, %al\n";
    emit_mem_store(1);

    //retrieve start of string
    emit_pop("%rax", "StringLiteral::emit_asm() : string start");
}

// -- HASH -- 
size_t FloatLiteral::hash() {
    return std::hash<float>{}(val);
}

size_t IntegerLiteral::hash() {
    return (size_t) val;
}

size_t SizeofLiteral::hash() {
    return type->hash();
}

size_t CharLiteral::hash() {
    return (size_t) val;
}

size_t StringLiteral::hash() {
    return std::hash<std::string>()(val);
}

// -- EQUALS --
bool FloatLiteral::equals(Literal *_other) {
    if(dynamic_cast<FloatLiteral*>(_other) == nullptr) return false;
    FloatLiteral *other = dynamic_cast<FloatLiteral*>(_other);
    return val == other->val;
}

bool IntegerLiteral::equals(Literal *_other) {
    if(dynamic_cast<IntegerLiteral*>(_other) == nullptr) return false;
    IntegerLiteral *other = dynamic_cast<IntegerLiteral*>(_other);

    return val == other->val;
}

bool SizeofLiteral::equals(Literal *_other) {
    if(dynamic_cast<SizeofLiteral*>(_other) == nullptr) return false;
    SizeofLiteral *other = dynamic_cast<SizeofLiteral*>(_other);

    return type->equals(other->type);
}

bool CharLiteral::equals(Literal *_other) {
    if(dynamic_cast<CharLiteral*>(_other) == nullptr) return false;
    CharLiteral *other = dynamic_cast<CharLiteral*>(_other);

    return val == other->val;
}

bool StringLiteral::equals(Literal *_other) {
    if(dynamic_cast<StringLiteral*>(_other) == nullptr) return false;
    StringLiteral *other = dynamic_cast<StringLiteral*>(_other);

    return val == other->val;
}

// -- MAKE COPY --
Literal* FloatLiteral::make_copy() {
    return new FloatLiteral(val);
}

Literal* IntegerLiteral::make_copy() {
    return new IntegerLiteral(val);
}

Literal* SizeofLiteral::make_copy() {
    return new SizeofLiteral(type->make_copy());
}

Literal* CharLiteral::make_copy() {
    return new CharLiteral(val);
}

Literal* StringLiteral::make_copy() {
    return new StringLiteral(val);
}

// -- TO STRING --
std::string FloatLiteral::to_string() {
    return std::to_string(val);
}

std::string IntegerLiteral::to_string() {
    return std::to_string(val);
}

std::string SizeofLiteral::to_string() {
    return "sizeof(" + type->to_string() + ")";
}

std::string CharLiteral::to_string() {
    return "'" + std::string(1, val) + "'";
}

std::string StringLiteral::to_string() {
    return "\"" + val + "\"";
}

// -- REPLACE TEMPLATED TYPES --
bool FloatLiteral::replace_templated_types(TemplateMapping *mapping) {
    return true;
}

bool IntegerLiteral::replace_templated_types(TemplateMapping *mapping) {
    return true;
}

bool SizeofLiteral::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) {type = x; return true;}
    else return type->replace_templated_types(mapping);
}

bool CharLiteral::replace_templated_types(TemplateMapping *mapping) {
    return true;
}

bool StringLiteral::replace_templated_types(TemplateMapping *mapping) {
    return true;
}