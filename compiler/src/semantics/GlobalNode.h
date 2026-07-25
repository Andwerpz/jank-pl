#pragma once
#include "../parser/parser.h"
#include "ASTNode.h"

struct Identifier;

struct GlobalNode : public ASTNode {
    Identifier *id;
    std::vector<Identifier*> dependencies;

    GlobalNode(parser::token *tok);
    GlobalNode(const GlobalNode& other);
    GlobalNode(Identifier *_id, std::vector<Identifier*> _dependencies);

    static GlobalNode* convert(parser::global_node *gn);
    GlobalNode* make_copy() override;
};