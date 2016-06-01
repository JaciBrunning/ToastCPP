#pragma once

#include "toast/library.hpp"
#include "json11.h"

#include <string>
#include <sstream>
#include <vector>

namespace Toast {
    class Config {
    public:
        API Config(std::string config_name);
        void load();
        API void reload();
        
        API static void unpack(Json *master_obj, std::string key, Json def);
        API static Json deepMerge(Json *a1, Json *a2);
        
        API Json getObject(std::string name);
        API Json to_string();
        API Json to_string(int indent);
        
        API bool has(std::string name);
        API void putDefault(std::string key, Json value);
        API Json getOrDefault(std::string name, Json def);
        API Json get(std::string name, Json def);
        
        API int getInt(std::string name, int def);
        API double getDouble(std::string name, double def);
        API float getFloat(std::string name, float def);
        API long getLong(std::string name, long def);
        API bool getBool(std::string name, bool def);
        API std::string getString(std::string name, std::string def);
        
        API std::vector<Json> getVector(std::string name, std::vector<Json> def);
        
        API Json *get_root_obj();
        API std::string get_config_name();
        API std::string get_config_file();
        
    private:
        Json master_obj;
        Json defaults_obj;
        std::string name;
        std::string file;
    };
}