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

TemplatedFunction::TemplatedFunction(TemplateHeader *_header, Function *_function) {
    assert(_header != nullptr);
    assert(_function != nullptr);

    header = _header;
    function = _function;
}

TemplatedFunction* TemplatedFunction::convert(parser::templated_function *f) {
    TemplateHeader *header = TemplateHeader::convert(f->t0);
    Function *function = Function::convert(f->t2);
    return new TemplatedFunction(header, function);
}

bool TemplatedFunction::is_well_formed() {
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

TemplateMapping* TemplatedFunction::calc_mapping(FunctionCall *fc) {
    std::cout << "TEMPLATED FUNCTION GENERATE MAPPING : " << fc->to_string() << "\n";

    // - does number of arguments match?
    if(function->parameters.size() != fc->argument_list.size()) return nullptr;

    // - does identifier match?
    if(!function->id->equals(fc->id)) return nullptr;

    //we should be able to match all the templated types purely from the function call signature. 
    //before matching, we should have resolved all templates in all the function call arguments. 
    std::vector<Type*> arg_types;
    for(int i = 0; i < fc->argument_list.size(); i++){
        Type *nt = fc->argument_list[i]->resolve_type();
        if(nt == nullptr) {
            return nullptr;
        }
        arg_types.push_back(nt);
    }

    std::cout << "ARG TYPES : ";
    for(int i = 0; i < arg_types.size(); i++) std::cout << arg_types[i]->to_string() << " ";
    std::cout << "\n";

    //compare against function signature
    FunctionSignature *fs = function->resolve_function_signature();
    TemplateMapping *mapping = new TemplateMapping();
    for(int i = 0; i < arg_types.size(); i++){
        Type *arg_type = arg_types[i]->make_copy();
        Type *param_type = function->parameters[i]->type->make_copy();

        arg_type = arg_type->remove_reference();
        param_type = param_type->remove_reference();

        std::cout << "TRY TO MATCH : " << arg_type->to_string() << " " << param_type->to_string() << "\n";
        TemplateMapping *nm = arg_type->generate_mapping(param_type, header);
        
        if(nm == nullptr) return nullptr;
        std::cout << "NEXT MAPPING : " << nm->to_string() << "\n";
        if(!mapping->merge_with_mapping(nm)) return nullptr;
    }

    // - are all the template types mapped?
    if(mapping->mapping.size() != header->types.size()) {
        return nullptr;
    }

    return mapping;
}

Function* TemplatedFunction::gen_function(FunctionCall *fc) {
    std::cout << "TRYING TO GENERATE FROM FC : " << fc->to_string() << "\n";

    // - is there a mapping?
    TemplateMapping *mapping = this->calc_mapping(fc);
    if(mapping == nullptr) return nullptr;

    //try to construct
    std::cout << "TRY TO CONSTRUCT WITH MAPPING : " << mapping->to_string() << "\n";
    Function *n_function = this->function->make_copy();
    if(!n_function->replace_templated_types(mapping)) return nullptr;
    std::cout << "DONE CONSTRUCT : " << n_function->resolve_function_signature()->to_string() << "\n";
    return n_function;
}