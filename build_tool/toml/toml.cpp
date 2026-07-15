#include "toml.h"
#include "../utils/utils.h"
#include "parser.h"

namespace toml {
    
    // decomposes the key into the list of steps. 
    std::vector<std::string> convert_key(parser::key* key) {

    }

    // if this successfully can define a new key-value pair at the provided key, returns a pointer to the value. 
    // otherwise throws an exception. 
    toml::value* new_value(toml::table* root_table, parser::key* key) {

    }

    // if this successfully can define a new table at the provided key, returns a pointer to it
    // otherwise throws an exception. 
    // if in_array is true, expects the key to resolve to an array of tables
    toml::table* new_table(toml::table* root_table, parser::key* key, bool in_array) {
        std::vector<std::string> key_strs = convert_key(key);
        bool last_is_array = false;
        assert(key_strs.size() >= 1);
        for(int i = 0; i < key_strs.size(); i++) {
            std::string str = key_strs[i];

            // get the value at the next step
            // if the next step doesn't exist, create a table here
            // if in_array and this is the last step, should instead create a table array
            if(!root_table->contains(str)) {
                if(i == key_strs.size() - 1 && in_array) {
                    toml::array arr;
                    arr.is_table_array = true;
                    toml::value val(std::make_unique<toml::array>(arr));
                    root_table->add_entry(str, std::move(val));
                }
                else {
                    toml::value val(std::make_unique<toml::table>());
                    root_table->add_entry(str, std::move(val));
                }
            }
            assert(root_table->contains(str));
            toml::value& val = (*root_table)[str];

            // table array semantics
            if(i == key_strs.size() - 1) {
                if(in_array && !val.is_array()) {
                    throw std::runtime_error("TOML semantic error : table array entry didn't resolve to array");
                }
                if(!in_array && val.is_array()) {
                    throw std::runtime_error("TOML semantic error : non table array entry resolved to array");
                }
            }

            // get the next root table
            root_table = nullptr;
            if(val.is_table()) {
                root_table = &val.get_table();
            }
            else if(val.is_array()) {
                toml::array& arr = val.get_array();
                if(!arr.is_table_array) {
                    throw std::runtime_error("TOML semantic error : table indexing into non table array");
                }
                
                // if this is the last step, append a new array onto the end
                // otherwise, just take the last table in the array
                if(i == key_strs.size() - 1) {
                    assert(in_array);   // this has already been checked above
                    toml::table table;
                    toml::value val(std::make_unique<toml::table>(table));
                    arr.push_back(std::move(val));
                    root_table = &table;
                }
                else {
                    assert(arr.size() > 0);
                    assert(arr[arr.size() - 1].is_table());
                    root_table = &(arr[arr.size() - 1].get_table());
                }
            }
            else {
                throw std::runtime_error("TOML semantic error : invalid table path");
            }
            assert(root_table != nullptr);
        }
        if(root_table->is_defined) {
            throw std::runtime_error("TOML semantic error : table redefinition");
        }
        root_table->is_defined = true;
        return root_table;
    }
    
    toml::table parse(std::string filepath) {
        // open da file
        std::string toml_str = read_file(filepath);

        // parse da file
        parser::set_s(toml_str);
        parser::set_gen_errors(false);
        parser::toml* toml = parser::toml::parse();
        if(!parser::check_finished_parsing(true)) {
            throw std::runtime_error("TOML syntax error : " + filepath);
        }

        // extract statements
        std::vector<parser::statement*> statements;
        for(parser::toml::a0* x : toml->t0) {
            statements.push_back(x->t1);
        }

        // add statements to table in order
        toml::table root_table;
        toml::table* active_table = nullptr;
        for(parser::statement* statement : statements) {
            if(statement->is_a0) {          // key-value pair
                parser::key_value* kv = statement->t0->t0;
                
            }
            else if(statement->is_a1) {     // table 
                parser::table* t = statement->t1->t0;
            }
            else assert(false);
        }

        return table;
    }

    // -- VALUE --

    // -- TABLE --

    // -- ARRAY --

}

