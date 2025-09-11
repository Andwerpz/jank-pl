#include "TemplatedFunction.h"
#include "TemplateHeader.h"
#include "Function.h"
#include "FunctionCall.h"
#include "utils.h"
#include "Type.h"
#include "TemplateMapping.h"
#include "Expression.h"
#include "Identifier.h"
#include "FunctionSignature.h"
#include "Parameter.h"
#include "Statement.h"

TemplatedFunction::TemplatedFunction(TemplateHeader *_header, Function *_function) {
    assert(_header != nullptr);
    assert(_function != nullptr);

    header = _header;
    function = _function;
}

TemplatedFunction* TemplatedFunction::convert(parser::templated_function *f) {
    TemplateHeader *header = new TemplateHeader({});
    if(f->t0.has_value()) header = TemplateHeader::convert(f->t0.value()->t0);
    Function *function = Function::convert(f->t1);
    return new TemplatedFunction(header, function);
}

bool TemplatedFunction::is_well_formed() {
    // - functions with templates cannot have export modifier
    if(function->is_export && header->types.size() != 0) {
        std::cout << "Templated functions cannot have export modifier : " << function->resolve_function_signature()->to_string() << "\n";
        return false;
    }

    // - are all of the templated basetypes not declared?
    for(int i = 0; i < header->types.size(); i++){
        if(is_basetype_declared(header->types[i])) {
            std::cout << "Template basetype " << header->types[i]->to_string() << " already declared\n";
            return false;
        }
    }

    // - are there any duplicate function types?
    for(int i = 0; i < header->types.size(); i++){
        for(int j = 0; j < i; j++){
            if(header->types[i]->equals(header->types[j])) {
                std::cout << "Duplicate template basetype : " << header->types[i]->to_string() << "\n";
                return false;
            }
        }
    }

    return true;
}

TemplateMapping* TemplatedFunction::calc_mapping(std::vector<Type*> arg_types) {
    // - does the number of arguments match?
    if(function->parameters.size() != arg_types.size()) return nullptr;

    // - generate mapping
    FunctionSignature *fs = function->resolve_function_signature();
    TemplateMapping *mapping = new TemplateMapping();
    for(int i = 0; i < arg_types.size(); i++){
        Type *arg_type = arg_types[i]->make_copy();
        Type *param_type = function->parameters[i]->type->make_copy();

        // - are the reference semantics correct? 
        //T& can only map to T& argument, while T can map to T and T&
        //so the only bad combo is param_type = T& and arg_type = T
        if(dynamic_cast<ReferenceType*>(param_type) != nullptr && dynamic_cast<ReferenceType*>(arg_type) == nullptr) {
            return nullptr;
        }

        arg_type = arg_type->remove_reference();
        param_type = param_type->remove_reference();

        TemplateMapping *nm = arg_type->generate_mapping(param_type, header);
        
        if(nm == nullptr) return nullptr;
        if(!mapping->merge_with_mapping(nm)) return nullptr;
    }

    // - are all the template types mapped?
    if(mapping->mapping.size() != header->types.size()) {
        return nullptr;
    }

    return mapping;
}

TemplateMapping* TemplatedFunction::calc_mapping(FunctionCall *fc) {
    // - does number of arguments match?
    if(function->parameters.size() != fc->argument_list.size()) return nullptr;

    // - does identifier match?
    if(!function->id->equals(fc->id)) return nullptr;

    // - does the enclosing type match?
    if(function->enclosing_type.has_value() != fc->target_type.has_value()) return nullptr;
    if(function->enclosing_type.has_value() && !function->enclosing_type.value()->equals(fc->target_type.value())) return nullptr;

    //we should be able to match all the templated types purely from the function call signature. 
    //before matching, we should have resolved all templates in all the function call arguments. 
    std::vector<Type*> arg_types;
    for(int i = 0; i < fc->argument_list.size(); i++){
        Type *nt = fc->argument_list[i]->resolve_type();
        if(nt == nullptr) {
            return nullptr;
        }
        assert(dynamic_cast<ReferenceType*>(nt) == nullptr);
        if(fc->argument_list[i]->is_lvalue()) nt = new ReferenceType(nt);
        arg_types.push_back(nt);
    }

    return calc_mapping(arg_types);
}

Function* TemplatedFunction::gen_function(FunctionCall *fc) {
    // - see if a previously generated function works
    for(int i = 0; i < generated_functions.size(); i++){
        if(generated_functions[i]->is_valid_call(fc)) return generated_functions[i];
    }

    // - is there a mapping?
    TemplateMapping *mapping = this->calc_mapping(fc);
    if(mapping == nullptr) return nullptr;

    // - try to construct
    Function *n_function = this->function->make_copy();
    if(!n_function->replace_templated_types(mapping)) {
        assert(false);
        return nullptr;
    }

    // - ok we good
    assert(n_function->is_valid_call(fc));
    generated_functions.push_back(n_function);
    if(!add_function(n_function)) assert(false);

    return n_function;
}

bool TemplatedFunction::replace_templated_types(TemplateMapping *mapping) {
    //make sure the mapped types don't interfere with this mapping
    for(int i = 0; i < header->types.size(); i++){
        Type *t = header->types[i]->make_copy();
        for(int j = 0; j < mapping->mapping.size(); j++){
            Type *mt = mapping->mapping[j].first->make_copy();
            if(t->equals(mt)) return false;
        }
    }

    return function->replace_templated_types(mapping);
}

TemplatedFunction* TemplatedFunction::make_copy() {
    return new TemplatedFunction(header->make_copy(), function->make_copy());
}
