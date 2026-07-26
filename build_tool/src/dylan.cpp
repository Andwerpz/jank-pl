#include "dylan.h"

// tool for managing packages
// a package is expected to look like this:
// package_root/
//   config.toml
//   deps.toml
//   src/
//     <source files here>
//   build/
//     <build artifacts here>
//   tmp/
//     <tmp files here>
//   bin/
//     <build results here>

// what should this tool be able to do?
// - build some target (build)
// - run executable generated from a target (run)
// - delete generated files (clean)
// - run tests? (test)
//   - how would this work, what are the responsibilities of the build tool vs test runner
// - list all targets (targets)
// - print cool statistics about the package 
//   - dependency graph (deps)
//   - lines of code (loc)
//   - file compile times

// structure of config.toml:
/*
# package metadata
[package]
name = "jank-compiler"            # package name

# targets
# each target should specify some sort of thing to build
[[target]]
name = "compiler"
type = "binary"
entry = "jjc.jank"  # relative to source path
output = "jjc"      # relative to bin path

# dependencies 
# each dependency is assumed to be another jank package
# TODO have packages be installed at a known location like /.jank and look for packages there
#   if the user doesn't specify a path
[[dependency]]
name = "jank-stdlib"                    # should match with the name of the package
alias = "std"                           # optional, if omitted will just be the name
path = "/home/steven/jank-pl/stdlib"    

# default includes
# useful for automatically including stuff like <std::malloc>, <std::syscall>, 
#   or <std::defs> in every source file
[[default-include]]
package = "std"     # alias of the package you want to include
path = "malloc"     # relative to package source directory and excluding extension

# TODO think about package local default includes?
#   maybe can omit the 'alias' field?
#   but I still want to store an alias because jjc takes in an alias

# TODO also be able to specify compilation args for each individual file or all files?
*/

// structure of deps.toml
/*
# for each source file, should have an array of tables giving you 
# - package where the source file comes from
# - path of source file inside package
"jjc.jank" = [
    { package = "jank-compiler", path = "semantics/utils.jank" },
    { package = "jank-stdlib", path = "vector.jank" },
]
*/

// incremental compilation notes
// - the build tool should query the compiler to figure out all (direct and indirect) dependencies for each file
// - when a file changes, its dependencies can change.
// - if a source file is older than its artifact, then we consider it 'unchanged', otherwise it's 'changed'. 
// - when building, have set of 'changed' files. 
// - should recompile any file that is changed, or has a dependency that is changed. 
//   - is this correct? the goal is to compile exactly the set of files that need to be compiled. 
//   - failure mode 1: recompile a file that doesn't need to be compiled. 
//     - don't care about this for now. 
//   - failure mode 2: didn't recompile a file that needs to be compiled. 
//     - if we didn't recompile a file, that means that it and its dependencies are not changed. 
//     - suppose that A depends only on B and B depends only on C. If C changes then should we recompile A? 
//       my argument is that if we needed to recompile A based off of a change in C, then it should've been a dependency of A. 
// - what i need to be true: changing any file that is not a dependency of A should not cause A to need to recompile. 
//   - this should already be true. When compiling A, jjc looks for exactly the set of files needed to compile A. 
//     if you modify some file not included in this set, the compiler will never notice because it will never actually find
//     it while compiling A. 
//   - so the main improvements I can make to this system is making the compiler smarter so that it needs to load less files 
//     when compiling some file A. 

// executes the given executable with the given arguments
// returns the status code
// if redirect_stdout is true, then redirects STDOUT to /dev/null
int exec(fs::path bin_path, std::vector<std::string> args, bool redirect_stdout) {
    pid_t pid = fork();
    if(pid == 0) {
        if(redirect_stdout) {
            // redirect output to trash
            int dev_null = open("/dev/null", O_WRONLY);
            if (dev_null == -1) {
                perror("open /dev/null failed");
                _exit(1);
            }
            if (dup2(dev_null, STDOUT_FILENO) == -1) {
                perror("dup2 failed");
                close(dev_null);
                _exit(1);
            }
            close(dev_null);
        }

        std::vector<char*> argv;
        argv.reserve(args.size() + 1);
        for (std::string& arg : args) {
            argv.push_back(arg.data());
        }
        argv.push_back(nullptr);

        execvp(bin_path.string().c_str(), argv.data());
        perror("compiler exec failed");
        _exit(1);
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            return 1;
        }
    }
    return 0;
}

// compiles the file at src_path into out_path
// assumes src_path and out_path are absolute
// returns 0 on success, nonzero on failure
int compile(fs::path src_path, fs::path out_path, std::vector<std::string> args) {
    std::vector<std::string> command_args = {
        "jjc",
        src_path,
        "-o",
        out_path
    };
    command_args.insert(command_args.end(), args.begin(), args.end());

    std::cout << "Compiling : " << src_path.string() << std::endl;
    return exec("jjc", command_args, true);
}

// links and assembles the given assembly files into out_path
// assumes all paths in asm_paths and out_path are absolute
// returns 0 on success, nonzero on failure
int assemble(std::vector<fs::path> asm_paths, fs::path out_path) {
    std::vector<std::string> command_args = {
        "gcc",
        "-g",                           // debug metadata, should probably be optional
        "-x", "assembler",              //gcc expects .s files to be assembly, tell it that all files are assembly
        "-nostartfiles", "-nostdlib",   //tell gcc that we're not compiling C assembly
        "-m64"                          //64 bit mode?
    };
    command_args.insert(command_args.end(), asm_paths.begin(), asm_paths.end());
    command_args.push_back("-o");
    command_args.push_back(out_path);

    std::cout << "Assembling : " << out_path.string() << std::endl;
    return exec("gcc", command_args, false);
}

// writes the provided toml file to the given path
// assumes that the path doesn't correspond to an existing directory
// assumes that the path is an absolute path
void write_toml(const toml::table& toml, const fs::path path) {
    if(fs::is_directory(path)) {
        throw std::runtime_error("Tried to write toml to directory : " + path.string());
    }
    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("Failed to open file : " + path.string());
    }
    output << toml;
    if (!output) {
        throw std::runtime_error("failed to write toml to file : " + path.string());
    }
    output.close();
}

// returns true if root is an ancestor of target
// use to tell if a directory contains a file
bool is_ancestor(const fs::path& root, const fs::path& target) {
    // normalize paths to resolve "." and ".." components lexically
    auto canon_root = root.lexically_normal();
    auto canon_target = target.lexically_normal();

    // compare path elements
    // don't want to take a string prefix because something like
    //   "/home/bla", "/home/bla.txt"
    // would then be considered to be containing
    auto [target_it, prefix_it] = std::mismatch(
        canon_target.begin(), canon_target.end(),
        canon_root.begin(), canon_root.end()
    );
    
    // if we reached the end of the root, it is an ancestor of target
    return prefix_it == canon_root.end();
}

// assumes that root is an ancestor of target
// returns the relative path from root to target
fs::path get_relative_path(const fs::path& root, const fs::path& target) {
    auto canon_root = root.lexically_normal();
    auto canon_target = target.lexically_normal();

    assert(is_ancestor(canon_root, canon_target));
    fs::path rel_path = fs::path(canon_target).lexically_relative(canon_root);
    return rel_path;
}

// if we can already find a package in all_packages, return it. 
// otherwise try to load it from the filesystem. 
// this checks that the new package doesn't contain and isn't contained by any other loaded package
// TODO if we ever make it so that we can configure important directories from config.toml, 
//   should also check that these important directories aren't containing each other
package* load_package(fs::path package_path) {
    // check if we've already loaded this package
    for(package* p : all_packages) {
        if(fs::equivalent(p->path, package_path)) {
            return p;
        }
    }

    // create package struct
    package* pack = new package();
    
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
    pack->config = config;

    // find deps.toml
    toml::table deps;
    try {
        deps = toml::parse_file((package_path / "deps.toml").string());
    } catch(const toml::parse_error& e) {
        throw std::runtime_error(std::string("Failed to load deps.toml : ").append(e.what()));
    }
    pack->deps = deps;

    // load package name
    std::optional<std::string> _package_name = config["package"]["name"].value<std::string>();
    if(!_package_name.has_value()) {
        throw std::runtime_error("package.name missing from config.toml");
    }
    // make sure there isn't another package with the same name
    for(package* _pack : all_packages) {
        if(_pack->name == _package_name.value()) {
            throw std::runtime_error("Duplicate package name : " + pack->name);
        }
    }
    pack->name = _package_name.value();

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
            throw std::runtime_error("Dependency missing field : " + field);
        }
        return _val.value();
    };

    // targets
    std::vector<target> targets;
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

            // ensure that another target with the same name doesn't exist
            for(const auto& _target : targets) {
                if(_target.name == name) {
                    throw std::runtime_error("Duplicate target : " + name + " in package : " + pack->name);
                }
            }

            targets.push_back({name, type, entry, output});        
        }
    }
    pack->targets = targets;

    // dependencies
    std::vector<std::pair<std::string, package*>> dependencies;
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
            std::string name = get_toml_table_field(dependency, "name");
            fs::path path = get_toml_table_field(dependency, "path");
            std::string alias = name;
            if(dependency->contains("alias")) {
                alias = get_toml_table_field(dependency, "alias");
            }

            // load dependency package
            package* dep = load_package(path);
            assert(dep != nullptr);
            if(dep->name != name) {
                throw std::runtime_error("Dependency name mismatch : " + name + " vs. " + dep->name);
            }

            // ensure that another dependency with the same alias doesn't exist
            // ensure that another dependency with the same package doesn't exist
            for(const auto &[_alias, _dep] : dependencies) {
                if(_alias == alias) {
                    throw std::runtime_error("Duplicate dependency alias : " + alias + " in package : " + pack->name);
                }
                if(_dep == dep) {
                    throw std::runtime_error("Duplicate dependency : " + name + " in package : " + pack->name);
                }
            }

            dependencies.push_back({alias, dep});
        }
    }
    pack->dependencies = dependencies;

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
            package* dep = nullptr;
            for(const auto&[_alias, _dep] : dependencies) {
                if(_alias == alias) {
                    assert(dep == nullptr);
                    dep = _dep;
                }
            }
            if(dep == nullptr) {
                throw std::runtime_error("Default include refers to unknown package : " + alias);
            }
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

            default_includes.push_back({alias, path});
        }
    }
    pack->default_includes = default_includes;

    all_packages.push_back(pack);
    return pack;
}

package* get_package(std::string name) {
    for(package* pack : all_packages) {
        if(pack->name == name) {
            return pack;
        }
    }
    throw std::runtime_error("Failed to find package with name : " + name);
}

package* get_package(package* pack, std::string alias) {
    for(const auto &[_alias, dep] : pack->dependencies) {
        if(_alias == alias) {
            return dep;
        }
    }
    throw std::runtime_error("Failed to find dependency of package : " + pack->name + " with alias : " + alias);
}

target get_target(package* pack, std::string target_name) {
    for(const auto& target : pack->targets) {
        if(target.name == target_name) {
            return target;
        }
    }
    throw std::runtime_error("Failed to find target with name : " + target_name);
}

// creates all the commandline arguments needed to inform the compiler of the 
//   currently loaded package graph
std::vector<std::string> generate_compiler_package_args(package* current_pack) {
    std::vector<std::string> res;

    // register all packages
    for(package* pack : all_packages) {
        res.push_back("--package");
        res.push_back(pack->name);
        res.push_back(pack->src_path);
    }

    // register all package dependencies
    for(package* pack : all_packages) {
        for(auto &[alias, dep] : pack->dependencies) {
            res.push_back("--package-dependency");
            res.push_back(pack->name);
            res.push_back(alias);
            res.push_back(dep->name);
        }
    }

    // register all default includes
    for(package* pack : all_packages) {
        for(const auto&[alias, path] : pack->default_includes) {
            res.push_back("--package-default-include");
            res.push_back(pack->name);
            res.push_back(alias);
            res.push_back(path.string());
        }
    }

    // set current package
    res.push_back("--current-package");
    res.push_back(current_pack->name);

    return res;
}

// retrieves the list of dependencies of this file
// filepath is relative to pack->src_path
// if there is not a list of dependencies for this file, returns std::nullopt
// otherwise returns a list of {package, path} pairs where 
//   path is relative to the corresponding package source filepath
std::optional<std::vector<std::pair<package*, fs::path>>> get_dependencies(package* pack, fs::path filepath) {
    const std::string filepath_str = filepath.lexically_normal().string();
    const toml::node* dependencies_node = pack->deps.get(filepath_str);
    if(dependencies_node == nullptr) {
        // don't have list of dependencies for this file
        return std::nullopt;
    }

    const toml::array* dependencies_array = dependencies_node->as_array();
    if(dependencies_array == nullptr) {
        throw std::runtime_error("Dependencies should be an array of tables: " + filepath.string());
    }

    std::vector<std::pair<package*, fs::path>> dependencies;
    dependencies.reserve(dependencies_array->size());
    for(const toml::node& node : *dependencies_array) {
        const toml::table* dependency_table = node.as_table();
        if(dependency_table == nullptr) {
            throw std::runtime_error("Each dependency should be a table: " + filepath.string());
        }

        const toml::node* package_node = dependency_table->get("package");
        const toml::node* path_node = dependency_table->get("path");
        if (package_node == nullptr || path_node == nullptr) {
            throw std::runtime_error("Dependency is missing 'package' or 'path': " + filepath.string());
        }

        std::optional<std::string> package_name = package_node->value<std::string>();
        std::optional<std::string> dependency_path = path_node->value<std::string>();
        if (!package_name || !dependency_path) {
            throw std::runtime_error("Dependency 'package' and 'path' must be strings: " + filepath.string());
        }

        package* dependency_package = get_package(*package_name);
        if (dependency_package == nullptr) {
            throw std::runtime_error("Unknown dependency package '" + package_name.value() + "' referenced by " + filepath.string());
        }

        dependencies.push_back({dependency_package, fs::path(*dependency_path).lexically_normal()});
    }
    return dependencies;
}

// figures out which package owns the provided filepath
// the provided filepath is assumed to be absolute
// looks at all loaded packages
package* find_owner(fs::path filepath) {
    for(package* pack : all_packages) {
        if(is_ancestor(pack->path, filepath)) {
            return pack;
        }
    }
    throw std::runtime_error("Failed to find owning package for : " + filepath.string());
}

// finds all files under pack->src_path that have the extension `.jank`
// returned files are relative to pack->src_path
std::vector<fs::path> find_all_source_files(const package* pack) {
    assert(fs::exists(pack->src_path) && fs::is_directory(pack->src_path));

    std::vector<fs::path> source_files;
    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(pack->src_path)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (entry.path().extension() != ".jank") {
            continue;
        }
        source_files.push_back(fs::relative(entry.path(), pack->src_path));
    }
    std::sort(source_files.begin(), source_files.end());

    return source_files;
}

// removes all build artifacts
void clean(const package* pack) {
    auto clean_dir = [](fs::path dir) {
        assert(fs::exists(dir) && fs::is_directory(dir));
        fs::remove_all(dir);
        fs::create_directories(dir);
    };

    clean_dir(pack->build_path);
    clean_dir(pack->tmp_path);
    clean_dir(pack->bin_path);
}

// ensures all source files in the package are compiled
// only recompiles files within the given package
//   recompiling files from dependency packages is handled elsewhere
void build_sources(package* pack) {
    std::cout << "Building sources : " << pack->name << std::endl;

    // find all source files in package source dir
    std::vector<fs::path> source_files = find_all_source_files(pack);

    // check which source files have changed
    // a source file changed if it has been modified after the last time its corresponding 
    //   artifact has been modified. 
    std::vector<fs::path> changed_files;
    for(const fs::path& source_file : source_files) {
        const fs::path source_abs = pack->src_path / source_file;
        assert(fs::exists(source_abs));
        fs::path build_abs = pack->build_path / source_file;
        build_abs.replace_extension(".s");

        bool changed = false;
        if(!fs::exists(build_abs)) {
            changed = true;
        }
        else if(fs::last_write_time(source_abs) > fs::last_write_time(build_abs)) {
            changed = true;
        }

        if(changed) {
            changed_files.push_back(source_file);
        }
    }

    // figure out which source files need to be recompiled
    // a source file needs to be recompiled if it or any of its dependencies have changed
    // also should compile any source file that doesn't have an entry in deps. 
    std::vector<fs::path> to_recompile;
    for(const fs::path& source_file : source_files) {
        bool should_recompile = false;

        // should recompile if you have been changed. 
        for(int i = 0; i < changed_files.size(); i++) {
            if(fs::equivalent(pack->src_path / source_file, pack->src_path / changed_files[i])) {
                should_recompile = true;
            }
        }

        // should recompile if any of your dependencies have been changed
        // or if you don't have a dependencies entry
        std::optional<std::vector<std::pair<package*, fs::path>>> dependencies = get_dependencies(pack, source_file);
        if(dependencies.has_value()) {
            for(auto &[dep, dep_path] : dependencies.value()) {
                // see if this dependency is from the current package
                if(dep != pack) {
                    continue;
                }

                // check if it's changed
                for(const fs::path changed : changed_files) {
                    if(dep_path == changed) {
                        should_recompile = true;
                    }
                }
            }
        }
        else {
            should_recompile = true;
        }
        
        if(should_recompile) {
            to_recompile.push_back(source_file);
        }
    }

    // recompile, update deps
    for(const fs::path& source_file : to_recompile) {
        const fs::path source_abs = pack->src_path / source_file;
        fs::path build_abs = pack->build_path / source_file;
        const fs::path deps_abs = pack->tmp_path / "deps.tmp";
        assert(fs::exists(source_abs));
        build_abs = build_abs.replace_extension(".s");

        std::vector<std::string> compile_args = generate_compiler_package_args(pack);
        compile_args.push_back("-S");
        compile_args.push_back("--emit-dependencies");
        compile_args.push_back(deps_abs);

        if(compile(source_abs, build_abs, compile_args)) {
            throw std::runtime_error("Failed to compile : " + source_file.string());
        }

        // read deps file
        std::ifstream input(deps_abs);
        if(!input) {
            throw std::runtime_error("Failed to open dependency file: " + deps_abs.string());
        }

        toml::array dependencies;
        std::string package_name;
        std::string dependency_path;
        while(std::getline(input, package_name)) {
            if (package_name.empty()) {
                continue;
            }
            if(!std::getline(input, dependency_path)) {
                throw std::runtime_error("Dependency package '" + package_name + "' is missing its path in: " + deps_abs.string());
            }
            if (dependency_path.empty()) {
                throw std::runtime_error("Dependency package '" + package_name + "' has an empty path in: " + deps_abs.string());
            }

            // convert dependency path to be relative to package src path
            package* dep = get_package(package_name);
            assert(dep != nullptr);
            fs::path dependency_rel = get_relative_path(dep->src_path, dependency_path);

            toml::table dependency{
                { "package", package_name },
                { "path", dependency_rel.string() }
            };
            dependency.is_inline(true);
            dependencies.push_back(std::move(dependency));
        }
        pack->deps.insert_or_assign(source_file.lexically_normal().string(), std::move(dependencies));
    }
    if(to_recompile.size() == 0) {
        std::cout << "Everything up to date :D" << std::endl;
    }

    // write deps to deps.toml
    write_toml(pack->deps, pack->path / "deps.toml");
}

// ensures all dependencies of the package are built
// this includes transitive dependencies
// TODO decide if we even care about circular package dependencies
void build_dependencies(package* pack) {
    std::vector<package*> all_dependencies;
    std::function<void(package*)> find_deps = [&](package* p) {
        for(package* dep : all_dependencies) {
            if(dep == p) return;
        }
        all_dependencies.push_back(p);
        for(auto &[alias, ndep] : p->dependencies) {
            find_deps(ndep);
        }
    };
    find_deps(pack);
    for(package* dep : all_dependencies) {
        build_sources(dep);
    }
}

// builds the provided target
void build_target(package* pack, const target& tgt) {
    // ensure everything is built
    build_dependencies(pack);

    // build 
    if(tgt.type == "binary") {
        // create driver code
        const fs::path entry_abs = pack->src_path / tgt.entry;
        const fs::path driver_abs = pack->tmp_path / "driver.tmp";
        std::vector<std::string> compile_args = generate_compiler_package_args(pack);
        compile_args.push_back("-S");
        compile_args.push_back("--startup-only");
        if(compile(entry_abs, driver_abs, compile_args)) {
            throw std::runtime_error("Failed to create driver code : " + entry_abs.string());
        }

        // recursively find all required source files for this target
        std::vector<std::pair<package*, fs::path>> source_files;    // {package, relative path}
        std::function<void(package*, fs::path)> find_files = [&source_files, &find_files](package* pack, fs::path filepath) {
            // see if we've already found this
            for(auto &[_pack, _filepath] : source_files) {
                if(_pack == pack && _filepath == filepath) {
                    return;
                }
            }

            // add to source files
            source_files.push_back({pack, filepath});

            // find all source files this one depends on
            std::optional<std::vector<std::pair<package*, fs::path>>> dependencies = get_dependencies(pack, filepath);
            assert(dependencies.has_value());
            for(auto &[dep, path] : dependencies.value()) {
                find_files(dep, path);
            }
        };
        find_files(pack, tgt.entry);

        // locate corresponding assembly files
        std::vector<fs::path> asm_files;
        asm_files.push_back(driver_abs);
        for(auto &[pack, source_file] : source_files) {
            fs::path asm_file_rel = source_file;
            asm_file_rel.replace_extension(".s");
            asm_files.push_back(pack->build_path / asm_file_rel);
        }

        // make sure output directory exists
        const fs::path output_abs = pack->bin_path / tgt.output;
        fs::path output_dir = output_abs;
        output_dir.remove_filename();
        fs::create_directories(output_dir);

        // assemble
        if(assemble(asm_files, output_abs)) {
            throw std::runtime_error("Failed to assemble");
        }
    }
    else {
        throw std::runtime_error("Unknown target type : " + tgt.type);
    }
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "USAGE : dylan\n";
        std::cout << "new <package_path> : creates a new package\n";
        std::cout << "compile : ensures all source files are compiled\n";
        std::cout << "build <target> : builds the target\n";
        std::cout << "run <target> : builds and runs the executable generated by the target\n";
        std::cout << "clean : removes all artifacts from the build directory\n";
        return 1;
    }
    int argptr = 1;
    std::string mode = argv[argptr ++];

    // find CWD
    {
        // just make package root CWD for now
        std::string cwd;
        char _cwd_path[PATH_MAX];
        if (getcwd(_cwd_path, sizeof(_cwd_path)) != NULL) {
            cwd = std::string(_cwd_path);
        }
        else {
            std::cout << "Could not find CWD\n";
            return 1;
        }
        cwd_path = fs::path(cwd);
    }

    // special case for new packages
    if(mode == "new") {
        if(argc != 3) {
            std::cout << "USAGE : dylan new <package_path>\n";
            return 1;
        }
        fs::path package_path = cwd_path / argv[argptr ++];
        package_path = package_path.lexically_normal();
        std::string package_name = package_path.filename().string();
        
        // make sure package name fits [a-zA-Z_-]+
        if(package_name.size() == 0) {
            std::cout << "Invalid package name : package name must be non-empty\n";
            return 1;
        }
        for(char c : package_name) {
            if('A' <= c && c <= 'Z') continue;
            if('a' <= c && c <= 'z') continue;
            if(c == '_' || c == '-') continue;
            std::cout << "Invalid package name : invalid character : " << c << "\n";
            return 1;
        }
        // see if there is anything existing at this path
        if(fs::exists(package_path)) {
            std::cout << "Something already exists at " << package_path.string() << "\n";
            return 1;
        }
        // the path must refer to a directory
        if(package_path.has_extension()) {
            std::cout << "Package path must not have an extension\n";
            return 1;
        }
        
        std::cout << "Creating package \"" << package_name << "\" at " << package_path.string() << std::endl;
        try {
            // make package folder
            fs::create_directories(package_path);

            // make config.toml
            {
                std::ofstream config(package_path / "config.toml");
                if (!config) {
                    throw std::runtime_error("Failed to create config.toml");
                }
                config
                    << "[package]\n"
                    << "name = \"" << package_name << "\"\n"
                    << "\n";

                // add jank-stdlib as a dependency  
                fs::path stdlib_path = "/home/steven/jank-pl/stdlib";
                config
                    << "[[dependency]]\n"
                    << "name = \"jank-stdlib\"\n"
                    << "alias = \"std\"\n"
                    << "path = \"" << stdlib_path.string() << "\"\n"
                    << "\n";

                // add default includes
                // - <std::memory>
                // - <std::error>
                // - <std::defs>
                // - <std::syscall>
                // - <std::malloc>
                auto add_default_include = [&config](std::string path) {
                    config
                        << "[[default-include]]\n"
                        << "package = \"std\"\n"
                        << "path = \"" << path << "\"\n"
                        << "\n";
                };
                add_default_include("memory");
                add_default_include("error");
                add_default_include("defs");
                add_default_include("syscall");
                add_default_include("malloc");

                // add target : main.jank -> main
                config
                    << "[[target]]\n"
                    << "name = \"main\"\n"
                    << "type = \"binary\"\n"
                    << "entry = \"main.jank\"\n"
                    << "output = \"main\"\n"
                    << "\n";
            }

            // make deps.toml
            {
                std::ofstream deps(package_path / "deps.toml");
                if (!deps) {
                    throw std::runtime_error("Failed to create deps.toml");
                }
                // empty file
            }

            // make directories
            fs::create_directories(package_path / "src");
            fs::create_directories(package_path / "build");
            fs::create_directories(package_path / "tmp");
            fs::create_directories(package_path / "bin");

            // create main.jank
            {
                std::ofstream main_file(package_path / "src" / "main.jank");
                if (!main_file) {
                    throw std::runtime_error("Failed to create src/main.jank");
                }
                main_file << 
R"(#include <iostream>;

i32 main() {
    cout << "bello\n";
    return 0;
}
)";
            }

            std::cout << "Package created successfully\n";
            return 0;
        }
        catch (const fs::filesystem_error& error) {
            std::cerr << "Filesystem error : " << error.what() << '\n';

            std::error_code cleanup_error;
            fs::remove_all(package_path, cleanup_error);
            return 1;
        }
        catch (const std::exception& error) {
            std::cerr << "Failed to create package : " << error.what() << '\n';

            std::error_code cleanup_error;
            fs::remove_all(package_path, cleanup_error);
            return 1;
        }
        assert(false);
    }

    // load package
    package* pack = nullptr;
    try {
        // just make package root CWD for now
        pack = load_package(cwd_path);
        if(pack == nullptr) {
            std::cout << "Failed to load package\n";
            return 1;
        }
    } 
    catch(const std::runtime_error& e) {
        throw std::runtime_error(std::string("Failed to load package : ").append(e.what()));
    }  
    assert(pack != nullptr);

    // do action
    if(mode == "compile") {             // build all sources the current package depends on
        if(argc > 2) {
            std::cout << "USAGE : dylan compile\n";
            return 1;
        }
        build_dependencies(pack);
    }
    else if(mode == "build") {          // build the given target
        if(argc == 2) {
            std::cout << "Available targets : \n";
            for(const target& tgt : pack->targets) {
                std::cout << tgt.name << "\n";
            }
            return 1;
        }
        if(argc > 3) {
            std::cout << "USAGE : dylan build <target>\n";
            return 1;
        }
        std::string target_name = argv[argptr ++];

        // find target
        target tgt = get_target(pack, target_name);

        // build target
        build_target(pack, tgt);
    }
    else if(mode == "run") {            // execute the given target
        if(argc == 2) {
            std::cout << "Available targets : \n";
            for(const target& tgt : pack->targets) {
                std::cout << tgt.name << "\n";
            }
            return 1;
        }
        if(argc > 3) {
            std::cout << "USAGE : dylan run <target>\n";
            return 1;
        }
        std::string target_name = argv[argptr ++];

        // find target
        target tgt = get_target(pack, target_name);

        // can only run when target type is "binary"
        if(tgt.type != "binary") {
            std::cout << "Can only run targets of type \"binary\"\n";
            return 1;
        }

        // build target
        build_target(pack, tgt);

        // run target
        fs::path output_path = pack->bin_path / tgt.output;
        if(!fs::exists(output_path)) {
            std::cout << "Output binary does not exist : " << output_path.string() << std::endl;
            return 1;
        }
        return exec(output_path, {}, false);
    }
    else if(mode == "clean") {          // clean build artifacts
        if(argc > 2) {
            std::cout << "USAGE : dylan clean\n";
            return 1;
        }
        clean(pack);
    }
    else {
        std::cout << "Unknown mode : " << mode << "\n";
        return 1;
    }

    return 0;
}
