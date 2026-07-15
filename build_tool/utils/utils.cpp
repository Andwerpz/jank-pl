#include <chrono>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <iostream>
#include <fstream>

#include "utils.h"

std::string read_file(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::ostringstream buffer;
    buffer << file.rdbuf(); 
    return buffer.str();     
}

std::vector<std::string> str_split(const std::string& s, char sep) {
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

std::string extract_folder_path(std::string path) {
    for(int i = (int) path.size() - 1; i >= 0; i--){
        if(path[i] == '/') return path.substr(0, i + 1);
    }
    //there is no '/', just return "./"
    return "./";
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

std::string normalize_path(std::string path) {
    std::vector<std::string> parts = str_split(path, '/');
    std::vector<std::string> nparts;
    for(int i = 0; i < parts.size(); i++) {
        if(parts[i] == ".") continue;
        if(parts[i] == "..") {
            if(nparts.size() > 0) nparts.pop_back();
            continue;
        }
        nparts.push_back(parts[i]);
    }
    path = "";
    for(int i = 0; i < nparts.size(); i++) {
        path += "/" + nparts[i];
    }
    return path;
}

