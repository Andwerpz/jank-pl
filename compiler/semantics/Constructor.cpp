#include "Constructor.h"
#include "Type.h"
#include "Parameter.h"
#include "Statement.h"
#include "ConstructorSignature.h"
#include "utils.h"
#include "Identifier.h"
#include "TemplateMapping.h"
#include "Expression.h"
#include "primitives.h"
#include "Literal.h"
#include "Declaration.h"
#include "CompilationContext.h"
#include "DefinitionSpace.h"

// -- CONSTRUCTOR --
Constructor::Constructor(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Constructor::Constructor(const Constructor& other) : ASTNode(other) {
    type = other.type->make_copy();
    for(int i = 0; i < other.parameters.size(); i++) {
        parameters.push_back(other.parameters[i]->make_copy());
    }
    is_generated = other.is_generated;
}

Constructor::Constructor(Type* _type, std::vector<Parameter*> _parameters) : ASTNode() {
    type = _type;
    parameters = _parameters;
    assert(type != nullptr);
    for(int i = 0; i < parameters.size(); i++) assert(parameters[i] != nullptr);
}

StructConstructor::StructConstructor(parser::token *tok) : Constructor(tok) {
    // do nothing
}

StructConstructor::StructConstructor(const StructConstructor& other) : Constructor(other) {
    body = dynamic_cast<CompoundStatement*>(other.body->make_copy());
}

StructConstructor::StructConstructor(Type* _type, std::vector<Parameter*> _parameters, CompoundStatement *_body) : Constructor(_type, _parameters) {
    body = _body;
    assert(body != nullptr);
}

PrimitiveConstructor::PrimitiveConstructor(const PrimitiveConstructor& other) : Constructor(other) {
    is_copy_constructor = other.is_copy_constructor;
}

PrimitiveConstructor::PrimitiveConstructor(Type *_type, bool _is_copy_constructor) : Constructor(_type, {}) {
    is_copy_constructor = _is_copy_constructor;

    // primitive constructor should have primitive type
    assert(is_type_primitive(type));

    // copy constructor should have one argument of the copied type
    if(is_copy_constructor) {
        parameters.push_back(new Parameter(type->make_copy(), new Identifier("x")));
    }
}

// -- CONVERT --
Constructor* Constructor::convert(parser::constructor *c) {
    StructConstructor* result = new StructConstructor(c);
    result->type = BaseType::convert(c->t0->t0);
    parser::parameter_list *pl = c->t0->t4;
    result->parameters = convert_parameter_list(pl); 
    result->body = CompoundStatement::convert(c->t2);
    return result;
}

// -- RESOLVE CONSTRUCTOR SIGNATURE --
ConstructorSignature* Constructor::resolve_constructor_signature() const {
    std::vector<Type*> input_types;
    for(int i = 0; i < parameters.size(); i++){
        input_types.push_back(parameters[i]->type->make_copy());
    }
    return new ConstructorSignature(type->make_copy(), input_types);
}

// -- EQUALS --
bool StructConstructor::equals(Constructor* _other) const {
    if(!dynamic_cast<StructConstructor*>(_other)) return false;
    StructConstructor *other = dynamic_cast<StructConstructor*>(_other);
    return this->resolve_constructor_signature()->equals(other->resolve_constructor_signature());
}

bool PrimitiveConstructor::equals(Constructor *_other) const {
    if(!dynamic_cast<PrimitiveConstructor*>(_other)) return false;
    PrimitiveConstructor *other = dynamic_cast<PrimitiveConstructor*>(_other);
    if(!type->equals(other->type)) return false;
    if(is_copy_constructor != other->is_copy_constructor) return false;
    return true;
}

// -- IS WELL FORMED --
bool StructConstructor::is_well_formed(CompilationContext *ctx) {
    ConstructorSignature *cs = resolve_constructor_signature();

    push_declaration_stack();

    //print constructor header
    if(asm_debug) fout << "# " << cs->to_string() << "\n";
    std::string label = get_constructor_label(cs);
    assert(label.size() >= 2 && label[0] == '\"' && label[label.size() - 1] == '\"');
    std::string label_noquotes = label.substr(1, label.size() - 2);
    if(is_generated) {
        fout << ".section \".text." << label_noquotes << "\",\"axG\",@progbits," << label << ",comdat\n";
        fout << ".weak " << label << "\n";
    }
    else {
        fout << ".section \".text." << label_noquotes << "\",\"ax\",@progbits\n";
        fout << ".globl " << label << "\n";
    }
    fout << label << ":\n";

    //setup stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";

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
        std::cout << "Constructor undeclared return type : " << type->to_string() << "\n";
        return false;
    }

    //register self as variable (Type& this)
    local_offset = 8 + 8 * parameters.size();
    {
        //adjust local offset for 'extra variable'
        local_offset += 8;

        //register self as variable (Type& this)
        Type *vt = new ReferenceType(this->type->make_copy());
        Identifier *vid = new Identifier("this");
        Variable* v = add_stack_variable(vt, vid);
        if(v == nullptr) {
            std::cout << "Unable to add variable : " << vt << " " << vid << "\n";
            return false;
        }
        local_offset -= 8;
    }
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
        std::cout << "Constructor body not well formed\n";
        return false;
    }

    //add trailing return
    ReturnStatement *rs = new ReturnStatement(std::nullopt);
    if(!rs->is_well_formed(ctx)) {
        std::cout << "Trailing return failed??";
        assert(0);  
    }

    fout << "\n";

    //unregister parameters as variables
    pop_declaration_stack(ctx);

    //local stack should be empty before returning
    assert(stack_desc.size() == 0);

    return true;
}

bool PrimitiveConstructor::is_well_formed(CompilationContext *ctx) {
    assert(is_type_primitive(type));
    ConstructorSignature *cs = this->resolve_constructor_signature();

    //print constructor header
    if(asm_debug) fout << "# " << cs->to_string() << "\n";
    std::string label = get_constructor_label(cs);
    assert(label.size() >= 2 && label[0] == '\"' && label[label.size() - 1] == '\"');
    std::string label_noquotes = label.substr(1, label.size() - 2);
    if(is_generated) {
        fout << ".section \".text." << label_noquotes << "\",\"axG\",@progbits," << label << ",comdat\n";
        fout << ".weak " << label << "\n";
    }
    else {
        fout << ".section \".text." << label_noquotes << "\",\"ax\",@progbits\n";
        fout << ".globl " << label << "\n";
    }
    fout << label << ":\n";

    //setup stack frame
    fout << indent() << "push %rbp\n";  //should not be managed by local_offset
    fout << indent() << "mov %rsp, %rbp\n";

    if(is_copy_constructor) {
        //really jank solution (holy moly dylan janky)
        //just do 'this = x'
        //will have to rethink this if we prohibit assigning to 'this'
        //may have to do this a bit more low level

        assert(parameters.size() == 1);
        push_declaration_stack();

        local_offset = 8 + 8 * parameters.size() + 8;
        //register 'this' as variable
        Identifier *thisid = new Identifier("this");
        Variable *vthis = add_stack_variable(type, thisid);
        assert(vthis != nullptr);
        local_offset -= 8;

        //register only input as variable
        Identifier *xid = parameters[0]->id;
        Variable *vx = add_stack_variable(parameters[0]->type, xid);
        local_offset -= 8;

        //set local offset equal to %rsp
        local_offset = 0;

        //check that the local stack is empty so far
        assert(stack_desc.size() == 0);

        Expression *a_expr = new Expression(new ExprBinary(new ExprPrimary(thisid), "=", new ExprPrimary(xid)));
        assert(a_expr->resolve_type(ctx)->equals(type));
        a_expr->emit_asm(ctx);
        
        pop_declaration_stack(ctx);
    }

    //add trailing return
    fout << indent() << "pop %rbp\n";   //should not be managed by local_offset
    fout << indent() << "ret\n";

    fout << "\n";
    
    return true;
}

// -- MAKE COPY --
Constructor* StructConstructor::make_copy() {
    return new StructConstructor(*this);
}

Constructor* PrimitiveConstructor::make_copy() {
    return new PrimitiveConstructor(*this);
}

// -- REPLACE TEMPLATED TYPES --
bool StructConstructor::replace_templated_types(TemplateMapping *mapping) {
    if(auto x = mapping->find_mapped_type(type)) type = x;
    else if(!type->replace_templated_types(mapping)) return false;
    for(int i = 0; i < parameters.size(); i++){
        if(!parameters[i]->replace_templated_types(mapping)) return false;
    }
    if(!body->replace_templated_types(mapping)) return false;
    return true;
}

bool PrimitiveConstructor::replace_templated_types(TemplateMapping *mapping) {
    if(mapping->find_mapped_type(type)) {
        assert(false);
        return false;
    }
    if(!type->replace_templated_types(mapping)) {
        assert(false);
        return false;
    }   
    return true;
}

// -- LOOK FOR TEMPLATES --
bool StructConstructor::look_for_templates(CompilationContext *ctx) {
    if(!type->look_for_templates(ctx)) return false;
    for(int i = 0; i < parameters.size(); i++){
        if(!parameters[i]->look_for_templates(ctx)) return false;
    }
    if(!body->look_for_templates(ctx)) return false;
    return true;
}

bool PrimitiveConstructor::look_for_templates(CompilationContext *ctx) {
    if(!type->look_for_templates(ctx)) {
        assert(false);
        return false;
    }
    return true;
}