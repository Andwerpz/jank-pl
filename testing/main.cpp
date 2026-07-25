#include <bits/stdc++.h>
using namespace std;

// jank test runner

// all tests should have the following format:
// ./tests/<testname>
//   info.txt
//   out.txt
//   main.jank
//   src
//     <other source files>

// info.txt holds information regarding stuff like
// - whether or not the code should compile
// - what arguments to use when running the generated executable

// out.txt holds the expected STDOUT of the generated executable

// for source files, it's expected that main.jank exists and has a valid entry point
// all other source files are optional. 

// to make a new test, just write main.jank and your other source files into ./wip_test
// the test runner should automatically create your test and populate info.txt and out.txt 
//   make sure to double check the generated test afterwards 

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


#include <filesystem>
namespace fs = std::filesystem;

string compiler_path = "../compiler/jjc.exe";

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

struct TestInfo {
    bool should_compile;
    vector<string> args;
};

TestInfo parse_test_info(string test_path) {
    ifstream fin(test_path + "/info.txt");
    if(!fin) {
        throw runtime_error("failed to open " + test_path + "/info.txt");
    }

    TestInfo info;

    int should_compile = -1;
    while(!fin.eof()) {
        string line;
        getline(fin, line);
        vector<string> tok = str_split(line, ' ');
        if(tok.size() == 0) continue;
        if(tok[0] == "compiled") {
            should_compile = stoi(tok[1]);
            if(should_compile != 0 && should_compile != 1) {
                std::cout << "Invalid value after 'compiled', should be in {0, 1}" << std::endl;
                exit(1);
            }
        }
        else if(tok[0] == "args") {
            info.args.clear();
            for(int i = 1; i < tok.size(); i++) info.args.push_back(tok[i]);
        }
        else {
            std::cout << "Unknown info : " + tok[0] << std::endl;
            exit(1);
        }
    }
    if(should_compile == -1) {
        std::cout << "Didn't find 'compiled'" << std::endl;
        exit(1);
    }
    info.should_compile = should_compile;

    return info;
}

//recursively copies src directory into dst
int copy_directory(const string& src, const string& dst) {
    pid_t pid = fork();

    if(pid == 0) {
        execlp(
            "cp",
            "cp",
            "-r",
            (src + "/.").c_str(),
            dst.c_str(),
            (char*) nullptr
        );

        perror("cp failed");
        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);

    return !WIFEXITED(status) || WEXITSTATUS(status) != 0;
}

//prints the entire file contents to STDOUT
void print_file(const string& filepath) {
    ifstream fin(filepath);
    if(!fin) {
        std::cout << "failed to open " << filepath << std::endl;
        exit(1);
    }
    cout << fin.rdbuf();
}

// compiles the provided source file into exe_path
int run_compiler(string src_path, string exe_path) {
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

        execl(compiler_path.c_str(), "jjc", src_path.c_str(), "-o", exe_path.c_str(), "--follow-includes", (char*) nullptr);
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

// runs an exe with the provided arguments
// redirects STDOUT to out_path
int run_exe(string exe_path, vector<string> args, string out_path) {
    pid_t pid = fork();
    if(pid == 0) {
        //redirect output to a.out
        int fd = open(out_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(fd < 0) {
            cout << "create fd failed\n";
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);

        //prepare arguments
        // first arg is exe path
        char** argv = (char**) malloc(sizeof(char*) * (args.size() + 2));
        argv[0] = (char*) malloc(sizeof(char) * exe_path.size() + 1);
        memcpy(argv[0], exe_path.c_str(), exe_path.size() + 1);
        for(int i = 1; i <= args.size(); i++) {
            string arg = args[i - 1];
            argv[i] = (char*) malloc(sizeof(char) * (arg.size() + 1));
            memcpy(argv[i], arg.c_str(), arg.size() + 1);
        }
        argv[args.size() + 1] = nullptr;

        //exec
        execv(exe_path.c_str(), argv);
        perror("execl exe failed");
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

// Validates a test without checking its output against out.txt.
// If the test should compile, this leaves its generated output in ./a.out.
int validate_test(const string& test_path) {
    TestInfo info = parse_test_info(test_path);
    string exe_path = "./a.exe";
    string out_path = "./a.out";

    bool compiled = !run_compiler(test_path + "/main.jank", exe_path);
    if(compiled != info.should_compile) {
        std::cout << "expected to compile: " << info.should_compile << " actual: " << compiled << std::endl;
        return 1;
    }

    if(!compiled) {
        return 0;
    }

    if(run_exe(exe_path, info.args, out_path)) {
        std::cout << "test compiled but failed while running" << std::endl;
        return 1;
    }

    return 0;
}

// runs the entire test. 
// returns 0 on success, nonzero on failure
int run_test(string test_path, string work_path, string& error_message) {
    //parse info.txt
    TestInfo info = parse_test_info(test_path);
    string exe_path = work_path + "/a.exe";
    string out_path = work_path + "/a.out";

    //see if it compiles
    bool compiled = !run_compiler(test_path + "/main.jank", exe_path);
    if(compiled != info.should_compile) {
        error_message = ("expected to compile : " + to_string(info.should_compile) + " actual : " + to_string(compiled));
        return 1;
    }

    if(compiled) {
        //see if it runs
        int run_status = run_exe(exe_path, info.args, out_path);
        if(!run_status) {
            //run success, see if outputs match
            if(!are_files_equal(out_path, test_path + "/out.txt")) {
                error_message = "output mismatch";
                return 1;
            }
        }
        else {
            //this should not happen
            error_message = "compiled but did not run";
            return 1;
        }
    }

    return 0;
}

void run_tests_multithreaded(vector<string> tests, size_t thread_count = std::thread::hardware_concurrency()) {
    assert(thread_count >= 1);
    std::cout << "THREAD COUNT : " << thread_count << std::endl;
    
    vector<bool> passed(tests.size(), false);
    vector<string> error_output(tests.size());
    atomic<size_t> next_test = 0;

    size_t test_name_width = 0;
    for(const string& test : tests) {
        test_name_width = max(test_name_width, test.size());
    }

    auto worker = [&]() {
        while(true) {
            size_t index = next_test.fetch_add(1);
            if(index >= tests.size()) {
                break;
            }

            const string& testname = tests[index];
            string test_path = "./tests/" + testname;
            string work_path = "./tmp/tests/" + testname;

            auto start_time = std::chrono::steady_clock::now();

            fs::remove_all(work_path);
            fs::create_directories(work_path);

            string error_message;
            int status = run_test(test_path, work_path, error_message);

            passed[index] = (status == 0);
            error_output[index] = std::move(error_message);

            auto end_time = std::chrono::steady_clock::now();
            double elapsed_s = std::chrono::duration<double>(end_time - start_time).count();

            std::osyncstream output(cout);

            output << "Running "
                << std::left
                << std::setw(static_cast<int>(test_name_width))
                << testname
                << " : ";

            if(passed[index]) {
                output << "\033[1;32mPASSED\033[0m";
            }
            else {
                output << "\033[1;31mFAILED\033[0m";
            }

            output << "  ("
                << std::fixed
                << std::setprecision(2)
                << elapsed_s
                << " s)\n";
        }
    };

    auto start_time = std::chrono::steady_clock::now();
    vector<thread> workers;
    for(size_t i = 0; i < thread_count; i++) {
        workers.emplace_back(worker);
    }
    for(thread& worker_thread : workers) {
        worker_thread.join();
    }
    auto end_time = std::chrono::steady_clock::now();
    double elapsed_ms = std::chrono::duration<double>(end_time - start_time).count();
    std::cout << "Total time elapsed : " << std::fixed << std::setprecision(2) << elapsed_ms << " s" << std::endl;

    bool passed_all = true;
    for(int i = 0; i < tests.size(); i++) {
        if(!passed[i]) {
            passed_all = false;
        }
    }
    if(passed_all) {
        cout << "All tests passed" << std::endl;
    }
    else {
        cout << "Failed tests : " << std::endl;
        for(int i = 0; i < tests.size(); i++) {
            if(!passed[i]) {
                cout << tests[i] << " : " << error_output[i] << "\n";
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        cout << "Usage : \n";
        cout << "main run-wip\n";
        cout << "main run-test <testname>\n";
        cout << "main run-tests <--start <start_test>>\n";
        cout << "main gen-test <testname>\n";
        return 1;
    }

    int argptr = 1;
    string mode = string(argv[argptr ++]);
    if(mode == "run-wip") {
        string test_path = "./wip_test";
        TestInfo info = parse_test_info(test_path);
        if(validate_test(test_path)) {
            std::cout << "WIP test failed to validate" << std::endl;
        }
        else {
            std::cout << "WIP test validated" << std::endl;
            if(info.should_compile) {
                std::cout << "Test STDOUT : " << std::endl;
                print_file("./a.out");
            }
        }
    }
    else if(mode == "run-test") {
        if(argc < 3) {
            std::cout << "Too few arguments for run-test" << std::endl;
            return 1;
        }
        string testname = argv[argptr ++];
        if(argptr != argc) {
            cout << "Unexpected argument: " << argv[argptr] << "\n";
            return 1;
        }

        run_tests_multithreaded({testname});
    }
    else if(mode == "run-tests") {
        std::string start_test = "";

        //parse arguments
        while(argptr != argc) {
            string next(argv[argptr ++]);
            if(next == "--start") {
                start_test = argv[argptr ++];
            }
            else {
                cout << "Unknown argument : " << next << "\n";
                return 1;
            }
        }

        //gather all tests
        vector<string> tests;
        for(auto entry : fs::directory_iterator("./tests")) {
            assert(entry.is_directory());
            string testname = entry.path().filename().string();
            if(start_test.size() != 0 && testname < start_test) {
                continue;
            }
            tests.push_back(testname);
        }
        sort(tests.begin(), tests.end());

        //run tests
        run_tests_multithreaded(tests);
    }
    else if(mode == "gen-test") {
        if(argc < 3) {
            std::cout << "Too few arguments for gen-test" << std::endl;
            return 1;
        }
        string testname = argv[argptr ++];
        if(argptr != argc) {
            cout << "Unexpected argument: " << argv[argptr] << "\n";
            return 1;
        }

        string wip_path = "./wip_test";
        string test_path = "./tests/" + testname;

        if(!fs::exists(wip_path) || !fs::is_directory(wip_path)) {
            cout << "Could not find " << wip_path << "\n";
            return 1;
        }
        if(!fs::exists(wip_path + "/main.jank")) {
            cout << "Could not find " << wip_path << "/main.jank" << std::endl;
            return 1;
        }
        if(!fs::exists(wip_path + "/info.txt")) {
            cout << "Could not find " << wip_path << "/info.txt" << std::endl;
            return 1;
        }
        if(fs::exists(test_path)) {
            cout << "Test " << testname << " already exists" << std::endl;
            return 1;
        }

        // validate WIP test
        if(validate_test(wip_path)) {
            cout << "WIP test failed to validate" << std::endl;
            return 1;
        }

        // move WIP test
        TestInfo info = parse_test_info(wip_path);
        fs::create_directories(test_path);
        if(copy_directory(wip_path, test_path)) {
            cout << "Failed to copy " << wip_path << " into " << test_path << std::endl;
            return 1;
        }

        // see if we need to generate out.txt
        if(info.should_compile) {
            fs::copy_file(
                "a.out",
                test_path + "/out.txt",
                fs::copy_options::overwrite_existing
            );
        }

        cout << "Generated test " << testname << "\n";
        return 0;
    }
    else {
        cout << "Invalid mode : " << mode << "\n";
        return 1;
    }

    return 0;
}