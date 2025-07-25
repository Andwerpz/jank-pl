#include "GlobalNode.h"
#include "Identifier.h"
#include "utils.h"

GlobalNode::GlobalNode(Identifier *_id, std::vector<Identifier*> _dependencies) {
    id = _id;
    dependencies = _dependencies;
    assert(id != nullptr);
    for(int i = 0; i < dependencies.size(); i++) assert(dependencies[i] != nullptr);
}

GlobalNode* GlobalNode::convert(parser::global_node *gn) {
    Identifier *id = Identifier::convert(gn->t2);
    std::vector<Identifier*> dependencies;
    if(gn->t3 != nullptr) {
        dependencies = convert_identifier_list(gn->t3->t2);
    }
    return new GlobalNode(id, dependencies);
}

GlobalNode* GlobalNode::make_copy() {
    std::vector<Identifier*> _dependencies;
    for(int i = 0; i < dependencies.size(); i++) _dependencies.push_back(dependencies[i]->make_copy());
    return new GlobalNode(id->make_copy(), _dependencies);
}