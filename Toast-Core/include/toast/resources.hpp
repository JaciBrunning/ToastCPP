#pragma once

#include "toast/library.hpp"

#include <string>

namespace Toast {
	namespace Resources {
		API std::string get_resource_file(std::string module, std::string name);
	}
}