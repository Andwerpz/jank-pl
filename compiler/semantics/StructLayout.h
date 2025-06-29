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
    std::vector<MemberVariable*> member_variables;
    std::vector<std::pair<Identifier*, int>> offset_map;
    int size;

    StructLayout(std::vector<MemberVariable*> _member_variables, std::vector<std::pair<Identifier*, int>> _offset_map, int _size);

    int get_offset(Identifier *id);
    Type* get_type(Identifier *id);
    int get_size();       
};