#include "PackageGraph.h"
#include "Package.h"
#include "Target.h"

#include <functional>

// if we've already loaded this package, return it
// otherwise try to load it from the filesystem. 
// TODO checks that the new package doesn't contain and isn't contained by any other loaded package
// TODO if we ever make it so that we can configure important directories from config.toml, 
//   should also check that these important directories aren't containing each other
Package* PackageGraph::load_package(const fs::path& package_path) {
    // check if we've already loaded this package
    for(Package* p : packages) {
        if(fs::equivalent(p->path, package_path)) {
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
    toml::table deps;
    try {
        deps = toml::parse_file((package_path / "deps.toml").string());
    } catch(const toml::parse_error& e) {
        throw std::runtime_error(std::string("Failed to load deps.toml : ").append(e.what()));
    }

    // load package metadata
    {
        // name
        std::optional<std::string> _name = config["package"]["name"].value<std::string>();
        if(!_name.has_value()) {
            throw std::runtime_error("package.name missing from config.toml");
        }
        // make sure there isn't another package with the same name
        for(Package* _pack : packages) {
            if(_pack->name == _name.value()) {
                throw std::runtime_error("Duplicate package name : " + pack->name);
            }
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
            throw std::runtime_error("Invalid package.environment: " + environment.value());
        }

        // local include mode
        std::optional<std::string> local_include_mode = config["package"]["local-include-mode"].value<std::string>();
        if(!local_include_mode.has_value()) {
            // default local include mode is absolute
            pack->local_include_mode = Package::LocalIncludeMode::Absolute;
        }
        else if(local_include_mode.value() == "absolute") {
            pack->local_include_mode = Package::LocalIncludeMode::Absolute;
        }
        else if(local_include_mode.value() == "relative") {
            pack->local_include_mode = Package::LocalIncludeMode::Relative;
        }
        else {
            throw std::runtime_error("Invalid package.local-include-mode: " + local_include_mode.value());
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

    auto get_toml_table_field = [](toml::table* t, std::string field) {
        std::optional<std::string> _val = (*t)[field].value<std::string>();
        if(!_val.has_value()) {
            throw std::runtime_error("Missing field : " + field);
        }
        return _val.value();
    };

    // package dependencies
    std::vector<std::string> package_dependencies;
    std::unordered_map<std::string, std::string> alias_map;
    std::unordered_map<std::string, std::string> reverse_alias_map;
    if(config.contains("dependency")) {
        toml::array* _dependencies = config["dependency"].as_array();
        if(!_dependencies) {
            throw std::runtime_error("'dependency' must be an array");
        }
        for(toml::node& node : *_dependencies) {
            toml::table* dependency = node.as_table();
            if(!dependency) {
                throw std::runtime_error("Every element of 'dependency' must be a table");
            }

            // process dependency
            std::string dep_name = get_toml_table_field(dependency, "package");
            std::string alias = dep_name;
            if(dependency->contains("alias")) {
                alias = get_toml_table_field(dependency, "alias");
            }

            // load dependency package
            Package* dep = nullptr;
            if(dependency->contains("path")) {
                // load package from path
                fs::path path = get_toml_table_field(dependency, "path");
                dep = load_package(path);
                if(dep->name != dep_name) {
                    throw std::runtime_error("Dependency name mismatch : " + dep_name + " vs. " + dep->name);
                }
            }
            else {
                // load package from library
                dep = load_package_from_library(dep_name);
                assert(dep->name == dep_name);
            }
            assert(dep != nullptr);

            // ensure that another dependency with the same alias doesn't exist
            if(alias_map.contains(alias)) {
                throw std::runtime_error("Duplicate dependency alias : " + alias + " in package : " + pack->name);
            }

            // ensure that another dependency with the same package doesn't exist
            for(const auto& _dep_name : package_dependencies) {
                if(_dep_name == dep_name) {
                    throw std::runtime_error("Duplicate dependency : " + dep_name + " in package : " + pack->name);
                }
            }
            assert(!reverse_alias_map.contains(dep_name));

            package_dependencies.push_back(dep_name);
            alias_map.insert({alias, dep_name});
            reverse_alias_map.insert({dep_name, alias});
        }
    }
    pack->package_dependencies = package_dependencies;
    pack->alias_map = alias_map;
    pack->reverse_alias_map = reverse_alias_map;

    // targets
    std::vector<Target*> targets;
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

            // process target
            std::string name = get_toml_table_field(target, "name");
            std::string type = get_toml_table_field(target, "type");
            std::string entry = get_toml_table_field(target, "entry");
            std::string output = get_toml_table_field(target, "output");
            std::string runtime_alias = get_toml_table_field(target, "runtime");

            // ensure that another target with the same name doesn't exist
            for(const auto& _target : targets) {
                if(_target->name == name) {
                    throw std::runtime_error("Duplicate target : " + name + " in package : " + pack->name);
                }
            }

            // ensure runtime package is actually a runtime package
            if(!alias_map.contains(runtime_alias)) {
                throw std::runtime_error("Target : " + name + " refers to runtime package : " + runtime_alias + " that isn't in dependencies for package : " + pack->name);
            }
            std::string runtime_name = alias_map[runtime_alias];
            Package* runtime_package = get_package(runtime_name);
            assert(runtime_package != nullptr);
            if(runtime_package->type != Package::Type::Runtime) {
                throw std::runtime_error("Target : " + name + " refers to runtime package : " + runtime_name + " that isn't a runtime package for package : " + pack->name);
            }
            
            Target *_target = new Target();
            _target->name = name;
            _target->type = type;
            _target->entry = entry;
            _target->output = output;
            _target->runtime = runtime_name;
            targets.push_back(_target);        
        }
    }
    pack->targets = targets;

    // default includes
    std::vector<std::pair<std::string, fs::path>> default_includes;
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

            // process default include
            std::string alias = get_toml_table_field(default_include, "package");
            fs::path path = get_toml_table_field(default_include, "path");

            // find package referred to by alias
            if(!alias_map.contains(alias)) {
                throw std::runtime_error("Default include refers to alias that does not exist : " + alias + " in package : " + pack->name);
            }
            std::string dep_name = alias_map[alias];
            Package* dep = get_package(dep_name);
            assert(dep != nullptr);
            assert(dep != pack);

            // make sure path refers to existing source file
            if(path.has_extension()) {
                throw std::runtime_error("Default include path should not have extension : " + path.string());
            }
            fs::path source_abs = dep->src_path / path;
            source_abs = source_abs.replace_extension(".jank");
            if(!fs::exists(source_abs)) {
                throw std::runtime_error("Default include source file does not exist : " + source_abs.string());
            }
            if(!fs::is_regular_file(source_abs)) {
                throw std::runtime_error("Default include source file should be a regular file : " + source_abs.string());
            }

            default_includes.push_back({dep_name, path});
        }
    }
    pack->default_includes = default_includes;

    // source dependencies
    // unknown packages and paths should be able to fail silently here, 
    //   we'll check these when building sources and regenerate if needed
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

    packages.push_back(pack);
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
        for(const std::string& dep_name : pack->package_dependencies) {
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


