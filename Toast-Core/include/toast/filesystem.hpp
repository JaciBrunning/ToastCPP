#pragma once

#include <string>
#include <vector>
#include "toast/library.hpp"
#include <string.h>

namespace Toast {
    namespace Filesystem {
        API std::string join(std::string path1, std::string path2);
        API std::string path(std::string path);
        API std::string path_mkdir(std::string path);
        API std::string absolute(std::string path);
        API void rmdir(std::string path);
        API void mkdir(std::string path);
        API std::vector<std::string> ls(std::string path);
        API std::vector<std::string> ls_local(std::string path);
        
        API void initialize();
    }
}