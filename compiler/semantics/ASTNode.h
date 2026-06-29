#pragma once
#include "../parser/parser.h"

// TODO remove line and col information from SourceSpan
// - only need this information for highlighting extension, 
//   really the highlighting extension should generate this information by itself. 

struct SourceSpan {
    // index into filepath vector 
    // TODO actually make this filepath vector
    // - should store list of filepaths relative to CWD
    int file_id;

    // starting line and column (inclusive) 
    int start_ptr;
    int start_line;
    int start_col;

    // ending line and column (exclusive)
    int end_ptr;
    int end_line;
    int end_col;

    // indicates a purely compiler generated ASTNode
    // if this is true, source location information should be invalid
    bool synthesized;
};

struct ASTNode {
    // information regarding where this ASTNode came from
    SourceSpan span;

    // should we be compiling this AST node? (typechecking and emitting assembly)
    // should be true if
    // - this node comes from the target file
    // - this node was generated somehow by the compiler during compilation
    bool should_compile;

    // invoking these have some specific semantic meanings. 
    // these should be mirrored in everything that inherits from ASTNode
    ASTNode(parser::token* token);                          // converting from a parser::token
    ASTNode(const ASTNode& other);                          // copying from existing ASTNode
    ASTNode();                                              // synthesizing new ASTNode

    // should just directly call copy constructor, makes code easier to read
    virtual ASTNode* make_copy() = 0;
};
