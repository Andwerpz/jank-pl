#include "StructLayout.h"
#include "Identifier.h"
#include "StructDefinition.h"
#include "Type.h"

StructLayout::StructLayout(std::vector<MemberVariable*> _member_variables, std::vector<std::pair<Identifier*, int>> _offset_map, int _size) {
    member_variables = _member_variables;
    offset_map = _offset_map;
    size = _size;
}

int StructLayout::get_offset(Identifier *id) {
    for(int i = 0; i < offset_map.size(); i++){
        if(id->equals(offset_map[i].first)) return offset_map[i].second;
    }
    assert(false);
}

Type* StructLayout::get_type(Identifier *id) {
    for(int i = 0; i < member_variables.size(); i++){
        if(*id == *(member_variables[i]->id)) {
            return member_variables[i]->type;
        }
    }
    return nullptr;
}

int StructLayout::get_size() {
    return size;
}