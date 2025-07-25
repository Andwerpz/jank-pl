#pragma once
#include "../parser/parser.h"

struct Identifier;

struct GlobalNode {
    Identifier *id;
    std::vector<Identifier*> dependencies;
    GlobalNode(Identifier *_id, std::vector<Identifier*> _dependencies);

    static GlobalNode* convert(parser::global_node *gn);

    GlobalNode* make_copy();
};