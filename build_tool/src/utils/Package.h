#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include "../toml/toml.h"

#include <filesystem>
namespace fs = std::filesystem;

// a package is expected to look like this:
// package_root/
//   config.toml
//   deps.toml
//   hash.toml
//   src/
//     <source files here>
//   build/
//     <build artifacts here>
//   tmp/
//     <tmp files here>
//   bin/
//     <build results here>

// structure of config.toml:
/*
# package metadata
[package]
name = "jank-compiler"              # package name
type = "library"                    # package type
environment = "hosted"              # package environment requirements

# dependencies 
# each dependency is assumed to be another jank package
[[dependency]]
package = "jank-stdlib"                 # should match with the name of the package
alias = "std"                           # optional, if omitted will just be the package name
path = "/home/steven/jank-pl/stdlib"    # if you specify a path, this will be assumed to be the path to the project
                                        # otherwise dylan will look inside system and user package libraries
export = false                          # whether or not this dependency should be implicitly depended on by 
                                        # packages that depend on this package. Useful for making libraries. 

[[dependency]]
package = "jank-runtime"

# default includes
# useful for automatically including stuff like <std::defs> in every source file
[[default-include]]
package = "std"     # alias of the package you want to include
path = "defs"       # relative to package source directory and excluding extension

# targets
# each target should specify some sort of thing to build
[[target]]
name = "compiler"
type = "binary"
entry = "jjc.jank"          # relative to source path
output = "jjc"              # relative to bin path
runtime = "jank-runtime"    # what runtime package to link with the final exe


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

// structure of hash.toml
/*
# for each source file, should store a hash of itself + all of its dependencies + config.toml
# when building, this hash gets recomputed to decide whether or not to recompile the file
"jjc.jank" = "..."
*/

// package type can be
// - library
//   - 'normal' package that exposes source code that other packages can include
//   - this is the default package type
// - runtime
//   - package that is responsible for defining compiler intrinsics
//   - the code in this package cannot be included from other packages, but this package
//     can include code from other packages. 
//     - this is enforced by not registering runtime packages as dependencies package manifests
//     - packages definitely should be able to register runtime packages as dependencies in the config
//       as how else would you be able to set the runtime package of a target?

// package environment can be
// - hosted
//   - this package assumes an underlying operating system
//   - this is the default package environment
// - freestanding
//   - this package does not assume an underlying operating system. 
//   - freestanding packages can only depend on other freestanding packages. 
//     - this should be enforced in the package config

// a bundle of source code
// all paths are absolute
// aliases should all be resolved when parsing the package
//   this struct should only hold package names
// package dependency exports should be resolved when parsing the package
struct Package {
    enum State {
        Unparsed        = 0,
        Parsed          = 1,
        Resolved        = 2,
    };
    State state = State::Unparsed;

    std::string name;
    fs::path path;

    enum Type {
        Library, Runtime, 
    };
    Type type;

    enum Environment {
        Hosted, Freestanding,
    };
    Environment environment;

    fs::path src_path;
    fs::path build_path;
    fs::path tmp_path;
    fs::path bin_path;

    // dependencies as read in from config.toml
    struct Dependency {
        std::string package;
        std::string alias;
        std::optional<fs::path> path;       // if not present, this refers to a package in the library
        bool is_exported;
    };
    std::vector<Dependency> dependencies;

    // something you can build
    struct Target {
        enum Type {
            Binary
        };

        std::string name;
        Type type;
        fs::path entry;
        fs::path output;
        std::string runtime;
    };
    std::vector<Target> targets;

    // an include that is added by default to all source files in this package
    struct DefaultInclude {
        std::string package;
        fs::path path;
    };
    std::vector<DefaultInclude> default_includes;

    // list of package names this package directly depends on
    // this does not include the package itself
    std::vector<std::string> resolved_dependencies;

    // resolved map from alias to package dependency name
    std::unordered_map<std::string, std::string> alias_map;

    // resolved map from package dependency name to alias
    std::unordered_map<std::string, std::string> reverse_alias_map;

    // map from source file to list of {package name, source file} pairs
    // the list of source files that this source file depends on.
    // note that packages in this list do not have to be direct dependencies of this package
    //   they can be transitively included from packages not directly depended on. 
    std::unordered_map<fs::path, std::vector<std::pair<std::string, fs::path>>> source_dependencies;

    // map from source file to sha256 hash of dependencies
    std::unordered_map<fs::path, std::string> dependency_hashes;

    Target get_target(const std::string& name) const;
    std::vector<fs::path> find_all_source_files() const;
    std::optional<std::vector<std::pair<std::string, fs::path>>> get_source_dependencies(const fs::path& source_file);
    void write_source_dependencies();
    std::optional<std::string> get_dependency_hash(const fs::path& source_file);
    void set_dependency_hash(const fs::path& source_file, std::string hash);
    void write_dependency_hashes();
};
