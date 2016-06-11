#include "toast/config.hpp"
#include "toast/filesystem.hpp"
#include "toast/util.hpp"

#include <fstream>

using namespace Toast;
using namespace std;

Config::Config(string config_name) : _log("Config-" + config_name), _first_load(true) {
    name = config_name;
    file = Filesystem::path("config/" + config_name + ".json");
    defaults_obj = Json::object();
}

Config *Config::load() {
    return reload();
}

Config *Config::reload() {
    string json_config = "";
    
    try {
        ifstream file_in(file);
        string tmp;
        while (getline(file_in, tmp)) {
            json_config += tmp + "\n";
        }
        file_in.close();
    } catch (const std::exception &e) { }
    
    if (trim(json_config).empty()) json_config = "{}";
    try {
        Json obj = Json::parse(json_config);
        master_obj = deepMerge(defaults_obj, obj);
        string json = to_string(4);
        
        ofstream file_out;
        file_out.open(file);
        file_out << json << endl;
        file_out.close();
    } catch (const std::exception &e) {
        // Parsing error, it's okay, we'll fall back to what we had before. The user likely
        // made a mistake when editing that produced malformed JSON.
        
        if (_first_load) {
            _log.error("Error! JSON File is malformed! Substituting default values!");
            Json emt = Json::object();
            master_obj = deepMerge(defaults_obj, emt);
        } else {
            _log.error("Error! JSON File is malformed! Using old values!");
        }
        
        string json = to_string(4);
        
        ofstream file_out;
        file_out.open(file);
        file_out << json << endl;
        file_out.close();
    }
    _first_load = false;
    return this;
}

void Config::unpack(Json *master_obj, string key, Json def) {
    vector<string> split_str = split(key, '.');
    Json last_obj = *master_obj;
    for (int i = 0; i < split_str.size(); i++) {
        string cur = split_str[i];
        if (!(last_obj.has(cur) && last_obj.get(cur).type() == Json::Type::OBJECT)) {
            Json obj = Json::object();
            last_obj.set(cur, obj);
        }
        if (i == split_str.size() - 1) {
            last_obj.set(cur, def);
        } else {
            last_obj = last_obj.get(cur);
        }
    }
}

Json Config::deepMerge(Json a1, Json a2) {
    Json d = Json::object();
    Json c = Json::object();
    
    for (auto key : a1.keys()) {
        unpack(&d, key, a1.get(key));
    }
    
    for (auto key : a2.keys()) {
        unpack(&c, key, a2.get(key));
    }
    
    for (auto kC : c.keys()) {
        Json vC = c[kC];
        if (d.has(kC)) {
            if (vC.type() == Json::Type::OBJECT) {
                d.set(kC, deepMerge(d.get(kC), vC));
            } else {
                d.set(kC, vC);
            }
        } else {
            d.set(kC, vC);
        }
    }
    
    return d;
}

Json Config::getObject(string name) {
    try {
        vector<string> split_str = split(name, '.');
        Json last_obj = master_obj;
        for (auto key : split_str) {
            last_obj = last_obj.get(key);
        }
        return last_obj;
    } catch (const std::exception &e) {}
    Json obj;
    return obj;
}

string Config::to_string() {
    return master_obj.stringify();
}

string Config::to_string(int indent) {
    return master_obj.stringify(indent);
}

bool Config::has(string name) {
    return master_obj.has(name);
}

void Config::putDefault(string key, Json value) {
    defaults_obj.set(key, value);
    reload();
}

Json Config::getOrDefault(string name, Json def) {
    if (!has(name)) {
        putDefault(name, def);
    }
    Json get = getObject(name);
    return get.type() == Json::Type::JSNULL ? def : get;
}

Json Config::get(string name, Json def) {
    return getOrDefault(name, def);
}

int Config::get_int(string name, int def) {
    return (int) get(name, Json(def));
}

double Config::get_double(string name, double def) {
    return (double) get(name, Json(def));
}

float Config::get_float(string name, float def) {
    return (float) get(name, Json(def));
}

long Config::get_long(string name, long def) {
    return (long) get(name, Json(def));
}

bool Config::get_bool(string name, bool def) {
    return (bool) get(name, Json(def));
}

string Config::get_string(string name, string def) {
    return (string) get(name, Json(def));
}

vector<Json> Config::get_vector(string name, vector<Json> def) {
    Json arr = Json::array();
    for (Json t : def) {
        arr << t;
    }
    Json arr2 = get(name, arr);
    vector<Json> ret;
    for (int i = 0; i < arr2.size(); i++) {
        ret.push_back(Json(arr2[i]));
    }
    return ret;
}

Json *Config::get_root_obj() {
    return &master_obj;
}

string Config::get_config_name() {
    return name;
}

string Config::get_config_file() {
    return file;
}