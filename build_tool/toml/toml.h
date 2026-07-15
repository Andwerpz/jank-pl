#include <string>
#include <variant>
#include <unordered_map>
#include <memory>

#include "parser.h"

// simple TOML library
// should parse and do semantic checks

// we're using v1.1.0
// toml.io/en/v1.1.0

namespace toml {
    struct value;
    struct table;
    struct array;

    // a value can be anything
    struct value {
        using storage = std::variant<
            std::string,
            int64_t,
            bool,
            std::unique_ptr<toml::table>,
            std::unique_ptr<toml::array>
        >;
        storage data;

        value() = delete;
        value(const value::storage& data);

        // don't want these being copied around
        // can move them tho
        value(const value&) = delete;
        value& operator=(const value&) = delete;
        value(value&&) noexcept = default;
        value& operator=(value&&) noexcept = default;

        // indexing into a value treats it like a table or an array
        // throws a runtime error if it's a type mismatch or the entry doesn't exist
        value& operator[](const std::string& key);
        value& operator[](const int& ind);

        bool is_string();
        bool is_int();
        bool is_bool();
        bool is_table();
        bool is_array();

        toml::table& get_table();
        toml::array& get_array();
        std::string get_string();
        int64_t get_int();
        bool get_bool();
    };

    // a table holds an unordered list key value pairs
    struct table {
        std::unordered_map<std::string, std::unique_ptr<toml::value>> entries;

        // used during initialization
        bool is_defined = false;    

        table() = default;

        // don't want these being copied around
        // can move them tho
        table(const table&) = delete;
        table& operator=(const table&) = delete;
        table(table&&) noexcept = default;
        table& operator=(table&&) noexcept = default;

        bool contains(const std::string& key) const;
        value& operator[](const std::string& key);

        void add_entry(const std::string& key, toml::value&& val);
        void remove_entry(const std::string& key);
    };


    // an array holds an ordered list of values
    struct array {
        std::vector<std::unique_ptr<toml::value>> elements;

        // used during initialization
        bool is_table_array = false;

        array() = default;

        // don't want these being copied around
        // can move them tho
        array(const array&) = delete;
        array& operator=(const array&) = delete;
        array(array&&) noexcept = default;
        array& operator=(array&&) noexcept = default;

        size_t size() const;
        value& operator[](const int& ind);
        void push_back(toml::value&& val);
    };

    toml::table* parse(const std::string& filepath);
};

