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

struct Package;
struct Target;
struct PackageGraph;

// -- PACKAGE MANIFEST --
void generate_package_manifest(
    const fs::path out_file, 
    std::string current_package_name, 
    std::optional<std::string> runtime_package_name
);
void generate_default_package_manifest(const fs::path& out_file);

// -- BUILD --
void clean(const std::string& package_name);
void build_sources(const std::string& package_name);
void build_dependencies(const std::string& package_name);
void build_target(const std::string& package_name, const std::string& target_name);

