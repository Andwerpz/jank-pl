#include "TemplatedOperator.h"
#include "Operator.h"
#include "TemplateHeader.h"
#include "Type.h"
#include "utils.h"
#include "Parameter.h"
#include "TemplateMapping.h"
#include "Expression.h"
#include "OperatorCall.h"
#include "OperatorSignature.h"

TemplatedOperator::TemplatedOperator(TemplateHeader *_header, Operator *_op) {
    assert(_header != nullptr);
    assert(_op != nullptr);
    header = _header;
    op = _op;
}

TemplatedOperator* TemplatedOperator::convert(parser::templated_overload *o) {
    TemplateHeader *header = new TemplateHeader({});
    if(o->t0.has_value()) header = TemplateHeader::convert(o->t0.value()->t0);
    Operator *op = OperatorOverload::convert(o->t1);
    return new TemplatedOperator(header, op);
}

bool TemplatedOperator::is_well_formed() {
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

    // - does the operator even resolve to a valid operator signature?
    if(!op->resolve_operator_signature()) {
        std::cout << "Operator does not resolve to operator signature\n";
        return false;
    }

    return true;
}

TemplateMapping* TemplatedOperator::calc_mapping(OperatorSignature *oos) {
    OperatorSignature *os = op->resolve_operator_signature();

    // - does the operator resolve to an operator signature?
    if(os == nullptr) return nullptr;
    
    // - does left argument exist in both?
    if(oos->left.has_value() != os->left.has_value()) return nullptr;
    
    // - does right argument exist in both?
    if(oos->right.has_value() != os->right.has_value()) return nullptr;

    // - does the operator match?
    if(oos->op != os->op) return nullptr;

    //try to construct mapping
    TemplateMapping *mapping = new TemplateMapping();
    if(oos->left.has_value()) {
        Type *larg_type = oos->left.value();
        Type *lparam_type = os->left.value();
        
        // - are the reference semantics correct?
        if(dynamic_cast<ReferenceType*>(lparam_type) != nullptr && dynamic_cast<ReferenceType*>(larg_type) == nullptr) {
            return nullptr;
        }

        larg_type = larg_type->remove_reference();
        lparam_type = lparam_type->remove_reference();

        TemplateMapping *nm = larg_type->generate_mapping(lparam_type, header);
        if(nm == nullptr) return nullptr;
        if(!mapping->merge_with_mapping(nm)) return nullptr;
    }
    if(oos->right.has_value()) {
        Type *rarg_type = oos->right.value();
        Type *rparam_type = os->right.value();
        
        // - are the reference semantics correct?
        if(dynamic_cast<ReferenceType*>(rparam_type) != nullptr && dynamic_cast<ReferenceType*>(rarg_type) == nullptr) {
            return nullptr;
        }

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

TemplateMapping* TemplatedOperator::calc_mapping(OperatorCall *oc) {
    OperatorSignature *os = op->resolve_operator_signature();

    // - does the operator match?
    if(os->op != oc->op) return nullptr;

    // - does left argument exist in both?
    if(os->left.has_value() != oc->left.has_value()) return nullptr;
    
    // - does right argument exist in both?
    if(os->right.has_value() != oc->right.has_value()) return nullptr;

    std::optional<Type*> left = std::nullopt, right = std::nullopt;
    if(oc->left.has_value()) {
        left = oc->left.value()->resolve_type();
        if(left.value() == nullptr) return nullptr;
        if(oc->left.value()->is_lvalue()) left = new ReferenceType(left.value());
    }
    if(oc->right.has_value()) {
        right = oc->right.value()->resolve_type();
        if(right.value() == nullptr) return nullptr;
        if(oc->right.value()->is_lvalue()) right = new ReferenceType(right.value());
    }
    return this->calc_mapping(new OperatorSignature(left, oc->op, right));
}

Operator* TemplatedOperator::gen_operator(OperatorCall *oc) {
    // - see if a previously generated operator works
    for(int i = 0; i < generated_operators.size(); i++){
        if(generated_operators[i]->is_valid_call(oc)) return generated_operators[i];
    }

    // - is there a mapping?
    TemplateMapping *mapping = this->calc_mapping(oc);
    if(mapping == nullptr) return nullptr;

    // - try to construct
    Operator *n_operator = this->op->make_copy();
    if(!n_operator->replace_templated_types(mapping)) {
        assert(false);
        return nullptr;
    }

    // - ok we good
    assert(n_operator->is_valid_call(oc));
    generated_operators.push_back(n_operator);
    if(!add_operator(n_operator)) assert(false);

    return n_operator;
}

bool TemplatedOperator::replace_templated_types(TemplateMapping *mapping) {
    //make sure the mapped types don't interfere with this mapping
    for(int i = 0; i < header->types.size(); i++){
        Type *t = header->types[i]->make_copy();
        for(int j = 0; j < mapping->mapping.size(); j++){
            Type *mt = mapping->mapping[i].first->make_copy();
            if(t->equals(mt)) return false;
        }
    }

    return op->replace_templated_types(mapping);
}