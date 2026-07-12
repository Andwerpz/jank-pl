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

#include "semantics/utils.h"

#include <unistd.h>  
#include <sys/wait.h>
#include <cstring>
#include <cstdio>  
#include <limits.h>
#include <libgen.h>
#include <sys/stat.h>

int compile(std::string src_path, char tmp_filename[]) {
    //direct output to tmp file
    fout = std::ofstream(tmp_filename);

    bool success = false;
    if(recursive_compile) {
        success = compile_all(src_path);
    }
    else if(only_emit_driver) {
        success = emit_driver(src_path);
    }
    else {
        success = compile(src_path);
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

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "USAGE : jjc { <filepath> , <flag> }\n";
        std::cout << "-S : generate assembly instead of executable\n";
        std::cout << "-o <path> : write output to <path>\n";
        std::cout << "-k : kernel mode\n";
        std::cout << "--recursive : looks for all dependencies of the target and compiles them as well\n";
        std::cout << "--no-default-includes : omits default stdlib includes\n";
        std::cout << "--time : prints some timing info\n";
        std::cout << "--asm-debug : assembly debug mode (prints some helpful (?) comments in the generated assembly)\n";
        std::cout << "--startup-only : only emits startup and driver code\n";
        return 1;
    }
    int argptr = 1;
    std::vector<std::string> filepaths;
    std::string dst_dir = "a.out";

    //read in arguments
    bool return_asm = false;
    while(argptr < argc) {
        std::string arg(argv[argptr ++]);
        if(arg == "-S") {
            return_asm = true;
        }
        else if(arg == "-o") {
            if(argptr >= argc) {
                std::cout << "Missing output directory after -o\n";
                return 1;
            }
            dst_dir = std::string(argv[argptr ++]);
        }
        else if(arg == "-k") {
            kernel_mode = true;
        }
        else if(arg == "--recursive") {
            recursive_compile = true;
        }
        else if(arg == "--no-default-includes") {
            no_default_includes = true;
        }
        else if(arg == "--time") {
            print_timing_info = true;
            std::cout << "print timing info WIP" << std::endl;
            return 1;
        }
        else if(arg == "--asm-debug") {
            asm_debug = true;
        }
        else if(arg == "--startup-only") {
            only_emit_driver = true;
            return_asm = true;
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
        std::cout << "--recursive requires exactly one source file\n";
        return 1;
    }
    if(recursive_compile && only_emit_driver) {
        std::cout << "--recursive and --startup-only are mutually exclusive\n";
        return 1;
    }
    if(filepaths.size() > 1) {
        std::cout << "multiple files WIP\n";
        return 1;
    }

    //figure out compiler absolute directory
    {
        // /proc/self/exe is symlink to currently running binary
        // so readlink should give the absolute path no matter how this was invoked
        char exe_path[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
        if (len != -1) {
            exe_path[len] = '\0';
            compiler_dir = dirname(exe_path);
            std::cout << "COMPILER DIR : " << compiler_dir << "\n";
        }
        else {
            std::cout << "Could not find jjc path\n";
            return 1;
        }
    }

    //figure out stdlib absolute directory
    {
        //for now just hardcode it
        stdlib_dir = compiler_dir + "/../stdlib/src";
    }

    //figure out current working directory
    {
        char cwd_path[PATH_MAX];
        if (getcwd(cwd_path, sizeof(cwd_path)) != NULL) {
            cwd_dir = std::string(cwd_path);
            std::cout << "CWD : " << cwd_dir << "\n";
        }
        else {
            std::cout << "Could not find cwd\n";
            return 1;
        }
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
        std::ifstream src(asm_file);
        std::ofstream dst(dst_dir);
        if(!src || !dst) {
            std::cout << "Failed to copy over from " << asm_file << " to " << dst_dir << "\n";
            std::remove(asm_file);
            return 1;
        }
        dst << src.rdbuf();
        std::remove(asm_file);
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
        std::ifstream src(exe_file);
        std::ofstream dst(dst_dir);
        if(!src || !dst) {
            std::cout << "Failed to copy over from " << exe_file << " to " << dst_dir << "\n";
            std::remove(exe_file);
            return 1;
        }
        dst << src.rdbuf();

        //give file execute permissions
        chmod(dst_dir.c_str(), 0755);
    }
    
    //delete exe file
    std::remove(exe_file);
    
    return 0;
}
