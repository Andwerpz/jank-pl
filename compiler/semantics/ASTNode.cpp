#include "ASTNode.h"
#include "utils.h"

ASTNode::ASTNode() {
    this->span.file_id = -1;
    this->span.start_ptr = -1;
    this->span.start_line = -1;
    this->span.start_col = -1;
    this->span.end_ptr = -1;
    this->span.end_line = -1;
    this->span.end_col = -1;
    this->span.synthesized = true;
}

ASTNode::ASTNode(parser::token* token) {
    assert(token != nullptr);

    assert(source_files.size() >= 1);
    this->span.file_id = source_files.size() - 1;
    this->span.start_ptr = token->start_ctx.ptr;
    this->span.start_line = token->start_ctx.line;
    this->span.start_col = token->start_ctx.col;
    this->span.end_ptr = token->end_ctx.ptr;
    this->span.end_line = token->end_ctx.line;
    this->span.end_col = token->end_ctx.col;
    this->span.synthesized = false;
}

ASTNode::ASTNode(const ASTNode& other) {
    this->span.file_id = other.span.file_id;
    this->span.start_ptr = other.span.start_ptr;
    this->span.start_line = other.span.start_line;
    this->span.start_col = other.span.start_col;
    this->span.end_ptr = other.span.end_ptr;
    this->span.end_line = other.span.end_line;
    this->span.end_col = other.span.end_col;
    this->span.synthesized = other.span.synthesized;
}


