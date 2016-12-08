#pragma once

#include "toast/library.hpp"

#include <string>

namespace Toast {
	namespace Resources {
		struct Resource {
			std::string file;
			long size, offset, _trx_offset;
			API FILE *open();
			API void seek(FILE *fp);
			API void close(FILE *fp);
		};
		API void link_trx_file(std::string name, std::string filename);
		API Resource *get_resource_file(std::string module, std::string name);
		API std::string read_resource(Resource *resource);
		API int read_resource(Resource *resource, char *buffer);
		API std::string get_and_read(std::string module, std::string name);
	}
}