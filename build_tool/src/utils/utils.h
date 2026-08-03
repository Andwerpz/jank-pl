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

#include "../toml/toml.h"

// -- GENERAL UTILS --
std::vector<std::string> str_split(const std::string& s, char sep);
int exec(fs::path bin_path, std::vector<std::string> args, bool redirect_stdout);
int compile(fs::path src_path, fs::path out_path, std::vector<std::string> args);
int assemble(std::vector<fs::path> asm_paths, fs::path out_path);
void write_toml(const toml::table& toml, const fs::path path);
bool is_ancestor(const fs::path& root, const fs::path& target);
fs::path get_relative_path(const fs::path& root, const fs::path& target);
void copy_directory(const fs::path& src, const fs::path& dst);

// -- TOML --
template<typename T = std::string>
T get_toml_table_field(toml::table* table, const std::string& field) {
    auto value = (*table)[field].value<T>();
    if (!value) {
        throw std::runtime_error("Missing or invalid field: " + std::string(field));
    }
    return *value;
}

// -- HASHING --
using Sha256Hash = std::array<std::uint8_t, 32>;
Sha256Hash sha256(std::span<const std::uint8_t> data);
Sha256Hash sha256(std::string str);
Sha256Hash sha256_file(const fs::path& path);
Sha256Hash combine_hashes(const Sha256Hash& first, const Sha256Hash& second);
std::string hash_to_hex(const Sha256Hash& hash);
Sha256Hash hex_to_hash(const std::string& hex);

// important paths
// inline fs::path system_library_path = "/usr/lib/jank";
inline fs::path user_library_path;
inline fs::path cwd_path;
