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
#include "CompilationContext.h"

// -- CONVERT CONSTRUCTOR --
Literal::Literal(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

FloatLiteral::FloatLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

IntegerLiteral::IntegerLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

SizeofLiteral::SizeofLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

CharLiteral::CharLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

StringLiteral::StringLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

SyscallLiteral::SyscallLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

IntrinsicLiteral::IntrinsicLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

HexLiteral::HexLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

BinaryLiteral::BinaryLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

OctalLiteral::OctalLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

FunctionPointerLiteral::FunctionPointerLiteral(parser::token *tok) : Literal(tok) {
    // do nothing
}

// -- COPY CONSTRUCTOR --
Literal::Literal(const Literal& other) : ASTNode(other) {
    // do nothing
}

FloatLiteral::FloatLiteral(const FloatLiteral& other) : Literal(other) {
    val = other.val;
}

IntegerLiteral::IntegerLiteral(const IntegerLiteral& other) : Literal(other) {
    val = other.val;
}

SizeofLiteral::SizeofLiteral(const SizeofLiteral& other) : Literal(other) {
    type = other.type->make_copy();
}

CharLiteral::CharLiteral(const CharLiteral& other) : Literal(other) {
    val = other.val;
}

StringLiteral::StringLiteral(const StringLiteral& other) : Literal(other) {
    val = other.val;
}

SyscallLiteral::SyscallLiteral(const SyscallLiteral& other) : Literal(other) {
    syscall_id = other.syscall_id;
    for(int i = 0; i < other.arguments.size(); i++) arguments.push_back(other.arguments[i]->make_copy());
    type = other.type->make_copy();
}

IntrinsicLiteral::IntrinsicLiteral(const IntrinsicLiteral& other) : Literal(other) {
    intrinsic_name = other.intrinsic_name;
    for(int i = 0; i < other.arguments.size(); i++) arguments.push_back(other.arguments[i]->make_copy());
}

HexLiteral::HexLiteral(const HexLiteral& other) : Literal(other) {
    hex_str = other.hex_str;
}

BinaryLiteral::BinaryLiteral(const BinaryLiteral& other) : Literal(other) {
    bin_str = other.bin_str;
}

OctalLiteral::OctalLiteral(const OctalLiteral& other) : Literal(other) {
    oct_str = other.oct_str;
}

FunctionPointerLiteral::FunctionPointerLiteral(const FunctionPointerLiteral& other) : Literal(other) {
    id = other.id->make_copy();
    for(int i = 0; i < other.param_types.size(); i++) param_types.push_back(other.param_types[i]->make_copy());
}

// -- SYNTHESIS CONSTRUCTOR --
Literal::Literal() : ASTNode() {
    // do nothing
}

FloatLiteral::FloatLiteral(float _val) : Literal() {
    val = _val;
}

IntegerLiteral::IntegerLiteral(int _val) : Literal() {
    val = _val;
}

SizeofLiteral::SizeofLiteral(Type *_type) : Literal() {
    type = _type;
}

CharLiteral::CharLiteral(char _val) : Literal() {
    val = _val;
}

StringLiteral::StringLiteral(std::string _val) : Literal() {
    val = _val;
}

SyscallLiteral::SyscallLiteral(int _syscall_id, std::vector<Expression*> _arguments, Type *_type) : Literal() {
    syscall_id = _syscall_id;
    arguments = _arguments;
    type = _type;
}

IntrinsicLiteral::IntrinsicLiteral(std::string _intrinsic_name, std::vector<Expression*> _arguments) {
    intrinsic_name = _intrinsic_name;
    arguments = _arguments;
}

HexLiteral::HexLiteral(std::string _hex_str) : Literal() {
    hex_str = _hex_str;
}

BinaryLiteral::BinaryLiteral(std::string _bin_str) : Literal() {
    bin_str = _bin_str;
}

OctalLiteral::OctalLiteral(std::string _oct_str) : Literal() {
    oct_str = _oct_str;
}

FunctionPointerLiteral::FunctionPointerLiteral(Identifier *_id, std::vector<Type*> _param_types) : Literal() {
    id = _id;
    param_types = _param_types;
    assert(id != nullptr);
    for(int i = 0; i < param_types.size(); i++) assert(param_types[i] != nullptr);
}

// -- CONVERT --
Literal* Literal::convert(parser::literal *l) {
    if(l->is_a0) {      //hex literal
        parser::literal_hex *lit = l->t0->t0;
        return HexLiteral::convert(lit);
    }
    else if(l->is_a1) { //binary literal
        parser::literal_binary *lit = l->t1->t0;
        return BinaryLiteral::convert(lit);
    }
    else if(l->is_a2) { //octal literal
        parser::literal_octal *lit = l->t2->t0;
        return OctalLiteral::convert(lit);
    }
    else if(l->is_a3) { //float literal
        parser::literal_float *lit = l->t3->t0;
        return FloatLiteral::convert(lit);
    }
    else if(l->is_a4) { //integer literal
        parser::literal_integer *lit = l->t4->t0;
        return IntegerLiteral::convert(lit);
    }
    else if(l->is_a5) { //sizeof literal
        parser::literal_sizeof *lit = l->t5->t0;
        return SizeofLiteral::convert(lit);
    }
    else if(l->is_a6) { //char literal
        parser::literal_char *lit = l->t6->t0;
        return CharLiteral::convert(lit);
    }
    else if(l->is_a7) { //string literal    
        parser::literal_string *lit = l->t7->t0;
        return StringLiteral::convert(lit);
    }   
    else if(l->is_a8) { //sizeof literal
        parser::literal_syscall *lit = l->t8->t0;
        return SyscallLiteral::convert(lit);
    }
    else if(l->is_a9) { //intrinsic literal
        parser::literal_intrinsic *lit = l->t9->t0;
        return IntrinsicLiteral::convert(lit);
    }
    else if(l->is_a10) { //function pointer literal
        parser::literal_function_pointer *lit = l->t10->t0;
        return FunctionPointerLiteral::convert(lit);
    }
    else assert(false);    
}

FloatLiteral* FloatLiteral::convert(parser::literal_float *lit) {
    FloatLiteral* result = new FloatLiteral(lit);
    result->val = std::stof(lit->to_string());
    return result;
}

IntegerLiteral* IntegerLiteral::convert(parser::literal_integer *lit) {
    IntegerLiteral* result = new IntegerLiteral(lit);
    result->val = std::stoi(lit->to_string());
    return result;
}

SizeofLiteral* SizeofLiteral::convert(parser::literal_sizeof *lit) {
    SizeofLiteral* result = new SizeofLiteral(lit);
    result->type = Type::convert(lit->t4);
    return result;
}

CharLiteral* CharLiteral::convert(parser::literal_char *lit) {
    CharLiteral* result = new CharLiteral(lit);
    parser::literal_char::a0 *c = lit->t1;
    if(c->is_b2) {      //escape
        parser::escape *e = c->t2->t0;  
        result->val = escape_to_char(e);
    }
    else {              //not escape
        result->val = c->to_string()[0];
    }
    return result;
}

StringLiteral* StringLiteral::convert(parser::literal_string *lit) {
    StringLiteral* result = new StringLiteral(lit);
    std::vector<parser::literal_string::a0*> chars = lit->t1;
    for(int i = 0; i < chars.size(); i++){
        parser::literal_string::a0* c = chars[i];
        result->val += c->to_string();
    }
    return result;
}

SyscallLiteral* SyscallLiteral::convert(parser::literal_syscall *lit) {
    SyscallLiteral* result = new SyscallLiteral(lit);
    IntegerLiteral *ilit = IntegerLiteral::convert(lit->t4);
    result->syscall_id = ilit->val;
    if(lit->t9.has_value()) result->arguments = convert_argument_list(lit->t9.value()->t3);
    result->type = Type::convert(lit->t8);  
    return result;
}

IntrinsicLiteral* IntrinsicLiteral::convert(parser::literal_intrinsic *lit) {
    IntrinsicLiteral* result = new IntrinsicLiteral(lit);
    StringLiteral *slit = StringLiteral::convert(lit->t4);
    result->intrinsic_name = slit->val;
    if(lit->t5.has_value()) result->arguments = convert_argument_list(lit->t5.value()->t3);
    return result;
}

HexLiteral* HexLiteral::convert(parser::literal_hex *lit) {
    HexLiteral* result = new HexLiteral(lit);
    std::string str = lit->to_string();
    assert(str.size() >= 3);
    result->hex_str = str.substr(2);
    return result;
}

BinaryLiteral* BinaryLiteral::convert(parser::literal_binary *lit) {
    BinaryLiteral* result = new BinaryLiteral(lit);
    std::string str = lit->to_string();
    assert(str.size() >= 3);
    result->bin_str = str.substr(2);
    return result;
}

OctalLiteral* OctalLiteral::convert(parser::literal_octal *lit) {
    OctalLiteral* result = new OctalLiteral(lit);
    std::string str = lit->to_string();
    assert(str.size() >= 3);
    result->oct_str = str.substr(2);
    return result;
}

FunctionPointerLiteral* FunctionPointerLiteral::convert(parser::literal_function_pointer *lit) {
    FunctionPointerLiteral* result = new FunctionPointerLiteral(lit);
    result->id = Identifier::convert(lit->t3);
    result->param_types = convert_type_list(lit->t7);
    return result;
}

// -- RESOLVE TYPE --
Type* FloatLiteral::resolve_type(CompilationContext *ctx) {
    return primitives::f32->make_copy();
}

Type* IntegerLiteral::resolve_type(CompilationContext *ctx) {
    return primitives::i32->make_copy();
}

Type* SizeofLiteral::resolve_type(CompilationContext *ctx) {
    return primitives::u64->make_copy();
}

Type* CharLiteral::resolve_type(CompilationContext *ctx) {
    return primitives::u8->make_copy();
}

Type* StringLiteral::resolve_type(CompilationContext *ctx) {
    return new PointerType(primitives::u8->make_copy());
}

Type* SyscallLiteral::resolve_type(CompilationContext *ctx) {
    // - see if syscalls are allowed
    if(no_syscall) {
        std::cout << "Syscalls have been disabled\n";
        return nullptr;
    }
    
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
        if(arguments[i]->resolve_type(ctx) == nullptr){
            std::cout << "Syscall argument does not resolve to type\n";
            return nullptr;
        }
    }

    //assume that the return type is correct
    return type->make_copy();
}

Type* IntrinsicLiteral::resolve_type(CompilationContext *ctx) {
    // - see if we can find corresponding intrinsic
    Intrinsic *intrinsic = get_intrinsic(intrinsic_name);
    if(intrinsic == nullptr) {
        std::cout << "Failed to find intrinsic for intrinsic literal : " << intrinsic_name << "\n";
        return nullptr;
    }

    // - see if argument types match intrinsic parameter types
    if(arguments.size() != intrinsic->parameter_types.size()) {
        std::cout << "Intrinsic literal argument amount does not match intrinsic\n";
        return nullptr;
    }
    for(int i = 0; i < arguments.size(); i++) {
        Type *atype = arguments[i]->resolve_type(ctx);
        if(!atype->equals(intrinsic->parameter_types[i])) {
            std::cout << "Intrinsic literal incorrect argument type for intrinsic : " << intrinsic_name << " : " << atype->to_string() << " vs. " << intrinsic->parameter_types[i]->to_string() << "\n";
            return nullptr;
        }
    }

    //return type is intrinsic return type
    return intrinsic->return_type->make_copy();
}

Type* HexLiteral::resolve_type(CompilationContext *ctx) {
    // - can hex string fit in u64?
    if(hex_str.size() > 16) {
        std::cout << "Hex value too large : 0x" << hex_str << "\n";
        return nullptr;
    }

    // - make sure all characters are correct
    for(int i = 0; i < hex_str.size(); i++){
        char c = hex_str[i];
        if('0' <= c && c <= '9') continue;
        if('a' <= c && c <= 'f') continue;
        if('A' <= c && c <= 'F') continue;
        std::cout << "Invalid hex character : " << c << " in hex string : " << hex_str << "\n";
        return nullptr;
    }

    return primitives::u64->make_copy();
}

Type* BinaryLiteral::resolve_type(CompilationContext *ctx) {
    // - can binary string fit in u64?
    if(bin_str.size() > 64) {
        std::cout << "Binary value too large : 0b" << bin_str << "\n";
        return nullptr;
    }

    return primitives::u64->make_copy();
}

Type* OctalLiteral::resolve_type(CompilationContext *ctx) {
    // - can octal string fit in u64?
    if(oct_str.size() > 21) {
        std::cout << "Octal value too large : 0o" << oct_str << "\n";
        return nullptr;
    }

    return primitives::u64->make_copy();
}

Type* FunctionPointerLiteral::resolve_type(CompilationContext *ctx) {
    FunctionSignature *fs = new FunctionSignature(id, param_types);

    // - is there a corresponding function to this?
    // TODO decide if this should also map to operators
    // - how are we going to deal with builtin operators?
    // - this feature would be very nice actually
    Function *f = ctx->get_called_function(fs);
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

void FloatLiteral::emit_asm(CompilationContext *ctx) {
    fout << indent() << "mov " << float_to_hex_string(val) << ", %eax\n";
}

void IntegerLiteral::emit_asm(CompilationContext *ctx) {
    fout << indent() << "mov $" << val << ", %rax\n";
}

void SizeofLiteral::emit_asm(CompilationContext *ctx) {
    int sz = type->calc_size();
    fout << indent() << "mov $" << sz << ", %rax\n";
}

void CharLiteral::emit_asm(CompilationContext *ctx) {
    fout << indent() << "movb $" << (int) val << ", %al\n";
}

void StringLiteral::emit_asm(CompilationContext *ctx) {
    //add this string literal to controller
    add_string_literal(val);

    //get label
    std::string label = get_string_literal_label(val);

    //load address into %rax using %rip relative addressing
    fout << indent() << "lea " << label << "(%rip), %rax\n";
}

void SyscallLiteral::emit_asm(CompilationContext *ctx) {
    if(asm_debug) fout << indent() << "# syscall : " << syscall_id << "\n";

    //gather all arguments into tmp variables
    push_declaration_stack();
    std::vector<Variable*> argv;
    for(int i = 0; i < arguments.size(); i++){
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Type *vt = arguments[i]->resolve_type(ctx);
        assert(vt != nullptr);
        Variable *v = emit_initialize_stack_variable(ctx, vt, id, arguments[i]);
        argv.push_back(v);
    }
    assert(argv.size() == arguments.size());

    //insert syscall id into %rax
    assert(syscall_id >= 0);
    fout << indent() << "mov $" << syscall_id << ", %rax\n";

    //insert arguments into argument registers
    //zero out any extras
    std::vector<std::string> arg_regs = {"%rdi", "%rsi", "%rdx", "%r10", "%r8", "%r9"};
    assert(arguments.size() <= 6);
    for(int i = 0; i < 6; i++) {
        if(arguments.size() > i) fout << indent() << "movq " << argv[i]->addr << ", " << arg_regs[i] << "\n";
        else fout << indent() << "mov $0, " << arg_regs[i] << "\n";
    }

    //do syscall
    fout << indent() << "syscall\n";

    //cleanup tmp variables
    pop_declaration_stack(ctx);

    if(asm_debug) fout << indent() << "# done syscall : " << syscall_id << "\n";
}

void IntrinsicLiteral::emit_asm(CompilationContext *ctx) {
    if(asm_debug) fout << indent() << "# intrinsic : " << intrinsic_name << "\n";

    //gather all arguments into tmp variables
    push_declaration_stack();
    std::vector<Variable*> argv;
    for(int i = 0; i < arguments.size(); i++){
        Identifier *id = new Identifier(create_new_tmp_variable_name());
        Type *vt = arguments[i]->resolve_type(ctx);
        assert(vt != nullptr);
        Variable *v = emit_initialize_stack_variable(ctx, vt, id, arguments[i]);
        argv.push_back(v);
    }
    assert(argv.size() == arguments.size());

    //call intrinsic
    emit_intrinsic_call(intrinsic_name);

    //cleanup tmp variables
    pop_declaration_stack(ctx);

    if(asm_debug) fout << indent() << "# done intrinsic : " << intrinsic_name << "\n";
}

void HexLiteral::emit_asm(CompilationContext *ctx) {
    assert(hex_str.size() >= 1 && hex_str.size() <= 16);
    fout << indent() << "mov $0x" << hex_str << ", %rax\n";
}

void BinaryLiteral::emit_asm(CompilationContext *ctx) {
    assert(bin_str.size() >= 1 && bin_str.size() <= 64);
    uint64_t val = 0;
    for(int i = 0; i < bin_str.size(); i++){
        val = (val * 2) + (bin_str[i] - '0');
    }
    fout << indent() << "mov $" << val << ", %rax\n";
}

void OctalLiteral::emit_asm(CompilationContext *ctx) {
    assert(oct_str.size() >= 1 && oct_str.size() <= 21);
    uint64_t val = 0;
    for(int i = 0; i < oct_str.size(); i++) {
        val = (val * 8) + (oct_str[i] - '0');
    }
    fout << indent() << "mov $" << val << ", %rax\n";
}

void FunctionPointerLiteral::emit_asm(CompilationContext *ctx) {
    FunctionSignature *fs = new FunctionSignature(id, param_types);
    Function *f = ctx->get_called_function(fs);
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

size_t IntrinsicLiteral::hash() {
    size_t hash = 0;
    hash_combine(hash, std::hash<std::string>()(intrinsic_name));
    for(int i = 0; i < arguments.size(); i++) {
        hash_combine(hash, arguments[i]->hash());
    }
    return hash;
}

size_t HexLiteral::hash() {
    return std::hash<std::string>()(hex_str);
}

size_t BinaryLiteral::hash() {
    return std::hash<std::string>()(bin_str);
}

size_t OctalLiteral::hash() {
    return std::hash<std::string>()(oct_str);
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

bool IntrinsicLiteral::equals(Literal *_other) {
    if(dynamic_cast<IntrinsicLiteral*>(_other) == nullptr) return false;
    IntrinsicLiteral *other = dynamic_cast<IntrinsicLiteral*>(_other);

    if(intrinsic_name != other->intrinsic_name) return false;
    if(arguments.size() != other->arguments.size()) return false;
    for(int i = 0; i < arguments.size(); i++){
        if(!arguments[i]->equals(other->arguments[i])) return false;
    }
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

bool OctalLiteral::equals(Literal *_other) {
    if(dynamic_cast<OctalLiteral*>(_other) == nullptr) return false;
    OctalLiteral *other = dynamic_cast<OctalLiteral*>(_other);

    return oct_str == other->oct_str;
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
    return new FloatLiteral(*this);
}

Literal* IntegerLiteral::make_copy() {
    return new IntegerLiteral(*this);
}

Literal* SizeofLiteral::make_copy() {
    return new SizeofLiteral(*this);
}

Literal* CharLiteral::make_copy() {
    return new CharLiteral(*this);
}

Literal* StringLiteral::make_copy() {
    return new StringLiteral(*this);
}

Literal* SyscallLiteral::make_copy() {
    return new SyscallLiteral(*this);
}

Literal* IntrinsicLiteral::make_copy() {
    return new IntrinsicLiteral(*this);
}

Literal* HexLiteral::make_copy() {
    return new HexLiteral(*this);
}

Literal* BinaryLiteral::make_copy() {
    return new BinaryLiteral(*this);
}

Literal* OctalLiteral::make_copy() {
    return new OctalLiteral(*this);
}

Literal* FunctionPointerLiteral::make_copy() {
    return new FunctionPointerLiteral(*this);
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

std::string IntrinsicLiteral::to_string() {
    std::string ret = "intrinsic(" + intrinsic_name;
    for(int i = 0; i < arguments.size(); i++) {
        ret += ", " + arguments[i]->to_string();
    }
    ret += ")";
    return ret;
}

std::string HexLiteral::to_string() {
    return "0x" + hex_str;
}

std::string BinaryLiteral::to_string() {
    return "0b" + bin_str;
}

std::string OctalLiteral::to_string() {
    return "0o" + oct_str;
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
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    return true;
}

bool IntrinsicLiteral::replace_templated_types(TemplateMapping *mapping) {
    for(int i = 0; i < arguments.size(); i++) {
        if(!arguments[i]->replace_templated_types(mapping)) return false;
    }
    return true;
}

bool HexLiteral::replace_templated_types(TemplateMapping *mapping) {
    return true;
}

bool BinaryLiteral::replace_templated_types(TemplateMapping *mapping) {
    return true;
}

bool OctalLiteral::replace_templated_types(TemplateMapping *mapping) {
    return true;
}

bool FunctionPointerLiteral::replace_templated_types(TemplateMapping *mapping) {
    for(int i = 0; i < param_types.size(); i++){
        if(auto x = mapping->find_mapped_type(param_types[i])) param_types[i] = x;
        else if(!param_types[i]->replace_templated_types(mapping)) return false;
    }
    return true;
}