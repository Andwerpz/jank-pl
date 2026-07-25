#include <chrono>
#include <algorithm>
#include <iomanip>
#include <functional>

#include "utils.h"
#include "Type.h"
#include "Function.h"
#include "Expression.h"
#include "Identifier.h"
#include "FunctionSignature.h"
#include "FunctionCall.h"
#include "StructDefinition.h"
#include "Constructor.h"
#include "ConstructorSignature.h"
#include "ConstructorCall.h"
#include "TemplatedStructDefinition.h"
#include "Program.h"
#include "TemplatedFunction.h"
#include "StructLayout.h"
#include "primitives.h"
#include "Destructor.h"
#include "DestructorCall.h"
#include "Statement.h"
#include "Literal.h"
#include "Parameter.h"
#include "Declaration.h"
#include "TemplateHeader.h"
#include "OperatorSignature.h"
#include "Operator.h"
#include "TemplatedOperator.h"
#include "OperatorCall.h"
#include "CompilationContext.h"
#include "DefinitionSpace.h"
#include "ASTNode.h"
#include "TemplateMapping.h"
#include "GlobalDeclaration.h"
#include "GlobalNode.h"

// -- HASHING STRUCTS --
size_t TypeHash::operator()(Type* t) const {
    return t ? t->hash() : 0;
}
bool TypeEquals::operator()(Type* lhs, Type* rhs) const {
    return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
}

size_t IdentifierHash::operator()(Identifier* id) const {
    return id ? id->hash() : 0;
}
bool IdentifierEquals::operator()(Identifier *lhs, Identifier *rhs) const {
    return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
}

size_t FunctionSignatureHash::operator()(FunctionSignature *fc) const {
    return fc ? fc->hash() : 0;
}
bool FunctionSignatureEquals::operator()(FunctionSignature* lhs, FunctionSignature *rhs) const {
    return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
}

size_t ConstructorSignatureHash::operator()(ConstructorSignature *cs) const {
    return cs ? cs->hash() : 0;
}
bool ConstructorSignatureEquals::operator()(ConstructorSignature *lhs, ConstructorSignature *rhs) const {
    return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
}

size_t OperatorSignatureHash::operator()(OperatorSignature *os) const {
    return os ? os->hash() : 0;
}
bool OperatorSignatureEquals::operator()(OperatorSignature* lhs, OperatorSignature *rhs) const {
    return (!lhs || !rhs) ? lhs == rhs : lhs->equals(rhs);
}

size_t DeclarableHash::operator()(std::pair<Type*, Expression*> p) const {
    size_t hash = 0;
    if(p.first) hash_combine(hash, p.first->hash());
    else hash_combine(hash, 0);
    if(p.second) hash_combine(hash, p.second->hash());
    else hash_combine(hash, 0);
    return hash;
}
bool DeclarableEquals::operator()(std::pair<Type*, Expression*> lhs, std::pair<Type*, Expression*> rhs) const {
    if((lhs.first == nullptr) ^ (rhs.first == nullptr)) return false;
    if((lhs.second == nullptr) ^ (rhs.second == nullptr)) return false;
    if(lhs.first && !lhs.first->equals(rhs.first)) return false;
    if(lhs.second && !lhs.second->equals(rhs.second)) return false;
    return true;
}

// -- UTIL STRUCTS --
Variable::Variable(bool _is_global, bool _is_extern, Type *_type, Identifier *_id, std::string _addr) {
    is_global = _is_global;
    is_extern = _is_extern;
    id = _id;
    type = _type;
    addr = _addr;
}

LoopContext::LoopContext(std::string _start_label, std::string _assignment_label, std::string _end_label, int _declaration_layer) {
    start_label = _start_label;
    assignment_label = _assignment_label;
    end_label = _end_label;
    declaration_layer = _declaration_layer;
}

Package::Package() {
    is_named = false;
    name = "";
    path = "";
}

Package::Package(std::string _name, std::string _path) {
    is_named = true;
    name = _name;
    path = _path;
}

// -- GENERAL UTILS --
std::vector<std::pair<std::string, ld>> duration_stats;

ld current_time_seconds() {
    using namespace std::chrono;
    auto now = system_clock::now().time_since_epoch();
    return duration_cast<duration<ld>>(now).count();
}

void add_duration_stat(std::string name, ld dur) {
    duration_stats.push_back({name, dur});
}

void print_duration_stats() {
    std::sort(duration_stats.begin(), duration_stats.end(), [](std::pair<std::string, ld> a, std::pair<std::string, ld> b) -> bool {
        return a.second < b.second;
    });
    for(int i = 0; i < duration_stats.size(); i++){
        std::cout << duration_stats[i].first << " : " << std::fixed << std::setprecision(3) << duration_stats[i].second << "\n";
    }
}

void hash_combine(size_t& seed, size_t value) {
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

std::string read_file(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::ostringstream buffer;
    buffer << file.rdbuf(); 
    return buffer.str();     
}

std::vector<std::string> str_split(const std::string& s, char sep) {
    std::vector<std::string> ret;
    for(int i = 0; i < s.size(); ) {
        if(s[i] == sep) {
            i ++;
            continue;
        }
        int r = i;
        while(r != s.size() && s[r] != sep) r ++;
        ret.push_back(s.substr(i, r - i));
        i = r;
    }
    return ret;
}

std::string extract_filename(std::string path) {
    return *(str_split(path, '/').rbegin());
}

std::string extract_folder_path(std::string path) {
    for(int i = (int) path.size() - 1; i >= 0; i--){
        if(path[i] == '/') return path.substr(0, i + 1);
    }
    //there is no '/', just return "./"
    return "./";
}

std::string extract_stem(std::string filename) {
    for(int i = filename.size() - 1; i >= 0; i--) {
        if(filename[i] == '.') return filename.substr(0, i);
    }
    return filename;
}   

std::string extract_ext(std::string filename) {
    for(int i = filename.size() - 1; i >= 0; i--){
        if(filename[i] == '.') return filename.substr(i);
    }
    return "";
}

std::string normalize_path(std::string path) {
    std::vector<std::string> parts = str_split(path, '/');
    std::vector<std::string> nparts;
    for(int i = 0; i < parts.size(); i++) {
        if(parts[i] == ".") continue;
        if(parts[i] == "..") {
            if(nparts.size() > 0) nparts.pop_back();
            continue;
        }
        nparts.push_back(parts[i]);
    }
    path = "";
    for(int i = 0; i < nparts.size(); i++) {
        path += "/" + nparts[i];
    }
    return path;
}

std::string labelize_path(std::string path) {
    path = normalize_path(path);
    std::vector<std::string> parts = str_split(path, '/');
    std::string label = "";
    for(int i = 0; i < parts.size(); i++) {
        label += "_" + parts[i];
    }
    return label;
}

std::string cwd_rel_to_absolute(std::string path){
    if(path[0] == '/') return path;
    return cwd_dir + "/" + path;
}

std::string libj_to_absolute(std::string name) {
    return compiler_dir + "/libj/" + name + ".jank";
}

// -- PARSE UTILS --
//"n" | "t" | "r" | "f" | "b" | "\"" | "\\" | "'" | "0"
char escape_to_char(parser::escape *e) {
    char val;
    char eid = e->to_string()[1];
    if(eid == 'n') val = '\n';
    else if(eid == 't') val = '\t';
    else if(eid == 'r') val = '\r';
    else if(eid == 'f') val = '\f';
    else if(eid == 'b') val = '\b';
    else if(eid == '\"') val = '\"';
    else if(eid == '\\') val = '\\';
    else if(eid == '\'') val = '\'';
    else if(eid == '0') val = '\0';
    else assert(false);
    return val;
}

std::vector<Type*> convert_type_list(parser::type_list *t) {
    std::vector<Type*> arr;
    if(t->t0.has_value()) {
        arr.push_back(Type::convert(t->t0.value()->t0));
        for(int i = 0; i < t->t0.value()->t1.size(); i++){
            arr.push_back(Type::convert(t->t0.value()->t1[i]->t3));
        }
    }
    return arr;
}

std::vector<Parameter*> convert_parameter_list(parser::parameter_list *t) {
    std::vector<Parameter*> arr;
    if(t->t0.has_value()) {
        arr.push_back(Parameter::convert(t->t0.value()->t0));
        for(int i = 0; i < t->t0.value()->t1.size(); i++){
            arr.push_back(Parameter::convert(t->t0.value()->t1[i]->t3));
        }
    }
    return arr;
}

std::vector<Expression*> convert_argument_list(parser::argument_list *t) {
    std::vector<Expression*> arr;
    if(t->t0.has_value()) {
        arr.push_back(Expression::convert(t->t0.value()->t0));
        for(int i = 0; i < t->t0.value()->t1.size(); i++){
            arr.push_back(Expression::convert(t->t0.value()->t1[i]->t3));
        }
    }
    return arr;
}

std::vector<Identifier*> convert_identifier_list(parser::identifier_list *t) {
    std::vector<Identifier*> arr;
    if(t->t0.has_value()) {
        arr.push_back(Identifier::convert(t->t0.value()->t0));
        for(int i = 0; i < t->t0.value()->t1.size(); i++){
            arr.push_back(Identifier::convert(t->t0.value()->t1[i]->t3));
        }
    }
    return arr;
}


// -- EMIT ASM HELPERS --
std::string indent() {
    return "    ";  // 4 spaces
}

std::string create_new_label() {
    std::string ans = "L" + std::to_string(label_counter++);
    return ans;
}

std::string create_new_tmp_variable_name() {
    std::string ans = "TMP:" + std::to_string(tmp_variable_counter ++);
    return ans;
}

void emit_push(std::string reg, std::string desc){
    stack_desc.push_back(desc);
    fout << indent() << "push " << reg << (asm_debug? "    # " + desc : "") << "\n";
    local_offset -= 8;
}

void emit_pop(std::string reg, std::string desc){
    if(stack_desc.size() == 0) {
        std::cout << "Stack empty : " << desc << std::endl;
        assert(false);
    }
    if(stack_desc[stack_desc.size() - 1] != desc) {
        std::cout << "Stack desc doesn't match : " << desc << std::endl;
        dump_stack_desc();
        assert(false);
    }
    stack_desc.pop_back();

    fout << indent() << "pop " << reg << (asm_debug? "    # " + desc : "") <<"\n";
    local_offset += 8;
}

void emit_add_rsp(int amt, std::string desc){
    assert(amt % 8 == 0);
    assert(amt >= 0);
    for(int i = 0; i < amt / 8; i++) {
        if(stack_desc.size() == 0) {
            std::cout << "Stack empty : " << desc << std::endl;
            assert(false);
        }
        if(stack_desc[stack_desc.size() - 1] != desc) {
            std::cout << "Stack desc doesn't match : " << desc << std::endl;
            dump_stack_desc();
            assert(false);
        }
        stack_desc.pop_back();
    }

    fout << indent() << "add $" << amt << ", %rsp" << (asm_debug? "    # " + desc : "") << "\n";
    local_offset += amt;
}

void emit_add_rsp(int amt, std::vector<std::string> desc_list) {
    assert(amt % 8 == 0);
    assert(amt >= 0);
    assert(amt / 8 == desc_list.size());

    if(desc_list.size() > stack_desc.size()) {
        std::cout << "Not enough elements in stack : multi emit add" << std::endl;
        dump_stack_desc();
        assert(false);
    }

    for(int i = 0; i < amt / 8; i++) {
        if(stack_desc[stack_desc.size() - 1] != desc_list[desc_list.size() - 1]) {
            std::cout << "Stack desc doesn't match : " << desc_list[desc_list.size() - 1] << std::endl;
            dump_stack_desc();
            assert(false);
        }
        stack_desc.pop_back();
        desc_list.pop_back();
    }

    fout << indent() << "add $" << amt << ", %rsp\n";
    local_offset += amt;
}

void emit_sub_rsp(int amt, std::string desc) {
    assert(amt % 8 == 0);
    assert(amt >= 0);
    for(int i = 0; i < amt / 8; i++){
        stack_desc.push_back(desc);
    }

    if(asm_debug) fout << indent() << "# " << desc << "\n";
    fout << indent() << "sub $" << amt << ", %rsp\n";
    local_offset -= amt;
}


// -- EMIT ASM MEMORY ACCESS --
//expects %rax = array start, %rbx = array index
//will put return value into %rax
//always zero extends %rax
void emit_retrieve_array(int sz) {
    if(sz == 1) fout << indent() << "movzbl (%rax, %rbx, 1), %eax\n";
    else if(sz == 2) fout << indent() << "movzwl (%rax, %rbx, 2), %eax\n";
    else if(sz == 4) fout << indent() << "movl (%rax, %rbx, 4), %eax\n";
    else if(sz == 8) fout << indent() << "movq (%rax, %rbx, 8), %rax\n";
    else {
        std::cout << "Cannot retrieve from array with element of size : " << sz << std::endl;
        assert(false);
    }
}

//expects %rax = array start, %rbx = array index
//will put address of element into %rax
void emit_address_array(int sz) {
    if(sz == 1) fout << indent() << "lea (%rax, %rbx, 1), %rax\n";
    else if(sz == 2) fout << indent() << "lea (%rax, %rbx, 2), %rax\n";
    else if(sz == 4) fout << indent() << "lea (%rax, %rbx, 4), %rax\n";
    else if(sz == 8) fout << indent() << "lea (%rax, %rbx, 8), %rax\n";
    else {
        emit_push("%rax", "emit_address_array() : save array start");
        fout << indent() << "mov $" << sz << ", %rax\n";
        fout << indent() << "imul %rax, %rbx\n";
        emit_pop("%rax", "emit_address_array() : save array start");
        fout << indent() << "add %rbx, %rax\n";
    }
}

//expects %rax = array start, %rbx = array index, %rcx = value
//writes %rcx (or whatever portion of %rcx) into array
void emit_write_array(int sz) {
    if(sz == 1) fout << indent() << "movb %cl, (%rax, %rbx, 1)\n";
    else if(sz == 2) fout << indent() << "movw %cx, (%rax, %rbx, 2)\n";
    else if(sz == 4) fout << indent() << "movl %ecx, (%rax, %rbx, 4)\n";
    else if(sz == 8) fout << indent() << "movq %rcx, (%rax, %rbx, 8)\n";
    else {
        std::cout << "Cannot write array with element of size : " << sz << std::endl;
        assert(false);
    }
}

//expects %rax = address
//will put return value into %rax
//always zero extends %rax if the amount is less than 8 bytes
void emit_mem_retrieve(int sz) {
    if(sz == 1) fout << indent() << "movzbq (%rax), %rax\n";
    else if(sz == 2) fout << indent() << "movzwq (%rax), %rax\n";
    else if(sz == 4) fout << indent() << "movl (%rax), %eax\n";
    else if(sz == 8) fout << indent() << "movq (%rax), %rax\n";
    else {
        std::cout << "Cannot retrieve element of size : " << sz << std::endl;
        assert(false);
    }   
}

//expects %rax = value, %rbx = mem address
//stores %rax into (%rbx)
void emit_mem_store(int sz) {
    if(sz == 1) fout << indent() << "movb %al, (%rbx)\n";
    else if(sz == 2) fout << indent() << "movw %ax, (%rbx)\n";
    else if(sz == 4) fout << indent() << "movl %eax, (%rbx)\n";
    else if(sz == 8) fout << indent() << "movq %rax, (%rbx)\n";
    else {
        std::cout << "Cannot store element of size : " << sz << std::endl;
        assert(false);
    }
}

//allocates sz_bytes memory by calling malloc. Resulting address is in %rax
void emit_malloc(int sz_bytes) {
    assert(sz_bytes >= 0);

    FunctionSignature *malloc_signature = new FunctionSignature(new Identifier("malloc"), {primitives::u64});
    std::string malloc_label = get_function_label(malloc_signature);

    fout << indent() << "mov $" << sz_bytes << ", %rax\n";
    emit_push("%rax", "emit_malloc() : malloc arg");
    fout << indent() << "call " << malloc_label << "\n";
    emit_add_rsp(8, "emit_malloc() : malloc arg");
}

//frees sz_bytes memory at address provided by %rax
//resulting free status is in %rax
void emit_free(int sz_bytes) {
    assert(sz_bytes >= 0);

    FunctionSignature *free_signature = new FunctionSignature(new Identifier("free"), {new PointerType(primitives::_void), primitives::u64});
    std::string free_label = get_function_label(free_signature);

    emit_push("%rax", "emit_free() : free addr");
    fout << indent() << "mov $" << sz_bytes << ", %rax\n";
    emit_push("%rax", "emit_free() : free sz_bytes");
    fout << indent() << "call " << free_label << "\n";
    std::vector<std::string> stk_strs = {"emit_free() : free addr", "emit_free() : free sz_bytes"};
    emit_add_rsp(16, stk_strs);
}

//assumes reference type is in %rax (so some sort of memory pointer)
//returns with value in %rax, address in %rcx
void emit_dereference(Type *rt) {
    assert(rt != nullptr);
    Type *t = nullptr;
    if(auto _t = dynamic_cast<ReferenceType*>(rt)) {
        t = _t->type;
    }
    else if(auto _t = dynamic_cast<PointerType*>(rt)) {
        t = _t->type;
    }
    else {
        std::cout << "Can only emit dereference for pointer or reference type" << std::endl;
        assert(false);
    }
    
    if(asm_debug) fout << indent() << "# dereferencing to type " << t->to_string() << "\n";
    fout << indent() << "mov %rax, %rcx\n";
    if(is_type_primitive(t)) {
        //we should only load stuff if the base type is a primitive. 
        int sz = t->calc_size();
        emit_mem_retrieve(sz); 
    }
}


// -- EMIT ASM CLEANUP --
//expects mem addr of the struct to be in %rax
void emit_destructor_call(CompilationContext *ctx, Type *t, bool should_dealloc) {
    DestructorCall *dc = new DestructorCall(t);
    assert(dc->resolve_type(ctx) != nullptr);
    dc->emit_asm(ctx, should_dealloc);
}

//expects mem addr of the struct to be in %rax, returns with mem addr of the struct in %rax
//calls destructor of all member variables of the struct in reverse order
//does not deallocate struct memory
void emit_cleanup_struct(CompilationContext *ctx, Type *t) {
    StructLayout *sl = get_struct_layout(t);
    assert(sl != nullptr);
    if(auto atype = dynamic_cast<ArrayType*>(t)) {
        Type *bt = atype->type;
        int bt_sz = bt->calc_size();
        assert(bt != nullptr);
        if(!is_type_primitive(bt)) {
            // TODO replace this with a loop or smth
            for(int i = atype->amt - 1; i >= 0; i--){
                //save base struct address
                emit_push("%rax", "emit_cleanup_struct() : target struct");

                //move member variable address into %rax
                fout << indent() << "add $" << i * bt_sz << ", %rax\n";

                //call destructor, no dealloc
                emit_destructor_call(ctx, bt, false);

                //retrieve base struct address
                emit_pop("%rax", "emit_cleanup_struct() : target struct");
            }
        }
    }
    else {
        for(int i = (int) sl->member_variables.size() - 1; i >= 0; i--){
            Type *mvt = sl->member_variables[i]->type;
            Identifier *mvid = sl->member_variables[i]->id;
            int offset = sl->get_offset(mvid);
            if(!is_type_primitive(mvt)) {
                //save base struct address
                emit_push("%rax", "emit_cleanup_struct() : target struct");

                //move member variable address into %rax
                fout << indent() << "add $" << offset << ", %rax\n";

                //call destructor, no dealloc
                emit_destructor_call(ctx, mvt, false);

                //retrieve base struct address
                emit_pop("%rax", "emit_cleanup_struct() : target struct");
            }
        }
    }
}

//just emits frees, should not actually affect the controller
//this should only be called outside of pop_declaration_stack() in special cases (return, break, continue)
//specifically saves registers %rax, %rcx so that this can be called inside expressions
//specifically ignores variables named in 'ignore' (used in return to let variables escape destruction if we return them)
void emit_cleanup_declaration_stack_layer(CompilationContext *ctx, int layer_ind, std::vector<Identifier*> ignore) {
    assert(layer_ind >= 0 && layer_ind < declaration_stack.size());
    std::vector<Variable*> layer = declaration_stack[layer_ind];

    //see if there is anything to do
    std::vector<Variable*> to_clean;
    for(int i = 0; i < layer.size(); i++) {
        assert(!layer[i]->is_global);
        Type *t = layer[i]->type;
        Identifier* id = layer[i]->id;
        if(is_type_primitive(t)) continue;
        bool ignored = false;
        for(int j = 0; j < ignore.size(); j++) {
            if(ignore[j]->equals(id)) {
                ignored = true;
                break;
            }
        }
        if(ignored) {
            continue;
        }
        to_clean.push_back(layer[i]);
    }
    if(to_clean.size() == 0) {
        return;
    }
    
    //save %rax, %rcx
    emit_push("%rax", "emit_cleanup_declaration_stack_layer() : save %rax");
    emit_push("%rcx", "emit_cleanup_declaration_stack_layer() : save %rcx");

    //destruct any non-primitive variables
    assert(to_clean.size() != 0);
    for(int i = to_clean.size() - 1; i >= 0; i--){
        assert(!to_clean[i]->is_global);
        Type *t = to_clean[i]->type;
        if(!is_type_primitive(t)) {
            //put addr to struct in %rax
            fout << indent() << "movq " << to_clean[i]->addr << ", %rax\n";

            //call destructor
            emit_destructor_call(ctx, t, true);
        }
    }   

    //retrieve %rax, %rcx
    emit_pop("%rcx", "emit_cleanup_declaration_stack_layer() : save %rcx");
    emit_pop("%rax", "emit_cleanup_declaration_stack_layer() : save %rax");
}

void emit_cleanup_declaration_stack_layer(CompilationContext *ctx, int layer_ind) {
    emit_cleanup_declaration_stack_layer(ctx, layer_ind, {});
}


// -- EMIT ASM INITIALIZATION --
//expects memory address in %rax, places primitive into address, returns with memory address in %rax
//expects memory to already have been allocated
void emit_initialize_primitive(Type *t) {
    if(!is_type_primitive(t)) {
        std::cout << "Tried to initialize unrecognized primitive type : " << t->to_string() << std::endl;
        assert(false);
    }

    //every primitive should be 0 initialized
    int sz = t->calc_size();
    if(sz == 1) fout << indent() << "movb $0, (%rax)\n";
    else if(sz == 2) fout << indent() << "movw $0, (%rax)\n";
    else if(sz == 4) fout << indent() << "movl $0, (%rax)\n";
    else if(sz == 8) fout << indent() << "movq $0, (%rax)\n";
    else assert(false);
}

//expects memory address in %rax, initializes struct, returns with memory address in %rax
//expects memory to already have been allocated
void emit_initialize_array(CompilationContext *ctx, ArrayType *t) {
    if(debug) std::cout << "EMIT INITIALIZE ARRAY : " << t->to_string() << std::endl;
    if(asm_debug) fout << indent() << "# initialize array " << t->to_string() << "\n";

    emit_push("%rax", "emit_initialize_array() :: save original %rax");

    Type *bt = t->type;
    int bt_sz = bt->calc_size();

    for(int i = 0; i < t->amt; i++){
        if(is_type_primitive(bt)) {
            emit_initialize_primitive(bt);
        }
        else {
            //invoke struct default constructor
            emit_push("%rax", "emit_initialize_array() :: save %rax before constructor call");

            //%rax already holds member struct memory address
            ConstructorCall *cc = new ConstructorCall(std::nullopt, bt, {});
            assert(cc->resolve_type(ctx)->equals(bt));
            cc->emit_asm(ctx, true);

            emit_pop("%rax", "emit_initialize_array() :: save %rax before constructor call");
        }

        //increment %rax
        fout << indent() << "add $" << bt_sz << ", %rax\n";
    }

    emit_pop("%rax", "emit_initialize_array() :: save original %rax");

    if(asm_debug) fout << indent() << "# done initialize array memory " << t->to_string() << "\n";
}

//expects memory address in %rax, initializes struct, returns with memory address in %rax
//expects memory to already have been allocated
void emit_initialize_struct(CompilationContext *ctx, Type *t) {
    assert(t != nullptr);

    //check if we're actually trying to initialize a primitive
    if(is_type_primitive(t)) {
        emit_initialize_primitive(t);
        return;
    }

    //check if we're trying to initialize an array
    if(auto x = dynamic_cast<ArrayType*>(t)) {
        emit_initialize_array(ctx, x);
        return;
    }

    StructLayout *sl = get_struct_layout(t);
    assert(sl != nullptr);

    if(asm_debug) fout << indent() << "# initialize struct " << t->to_string() << "\n";
    
    emit_push("%rax", "emit_initialize_struct() :: save original %rax");

    for(int i = 0; i < sl->member_variables.size(); i++){
        MemberVariable *mv = sl->member_variables[i];
        int mv_size = mv->type->calc_size();
        if(is_type_primitive(mv->type)) {
            emit_initialize_primitive(mv->type);
        }
        else {
            //invoke struct default constructor
            emit_push("%rax", "emit_initialize_struct() :: save %rax before constructor call");

            //%rax already holds member struct memory address
            ConstructorCall *cc = new ConstructorCall(std::nullopt, mv->type->make_copy(), {});
            assert(cc->resolve_type(ctx)->equals(mv->type));
            cc->emit_asm(ctx, true);

            emit_pop("%rax", "emit_initialize_struct() :: save %rax before constructor call");
        }
        
        //increment %rax
        fout << indent() << "add $" << mv_size << ", %rax\n";
    }

    emit_pop("%rax", "emit_initialize_struct() :: save original %rax");

    if(asm_debug) fout << indent() << "# done initialize struct memory " << t->to_string() << "\n";
}

//allocates the next stack slot and initializes the variable into it. 
//also initializes a variable into the global compiler context
//returns the variable on success, nullptr on failure. 
Variable* emit_initialize_stack_variable(CompilationContext *ctx, Type *vt, Identifier *id, std::optional<Expression*> expr) {
    //is the variable already declared?
    if(is_variable_declared(id)) {
        std::cout << "Cannot redeclare variable : " << vt->to_string() << " " << id->name << "\n";
        return nullptr;
    }

    //claim next stack slot
    emit_sub_rsp(8, id->name);
    std::string addr_str = std::to_string(local_offset) + "(%rbp)";

    //register variable
    Variable *v = add_variable(vt, id, addr_str, false, false);
    assert(v != nullptr);

    //initialize
    if(!emit_initialize_variable(ctx, vt, id, expr, addr_str, false, false)) {
        return nullptr;
    }

    return v;
}

//emits assembly to initialize a global variable 
bool emit_initialize_global_variable(CompilationContext *ctx, Type *vt, Identifier *id, std::optional<Expression*> expr, bool is_extern) {
    std::string addr_str = id->name + "(%rip)";
    return emit_initialize_variable(ctx, vt, id, expr, addr_str, true, is_extern);
}

//should be logically similar to is_declarable(), except this one emits a variable declaration 
//evaluates the expression and initializes the expression value into the provided memory address
//if for some reason is unable to initialize the variable, returns nullptr
//expects the address at addr_str to be already allocated
//note that this does not register a Variable, use 'emit_initialize_stack_variable' for that
//  this method does expect that the variable being initialized is already registered
bool emit_initialize_variable(CompilationContext *ctx, Type *vt, Identifier *id, std::optional<Expression*> expr, std::string addr_str, bool is_global, bool is_extern) { 
    assert(vt != nullptr);
    assert(id != nullptr);
    if(expr.has_value()) assert(expr.value() != nullptr);

    if(debug) std::cout << "Initialize variable : " << vt->to_string() << " " << id->name << std::endl;

    // - make sure vt is declared
    if(!ctx->is_type_declared(vt)) {
        std::cout << "Type " << vt->to_string() << " is not declared\n";
        std::cout << "Context : " << std::endl;
        for(CompilationContext::DefinitionSpaceView dsv : ctx->definition_spaces) {
            std::cout << "    " << dsv.space->get_filepath() << " : " << static_cast<int>(dsv.vis) << std::endl;
        }
        return false;
    }
    // - make sure vt is not void
    if(vt->equals(primitives::_void)) {
        std::cout << "Cannot initialize a variable of type void\n";
        return false;
    }
    // - does the expression resolve to a type?
    if(expr.has_value() && expr.value()->resolve_type(ctx) == nullptr){
        std::cout << "Expression does not resolve to type\n";
        return false;
    }

    if(dynamic_cast<ReferenceType*>(vt) != nullptr) {
        vt = dynamic_cast<ReferenceType*>(vt)->type;
        // - extern variables cannot be of reference type
        if(is_extern) {
            std::cout << "Extern variable cannot be reference type\n";
            return false;
        }
        // - must assign a value when initializing references
        if(!expr.has_value()) {
            std::cout << "Cannot default initialize reference\n";
            return false;
        }
        // - must use l-value when binding references
        if(!expr.value()->is_lvalue(ctx)) {
            std::cout << "Cannot assign r-value to reference\n";
            return false;
        }
        // - must bind to reference something of matching type (no conversions) 
        if(*vt != *(expr.value()->resolve_type(ctx))) {
            std::cout << "Cannot assign non-matching type to reference\n";
            return false;
        }

        //zero initialize primitive
        fout << indent() << "movq $0, " << addr_str << "\n";

        //evaluate expr
        //%rax = value, %rcx = addr
        expr.value()->emit_asm(ctx);

        //save addr into given addr
        fout << indent() << "movq %rcx, " << addr_str << "\n";
    }
    else if(is_type_primitive(vt)) {
        if(!is_extern) {
            //zero initialize variable
            fout << indent() << "movq $0, " << addr_str << "\n";
        }
        if(expr.has_value()) {
            //assign the variable to the value
            Expression *a_expr = new Expression(new ExprBinary(new ExprPrimary(id), "=", expr.value()->expr_node));
            if(a_expr->resolve_type(ctx) == nullptr) {
                std::cout << "Cannot assign expression into variable type : " << vt->to_string() << ", " << expr.value()->to_string() << "\n";
                return false;
            }
            a_expr->emit_asm(ctx);
        }
    }
    else {
        // - don't call default constructor, then immediately call copy constructor. Should only call one constructor
        // - don't call copy constructor when initializing a variable using a r-value
        //   the variable should instead just become the r-value
        //   make sure to not auto-dealloc the r-value after emitting the expression
        // - if const gets implemented, don't have to copy l-values into const variables, can just pass a reference

        if(expr.has_value()) {
            //if the expression type is the same as the variable type, we can directly assign
            //otherwise we have to create a subexpression to do overload resolution
            Type* et = expr.value()->resolve_type(ctx);
            assert(et != nullptr);

            if(!et->equals(vt)) {
                //default construct variable
                ConstructorCall *cc = new ConstructorCall(std::nullopt, vt, {});
                assert(cc->resolve_type(ctx)->equals(vt));
                cc->emit_asm(ctx, false);
                fout << indent() << "movq %rax, " << addr_str << "\n";

                //evaluate assignment expression
                Expression *a_expr = new Expression(new ExprBinary(new ExprPrimary(id), "=", expr.value()->expr_node));
                if(a_expr->resolve_type(ctx) == nullptr) {
                    std::cout << "Cannot assign expression into variable type : " << vt->to_string() << ", " << expr.value()->to_string() << "\n";
                    return false;
                }
                a_expr->emit_asm(ctx);
            }
            else if(expr.value()->is_lvalue(ctx)) {
                //evaluate expression, make copy
                ConstructorCall *cc = new ConstructorCall(std::nullopt, vt->make_copy(), {expr.value()->make_copy()});
                assert(cc->resolve_type(ctx)->equals(vt));
                cc->emit_asm(ctx, false);

                //move copy into variable
                fout << indent() << "movq %rax, " << addr_str << "\n";
            }
            else {
                //evaluate expression
                expr.value()->emit_asm(ctx, false);  //don't dealloc r-value struct
                
                //move directly into variable
                fout << indent() << "movq %rax, " << addr_str << "\n";
            }
        }
        else if(!is_extern) {
            //default construct variable
            ConstructorCall *cc = new ConstructorCall(std::nullopt, vt, {});
            assert(cc->resolve_type(ctx)->equals(vt));
            cc->emit_asm(ctx, false);

            //save pointer to addr
            fout << indent() << "movq %rax, " << addr_str << "\n";
        }
    }

    return true;
}


// -- LOCAL VARIABLE STATE --
Variable* get_variable(Identifier *id) {
    assert(id != nullptr);
    for(int i = 0; i < declared_variables.size(); i++){
        if(*id == *(declared_variables[i]->id)) {
            return declared_variables[i];
        }
    }
    return nullptr;
}

Type* get_variable_type(Identifier *id) {
    assert(id != nullptr);
    Variable *v = get_variable(id);
    if(v == nullptr) {
        return nullptr;
    }
    return v->type;
}

//takes whatever the current local offset is as the address
Variable* add_stack_variable(Type *t, Identifier *id) {
    return add_variable(t, id, std::to_string(local_offset) + "(%rbp)", false, false);
}

//takes the name as the address
Variable* add_global_variable(Type *t, Identifier *id, bool is_extern) {
    return add_variable(t, id, id->name + "(%rip)", true, is_extern);
}

Variable* add_variable(Type *t, Identifier *id, std::string addr_str, bool is_global, bool is_extern) {
    assert(t != nullptr && id != nullptr);
    if(is_extern) assert(is_global);                        //extern variables must be global
    if(!is_global) assert(declaration_stack.size() != 0);   //stack variables must be declared on an existing declaration stack
    if(is_variable_declared(id)) {
        std::cout << "Cannot redeclare " << t->to_string() << " " << id->name << "\n";
        return nullptr;
    }
    Variable *v = new Variable(is_global, is_extern, t, id, addr_str);
    declared_variables.push_back(v);
    if(!is_global) declaration_stack.rbegin()->push_back(v);
    else declared_global_variables.push_back(v);
    return v;
}

//removes the given variable from the global context
//if the variable is a global variable, also removes it from the global variable list
void remove_variable(Identifier *id) {
    for(int i = 0; i < declared_variables.size(); i++) {
        if(!id->equals(declared_variables[i]->id)) continue;
        Variable *v = declared_variables[i];
        if(v->is_global) {
            bool found = false;
            for(int j = 0; j < declared_global_variables.size(); j++) {
                if(!id->equals(declared_global_variables[j]->id)) continue;
                found = true;
                declared_global_variables.erase(declared_global_variables.begin() + j);
                break;
            }
            assert(found);
        }
        declared_variables.erase(declared_variables.begin() + i);
        return;
    }
    assert(false);
}

//can only be called when there are only global variables registered
//this just checks that by making sure the amount of global variables is equal to the amount of
//  total declared variables
void remove_all_global_variables() {
    assert(declaration_stack.size() == 0);
    assert(declared_variables.size() == declared_global_variables.size());
    declared_variables.clear();
    declared_global_variables.clear();
}

bool is_variable_declared(Identifier *id) {
    for(int i = 0; i < declared_variables.size(); i++) {
        if(id->equals(declared_variables[i]->id)) {
            return true;
        }
    }
    return false;
}

void push_declaration_stack() {
    declaration_stack.push_back(std::vector<Variable*>(0));
}

//cleans up one layer of the declaration stack
//destructs any non-primitive variables
//adjusts the stack pointer
//do_free = false should only be the case from 
// - return, we can't free the return value
// - after function call returns, function itself is freeing function arguments
void pop_declaration_stack(CompilationContext *ctx, bool do_free) {
    assert(declaration_stack.size() != 0);

    //retrieve layer to be removed
    std::vector<Variable*> top = *(declaration_stack.rbegin());

    //if this is not the function parameter layer, adjust %rsp
    if(declaration_stack.size() > 1) {
        //free heap allocated structs. 
        if(do_free) emit_cleanup_declaration_stack_layer(ctx, declaration_stack.size() - 1);

        //adjust %rsp
        std::vector<std::string> desc_list;
        for(int i = 0; i < top.size(); i++){
            assert(!top[i]->is_global);
            desc_list.push_back(top[i]->id->name);
        }
        emit_add_rsp(top.size() * 8, desc_list);
    }

    //unregister variables as declared
    for(int i = 0; i < top.size(); i++){
        assert(!top[i]->is_global);
        remove_variable(top[i]->id);
    }

    //remove layer
    declaration_stack.pop_back();
}

void dump_stack_desc() {
    for(int i = 0; i < stack_desc.size(); i++){
        std::cout << stack_desc[i] << std::endl;
    }
}


// -- LOOP STATE --
void push_loop_stack(std::string start_label, std::string assignment_label, std::string end_label) {
    assert(declaration_stack.size() != 0);
    int declaration_layer = declaration_stack.size() - 1;
    LoopContext *lc = new LoopContext(start_label, assignment_label, end_label, declaration_layer);
    loop_stack.push_back(lc);
}

void pop_loop_stack(std::string start_label, std::string assignment_label, std::string end_label) {
    assert(loop_stack.size() != 0);
    LoopContext *lc = *(loop_stack.rbegin());
    loop_stack.pop_back();
    assert(lc->start_label == start_label);
    assert(lc->assignment_label == assignment_label);
    assert(lc->end_label == end_label);
}


// -- CONTEXT UTILS --
bool is_type_primitive(Type *t) {
    if(dynamic_cast<BaseType*>(t)) {                        //is it explicitly mentioned?
        for(BaseType *ot : builtin_definition_space->get_basetypes(Visibility::All)) {
            if(t->equals(ot)) {
                return true;
            }
        }
        return false;
    }          
    if(dynamic_cast<PointerType*>(t)) return true;          //is it a pointer?
    if(dynamic_cast<ReferenceType*>(t)) return true;        //is it a reference?
    if(dynamic_cast<FunctionPointerType*>(t)) return true;  //is it a function pointer?
    return false;
}

DefinitionSpace* get_definition_space(std::string filepath, Package* package) {
    assert(package != nullptr);

    // filepath should be absolute
    assert(filepath[0] == '/');
    filepath = normalize_path(filepath);

    if(definition_spaces.contains(filepath)) {
        DefinitionSpace *ds = definition_spaces.at(filepath);
        // assert(ds->get_package() == package);
        if(ds->get_package() != package) {
            std::cout << "FILEPATH : " << filepath << std::endl;
            std::cout << "DS->FILEPATH : " << ds->get_filepath() << std::endl;
            std::cout << "PACKAGE : " << package->name << std::endl;
            std::cout << "DS->PACKAGE : " << ds->get_package()->name << std::endl;
            assert(ds->get_package() == package);
        }   
        return definition_spaces.at(filepath);
    }
    DefinitionSpace *ds = new DefinitionSpace(filepath, package);
    definition_spaces.insert({filepath, ds});
    return ds;
}

DefinitionSpace* get_definition_space(BaseType* t) {
    assert(basetype_to_definition_space.contains(t));
    return basetype_to_definition_space.at(t);
}

DefinitionSpace* get_definition_space(Type *t) {
    //find 'base type'
    Type *bt = nullptr;
    {
        Type *tmp = t;
        while(true) {
            if(dynamic_cast<BaseType*>(tmp)) {
                bt = tmp;
                break;
            }
            else if(dynamic_cast<PointerType*>(tmp)) {
                bt = tmp;
                break;
            }
            else if(dynamic_cast<ReferenceType*>(tmp)) {
                tmp = dynamic_cast<ReferenceType*>(tmp)->type;   
            }
            else if(dynamic_cast<ArrayType*>(tmp)) {
                tmp = dynamic_cast<ArrayType*>(tmp)->type;
            }
            else if(dynamic_cast<TemplatedType*>(tmp)) {
                tmp = dynamic_cast<TemplatedType*>(tmp)->base_type;   
            }
            else if(dynamic_cast<FunctionPointerType*>(tmp)) {
                bt = tmp;
                break;
            }
            else assert(false);
        }
    }
    assert(bt != nullptr);

    //find definition space
    DefinitionSpace *ds = nullptr;
    if(dynamic_cast<BaseType*>(bt)) {
        ds = get_definition_space(dynamic_cast<BaseType*>(bt));
    }
    else if(dynamic_cast<PointerType*>(bt)) {
        ds = builtin_definition_space;
    }
    else if(dynamic_cast<FunctionPointerType*>(bt)) {
        ds = builtin_definition_space;
    }
    else assert(false);
    assert(ds != nullptr);

    return ds;
}

//just checks to make sure that it's composed of declared base types. 
//also, none of the template types can be ReferenceType
bool is_templated_type_well_formed(TemplatedType *t, CompilationContext *ctx) {
    assert(t != nullptr);
    if(!ctx->is_basetype_declared(t->base_type)) {
        std::cout << "is_templated_type_well_formed() : Basetype not declared : " << t->base_type->to_string() << std::endl;
        return false;
    }
    for(int i = 0; i < t->template_types.size(); i++){
        Type *nt = t->template_types[i];
        assert(nt != nullptr);
        while(true) {
            if(dynamic_cast<ReferenceType*>(nt)) {
                std::cout << "is_templated_type_well_formed() : Template type variable cannot be reference type : " << nt->to_string() << std::endl;
                return false;
            }
            else if(auto x = dynamic_cast<PointerType*>(nt)) {
                nt = x->type;
            }
            else if(auto x = dynamic_cast<BaseType*>(nt)) {
                if(!ctx->is_basetype_declared(x)) {
                    std::cout << "is_templated_type_well_formed() : Template type variable not declared : " << x->to_string() << std::endl;
                    std::cout << "Context : " << std::endl;
                    for(CompilationContext::DefinitionSpaceView dsv : ctx->definition_spaces) {
                        std::cout << "    " << dsv.space->get_filepath() << std::endl;
                    }
                    return false;
                }
                break;
            }
            else if(auto x = dynamic_cast<TemplatedType*>(nt)) {
                if(!is_templated_type_well_formed(x, ctx)) return false;
                break;
            }
            else if(auto x = dynamic_cast<ArrayType*>(nt)) {
                nt = x->type;
            }
            else assert(false);
        }
    }
    return true;
}

//look through all declared templated struct defs. If one matches, create it. 
// it should be guaranteed that there is at most one matching templated struct
//returns false if it creates a malformed type, or can't create the type
bool create_templated_type(TemplatedType *t, CompilationContext* ctx) {
    //ensure templated type is well formed (composed of declared basetypes)
    if(!is_templated_type_well_formed(t, ctx)) {
        std::cout << "Templated type not well formed : " << t->to_string() << std::endl;
        return false;
    }

    //ensure template BaseType is visible from this context
    if(!ctx->is_basetype_declared(t->base_type)) {
        std::cout << "Templated BaseType is not accessible from the current context : " << t->base_type->to_string() << std::endl;
        return false;
    }

    //have we already generated this type?
    if(ctx->is_type_declared(t)) return true;

    //find definition space that declares the templated type's basetype
    DefinitionSpace *ds = get_definition_space(t->base_type);
    if(ds == nullptr) {
        std::cout << "Could not find DefinitionSpace for BaseType : " << t->base_type->to_string() << std::endl;
        return false;
    }

    //try to generate it
    TemplatedStructDefinition *tsd = get_templated_struct_definition(t->base_type);
    if(tsd == nullptr) {
        std::cout << "Failed to get templated struct definition for : " << t->base_type->to_string() << std::endl;
        assert(false);
        return false;
    }
    StructDefinition *sd = tsd->gen_struct_def(ctx, t);
    if(sd == nullptr) {
        std::cout << "Failed to generate templated struct instantiation : " << t->to_string() << std::endl;
        return false;
    }
    assert(sd->type->equals(t));

    //generate compilation context for this template instantiation
    TemplateMapping *tm = tsd->calc_mapping(t);
    assert(tm != nullptr);
    std::vector<BaseType*> basetypes;
    tm->find_all_basetypes(basetypes);
    CompilationContext *template_ctx = ds->create_compilation_context();
    assert(template_ctx != nullptr);
    for(BaseType *bt : basetypes) {
        DefinitionSpace *ds = get_definition_space(bt);
        assert(ds != nullptr);
        template_ctx->add_definition_space(ds, Visibility::Public);
    }

    //add this type as declared
    // TODO figure out what visibility this should be via templated struct def
    if(!add_struct(ds, sd, Visibility::Public, template_ctx)) {
        std::cout << "Unable to add templated struct instantiation : " << t->to_string() << std::endl;
        return false;
    }

    return true;
}

bool create_array_type(ArrayType *t, CompilationContext *ctx) {
    //ensure array base type is declared
    if(!ctx->is_type_declared(t->type)) {
        std::cout << "Array base type not declared : " << t->type->to_string() << std::endl;
        return false;
    }
    
    //is amt <= 0?
    if(t->amt <= 0) {
        std::cout << "Array type must have amount > 0 : " << t->to_string() << std::endl;
        return false;
    }

    //have we already generated this type?
    if(ctx->is_type_declared(t)) return true;

    //find definition space to add this type to
    DefinitionSpace *ds = get_definition_space(t);
    assert(ds != nullptr);

    //generate relevant methods to type
    //  - default constructor
    //  - copy constructor
    //  - destructor
    // TODO if the base type is BaseType, need to figure out visibility level from templated struct def
    Visibility vis = Visibility::Public;
    if(!ds->add_type(t, vis, Origin::Generated)) {
        std::cout << "adding arraytype failed : " << t->to_string() << "\n";
        assert(false);
        return false;
    }

    Constructor *default_constructor = new StructConstructor(t, {}, new CompoundStatement(std::vector<Statement*>{}));

    Identifier *xid = new Identifier("x");
    Identifier *thisid = new Identifier("this");
    Identifier *iid = new Identifier("i");
    Constructor *copy_constructor = new StructConstructor(
        t,
        {new Parameter(new ReferenceType(t->make_copy()), xid->make_copy())},
        new CompoundStatement({
            new ForStatement(
                new Declaration(primitives::i32->make_copy(), iid->make_copy(), new Expression(new ExprPrimary(new IntegerLiteral(0)))),
                new Expression(new ExprBinary(new ExprPrimary(iid->make_copy()), "<", new ExprPrimary(new IntegerLiteral(t->amt)))),
                new Expression(new ExprPostfix(new ExprPrimary(iid->make_copy()), "++")),
                new CompoundStatement({
                    new ExpressionStatement(new Expression(
                        new ExprBinary(
                            new ExprPostfix(new ExprPrimary(thisid), new Expression(new ExprPrimary(iid->make_copy()))),
                            "=",
                            new ExprPostfix(new ExprPrimary(xid), new Expression(new ExprPrimary(iid->make_copy())))
                        )
                    ))
                })
            )
        })
    );

    Destructor *destructor = new Destructor(t, new CompoundStatement(std::vector<Statement*>{}));

    // construct compilation context for array type
    CompilationContext *array_ctx = ds->create_compilation_context();
    assert(array_ctx != nullptr);

    if(!add_constructor(ds, default_constructor, Visibility::Public, array_ctx)) {
        std::cout << "Failed to add array default constructor : " << t->to_string() << std::endl;
        return false;
    }
    if(!add_constructor(ds, copy_constructor, Visibility::Public, array_ctx)) {
        std::cout << "Failed to add array copy constructor : " << t->to_string() << std::endl;
        return false;
    }
    if(!add_destructor(ds, destructor, Visibility::Public, array_ctx)) {
        std::cout << "Failed to add array destructor : " << t->to_string() << std::endl;
        return false;
    }

    return true;
}

bool add_templated_function(DefinitionSpace* ds, TemplatedFunction* x, Visibility vis, CompilationContext* ctx) {
    if(!ds->add_templated_function(x, vis, Origin::Generated)) {
        std::cout << "Failed to add templated function : " << x->function->resolve_function_signature()->to_string() << std::endl;
        return false;
    }

    if(!x->is_well_formed(ctx)) {
        std::cout << "Templated function not well formed : " << x->function->resolve_function_signature()->to_string() << std::endl;
        return false;
    }

    return true;
}

bool add_function(DefinitionSpace* ds, Function* x, Visibility vis, CompilationContext* ctx) {
    x->is_generated = true;
    if(!ds->add_function(x, vis, Origin::Generated)) {
        std::cout << "Failed to add function : " << x->resolve_function_signature()->to_string() << std::endl;
        return false;
    }

    if(!x->look_for_templates(ctx)) {
        std::cout << "Failed to resolve templates in function : " << x->resolve_function_signature()->to_string() << std::endl;
        return false;
    }

    work_queue.push({x, ctx});
    return true;
}

bool add_operator(DefinitionSpace* ds, Operator* x, Visibility vis, CompilationContext* ctx) {
    x->is_generated = true;
    if(!ds->add_operator(x, vis, Origin::Generated)) {
        std::cout << "Failed to add operator : " << x->resolve_operator_signature()->to_string() << std::endl;
        return false;
    }

    if(!x->look_for_templates(ctx)) {
        std::cout << "Failed to resolve templates in operator : " << x->resolve_operator_signature()->to_string() << std::endl;
        return false;
    }

    if(dynamic_cast<OperatorOverload*>(x)) {
        OperatorOverload *oo = dynamic_cast<OperatorOverload*>(x);
        work_queue.push({oo, ctx});
    }
    return true;
}

bool add_struct(DefinitionSpace *ds, StructDefinition *x, Visibility vis, CompilationContext *ctx) {
    //add struct to definition space
    Type *t = x->type;
    if(!ds->add_struct(x, vis, Origin::Generated)) {
        std::cout << "Failed to add struct : " << x->type->to_string() << std::endl;
        return false;
    }

    //add all functions, constructors, destructors
    for(int i = 0; i < x->functions.size(); i++){
        TemplatedFunction *f = x->functions[i];
        if(!add_templated_function(ds, f, vis, ctx)) {
            std::cout << "Failed to add struct member function : " << f->function->resolve_function_signature()->to_string() << "\n";
            return false;
        } 
    }
    for(int i = 0; i < x->constructors.size(); i++) {
        Constructor *c = x->constructors[i];
        if(!add_constructor(ds, c, vis, ctx)) {
            std::cout << "Failed to add struct constructor : " << c->resolve_constructor_signature()->to_string() << "\n";
            return false;
        }
    }
    for(int i = 0; i < x->destructors.size(); i++){
        Destructor *d = x->destructors[i];
        if(!add_destructor(ds, d, vis, ctx)) {
            std::cout << "Failed to add destructor : " << t->to_string() << "\n";
            return false;
        }
    }

    //default default constructor
    add_constructor(
        ds,
        new StructConstructor(
            t->make_copy(), 
            {}, 
            new CompoundStatement(std::vector<Statement*>{})
        ),
        vis,
        ctx
    );

    //default copy constructor
    // - for each field, just does default copy constructor
    {
        Identifier *oid = new Identifier("other");
        Identifier *tid = new Identifier("this");

        std::vector<Statement*> statements;
        for(int i = 0; i < x->member_variables.size(); i++){
            Identifier *mvi = x->member_variables[i]->id;
            statements.push_back(
                new ExpressionStatement(new Expression(new ExprBinary(
                    new ExprPostfix(
                        new ExprPrimary(tid),
                        std::make_pair(".", mvi->make_copy())
                    ),
                    "=",
                    new ExprPostfix(
                        new ExprPrimary(oid),
                        std::make_pair(".", mvi->make_copy())
                    )
                )))
            );
        }

        CompoundStatement *body = new CompoundStatement(statements);
        add_constructor(
            ds,
            new StructConstructor(
                t->make_copy(), 
                {new Parameter(new ReferenceType(t->make_copy()), oid->make_copy())}, 
                body
            ),
            vis,
            ctx
        );
    }

    //default destructor
    add_destructor(
        ds,
        new Destructor(
            t->make_copy(), 
            new CompoundStatement(std::vector<Statement*>{})
        ),
        vis,
        ctx
    );

    // resolve templates in struct definition
    if(!x->look_for_templates(ctx)) {
        std::cout << "Unable to resolve all templates in " << t->to_string() << std::endl;
        return false;
    }

    // check if struct is well formed
    if(!x->is_well_formed(ctx)) {
        std::cout << "Struct is not well formed : " << t->to_string() << std::endl;
        return false;
    }

    return true;
}

bool add_constructor(DefinitionSpace* ds, Constructor* x, Visibility vis, CompilationContext* ctx) {
    x->is_generated = true;
    if(!ds->add_constructor(x, vis, Origin::Generated)) {
        return false;
    }

    if(!x->look_for_templates(ctx)) {
        std::cout << "Failed to resolve templates in constructor : " << x->resolve_constructor_signature()->to_string() << std::endl;
        return false;
    }

    work_queue.push({x, ctx});
    return true;
}

bool add_destructor(DefinitionSpace* ds, Destructor* x, Visibility vis, CompilationContext* ctx) {
    x->is_generated = true;
    if(!ds->add_destructor(x, vis, Origin::Generated)) {
        return false;
    }

    if(!x->look_for_templates(ctx)) {
        std::cout << "Failed to resolve templates in destructor : " << x->type->to_string() << std::endl;
        return false;
    }

    work_queue.push({x, ctx});
    return true;
}


// -- GLOBAL UTILS --
StructDefinition* get_struct_definition(Type *t) {
    for(StructDefinition *sd : struct_definitions) {
        if(t->equals(sd->type)) {
            return sd;
        }
    }
    return nullptr;
}

TemplatedStructDefinition* get_templated_struct_definition(BaseType *t) {
    DefinitionSpace* ds = get_definition_space(t);
    for(TemplatedStructDefinition *tsd : ds->get_templated_structs(Visibility::All)) {
        if(t->equals(tsd->struct_def->type)) {
            return tsd;
        }
    }
    return nullptr;
}

//just tries to retrieve the struct layout of the given type
//doesn't try to generate it if missing
StructLayout* _get_struct_layout(Type *t) {
    assert(t != nullptr);
    for(int i = 0; i < struct_layouts.size(); i++){
        if(struct_layouts[i].first->equals(t)) {
            return struct_layouts[i].second;
        }
    }
    return nullptr;
}

//tries to construct the struct layout for the given type. 
// - first tries to see if it's already generated
// - otherwise generates it and adds it to the global list
// - dynamically parses includes to find missing struct definitions. 
//   - more specifically, when we're constructing the struct layout of some type T, 
//     it may depend on some or all of the imports of the file defining T. 
//   - Before trying to see if member variable types are defined, first just make sure to
//     parse the imports of the file defining the struct. 
bool _construct_struct_layout(Type *t, std::vector<Type*> type_stack, int& byte_off) {
    assert(t != nullptr);

    // - have we gone too deep?
    int struct_depth_limit = 32;
    if(type_stack.size() > struct_depth_limit) {
        std::cout << "Struct member nesting too deep (depth > " << struct_depth_limit << ")\n";
        return false;
    }

    // - is this initialization infinite recursive?
    // this check really isn't necessary, but it's nice to have some extra info
    for(int i = 0; i < type_stack.size(); i++){
        if(t->equals(type_stack[i])) {
            std::cout << "Infinite recursive struct initialization : " << t->to_string() << " contains itself\n";
            return false;
        }
    }
    
    // - handle primitives seperately
    if(is_type_primitive(t)) {
        byte_off += t->calc_size();
        return true;
    }

    // - see if we've already generated this struct layout
    if(auto *sl = _get_struct_layout(t)) {
        byte_off += sl->get_size();
        return true;
    }

    // - handle arrays seperately
    // ArrayTypes don't generate a StructDefinition 
    if(auto atype = dynamic_cast<ArrayType*>(t)) {
        StructLayout *nsl = nullptr;
        if(is_type_primitive(atype->type)) {
            //directly generate struct layout
            nsl = new StructLayout({}, {}, atype->type->calc_size() * atype->amt);
        }
        else {
            //generate struct layout for underlying type 
            StructLayout *sl = get_struct_layout(atype->type);
            if(sl != nullptr) nsl = new StructLayout({}, {}, atype->type->calc_size() * atype->amt);
        }

        if(nsl == nullptr) {
            std::cout << "Failed to generate struct layout for ArrayType : " << t->to_string() << std::endl;
            return false;
        }

        struct_layouts.push_back({t, nsl});
        byte_off += nsl->get_size();
        return true;
    }

    // - ok, generate the struct layout 
    //ensure the definition space defining t has all of its imports resolved
    DefinitionSpace *ds = get_definition_space(t);
    assert(ds != nullptr);
    if(!ds->ensure_ready()) {
        return false;
    }

    //get struct definition
    StructDefinition *sd = get_struct_definition(t);
    if(sd == nullptr) {
        std::cout << "Could not find struct definition : " << t->to_string() << std::endl;
        assert(false);
    }

    type_stack.push_back(t);

    std::vector<MemberVariable*> member_variables;
    std::vector<std::pair<Identifier*, int>> offset_map;
    int size = 0;
    int old_byte_off = byte_off;

    for(int i = 0; i < sd->member_variables.size(); i++){
        MemberVariable *mv = sd->member_variables[i];
        assert(mv != nullptr);

        // - we should be able to create struct layout for all member variables
        if(!_construct_struct_layout(mv->type, type_stack, byte_off)) {
            return false;
        }

        member_variables.push_back(mv->make_copy());
        offset_map.push_back(std::make_pair(mv->id->make_copy(), size));
        size += byte_off - old_byte_off;
        old_byte_off = byte_off;
    }

    type_stack.pop_back();

    // - add struct layout to global list
    StructLayout *sl = new StructLayout(member_variables, offset_map, size);
    struct_layouts.push_back({t, sl});

    if(debug) {
        std::cout << "STRUCT LAYOUT : " << t->to_string() << " : " << size << "\n";
        for(int i = 0; i < offset_map.size(); i++){
            std::cout << offset_map[i].first->name << " " << offset_map[i].second << "\n";
        }
    }

    return true;
}

//wrapper around _construct_struct_layout
bool construct_struct_layout(Type *t) {
    std::vector<Type*> type_stack;
    int byte_off = 0;
    return _construct_struct_layout(t, type_stack, byte_off);
}

StructLayout* get_struct_layout(Type *t) {
    assert(!is_type_primitive(t));

    //check if it's already generated
    StructLayout *sl = _get_struct_layout(t);
    if(sl) return sl;

    //try to construct one
    if(construct_struct_layout(t)) {
        return _get_struct_layout(t);
    }

    return nullptr;
}

std::string get_function_label(FunctionSignature *fs) {
    assert(fs != nullptr);
    if(!function_label_map.count(fs)) {
        std::cout << "Unable to find label for : " << fs->to_string() << "\n";
        assert(false);
    }
    return function_label_map[fs];
}

std::string get_constructor_label(ConstructorSignature *cs) {
    assert(cs != nullptr);
    assert(constructor_label_map.count(cs));
    return constructor_label_map[cs];
}

std::string get_destructor_label(Type *t) {
    assert(t != nullptr);
    assert(destructor_label_map.count(t));
    return destructor_label_map[t];
}

std::string get_operator_label(OperatorSignature *os) {
    assert(os != nullptr);
    assert(operator_label_map.count(os));
    return operator_label_map[os];
}

bool add_string_literal(std::string str) {
    if(string_literal_label_map.count(str)) return true;
    string_literal_label_map[str] = create_new_label();
    return true;
}

std::string get_string_literal_label(std::string str) {
    assert(string_literal_label_map.count(str));
    return string_literal_label_map[str];
}


// -- PACKAGES --

// TODO make sure that the package path exists and doesn't completely contain some other package. 
// maybe that should be the job of the package manager?
// but I should still check that here and fail to add if it's the case. 
bool add_package(std::string name, std::string path) {
    for(Package* p : packages) {
        if(p->name == name) {
            std::cout << "Duplicate package : " << name << std::endl;
            return false;
        }
    }
    Package* p = new Package(name, path);
    packages.push_back(p);
    return true;
}

Package* get_package(std::string name) {
    for(Package* p : packages) {
        if(p->name == name) {
            return p;
        }
    }
    std::cout << "Failed to find package with name : " << name << std::endl;
    return nullptr;
}

// package named 'name' is dependent on package named 'dep_name'
// ensures that a package doesn't depend on itself
// ensures that we don't add duplicate dependencies
// ensures that we don't add two dependencies with the same alias
// doesn't care about circular dependencies between packages
//   that's the build tool's job
bool add_package_dependency(Package* package, std::string alias, Package* dep) {
    assert(package != nullptr);
    assert(dep != nullptr);

    // see if the package is depending on itself
    if(package == dep) {
        return false;
    }
    assert(package->name != dep->name);
     
    // see if we've already added this dependency
    // see if another dependency has the same alias
    for(auto &[_alias, _dep] : package->dependencies) {
        if(_dep->name == dep->name) {
            std::cout << "Package \"" << package->name << "\" already has dependency \"" << dep->name << "\"" << std::endl;
            return false;
        }
        if(_alias == alias) {
            std::cout << "Package \"" << package->name << "\" already has dependency with alias \"" << alias << "\"" << std::endl;
            return false;
        }
    }

    // add the dependency
    package->dependencies.push_back({alias, dep});
    return true;
}

// it probably should be the build tool's responsibility to make sure that the include path is sane. 
// the compiler will just spit out an error if it can't resolve it. 
bool add_package_default_include(Package* package, std::string alias, std::string include_path) {
    assert(package != nullptr);

    // ensure a dependency with the given alias exists
    bool found = false;
    for(auto &[_alias, _dep] : package->dependencies) {
        if(_alias == alias) {
            assert(!found);
            found = true;
        }
    }
    if(!found) {
        std::cout << "Package \"" << package->name << "\" does not have dependency with alias \"" << alias << "\"" << std::endl;
        return false;
    }

    // add the default include
    package->default_includes.push_back({alias, include_path});
    return true;
}

// this should only work if current_package is nullptr
// should only be able to set it once. 
bool set_current_package(Package* package) {
    if(current_package != nullptr) {
        return false;
    }
    current_package = package;
    return true;
}


// -- CONTROLLER --
void initialize_controller() {
    //construct builtin definition space
    {
        builtin_definition_space = new DefinitionSpace();

        // add primitive related declarations
        primitives::init_primitives();

        // add 'u8** environ'
        // have to manually emit an 'environ' label in startup code
        GlobalDeclaration *environ_gd = new GlobalDeclaration(
            std::optional<Identifier*>(new Identifier("__GLOBAL_FIRST__")), 
            true,       // is_extern
            new Declaration(
                new PointerType(new PointerType(primitives::u8->make_copy())),
                new Identifier("environ"),
                std::nullopt
            )
        );
        builtin_definition_space->add_global_variable(environ_gd, Visibility::Public, Origin::Source);

        // add '__GLOBAL_FIRST__' global node
        GlobalNode *global_first_gn = new GlobalNode(new Identifier("__GLOBAL_FIRST__"), {});
        builtin_definition_space->add_global_node(global_first_gn);

        builtin_definition_space->ensure_ready();
    }
}

bool compile(std::string target_filepath, Package* target_package) {
    assert(target_filepath.size() != 0);
    std::cout << " -- COMPILING FILE : " << target_filepath << std::endl;

    // place target file concrete instantiations into work queue
    // don't place generated instantiations into queue,
    //   if we just compiled another file, there could still be generated instantiations. 
    {
        DefinitionSpace* ds = get_definition_space(target_filepath, target_package);
        ds->ensure_ready();

        assert(ds != nullptr);
        CompilationContext* ctx = ds->create_compilation_context();
        if(ctx == nullptr) {
            std::cout << "Failed to create compilation context for target file" << std::endl;
            return false;
        }

        for(Function *f : ds->get_functions(Visibility::All, Origin::Source)) {
            assert(!f->is_generated);
            work_queue.push({f, ctx});
        }
        for(Operator *o : ds->get_operators(Visibility::All, Origin::Source)) {
            assert(!o->is_generated);

            // we only want to process overloads
            if(dynamic_cast<OperatorOverload*>(o) == nullptr) continue;
            OperatorOverload *oo = dynamic_cast<OperatorOverload*>(o);
            assert(oo != nullptr);
            work_queue.push({oo, ctx});
        }
        for(Constructor *c : ds->get_constructors(Visibility::All, Origin::Source)) {
            assert(!c->is_generated);
            work_queue.push({c, ctx});
        }
        for(Destructor *d : ds->get_destructors(Visibility::All, Origin::Source)) {
            assert(!d->is_generated);
            work_queue.push({d, ctx});
        }
        for(GlobalDeclaration *gd : ds->get_global_variables(Visibility::All)) {
            work_queue.push({gd, ctx});
        }
    }

    // process all items in queue
    while(work_queue.size() != 0) {
        ASTNode* node = work_queue.front().first;
        CompilationContext* ctx = work_queue.front().second;
        work_queue.pop();

        if(declared_variables.size() != 0) {
            std::cout << "BEFORE QUEUE ITEM : oops, variables still declared : " << std::endl;
            for(Variable *v : declared_variables) {
                Type *t = v->type;
                Identifier *id = v->id;
                std::cout << t->to_string() << " " << id->to_string() << ";" << std::endl;
            }
            assert(false);
        }

        assert(declaration_stack.size() == 0);
        assert(declared_variables.size() == 0);
        assert(declared_global_variables.size() == 0);

        // ensure context is ready
        if(!ctx->ensure_ready()) {
            std::cout << "Failed to initialize compilation context" << std::endl;
            return false;
        }

        //add all globals visible to the current context
        // TODO replace this with custom iterator
        for(CompilationContext::DefinitionSpaceView dsv : ctx->definition_spaces) {
            DefinitionSpace *ds = dsv.space;
            Visibility vis = dsv.vis;
            for(GlobalDeclaration *gd : ds->get_global_variables(vis)) {
                Type *type = gd->declaration->type;
                Identifier *id = gd->declaration->id;
                bool is_extern = gd->is_extern;
                add_global_variable(type, id, is_extern);
            }
        }

        if(dynamic_cast<Function*>(node) != nullptr) {
            Function *f = dynamic_cast<Function*>(node);
            assert(f != nullptr);
            enclosing_return_type = f->type->make_copy();
            enclosing_type = f->enclosing_type;

            std::cout << "CHECKING FUNCTION : " << f->resolve_function_signature()->to_string() << std::endl;
            if(!f->is_well_formed(ctx)) {
                std::cout << "Function not well formed : " << f->resolve_function_signature()->to_string() << std::endl;
                return false;
            }
        }
        else if(dynamic_cast<OperatorOverload*>(node) != nullptr) {
            OperatorOverload *oo = dynamic_cast<OperatorOverload*>(node);
            assert(oo != nullptr);
            enclosing_return_type = oo->type->make_copy();
            enclosing_type = std::nullopt;

            assert(oo->resolve_operator_signature() != nullptr);
            std::cout << "CHECKING OVERLOAD : " << oo->resolve_operator_signature()->to_string() << "\n";
            if(!oo->is_well_formed(ctx)) {
                std::cout << "OperatorOverload not well formed : " << oo->resolve_operator_signature()->to_string() << std::endl;
                return false;
            }
        }
        else if(dynamic_cast<Constructor*>(node) != nullptr) {
            Constructor *c = dynamic_cast<Constructor*>(node);
            assert(c != nullptr);
            enclosing_return_type = primitives::_void->make_copy();
            enclosing_type = c->type->make_copy();

            std::cout << "CHECKING CONSTRUCTOR : " << c->resolve_constructor_signature()->to_string() << std::endl;
            if(!c->is_well_formed(ctx)) {
                std::cout << "Constructor not well formed : " << c->resolve_constructor_signature()->to_string() << std::endl;
                return false;
            }
        }
        else if(dynamic_cast<Destructor*>(node) != nullptr) {
            Destructor *d = dynamic_cast<Destructor*>(node);
            assert(d != nullptr);
            enclosing_return_type = primitives::_void->make_copy();
            enclosing_type = d->type->make_copy();

            std::cout << "CHECKING DESTRUCTOR : ~" << d->type->to_string() << "()" << std::endl;
            if(!d->is_well_formed(ctx)) {
                std::cout << "Destructor not well formed : ~" << d->type->to_string() << "()" << std::endl;
                return false;
            }
        }
        else if(dynamic_cast<GlobalDeclaration*>(node) != nullptr) {
            // emit init and cleanup function labels for this global variable
            GlobalDeclaration *gd = dynamic_cast<GlobalDeclaration*>(node);
            assert(gd != nullptr);
            Type *t = gd->declaration->type;
            Identifier *id = gd->declaration->id;
            bool is_extern = gd->is_extern;
            std::optional<Expression*> expr = gd->declaration->expr;

            std::cout << "CHECKING GLOBAL VARIABLE : " << t->to_string() << " " << id->to_string() << ";" << std::endl;

            // address is just label + %rip relative addressing
            std::string addr_str = id->to_string() + "(%rip)";  

            // just have these set for completeness
            enclosing_return_type = primitives::_void->make_copy();
            enclosing_type = std::nullopt;

            // resolve templates 
            if(!gd->look_for_templates(ctx)) {
                return false;
            }

            // init
            {
                std::string label = gd->generate_init_label();
                fout << ".section \".text." << label << "\",\"ax\",@progbits\n";
                fout << ".globl " << label << "\n";
                fout << label << ":\n";

                //setup function stack frame
                fout << indent() << "push %rbp\n";  //should not be managed by local_offset
                fout << indent() << "mov %rsp, %rbp\n";

                push_declaration_stack();

                //set local offset equal to %rsp
                local_offset = 0;

                //check that the local stack is empty so far
                assert(stack_desc.size() == 0);

                //evaluate global variable initialization
                if(!emit_initialize_variable(ctx, t, id, expr, addr_str, true, is_extern)) {
                    return false;
                }

                //add return
                ReturnStatement *rs = new ReturnStatement(std::nullopt);
                if(!rs->is_well_formed(ctx)) {
                    std::cout << "Trailing return failed??";
                    assert(0);  
                }

                fout << "\n";

                //cleanup declaration stack
                pop_declaration_stack(ctx);

                //local stack should be empty
                assert(stack_desc.size() == 0);
            }

            // cleanup
            {
                std::string label = gd->generate_cleanup_label();
                fout << ".section \".text." << label << "\",\"ax\",@progbits\n";
                fout << ".globl " << label << "\n";
                fout << label << ":\n";

                //setup function stack frame
                fout << indent() << "push %rbp\n";  //should not be managed by local_offset
                fout << indent() << "mov %rsp, %rbp\n";

                push_declaration_stack();

                //set local offset equal to %rsp
                local_offset = 0;

                //check that the local stack is empty so far
                assert(stack_desc.size() == 0);

                //evaluate global variable cleanup
                if(!is_type_primitive(t)) {
                    //move global variable address into %rax
                    fout << indent() << "movq " << addr_str << ", %rax\n";

                    //emit destructor call
                    emit_destructor_call(ctx, t);
                }

                //add return
                ReturnStatement *rs = new ReturnStatement(std::nullopt);
                if(!rs->is_well_formed(ctx)) {
                    std::cout << "Trailing return failed??";
                    assert(0);  
                }

                fout << "\n";

                //cleanup declaration stack
                pop_declaration_stack(ctx);

                //local stack should be empty
                assert(stack_desc.size() == 0);
            }
        }
        else { 
            std::cout << "Unrecognized ASTNode type in work queue" << std::endl;
            assert(false);
        }

        //remove all global variables 
        remove_all_global_variables();
        
        if(declared_variables.size() != 0) {
            std::cout << "AFTER QUEUE ITEM : oops, variables still declared : " << std::endl;
            for(Variable *v : declared_variables) {
                Type *t = v->type;
                Identifier *id = v->id;
                std::cout << t->to_string() << " " << id->to_string() << ";" << std::endl;
            }
            assert(false);
        }

        assert(declaration_stack.size() == 0);
        assert(declared_variables.size() == 0);
    }

    // emit global storage
    fout << ".section .data\n";
    {
        DefinitionSpace *ds = get_definition_space(target_filepath, target_package);
        assert(ds != nullptr);
        for(GlobalDeclaration *gd : ds->get_global_variables(Visibility::All)) {
            Type *t = gd->declaration->type;
            Identifier *id = gd->declaration->id;
            std::string label = id->to_string();
            if(!gd->is_extern) {
                fout << ".globl " << label << "\n";
                fout << label << ": .quad 0";
                if(asm_debug) fout << " # " << t->to_string() << " " << id->to_string();
                fout << "\n";
            }
        }
    }
    fout << "\n";

    // emit string literal storage
    fout << ".section .rodata\n";
    for(auto i = string_literal_label_map.begin(); i != string_literal_label_map.end(); i++) {
        std::string val = i->first;
        std::string label = i->second;
        fout << label << ": .string \"" << val << "\"\n";
    }
    fout << "\n";

    // cleanup
    string_literal_label_map.clear();

    return true;
}

bool emit_driver(std::string target_filepath, Package* target_package) {
    std::cout << " -- EMIT DRIVER CODE : " << target_filepath << std::endl;

    DefinitionSpace *target_ds = get_definition_space(target_filepath, target_package);
    target_ds->ensure_ready();

    // find entry point
    // there should be exactly one function that can be a valid entry point in the target file
    Function* main_func = nullptr;
    for(Function* f : target_ds->get_functions(Visibility::All, Origin::Source)) {
        if(f->is_main()) {
            if(main_func != nullptr) {
                std::cout << "Found two valid entry points" << std::endl;
                return false;
            }
            main_func = f;
        }
    }
    if(main_func == nullptr) {
        std::cout << "Didn't find entry point" << std::endl;
        return false;
    }

    // find all reachable definition spaces
    std::vector<DefinitionSpace*> all_definition_spaces;
    all_definition_spaces.push_back(builtin_definition_space);
    std::function<void(DefinitionSpace*)> find_definition_spaces = 
    [&all_definition_spaces, &find_definition_spaces](DefinitionSpace *ds) -> void {
        for(int i = 0; i < all_definition_spaces.size(); i++) {
            if(all_definition_spaces[i] == ds) return;
        }

        all_definition_spaces.push_back(ds);
        for(DefinitionSpace* nds : ds->get_included_definition_spaces()) {
            find_definition_spaces(nds);
        }
    };
    find_definition_spaces(target_ds);

    // find all global variables / global variable nodes
    std::vector<GlobalNode*> all_global_nodes;
    std::vector<GlobalDeclaration*> all_global_variables;
    for(DefinitionSpace *ds : all_definition_spaces) {
        for(GlobalNode *gn : ds->get_global_nodes()) {
            all_global_nodes.push_back(gn);
        }
        for(GlobalDeclaration *gd : ds->get_global_variables(Visibility::All)) {
            all_global_variables.push_back(gd);
        }
    }

    // make sure there are no duplicate global nodes
    for(int i = 0; i < all_global_nodes.size(); i++) {
        for(int j = i + 1; j < all_global_nodes.size(); j++) {
            GlobalNode *gna = all_global_nodes[i];
            GlobalNode *gnb = all_global_nodes[j];
            if(gna->id->equals(gnb->id)) {
                std::cout << "Duplicate global node : " << gna->id->to_string() << std::endl;
                return false;
            }
        }
    }

    // make sure all global node dependencies are sane
    for(int i = 0; i < all_global_nodes.size(); i++) {
        GlobalNode *gn = all_global_nodes[i];

        // make sure all dependencies exist
        for(Identifier *dep : gn->dependencies) {
            bool found = false;
            for(int j = 0; j < all_global_nodes.size(); j++) {
                if(dep->equals(all_global_nodes[j]->id)) {
                    found = true;
                }
            }
            if(!found) {
                std::cout << "Global node has dependency that does not exist : " << dep->to_string() << std::endl;
                return false;
            }
        }
        
        // make sure there are no duplicate dependencies
        for(int ii = 0; ii < gn->dependencies.size(); ii++) {
            for(int jj = ii + 1; jj < gn->dependencies.size(); jj++) {
                Identifier *ida = gn->dependencies[ii];
                Identifier *idb = gn->dependencies[jj];
                if(ida->equals(idb)) {
                    std::cout << "Global node has duplicate dependency : " << ida->to_string() << std::endl;
                    return false;
                }
            }
        }

        // make sure it doesn't depend on itself
        for(Identifier *dep : gn->dependencies) {
            if(dep->equals(gn->id)) {
                std::cout << "Global node depends on itself : " << gn->id->to_string() << std::endl;
            }
        }
    }

    // make sure all global declarations correspond to an existing global node
    for(GlobalDeclaration *gd : all_global_variables) {
        if(!gd->node_id.has_value()) {
            continue;
        }
        Identifier *node_id = gd->node_id.value();
        assert(node_id != nullptr);

        bool found = false;
        for(GlobalNode *gn : all_global_nodes) {
            if(node_id->equals(gn->id)) {
                found = true;
            }
        }
        if(!found) {
            std::cout << "Global declaration node id doesn't exist : " << node_id->to_string() << std::endl;
            return false;
        }
    }

    // determine global variable initialization order
    {  
        int n = all_global_nodes.size();

        // map node identifiers to indices
        std::map<std::string, int> id_map;
        for(int i = 0; i < n; i++){
            GlobalNode *gn = all_global_nodes[i];
            id_map[gn->id->name] = i;
        }

        // do topological sort
        std::vector<int> node_order;
        {   
            // create directed graph
            //  edge from A to B means that A needs to go before B
            std::vector<std::vector<int>> c(n);
            for(int i = 0; i < n; i++){
                GlobalNode *gn = all_global_nodes[i];
                for(int j = 0; j < gn->dependencies.size(); j++){
                    std::string dep = gn->dependencies[j]->to_string();
                    c[id_map[dep]].push_back(i);
                }                
            }

            // '__GLOBAL_FIRST__' should go before every other node
            for(int i = 0; i < n; i++){
                if(i == id_map["__GLOBAL_FIRST__"]) continue;
                c[id_map["__GLOBAL_FIRST__"]].push_back(i);
            }

            // sort
            std::vector<int> indeg(n, 0);
            for(int i = 0; i < n; i++){
                for(int x : c[i]) indeg[x] ++;
            }
            std::queue<int> q;
            for(int i = 0; i < n; i++){
                if(indeg[i] == 0) q.push(i);
            }
            node_order = {};
            while(q.size() != 0){
                int cur = q.front();
                q.pop();
                node_order.push_back(cur);
                for(int x : c[cur]) {
                    indeg[x] --;
                    if(indeg[x] == 0) q.push(x);
                }
            }
        }
        if(node_order.size() != n) {
            std::cout << "Failed to find topological ordering of global nodes\n";
            return false;
        }
        std::map<std::string, int> order_map;
        std::cout << "Global node initialization order:\n";
        for(int i = 0; i < n; i++){
            int ind = node_order[i];
            order_map[all_global_nodes[ind]->id->name] = i;
            std::cout << all_global_nodes[ind]->id->name << "\n";
        }

        // sort global variables by initialization order
        std::sort(
            all_global_variables.begin(), 
            all_global_variables.end(), 
            [&order_map, &all_global_nodes](GlobalDeclaration *a, GlobalDeclaration *b) -> bool {
                int aorder = a->node_id.has_value()? order_map[a->node_id.value()->name] : all_global_nodes.size();
                int border = b->node_id.has_value()? order_map[b->node_id.value()->name] : all_global_nodes.size();
                return aorder < border;
            }
        );
    }

    // emit storage for 'u8** environ'
    fout << ".section .data\n";
    fout << ".globl environ\n";
    fout << "environ: .quad 0\n";
    fout << "\n";

    // emit _start label
    fout << ".section .text\n";
    fout << ".globl _start\n";
    fout << "_start:\n";

    // parse argc, argv, envp, auxv, reserve stack slot for exit status
    // result should be stack with
    //   u64 argc
    //   u8** argv <-- %rsp
    if(!kernel_mode) {          
        // OS should initialize stack with this state:
        // https://refspecs.linuxfoundation.org/ELF/zSeries/lzsabi0_zSeries/x895.html#AUXSTRUCT
        //         [environment strings...]
        //         [argument strings...]
        //         AT_NULL                          <- AT_NULL = {0, 0}
        //         auxv[0] ... auxv[n-1]            <- {u64 type, void* ptr}
        //         NULL
        //         envp[0] ... envp[n-1]
        //         NULL                           
        //         argv[0] ... argv[argc-1]         <- array of pointers (on the stack)
        // %rsp -> argc
        
        fout << indent() << "movq 0(%rsp), %r8\n";          //argc in %r8
        fout << indent() << "lea 8(%rsp), %r9\n";           //argv in %r9

        //deal with envp
        //put it inside global variable 'u8** environ'
        fout << indent() << "lea 8(%r9, %r8, 8), %r10\n";     //envp in %r10
        fout << indent() << "movq %r10, environ(%rip)\n";

        //TODO deal with auxv

        //reserve stack slot for exit status
        fout << indent() << "sub $8, %rsp\n";

        //save argc, argv to stack
        fout << indent() << "pushq %r8\n";
        fout << indent() << "pushq %r9\n";

        // setup initial stack frame
        fout << indent() << "mov %rsp, %rbp\n";
    }

    // emit global variable init
    for(GlobalDeclaration *gd : all_global_variables) {
        if(gd->is_extern) continue;
        std::string label = gd->generate_init_label();
        fout << indent() << "call " << label << "\n";
    }

    // emit call to entry point
    {
        //emit call
        std::string label = get_function_label(main_func->resolve_function_signature());
        fout << indent() << "call " << label << "\n";

        //%rax should contain exit status, save it to reserved slot
        fout << indent() << "movq %rax, 16(%rbp)\n";
    }

    // emit global variable cleanup
    reverse(all_global_variables.begin(), all_global_variables.end());
    for(GlobalDeclaration *gd : all_global_variables) {
        if(gd->is_extern) continue;
        std::string label = gd->generate_cleanup_label();
        fout << indent() << "call " << label << "\n";
    }

    // emit exit
    if(!kernel_mode) {
        //get sys_exit(i32 status) label
        std::string exit_label = get_function_label(new FunctionSignature(new Identifier("sys_exit"), {primitives::i32->make_copy()}));

        //load exit status from stack
        fout << indent() << "movq 16(%rbp), %rax\n";

        //emit call
        fout << indent() << "push %rax\n";
        fout << indent() << "call " << exit_label << "\n";
    }
    else {
        // we can't really exit, TODO decide what to do here
        // can't really emit an error, just have to assume that the kernel never gets here
    }

    return true;
}

bool compile_all(std::string target_filepath, Package* target_package) {
    DefinitionSpace *target_ds = get_definition_space(target_filepath, target_package);

    // find all reachable definition spaces
    // (that correspond to a source file)
    std::vector<DefinitionSpace*> all_definition_spaces;
    std::function<bool(DefinitionSpace*)> find_definition_spaces = 
    [&all_definition_spaces, &find_definition_spaces](DefinitionSpace *ds) -> bool {
        for(int i = 0; i < all_definition_spaces.size(); i++) {
            if(all_definition_spaces[i] == ds) return true;
        }
        all_definition_spaces.push_back(ds);

        if(!ds->ensure_parsed()) {
            std::cout << "Failed to parse : " << ds->get_filepath() << std::endl;
            return false;
        }
        for(DefinitionSpace* nds : ds->get_included_definition_spaces()) {
            if(!find_definition_spaces(nds)) {
                return false;
            }
        }
        return true;
    };
    if(!find_definition_spaces(target_ds)) {
        return false;
    }

    // compile all files
    for(DefinitionSpace *ds : all_definition_spaces) {
        std::string filepath = ds->get_filepath();
        Package* package = ds->get_package();
        if(!compile(filepath, package)) {
            return false;
        }
    }

    // emit driver code
    if(!emit_driver(target_filepath, target_package)) {
        return false;
    }

    return true;
}

