#pragma once
#include "../parser/parser.cpp"

struct Type {
    static Type* convert(parser::type *t);
    virtual int calc_size() = 0;

    virtual bool equals(const Type *other) const = 0;
    bool operator==(const Type& other) const {return equals(&other);}
    bool operator!=(const Type& other) const {return !equals(&other);}
    virtual size_t hash() const = 0;
    virtual std::string to_string() = 0;
};

struct BaseType : public Type {
    std::string name;
    BaseType(std::string _name) {
        name = _name;
    }
    static BaseType* convert(parser::base_type *t);

    int calc_size() override;
    bool equals(const Type *other) const override;
    size_t hash() const override;
    std::string to_string() override;
};  

struct PointerType : public Type {
    Type *type;
    PointerType(Type *_type) {
        assert(_type != nullptr);
        type = _type;
    }

    int calc_size() override;
    bool equals(const Type *other) const override;
    size_t hash() const override;
    std::string to_string() override;
};

struct ReferenceType : public Type {
    Type *type;
    ReferenceType(Type *_type) {
        assert(_type != nullptr);
        type = _type;
    }

    int calc_size() override;
    bool equals(const Type *other) const override;
    size_t hash() const override;
    std::string to_string() override;
};