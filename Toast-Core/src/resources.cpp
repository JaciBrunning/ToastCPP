#include "toast/resources.hpp"
#include "toast/filesystem.hpp"

using namespace Toast;

std::string Resources::get_resource_file(std::string module, std::string name) {
	return Filesystem::join("resources/" + module, name);
}