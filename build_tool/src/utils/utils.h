#pragma once
#include <string>
#include <optional>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

std::string read_file(const std::string& filename);
std::string read_cstr(char* s);
std::vector<std::string> str_split(const std::string& s, char sep);
std::string extract_filename(std::string path);             // given path to file, gives you the filename
std::string extract_folder_path(std::string path);          // given path to file, gives you path to folder the file is in
std::string extract_stem(std::string filename);             // given filename 'foo.jank', gives you 'foo'
std::string extract_ext(std::string filename);              // given filename 'foo.jank', gives you 'jank'
std::string normalize_path(std::string path);               // given an absolute filepath, removes all relative moves ("..", ".")

