#include "TemplatedFunction.h"
#include "TemplateHeader.h"
#include "Function.h"
#include "FunctionCall.h"
#include "utils.h"
#include "Type.h"
#include "TemplateMapping.h"
#include "Expression.h"

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
    //we should be able to match all the templated types purely from the function call signature. 
    //before matching, we should have resolved all templates in all the function call arguments. 
    std::vector<Type*> arg_types;
    for(int i = 0; i < fc->argument_list.size(); i++){
        Type *nt = fc->argument_list[i]->resolve_type();
        if(nt == nullptr) {
            return nullptr;
        }
    }

    //TODO compare against function signature
    FunctionSignature *fs = function->resolve_function_signature();
    TemplateMapping *mapping = new TemplateMapping();
    
}

Function* TemplatedFunction::gen_function(FunctionCall *fc) {
    return nullptr;
}