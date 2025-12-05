#include <bits/stdc++.h>
using namespace std;

//running tests

#include <unistd.h>  
#include <sys/wait.h>
#include <cstring>
#include <cstdio>  
#include <limits.h>
#include <libgen.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <filesystem>
namespace fs = std::filesystem;

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
    for(int i = 0; i < s.size(); ) {
        if(s[i] == sep) {
            i ++;
            continue;
        }
        int r = i;
        while(r != s.size() && s[r] != sep) r ++;
        ret.push_back(s.substr(i, r - i));
        i = r;
    }
    return ret;
}

std::string extract_filename(std::string path) {
    return *(str_split(path, '/').rbegin());
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

string compiler_path = "../compiler/jjc.exe";

int run_compiler(string src_path, string dst_path) {
    pid_t pid = fork();
    if(pid == 0) {
        //redirect compiler output to "compiler.out"
        int fd = open("compiler.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(fd < 0) {
            cout << "create fd failed\n";
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);

        execl(compiler_path.c_str(), "jjc", src_path.c_str(), "-o", dst_path.c_str(), (char*) NULL);
        perror("compiler exec failed");
        exit(1);
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

int compile_from_test(string testname) {
    return run_compiler("./tests/" + testname, "a.exe");
}

int run_aexe(vector<string> args) {
    pid_t pid = fork();
    if(pid == 0) {
        //redirect output to a.out
        int fd = open("a.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(fd < 0) {
            cout << "create fd failed\n";
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);

        //prepare arguments
        char** argv = (char**) malloc(sizeof(char*) * (args.size() + 1));
        for(int i = 0; i < args.size(); i++) {
            argv[i] = (char*) malloc(sizeof(char) * (args[i].size() + 1));
            memcpy(argv[i], args[i].c_str(), args[i].size() + 1);
        }
        argv[args.size()] = nullptr;

        //exec
        execv("a.exe", argv);
        perror("execl a.exe failed");
        exit(1);
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

//removes a.exe, a.out
void clean_tmp() {
    fs::remove("a.exe");
    fs::remove("a.out");
}

bool are_files_equal(string file1, string file2) {
    ifstream f1(file1, std::ios::binary | std::ios::ate);
    ifstream f2(file2, std::ios::binary | std::ios::ate);

    if(!f1) throw runtime_error("failed to open " + file1);
    if(!f2) throw runtime_error("failed to open " + file2);

    // Compare file sizes first
    if (f1.tellg() != f2.tellg()) return false;

    // Go back to beginning
    f1.seekg(0);
    f2.seekg(0);

    // Compare byte by byte
    std::istreambuf_iterator<char> begin1(f1);
    std::istreambuf_iterator<char> begin2(f2);
    return std::equal(begin1, std::istreambuf_iterator<char>(), begin2);
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        cout << "Usage : \n";
        cout << "main run-tests\n";
        cout << "main gen-test <jankpath> <testname> <-a <argc> <argv>>\n";
        cout << "main regen-tests\n";
        return 1;
    }

    string mode = string(argv[1]);
    if(mode == "run-tests") {
        //gather all tests
        vector<string> tests;
        for(auto entry : fs::directory_iterator("./tests")) {
            assert(entry.is_directory());
            tests.push_back(entry.path().filename().string());
        }

        bool passing = true;
        try {
            for(int i = 0; i < tests.size(); i++) {
                cout << "Running " << tests[i] << " : " << flush;
                string testdir = "./tests/" + tests[i];
    
                ifstream info_fin(testdir + "/info.txt");
                int should_compile = -1;
                vector<string> args = {"a.exe"};
                while(!info_fin.eof()) {
                    string line;
                    getline(info_fin, line);
                    vector<string> tok = str_split(line, ' ');
                    if(tok.size() == 0) continue;
                    if(tok[0] == "compiled") {
                        should_compile = stoi(tok[1]);
                    }
                    else if(tok[0] == "args") {
                        args.clear();
                        for(int i = 1; i < tok.size(); i++) args.push_back(tok[i]);
                    }
                    else throw runtime_error("Unknown info : " + tok[0]);
                }

                if(should_compile != 0 && should_compile != 1) {
                    throw runtime_error("could not find 'compiled' in info");
                }
    
                //see if it compiles
                bool compiled = !run_compiler(testdir + "/src.jank", "a.exe");
                if(compiled != should_compile) {
                    throw runtime_error("expected to compile : " + to_string(should_compile) + " actual : " + to_string(compiled));
                }

                if(compiled) {
                    //see if it runs
                    int run_status = run_aexe(args);
                    if(!run_status) {
                        //run success, see if outputs match
                        if(!are_files_equal("a.out", testdir + "/out.txt")) {
                            throw runtime_error("output mismatch");
                        }
                    }
                    else {
                        //this should not happen
                        throw runtime_error("compiled but did not run");
                    }
                }
                
                //clean up
                clean_tmp();

                cout << "PASSED" << endl;
            }
        } catch(runtime_error e) {
            cout << "FAILED : " << e.what() << endl;
            passing = false;
        }

        cout << (passing? "All tests passed" : "Tests failed") << endl;
    }
    else if(mode == "regen-tests") {
        //just the src.jank from each test should be enough to completely regenerate it. 
        //should just extract the src.jank, delete the test, and call gen-test to regenerate
        //upd : now that we've introduced arguments, should also look at info.txt to see if we have any arguments
        //TODO
        assert(false);
    }
    else if(mode == "gen-test") {        
        bool failed = false;
        int compile_status, run_status;
        string jankpath = string(argv[2]);
        string testname = string(argv[3]);
        vector<string> args = {"a.exe"};

        //parse arguments
        int argptr = 4;
        while(argptr != argc) {
            string next(argv[argptr ++]);
            if(next == "-a") {
                int amt = stoi(argv[argptr ++]);
                if(argptr + amt > argc) {
                    cout << "Invalid -a amt : " << amt << "\n";
                    return 1;
                }
                for(int i = 0; i < amt; i++) {
                    args.push_back(string(argv[argptr ++]));
                }
            }
            else {
                cout << "Unknown argument : " << next << "\n";
                return 1;
            }
        }

        string n_testdir = "./tests/" + testname;

        //check if test already exists
        if(fs::exists(n_testdir) && fs::is_directory(n_testdir)) {
            cout << "Test " << testname << " already exists\n";
            goto gen_failed;
        }

        //create test directory
        fs::create_directory(n_testdir);
        
        //copy .jank into test directory
        fs::copy_file(jankpath, n_testdir + "/src.jank");

        //try to compile test   
        compile_status = run_compiler(n_testdir + "/src.jank", "a.exe");

        //if compiles, try to run exe
        if(!compile_status) {
            run_status = run_aexe(args);
            if(!run_status) {
                //copy output to test
                fs::copy_file("a.out", n_testdir + "/out.txt");
            }
            else {
                //if it compiles, it must run properly
                goto gen_failed;
            }
        }

        //print some metadata
        {
            ofstream fout(n_testdir + "/info.txt");
            fout << "compiled" << " " << (compile_status == 0) << "\n";
            fout << "args" << " ";
            for(string s : args) fout << s << " ";
            fout << "\n";
        }
        
        goto gen_done;
        gen_failed : {
            //delete test directory
            fs::remove_all(n_testdir);
            failed = true;
        }

        gen_done : {
            clean_tmp();
        }

        return failed;
    }
    else {
        cout << "Invalid mode : " << mode << "\n";
        return 1;
    }

    return 0;
}