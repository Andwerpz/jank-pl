#include <iostream>
#include <string>
#include <iterator>
#include "../../../compiler/parser/parser.h"
#include <set>

std::set<std::string> keywords = {
	"for", "while", "if", "else", "return", "struct", "template", "operator", "extern", "export", "new",
	"typedef", "#include", "#global_node",
	"syscall", "sizeof", "asm!",
};

std::set<std::string> builtin_types = {
	"u8", "u16", "u32", "u64",
	"i8", "i16", "i32", "i64",
	"f32",
	"void",
};

struct highlight_token {
	std::string type;
	int line;
	int col;
	int length;

	highlight_token(parser::token *tok, std::string _type) {
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

void generate_highlight_tokens(parser::token *tok, bool in_struct);

std::vector<highlight_token> out_tokens;

void generate_highlight_tokens(parser::token *tok, bool in_struct) {
	//types
	if(tok->token_type == "base_type") {
		//builtin types
		if(builtin_types.count(tok->to_string())) {
			out_tokens.push_back(highlight_token(tok, "keyword"));
		}
		else {
			out_tokens.push_back(highlight_token(tok, "type"));
		}
		return;
	}

	//strings
	if(tok->token_type == "literal_string" || tok->token_type == "literal_char" ||
		tok->token_type == "library_path" || tok->token_type == "inline_asm_string") {
		out_tokens.push_back(highlight_token(tok, "string"));
		return;
	}

	//keywords
	if(tok->token_type == "terminal" && keywords.count(tok->to_string())) {
		out_tokens.push_back(highlight_token(tok, "keyword"));
		return;
	}
	if(in_struct && tok->token_type == "identifier" && tok->to_string() == "this") {
		out_tokens.push_back(highlight_token(tok, "keyword"));
		return;
	}

	//numbers
	if(tok->token_type == "literal_integer" || tok->token_type == "literal_float" || tok->token_type == "literal_hex" || tok->token_type == "literal_binary") {
		out_tokens.push_back(highlight_token(tok, "number"));
		return;	
	}

	//comments
	if(tok->token_type == "line_comment" || tok->token_type == "multiline_comment") {
		out_tokens.push_back(highlight_token(tok, "comment"));
		return;
	}

	//generate highlighting for children
	if(tok->token_type == "struct_definition") in_struct = true;
	for(int i = 0; i < tok->token_children.size(); i++){
		generate_highlight_tokens(tok->token_children[i], in_struct);
	}
}

int main() {
	//read in everything from stdin
	std::string input((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());

	//use parser
	parser::set_s(input);
	parser::program *p = parser::program::parse();
	if(!parser::check_finished_parsing(false)) {
		std::cout << "[]\n";
		return 0;
	}
	p->postprocess();

	//generate highlighting tokens
	generate_highlight_tokens(p, false);

	//output
	std::cout << "[";
	for(int i = 0; i < out_tokens.size(); i++){
		std::cout << out_tokens[i].to_json_str();
		if(i != out_tokens.size() - 1) std::cout << ", ";
	}
	std::cout << "]";

	return 0;
}

