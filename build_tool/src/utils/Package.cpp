#include "Package.h"
#include <algorithm>

Package::Target Package::get_target(const std::string& name) const {
    for(const Package::Target& tgt : targets) {
        if(tgt.name == name) {
            return tgt;
        }
    }
    throw std::runtime_error("Failed to get target with name : " + name + " for package : " + this->name);
}

// finds all files under src_path that have the extension `.jank`
// returned files are relative to src_path
std::vector<fs::path> Package::find_all_source_files() const {
    assert(fs::exists(src_path) && fs::is_directory(src_path));
    std::vector<fs::path> source_files;
    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(src_path)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (entry.path().extension() != ".jank") {
            continue;
        }
        source_files.push_back(fs::relative(entry.path(), src_path));
    }
    std::sort(source_files.begin(), source_files.end());
    return source_files;
}

// if the source file has source dependencies, returns them
// otherwise returns nullopt
std::optional<std::vector<std::pair<std::string, fs::path>>> Package::get_source_dependencies(const fs::path& source_file) {
    if(source_dependencies.contains(source_file)) {
        return source_dependencies[source_file];
    }
    else {
        return std::nullopt;
    }
}

// writes source dependencies to pack->path / deps.toml
// only write dependencies of source files that exist
void Package::write_source_dependencies() {
    // generate deps.toml
    std::vector<fs::path> all_source_files = find_all_source_files();
    sort(all_source_files.begin(), all_source_files.end());
    toml::table root;
    for(const fs::path& source_file : all_source_files) {
        if(!source_dependencies.contains(source_file)) {
            continue;
        }

        std::string key = source_file.string();
        toml::array dependencies;
        for(const auto& [package_name, dependency_path] : source_dependencies[source_file]) {
            toml::table dependency;
            dependency.insert("package", package_name);
            dependency.insert("path", dependency_path.generic_string());
            dependency.is_inline(true);
            dependencies.push_back(std::move(dependency));
        }
        root.insert(key, std::move(dependencies));
    }

    // write deps.toml
    const fs::path toml_abs = path / "deps.toml";
    std::ofstream output(toml_abs, std::ios::trunc);
    if(!output) {
        throw std::runtime_error("Failed to open deps.toml for writing : " + toml_abs.string());
    }
    output << root;
    if(!output) {
        throw std::runtime_error("failed to write deps.toml : " + toml_abs.string());
    }
}

// if the source file has a dependency hash, returns it
// otherwise returns std::nullopt
std::optional<std::string> Package::get_dependency_hash(const fs::path& source_file) {
    if(dependency_hashes.contains(source_file)) {
        return dependency_hashes[source_file];
    }   
    else {
        return std::nullopt;
    }
}

void Package::set_dependency_hash(const fs::path& source_file, std::string hash) {
    dependency_hashes[source_file] = hash;
}

// writes dependency hashes to pack->path / hash.toml
// only writes hashes of source files that exist
void Package::write_dependency_hashes() {
    // generate hash.toml
    std::vector<fs::path> all_source_files = find_all_source_files();
    sort(all_source_files.begin(), all_source_files.end());
    toml::table root;
    for(const fs::path& source_file : all_source_files) {
        if(!dependency_hashes.contains(source_file)) {
            continue;
        }

        std::string key = source_file.string();
        root.insert(key, dependency_hashes[source_file]);
    }

    // write hash.toml
    const fs::path toml_abs = path / "hash.toml";
    std::ofstream output(toml_abs, std::ios::trunc);
    if(!output) {
        throw std::runtime_error("Failed to open hash.toml for writing : " + toml_abs.string());
    }
    output << root;
    if(!output) {
        throw std::runtime_error("failed to write hash.toml : " + toml_abs.string());
    }
}
