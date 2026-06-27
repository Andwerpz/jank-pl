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

#include "semantics/Program.h"
#include "semantics/StructDefinition.h"
#include "semantics/Function.h"
#include "semantics/Identifier.h"
#include "semantics/Type.h"
#include "semantics/Statement.h"
#include "semantics/FunctionSignature.h"
#include "semantics/Parameter.h"
#include "semantics/Constructor.h"
#include "semantics/ConstructorSignature.h"
#include "semantics/Include.h"

#include "semantics/utils.h"

#include <unistd.h>  
#include <sys/wait.h>
#include <cstring>
#include <cstdio>  
#include <limits.h>
#include <libgen.h>
#include <sys/stat.h>

bool print_timing_info = false;
bool no_default_includes = false;

std::string compiler_dir;
std::string cwd_dir;

std::string read_file(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::ostringstream buffer;
    buffer << file.rdbuf(); 
    return buffer.str();     
}

std::string read_cstr(char* s) {
    std::string ans = "";
    int ptr = 0;
    while(s[ptr] != '\0') {
        ans.push_back(s[ptr ++]);
    }
    return ans;
}

std::vector<std::string> str_split(std::string s, char sep) {
    std::vector<std::string> ret;
    int l = 0;
    for(int i = 0; i < s.size(); i++) {
        if(s[i] == sep) {
            ret.push_back(s.substr(l, i - l));
            l = i + 1;
        }
    }
    ret.push_back(s.substr(l, s.size() - l));
    return ret;
}

std::string extract_filename(std::string path) {
    return *(str_split(path, '/').rbegin());
}

std::string extract_folder_path(std::string path) {
    for(int i = (int) path.size() - 1; i >= 0; i--){
        if(path[i] == '/') return path.substr(0, i + 1);
    }
    //there is no '/', just return "./"
    return "./";
}

std::string extract_stem(std::string filename) {
    for(int i = filename.size() - 1; i >= 0; i--) {
        if(filename[i] == '.') return filename.substr(0, i);
    }
    return filename;
}   

std::string extract_ext(std::string filename) {
    for(int i = filename.size() - 1; i >= 0; i--){
        if(filename[i] == '.') return filename.substr(i);
    }
    return "";
}

std::string cwd_rel_to_absolute(std::string path){
    if(path[0] == '/') return path;
    return cwd_dir + "/" + path;
}

std::string libj_to_absolute(std::string name) {
    return compiler_dir + "/libj/" + name + ".jank";
}

int gen_asm(std::string src_path, char tmp_filename[]) {
    std::cout << "--- GATHERING FILES + PARSING + CONVERTING ---" << std::endl;
    Program *program = new Program();
    ld parse_duration = 0, convert_duration = 0;
    {
        std::queue<std::string> to_parse;
        std::set<std::string> parsed_paths;

        //include default libraries
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

        if(no_default_includes) default_includes.clear();

        for(std::string s : default_includes) {
            std::string npath = libj_to_absolute(s);
            to_parse.push(npath);
            parsed_paths.insert(npath);
        }

        //base source file
        to_parse.push(cwd_rel_to_absolute(src_path));
        parsed_paths.insert(cwd_rel_to_absolute(src_path));

        while(to_parse.size() != 0){
            std::string cpath = to_parse.front();
            to_parse.pop();
            std::cout << cpath << std::endl;

            std::string code = read_file(cpath);

            ld parse_start_time = current_time_seconds();
            parser::set_s(code);
            parser::set_gen_errors(false);
            parser::program *pp = parser::program::parse();
            if(!parser::check_finished_parsing(true) || parser::get_errors().size() != 0) {
                std::cout << "SYNTAX ERROR\n";
                return 1;
            }
            parse_duration += current_time_seconds() - parse_start_time;

            ld convert_start_time = current_time_seconds();
            Program *np = Program::convert(pp);
            program->add_all(np);
            convert_duration += current_time_seconds() - convert_start_time;

            //grab all includes
            for(int i = 0; i < np->includes.size(); i++){
                Include *inc = np->includes[i];
                std::string npath;
                if(inc->is_library_include) npath = compiler_dir + "/libj/" + inc->path + ".jank";
                else npath = extract_folder_path(cpath) + inc->path;

                //check if we already parsed
                if(parsed_paths.count(npath)) continue;

                parsed_paths.insert(npath);
                to_parse.push(npath);
            }
        }        
    }   

    std::cout << "--- CHECK PROGRAM SEMANTICS ---" << std::endl;
    ld semantics_duration;
    {
        ld semantics_start_time = current_time_seconds();

        fout = std::ofstream(tmp_filename);
        if(!program->is_well_formed()) {
            std::cout << "Program not well formed\n";
            fout.close();
            return 1;
        }
        std::cout << "Program is well formed\n";
        fout.close();

        semantics_duration = current_time_seconds() - semantics_start_time;
        
    }

    if(print_timing_info) {
        std::cout << "--- TIMING INFO ---" << "\n";
        print_duration_stats();
        
        std::cout << "--- TIMING OVERALL ---" << "\n";
        std::cout << std::fixed << std::setprecision(3) << "Parse Duration : " << parse_duration << "\n";
        std::cout << std::fixed << std::setprecision(3) << "Convert Duration : " << convert_duration << "\n";
        std::cout << std::fixed << std::setprecision(3) << "Semantics Duration : " << semantics_duration << "\n";
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
        std::cout << "USAGE : <filepath>\n";
        std::cout << "-S : generate assembly instead of executable\n";
        std::cout << "-o <out_filepath>\n";
        std::cout << "-k : kernel mode\n";
        std::cout << "-ad : assembly debug mode (prints some helpful (?) comments in the generated assembly)\n";
        std::cout << "-nodefincl : no default includes\n";
        return 1;
    }
    int argptr = 1;
    std::string filepath = read_cstr(argv[argptr ++]);
    std::string dst_dir = "a.out";

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
        else if(arg == "-ad") {
            asm_debug = true;
        }
        else if(arg == "-time") {
            print_timing_info = true;
        }
        else if(arg == "-nodefincl") {
            no_default_includes = true;
        }
        else {
            std::cout << "Unrecognized commandline argument : " << arg << "\n";
            return 1;
        }
    }

    char asm_file[] = "jjc_asmXXXXXX";
    int fd = mkstemp(asm_file);
    if(fd == -1) {
        perror("mkstemp failed");
        return 1;
    }
    close(fd);

    pid_t pid = fork();
    if(pid == 0) {
        exit(gen_asm(filepath, asm_file));
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
