#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>  
#include <sys/wait.h>
#include <cstring>
#include <cstdio>  
#include <limits.h>
#include <libgen.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syncstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <functional>

#include <filesystem>
namespace fs = std::filesystem;

#include "toml/toml.h"
#include "utils/utils.h"

// tool for managing projects
// a project is expected to look like this:
// project_root/
//   config.toml
//   deps.toml
//   src/
//     <source files here>
//   build/
//     <build artifacts here>
//   tmp/
//     <tmp files here>

// what should this tool be able to do?
// - build the project (build)
// - run the project (run)
// - delete generated files (clean)
// - run tests? (test)
//   - how would this work, what are the responsibilities of the build tool vs test runner
// - list all targets (targets)
// - print cool statistics about the project 
//   - dependency graph (deps)
//   - lines of code (loc)
//   - file compile times

// structure of config.toml:
/*
# project metadata
[project]
name: "jank-compiler"            # project name

# targets
# each target should specify some sort of thing to build
[[target]]
name = "compiler"
type = "binary"
entry = "jjc.jank"  # relative to source path
output = "bin/jjc"  # relative to project root

# dependencies 
# each dependency is assumed to be another jank project
# TODO have packages be installed at a known location like /.jank and look for packages there
#   if the user doesn't specify a path
[[dependency]]
name = "jank-stdlib"                    # should match with the name of the project
alias = "std"                           # optional, if omitted will just be the name
path = "/home/steven/jank-pl/stdlib"    

# TODO also be able to specify compilation args for each individual file or all files?
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

std::string stdlib_path;

// something you can build
struct target {
    std::string name;
    std::string type;
    std::string entry;
    std::string output;
};

// external library description
struct package {    
    std::string name;
    std::string alias;
    std::string path;
};

struct project {
    std::string root;
    std::string name;

    toml::table config;
    toml::table deps;

    std::string src_path;
    std::string build_path;
    std::string tmp_path;

    std::vector<target> targets;
    std::vector<package> dependencies;
};

project* load_project(std::string project_path) {
    // find config.toml
    toml::table config;
    try {
        config = toml::parse(project_path + "/config.toml");
    } catch(const toml::parse_error& e) {
        throw std::runtime_error(std::string("Failed to load config.toml : ").append(e.what()));
    }

    // load project name
    std::optional<std::string> _project_name = config["project"]["name"].value<std::string>();
    if(!_project_name.has_value()) {
        throw std::runtime_error("project.name missing from config.toml");
    }
    std::string project_name = _project_name.value();

    // find /src
    // TODO load this from config.toml
    std::string src_path = "/src";
    if(!fs::exists(fs::path(project_path + src_path))) {
        throw std::runtime_error("Source directory does not exist : " + src_path);
    }
    if(!fs::is_directory(fs::path(project_path + src_path))) {
        throw std::runtime_error("Source path is not a directory : " + src_path);
    }

    // find /build
    // TODO load this from config.toml
    std::string build_path = "/build";
    if(!fs::exists(fs::path(project_path + build_path))) {
        throw std::runtime_error("Build directory does not exist : " + build_path);
    }
    if(!fs::is_directory(fs::path(project_path + build_path))) {
        throw std::runtime_error("Build path is not a directory : " + build_path);
    }

    // find /tmp
    // TODO load this from config.toml
    std::string tmp_path = "/tmp";
    if(!fs::exists(fs::path(project_path + tmp_path))) {
        throw std::runtime_error("Temp directory does not exist : " + tmp_path);
    }
    if(!fs::is_directory(fs::path(project_path + tmp_path))) {
        throw std::runtime_error("Temp path is not a directory : " + tmp_path);
    }

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
            auto get_field = [](toml::table* t, std::string field) {
                std::optional<std::string> _val = (*t)[field].value<std::string>();
                if(!_val.has_value()) {
                    throw std::runtime_error("Target missing field : " + field);
                }
                return _val.value();
            };
            std::string name = get_field(target, "name");
            std::string type = get_field(target, "type");
            std::string entry = get_field(target, "entry");
            std::string output = get_field(target, "output");

            targets.push_back({name, type, entry, output});        
        }
    }

    // dependencies
    std::vector<package> dependencies;
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
            auto get_field = [](toml::table* t, std::string field) {
                std::optional<std::string> _val = (*t)[field].value<std::string>();
                if(!_val.has_value()) {
                    throw std::runtime_error("Dependency missing field : " + field);
                }
                return _val.value();
            };
            std::string name = get_field(dependency, "name");
            std::string path = get_field(dependency, "path");
            std::string alias = name;
            if(dependency->contains("alias")) {
                alias = get_field(dependency, "alias");
            }

            dependencies.push_back({name, alias, path});
        }
    }

    // make sure no two targets have the same name
    for(int i = 0; i < targets.size(); i++) {
        for(int j = 0; j < i; j++) {
            if(targets[i].name == targets[j].name) {
                throw std::runtime_error("Duplicate target name : " + targets[i].name);
            }
        }
    }

    // find deps.toml
    toml::table deps;
    try {
        config = toml::parse(project_path + "/deps.toml");
    } catch(const toml::parse_error& e) {
        throw std::runtime_error(std::string("Failed to load deps.toml : ").append(e.what()));
    }

    // assemble project struct
    project* proj = new project();
    proj->root = project_path;
    proj->name = project_name;
    proj->config = config;
    proj->deps = deps;
    proj->src_path = src_path;
    proj->build_path = build_path;
    proj->tmp_path = tmp_path;
    proj->targets = targets;
    proj->dependencies = dependencies;

    return proj;
}

// compiles the file at src_path into out_path
// assumes src_path and out_path are absolute
// returns 0 on success, nonzero on failure
int compile(std::string src_path, std::string out_path, std::vector<std::string> args) {
    pid_t pid = fork();
    if(pid == 0) {
        std::vector<std::string> command_args = {
            "jjc",
            src_path,
            "-o",
            out_path
        };
        command_args.insert(command_args.end(), args.begin(), args.end());

        std::vector<char*> argv;
        argv.reserve(command_args.size() + 1);
        for (std::string& arg : command_args) {
            argv.push_back(arg.data());
        }
        argv.push_back(nullptr);

        execvp("jjc", argv.data());
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

// links and assembles the given assembly files into out_path
// assumes all paths in asm_paths and out_path are absolute
// returns 0 on success, nonzero on failure
int assemble(std::vector<std::string> asm_paths, std::string out_path) {
    pid_t pid = fork();
    if(pid == 0) {
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

        std::vector<char*> argv;
        argv.reserve(command_args.size() + 1);
        for (std::string& arg : command_args) {
            argv.push_back(arg.data());
        }
        argv.push_back(nullptr);

        execvp("gcc", argv.data());
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

// writes the provided toml file to the given path
// assumes that the path doesn't correspond to an existing directory
// assumes that the path is an absolute path
void write_toml(const toml::table& toml, const std::string path) {
    if(fs::is_directory(path)) {
        throw std::runtime_error("Tried to write toml to directory : " + path);
    }
    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("Failed to open file : " + path);
    }
    output << toml;
    if (!output) {
        throw std::runtime_error("failed to write toml to file : " + path);
    }
}

// retrieves the list of dependencies of this file
// if there is not a list of dependencies for this file, returns std::nullopt
std::optional<std::vector<std::string>> get_dependencies(project* proj, std::string filepath) {
    toml::node* _dependencies = proj->deps.get(filepath);
    if(_dependencies == nullptr) {
        return std::nullopt;
    }

    std::vector<std::string> dependencies;
    toml::array* _dependencies_arr = _dependencies->as_array();
    if (_dependencies_arr == nullptr) {
        throw std::runtime_error("Dependencies should be an array of strings : " + filepath);
    }
    for (const toml::node& node : *_dependencies_arr) {
        auto dependency = node.value<std::string>();
        if(!dependency) {
            throw std::runtime_error("Dependencies should be an array of strings : " + filepath);
        }
        assert(dependency.has_value());
        dependencies.push_back(dependency.value());
    }
    return dependencies;
}

// figures out which package owns the provided filepath
package* find_owner(const project* proj, std::string filepath) {
    // TODO
}

// finds all files under proj->src_path that have the extension `.jank`
// returned files are relative to proj->src_path
std::vector<std::string> find_all_source_files(const project* proj) {
    const fs::path src_root = fs::path(proj->root) / proj->src_path;
    assert(fs::exists(src_root) && fs::is_directory(src_root));

    std::vector<std::string> source_files;
    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(src_root)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (entry.path().extension() != ".jank") {
            continue;
        }
        source_files.push_back(fs::relative(entry.path(), src_root).generic_string());
    }
    std::sort(source_files.begin(), source_files.end());

    return source_files;
}

// removes all artifacts from the build directory
void clean(const project* proj) {
    const fs::path build_root = fs::path(proj->root) / proj->build_path;
    assert(fs::exists(build_root) && fs::is_directory(build_root));

    fs::remove_all(build_root);
    fs::create_directories(build_root);
}

// ensures all dependencies of the project are compiled
void build_dependencies(project* proj) {
    // TODO
}

// ensures all source files in the project are compiled
void build_sources(project* proj) {
    std::vector<std::string> source_files = find_all_source_files(proj);

    // check which source files have changed
    // a source file changed if it has been modified after the last time its corresponding 
    //   artifact has been modified. 
    std::vector<std::string> changed_files;
    for(const std::string& source_file : source_files) {
        const fs::path source_abs = fs::path(proj->root) / proj->src_path / source_file;
        const fs::path build_abs = fs::path(proj->root) / proj->build_path / source_file;
        assert(fs::exists(source_abs));

        bool changed = false;
        if(!fs::exists(build_abs)) {
            changed = true;
        }
        if(fs::last_write_time(source_abs) > fs::last_write_time(build_abs)) {
            changed = true;
        }

        if(changed) {
            changed_files.push_back(source_file);
        }
    }

    // figure out which source files need to be recompiled
    // a source file needs to be recompiled if it or any of its dependencies have changed
    // also should compile any source file that doesn't have an entry in deps. 
    std::vector<std::string> to_recompile;
    for(const std::string& source_file : source_files) {
        bool should_recompile = false;

        // should recompile if you have been changed. 
        for(int i = 0; i < changed_files.size(); i++) {
            if(source_file == changed_files[i]) {
                should_recompile = true;
            }
        }

        // should recompile if any of your dependencies have been changed
        // or if you don't have a dependencies entry
        std::optional<std::vector<std::string>> dependencies = get_dependencies(proj, source_file);
        if(dependencies.has_value()) {
            for(const std::string& dependency : dependencies.value()) {
                for(const std::string changed : changed_files) {
                    if(changed == dependency) {
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
    for(const std::string source_file : source_files) {
        const fs::path source_abs = fs::path(proj->root) / proj->src_path / source_file;
        const fs::path build_abs = fs::path(proj->root) / proj->build_path / source_file;
        const fs::path deps_abs = fs::path(proj->root) / proj->tmp_path / "deps.tmp";
        assert(fs::exists(source_abs));

        if(compile(source_abs, build_abs, {"-S", "--emit-dependencies", deps_abs})) {
            throw std::runtime_error("Failed to compile : " + source_file);
        }

        // read deps file
        std::ifstream input(deps_abs);
        if(!input) {
            throw std::runtime_error("Failed to open dependency file: " + deps_abs.string());
        }
        toml::array dependencies;
        std::string line;
        while(std::getline(input, line)) {
            if (line.empty()) {
                continue;
            }
            dependencies.push_back(line);
        }
        proj->deps.insert_or_assign(source_file, std::move(dependencies));
    }

    // write deps to deps.toml
    write_toml(proj->deps, proj->root + "/deps.toml");
}

// builds the provided target
void build_target(project* proj, const target& tgt) {
    // ensure all sources are built
    build_sources(proj);

    // build 
    if(tgt.type == "binary") {
        // create driver code
        const fs::path entry_abs = fs::path(proj->root) / proj->src_path / tgt.entry;
        const fs::path driver_abs = fs::path(proj->root) / proj->tmp_path / "driver.tmp";
        if(compile(entry_abs, driver_abs, {"-S", "--startup-only"})) {
            throw std::runtime_error("Failed to create driver code : " + entry_abs.string());
        }

        // recursively find all required source files for this target
        std::vector<std::pair<project*, std::string>> source_files;
        std::function<void(project*, std::string)> find_files = [&source_files](project* proj, std::string file) {
            // see if we've already found this
            for(int i = 0; i < source_files.size(); i++) {
                if(proj == source_files[i].first && file == source_files[i].second) {
                    return;
                }
            }

            // find all source files this one depends on
            // TODO
        };

        // locate corresponding assembly files
        std::vector<std::string> asm_files;
        for(auto &[proj, source_file] : source_files) {
            // TODO
        }

        // assemble
        const fs::path output_abs = fs::path(proj->root) / tgt.output;
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
        std::cout << "new <project_path> : creates a new project\n";
        std::cout << "compile : ensures all source files are compiled\n";
        std::cout << "build <target> : builds the target\n";
        std::cout << "run <target> : builds and runs the executable generated by the target\n";
        std::cout << "clean : removes all artifacts from the build directory\n";
        return 1;
    }
    int argptr = 1;
    std::string mode = argv[argptr ++];

    // find stdlib 
    {
        // just hardcode it for now
        stdlib_path = "/home/steven/jank-pl/stdlib";
        assert(fs::is_directory(stdlib_path));
    }

    // special case for new projects
    if(mode == "new") {
        if(argc != 3) {
            std::cout << "USAGE : dylan new <project_path>\n";
            return 1;
        }
        std::string project_path = argv[argptr ++];
        std::string project_name = extract_filename(project_path);  

        std::cout << "PROJECT NAME : " << project_name << "\n";

        // TODO
        assert(false);
    }

    // load project
    project* proj = nullptr;
    try {
        // just make project root CWD for now
        std::string cwd;
        char cwd_path[PATH_MAX];
        if (getcwd(cwd_path, sizeof(cwd_path)) != NULL) {
            cwd = std::string(cwd_path);
        }
        else {
            std::cout << "Could not find CWD\n";
            return 1;
        }
        proj = load_project(cwd);
        if(proj == nullptr) {
            std::cout << "Failed to load project\n";
            return 1;
        }
    } catch(const toml::parse_error& e) {
        throw std::runtime_error(std::string("Failed to load project : ").append(e.what()));
    }  
    assert(proj != nullptr);

    // do action
    if(mode == "compile") {
        if(argc > 2) {
            std::cout << "USAGE : dylan compile\n";
            return 1;
        }
        build_sources(proj);
    }
    else if(mode == "build") {       // build the given target
        if(argc == 2) {
            std::cout << "Available targets : \n";
            for(const target& tgt : proj->targets) {
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
        bool found_target = false;
        target tgt;
        for(int i = 0; i < proj->targets.size(); i++) {
            if(proj->targets[i].name == target_name) {
                tgt = proj->targets[i];
                found_target = true;
                break;
            }
        }
        if(!found_target) {
            std::cout << "Failed to find target : " << target_name << "\n";
            return 1;
        }

        // build target
        build_target(proj, tgt);
    }
    else if(mode == "run") {    // execute the given target
        assert(false); // TODO
    }
    else if(mode == "clean") {  // clean build directory
        clean(proj);
    }
    else {
        std::cout << "Unknown mode : " << mode << "\n";
        return 1;
    }

    return 0;
}
