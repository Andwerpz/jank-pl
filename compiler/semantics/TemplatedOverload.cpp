#include "TemplatedOverload.h"
#include "Overload.h"
#include "TemplateHeader.h"
#include "Type.h"
#include "utils.h"
#include "Parameter.h"
#include "TemplateMapping.h"
#include "Expression.h"

TemplatedOverload::TemplatedOverload(TemplateHeader *_header, Overload *_overload) {
    assert(_header != nullptr);
    assert(_overload != nullptr);
    header = _header;
    overload = _overload;
}

TemplatedOverload* TemplatedOverload::convert(parser::templated_overload *o) {
    TemplateHeader *header = TemplateHeader::convert(o->t0);
    Overload *overload = Overload::convert(o->t2);
    return new TemplatedOverload(header, overload);
}

bool TemplatedOverload::is_well_formed() {
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

    // - does the overload even resolve to a valid operator signature?
    if(!overload->resolve_operator_signature()) {
        std::cout << "Overload does not resolve to operator signature\n";
        return false;
    }

    return true;
}

TemplateMapping* TemplatedOverload::calc_mapping(std::optional<Expression*> left, std::string op, std::optional<Expression*> right) {
    OperatorSignature *os = overload->resolve_operator_signature();

    // - does the overload resolve to an operator signature?
    if(os == nullptr) return nullptr;
    
    // - does left argument exist in both?
    if(left.has_value() != os->left.has_value()) return nullptr;
    
    // - does right argument exist in both?
    if(right.has_value() != os->right.has_value()) return nullptr;

    // - does the operator match?
    if(os->op != op) return nullptr;

    //try to construct mapping
    TemplateMapping *mapping = new TemplateMapping();
    if(left.has_value()) {
        Type *larg_type = left.value()->resolve_type();
        Type *lparam_type = os->left.value();
        if(larg_type == nullptr) return nullptr;
        larg_type = larg_type->remove_reference();
        lparam_type = lparam_type->remove_reference();

        TemplateMapping *nm = larg_type->generate_mapping(lparam_type, header);
        if(nm == nullptr) return nullptr;
        if(!mapping->merge_with_mapping(nm)) return nullptr;
    }
    if(right.has_value()) {
        Type *rarg_type = right.value()->resolve_type();
        Type *rparam_type = os->right.value();
        if(rarg_type == nullptr) return nullptr;
        rarg_type = rarg_type->remove_reference();
        rparam_type = rparam_type->remove_reference();

        TemplateMapping *nm = rarg_type->generate_mapping(rparam_type, header);
        if(nm == nullptr) return nullptr;
        if(!mapping->merge_with_mapping(nm)) return nullptr;
    }

    // - are all the template types mapped?
    if(mapping->mapping.size() != header->types.size()) {
        return nullptr;
    }

    return mapping;
}

Overload* TemplatedOverload::gen_overload(std::optional<Expression*> left, std::string op, std::optional<Expression*> right) {
    // - is there a mapping?
    TemplateMapping *mapping = this->calc_mapping(left, op, right);
    if(mapping == nullptr) return nullptr;

    //try to construct
    Overload *n_overload = this->overload->make_copy();
    if(!n_overload->replace_templated_types(mapping)) return nullptr;
    return n_overload;
}