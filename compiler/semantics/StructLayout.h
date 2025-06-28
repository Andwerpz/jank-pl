#pragma once
#include <vector>
#include <unordered_map>
#include <string>

struct MemberVariable;
struct Identifier;
struct StructDefinition;
struct Type;

//holds information on where all the stuff is supposed to go within the heap portion of the struct
// - for every identifier, stores the relative offset within the struct
// - also holds the total size of the heap portion in bytes
struct StructLayout {
    StructLayout *layout;
    bool processed;

    std::vector<MemberVariable*> member_variables;
    std::vector<std::pair<Identifier*, int>> offset_map;
    std::unordered_map<std::string, int> offset_map;
    int size;

    StructLayout(StructDefinition *_layout);

    int get_offset(Identifier *id);
    Type* get_type(Identifier *id);
    int get_size();       
};