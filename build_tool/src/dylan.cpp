#include "dylan.h"

#include "toml/toml.h"
#include "utils/utils.h"
#include "utils/Package.h"
#include "utils/PackageGraph.h"
#include "utils/Target.h"

// tool for managing packages
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

// incremental compilation notes
// - the build tool should query the compiler to figure out all (direct and indirect) dependencies for each file
// - when a file changes, its dependencies can change.
// - if a source file is older than its artifact, then we consider it 'unchanged', otherwise it's 'changed'. 
//   - actually a more robust way to see if a file is 'changed' is to compute a hash of the entire file 
//     and compare it to a previously generated hash. 
//   - this solves cases where a package is depended on by multiple packages, is built by one of them, but the 
//     other packages aren't notified. 
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

PackageGraph* graph;

// -- PACKAGE MANIFEST
// generates a package manifest and places it into out_file
// sets the current package
void generate_package_manifest(
    const fs::path out_file, 
    std::string current_package_name, 
    std::optional<std::string> runtime_package_name
) {
    std::ofstream fout(out_file);
    if(!fout) {
        throw std::runtime_error("Failed to open : " + out_file.string() + " when writing package manifest");
    }

    // find all packages relevant to the current package and runtime package
    std::vector<Package*> packages;
    std::function<void(std::string)> add_package = [&packages](const std::string& package_name) {
        std::vector<Package*> all_deps = graph->get_package_dependencies(package_name);
        for(Package* dep : all_deps) {
            bool found = false;
            for(Package* _dep : packages) {
                if(_dep == dep) found = true;
            }
            if(found) continue;
            packages.push_back(dep);
        }
    };
    add_package(current_package_name);
    if(runtime_package_name.has_value()) add_package(runtime_package_name.value());
    
    // register all packages
    for(Package* pack : packages) {
        fout << "package " << pack->name << " " << pack->src_path.string() << "\n";
    }

    // set current package
    Package* current_package = graph->get_package(current_package_name);
    assert(current_package != nullptr);
    fout << "current-package " << current_package->name << "\n";

    // register all package dependencies
    for(Package* pack : packages) {
        for(const auto &dep_name : pack->package_dependencies) {
            assert(pack->reverse_alias_map.contains(dep_name));
            Package* dep = graph->get_package(dep_name);
            if(dep->type == Package::Type::Runtime) {
                // don't allow runtime packages to be depended on
                continue;
            }
            std::string alias = pack->reverse_alias_map[dep_name];
            fout << "package-dependency " << pack->name << " " << alias << " " << dep_name << "\n";
        }
    }

    // register all default includes
    for(Package* pack : packages) {
        for(const auto&[dep_name, path] : pack->default_includes) {
            assert(pack->reverse_alias_map.contains(dep_name));
            Package* dep = graph->get_package(dep_name);
            assert(dep->type != Package::Type::Runtime);
            std::string alias = pack->reverse_alias_map[dep_name];
            fout << "package-default-include " << pack->name << " " << alias << " " << path.string() << "\n";
        }
    }

    // set runtime files
    if(runtime_package_name.has_value()) {
        Package* runtime_package = graph->get_package(runtime_package_name.value());
        assert(runtime_package != nullptr);
        std::vector<fs::path> source_files = runtime_package->find_all_source_files();
        for(const fs::path& source_file : source_files) {
            const fs::path abs_path = runtime_package->src_path / source_file;
            fout << "runtime " << runtime_package->name << " " << abs_path.string() << "\n";
        }
    }

    fout.close();
}

// generates a package manifest and places it into out_file
// does not set a current package
// used for giving jjc a default package manifest
// all packages here should be able to be loaded from the library
void generate_default_package_manifest(const fs::path& out_file) {
    std::ofstream fout(out_file);
    if(!fout) {
        throw std::runtime_error("Failed to open : " + out_file.string() + " when writing package manifest");
    }

    // make sure to load relevant library packages
    graph->load_package_from_library("jank-stdlib");
    graph->load_package_from_library("jank-runtime");
    
    // find all relevant packages
    std::vector<Package*> packages;
    std::function<void(std::string)> add_package = [&packages](const std::string& package_name) {
        std::vector<Package*> all_deps = graph->get_package_dependencies(package_name);
        for(Package* dep : all_deps) {
            bool found = false;
            for(Package* _dep : packages) {
                if(_dep == dep) found = true;
            }
            if(found) continue;
            packages.push_back(dep);
        }
    };
    add_package("jank-stdlib");
    add_package("jank-runtime");
    
    // register all packages
    for(Package* pack : packages) {
        fout << "package " << pack->name << " " << pack->src_path.string() << "\n";
    }

    // set current package
    fout << "no-current-package" << "\n";

    // register all package dependencies
    for(Package* pack : packages) {
        for(const auto &dep_name : pack->package_dependencies) {
            assert(pack->reverse_alias_map.contains(dep_name));
            Package* dep = graph->get_package(dep_name);
            if(dep->type == Package::Type::Runtime) {
                // don't allow runtime packages to be depended on
                continue;
            }
            std::string alias = pack->reverse_alias_map[dep_name];
            fout << "package-dependency " << pack->name << " " << alias << " " << dep_name << "\n";
        }
    }

    // register all default includes
    for(Package* pack : packages) {
        for(const auto&[dep_name, path] : pack->default_includes) {
            assert(pack->reverse_alias_map.contains(dep_name));
            Package* dep = graph->get_package(dep_name);
            assert(dep->type != Package::Type::Runtime);
            std::string alias = pack->reverse_alias_map[dep_name];
            fout << "package-default-include " << pack->name << " " << alias << " " << path.string() << "\n";
        }
    }

    // register default package dependencies
    fout << "default-package-dependency " << "std " << "jank-stdlib" << "\n";

    // register default package default includes
    fout << "default-package-default-include " << "std " << "memory" << "\n";
    fout << "default-package-default-include " << "std " << "error" << "\n";
    fout << "default-package-default-include " << "std " << "defs" << "\n";
    fout << "default-package-default-include " << "std " << "syscall" << "\n";
    fout << "default-package-default-include " << "std " << "malloc" << "\n";

    // set runtime files
    {
        Package* runtime_package = graph->get_package("jank-runtime");
        assert(runtime_package != nullptr);
        std::vector<fs::path> source_files = runtime_package->find_all_source_files();
        for(const fs::path& source_file : source_files) {
            const fs::path abs_path = runtime_package->src_path / source_file;
            fout << "runtime " << runtime_package->name << " " << abs_path.string() << "\n";
        }
    }

    fout.close();
}

void create_new_package(const fs::path& package_path, std::string package_name) {
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
        config
            << "[[dependency]]\n"
            << "package = \"jank-stdlib\"\n"
            << "alias = \"std\"\n"
            << "\n";

        // add jank-runtime as a dependency
        config
            << "[[dependency]]\n"
            << "package = \"jank-runtime\"\n"
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
            << "runtime = \"jank-runtime\"\n"
            << "\n";
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
}

// -- BUILD --
// removes all build artifacts
void clean(const std::string& package_name) {
    Package* pack = graph->get_package(package_name);
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
void build_sources(const std::string& package_name) {
    std::cout << "Building sources : " << package_name << std::endl;

    // get package
    Package* pack = graph->get_package(package_name);

    // find all source files in package source dir
    std::vector<fs::path> source_files = pack->find_all_source_files();

    // figure out which source files need to be recompiled
    // should recompile if 
    // - you don't have a dependencies entry
    // - any of your dependencies entries are invalid
    //   - missing source file
    //   - invalid package
    // - you or any of your source dependencies have changed
    std::function<bool(const fs::path&)> should_recompile = [&should_recompile, &pack](const fs::path& source_file) -> bool {
        std::optional<std::vector<std::pair<std::string, fs::path>>> dependencies = graph->get_source_dependencies(pack->name, source_file);

        // - does this source file have a dependencies entry?
        if(!dependencies.has_value()) {
            return true;
        }

        // - are any of your dependencies entries invalid?
        for(auto &[dep_name, dep_source_file] : dependencies.value()) {
            // see if we can find this package
            Package* dep = nullptr;
            try {
                dep = graph->get_package(dep_name);
            } catch(std::runtime_error e) {
                return true;
            }
            assert(dep != nullptr);

            // check if source file exists
            fs::path dep_source_file_abs = dep->src_path / dep_source_file;
            if(!fs::exists(dep_source_file_abs)) {
                return true;
            }
            if(!fs::is_regular_file(dep_source_file_abs)) {
                return true;
            }
        }

        // - do we have an existing hash for this source file?
        std::optional<std::string> prev_dependency_hash = pack->get_dependency_hash(source_file);
        if(!prev_dependency_hash.has_value()) {
            return true;
        }

        // - does the current hash match the previous one?
        std::string cur_dependency_hash = graph->compute_source_dependency_hash(pack->name, source_file);
        if(cur_dependency_hash != prev_dependency_hash.value()) {
            return true;
        }

        // no need to recompile
        return false;
    };

    std::vector<fs::path> to_recompile;
    for(const fs::path& source_file : source_files) {
        if(should_recompile(source_file)) {
            to_recompile.push_back(source_file);
        }
    }

    // recompile, update deps
    for(const fs::path& source_file : to_recompile) {
        const fs::path source_abs = pack->src_path / source_file;
        assert(fs::exists(source_abs));
        const fs::path deps_abs = pack->tmp_path / "deps.tmp";        

        // compute source dependency hash
        std::string dep_hash = graph->compute_source_dependency_hash(pack->name, source_file);
        pack->set_dependency_hash(source_file, dep_hash);

        // compile
        {
            fs::path build_abs = pack->build_path / source_file;
            build_abs = build_abs.replace_extension(".s");

            const fs::path manifest_abs = pack->tmp_path / "manifest.jpm";
            generate_package_manifest(manifest_abs, package_name, std::nullopt);

            std::vector<std::string> compile_args;
            compile_args.push_back("-S");
            compile_args.push_back("--emit-dependencies");
            compile_args.push_back(deps_abs);
            compile_args.push_back("--package-manifest");
            compile_args.push_back(manifest_abs.string());

            // allow runtime packages to define intrinsics
            if(pack->type == Package::Type::Runtime) {
                compile_args.push_back("--intrinsic-provider");
            }

            if(compile(source_abs, build_abs, compile_args)) {
                throw std::runtime_error("Failed to compile : " + source_file.string());
            }
        }
        if(!fs::exists(deps_abs)) {
            throw std::runtime_error("Compiler didn't create deps file");
        }

        // read deps file
        std::ifstream input(deps_abs);
        if(!input) {
            throw std::runtime_error("Failed to open dependency file: " + deps_abs.string());
        }

        std::vector<std::pair<std::string, fs::path>> dependencies;
        std::string dep_name;
        std::string dependency_path;
        while(std::getline(input, dep_name)) {
            if (dep_name.empty()) {
                continue;
            }
            if(!std::getline(input, dependency_path)) {
                throw std::runtime_error("Dependency package '" + dep_name + "' is missing its path in: " + deps_abs.string());
            }
            if (dependency_path.empty()) {
                throw std::runtime_error("Dependency package '" + dep_name + "' has an empty path in: " + deps_abs.string());
            }

            // convert dependency path to be relative to package src path
            Package* dep = graph->get_package(dep_name);
            assert(dep != nullptr);
            fs::path dependency_rel = get_relative_path(dep->src_path, dependency_path);

            dependencies.push_back({dep_name, dependency_rel});
        }
        pack->source_dependencies[source_file] = dependencies;
    }
    if(to_recompile.size() == 0) {
        std::cout << "Everything up to date :D" << std::endl;
    }
    else {
        // write deps to deps.toml
        pack->write_source_dependencies();

        // write hashes to hash.toml
        pack->write_dependency_hashes();
    }
}

// ensures all dependencies of the package are built
// this includes transitive dependencies
// TODO decide if we even care about circular package dependencies
void build_dependencies(const std::string& package_name) {
    std::vector<std::string> all_dependencies;
    std::function<void(std::string)> find_deps = [&all_dependencies, &find_deps](std::string package_name) {
        for(std::string dep_name : all_dependencies) {
            if(dep_name == package_name) return;
        }
        all_dependencies.push_back(package_name);
        Package *pack = graph->get_package(package_name);
        for(const auto& dep_name : pack->package_dependencies) {
            find_deps(dep_name);
        }
    };
    find_deps(package_name);
    for(std::string dep_name : all_dependencies) {
        build_sources(dep_name);
    }
}

// builds the provided target
void build_target(const std::string& package_name, const std::string& target_name) {
    // ensure this package is built
    build_dependencies(package_name);

    // build 
    Package* pack = graph->get_package(package_name);
    Target* tgt = pack->get_target(target_name);
    if(tgt->type == "binary") {
        const fs::path entry_abs = pack->src_path / tgt->entry;
        const fs::path driver_abs = pack->tmp_path / "driver.tmp";

        // create driver code
        {
            Package* runtime_pack = graph->get_package(tgt->runtime);
            const fs::path manifest_abs = pack->tmp_path / "manifest.jpm";
            generate_package_manifest(manifest_abs, pack->name, runtime_pack->name);

            std::vector<std::string> compile_args;
            compile_args.push_back("-S");
            compile_args.push_back("--startup-only");
            compile_args.push_back("--package-manifest");
            compile_args.push_back(manifest_abs.string());

            if(compile(entry_abs, driver_abs, compile_args)) {
                throw std::runtime_error("Failed to create driver code : " + entry_abs.string());
            }
        }

        // recursively find all required source files for this target
        std::vector<std::pair<Package*, fs::path>> source_files;    // {package, relative path}
        std::function<void(Package*, fs::path)> find_files = [&source_files, &find_files](Package* pack, fs::path filepath) {
            // see if we've already found this
            for(auto &[_pack, _filepath] : source_files) {
                if(_pack == pack && _filepath == filepath) {
                    return;
                }
            }

            // add to source files
            source_files.push_back({pack, filepath});

            // find all source files this one depends on
            std::optional<std::vector<std::pair<std::string, fs::path>>> dependencies = pack->get_source_dependencies(filepath);
            assert(dependencies.has_value());
            for(auto &[dep_name, path] : dependencies.value()) {
                Package* dep = graph->get_package(dep_name);
                find_files(dep, path);
            }
        };
        find_files(pack, tgt->entry);
        {
            Package* runtime_pack = graph->get_package(tgt->runtime);
            std::vector<fs::path> runtime_source_files = runtime_pack->find_all_source_files();
            for(const fs::path& source_file : runtime_source_files) {
                find_files(runtime_pack, source_file);
            }
        }

        // locate corresponding assembly files
        std::vector<fs::path> asm_files;
        asm_files.push_back(driver_abs);
        for(auto &[pack, source_file] : source_files) {
            fs::path asm_file_rel = source_file;
            asm_file_rel.replace_extension(".s");
            asm_files.push_back(pack->build_path / asm_file_rel);
        }

        // make sure output directory exists
        const fs::path output_abs = pack->bin_path / tgt->output;
        fs::path output_dir = output_abs;
        output_dir.remove_filename();
        fs::create_directories(output_dir);

        // assemble
        if(assemble(asm_files, output_abs)) {
            throw std::runtime_error("Failed to assemble");
        }
    }
    else {
        throw std::runtime_error("Unknown target type : " + tgt->type);
    }
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "USAGE : dylan <mode>\n";
        std::cout << "\n";

        std::cout << "== GENERAL ==\n";
        std::cout << "new <package_path> : creates a new package\n";
        std::cout << "default-manifest : generates the default package manifest for jjc\n";
        std::cout << "\n";

        std::cout << "== PACKAGE ==\n";
        std::cout << "compile : ensures all source files are compiled\n";
        std::cout << "build <target> : builds the target\n";
        std::cout << "run <target> { <args> } : builds and runs the executable generated by the target\n";
        std::cout << "install : installs the package for the current user\n";
        std::cout << "uninstall : uninstalls the current package for the current user\n";
        std::cout << "clean : removes all build artifacts\n";
        std::cout << "\n";
        return 1;
    }
    int argptr = 1;
    std::string mode = argv[argptr ++];

    // find user package library
    {
        const char* home = std::getenv("HOME");
        if (home == nullptr) {
            throw std::runtime_error("Could not find user package library : HOME is not defined");
        }
        user_library_path = fs::path(home) / ".jank";
    }

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

    // create package graph
    graph = new PackageGraph();

    // modes not requiring loading package at cwd
    if(mode == "new") {
        if(argc != 3) {
            std::cout << "USAGE : dylan new <package_path>\n";
            return 1;
        }
        fs::path package_path = cwd_path / argv[argptr ++];
        package_path = package_path.lexically_normal();
        std::string package_name = package_path.filename().string();
        
        // make sure package name fits [a-zA-Z0-9_-]+
        if(package_name.size() == 0) {
            std::cout << "Invalid package name : package name must be non-empty\n";
            return 1;
        }
        for(char c : package_name) {
            if('A' <= c && c <= 'Z') continue;
            if('a' <= c && c <= 'z') continue;
            if('0' <= c && c <= '9') continue;
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
            create_new_package(package_path, package_name);
        }
        catch(const std::runtime_error& e) {
            std::cout << "Failed to create package : " << e.what() << "\n";
            fs::remove_all(package_path);
            return 1;
        }
        std::cout << "Package created successfully\n";
        return 0;
    }
    else if(mode == "default-manifest") {
        if(argc != 2) {
            std::cout << "USAGE : dylan default-manifest\n";
            return 1;
        }

        fs::path default_manifest_path = user_library_path / "default.jpm";
        std::cout << "Creating default package manifest at : " << default_manifest_path.string() << std::endl;
        try { 
            generate_default_package_manifest(default_manifest_path);
        } 
        catch (const std::runtime_error& e) {
            std::cout << "Failed to create default package manifest\n";
            return 1;
        }
        std::cout << "Default package manifest created successfully\n";
        return 0;
    }

    // load package at cwd
    Package* pack = nullptr;
    try {
        // just make package root CWD for now
        pack = graph->load_package(cwd_path);
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
        build_dependencies(pack->name);
    }
    else if(mode == "build") {          // build the given target
        if(argc == 2) {
            std::cout << "Available targets : \n";
            for(const Target* tgt : pack->targets) {
                std::cout << tgt->name << "\n";
            }
            return 1;
        }
        if(argc > 3) {
            std::cout << "USAGE : dylan build <target>\n";
            return 1;
        }
        std::string target_name = argv[argptr ++];

        // build target
        build_target(pack->name, target_name);
    }
    else if(mode == "run") {            // execute the given target
        if(argc == 2) {
            std::cout << "Available targets : \n";
            for(const Target* tgt : pack->targets) {
                std::cout << tgt->name << "\n";
            }
            return 1;
        }
        std::string target_name = argv[argptr ++];\
        std::vector<std::string> args;
        while(argptr < argc) {
            args.push_back(argv[argptr ++]);
        }

        // can only run when target type is "binary"
        Target* tgt = pack->get_target(target_name);
        if(tgt->type != "binary") {
            std::cout << "Can only run targets of type \"binary\"\n";
            return 1;
        }

        // build target
        build_target(pack->name, target_name);

        // run target
        fs::path output_path = pack->bin_path / tgt->output;
        if(!fs::exists(output_path)) {
            std::cout << "Output binary does not exist : " << output_path.string() << std::endl;
            return 1;
        }
        return exec(output_path, args, false);
    }
    else if(mode == "install") {
        if(argc > 2) {
            std::cout << "USAGE : dylan install\n";
            return 1;
        }

        // install
        fs::path install_path = user_library_path / pack->name;
        try {
            std::cout << "Installing " << pack->name << " to " << install_path.string() << std::endl;
            fs::remove_all(install_path);
            copy_directory(pack->path, install_path);
            std::cout << "Installation successful\n";
        } catch(const std::runtime_error& e) {
            std::cout << "Failed to install : " << e.what() << std::endl;
            return 1;
        }
    }
    else if(mode == "uninstall") {
        if(argc > 2) {
            std::cout << "USAGE : dylan uninstall\n";
            return 1;
        }

        // uninstall
        fs::path install_path = user_library_path / pack->name;
        try {
            std::cout << "Uninstalling " << pack->name << " from " << install_path.string() << std::endl;
            fs::remove_all(install_path);
            std::cout << "Uninstallation successful\n";
        } catch(const std::runtime_error& e) {
            std::cout << "Failed to uninstall : " << e.what() << std::endl;
            return 1;
        }
    }
    else if(mode == "clean") {          // clean build artifacts
        if(argc > 2) {
            std::cout << "USAGE : dylan clean\n";
            return 1;
        }
        clean(pack->name);
    }
    else if(mode == "manifest") {
        // parse args
        fs::path out_path = cwd_path / "manifest.jpm";
        std::optional<std::string> runtime_package_name = std::nullopt;
        while(argptr < argc) {
            std::string flag = argv[argptr ++];
            if(flag == "-o") {
                if(argptr + 1 > argc) {
                    std::cout << "USAGE : dylan manifest -o <path>\n";
                    return 1;
                }
                out_path = argv[argptr ++];
            }
            else if(flag == "--runtime") {
                if(argptr + 1 > argc) {
                    std::cout << "USAGE : dylan manifest --runtime <package-name>\n";
                    return 1;
                }
                runtime_package_name = argv[argptr ++];
            }
            else {
                std::cout << "Unrecognized flag : " << flag << "\n";
                return 1;
            }
        }

        // write manifest
        generate_package_manifest(out_path, pack->name, runtime_package_name);
    }
    else {
        std::cout << "Unknown mode : " << mode << "\n";
        return 1;
    }

    return 0;
}
