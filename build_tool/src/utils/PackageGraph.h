#pragma once
#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <fstream>

#include <filesystem>
namespace fs = std::filesystem;

#include "../toml/toml.h"
#include "utils.h"

struct Package;
struct Target;

// graph of packages and their dependencies
struct PackageGraph {
    // all packages loaded by this PackageGraph
    std::vector<Package*> packages;

    Package* load_package(const fs::path& package_path);
    Package* load_package_from_library(const std::string& package_name);
    Package* get_package(const std::string& name);
    Package* find_owner(const fs::path& filepath);
    
    std::optional<std::vector<std::pair<std::string, fs::path>>> get_source_dependencies(const std::string& package_name, const fs::path& filepath);
    std::vector<Package*> get_package_dependencies(const std::string& package_name);
};
