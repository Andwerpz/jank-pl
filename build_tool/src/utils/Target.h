#pragma once
#include <string>

#include <filesystem>
namespace fs = std::filesystem;

// something you can build
struct Target {
    std::string name;
    std::string type;
    fs::path entry;
    fs::path output;
    std::string runtime;
};
