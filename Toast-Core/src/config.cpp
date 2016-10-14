#include "toast/config.hpp"
#include "toast/filesystem.hpp"
#include "toast/util.hpp"

#include <fstream>

using namespace Toast;
using namespace std;

enum CVAL_Type {
	cvt_float, cvt_integer, cvt_bool, cvt_string, cvt_child
};

struct CVAL_Container {
	const char *key;
	void *ptr;
	int type;
};

ConfigBase::ConfigBase() {
	alloc_size = 10;
	mem = malloc(alloc_size * sizeof(CVAL_Container));
	run_size = 0;
}

static void insert(ConfigBase *cfg, CVAL_Container container) {
	if (cfg->run_size >= cfg->alloc_size) {
		cfg->alloc_size += 10;
		cfg->mem = realloc(cfg->mem, cfg->alloc_size);
	}
	CVAL_Container *m = (CVAL_Container *)cfg->mem;
	m[cfg->run_size] = container;
	cfg->run_size++;
}

float ConfigBase::config_var(const char *key, float *ptr, float def) {
	insert(this, { key, ptr, cvt_float } );
	return def;
}

int ConfigBase::config_var(const char *key, int *ptr, int def) {
	insert(this, { key, ptr, cvt_integer });
	return def;
}

bool ConfigBase::config_var(const char *key, bool *ptr, bool def) {
	insert(this, { key, ptr, cvt_bool });
	return def;
}

std::string ConfigBase::config_var(const char *key, std::string *ptr, std::string def) {
	insert(this, { key, ptr, cvt_string });
	return def;
}

bool ConfigBase::config_sub(const char *key, ConfigBase *ptr) {
	insert(this, { key, ptr, cvt_child });
	return true;
}

void ConfigBase::free() const {
	::free(mem);
}

// Config

Config::Config(std::string filename) : _filename(filename), log("Config-" + filename) { }

std::string Config::get_file() {
	return Filesystem::path("config/" + _filename + ".json");
}

static bool skip_whitespace(const char **data) {
	while (**data != 0 && (**data == ' ' || **data == '\t' || **data == '\r' || **data == '\n'))
		(*data)++;

	return **data != 0;
}

// Convert a quoted string to a native string based on the JSON spec
static bool pull_string(const char **data, std::string &out) {
	(*data)++;
	out = "";
	while (**data != 0) {
		char nc = **data;
		if (nc == '\\') {
			(*data)++;
			switch (**data) {
			case '"': nc = '"'; break;
			case '\\': nc = '\\'; break;
			case '/': nc = '/'; break;
			case 'b': nc = '\b'; break;
			case 'f': nc = '\f'; break;
			case 'n': nc = '\n'; break;
			case 'r': nc = '\r'; break;
			case 't': nc = '\t'; break;
			case 'u': {
				if (strlen(*data) < 5) return false;
				nc = 0;
				for (int i = 0; i < 4; i++) {
					(*data)++;
					nc <<= 4;
					if (**data >= '0' && **data <= '9') 
						nc |= (**data - '0');
					else if (**data >= 'A' && **data <= 'F') 
						nc |= (10 + (**data - 'A'));
					else if (**data >= 'a' && **data <= 'f') 
						nc |= (10 + (**data - 'a'));
					else return false;
				}
				break;
			}
			default:
				return false;
			}
		} else if (nc == '"') {
			(*data)++;
			out.reserve();
			return true;
		} else if (nc < ' ' && nc != '\t')	// Not Allowed (invalid)
			return false;

		out += nc;
		(*data)++;
	}
	return false;
}

static float parse_num(const char **data) {
	float num = 0;
	while (**data != 0 && **data >= '0' && **data <= '9')
		num = num * 10 + (*(*data)++ - '0');

	return num;
}

static float parse_decimal(const char **data) {
	float decimal = 0.0;
	float factor = 0.1;
	while (**data != 0 && **data >= '0' && **data <= '9') {
		int digit = (*(*data)++ - '0');
		decimal = decimal + digit * factor;
		factor *= 0.1;
	}
	return decimal;
}

static bool json_parse(const char **data, CVAL_Container *cfg) {
	bool qb, qs; // Quote Big, Quote Small

	if (!skip_whitespace(data)) return false;
	if (cfg->type == cvt_bool) {
		if (strncmp(*data, "true", 4) == 0) {
			*(bool *)(cfg->ptr) = true;
			(*data) += 4;
			return true;
		} else if (strncmp(*data, "false", 5) == 0) {
			*(bool *)(cfg->ptr) = false;
			(*data) += 5;
			return true;
		} else {
			return false;
		}
	} else if (cfg->type == cvt_string) {
		if (**data == '"') {
			std::string tmp;
			std::string *ptr = (std::string *)cfg->ptr;
			bool success = pull_string(data, tmp);
			// This prevents a partial write to configuration resulting in a malformed variable
			// if the config is malformed, as opposed to falling back to the default value and rewriting
			// the configuration file.
			if (success) *ptr = tmp;	
			return success;
		} else {
			return false;
		}
	} else if (cfg->type == cvt_child) {
		if (**data == '{') {
			(*data)++;
			while (**data != 0) {
				if (!skip_whitespace(data)) return false;
				if (**data == '}') {
					(*data)++;
					return true;
				}

				std::string name;
				if (!pull_string(data, name) || 
					!skip_whitespace(data) || 
					*((*data)++) != ':' ||
					!skip_whitespace(data)) return false;

				// Find CFG
				ConfigBase *cfg_obj = (ConfigBase *)cfg->ptr;
				CVAL_Container *cont = NULL, *members = (CVAL_Container *)cfg_obj->mem;
				for (int i = 0; i < cfg_obj->run_size; i++) {
					if (strncmp(members[i].key, name.c_str(), name.length()) == 0) {
						cont = &members[i];
						break;
					}
				}

				if (cont == NULL) {
					// Key was not valid
					return false;
				}

				bool success = json_parse(data, cont);
				if (!success) return false;

				if (!skip_whitespace(data)) return false;
				if (**data == '}') {
					(*data)++;
					return true;
				}

				if (**data != ',') return false;

				(*data)++;
			}

			return false;
		} else {
			return false;
		}
	} else if (**data == '-' || (**data >= '0' && **data <= '9')) {
		bool negative = **data == '-';
		if (negative) (*data)++;

		float number = 0.0;
		if (**data == '0') 
			(*data)++;
		else if (**data >= '1' && **data <= '9') {
			number = parse_num(data);
		} else 
			return false;

		if (**data == '.') {
			(*data)++;
			if (!(**data >= '0' && **data <= '9'))
				return false;
			number += parse_decimal(data);
		}

		if (**data == 'E' || **data == 'e') {
			(*data)++;
			bool neg_expo = false;
			if (**data == '-' || **data == '+') {
				neg_expo = **data == '-';
				(*data)++;
			}

			if (!(**data >= '0' && **data <= '9'))
				return false;

			float expo = parse_num(data);
			for (float i = 0.0; i < expo; i++)
				number = neg_expo ? (number / 10.0) : (number * 10.0);
		}

		if (negative) number *= -1;

		if (cfg->type == cvt_integer) {
			*(int *)(cfg->ptr) = (int)number;
		} else if (cfg->type == cvt_float) {
			*(float *)(cfg->ptr) = (float)number;
		} else 
			return false;

		return true;
	}
	return false;
}

static std::string indent_str(int indent) {
	std::string a(indent, '\t');
	return a;
}

static void generate_json(ostream &out, CVAL_Container *cont, int indent, bool comma) {
	if (cont->type == cvt_child) {
		out << "{\n";
		ConfigBase *cfg_obj = (ConfigBase *)cont->ptr;
		CVAL_Container *children = (CVAL_Container *)(cfg_obj->mem);
		for (int i = 0; i < cfg_obj->run_size; i++) {
			const char *name = children[i].key;
			out << indent_str(indent + 1) << "\"" << name << "\": ";
			generate_json(out, &children[i], indent + 1, i != cfg_obj->run_size - 1);
		}
		out << indent_str(indent) <<  "}";
		if (comma) out << ",";
		out << "\n";
	} else if (cont->type == cvt_bool) {
		bool val = *(bool *)(cont->ptr);
		out << (val ? "true" : "false") << (comma ? ",\n" : "\n");
	} else if (cont->type == cvt_integer) {
		int val = *(int *)(cont->ptr);
		out << val << (comma ? ",\n" : "\n");
	} else if (cont->type == cvt_float) {
		float val = *(float *)(cont->ptr);
		out << val << (comma ? ",\n" : "\n");
	} else if (cont->type == cvt_string) {
		std::string val = *(std::string *)(cont->ptr);
		out << "\"" << val << "\"" << (comma ? ",\n" : "\n");	// TODO: Handle escapes
	}
}

void Config::load() {
	std::string json_config;
	try {
		ifstream file_in(get_file());
		string tmp;
		while (getline(file_in, tmp)) {
			json_config += tmp + "\n";
		}
		file_in.close();
	} catch (const std::exception &e) { }

	if (trim(json_config).empty()) json_config = "{}";

	// DURING PARSE: load the value
	// AFTER PARSE: clear the file and write it anew

	const char *strval = json_config.c_str();
	CVAL_Container cont = { "<root>", this, cvt_child };
	bool result = json_parse(&(strval), &cont);
	if (!result) {
		log.error("Malformed JSON Configuration File! Using defaults...");
	}

	try {
		ofstream file_out;
		file_out.open(get_file());
		generate_json(file_out, &cont, 0, false);
		file_out.close();
	} catch (const std::exception &e) {
		log.error("Could not write JSON Config file (" + get_file() + ")! [" + e.what() + "]");
	}
}