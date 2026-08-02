#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <optional>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <set>
#include <iomanip>
#include <algorithm>

#include "semantics/utils.h"

#include <unistd.h>  
#include <sys/wait.h>
#include <cstring>
#include <cstdio>  
#include <limits.h>
#include <libgen.h>
#include <sys/stat.h>

#include "semantics/DefinitionSpace.h"

#include <filesystem>
namespace fs = std::filesystem;

int compile(std::string src_path, char tmp_filename[]) {
    //direct output to tmp file
    fout = std::ofstream(tmp_filename);

    assert(current_package != nullptr);

    bool success = false;
    if(recursive_compile) {
        success = compile_all(src_path);
    }
    else if(only_emit_driver) {
        success = emit_driver(src_path, current_package);
    }
    else {
        success = compile(src_path, current_package);
    }

    if(!success) {
        std::cout << "Failed to compile : " << src_path << std::endl;
        return 1;
    }

    return 0;
}

int assemble(char src_path[], char res_path[]) {
    pid_t pid = fork();
    if(pid == 0) {
        execlp(
            "gcc", "gcc", 
            "-g",                           //debug metadata
            "-x", "assembler",              //gcc expects .s files to be assembly, tell it that all files are assembly
            "-nostartfiles", "-nostdlib",   //tell gcc that we're not compiling C assembly
            "-m64",                         //64 bit mode?
            src_path,
            "-o", res_path,
            (char*) NULL
        );
        perror("execlp gcc failed");
        return 1;
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                std::cout << "Terminated by signal " << sig << " (" << strsignal(sig) << ")" << std::endl;
            }
            std::cout << "Assembling Error" << std::endl;
            return 1;
        }
    }
    return 0; 
}

// moves the file at src to dst. 
// assumes the file at src exists
// if there is an existing file at dst, overwrites it
// removes the file at src
int move_file(const fs::path& src, const fs::path& dst) {
    try {
        fs::path src_abs = fs::absolute(src);
        fs::path dst_abs = fs::absolute(dst);
        if(dst_abs.has_parent_path()) {
            fs::create_directories(dst_abs.parent_path());
        }
        fs::copy_file(src_abs, dst_abs, fs::copy_options::overwrite_existing);
        fs::remove(src_abs);
    } catch(const std::runtime_error& e) {
        std::cout << "Failed to move file \"" << src.string() << "\" to \"" << dst.string() << "\" : " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

// parses a package manifest 
// returns 0 on success, 1 on failure
int load_package_manifest(std::string package_manifest_path) {
    std::ifstream fin(package_manifest_path);
    if(!fin) {
        std::cout << "Failed to open package manifest for reading" << std::endl;
        return 1;
    }
    std::string line;
    while(getline(fin, line)) {
        std::vector<std::string> tok = str_split(line, ' ');
        if(tok.size() == 0) {
            continue;
        }
        std::string type = tok[0];
        if(type == "package") {
            if(tok.size() != 3) {
                std::cout << "Malformed line : " << line << std::endl;
                return 1;
            }
            std::string name = tok[1];
            std::string path = tok[2];
            if(!add_package(name, path)) {
                std::cout << "Failed to add package : " << name << std::endl;
                return 1;
            }
        }
        else if(type == "package-dependency") {
            if(tok.size() != 4) {
                std::cout << "Malformed line : " << line << std::endl;
                return 1;
            }
            std::string name = tok[1];
            std::string alias = tok[2];
            std::string dep_name = tok[3];
            Package* pack = get_package(name);
            if(pack == nullptr) {
                std::cout << "Failed to find package with name : " << name << std::endl;
                return 1;
            }
            Package* dep = get_package(dep_name);
            if(dep == nullptr) {
                std::cout << "Failed to find package with name : " << dep_name << std::endl;
                return 1;
            }
            if(!add_package_dependency(pack, alias, dep)) {
                std::cout << "Failed to add package dependency : " << name << " " << alias << " " << dep_name << std::endl;
                return 1;
            }
        }
        else if(type == "package-default-include") {
            if(tok.size() != 4) {
                std::cout << "Malformed line : " << line << std::endl;
                return 1;
            }
            std::string name = tok[1];
            std::string alias = tok[2];
            std::string path = tok[3];
            Package* pack = get_package(name);
            if(pack == nullptr) {
                std::cout << "Failed to find package with name : " << name << std::endl;
                return 1;
            }
            if(!add_package_default_include(pack, alias, path)) {
                std::cout << "Failed to add package default include : " << name << " " << alias << " " << path << std::endl;
                return 1;
            }
        }
        else if(type == "current-package") {
            if(tok.size() != 2) {
                std::cout << "Malformed line : " << line << std::endl;
                return 1;
            }
            std::string name = tok[1];
            Package* pack = get_package(name);
            if(pack == nullptr) {
                std::cout << "Failed to find package with name : " << name << std::endl;
                return 1;
            }
            if(!set_current_package(pack)) {
                std::cout << "Failed to set current package" << std::endl;
                return 1;
            }
        }
        else if(type == "runtime") {
            if(tok.size() != 3) {
                std::cout << "Malformed line : " << line << std::endl;
                return 1;
            }
            std::string package_name = tok[1];
            std::string path = tok[2];
            Package* pack = get_package(package_name);
            if(pack == nullptr) {
                std::cout << "Failed to find package with name : " << package_name << std::endl;
                return 1;
            }
            if(!add_runtime_file(pack, path)) {
                std::cout << "Failed to add runtime file : " << package_name << " " << path << std::endl;
                return 1;
            }
        }
        else if(type == "no-current-package") {
            if(tok.size() != 1) {
                std::cout << "Malformed line : " << line << std::endl;
                return 1;
            }
            
            // set current package to be the default unnamed package
            if(!set_current_package(default_package)) {
                std::cout << "Failed to set current package to default package\n";
                return 1;
            }
        }
        else if(type == "default-package-dependency") {
            if(tok.size() != 3) {
                std::cout << "Malformed line : " << line << std::endl;
                return 1;
            }
            std::string alias = tok[1];
            std::string dep_name = tok[2];
            Package* dep = get_package(dep_name);
            if(dep == nullptr) {
                std::cout << "Failed to find package with name : " << dep_name << std::endl;
                return 1;
            }
            if(!add_package_dependency(default_package, alias, dep)) {
                std::cout << "Failed to add default package dependency : " << alias << " " << dep_name << std::endl;
                return 1;
            }
        }
        else if(type == "default-package-default-include") {
            if(tok.size() != 3) {
                std::cout << "Malformed line : " << line << std::endl;
                return 1;
            }
            std::string alias = tok[1];
            std::string path = tok[2];
            if(!add_package_default_include(default_package, alias, path)) {
                std::cout << "Failed to add default package default include : " << alias << " " << path << std::endl;
                return 1;
            }
        }
        else {
            std::cout << "Unknown package manifest line type : " << type << std::endl;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "USAGE : jjc { <filepath> , <flag> }\n";
        std::cout << "\n";

        std::cout << " == GENERAL == \n";
        std::cout << "-S : generate assembly instead of executable\n";
        std::cout << "-o <path> : write output to <path>\n";
        std::cout << "\n";

        std::cout << " == COMPILATION MODE == \n";
        std::cout << "--kernel : kernel mode\n";
        std::cout << "--follow-includes : recursively compile source files referenced by includes\n";
        std::cout << "--startup-only : only emits startup and driver code\n";
        std::cout << "--intrinsic-provider : allow functions to define intrinsics\n";
        std::cout << "--no-syscall : the compiler treats syscalls as compilation errors\n";
        std::cout << "\n";

        std::cout << " == PACKAGE == \n";
        std::cout << "--package-manifest <path> : tells the compiler where the package manifest is\n";
        std::cout << "--emit-dependencies <path> : writes file containing all files required during compilation\n";
        std::cout << "\n";

        std::cout << " == DEBUG == \n";
        std::cout << "--debug : makes the compiler print debug information while compiling\n";
        std::cout << "--time : prints some timing info\n";
        std::cout << "--asm-debug : assembly debug mode (prints some helpful (?) comments in the generated assembly)\n";
        std::cout << "\n";
        return 1;
    }
    int argptr = 1;
    std::vector<std::string> filepaths;
    std::string dst_file = "a.out";

    // find default package manifest
    std::string package_manifest_path;
    {
        const char* home = std::getenv("HOME");
        if (home == nullptr) {
            throw std::runtime_error("Could not find user package library : HOME is not defined");
        }
        package_manifest_path = fs::path(home) / ".jank" / "default.jpm";
    }

    // figure out compiler absolute directory
    {
        // /proc/self/exe is symlink to currently running binary
        // so readlink should give the absolute path no matter how this was invoked
        char exe_path[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
        if (len != -1) {
            exe_path[len] = '\0';
            compiler_dir = dirname(exe_path);
        }
        else {
            std::cout << "Could not find jjc path\n";
            return 1;
        }
    }

    // figure out current working directory
    {
        char cwd_path[PATH_MAX];
        if (getcwd(cwd_path, sizeof(cwd_path)) != NULL) {
            cwd_dir = std::string(cwd_path);
        }
        else {
            std::cout << "Could not find cwd\n";
            return 1;
        }
    }

    //read in arguments
    bool return_asm = false;
    while(argptr < argc) {
        std::string arg(argv[argptr ++]);
        // == GENERAL ==
        if(arg == "-S") {
            return_asm = true;
        }
        else if(arg == "-o") {
            if(argptr + 1 > argc) {
                std::cout << "USAGE : -o <path>\n";
                return 1;
            }
            dst_file = std::string(argv[argptr ++]);
        }

        // == COMPILATION MODE ==
        else if(arg == "--kernel") {
            kernel_mode = true;
        }
        else if(arg == "--follow-includes") {
            recursive_compile = true;

            // need to also compile runtime files
            // TODO figure out how to handle this without letting the user define intrinsics 
            //   within their own code
            intrinsic_provider = true;  
        }
        else if(arg == "--startup-only") {
            only_emit_driver = true;
            return_asm = true;
        }
        else if(arg == "--intrinsic-provider") {
            intrinsic_provider = true;
        }
        else if(arg == "--no-syscall") {
            no_syscall = true;
        }

        // == PACKAGE ==
        else if(arg == "--package-manifest") {
            if(argptr + 1 > argc) {
                std::cout << "USAGE : --package-manifest <path>\n";
                return 1;
            }
            package_manifest_path = argv[argptr ++];
        }
        else if(arg == "--emit-dependencies") {
            if(argptr + 1 > argc) {
                std::cout << "USAGE : --emit-dependencies <path>\n";
                return 1;
            }
            emit_dependencies = true;    
            emit_dependencies_dir = argv[argptr ++];
        }

        // == DEBUG ==
        else if(arg == "--debug") {
            debug = true;
        }
        else if(arg == "--time") {
            print_timing_info = true;
            std::cout << "print timing info WIP" << std::endl;
            return 1;
        }
        else if(arg == "--asm-debug") {
            asm_debug = true;
        }

        else {
            //assume everything else is a file
            filepaths.push_back(arg);

            // TODO check if we can actually open this file
            // if we can't, then say it's an unrecognized flag
        }
    }
    if(filepaths.size() == 0) {
        std::cout << "Need to supply at least one source file\n";
        return 1;
    }
    if(recursive_compile && filepaths.size() != 1) {
        std::cout << "--follow-imports requires exactly one source file\n";
        return 1;
    }
    if(recursive_compile && only_emit_driver) {
        std::cout << "--follow-imports and --startup-only are mutually exclusive\n";
        return 1;
    }
    if(filepaths.size() > 1) {
        std::cout << "multiple files WIP\n";
        return 1;
    }

    //load package manifest
    if(load_package_manifest(package_manifest_path)) {
        std::cout << "Failed to load package manifest\n";
        return 1;
    }
    if(current_package == nullptr) {
        std::cout << "Package manifest didn't set current package\n";
        return 1;
    }

    //normalize filepaths
    for(std::string& filepath : filepaths) {
        //make it absolute
        if(filepath[0] != '/') {
            filepath = cwd_rel_to_absolute(filepath);
        }

        //normalize
        filepath = normalize_path(filepath);
    }

    //initialize compilation controller
    initialize_controller();

    char asm_file[] = "jjc_asmXXXXXX";
    int fd = mkstemp(asm_file);
    if(fd == -1) {
        perror("mkstemp failed");
        return 1;
    }
    close(fd);

    pid_t pid = fork();
    if(pid == 0) {
        for(std::string filepath : filepaths) {
            int status = compile(filepath, asm_file);
            if(status) {
                exit(status);
            }
        }
        if(emit_dependencies) {
            std::vector<std::pair<std::string, std::string>> deps_list;
            for(auto i = definition_spaces.begin(); i != definition_spaces.end(); i++) {
                std::string filepath = i->first;
                std::string package_name;
                DefinitionSpace *ds = i->second;
                Package* package = ds->get_package();
                if(package->is_named) package_name = package->name;
                else package_name = "__unnamed__";
                deps_list.push_back({package_name, filepath});
            }
            std::sort(deps_list.begin(), deps_list.end());

            std::ofstream deps(emit_dependencies_dir);
            if(!deps) {
                std::cout << "Failed to open : " << emit_dependencies_dir << std::endl;
                return 1;
            }
            for(const auto&[package_name, filepath] : deps_list) {
                deps << package_name << "\n" << filepath << "\n";
            }
            deps.close();
        }
        exit(0);
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                std::cout << "Terminated by signal " << sig << " (" << strsignal(sig) << ")" << std::endl;
            }
            std::cout << "Compilation Error\n";
            std::remove(asm_file);
            return 1;
        }
    }
    
    if(return_asm) {
        int status = move_file(fs::path(asm_file), fs::path(dst_file));
        if(status) {
            std::remove(asm_file);
            return status;
        }
        return 0;
    }

    //gen_asm success, time to assemble
    char exe_file[] = "jjc_exeXXXXXX";
    fd = mkstemp(exe_file);
    if(fd == -1) {
        perror("mkstemp failed");
        std::remove(asm_file);
        return 1;
    }
    close(fd);
    pid = fork();
    if(pid == 0){
        exit(assemble(asm_file, exe_file));
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            std::cout << "Assembling Error\n";
            std::remove(asm_file);
            std::remove(exe_file);
            return 1;
        }
    }

    //gen exe done, remove asm file
    std::remove(asm_file);

    //move exe file to output
    {
        int status = move_file(fs::path(exe_file), fs::path(dst_file));
        if(status) {
            std::remove(exe_file);
            return status;
        }

        //give file execute permissions
        chmod(dst_file.c_str(), 0755);

        return 0;
    }
    
    //delete exe file
    std::remove(exe_file);
    
    return 0;
}
