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

Program::Program() {
    // do nothing
}

Program::Program(std::vector<StructDefinition*> _structs, std::vector<TemplatedStructDefinition*> _templated_structs, std::vector<TemplatedFunction*> _templated_functions, std::vector<TemplatedOperator*> _templated_operators, std::vector<Include*> _includes, std::vector<GlobalDeclaration*> _global_declarations, std::vector<GlobalNode*> _global_nodes, std::vector<Typedef*> _typedefs) {
    structs = _structs;

    templated_structs = _templated_structs;
    templated_functions = _templated_functions;
    templated_operators = _templated_operators;

    includes = _includes;

    global_declarations = _global_declarations;
    global_nodes = _global_nodes;

    typedefs = _typedefs;
}

Program* Program::convert(parser::program *p) {
    std::vector<StructDefinition*> structs;
    std::vector<Overload*> overloads;

    std::vector<TemplatedStructDefinition*> templated_structs;
    std::vector<TemplatedFunction*> templated_functions;
    std::vector<TemplatedOperator*> templated_operators;

    std::vector<Include*> includes;

    std::vector<GlobalDeclaration*> global_declarations;
    std::vector<GlobalNode*> global_nodes;

    std::vector<Typedef*> typedefs;

    for(int i = 0; i < p->t0.size(); i++){
        if(p->t0[i]->t1->is_c0) {  //struct definition
            structs.push_back(StructDefinition::convert(p->t0[i]->t1->t0->t0));
        }
        else if(p->t0[i]->t1->is_c1) {  //templated function
            templated_functions.push_back(TemplatedFunction::convert(p->t0[i]->t1->t1->t0));
        }
        else if(p->t0[i]->t1->is_c2) {  //templated struct definition
            templated_structs.push_back(TemplatedStructDefinition::convert(p->t0[i]->t1->t2->t0));
        }
        else if(p->t0[i]->t1->is_c3) {  //templated overload
            templated_operators.push_back(TemplatedOperator::convert(p->t0[i]->t1->t3->t0));
        }
        else if(p->t0[i]->t1->is_c4) {  //include
            includes.push_back(Include::convert(p->t0[i]->t1->t4->t0));
        }
        else if(p->t0[i]->t1->is_c5) {  //global declaration
            global_declarations.push_back(GlobalDeclaration::convert(p->t0[i]->t1->t5->t0));
        }
        else if(p->t0[i]->t1->is_c6) {  //global node
            global_nodes.push_back(GlobalNode::convert(p->t0[i]->t1->t6->t0));
        }
        else if(p->t0[i]->t1->is_c7) {  //typedef
            typedefs.push_back(Typedef::convert(p->t0[i]->t1->t7->t0));
        }
        else assert(false);
    }
    return new Program(structs, templated_structs, templated_functions, templated_operators, includes, global_declarations, global_nodes, typedefs);
}

void Program::add_all(Program *other) {
    for(int i = 0; i < other->structs.size(); i++) structs.push_back(other->structs[i]);

    for(int i = 0; i < other->templated_structs.size(); i++) templated_structs.push_back(other->templated_structs[i]);
    for(int i = 0; i < other->templated_functions.size(); i++) templated_functions.push_back(other->templated_functions[i]);
    for(int i = 0; i < other->templated_operators.size(); i++) templated_operators.push_back(other->templated_operators[i]);

    for(int i = 0; i < other->includes.size(); i++) includes.push_back(other->includes[i]);
    
    for(int i = 0; i < other->global_declarations.size(); i++) global_declarations.push_back(other->global_declarations[i]);
    for(int i = 0; i < other->global_nodes.size(); i++) global_nodes.push_back(other->global_nodes[i]);

    for(int i = 0; i < other->typedefs.size(); i++) typedefs.push_back(other->typedefs[i]);
}

bool Program::is_well_formed() {
    // - set up semantic checker controller
    reset_controller();
    std::cout << "DONE INIT CONTROLLER" << std::endl;

    fout << ".section .text\n";

    //register all basetypes
    std::cout << "REGISTERING BASETYPES" << std::endl;
    for(int i = 0; i < templated_structs.size(); i++){
        BaseType *sbt = dynamic_cast<BaseType*>(templated_structs[i]->struct_def->type);
        assert(sbt != nullptr);
        if(!add_basetype(sbt)) {
            std::cout << "Failed to add basetype : " << sbt->to_string() << "\n";
            return false;
        }
    }
    for(int i = 0; i < structs.size(); i++) {
        BaseType *sbt = dynamic_cast<BaseType*>(structs[i]->type);
        assert(sbt != nullptr);
        if(!add_basetype(sbt)) {
            std::cout << "Failed to add basetype : " << sbt->to_string() << "\n";
            return false;
        }
    }

    // - resolve typedefs
    //at this point all basetypes should be registered in the controller
    std::cout << "RESOLVING TYPEDEFS" << std::endl;
    {
        //register typedefs as basetypes
        for(int i = 0; i < typedefs.size(); i++){
            BaseType *tdbt = typedefs[i]->base_type;
            if(!add_basetype(tdbt)) {
                std::cout << "Failed to add basetype : " << tdbt->to_string() << "\n";
                return false;
            }
        }

        //map basetypes to ints
        std::map<std::string, int> btind;
        for(int i = 0; i < typedefs.size(); i++){
            btind[typedefs[i]->base_type->name] = i;
        }

        //for each typedef, find what other typedefs it depends on
        int n = typedefs.size();
        std::vector<std::vector<int>> c(n);
        for(int i = 0; i < n; i++){
            std::vector<BaseType*> bts;
            typedefs[i]->type->find_all_basetypes(bts);
            
            //make sure all basetypes are either declared or typedef'd
            for(int j = 0; j < bts.size(); j++){
                BaseType *bt = bts[j];
                if(is_basetype_declared(bt)) continue;
                int ind = -1;
                for(int k = 0; k < n; k++){
                    if(typedefs[k]->base_type->equals(bt)) ind = k;
                }
                if(ind == -1) {
                    std::cout << "Basetype \"" << bt->to_string() << "\" not declared in : " << typedefs[i]->to_string() << "\n";
                    return false;
                }
                c[ind].push_back(i);
            }
        }

        //toposort typedefs
        std::vector<int> indeg(n, 0);
        for(int i = 0; i < n; i++){
            for(int x : c[i]) indeg[x] ++;
        }
        std::vector<int> ord(0);
        std::queue<int> q;
        for(int i = 0; i < n; i++) if(indeg[i] == 0) q.push(i);
        while(q.size() != 0){
            int cur = q.front();
            q.pop();
            ord.push_back(cur);
            for(int x : c[cur]) if(-- indeg[x] == 0) q.push(x);
        }
        if(ord.size() != n) {
            std::cout << "Could not find ordering for typedefs\n";
            return false;
        }

        //for each typedef, replace with previously defined basetypes
        TemplateMapping *mapping = new TemplateMapping();
        for(int i = 0; i < n; i++){
            int cur = ord[i];
            mapping->add_mapping(typedefs[cur]->base_type, typedefs[cur]->type);
            if(i + 1 != n) {
                int next = ord[i + 1];
                typedefs[next]->replace_templated_types(mapping);
            }
        }

        std::cout << "RESOLVED TYPEDEFS : \n";
        for(int i = 0; i < n; i++){
            std::cout << typedefs[i]->base_type->to_string() << " : " << typedefs[i]->type->to_string() << "\n";
        }

        //go through everything and replace typedef types
        bool success = true;
        for(int i = 0; i < structs.size(); i++) success &= structs[i]->replace_templated_types(mapping);
        for(int i = 0; i < templated_structs.size(); i++) success &= templated_structs[i]->replace_templated_types(mapping);
        for(int i = 0; i < templated_functions.size(); i++) success &= templated_functions[i]->replace_templated_types(mapping);
        for(int i = 0; i < templated_operators.size(); i++) success &= templated_operators[i]->replace_templated_types(mapping);
        for(int i = 0; i < global_declarations.size(); i++) success &= global_declarations[i]->replace_templated_types(mapping);
        if(!success) {
            std::cout << "Failed to resolve typedefs\n";
            return false;
        }
    }

    //for all templated structs add them as templated structs
    std::cout << "ADDING TEMPLATED STRUCTS" << std::endl;
    for(int i = 0; i < templated_structs.size(); i++){
        BaseType *sbt = dynamic_cast<BaseType*>(templated_structs[i]->struct_def->type);
        if(!add_templated_struct(templated_structs[i])) {
            std::cout << "Failed to add templated struct : " << sbt->to_string() << "\n";
            return false;
        }
    }

    //for all structs, register them as types
    std::cout << "ADDING STRUCTS" << std::endl;
    for(int i = 0; i < structs.size(); i++) {
        BaseType *sbt = dynamic_cast<BaseType*>(structs[i]->type);
        if(!add_struct_type(structs[i])) {
            std::cout << "Failed to add type : " << structs[i]->type->to_string() << "\n";
            return false;
        }
    }

    //for all templated functions add them
    std::cout << "ADDING TEMPLATED FUNCTIONS" << std::endl;
    for(int i = 0; i < templated_functions.size(); i++){
        if(!add_templated_function(templated_functions[i])) {
            std::cout << "Failed to add templated function : " << templated_functions[i]->function->resolve_function_signature()->to_string() << "\n";
            return false;
        }
    }

    //for all templated overloads add them
    std::cout << "ADDING TEMPLATED OVERLOADS" << std::endl;
    for(int i = 0; i < templated_operators.size(); i++){
        if(!add_templated_operator(templated_operators[i])) {
            std::cout << "Failed to add templated overload : " << templated_operators[i]->op->resolve_operator_signature()->to_string() << "\n";
            return false;
        }
    }

    // - are all structs well formed?
    std::cout << "CHECKING STRUCTS WELL FORMED" << std::endl;
    for(int i = 0; i < structs.size(); i++){
        if(!structs[i]->is_well_formed()) {
            std::cout << "Struct not well formed : " << structs[i]->type->to_string() << "\n";
            return false;
        }
    }

    // - can we construct a struct layout for all structs?
    std::cout << "CONSTRUCT BASE STRUCT LAYOUTS" << std::endl;
    for(int i = 0; i < structs.size(); i++){
        Type *t = structs[i]->type;
        if(!construct_struct_layout(t)) {
            std::cout << "Unable to construct struct layout for type : " << t->to_string() << "\n";
            return false;
        }
    }

    // - are all the templated structs well formed?
    std::cout << "CHECKING TEMPLATED STRUCTS WELL FORMED" << std::endl;
    for(int i = 0; i < templated_structs.size(); i++){
        if(!templated_structs[i]->is_well_formed()) {
            std::cout << "Templated struct not well formed : " << templated_structs[i]->struct_def->type->to_string() << "\n";
            return false;
        }
    }

    // - are all the templated functions well formed?
    std::cout << "CHECKING TEMPLATED FUNCTIONS WELL FORMED" << std::endl;
    for(int i = 0; i < templated_functions.size(); i++){
        if(!templated_functions[i]->is_well_formed()) {
            std::cout << "Templated function not well formed : " << templated_functions[i]->function->resolve_function_signature()->to_string() << "\n";
            return false;
        }
    }

    // - are all the templated overloads well formed?
    std::cout << "CHECKING TEMPLATED OVERLOADS WELL FORMED" << std::endl;
    for(int i = 0; i < templated_operators.size(); i++){
        if(!templated_operators[i]->is_well_formed()) {
            std::cout << "Templated overload not well formed : " << templated_operators[i]->op->resolve_operator_signature()->to_string() << "\n";
            return false;
        }
    }

    // - entry point semantics
    {   
        // - there must be exactly one function that can be considered an entry point
        bool found_main = false;
        for(int i = 0; i < this->templated_functions.size(); i++) {
            TemplatedFunction* tf = this->templated_functions[i];
            if(!tf->is_main()) continue;

            if(found_main) {
                std::cout << "Found multiple valid mains\n";
                return false;
            }

            found_main = true;
        }
        if(!found_main) {
            std::cout << "Missing valid main function\n";
            return false;
        }
    }

    // - driver code
    {
        // - emit _start label
        fout << ".global _start\n";
        fout << "_start:\n";

        // - parse argc, argv, envp, auxv, reserve stack slot for exit status
        // result should be stack with
        // u64 argc
        // u8** argv <-- %rsp
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
            Variable* environ_v = add_global_variable(new PointerType(new PointerType(primitives::u8->make_copy())), new Identifier("environ"), false);
            fout << indent() << "lea 8(%r9, %r8, 8), %r10\n";     //envp in %r10
            fout << indent() << "movq %r10, " << environ_v->addr << "\n";

            //TODO deal with auxv

            //reserve stack slot for exit status
            fout << indent() << "sub $8, %rsp\n";

            //save argc, argv to stack
            fout << indent() << "pushq %r8\n";
            fout << indent() << "pushq %r9\n";
        }

        // - ensure proper local variable behaviour
        //setup global variable stack frame
        //%rbp should start by pointing towards %rsp for proper local variable behaviour
        push_declaration_stack();
        fout << indent() << "mov %rsp, %rbp\n";

        // - global variable initialization
        std::vector<GlobalDeclaration*> global_delcarations;
        {
            std::cout << "INITIALIZING GLOBAL VARIABLES" << std::endl;

            if(asm_debug) fout << indent() << "# start initialize global variables\n";

            std::vector<GlobalNode*> global_nodes = this->global_nodes;
            global_declarations = this->global_declarations;
            std::cout << "GLOBAL AMT : " << global_declarations.size() << "\n";

            //resolve global declaration templates
            for(int i = 0; i < global_declarations.size(); i++){
                if(!global_declarations[i]->look_for_templates()) {
                    std::cout << "Unable to resolve templates in declaration of global variable " << global_declarations[i]->declaration->id->name << "\n";
                    return false;
                }
            }

            //add __GLOBAL_FIRST__ node
            std::string gfirst_name = "__GLOBAL_FIRST__";
            global_nodes.push_back(new GlobalNode(new Identifier(gfirst_name), {}));

            //check for duplicate global nodes
            for(int i = 0; i < global_nodes.size(); i++){
                for(int j = i + 1; j < global_nodes.size(); j++){
                    if(global_nodes[i]->id->equals(global_nodes[j]->id)) {
                        std::cout << "Duplicate global node declaration : " << global_nodes[i]->id->name << "\n";
                        return false;
                    }
                }
            }

            //map node identifiers to indices
            std::map<std::string, int> id_map;
            for(int i = 0; i < global_nodes.size(); i++){
                id_map[global_nodes[i]->id->name] = i;
            }

            //all dependencies must be defined
            for(int i = 0; i < global_nodes.size(); i++){
                for(int j = 0; j < global_nodes[i]->dependencies.size(); j++){
                    if(!id_map.count(global_nodes[i]->dependencies[j]->name)) {
                        std::cout << "Undefined dependency \"" << global_nodes[i]->dependencies[j]->name << "\" in node \"" << global_nodes[i]->id->name << "\"\n";
                        return false;
                    }
                }
            }

            //do topological sort
            std::vector<int> node_order;
            {   
                int n = global_nodes.size();
                std::vector<std::vector<int>> c(n);
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < global_nodes[i]->dependencies.size(); j++){
                        c[id_map[global_nodes[i]->dependencies[j]->name]].push_back(i);
                    }                
                }

                //enforce that __GLOBAL_FIRST__ is first
                for(int i = 0; i < n; i++){
                    if(i == id_map[gfirst_name]) continue;
                    c[id_map[gfirst_name]].push_back(i);
                }

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
            if(node_order.size() != global_nodes.size()) {
                std::cout << "Failed to find topological ordering of global nodes\n";
                return false;
            }
            std::map<std::string, int> order_map;
            std::cout << "Global node initialization order:\n";
            for(int i = 0; i < global_nodes.size(); i++){
                int ind = node_order[i];
                order_map[global_nodes[ind]->id->name] = i;
                std::cout << global_nodes[ind]->id->name << "\n";
            }

            //sort by tier and initialize
            std::sort(global_declarations.begin(), global_declarations.end(), [&order_map, &global_nodes](GlobalDeclaration *a, GlobalDeclaration *b) -> bool {
                int aorder = a->node_id.has_value()? order_map[a->node_id.value()->name] : global_nodes.size();
                int border = b->node_id.has_value()? order_map[b->node_id.value()->name] : global_nodes.size();
                return aorder < border;
            });
            for(int i = 0; i < global_declarations.size(); i++){
                bool is_extern = global_declarations[i]->is_extern;
                Type *type = global_declarations[i]->declaration->type;
                Identifier *id = global_declarations[i]->declaration->id;
                
                std::string addr_str = id->name + "(%rip)";
                assert(addr_str.size() != 0);
                std::cout << "GLOBAL : " << type->to_string() << " " << id->name << "\n";

                if(asm_debug) fout << indent() << "# initialize global variable : " << type->to_string() << " " << id->name << "\n";
                Variable *v = emit_initialize_variable(type, id, global_declarations[i]->declaration->expr, addr_str, true, is_extern);
                if(asm_debug) fout << indent() << "# done initialize global variable : " << type->to_string() << " " << id->name << "\n";
            
                if(v == nullptr) {
                    std::cout << "Failed to initialize global variable : " << type->to_string() << " " << id->name << "\n";
                    return false;
                }
            }

            if(asm_debug) fout << indent() << "# done initialize global variables\n";

            std::cout << "DONE INITIALIZE GLOBAL VARIABLES" << std::endl;
        }

        // - emit call to main function
        {
            //make sure main function is first function
            std::sort(declared_functions.begin(), declared_functions.end(), [](Function *a, Function *b) -> bool {
                return a->is_main() > b->is_main();
            });
            Function *main = declared_functions[0];
            assert(main->is_main());

            //emit call
            std::string label = get_function_label(main->resolve_function_signature());
            fout << indent() << "call " << label << "\n";

            //%rax should contain exit status, save it to reserved slot
            fout << indent() << "movq %rax, 16(%rbp)\n";
        }

        // - global variable destruction
        emit_cleanup_global_variables();

        // - remove global stack frame from controller
        pop_declaration_stack();

        // - emit sys_exit()
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
            //we can't really exit, TODO decide what to do here
        }
    }

    // - check all functions and constructors, make sure they're well formed
    int function_ptr = 0;
    int constructor_ptr = 0;
    int operator_ptr = 0;
    int destructor_ptr = 0;
    while(function_ptr < declared_functions.size() || constructor_ptr < declared_constructors.size() || operator_ptr < declared_operators.size()) {
        while(function_ptr < declared_functions.size()) {
            ld process_start_time = current_time_seconds();
            Function *f = declared_functions[function_ptr ++];
            enclosing_return_type = f->type->make_copy();
            enclosing_type = f->enclosing_type;
            if(!f->is_well_formed()) {
                return false;
            }
            add_duration_stat("FUNCTION " + f->resolve_function_signature()->to_string(), current_time_seconds() - process_start_time);
        }
        while(constructor_ptr < declared_constructors.size()) {
            ld process_start_time = current_time_seconds();
            Constructor *c = declared_constructors[constructor_ptr ++];
            enclosing_return_type = primitives::_void->make_copy();
            enclosing_type = c->type;
            if(!c->is_well_formed()) {
                return false;
            }
            add_duration_stat("CONSTRUCTOR " + c->resolve_constructor_signature()->to_string(), current_time_seconds() - process_start_time);
        }
        while(operator_ptr < declared_operators.size()) {
            ld process_start_time = current_time_seconds();
            Operator *o = declared_operators[operator_ptr ++];
            if(dynamic_cast<OperatorOverload*>(o) == nullptr) continue; //only process overloads
            OperatorOverload *oo = dynamic_cast<OperatorOverload*>(o);
            assert(oo != nullptr);
            enclosing_return_type = oo->type;
            enclosing_type = std::nullopt;
            if(!oo->is_well_formed()) {
                return false;
            }
            add_duration_stat("OVERLOAD " + o->resolve_operator_signature()->to_string(), current_time_seconds() - process_start_time);
        }
        while(destructor_ptr < declared_destructors.size()) {
            ld process_start_time = current_time_seconds();
            Destructor *d = declared_destructors[destructor_ptr ++];
            enclosing_return_type = primitives::_void->make_copy();
            enclosing_type = d->type;
            if(!d->is_well_formed()) {
                return false;
            }
            add_duration_stat("DESTRUCTOR " + d->type->to_string() + "::~()", current_time_seconds() - process_start_time);
        }
    }

    // - emit .data section
    emit_data_section();

    assert(declaration_stack.size() == 0);

    return true;
}