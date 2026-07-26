#pragma once 

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

// something you can build
struct target {
    std::string name;
    std::string type;
    fs::path entry;
    fs::path output;
};

// all paths are absolute
struct package {
    std::string name;
    fs::path path;

    toml::table config;
    toml::table deps;

    fs::path src_path;
    fs::path build_path;
    fs::path tmp_path;
    fs::path bin_path;

    std::vector<target> targets;
    
    // {alias, package}
    std::vector<std::pair<std::string, package*>> dependencies;

    // {alias, include path}
    std::vector<std::pair<std::string, fs::path>> default_includes;
};

// -- GENERAL UTILS --
int exec(fs::path bin_path, std::vector<std::string> args, bool redirect_stdout);
int compile(fs::path src_path, fs::path out_path, std::vector<std::string> args);
int assemble(std::vector<fs::path> asm_paths, fs::path out_path);
void write_toml(const toml::table& toml, const fs::path path);
bool is_ancestor(const fs::path& root, const fs::path& target);
fs::path get_relative_path(const fs::path& root, const fs::path& target);

// -- PACKAGE --
package* load_package(fs::path package_path);
package* get_package(std::string name);
package* get_package(package* package, std::string alias);
target get_target(package* package, std::string target_name);
std::vector<std::string> generate_compiler_package_args(package* current_pack);
std::optional<std::vector<std::pair<package*, fs::path>>> get_dependencies(package* pack, fs::path filepath);
package* find_owner(fs::path filepath);
std::vector<fs::path> find_all_source_files(const package* pack);

// -- BUILD --
void clean(const package* pack);
void build_sources(package* pack);
void build_dependencies(package* pack);
void build_target(package* pack, const target& tgt);

inline fs::path cwd_path;

// should hold all packages ever loaded
inline std::vector<package*> all_packages;
