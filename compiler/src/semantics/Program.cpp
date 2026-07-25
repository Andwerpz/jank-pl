#include "Program.h"

#include <queue>
#include <map>
#include <algorithm>

#include "../parser/parser.h"
#include "StructDefinition.h"
#include "Function.h"
#include "utils.h"
#include "Type.h"
#include "FunctionSignature.h"
#include "Identifier.h"
#include "TemplatedStructDefinition.h"
#include "TemplatedFunction.h"
#include "Constructor.h"
#include "ConstructorSignature.h"
#include "TemplatedOperator.h"
#include "primitives.h"
#include "Include.h"
#include "GlobalDeclaration.h"
#include "Declaration.h"
#include "Destructor.h"
#include "GlobalNode.h"
#include "Typedef.h"
#include "TemplateMapping.h"
#include "Operator.h"
#include "OperatorSignature.h"
#include "TemplateHeader.h"

Program::Program(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

Program::Program(const Program& other) : ASTNode(other) {
    assert(false);  //shouldn't be making copies of Program
}

Program::Program(
    std::vector<TemplatedStructDefinition*> _templated_structs, 
    std::vector<TemplatedFunction*> _templated_functions, 
    std::vector<TemplatedOperator*> _templated_operators, 
    std::vector<Include*> _includes, 
    std::vector<GlobalDeclaration*> _global_declarations, 
    std::vector<GlobalNode*> _global_nodes, 
    std::vector<Typedef*> _typedefs
) : ASTNode() {
    templated_structs = _templated_structs;
    templated_functions = _templated_functions;
    templated_operators = _templated_operators;

    includes = _includes;

    global_declarations = _global_declarations;
    global_nodes = _global_nodes;

    typedefs = _typedefs;
}

Program::Program() : ASTNode() {
    // do nothing
}

Program* Program::convert(parser::program *p) {
    Program* result = new Program(p);
    for(int i = 0; i < p->t0.size(); i++){
        if(p->t0[i]->t1->is_c0) {       //templated function
            result->templated_functions.push_back(TemplatedFunction::convert(p->t0[i]->t1->t0->t0));
        }
        else if(p->t0[i]->t1->is_c1) {  //templated struct definition
            result->templated_structs.push_back(TemplatedStructDefinition::convert(p->t0[i]->t1->t1->t0));
        }
        else if(p->t0[i]->t1->is_c2) {  //templated overload
            result->templated_operators.push_back(TemplatedOperator::convert(p->t0[i]->t1->t2->t0));
        }
        else if(p->t0[i]->t1->is_c3) {  //include
            result->includes.push_back(Include::convert(p->t0[i]->t1->t3->t0));
        }
        else if(p->t0[i]->t1->is_c4) {  //global declaration
            result->global_declarations.push_back(GlobalDeclaration::convert(p->t0[i]->t1->t4->t0));
        }
        else if(p->t0[i]->t1->is_c5) {  //global node
            result->global_nodes.push_back(GlobalNode::convert(p->t0[i]->t1->t5->t0));
        }
        else if(p->t0[i]->t1->is_c6) {  //typedef
            result->typedefs.push_back(Typedef::convert(p->t0[i]->t1->t6->t0));
        }
        else assert(false);
    }
    return result;
}

Program* Program::make_copy() {
    return new Program(*this);
}
