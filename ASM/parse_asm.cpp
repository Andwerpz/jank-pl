#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

// Token types
enum class TokenType {
    Directive, Label, Mnemonic, Register,
    Immediate, Symbol, Separator, String,
    Comment, EOL, Unknown
};

struct Token {
    TokenType type;
    std::string text;
};

// Utility string functions
bool starts_with(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && str.substr(0, prefix.size()) == prefix;
}

bool ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// Classify token type
TokenType classify_token(const std::string& token) {
    if (starts_with(token, ".")) return TokenType::Directive;
    if (ends_with(token, ":")) return TokenType::Label;
    if (starts_with(token, "%")) return TokenType::Register;
    if (starts_with(token, "$")) return TokenType::Immediate;
    if (token == "," || token == ":") return TokenType::Separator;
    if (starts_with(token, "\"") && ends_with(token, "\"")) return TokenType::String;
    if (starts_with(token, "#")) return TokenType::Comment;
    if (std::regex_match(token, std::regex("[a-zA-Z]+"))) return TokenType::Mnemonic;
    return TokenType::Symbol;
}

// Tokenize a single line
std::vector<Token> tokenize_line(const std::string& line) {
    std::vector<Token> tokens;
    std::istringstream stream(line);
    std::string word;

    while (stream >> word) {
        word = trim(word);
        if (word.empty()) continue;
        TokenType type = classify_token(word);
        if (type == TokenType::Comment) break;  // skip the rest of the line
        tokens.push_back({ type, word });
    }

    tokens.push_back({ TokenType::EOL, "" });
    return tokens;
}

// Display tokens
void print_tokens(const std::vector<Token>& tokens) {
    for (const auto& tok : tokens) {
        std::cout << "Token: [" << tok.text << "]\tType: ";
        switch (tok.type) {
            case TokenType::Directive: std::cout << "Directive"; break;
            case TokenType::Label: std::cout << "Label"; break;
            case TokenType::Mnemonic: std::cout << "Mnemonic"; break;
            case TokenType::Register: std::cout << "Register"; break;
            case TokenType::Immediate: std::cout << "Immediate"; break;
            case TokenType::Symbol: std::cout << "Symbol"; break;
            case TokenType::Separator: std::cout << "Separator"; break;
            case TokenType::String: std::cout << "String"; break;
            case TokenType::Comment: std::cout << "Comment"; break;
            case TokenType::EOL: std::cout << "EOL"; break;
            default: std::cout << "Unknown"; break;
        }
        std::cout << '\n';
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./asm_tokenizer <filename>\n";
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << '\n';
        return 1;
    }

    std::string line;
    while (std::getline(infile, line)) {
        auto tokens = tokenize_line(line);
        print_tokens(tokens);
        std::cout << '\n';
    }

    return 0;
}
