#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// ----------------------------
// Example token struct from your lexer
struct Token {
    int line;
    int startChar;
    int length;
    std::string type; // e.g. "keyword", "variable", "number"
    uint32_t modifiers; // bitset
};

// Token type mapping for LSP
std::vector<std::string> tokenTypes = {
    "keyword", "variable", "number", "string", "comment"
};

int getTokenTypeIndex(const std::string &type) {
    for (size_t i = 0; i < tokenTypes.size(); i++)
        if (tokenTypes[i] == type) return (int)i;
    return 0; // default to keyword
}

// ----------------------------
// Simulated lexer (replace with yours)
std::vector<Token> lexSource(const std::string &source) {
    // Fake: mark "int" as keyword, "x" as variable, "5" as number
    return {
        {0, 0, 3, "keyword", 0},
        {0, 4, 1, "variable", 0},
        {0, 8, 1, "number", 0}
    };
}

// Encode tokens in LSP delta format
std::vector<int> encodeTokens(const std::vector<Token> &tokens) {
    std::vector<int> data;
    int prevLine = 0, prevChar = 0;
    for (auto &t : tokens) {
        int lineDelta = t.line - prevLine;
        int charDelta = (lineDelta == 0) ? (t.startChar - prevChar) : t.startChar;
        data.push_back(lineDelta);
        data.push_back(charDelta);
        data.push_back(t.length);
        data.push_back(getTokenTypeIndex(t.type));
        data.push_back(t.modifiers);

        prevLine = t.line;
        prevChar = t.startChar;
    }
    return data;
}

// ----------------------------
// Main LSP loop (super minimal)
int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.rfind("Content-Length:", 0) == 0) {
            int contentLength = std::stoi(line.substr(15));
            std::getline(std::cin, line); // skip empty line

            std::string jsonStr(contentLength, '\0');
            std::cin.read(&jsonStr[0], contentLength);

            auto msg = nlohmann::json::parse(jsonStr);

            std::string method = msg.value("method", "");
            if (method == "initialize") {
                // Reply with capabilities
                nlohmann::json result = {
                    {"capabilities", {
                        {"semanticTokensProvider", {
                            {"legend", {
                                {"tokenTypes", tokenTypes},
                                {"tokenModifiers", std::vector<std::string>{}}
                            }},
                            {"full", true}
                        }}
                    }}
                };
                nlohmann::json resp = {
                    {"jsonrpc", "2.0"},
                    {"id", msg["id"]},
                    {"result", result}
                };
                std::string out = resp.dump();
                std::cout << "Content-Length: " << out.size() << "\r\n\r\n" << out << std::flush;
            }
            else if (method == "textDocument/semanticTokens/full") {
                std::string uri = msg["params"]["textDocument"]["uri"];
                // For now, fake source code; replace with reading file content
                std::string source = "int x = 5;";

                auto tokens = lexSource(source);
                auto data = encodeTokens(tokens);

                nlohmann::json result = { {"data", data} };
                nlohmann::json resp = {
                    {"jsonrpc", "2.0"},
                    {"id", msg["id"]},
                    {"result", result}
                };
                std::string out = resp.dump();
                std::cout << "Content-Length: " << out.size() << "\r\n\r\n" << out << std::flush;
            }
        }
    }
}
