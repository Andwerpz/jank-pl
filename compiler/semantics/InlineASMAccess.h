#pragma once
#include "../parser/parser.h"

struct Identifier;

// emit_asm should prepare whatever address
// get_addr should spit out some inline dereferencing of that address
// any value that is accessed should be some l-value (so the referencing operator is not valid for this)

struct InlineASMAccess {
    static InlineASMAccess* convert(parser::inline_access *ia);
    virtual bool is_well_formed() = 0;
    virtual void emit_asm() = 0;
    virtual std::string get_addr() = 0;
    virtual InlineASMAccess* make_copy() = 0;
    virtual std::string to_string() = 0;
};

struct InlineASMVariable : public InlineASMAccess {
    Identifier *id;
    InlineASMVariable(Identifier *_id);

    static InlineASMVariable* convert(parser::inline_variable *ia);
    bool is_well_formed() override;
    void emit_asm() override;
    std::string get_addr() override;
    InlineASMAccess* make_copy() override;
    std::string to_string() override;
};

struct InlineASMMemberVariable : public InlineASMAccess {
    Identifier *id;
    std::string op;
    Identifier *member_id;
    InlineASMMemberVariable(Identifier *_id, std::string _op, Identifier *_member_id);

    static InlineASMMemberVariable* convert(parser::inline_member_variable *ia);
    bool is_well_formed() override;
    void emit_asm() override;
    std::string get_addr() override;
    InlineASMAccess* make_copy() override;
    std::string to_string() override;
};

struct InlineASMDereferencing : public InlineASMAccess {
    Identifier *id;
    InlineASMDereferencing(Identifier *_id);

    static InlineASMDereferencing* convert(parser::inline_dereferencing *ia);
    bool is_well_formed() override;
    void emit_asm() override;
    std::string get_addr() override;
    InlineASMAccess* make_copy() override;
    std::string to_string() override;
};
