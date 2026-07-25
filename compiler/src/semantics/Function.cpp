#include "Function.h"
#include "Identifier.h"
#include "Type.h"
#include "FunctionSignature.h"
#include "utils.h"
#include "Statement.h"
#include "Parameter.h"
#include "TemplateMapping.h"
#include "primitives.h"
#include "GlobalDeclaration.h"
#include "Program.h"
#include "Declaration.h"
#include "Expression.h"
#include "GlobalNode.h"
#include "utils.h"
#include "FunctionCall.h"
#include "CompilationContext.h"
#include "DefinitionSpace.h"

#include <algorithm>
#include <map>
#include <queue>

Function::Function(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Function::Function(const Function& other) : ASTNode(other) {
    enclosing_type = std::nullopt;
    if(other.enclosing_type.has_value()) enclosing_type = other.enclosing_type.value()->make_copy();
    is_export = other.is_export;
    type = other.type->make_copy();
    id = other.id->make_copy();
    for(int i = 0; i < other.parameters.size(); i++){
        parameters.push_back(other.parameters[i]->make_copy());
    }
    body = dynamic_cast<CompoundStatement*>(other.body->make_copy());
    is_generated = other.is_generated;
}

Function::Function(std::optional<Type*> _enclosing_type, bool _is_export, Type *_type, Identifier *_id, std::vector<Parameter*> _parameters, CompoundStatement *_body) : ASTNode() {
    enclosing_type = _enclosing_type;
    is_export = _is_export;
    id = _id;
    type = _type;
    parameters = _parameters;
    body = _body;

    assert(type != nullptr);
    assert(body != nullptr);
    assert(id != nullptr);
}

Function::Function(Type *_type, Identifier *_id, std::vector<Type*> input_types) : ASTNode() {
    enclosing_type = std::nullopt;
    id = _id;
    type = _type;
    for(int i = 0; i < input_types.size(); i++){
        parameters.push_back(new Parameter(input_types[i], new Identifier("v" + std::to_string(i))));
    }
    body = nullptr;

    assert(type != nullptr);
    assert(id != nullptr);
}

FunctionSignature* Function::resolve_function_signature() const {
    std::vector<Type*> input_types;
    for(int i = 0; i < parameters.size(); i++) input_types.push_back(parameters[i]->type);
    if(enclosing_type.has_value()) return new FunctionSignature(enclosing_type.value(), id, input_types);
    return new FunctionSignature(id, input_types);
}

bool Function::equals(const Function* other) const {
    return this->resolve_function_signature()->equals(other->resolve_function_signature());
}

bool Function::operator==(const Function& other) const {
    return this->equals(&other);
}

bool Function::operator!=(const Function& other) const {
    return !(*this == other);
}

Function* Function::convert(parser::function *f) {
    Function* result = new Function(f);
    result->enclosing_type = std::nullopt;  //need context for this
    result->is_export = f->t0.has_value();
    parser::function_definition *def = f->t1;
    parser::parameter_list *pl = def->t6;
    result->type = Type::convert(def->t0);
    result->id = new Identifier(def->t2->to_string());
    result->parameters = convert_parameter_list(pl);
    result->body = CompoundStatement::convert(f->t3);
    return result;
}

bool Function::is_well_formed(CompilationContext *ctx) {
    assert(body != nullptr);
    FunctionSignature *fs = resolve_function_signature();

    // - struct member functions cannot have export modifier
    if(enclosing_type.has_value() && is_export) {
        std::cout << "Struct member functions cannot have export modifier : " << resolve_function_signature()->to_string() << "\n";
        return false;
    }

    //print function header
    if(asm_debug) fout << "# " << fs->to_string() << "\n";
    std::string label = get_function_label(fs);
    std::string label_noquotes = label;
    if(label.size() >= 2 && label[0] == '\"' && label[label.size() - 1] == '\"') {
        label_noquotes = label.substr(1, label.size() - 2);
    }
    if(is_generated) {
        fout << ".section \".text." << label_noquotes << "\",\"axG\",@progbits," << label << ",comdat\n";
        fout << ".weak " << label << "\n";
    }
    else {
        fout << ".section \".text." << label_noquotes << "\",\"ax\",@progbits\n";
        fout << ".globl " << label << "\n";
    }
    fout << label << ":\n";

    //setup function stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";

    push_declaration_stack();
    
    for(int i = 0; i < parameters.size(); i++){
        // - does parameter correspond to existing type?
        if(!ctx->is_type_declared(parameters[i]->type)) {
            std::cout << "Undeclared type : " << parameters[i]->type->to_string() << "\n";
            return false;
        }
        // - is parameter type not void?
        if(parameters[i]->type->equals(primitives::_void)) {
            std::cout << "Parameter can't have type void\n";
            return false;
        }
    }

    // - is return type of function existing?
    if(!ctx->is_type_declared(type)) {
        std::cout << "Function undeclared return type : " << type->to_string() << " " << id->name << "\n";
        return false;
    }
    
    //if has enclosing type, register self as variable (Type& this)
    local_offset = 8 + 8 * parameters.size();
    if(enclosing_type.has_value()) {
        //adjust local offset for 'extra variable'
        local_offset += 8;

        //register self as variable (Type& this)
        Type *vt = new ReferenceType(enclosing_type.value());
        Identifier *vid = new Identifier("this");
        Variable* v = add_stack_variable(vt, vid);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << vt << " " << vid << "\n";
            return false;
        }
        local_offset -= 8;
    }

    //push parameters onto stack
    for(int i = 0; i < parameters.size(); i++){
        Variable* v = add_stack_variable(parameters[i]->type, parameters[i]->id);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << parameters[i]->type->to_string() << " " << parameters[i]->id->name << "\n";
            return false;
        }
        local_offset -= 8;
    }

    //set local offset equal to %rsp
    local_offset = 0;

    //check that the local stack is empty so far
    assert(stack_desc.size() == 0);

    // - make sure body is well formed
    if(!body->is_well_formed(ctx)) {
        std::cout << "Function body not well formed\n";
        return false;
    }
    
    // - if type is not void, check for existence of return statement as last reachable statement
    if(!type->equals(primitives::_void)) {
        // Note that if there is a statement before the last that is always returning, then any statement after
        // it is unreachable code, in which case we should print some warnings. 
        if(!body->is_always_returning()) {
            std::cout << "Non-void function doesn't always return\n";
            return false;
        }
    }
    else {
        //add trailing return for void functions
        ReturnStatement *rs = new ReturnStatement(std::nullopt);
        if(!rs->is_well_formed(ctx)) {
            std::cout << "Trailing return failed??";
            assert(0);  
        }
    }

    fout << "\n";

    //unregister parameters as variables
    pop_declaration_stack(ctx);

    //local stack should be empty before returning
    assert(stack_desc.size() == 0);

    return true;
}

Function* Function::make_copy() {
    return new Function(*this);
}

bool Function::replace_templated_types(TemplateMapping *mapping) {
    if(enclosing_type.has_value()) {
        Type *val = enclosing_type.value();
        if(auto x = mapping->find_mapped_type(val)) val = x;
        else if(!val->replace_templated_types(mapping)) return false;
        enclosing_type = val;
    }
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    for(int i = 0; i < parameters.size(); i++){
        if(!parameters[i]->replace_templated_types(mapping)) return false;
    }
    if(!body->replace_templated_types(mapping)) return false;
    return true;
}

bool Function::look_for_templates(CompilationContext *ctx) {
    if(enclosing_type.has_value()) if(!enclosing_type.value()->look_for_templates(ctx)) return false;
    if(!type->look_for_templates(ctx)) return false;
    for(int i = 0; i < parameters.size(); i++) if(!parameters[i]->look_for_templates(ctx)) return false;
    if(!body->look_for_templates(ctx)) return false;
    return true;
}

std::string Function::to_string() {
    std::string ret = "";
    ret += type->to_string() + " ";
    if(enclosing_type.has_value()) ret += enclosing_type.value()->to_string() + "::";
    ret += id->name + "(";
    for(int i = 0; i < parameters.size(); i++){
        ret += parameters[i]->to_string();
        if(i + 1 != parameters.size()) ret += ", ";
    }
    ret += ") ";
    ret += body->to_string();
    return ret;
}

bool Function::is_main() {
    FunctionSignature *fs = resolve_function_signature();
    assert(fs != nullptr);
    if(is_generated) return false;
    if(!type->equals(primitives::i32)) return false;
    if(fs->equals(new FunctionSignature(new Identifier("main"), {}))) return true;
    if(fs->equals(new FunctionSignature(new Identifier("main"), {primitives::u64->make_copy(), new PointerType(new PointerType(primitives::u8->make_copy()))}))) return true;
    return false;
}

bool Function::is_valid_call(CompilationContext *ctx, FunctionCall *fc) {
    // - do the identifiers match?
    if(!this->id->equals(fc->id)) {
        return false;
    }
    // - do the argument counts match?
    if(this->parameters.size() != fc->argument_list.size()) {
        return false;
    }
    // - does enclosing_type match? (must match exactly)
    if(this->enclosing_type.has_value() != fc->target_type.has_value()) {
        return false;
    }
    if(this->enclosing_type.has_value() && !this->enclosing_type.value()->equals(fc->target_type.value())) {
        return false;
    }

    // - can all the argument expressions be declared as the corresponding parameter types?
    bool is_viable = true;
    for(int i = 0; i < this->parameters.size(); i++) {
        Type *nt = this->parameters[i]->type;
        if(!ctx->is_declarable(nt, fc->argument_list[i])) {
            return false;
        }
    }
    
    //all checks passed
    return true;
}