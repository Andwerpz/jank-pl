#include "GlobalNode.h"
#include "Identifier.h"
#include "utils.h"

GlobalNode::GlobalNode(parser::token *tok) : ASTNode(tok) {
    // do nothing
}

GlobalNode::GlobalNode(const GlobalNode& other) : ASTNode(other) {
    id = other.id->make_copy();
    for(int i = 0; i < other.dependencies.size(); i++) dependencies.push_back(other.dependencies[i]->make_copy());
}

GlobalNode::GlobalNode(Identifier *_id, std::vector<Identifier*> _dependencies) : ASTNode() {
    id = _id;
    dependencies = _dependencies;
    assert(id != nullptr);
    for(int i = 0; i < dependencies.size(); i++) assert(dependencies[i] != nullptr);
}

GlobalNode* GlobalNode::convert(parser::global_node *gn) {
    GlobalNode* result = new GlobalNode(gn);
    result->id = Identifier::convert(gn->t2);
    if(gn->t3.has_value()) {
        result->dependencies = convert_identifier_list(gn->t3.value()->t2);
    }
    return result;
}

GlobalNode* GlobalNode::make_copy() {
    return new GlobalNode(*this);
}