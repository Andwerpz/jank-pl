#include <iostream>
#include <string>
#include <vector>

// tool for managing projects
// a project is expected to look like this:
// project_root/
//   config.toml
//   src/
//     <source files here>
//   build/
//     <build artifacts here>
//   bin/
//     <executables here>

// The user should be able to specify what file the entry point is in,
//   but for now I'll assume it's in src/main.jank
// The user should also be able to specify what path the final generated executable is
// Should also be capable of compiling to multiple 'targets'
//   for example something like this (ty mr.g)
/* TOML
[target.compiler]
type = "executable"
entry = "src/compiler.jank"
output = "bin/jjc"

[target.language_server]
type = "executable"
entry = "src/lsp.jank"
output = "bin/jank-lsp"
*/
//   so each target should be able to specify how it's built, then the build tool just
//   follows the instructions laid out by each target.

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

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "USAGE : dylan <project_root>\n";
        return 1;
    }
    int argptr = 1;
    std::string project_root = argv[argptr ++];



    return 0;
}
