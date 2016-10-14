#pragma once

#include "toast/library.hpp"
#include "toast/logger.hpp"

#include <string>
#include <sstream>
#include <vector>

#define CFG_PROPERTY(type, key, def)	\
	const char *_key_##key = #key;	\
	type key = config_var(_key_##key, &key, def)

#define SUB_CONFIG(type, key)	\
	type key;					\
	const char *_key_##key = #key;	\
	bool _is_reg_sub_##key = config_sub(_key_##key, &key)

#define CFG_FILE(type, file)	\
	type() : Toast::Config(file) {}
	

namespace Toast {

    struct ConfigBase {
		API ConfigBase();

		API float			config_var(const char *key, float *ptr, float def);
		API int				config_var(const char *key, int *ptr, int def);
		API bool			config_var(const char *key, bool *ptr, bool def);
		API std::string		config_var(const char *key, std::string *ptr, std::string def);

		API bool config_sub(const char *key, ConfigBase *ptr);
		API void free() const;

		void *mem;
		int alloc_size, run_size;
    };

	struct Config : ConfigBase {
		API Config(std::string filename);

		API std::string get_file();
		API void load();

	private:
		Logger log;
		std::string _filename;
	};

	struct SubConfig : ConfigBase { };

}