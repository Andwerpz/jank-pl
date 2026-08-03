#include "PackageGraph.h"
#include "Package.h"

#include <functional>
#include <unordered_set>

// should parse the given package and add it to the package graph
// returns the parsed package
// expects package_path to be absolute
// does not do semantic checks beyond what's required to parse the package. 
// if the package already exists in the graph, does nothing
// does not parse package dependencies
Package* PackageGraph::parse_package(const fs::path& package_path) {
    // check if we've already parsed this package
    for(Package* p : packages) {
        if(fs::equivalent(p->path, package_path)) {
            assert(p->state >= Package::State::Parsed);
            return p;
        }
    }

    // create package struct
    Package* pack = new Package();
    
    // see if this path exists
    if(!fs::is_directory(package_path)) {
        throw std::runtime_error("Package path is not a directory : " + package_path.string());
    }
    pack->path = package_path;

    // find config.toml
    toml::table config;
    try {
        config = toml::parse_file((package_path / "config.toml").string());
    } catch(const toml::parse_error& e) {
        throw std::runtime_error(std::string("Failed to load config.toml : ").append(e.what()));
    }

    // find deps.toml
    // if it doesn't exist, create it
    toml::table deps;
    const fs::path deps_path = package_path / "deps.toml";
    if(!fs::exists(deps_path)) {
        std::ofstream fout(deps_path);
        if(!fout) {
            throw std::runtime_error("Failed to create deps.toml");
        }
        // empty file
    }
    try {
        deps = toml::parse_file(deps_path.string());
    } catch(const toml::parse_error& e) {
        throw std::runtime_error(std::string("Failed to load deps.toml : ").append(e.what()));
    }

    // find hash.toml
    // if it doesn't exist, create it
    toml::table hash;
    const fs::path hash_path = package_path / "hash.toml";
    if(!fs::exists(hash_path)) {
        std::ofstream fout(hash_path);
        if(!fout) {
            throw std::runtime_error("Failed to create hash.toml");
        }
        // empty file
    }
    try {
        hash = toml::parse_file(hash_path.string());
    } catch(const toml::parse_error& e) {
        throw std::runtime_error(std::string("Failed to load hash.toml : ").append(e.what()));
    }

    // load package metadata
    {
        // name
        std::optional<std::string> _name = config["package"]["name"].value<std::string>();
        if(!_name.has_value()) {
            throw std::runtime_error("package.name missing from config.toml");
        }
        pack->name = _name.value();

        // type
        std::optional<std::string> type = config["package"]["type"].value<std::string>();
        if(!type.has_value()) {
            // default type is library
            pack->type = Package::Type::Library;
        }
        else if(type.value() == "library") {
            pack->type = Package::Type::Library;
        }
        else if(type.value() == "runtime") {
            pack->type = Package::Type::Runtime;
        }
        else {
            throw std::runtime_error("Invalid package.type : " + type.value());
        }

        // environment
        std::optional<std::string> environment = config["package"]["environment"].value<std::string>();
        if(!environment.has_value()) {
            // default environment is hosted
            pack->environment = Package::Environment::Hosted;
        }
        else if(environment.value() == "hosted") {
            pack->environment = Package::Environment::Hosted;
        }
        else if(environment.value() == "freestanding") {
            pack->environment = Package::Environment::Freestanding;
        }
        else {
            throw std::runtime_error("Invalid package.environment : " + environment.value());
        }
    }

    // find /src
    fs::path src_path = package_path / "src";
    if(!fs::exists(src_path)) {
        throw std::runtime_error("Source directory does not exist : " + src_path.string());
    }
    if(!fs::is_directory(src_path)) {
        throw std::runtime_error("Source path is not a directory : " + src_path.string());
    }
    pack->src_path = src_path;

    // find /build
    fs::path build_path = package_path / "build";
    if(!fs::exists(build_path)) {
        try {
            fs::create_directories(build_path);
        } catch(const std::runtime_error& e) {
            throw std::runtime_error("Failed to create build directory : " + build_path.string());
        }
    }
    if(!fs::is_directory(build_path)) {
        throw std::runtime_error("Build path is not a directory : " + build_path.string());
    }
    pack->build_path = build_path;

    // find /tmp
    fs::path tmp_path = package_path / "tmp";
    if(!fs::exists(tmp_path)) {
        try {
            fs::create_directories(tmp_path);
        } catch(const std::runtime_error& e) {
            throw std::runtime_error("Failed to create temp directory : " + tmp_path.string());
        }
    }
    if(!fs::is_directory(tmp_path)) {
        throw std::runtime_error("Temp path is not a directory : " + tmp_path.string());
    }
    pack->tmp_path = tmp_path;

    // find /bin
    fs::path bin_path = package_path / "bin";
    if(!fs::exists(bin_path)) {
        try {
            fs::create_directories(bin_path);
        } catch(const std::runtime_error& e) {
            throw std::runtime_error("Failed to create bin directory : " + bin_path.string());
        }
    }
    if(!fs::is_directory(bin_path)) {
        throw std::runtime_error("Bin path is not a directory : " + bin_path.string());
    }
    pack->bin_path = bin_path;

    // package dependencies
    std::vector<Package::Dependency> dependencies;
    if(config.contains("dependency")) {
        toml::array* _dependencies = config["dependency"].as_array();
        if(!_dependencies) {
            throw std::runtime_error("'dependency' must be an array");
        }
        for(toml::node& node : *_dependencies) {
            toml::table* _dependency = node.as_table();
            if(!_dependency) {
                throw std::runtime_error("Every element of 'dependency' must be a table");
            }

            // parse dependency
            Package::Dependency dependency;
            dependency.package = get_toml_table_field(_dependency, "package");
            dependency.alias = dependency.package;
            if(_dependency->contains("alias")) {
                dependency.alias = get_toml_table_field(_dependency, "alias");
            }
            dependency.path = std::nullopt;
            if(_dependency->contains("path")) {
                dependency.path = get_toml_table_field(_dependency, "path");
            }
            dependency.is_exported = false;
            if(_dependency->contains("export")) {
                dependency.is_exported = get_toml_table_field<bool>(_dependency, "export");
            }

            dependencies.push_back(dependency);
        }
    }
    pack->dependencies = dependencies;

    // targets
    std::vector<Package::Target> targets;
    if(config.contains("target")) {
        toml::array* _targets = config["target"].as_array();
        if(!_targets) {
            throw std::runtime_error("'target' must be an array");
        }
        for(toml::node& node : *_targets) {
            toml::table* target = node.as_table();
            if(!target) {
                throw std::runtime_error("Every element of 'target' must be a table");
            }

            // parse target
            Package::Target tgt;
            tgt.name = get_toml_table_field(target, "name");
            std::string target_type = get_toml_table_field(target, "type");
            if(target_type == "binary") {
                tgt.type = Package::Target::Type::Binary;
            }
            else {
                throw std::runtime_error("Unknown target type : " + target_type + " for target : " + tgt.name + " for package : " + pack->name);
            }
            tgt.entry = get_toml_table_field(target, "entry");
            tgt.output = get_toml_table_field(target, "output");
            tgt.runtime = get_toml_table_field(target, "runtime");    

            targets.push_back(tgt);
        }
    }
    pack->targets = targets;

    // default includes
    std::vector<Package::DefaultInclude> default_includes;
    if(config.contains("default-include")) {
        toml::array* _default_includes = config["default-include"].as_array();
        if(!_default_includes) {
            throw std::runtime_error("'default-include' must be an array");
        }
        for(toml::node& node : *_default_includes) {
            toml::table* default_include = node.as_table();
            if(!default_include) {
                throw std::runtime_error("Every element of 'default-include' must be a table");
            }

            // parse default include
            Package::DefaultInclude inc;
            inc.package = get_toml_table_field(default_include, "package");
            inc.path = get_toml_table_field(default_include, "path");

            default_includes.push_back(inc);
        }
    }
    pack->default_includes = default_includes;

    // source dependencies
    std::unordered_map<fs::path, std::vector<std::pair<std::string, fs::path>>> source_dependencies;
    for(auto &[_source_file, node] : deps) {
        fs::path source_file = _source_file.str();
        const toml::array* dependencies_array = node.as_array();
        if(dependencies_array == nullptr) {
            throw std::runtime_error("Dependencies should be an array of tables : " + pack->name);
        }

        std::vector<std::pair<std::string, fs::path>> dependencies;
        for(const toml::node& node : *dependencies_array) {
            const toml::table* dependency_table = node.as_table();
            if(dependency_table == nullptr) {
                throw std::runtime_error("Each dependency should be a table : " + pack->name);
            }

            // get package name, source path
            const toml::node* package_node = dependency_table->get("package");
            const toml::node* path_node = dependency_table->get("path");
            if (package_node == nullptr || path_node == nullptr) {
                throw std::runtime_error("Dependency is missing 'package' or 'path': " + source_file.string() + " for package : " + pack->name);
            }
            std::optional<std::string> package_name = package_node->value<std::string>();
            std::optional<std::string> dependency_path = path_node->value<std::string>();
            if (!package_name || !dependency_path) {
                throw std::runtime_error("Dependency 'package' and 'path' must be strings: " + source_file.string() + " for package : " + pack->name);
            }
            assert(package_name.has_value());
            assert(dependency_path.has_value());

            dependencies.push_back({package_name.value(), fs::path(*dependency_path).lexically_normal()});
        }
        source_dependencies.insert({source_file, dependencies});
    }
    pack->source_dependencies = source_dependencies;

    // source dependency hashes
    std::unordered_map<fs::path, std::string> dependency_hashes;
    for(auto &[_source_file, node] : hash) {
        fs::path source_file = _source_file.str();
        std::optional<std::string> _hash = node.value<std::string>();
        if(!_hash.has_value()) {
            throw std::runtime_error("Dependency hash should be a string : " + source_file.string() + " for package : " + pack->name);
        }
        std::string hash_str = _hash.value();
        dependency_hashes.insert({source_file.string(), hash_str});
    }
    pack->dependency_hashes = dependency_hashes;

    // make sure there isn't another package with the same name
    for(Package* _pack : packages) {
        if(_pack->name == pack->name) {
            throw std::runtime_error("Duplicate package name : " + pack->name + " at paths : " + pack->path.string() + " and " + _pack->path.string());
        }
    }

    // add package to package graph 
    pack->state = Package::State::Parsed;
    packages.push_back(pack);

    return pack;
}

Package* PackageGraph::parse_package_from_library(const std::string& package_name) {
    std::vector<fs::path> lib_paths = {
        user_library_path,
        // system_library_path,
    };
    for(const fs::path& lib_path : lib_paths) {
        try {
            fs::path package_path = lib_path / package_name;
            return parse_package(package_path);
        } catch(const std::runtime_error& e) {
            // ok, onto the next one. 
        }
    }
    throw std::runtime_error("Failed to parse package : " + package_name + " from library");
}

// should fully resolve a package
//   all alias mappings should be figured out
//   all aliases should be transformed into package names
// all semantic checks should be performed here. 
// if the package is already resolved, does nothing
// expects that the package is already parsed, throws an error if it isn't
// expects that all dependencies of this package are already parsed, throws an error if they aren't
void PackageGraph::resolve_package(const std::string& package_name) {
    // get the package
    Package* pack = get_package(package_name);
    if(pack->state == Package::State::Resolved) {
        return;
    }
    assert(pack->state == Package::State::Parsed);

    // make sure direct dependencies are well formed
    for(Package::Dependency& dependency : pack->dependencies) {
        // dependency should exist
        Package* dep = get_package(dependency.package);
        assert(dep != nullptr);

        // ensure this is not a self dependency
        if(dep == pack) {
            throw std::runtime_error("Explicit self dependency in package : " + pack->name);
        }
        
        // name of dependency should match the name in config
        if(dependency.package != dep->name) {
            throw std::runtime_error("Dependency name does not match config : " + dep->name + " vs. " + dependency.package + " for package : " + pack->name);   
        }

        // if dependency was specified via path, paths should match as well
        if(dependency.path.has_value() && !fs::equivalent(dependency.path.value(), dep->path)) {
            throw std::runtime_error("Dependency path does not match config : " + dep->path.string() + " vs. " + dependency.path.value().string() + " for package : " + pack->name);
        }
    }

    // figure out all packages this one directly depends on
    // this includes implicit dependencies via export
    // this explicitly does not include itself
    std::vector<Package*> package_dependencies;
    std::function<void(Package*)> find_implicit_dependencies = 
    [&package_dependencies, &pack, this, &find_implicit_dependencies](Package* dep) -> void {
        // explicitly exclude the original package
        if(dep == pack) return;

        // see if it already exists
        for(Package* _dep : package_dependencies) {
            if(_dep == dep) return;
        }
        package_dependencies.push_back(dep);

        // add any dependencies exported by dep
        for(Package::Dependency dependency : dep->dependencies) {
            if(!dependency.is_exported) continue;
            Package* exported_dep = this->get_package(dependency.package);
            find_implicit_dependencies(exported_dep);
        }
    };
    for(Package::Dependency& dependency : pack->dependencies) {
        Package* dep = get_package(dependency.package);
        find_implicit_dependencies(dep);
    }

    // make sure all direct dependencies are well formed
    std::vector<std::string> resolved_dependencies;
    for(Package* dep : package_dependencies) {
        // if we are freestanding, ensure that we aren't depending on a hosted package
        if(pack->environment == Package::Environment::Freestanding && dep->environment == Package::Environment::Hosted) {
            throw std::runtime_error("Freestanding package : " + pack->name + " cannot depend on hosted package : " + dep->name);
        }

        resolved_dependencies.push_back(dep->name);
    }
    pack->resolved_dependencies = resolved_dependencies;

    // figure out all alias mappings
    // this is just the union of all aliases defined by this package
    //   plus all the aliases defined by its dependencies
    // for now, we enforce a 1:1 alias to package mapping
    //   maybe can lift the restriction to allow multiple aliases to refer to the same package
    std::unordered_map<std::string, std::string> alias_map;
    std::unordered_map<std::string, std::string> reverse_alias_map;
    {
        // {alias, package name}
        std::vector<std::pair<std::string, std::string>> aliases;

        // aliases defined by this package
        for(Package::Dependency dependency : pack->dependencies) {
            aliases.push_back({dependency.alias, dependency.package});
        }

        // aliases exported by immediate dependencies
        for(Package* dep : package_dependencies) {
            for(Package::Dependency dependency : dep->dependencies) {
                if(!dependency.is_exported) continue;
                aliases.push_back({dependency.alias, dependency.package});
            }
        }

        for(const auto &[alias, name] : aliases) {
            if(alias_map.contains(alias)) {
                throw std::runtime_error("Duplicate alias : " + alias + " for package : " + pack->name);
            }
            if(reverse_alias_map.contains(name)) {
                throw std::runtime_error("Multiple aliases for one package : " + name + " for package : " + pack->name);
            }
            alias_map.insert({alias, name});
            reverse_alias_map.insert({name, alias});
        }
    }
    pack->alias_map = alias_map;
    pack->reverse_alias_map = reverse_alias_map;

    auto resolve_alias = [&alias_map, &pack](const std::string& alias) -> std::string {
        if(!alias_map.contains(alias)) {
            throw std::runtime_error("Unknown alias : " + alias + " for package : " + pack->name);
        }
        return alias_map[alias];
    };

    // resolve aliases in targets
    // make sure targets are well formed
    std::unordered_set<std::string> target_names;
    for(Package::Target& target : pack->targets) {
        // resolve aliases
        target.runtime = resolve_alias(target.runtime);

        // ensure that another target with the same name doesn't exist
        if(target_names.contains(target.name)) {
            throw std::runtime_error("Duplicate target name : " + target.name + " for package : " + pack->name);
        }
        target_names.insert(target.name);

        // ensure runtime package is actually a runtime package
        Package* runtime_package = get_package(target.runtime);
        assert(runtime_package != nullptr);
        if(runtime_package->type != Package::Type::Runtime) {
            throw std::runtime_error("Target : " + target.name + " refers to runtime package : " + target.runtime + " that isn't a runtime package for package : " + pack->name);
        }
    }
    
    // resolve aliases in default includes
    // make sure default includes are well formed
    for(Package::DefaultInclude& inc : pack->default_includes) {
        // resolve aliases
        inc.package = resolve_alias(inc.package);

        // make sure package referred to exists, and is not 
        Package* dep = get_package(inc.package);
        assert(dep != nullptr);

        // make sure this isn't referring to itself
        if(dep == pack) {
            throw std::runtime_error("Explicit self dependency in default include : " + pack->name);
        }

        // make sure this include refers to an existing source file
        if(inc.path.has_extension()) {
            throw std::runtime_error("Default include path should not have extension : " + dep->name + " : " + inc.path.string() + " for package : " + pack->name);
        }
        fs::path source_abs = dep->src_path / inc.path;
        source_abs = source_abs.replace_extension(".jank");
        if(!fs::exists(source_abs)) {
            throw std::runtime_error("Default include source file does not exist : " + dep->name + " : " + inc.path.string() + " for package : " + pack->name);
        }
        if(!fs::is_regular_file(source_abs)) {
            throw std::runtime_error("Default include should be a regular file : " + dep->name + " : " + inc.path.string() + " for package : " + pack->name);
        }
    }

    pack->state = Package::State::Resolved;
}

// if we've already loaded this package, return it
// otherwise parses it and all of its dependencies, and resolves them. 
Package* PackageGraph::load_package(const fs::path& package_path) {
    // check if we've already loaded this package
    for(Package* p : packages) {
        if(fs::equivalent(p->path, package_path)) {
            return p;
        }
    }

    // parse the package and all of its dependencies
    std::vector<std::string> all_packages;
    std::function<void(Package*)> parse_dependencies = [&all_packages, this, &parse_dependencies](Package* pack) -> void {
        // see if we already parsed the dependencies of this package
        for(const std::string& package_name : all_packages) {
            if(package_name == pack->name) return;
        }

        // parse dependencies
        all_packages.push_back(pack->name);
        for(Package::Dependency& dependency : pack->dependencies) {
            if(dependency.path.has_value()) {
                parse_dependencies(this->parse_package(dependency.path.value()));
            }
            else {
                parse_dependencies(this->parse_package_from_library(dependency.package));
            }
        }
    };
    Package* pack = parse_package(package_path);
    parse_dependencies(pack);

    // resolve the package and all of its dependencies
    for(std::string package_name : all_packages) {
        resolve_package(package_name);
    }

    return pack;
}

// looks inside system and user package library for a package of this name
// if both system and user libraries have the package installed, prefer the user one
Package* PackageGraph::load_package_from_library(const std::string& package_name) {
    std::vector<fs::path> lib_paths = {
        user_library_path,
        // system_library_path,
    };
    for(const fs::path& lib_path : lib_paths) {
        try {
            fs::path package_path = lib_path / package_name;
            return load_package(package_path);
        } catch(const std::runtime_error& e) {
            // ok, onto the next one. 
        }
    }
    throw std::runtime_error("Failed to load package : " + package_name + " from library");
}

// retrieves the package with the given name
Package* PackageGraph::get_package(const std::string& name) {
    for(Package* pack : packages) {
        if(pack->name == name) {
            return pack;
        }
    }
    throw std::runtime_error("Failed to find package with name : " + name);
}

// figures out which package owns the provided filepath
// the provided filepath is assumed to be absolute
// looks at all loaded packages
Package* PackageGraph::find_owner(const fs::path& filepath) {
    for(Package* pack : packages) {
        if(is_ancestor(pack->path, filepath)) {
            return pack;
        }
    }
    throw std::runtime_error("Failed to find owning package for : " + filepath.string());
} 

// retrieves the list of dependencies of this file
// filepath is relative to pack->src_path
// if there is not a list of dependencies for this file, returns std::nullopt
// otherwise returns a list of {package, path} pairs where 
//   path is relative to the corresponding package source filepath
std::optional<std::vector<std::pair<std::string, fs::path>>> PackageGraph::get_source_dependencies(const std::string& package_name, const fs::path& source_file) {
    Package* pack = get_package(package_name);
    if(pack->source_dependencies.contains(source_file)) {
        return pack->source_dependencies[source_file];
    }
    else {
        return std::nullopt;
    }
}

// retrieves the list of all packages that this package directly or indirectly depends on
// this list includes itself
std::vector<Package*> PackageGraph::get_package_dependencies(const std::string& package_name) {
    std::vector<std::string> package_names;
    std::function<void(std::string)> find_packages = [&package_names, this, &find_packages](const std::string& package_name) {
        for(const std::string& _package_name : package_names) {
            if(_package_name == package_name) return;
        }
        package_names.push_back(package_name);
        Package* pack = this->get_package(package_name);
        for(const std::string& dep_name : pack->resolved_dependencies) {
            find_packages(dep_name);
        }
    };
    find_packages(package_name);
    std::vector<Package*> packages;
    for(const std::string& _package_name : package_names) {
        packages.push_back(get_package(_package_name));
    }
    return packages;
}

// computes the source dependency hash of the given source file. 
// the source dependency hash is a function of 
// - contents of package config.toml
// - contents of source file + relative path
// - contents of all source files this one depends on + relative paths
// this method does not set the dependency hash in the package after computing it
// if the source dependencies for the given file are unavailable, this method will throw an error
// if some package or source file is missing, this method will throw an error
std::string PackageGraph::compute_source_dependency_hash(const std::string& package_name, const fs::path& filepath) {
    std::optional<std::vector<std::pair<std::string, fs::path>>> _source_dependencies = get_source_dependencies(package_name, filepath);
    if(!_source_dependencies.has_value()) {
        throw std::runtime_error("Compute source dependency hash expects that source dependencies for file are available : " + package_name + " " + filepath.string());
    }
    std::vector<std::pair<std::string, fs::path>> source_dependencies = _source_dependencies.value();

    // sort dependencies to make hash deterministic
    std::sort(source_dependencies.begin(), source_dependencies.end(), [](auto& a, auto& b) -> bool {
        if(a.first != b.first) {
            return a.first < b.first;
        }
        if(a.second.string() != b.second.string()) {
            return a.second.string() < b.second.string();
        }
        throw std::runtime_error("There should not be two equal entries in source dependencies");
    });

    Sha256Hash hash{};

    // hash config.toml
    Package* pack = get_package(package_name);
    assert(pack != nullptr);
    const fs::path config_toml_path = pack->path / "config.toml";
    hash = combine_hashes(hash, sha256_file(config_toml_path));

    // hash source dependencies
    // this should include the source file itself
    for(auto &[_package_name, source_file] : source_dependencies) {
        Package *pack = get_package(_package_name);
        assert(pack != nullptr);
        const fs::path source_file_abs = pack->src_path / source_file;
        hash = combine_hashes(hash, sha256(source_file.string()));
        hash = combine_hashes(hash, sha256_file(source_file_abs));
    }

    return hash_to_hex(hash);
}


