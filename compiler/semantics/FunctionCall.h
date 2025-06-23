#pragma once
#include <optional>
#include <vector>
#include "../parser/parser.h"

struct Type;
struct Identifier;
struct Expression;
struct Function;
struct TemplateMapping;

struct FunctionCall {
    std::optional<Type*> target_type;
    Identifier *id;
    std::vector<Expression*> argument_list;
    
    FunctionCall(Identifier *_id, std::vector<Expression*> _argument_list);
    FunctionCall(Type *_target_type, Identifier *_id, std::vector<Expression*> _argument_list);
    FunctionCall(std::optional<Type*> _target_type, Identifier *_id, std::vector<Expression*> _argument_list);

    static FunctionCall* convert(parser::function_call *f);
    Function* resolve_called_function();
    Type* resolve_type();
    void emit_asm();
    std::string to_string();
    size_t hash();
    bool equals(FunctionCall *other);
    FunctionCall* make_copy();
    bool replace_templated_types(TemplateMapping *mapping);
    void look_for_templates();
};