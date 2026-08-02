#include "utils.h"
#include <openssl/evp.h>
#include <sstream>

// -- GENERAL UTILS --
// executes the given executable with the given arguments
// returns the status code
// if suppress_successful_stdout is true, then does not print STDOUT on exec success
int exec(fs::path bin_path, std::vector<std::string> args, bool suppress_successful_stdout = true) {
    std::vector<std::string> command_args = {
        bin_path.string(),
    };
    command_args.insert(command_args.end(), args.begin(), args.end());

    int stdout_pipe[2] = {-1, -1};
    if(suppress_successful_stdout && pipe(stdout_pipe) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork();
    if(pid == 0) {
        if(suppress_successful_stdout) {
            // send child stdout into the pipe
            close(stdout_pipe[0]);
            if(dup2(stdout_pipe[1], STDOUT_FILENO) == -1) {
                perror("dup2 failed");
                close(stdout_pipe[1]);
                _exit(1);
            }
            close(stdout_pipe[1]);
        }

        std::vector<char*> argv;
        argv.reserve(command_args.size() + 1);
        for (std::string& arg : command_args) {
            argv.push_back(arg.data());
        }
        argv.push_back(nullptr);

        execvp(bin_path.string().c_str(), argv.data());
        perror("compiler exec failed");
        _exit(1);
    }

    // first read back child stdout
    std::string captured_stdout;
    if(suppress_successful_stdout) {
        close(stdout_pipe[1]);
        char buffer[4096];
        while(true) {
            ssize_t bytes_read = read(stdout_pipe[0], buffer, sizeof(buffer));
            if(bytes_read > 0) {
                captured_stdout.append(buffer, static_cast<size_t>(bytes_read));
            }
            else if(bytes_read == 0) {
                break;
            }
            else if(errno != EINTR) {
                perror("read failed");
                break;
            }
        }
        close(stdout_pipe[0]);
    }

    // then wait on child
    int status;
    while(waitpid(pid, &status, 0) == -1) {
        if(errno != EINTR) {
            perror("waitpid failed");
            return 1;
        }
    }

    bool succeeded = WIFEXITED(status) && WEXITSTATUS(status) == 0;
    if(!succeeded && suppress_successful_stdout) {
        std::cout << captured_stdout;
        std::cout.flush();
    }
    return status;
}

// compiles the file at src_path into out_path
// assumes src_path and out_path are absolute
// returns 0 on success, nonzero on failure
int compile(fs::path src_path, fs::path out_path, std::vector<std::string> args) {
    std::vector<std::string> command_args = {
        src_path,
        "-o",
        out_path
    };
    command_args.insert(command_args.end(), args.begin(), args.end());

    std::cout << "Compiling : " << src_path.string() << std::endl;
    return exec("jjc", command_args);
}

// links and assembles the given assembly files into out_path
// assumes all paths in asm_paths and out_path are absolute
// returns 0 on success, nonzero on failure
int assemble(std::vector<fs::path> asm_paths, fs::path out_path) {
    std::vector<std::string> command_args = {
        "-g",                           // debug metadata, should probably be optional
        "-x", "assembler",              //gcc expects .s files to be assembly, tell it that all files are assembly
        "-nostartfiles", "-nostdlib",   //tell gcc that we're not compiling C assembly
        "-m64"                          //64 bit mode?
    };
    command_args.insert(command_args.end(), asm_paths.begin(), asm_paths.end());
    command_args.push_back("-o");
    command_args.push_back(out_path);

    std::cout << "Assembling : " << out_path.string() << std::endl;
    return exec("gcc", command_args);
}

// writes the provided toml file to the given path
// assumes that the path doesn't correspond to an existing directory
// assumes that the path is an absolute path
void write_toml(const toml::table& toml, const fs::path path) {
    if(fs::is_directory(path)) {
        throw std::runtime_error("Tried to write toml to directory : " + path.string());
    }
    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("Failed to open file : " + path.string());
    }
    output << toml;
    if (!output) {
        throw std::runtime_error("failed to write toml to file : " + path.string());
    }
    output.close();
}

// returns true if root is an ancestor of target
// use to tell if a directory contains a file
bool is_ancestor(const fs::path& root, const fs::path& target) {
    // normalize paths to resolve "." and ".." components lexically
    auto canon_root = root.lexically_normal();
    auto canon_target = target.lexically_normal();

    // compare path elements
    // don't want to take a string prefix because something like
    //   "/home/bla", "/home/bla.txt"
    // would then be considered to be containing
    auto [target_it, prefix_it] = std::mismatch(
        canon_target.begin(), canon_target.end(),
        canon_root.begin(), canon_root.end()
    );
    
    // if we reached the end of the root, it is an ancestor of target
    return prefix_it == canon_root.end();
}

// assumes that root is an ancestor of target
// returns the relative path from root to target
fs::path get_relative_path(const fs::path& root, const fs::path& target) {
    auto canon_root = root.lexically_normal();
    auto canon_target = target.lexically_normal();

    assert(is_ancestor(canon_root, canon_target));
    fs::path rel_path = fs::path(canon_target).lexically_relative(canon_root);
    return rel_path;
}

// copies the given directory from src to dir
// before copying, creates dir if it doesn't exist
// if it already exists, dir is overwritten
// assumes that src exists
void copy_directory(const fs::path& src, const fs::path& dir) {
    assert(fs::exists(src));
    fs::create_directories(dir);
    auto options = fs::copy_options::recursive | fs::copy_options::overwrite_existing;
    fs::copy(src, dir, options);
}


// -- HASHING --
Sha256Hash sha256(std::span<const std::uint8_t> data) {
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if(context == nullptr) {
        throw std::runtime_error("EVP_MD_CTX_new failed");
    }
    if(
        EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1 ||
        EVP_DigestUpdate(context, data.data(), data.size()) != 1
    ) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("SHA-256 initialization/update failed");
    }
    Sha256Hash hash{};
    unsigned int hash_size = 0;
    if(EVP_DigestFinal_ex(context, hash.data(), &hash_size) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("SHA-256 finalization failed");
    }
    EVP_MD_CTX_free(context);
    if(hash_size != hash.size()) {
        throw std::runtime_error("Unexpected SHA-256 digest size");
    }
    return hash;
}

Sha256Hash sha256(std::string str) {
    return sha256(std::span<const std::uint8_t>((std::uint8_t*) str.data(), str.size()));
}

// computes hash of the contents of the file
// assumes that path is absolute
Sha256Hash sha256_file(const fs::path& path) {
    std::ifstream file(path, std::ios::binary);
    if(!file) {
        throw std::runtime_error("Failed to open file : " + path.string());
    }
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if(context == nullptr) {
        throw std::runtime_error("EVP_MD_CTX_new failed");
    }
    if(EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("SHA-256 initialization failed");
    }
    std::array<std::uint8_t, 64 * 1024> buffer{};
    while(file) {
        file.read(
            reinterpret_cast<char*>(buffer.data()),
            static_cast<std::streamsize>(buffer.size())
        );
        std::streamsize bytes_read = file.gcount();
        if(bytes_read > 0 && EVP_DigestUpdate(context, buffer.data(), static_cast<std::size_t>(bytes_read)) != 1) {
            EVP_MD_CTX_free(context);
            throw std::runtime_error("SHA-256 update failed");
        }
    }
    if(!file.eof()) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed while reading file: " + path.string());
    }
    Sha256Hash hash{};
    unsigned int hash_size = 0;
    if(EVP_DigestFinal_ex(context, hash.data(), &hash_size) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("SHA-256 finalization failed");
    }
    EVP_MD_CTX_free(context);
    if(hash_size != hash.size()) {
        throw std::runtime_error("Unexpected SHA-256 digest size");
    }
    return hash;
}

// combine hashes by hashing their concatenation
Sha256Hash combine_hashes(const Sha256Hash& first, const Sha256Hash& second) {
    std::array<std::uint8_t, 64> combined{};
    std::copy(first.begin(), first.end(), combined.begin());
    std::copy(second.begin(), second.end(), combined.begin() + first.size());
    Sha256Hash ret = sha256(combined);
    return ret;
}

std::string hash_to_hex(const Sha256Hash& hash) {
    constexpr char digits[] = "0123456789abcdef";
    std::string result;
    result.resize(hash.size() * 2);
    for(std::size_t i = 0; i < hash.size(); ++i) {
        result[i * 2]     = digits[hash[i] >> 4];
        result[i * 2 + 1] = digits[hash[i] & 0x0f];
    }
    return result;
}

Sha256Hash hex_to_hash(const std::string& hex) {
    if(hex.size() != 64) {
        throw std::invalid_argument("Invalid SHA-256 hex length: expected 64 characters");
    }
    auto hex_digit_value = [](char c) -> std::uint8_t {
        if(c >= '0' && c <= '9') {
            return static_cast<std::uint8_t>(c - '0');
        }
        if(c >= 'a' && c <= 'f') {
            return static_cast<std::uint8_t>(c - 'a' + 10);
        }
        if(c >= 'A' && c <= 'F') {
            return static_cast<std::uint8_t>(c - 'A' + 10);
        }
        throw std::invalid_argument(std::string("Invalid hexadecimal character: ") + c);
    };
    Sha256Hash hash{};
    for(std::size_t i = 0; i < hash.size(); ++i) {
        std::uint8_t high = hex_digit_value(hex[i * 2]);
        std::uint8_t low  = hex_digit_value(hex[i * 2 + 1]);
        hash[i] = static_cast<std::uint8_t>((high << 4) | low);
    }
    return hash;
}



