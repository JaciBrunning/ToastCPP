#pragma once

#include "toast/library.hpp"
#include "toast/logger.hpp"

#include "json11.h"

#include <string>
#include <sstream>
#include <vector>

namespace Toast {
    class Config {
    public:
        API Config(std::string config_name);
        API Config *load();
        API Config *reload();
        
        API static void unpack(Json *master_obj, std::string key, Json def);
        API static Json deepMerge(Json a1, Json a2);
        
        API Json getObject(std::string name);
        API std::string to_string();
        API std::string to_string(int indent);
        
        API bool has(std::string name);
        API void put_default(std::string key, Json value);
        API Json get_or_default(std::string name, Json def);
        API Json get(std::string name, Json def);
        
        API int get_int(std::string name, int def);
        API double get_double(std::string name, double def);
        API float get_float(std::string name, float def);
        API long get_long(std::string name, long def);
        API bool get_bool(std::string name, bool def);
        API std::string get_string(std::string name, std::string def);
        
        API std::vector<Json> get_vector(std::string name, std::vector<Json> def);
        
        API Json *get_root_obj();
        API std::string get_config_name();
        API std::string get_config_file();
        
    private:
        Json master_obj;
        Json defaults_obj;
        std::string name;
        std::string file;
        Toast::Logger _log;
        bool _first_load;
    };
}