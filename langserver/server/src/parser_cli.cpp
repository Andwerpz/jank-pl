#include <iostream>
#include <string>
#include <iterator>
#include "../../../compiler/parser/parser.h"
#include <set>

std::set<std::string> keywords = {
	"for", "while", "if", "else", "return", "struct", "template", "operator", "extern", "export", "new",
	"break", "continue",
	"typedef", "#include", "#global_node",
	"syscall", "sizeof", "asm!",
};

std::set<std::string> builtin_types = {
	"u8", "u16", "u32", "u64",
	"i8", "i16", "i32", "i64",
	"f32",
	"void",
};

struct semantic_token {
	std::string type;
	int line;
	int col;
	int length;

	semantic_token(parser::token *tok, std::string _type) {
		type = _type;
		line = tok->start_ctx.line;
		col = tok->start_ctx.line_off;
		length = tok->end_ctx.ptr - tok->start_ctx.ptr;
	}

	std::string to_json_str() {
		std::string ret = "{";
		ret += "\"type\": \"" + type + "\", ";
		ret += "\"line\": " + std::to_string(line) + ", ";
		ret += "\"col\": " + std::to_string(col) + ", ";
		ret += "\"length\": " + std::to_string(length) + "}";
		return ret;
	}
};

struct diagnostic_token {
	std::string message;
	std::string severity;	//one of 'error', 'warning', 'info', 'hint'
	int start_line;
	int start_col;
	int end_line;
	int end_col;

	diagnostic_token(parser::token *tok, std::string _message, std::string _severity) {
		message = _message;
		severity = _severity;
		start_line = tok->start_ctx.line;
		start_col = tok->start_ctx.line_off;
		end_line = tok->end_ctx.line;
		end_col = tok->end_ctx.line_off;
	}

	std::string to_json_str() {
		std::string ret = "{";
		ret += "\"message\": \"" + message + "\", ";
		ret += "\"severity\": \"" + severity + "\", ";
		ret += "\"start_line\": " + std::to_string(start_line) + ", ";
		ret += "\"start_col\": " + std::to_string(start_col) + ", ";
		ret += "\"end_line\": " + std::to_string(end_line) + ", ";
		ret += "\"end_col\": " + std::to_string(end_col) + "}";
		return ret;
	}
};

void generate_tokens(parser::token *tok, bool in_struct);

std::vector<semantic_token> highlight_tokens;
std::vector<diagnostic_token> error_tokens;

void generate_tokens(parser::token *tok, bool in_struct) {
	//types
	if(tok->token_type == "base_type") {
		//builtin types
		if(builtin_types.count(tok->to_string())) {
			highlight_tokens.push_back(semantic_token(tok, "keyword"));
		}
		else {
			highlight_tokens.push_back(semantic_token(tok, "type"));
		}
		return;
	}

	//strings
	if(tok->token_type == "literal_string" || tok->token_type == "literal_char" ||
		tok->token_type == "include_path_relative" || tok->token_type == "include_path_lib" ||
		tok->token_type == "inline_asm_string") {
		highlight_tokens.push_back(semantic_token(tok, "string"));
		return;
	}

	//keywords
	if(tok->token_type == "terminal" && keywords.count(tok->to_string())) {
		highlight_tokens.push_back(semantic_token(tok, "keyword"));
		return;
	}
	if(in_struct && tok->token_type == "identifier" && tok->to_string() == "this") {
		highlight_tokens.push_back(semantic_token(tok, "keyword"));
		return;
	}

	//numbers
	if(
		tok->token_type == "literal_integer" || 
		tok->token_type == "literal_float" || 
		tok->token_type == "literal_hex" || 
		tok->token_type == "literal_binary" || 
		tok->token_type == "literal_octal"
	) {
		highlight_tokens.push_back(semantic_token(tok, "number"));
		return;	
	}

	//comments
	if(tok->token_type == "line_comment" || tok->token_type == "multiline_comment") {
		highlight_tokens.push_back(semantic_token(tok, "comment"));
		return;
	}

	//errors
	if(tok->token_type == "error") {
		error_tokens.push_back(diagnostic_token(tok, "pls fix :3", "error"));
		return;
	}

	//generate highlighting for children
	if(tok->token_type == "struct_definition") in_struct = true;
	for(int i = 0; i < tok->token_children.size(); i++){
		generate_tokens(tok->token_children[i], in_struct);
	}
}

int main() {
	//read in everything from stdin
	std::string input((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());

	//use parser
	parser::set_s(input);
	parser::set_gen_errors(true);
	parser::program *p = parser::program::parse();
	if(!parser::check_finished_parsing(false)) {
		std::cout << "[]\n";
		return 0;
	}
	p->postprocess();

	//generate tokens
	generate_tokens(p, false);

	//output
	std::cout << "{";
	std::cout << "\"highlight_tokens\": [";
	for(int i = 0; i < highlight_tokens.size(); i++){
		std::cout << highlight_tokens[i].to_json_str();
		if(i != highlight_tokens.size() - 1) std::cout << ", ";
	}
	std::cout << "], ";
	std::cout << "\"error_tokens\": [";
	for(int i = 0; i < error_tokens.size(); i++){
		std::cout << error_tokens[i].to_json_str();
		if(i != error_tokens.size() - 1) std::cout << ", ";
	}
	std::cout << "]";
	std::cout << "}";

	return 0;
}

