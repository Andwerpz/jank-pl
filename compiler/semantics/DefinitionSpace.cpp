#include "DefinitionSpace.h"

#include "TemplatedFunction.h"
#include "TemplatedOperator.h"
#include "TemplatedStructDefinition.h"

#include "Function.h"
#include "Operator.h"
#include "StructDefinition.h"
#include "Constructor.h"
#include "Destructor.h"

#include "StructLayout.h"
#include "GlobalDeclaration.h"
#include "Typedef.h"

#include "Type.h"

#include "FunctionSignature.h"
#include "OperatorSignature.h"
#include "ConstructorSignature.h"

#include "FunctionCall.h"
#include "OperatorCall.h"
#include "ConstructorCall.h"
#include "DestructorCall.h"

#include "Expression.h"

#include "utils.h"

#include "TemplateHeader.h"
#include "TemplateMapping.h"
#include "Statement.h"
#include "Identifier.h"
#include "Parameter.h"
#include "Declaration.h"
#include "Program.h"
#include "Include.h"
#include "CompilationContext.h"
#include "GlobalNode.h"

// -- DECLARATION SET --
template <typename T>
void DeclarationSet<T>::Bucket::add(T* decl, Visibility vis) {
    all_decls.push_back(decl);
    switch(vis) {
    case Visibility::Public :
        public_decls.push_back(decl);
        break;
    case Visibility::Private :
        private_decls.push_back(decl);
        break;
    default:
        assert(false);
    }
}

template <typename T>
const std::vector<T*>& DeclarationSet<T>::Bucket::get(Visibility vis) const {
    switch(vis) {
    case Visibility::All :
        return all_decls;
    case Visibility::Public :
        return public_decls;
    case Visibility::Private :
        return private_decls;
    default:
        assert(false);
    }
}

template <typename T>
const std::vector<T*>& DeclarationSet<T>::Bucket::all() const {
    return all_decls;
}

template <typename T>
void DeclarationSet<T>::add(T* decl, Visibility vis, Origin orig) {
    assert(orig != Origin::All);
    all_decls.add(decl, vis);
    switch(orig) {
    case Origin::Source :
        source_decls.add(decl, vis);
        break;
    case Origin::Generated :
        generated_decls.add(decl, vis);
        break;
    default: 
        assert(false);
    }
}

template <typename T>
const std::vector<T*>& DeclarationSet<T>::get(Visibility vis, Origin orig) const {
    switch(orig) {
    case Origin::All :
        return all_decls.all();
    case Origin::Source :
        return source_decls.all();
    case Origin::Generated :
        return generated_decls.all();
    default:
        assert(false);
    }
}

template <typename T>
const std::vector<T*>& DeclarationSet<T>::all() const {
    return all_decls.all();
}

// -- DEFINITION SPACE --
DefinitionSpace::DefinitionSpace() {
    is_builtin = true;
    filepath = "";
    label_prefix = "__builtin__";
    state = DefinitionSpaceState::Unparsed;
}

DefinitionSpace::DefinitionSpace(std::string _filepath) {
    is_builtin = false;
    filepath = _filepath;
    state = DefinitionSpaceState::Unparsed;

    //normalize filepath
    if(filepath[0] != '/') {
        filepath = cwd_rel_to_absolute(filepath);
    }
    filepath = normalize_path(filepath);
    label_prefix = filepath;
}

// -- ADDING DECLARATIONS --
bool DefinitionSpace::add_templated_function(TemplatedFunction* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    if(is_templated_function_declared(x)) {
        std::cout << "Templated function already declared : " << x->function->resolve_function_signature()->to_string() << std::endl;
        return false;
    }
    templated_functions.add(x, vis, orig);

    if(x->header->types.size() != 0) {
        std::cout << "ADD TEMPLATED FUNCTION : " << x->function->resolve_function_signature()->to_string() << std::endl;
    }
    
    return true;
}

bool DefinitionSpace::add_templated_operator(TemplatedOperator* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    if(is_templated_operator_declared(x)) return false;
    templated_operators.add(x, vis, orig);

    if(x->header->types.size() != 0) {
        std::cout << "ADD TEMPLATED OPERATOR : " << x->op->resolve_operator_signature()->to_string() << "\n";
    }
    return true;
}

bool DefinitionSpace::add_templated_struct(TemplatedStructDefinition* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    if(is_templated_struct_declared(x)) return false;
    templated_structs.add(x, vis, orig);

    if(x->header->types.size() != 0) {
        std::cout << "ADD TEMPLATED STRUCT : " << x->struct_def->type->to_string() << "\n";
    }
    return true;
}

bool DefinitionSpace::add_operator(Operator* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    OperatorSignature *os = x->resolve_operator_signature();
    if(is_operator_declared(os)) return false;
    operators.add(x, vis, orig);

    //if this operator is an overload, generate a label
    if(dynamic_cast<OperatorOverload*>(x)) {
        OperatorOverload *oo = dynamic_cast<OperatorOverload*>(x);
        OperatorSignature *os = oo->resolve_operator_signature();
        std::string label = "\"" + label_prefix + "::" + os->to_string() + "\"";
        operator_label_map.insert({os, label});
    }

    std::cout << "ADD OPERATOR : " << os->to_string() << std::endl;
    return true;
}

bool DefinitionSpace::add_function(Function* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    FunctionSignature *fs = x->resolve_function_signature();
    if(is_function_declared(fs)) return false;
    functions.add(x, vis, orig);

    // generate label
    std::string label;
    if(x->is_export) {
        // should generate label equal to function id
        label = x->id->to_string();
    }
    else {
        // generate label normally
        label = "\"" + label_prefix + "::" + fs->to_string() + "\"";
    }
    function_label_map.insert({fs, label});

    std::cout << "ADD FUNCTION : " << fs->to_string() << std::endl;
    return true;
}

bool DefinitionSpace::add_struct(StructDefinition* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    Type *t = x->type;
    if(!is_type_declared(t)) {  
        // a non-templated struct should not be added here with an undeclared type
        // all basetypes should be declared by the time this is involved in compilation
        assert(dynamic_cast<BaseType*>(t) == nullptr);
        assert(dynamic_cast<TemplatedType*>(t));
        types.add(t, vis, orig);
    }
    structs.add(x, vis, orig);
    struct_definitions.push_back(x);

    std::cout << "ADD TYPE : " << t->to_string() << "\n";
    return true;
}

bool DefinitionSpace::add_constructor(Constructor* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    ConstructorSignature *cs = x->resolve_constructor_signature();
    if(is_constructor_declared(cs)) return false;
    constructors.add(x, vis, orig);
    
    // generate label
    std::string label = "\"" + label_prefix + "::" + cs->to_string() + "\"";
    constructor_label_map.insert({cs, label});

    std::cout << "ADD CONSTRUCTOR : " << cs->to_string() << std::endl;
    return true;
}

bool DefinitionSpace::add_destructor(Destructor* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    Type *t = x->type;
    if(is_destructor_declared(t)) return false;
    destructors.add(x, vis, orig);

    // generate label
    std::string label = "\"" + label_prefix + "::~" + t->to_string() + "\"";
    destructor_label_map.insert({t, label});

    std::cout << "ADD DESTRUCTOR : " << t->to_string() << std::endl;
    return true;
}

bool DefinitionSpace::add_global_variable(GlobalDeclaration* x, Visibility vis, Origin orig) {
    if(is_global_variable_declared(x)) return false;
    global_variables.add(x, vis, orig);

    return true;
}

bool DefinitionSpace::add_typedef(Typedef* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    typedefs.add(x, vis, orig);
    return true;
}

bool DefinitionSpace::add_type(Type* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    if(is_type_declared(x)) return false;
    types.add(x, vis, orig);
    return true;
}

bool DefinitionSpace::add_basetype(BaseType* x, Visibility vis, Origin orig) {
    assert(x != nullptr);
    if(!add_type(x, vis, orig)) return false;
    basetypes.add(x, vis, orig);
    assert(!basetype_to_definition_space.contains(x));
    basetype_to_definition_space.insert(std::make_pair(x, this));
    return true;
}

// -- RETRIEVING DECLARATIONS --
const std::vector<TemplatedFunction*>& DefinitionSpace::get_templated_functions(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::TypedefsResolved);
    return templated_functions.get(vis, orig);
}

const std::vector<TemplatedOperator*>& DefinitionSpace::get_templated_operators(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::TypedefsResolved);
    return templated_operators.get(vis, orig);
}

const std::vector<TemplatedStructDefinition*>& DefinitionSpace::get_templated_structs(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::TypedefsResolved);
    return templated_structs.get(vis, orig);
}

const std::vector<Operator*>& DefinitionSpace::get_operators(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::DeclarationsResolved);
    return operators.get(vis, orig);
}

const std::vector<Function*>& DefinitionSpace::get_functions(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::DeclarationsResolved);
    return functions.get(vis, orig);
}

const std::vector<StructDefinition*>& DefinitionSpace::get_structs(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::DeclarationsResolved);
    return structs.get(vis, orig);
}

const std::vector<Constructor*>& DefinitionSpace::get_constructors(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::DeclarationsResolved);
    return constructors.get(vis, orig);
}

const std::vector<Destructor*>& DefinitionSpace::get_destructors(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::DeclarationsResolved);
    return destructors.get(vis, orig);
}

const std::vector<Typedef*>& DefinitionSpace::get_typedefs(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::Parsed);
    return typedefs.get(vis, orig);
}

const std::vector<GlobalDeclaration*>& DefinitionSpace::get_global_variables(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::TypedefsResolved);
    return global_variables.get(vis, orig);
}

const std::vector<Type*>& DefinitionSpace::get_types(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::TypedefsResolved);
    return types.get(vis, orig);
}

const std::vector<BaseType*>& DefinitionSpace::get_basetypes(Visibility vis, Origin orig) {
    assert(state >= DefinitionSpaceState::Parsed);
    return basetypes.get(vis, orig);
}

// -- INITIALIZATION --
bool DefinitionSpace::ensure_parsed() {
    // did we already parse this file?
    if(state >= DefinitionSpaceState::Parsed) {
        return true;
    }

    // is this builtin? we can skip parsing if so
    // it's assumed that the declarations have been populated externally. 
    if(is_builtin) {
        state = DefinitionSpaceState::Parsed;
        return true;
    }

    // load source code
    // TODO check if the filepath given is actually valid
    std::string code = read_file(filepath);
    source_files.push_back(filepath);

    if(filepath == "/home/steven/jank-pl/compiler") {
        std::cout << "GOT CODE : \n" << code << std::endl;
        assert(false);
    }

    // parse source code
    parser::set_s(code);
    parser::set_gen_errors(false);
    parser::program *pp = parser::program::parse();
    if(!parser::check_finished_parsing(true) || parser::get_errors().size() != 0) {
        std::cout << "SYNTAX ERROR\n";
        return false;
    }

    // generate AST
    Program *p = Program::convert(pp);

    // grab all declarations
    for(TemplatedFunction* tf : p->templated_functions) {
        if(!add_templated_function(tf, Visibility::Public, Origin::Source)) {
            std::cout << "Failed to add templated function : " << tf->function->resolve_function_signature()->to_string() << std::endl;
            return false;
        }
    }
    for(TemplatedOperator* to : p->templated_operators) {
        if(!add_templated_operator(to, Visibility::Public, Origin::Source)) {
            std::cout << "Failed to add templated overload : " << to->op->resolve_operator_signature()->to_string() << std::endl;
            return false;
        }
    }
    for(TemplatedStructDefinition* ts : p->templated_structs) {
        if(!add_templated_struct(ts, Visibility::Public, Origin::Source)) {
            std::cout << "Failed to add templated struct : " << ts->struct_def->type->to_string() << std::endl;
            return false;
        }

        //also register BaseType
        BaseType* bt = dynamic_cast<BaseType*>(ts->struct_def->type);
        assert(bt != nullptr);
        if(!add_basetype(bt, Visibility::Public, Origin::Source)) {
            std::cout << "Failed to add BaseType : " << bt->to_string() << std::endl;
            return false;
        }
    }
    for(GlobalDeclaration* gd : p->global_declarations) {
        if(!add_global_variable(gd, Visibility::Public, Origin::Source)) {
            std::cout << "Failed to add global variable : " << gd->declaration->id->to_string() << std::endl;
            return false;
        }
    }
    for(Typedef* td : p->typedefs) {
        if(!add_typedef(td, Visibility::Public, Origin::Source)) {
            std::cout << "Failed to add typedef : " << td->to_string() << std::endl;
            return false;
        }
    }
    for(Include* inc : p->includes) {
        add_include(inc);
    }
    for(GlobalNode* gn : p->global_nodes) {
        add_global_node(gn);
    }

    // add default library includes
    {
        std::vector<std::string> default_includes = {
            "memory",
            "error",
            "defs",
        };  

        //if we're not in kernel mode, can include some utilities provided by the kernel
        if(!kernel_mode) {
            default_includes.push_back("syscall");
            default_includes.push_back("malloc");
        }

        //do we have default includes disabled?
        if(no_default_includes) {
            default_includes.clear();
        }

        for(std::string s : default_includes) {
            Include* inc = new Include(s, true);
            add_include(inc);
        }
    }

    state = DefinitionSpaceState::Parsed;
    return true;
}

bool DefinitionSpace::ensure_includes_parsed() {
    // did we already parse includes?
    if(state >= DefinitionSpaceState::IncludesParsed) {
        return true;
    }

    // make sure we've parsed this file
    if(!ensure_parsed()) {
        return false;
    }

    // parse all includes
    for(DefinitionSpace* inc_ds : included_definition_spaces) {
        if(!inc_ds->ensure_parsed()) {
            std::cout << "Could not parse include : \"" << inc_ds->get_filepath() << "\"" << std::endl;
            return false;
        }
    }

    state = DefinitionSpaceState::IncludesParsed;
    return true;
}

bool DefinitionSpace::ensure_typedefs_resolved() {
    // did we already do this?
    if(state >= DefinitionSpaceState::TypedefsResolved) {
        return true;
    }

    // make sure we've parsed all includes
    if(!ensure_includes_parsed()) {
        return false;
    }

    CompilationContext* ctx = create_compilation_context();
    assert(ctx != nullptr);

    // gather all typedefs
    // TODO write a custom iterator for ctx and just iterate through all the typedefs
    std::vector<Typedef*> all_typedefs;
    for(CompilationContext::DefinitionSpaceView dsv : ctx->definition_spaces) {
        DefinitionSpace *ds = dsv.space;
        Visibility vis = dsv.vis;
        for(Typedef *td : ds->get_typedefs(vis)) {
            all_typedefs.push_back(td);
        }
    }

    // make sure typedefs don't declare duplicate BaseTypes
    for(int i = 0; i < all_typedefs.size(); i++) {
        for(int j = i + 1; j < all_typedefs.size(); j++) {
            BaseType* ta = all_typedefs[i]->base_type;
            BaseType* tb = all_typedefs[j]->base_type;
            if(ta->equals(tb)) {
                std::cout << "Typedefs defining duplicate BaseType : \"" << ta->to_string() << "\"" << std::endl;
                return false;
            }
        }
    }

    // make sure none of the typedefs BaseType is the same as a declared BaseType
    for(Typedef* td : all_typedefs) {
        BaseType* bt = td->base_type;
        if(is_basetype_declared(bt)) {
            std::cout << "Typedef overwriting declared BaseType : " << td->to_string() << std::endl;
            return false;
        }
    }

    // apply typedefs
    // need to find an ordering to apply all the typedefs
    {
        int n = all_typedefs.size();

        // map typedef basetypes to ints
        std::map<std::string, int> btind;
        for(int i = 0; i < n; i++){
            btind[all_typedefs[i]->base_type->name] = i;
        }

        // for each typedef, find what other typedefs it depends on
        // builds a dependency graph between all typedefs
        std::vector<std::vector<int>> c(n);
        for(int i = 0; i < n; i++){
            std::vector<BaseType*> bts;
            all_typedefs[i]->type->find_all_basetypes(bts);
            
            //make sure all basetypes are either declared or typedef'd
            for(int j = 0; j < bts.size(); j++){
                BaseType *bt = bts[j];
                if(ctx->is_basetype_declared(bt)) {
                    // this basetype is declared
                    continue;
                }

                // find the typedef that declares this basetype
                int ind = -1;
                for(int k = 0; k < n; k++){
                    if(all_typedefs[k]->base_type->equals(bt)) {
                        ind = k;
                    }
                }
                if(ind == -1) {
                    std::cout << "Basetype \"" << bt->to_string() << "\" not declared in : " << all_typedefs[i]->to_string() << "\n";
                    return false;
                }
                c[ind].push_back(i);
            }
        }

        // toposort typedefs
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

        // incrementally build final type mapping. 
        TemplateMapping *mapping = new TemplateMapping();
        for(int i = 0; i < n; i++){
            int cur = ord[i];
            mapping->add_mapping(all_typedefs[cur]->base_type, all_typedefs[cur]->type);
            if(i + 1 != n) {
                int next = ord[i + 1];
                all_typedefs[next]->replace_templated_types(mapping);
            }
        }

        if(debug) {
            std::cout << "RESOLVED TYPEDEFS : \n";
            for(int i = 0; i < n; i++){
                std::cout << all_typedefs[i]->base_type->to_string() << " : " << all_typedefs[i]->type->to_string() << "\n";
            }
        }

        //go through everything and replace typedef types
        bool success = true;
        for(TemplatedFunction* tf : templated_functions.all()) {
            success &= tf->replace_templated_types(mapping);
        }
        for(TemplatedOperator* to : templated_operators.all()) {
            success &= to->replace_templated_types(mapping);
        }
        for(TemplatedStructDefinition* tsd : templated_structs.all()) {
            success &= tsd->replace_templated_types(mapping);
        }
        for(GlobalDeclaration* gd : global_variables.all()) {
            success &= gd->replace_templated_types(mapping);
        }
        if(!success) {
            std::cout << "Failed to apply typedefs\n";
            return false;
        }
    }

    delete ctx;

    state = DefinitionSpaceState::TypedefsResolved;
    return true;
}

bool DefinitionSpace::ensure_declarations_resolved() {
    // did we already do this?
    if(state >= DefinitionSpaceState::DeclarationsResolved) {
        return true;
    }

    // ensure our typedefs are resolved
    if(!ensure_typedefs_resolved()) {
        return false;
    }

    // ensure our imports typedefs are resolved
    for(DefinitionSpace* inc_ds : included_definition_spaces) {
        if(!inc_ds->ensure_typedefs_resolved()) {
            std::cout << "Could not resolve typedefs in include : \"" << inc_ds->get_filepath() << "\"" << std::endl;
            return false;
        }
    }

    CompilationContext* ctx = create_compilation_context();
    assert(ctx != nullptr);

    // ensure set of available basetypes are unique
    // TODO make this better by using a hashset or smth
    // TODO replace this with an actual iterator 
    std::vector<BaseType*> all_basetypes;
    for(CompilationContext::DefinitionSpaceView dsv : ctx->definition_spaces) {
        DefinitionSpace *ds = dsv.space;
        Visibility vis = dsv.vis;
        for(BaseType *bt : ds->get_basetypes(vis)) {
            all_basetypes.push_back(bt);
        }
    }
    for(int i = 0; i < all_basetypes.size(); i++) {
        for(int j = i + 1; j < all_basetypes.size(); j++) {
            BaseType* ta = all_basetypes[i];
            BaseType* tb = all_basetypes[j];
            if(ta->equals(tb)) {
                std::cout << "Duplicate BaseType : \"" << ta->to_string() << "\"" << std::endl;
                return false;
            }
        }
    }

    // add templated declarations with no template variables as concrete instantiations
    // we do this explicitly so that it's clear to other definition spaces that these declarations
    //   are compiled by this definition space. 
    // TODO properly figure out visibility
    // - add structs first so that all templated functions are registered
    for(TemplatedStructDefinition* tsd : templated_structs.get(Visibility::All, Origin::Source)) {
        if(tsd->header->types.size() == 0) {
            // add struct
            StructDefinition *sd = tsd->struct_def->make_copy();
            Type *t = sd->type;
            add_struct(sd, Visibility::Public, Origin::Source);

            // add struct member functions, constructors, destructors
            // don't want to add to work queue, just want to make this struct interface available
            for(int i = 0; i < sd->functions.size(); i++){
                TemplatedFunction *f = sd->functions[i];
                if(!add_templated_function(f, Visibility::Public, Origin::Source)) {
                    std::cout << "Failed to add struct member function : " << f->function->resolve_function_signature()->to_string() << "\n";
                    return false;
                } 
            }
            for(int i = 0; i < sd->constructors.size(); i++) {
                Constructor *c = sd->constructors[i];
                if(!add_constructor(c, Visibility::Public, Origin::Source)) {
                    std::cout << "Failed to add struct constructor : " << c->resolve_constructor_signature()->to_string() << "\n";
                    return false;
                }
            }
            for(int i = 0; i < sd->destructors.size(); i++){
                Destructor *d = sd->destructors[i];
                if(!add_destructor(d, Visibility::Public, Origin::Source)) {
                    std::cout << "Failed to add destructor : " << t->to_string() << "\n";
                    return false;
                }
            }
            
            // default default constructor
            add_constructor(
                new StructConstructor(
                    t->make_copy(), 
                    {}, 
                    new CompoundStatement(std::vector<Statement*>{})
                ),
                Visibility::Public,
                Origin::Source
            );

            // default copy constructor
            // - for each field, just does default copy constructor
            {
                Identifier *oid = new Identifier("other");
                Identifier *tid = new Identifier("this");

                std::vector<Statement*> statements;
                for(int i = 0; i < sd->member_variables.size(); i++){
                    Identifier *mvi = sd->member_variables[i]->id;
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
                    new StructConstructor(
                        t->make_copy(), 
                        {new Parameter(new ReferenceType(t->make_copy()), oid->make_copy())}, 
                        body
                    ),
                    Visibility::Public,
                    Origin::Source
                );
            }

            // default destructor
            add_destructor(
                new Destructor(
                    t->make_copy(), 
                    new CompoundStatement(std::vector<Statement*>{})
                ),
                Visibility::Public,
                Origin::Source
            );
            
            // resolve templates in struct definition
            if(!sd->look_for_templates(ctx)) {
                std::cout << "Failed to resolve templates in " << t->to_string() << std::endl;
                return false;
            }

            // check if struct is well formed
            if(!sd->is_well_formed(ctx)) {
                std::cout << "Struct not well formed : " << t->to_string() << std::endl;
                return false;
            }
        }
    }
    for(TemplatedFunction* tf : templated_functions.get(Visibility::All, Origin::Source)) {
        if(tf->header->types.size() == 0) {
            Function *f = tf->function->make_copy();
            tf->generated_functions.push_back(f);
            add_function(f, Visibility::Public, Origin::Source);
        }
    }
    for(TemplatedOperator* to : templated_operators.get(Visibility::All, Origin::Source)) {
        if(to->header->types.size() == 0) {
            Operator *o = to->op->make_copy();
            to->generated_operators.push_back(o);
            add_operator(o, Visibility::Public, Origin::Source);
        }
    }

    // look for templates in concrete instantiations
    // note that we already looked for templates in instantiated structs
    // don't want to do it here as then we'll start to look for templates within templated types
    for(Function *f : functions.get(Visibility::All, Origin::Source)) {
        if(f->is_generated) continue;
        if(!f->look_for_templates(ctx)) {
            std::cout << "DefinitionSpace :: Failed to resolve templates in function : " << f->resolve_function_signature()->to_string() << std::endl;
            return false;
        }
    }
    for(Operator *o : operators.get(Visibility::All, Origin::Source)) {
        if(o->is_generated) continue;
        if(!o->look_for_templates(ctx)) {
            std::cout << "Failed to resolve templates in operator : " << o->resolve_operator_signature()->to_string() << std::endl;
            return false;
        }
    }
    for(Constructor *c : constructors.get(Visibility::All, Origin::Source)) {
        if(c->is_generated) continue;
        if(!c->look_for_templates(ctx)) {
            std::cout << "Failed to resolve templates in constructor : " << c->resolve_constructor_signature()->to_string() << std::endl;
            return false;
        }
    }
    for(Destructor *d : destructors.get(Visibility::All, Origin::Source)) {
        if(d->is_generated) continue;
        if(!d->look_for_templates(ctx)) {
            std::cout << "Failed to resolve templates in destructor : " << d->type->to_string() << std::endl;
            return false;
        }
    }
    for(GlobalDeclaration *gd : global_variables.get(Visibility::All, Origin::Source)) {
        if(!gd->look_for_templates(ctx)) {
            std::cout << "Failed to resolve templates in global variable declaration : " << gd->declaration->to_string() << std::endl;
            return false;
        }
    }

    // check templated declarations + instantiated structs well formed
    // to do this, we mostly just need a stable list of BaseTypes in this context
    // it could be the case that typedefs are malformed in an import at this point, 
    //   but that will be caught when we resolve that import
    for(TemplatedFunction* tf : templated_functions.get(Visibility::All, Origin::Source)) {
        if(!tf->is_well_formed(ctx)) {
            std::cout << "Templated function not well formed : " << tf->function->resolve_function_signature()->to_string() << std::endl;
            return false;
        }
    }
    for(TemplatedOperator* to : templated_operators.get(Visibility::All, Origin::Source)) {
        if(!to->is_well_formed(ctx)) {
            std::cout << "Templated operator not well formed : " << to->op->resolve_operator_signature()->to_string() << std::endl;
            return false;
        }
    }
    for(TemplatedStructDefinition* tsd : templated_structs.get(Visibility::All, Origin::Source)) {
        if(!tsd->is_well_formed(ctx)) {
            std::cout << "Templated struct not well formed : " << tsd->struct_def->type->to_string() << std::endl;
            return false;
        }
    }
    for(StructDefinition *sd : structs.get(Visibility::All, Origin::Source)) {
        if(!sd->is_well_formed(ctx)) {
            std::cout << "Struct not well formed : " << sd->type->to_string() << std::endl;
            return false;
        }
    }

    delete ctx;

    state = DefinitionSpaceState::DeclarationsResolved;
    return true;
}

bool DefinitionSpace::ensure_includes_resolved() {
    // did we already do this?
    if(state >= DefinitionSpaceState::IncludesResolved) {
        return true;
    }

    // ensure we are locally resolved
    if(!ensure_declarations_resolved()) {
        return false;
    }

    // ensure all includes are locally resolved
    for(DefinitionSpace* inc_ds : included_definition_spaces) {
        if(!inc_ds->ensure_declarations_resolved()) {
            std::cout << "Could not locally resolve include : \"" << inc_ds->get_filepath() << "\"" << std::endl;
            return false;
        }
    }

    state = DefinitionSpaceState::IncludesResolved;
    return true;
}

// -- CHECKING IS DECLARED -- 

//do this by seeing if we can draw a bijection between the sets of calls that f and another function can handle
bool DefinitionSpace::is_templated_function_declared(TemplatedFunction* x) const {
    assert(x != nullptr);

    // gather arguments of input templated function
    std::vector<Type*> farg_list;
    for(int i = 0; i < x->function->parameters.size(); i++){
        farg_list.push_back(x->function->parameters[i]->type->make_copy());
    }

    // look for match
    for(int i = 0; i < templated_functions.all().size(); i++){
        TemplatedFunction *of = templated_functions.all()[i];

        // - do they have the same identifier?
        if(!x->function->id->equals(of->function->id)) {
            continue;
        }

        // - are enclosing types the same?
        if(x->function->enclosing_type.has_value() != of->function->enclosing_type.has_value()) continue;
        if(x->function->enclosing_type.has_value() && !x->function->enclosing_type.value()->equals(of->function->enclosing_type.value())) continue;

        // - can they both map to eachother?
        std::vector<Type*> arg_list;    
        for(int j = 0; j < of->function->parameters.size(); j++){
            arg_list.push_back(of->function->parameters[j]->type->make_copy());
        }
        if(of->calc_mapping(farg_list) == nullptr || x->calc_mapping(arg_list) == nullptr) {
            continue;
        }

        //found bijection
        std::cout << "Duplicate functions : " << x->function->resolve_function_signature()->to_string() << " : " << of->function->resolve_function_signature()->to_string() << std::endl;
        return true;
    }

    // nothing found
    return false;
}

//same idea as checking for duplicate templated function, but with less checks. 
bool DefinitionSpace::is_templated_operator_declared(TemplatedOperator* x) const {
    assert(x != nullptr);
    
    // look for match
    OperatorSignature *os = x->op->resolve_operator_signature();
    for(int i = 0; i < templated_operators.all().size(); i++){
        TemplatedOperator *oo = templated_operators.all()[i];
        OperatorSignature *oos = oo->op->resolve_operator_signature();

        // - can they both map to eachother?
        if(x->calc_mapping(oos) == nullptr || oo->calc_mapping(os) == nullptr) {
            continue;
        }

        //found bijection
        std::cout << "Duplicate operators : " << os->to_string() << " : " << oos->to_string() << "\n";
        return true;
    }

    // nothing found
    return false;
}

//for now just see if the basetype matches with another templated struct
bool DefinitionSpace::is_templated_struct_declared(TemplatedStructDefinition* x) const {
    assert(x != nullptr);
    for(int i = 0; i < templated_structs.all().size(); i++){
        if(x->struct_def->type->equals(templated_structs.all()[i]->struct_def->type)) return true;
    }
    return false;
}

bool DefinitionSpace::is_function_declared(FunctionSignature* fs) const {
    assert(fs != nullptr);
    for(int i = 0; i < functions.all().size(); i++){
        if(fs->equals(functions.all()[i]->resolve_function_signature())) {
            return true;
        }
    }
    return false;
}

bool DefinitionSpace::is_operator_declared(OperatorSignature* os) const {
    assert(os != nullptr);
    for(int i = 0; i < operators.all().size(); i++){
        if(os->equals(operators.all()[i]->resolve_operator_signature())) {
            return true;
        }
    }
    return false;
}

bool DefinitionSpace::is_constructor_declared(ConstructorSignature* cs) const {
    assert(cs != nullptr);
    for(int i = 0; i < constructors.all().size(); i++){
        if(cs->equals(constructors.all()[i]->resolve_constructor_signature())) {
            return true;
        }
    }
    return false;
}

bool DefinitionSpace::is_destructor_declared(Type* t) const {
    assert(t != nullptr);
    for(int i = 0; i < destructors.all().size(); i++){
        if(t->equals(destructors.all()[i]->type)) {
            return true;
        }
    }
    return false;
}

bool DefinitionSpace::is_global_variable_declared(GlobalDeclaration* gd) const {
    assert(gd != nullptr);
    for(int i = 0; i < global_variables.all().size(); i++) {
        if(gd->declaration->id->equals(global_variables.all()[i]->declaration->id)) {
            return true;
        }
    }
    return false;
}

bool DefinitionSpace::is_type_declared(Type* t) const {
    assert(
        dynamic_cast<BaseType*>(t) != nullptr ||
        dynamic_cast<ArrayType*>(t) != nullptr ||
        dynamic_cast<TemplatedType*>(t) != nullptr
    );
    for(Type *ot : types.all()){
        if(t->equals(ot)) return true;
    }
    return false;
}

bool DefinitionSpace::is_basetype_declared(BaseType* t) const {
    assert(t != nullptr);
    for(BaseType *ot : basetypes.all()) {
        if(t->equals(ot)) return true;
    }
    return false;
}


// -- MISC --
void DefinitionSpace::set_label_prefix(std::string _label_prefix) {
    label_prefix = _label_prefix;
}

std::string DefinitionSpace::get_filepath() const {
    return filepath;
}

void DefinitionSpace::add_include(Include* inc) {
    assert(inc != nullptr);

    //should only be adding includes when parsing
    assert(state == DefinitionSpaceState::Unparsed);

    //make sure we haven't added this include yet
    for(Include* _inc : includes) {
        if(inc->equals(_inc)) {
            return;
        }
    }

    //add the include
    std::string include_path;
    if(inc->is_library_include) {
        // stdlib include
        include_path = stdlib_dir + "/" + inc->path + ".jank";
    }
    else {
        // relative include
        include_path = extract_folder_path(get_filepath()) + inc->path;
    }
    DefinitionSpace *ds = get_definition_space(include_path);

    include_filepaths.push_back(include_path);
    included_definition_spaces.push_back(ds);
    includes.push_back(inc);

    if(inc->is_library_include) {
        ds->set_label_prefix("<" + inc->path + ">");
    }
}

const std::vector<Include*>& DefinitionSpace::get_includes() {
    assert(state >= DefinitionSpaceState::Parsed);
    return includes;
}

const std::vector<DefinitionSpace*>& DefinitionSpace::get_included_definition_spaces() {
    assert(state >= DefinitionSpaceState::Parsed);
    return included_definition_spaces;
}

void DefinitionSpace::add_global_node(GlobalNode* gn) {
    assert(gn != nullptr);

    //should only be adding global nodes when parsing
    assert(state == DefinitionSpaceState::Unparsed);

    global_nodes.push_back(gn);
}

const std::vector<GlobalNode*>& DefinitionSpace::get_global_nodes() {
    assert(state >= DefinitionSpaceState::Parsed);
    return global_nodes;
}

CompilationContext* DefinitionSpace::create_compilation_context() {
    // make sure we've parsed all our includes
    assert(state >= DefinitionSpaceState::IncludesParsed);

    CompilationContext* ctx = new CompilationContext();

    // add self
    ctx->add_definition_space(this, Visibility::All);

    // add all imports with public visibility
    for(DefinitionSpace* ds : included_definition_spaces) {
        ctx->add_definition_space(ds, Visibility::Public);
    }

    // add builtin definition space with public visibility
    ctx->add_definition_space(builtin_definition_space, Visibility::Public);

    return ctx;
}

bool DefinitionSpace::ensure_ready() {
    return ensure_includes_resolved();
}


