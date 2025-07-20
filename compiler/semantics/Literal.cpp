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
#include "Expression.h"
#include "Identifier.h"
#include "FunctionSignature.h"
#include "Function.h"
#include "Identifier.h"

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

SyscallLiteral::SyscallLiteral(int _syscall_id, std::vector<Expression*> _arguments, Type *_type) {
    syscall_id = _syscall_id;
    arguments = _arguments;
    type = _type;
}

HexLiteral::HexLiteral(std::string _hex_str) {
    hex_str = _hex_str;
}

BinaryLiteral::BinaryLiteral(std::string _bin_str) {
    bin_str = _bin_str;
}

FunctionPointerLiteral::FunctionPointerLiteral(Identifier *_id, std::vector<Type*> _param_types) {
    id = _id;
    param_types = _param_types;
    assert(id != nullptr);
    for(int i = 0; i < param_types.size(); i++) assert(param_types[i] != nullptr);
}

// -- CONVERT --
Literal* Literal::convert(parser::literal *l) {
    if(l->is_a0) { //hex literal
        parser::literal_hex *lit = l->t0->t0;
        return HexLiteral::convert(lit);
    }
    else if(l->is_a1) { //binary literal
        parser::literal_binary *lit = l->t1->t0;
        return BinaryLiteral::convert(lit);
    }
    else if(l->is_a2) {  //float literal
        parser::literal_float *lit = l->t2->t0;
        return FloatLiteral::convert(lit);
    }
    else if(l->is_a3) {  //integer literal
        parser::literal_integer *lit = l->t3->t0;
        return IntegerLiteral::convert(lit);
    }
    else if(l->is_a4) { //sizeof literal
        parser::literal_sizeof *lit = l->t4->t0;
        return SizeofLiteral::convert(lit);
    }
    else if(l->is_a5) { //char literal
        parser::literal_char *lit = l->t5->t0;
        return CharLiteral::convert(lit);
    }
    else if(l->is_a6) { //string literal    
        parser::literal_string *lit = l->t6->t0;
        return StringLiteral::convert(lit);
    }   
    else if(l->is_a7) { //sizeof literal
        parser::literal_syscall *lit = l->t7->t0;
        return SyscallLiteral::convert(lit);
    }
    else if(l->is_a8) { //function pointer literal
        parser::literal_function_pointer *lit = l->t8->t0;
        return FunctionPointerLiteral::convert(lit);
    }
    else assert(false);    
}

FloatLiteral* FloatLiteral::convert(parser::literal_float *lit) {
    float val = std::stof(lit->to_string());
    return new FloatLiteral(val);
}

IntegerLiteral* IntegerLiteral::convert(parser::literal_integer *lit) {
    return new IntegerLiteral(stoi(lit->to_string()));
}

SizeofLiteral* SizeofLiteral::convert(parser::literal_sizeof *lit) {
    Type *t = Type::convert(lit->t4);
    return new SizeofLiteral(t);
}

CharLiteral* CharLiteral::convert(parser::literal_char *lit) {
    char val;
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

StringLiteral* StringLiteral::convert(parser::literal_string *lit) {
    std::vector<parser::literal_string::a0*> chars = lit->t1;
    std::string val = "";
    for(int i = 0; i < chars.size(); i++){
        parser::literal_string::a0* c = chars[i];
        val += c->to_string();
    }
    return new StringLiteral(val);
}

SyscallLiteral* SyscallLiteral::convert(parser::literal_syscall *lit) {
    IntegerLiteral *ilit = IntegerLiteral::convert(lit->t4);
    int syscall_id = ilit->val;
    std::vector<Expression*> arguments;
    {
        parser::argument_list *arglist = lit->t12;
        if(arglist->t0 != nullptr) {
            arguments.push_back(Expression::convert(arglist->t0->t0));
            for(int i = 0; i < arglist->t0->t1.size(); i++){
                arguments.push_back(Expression::convert(arglist->t0->t1[i]->t3));
            }
        }
    }
    Type *type = Type::convert(lit->t8);  
    return new SyscallLiteral(syscall_id, arguments, type);
}

HexLiteral* HexLiteral::convert(parser::literal_hex *lit) {
    std::string hex_str = lit->to_string();
    assert(hex_str.size() >= 3);
    return new HexLiteral(hex_str.substr(2));
}

BinaryLiteral* BinaryLiteral::convert(parser::literal_binary *lit) {
    std::string bin_str = lit->to_string();
    assert(bin_str.size() >= 3);
    return new BinaryLiteral(bin_str.substr(2));
}

FunctionPointerLiteral* FunctionPointerLiteral::convert(parser::literal_function_pointer *lit) {
    Identifier *id = Identifier::convert(lit->t3);
    std::vector<Type*> param_types = convert_type_list(lit->t7);
    return new FunctionPointerLiteral(id, param_types);
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

Type* SyscallLiteral::resolve_type() {
    // - all syscall ids are >= 0. 
    if(syscall_id < 0) {
        std::cout << "Syscall id has to be >= 0\n";
        return nullptr;
    }

    // - number of arguments must be <= 6
    if(arguments.size() > 6) {
        std::cout << "Syscall number of arguments must be <= 6\n";
        return nullptr;
    }

    // - do all of the arguments resolve to a type?
    for(int i = 0; i < arguments.size(); i++){
        if(arguments[i]->resolve_type() == nullptr){
            std::cout << "Syscall argument does not resolve to type\n";
            return nullptr;
        }
    }

    //assume that the return type is correct
    return type->make_copy();
}

Type* HexLiteral::resolve_type() {
    // - can hex string fit in u64?
    if(hex_str.size() > 16) {
        std::cout << "Hex value too large : 0x" << hex_str << "\n";
        return nullptr;
    }

    return primitives::u64->make_copy();
}

Type* BinaryLiteral::resolve_type() {
    // - can binary string fit in u64?
    if(bin_str.size() > 64) {
        std::cout << "Binary value too large : 0b" << bin_str << "\n";
        return nullptr;
    }

    return primitives::u64->make_copy();
}

Type* FunctionPointerLiteral::resolve_type() {
    FunctionSignature *fs = new FunctionSignature(id, param_types);

    // - is there a corresponding function to this?
    // TODO decide if this should also map to templated functions. 
    // - if we decide to go down the route of every function being a templated function, 
    //   then maybe only allow mappings to functions with no templates.
    // TODO make the behaviour with templated functions consistent. 
    Function *f = get_function(fs);
    if(f == nullptr) {
        std::cout << "Function pointer doesn't map to existing function : " << to_string() << "\n";
        return nullptr;
    }

    std::vector<Type*> _param_types;
    for(int i = 0; i < param_types.size(); i++) _param_types.push_back(param_types[i]->make_copy());
    return new FunctionPointerType(f->type->make_copy(), _param_types);
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
    //add this string literal to controller
    add_string_literal(val);

    //get label
    std::string label = get_string_literal_label(val);

    //load address into %rax using %rip relative addressing
    fout << indent() << "lea " << label << "(%rip), %rax\n";
}

void SyscallLiteral::emit_asm() {
    if(asm_debug) fout << indent() << "# syscall : " << syscall_id << "\n";

    //gather all arguments into tmp variables
    push_declaration_stack();
    std::vector<Variable*> argv;
    for(int i = 0; i < arguments.size(); i++){
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Type *vt = arguments[i]->resolve_type();
        assert(vt != nullptr);
        Variable *v = emit_initialize_stack_variable(vt, id, arguments[i]);
        argv.push_back(v);
    }
    assert(argv.size() == arguments.size());

    //insert syscall id into %rax
    assert(syscall_id >= 0);
    fout << indent() << "mov $" << syscall_id << ", %rax\n";

    //insert arguments into argument registers
    assert(arguments.size() <= 6);
    if(arguments.size() >= 1) fout << indent() << "movq " << argv[0]->addr << ", %rdi\n";
    if(arguments.size() >= 2) fout << indent() << "movq " << argv[1]->addr << ", %rsi\n";
    if(arguments.size() >= 3) fout << indent() << "movq " << argv[2]->addr << ", %rdx\n";
    if(arguments.size() >= 4) fout << indent() << "movq " << argv[3]->addr << ", %r10\n";
    if(arguments.size() >= 5) fout << indent() << "movq " << argv[4]->addr << ", %r8\n";
    if(arguments.size() >= 6) fout << indent() << "movq " << argv[5]->addr << ", %r9\n";

    //do syscall
    fout << indent() << "syscall\n";

    //cleanup tmp variables
    pop_declaration_stack();

    if(asm_debug) fout << indent() << "# done syscall : " << syscall_id << "\n";
}

void HexLiteral::emit_asm() {
    assert(hex_str.size() >= 1 && hex_str.size() <= 16);
    fout << indent() << "mov $0x" << hex_str << ", %rax\n";
}

void BinaryLiteral::emit_asm() {
    assert(bin_str.size() >= 1 && bin_str.size() <= 64);
    uint64_t val = 0;
    for(int i = 0; i < bin_str.size(); i++){
        val = (val * 2) + (bin_str[i] - '0');
    }
    fout << indent() << "mov $" << val << ", %rax\n";
}

void FunctionPointerLiteral::emit_asm() {
    FunctionSignature *fs = new FunctionSignature(id, param_types);
    Function *f = get_function(fs);
    assert(f != nullptr);
    
    //load label address into register using %rip relative addressing. 
    std::string label = get_function_label(fs);
    fout << indent() << "lea " << label << "(%rip), %rax\n";
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

size_t SyscallLiteral::hash() {
    size_t hash = 0;
    hash_combine(hash, syscall_id);
    for(int i = 0; i < arguments.size(); i++){
        hash_combine(hash, arguments[i]->hash());
    }
    hash_combine(hash, type->hash());
    return hash;
}

size_t HexLiteral::hash() {
    return std::hash<std::string>()(hex_str);
}

size_t BinaryLiteral::hash() {
    return std::hash<std::string>()(bin_str);
}

size_t FunctionPointerLiteral::hash() {
    size_t hash = 0;
    hash_combine(hash, id->hash());
    for(int i = 0; i < param_types.size(); i++) hash_combine(hash, param_types[i]->hash());
    return hash;
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

bool SyscallLiteral::equals(Literal *_other) {
    if(dynamic_cast<SyscallLiteral*>(_other) == nullptr) return false;
    SyscallLiteral *other = dynamic_cast<SyscallLiteral*>(_other);

    if(syscall_id != other->syscall_id) return false;
    if(arguments.size() != other->arguments.size()) return false;
    for(int i = 0; i < arguments.size(); i++){
        if(!arguments[i]->equals(other->arguments[i])) return false;
    }
    if(!type->equals(other->type)) return false;
    return true;
}

bool HexLiteral::equals(Literal *_other) {
    if(dynamic_cast<HexLiteral*>(_other) == nullptr) return false;
    HexLiteral *other = dynamic_cast<HexLiteral*>(_other);

    return hex_str == other->hex_str;
}

bool BinaryLiteral::equals(Literal *_other) {
    if(dynamic_cast<BinaryLiteral*>(_other) == nullptr) return false;
    BinaryLiteral *other = dynamic_cast<BinaryLiteral*>(_other);

    return bin_str == other->bin_str;
}

bool FunctionPointerLiteral::equals(Literal *_other) {
    if(dynamic_cast<FunctionPointerLiteral*>(_other) == nullptr) return false;
    FunctionPointerLiteral *other = dynamic_cast<FunctionPointerLiteral*>(_other);

    if(!id->equals(other->id)) return false;
    if(param_types.size() != other->param_types.size()) return false;
    for(int i = 0; i < param_types.size(); i++){
        if(!param_types[i]->equals(other->param_types[i])) return false;
    }
    return true;
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

Literal* SyscallLiteral::make_copy() {
    std::vector<Expression*> _arguments;
    for(int i = 0; i < arguments.size(); i++) _arguments.push_back(arguments[i]->make_copy());
    return new SyscallLiteral(syscall_id, _arguments, type->make_copy());
}

Literal* HexLiteral::make_copy() {
    return new HexLiteral(hex_str);
}

Literal* BinaryLiteral::make_copy() {
    return new BinaryLiteral(bin_str);
}

Literal* FunctionPointerLiteral::make_copy() {
    std::vector<Type*> _param_types;
    for(int i = 0; i < param_types.size(); i++) _param_types.push_back(param_types[i]->make_copy());
    return new FunctionPointerLiteral(id->make_copy(), _param_types);
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

std::string SyscallLiteral::to_string() {
    std::string ret = "syscall(" + std::to_string(syscall_id) + ", ";
    for(int i = 0; i < arguments.size(); i++){
        ret += arguments[i]->to_string();
        if(i != arguments.size() - 1) ret += ", ";
    }
    ret += type->to_string();
    ret += ")";
    return ret;
}

std::string HexLiteral::to_string() {
    return "0x" + hex_str;
}

std::string BinaryLiteral::to_string() {
    return "0b" + bin_str;
}

std::string FunctionPointerLiteral::to_string() {
    std::string ret = "#<" + id->name + "(";
    for(int i = 0; i < param_types.size(); i++) {
        ret += param_types[i]->to_string();
        if(i + 1 != param_types.size()) ret += ", ";
    }
    ret += ")>";
    return ret;
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

bool SyscallLiteral::replace_templated_types(TemplateMapping *mapping) {
    for(int i = 0; i < arguments.size(); i++) {
        if(!arguments[i]->replace_templated_types(mapping)) return false;
    }
    if(!type->replace_templated_types(mapping)) return false;
    return true;
}

bool HexLiteral::replace_templated_types(TemplateMapping *mapping) {
    return true;
}

bool BinaryLiteral::replace_templated_types(TemplateMapping *mapping) {
    return true;
}

bool FunctionPointerLiteral::replace_templated_types(TemplateMapping *mapping) {
    for(int i = 0; i < param_types.size(); i++){
        if(!param_types[i]->replace_templated_types(mapping)) return false;
    }
    return true;
}