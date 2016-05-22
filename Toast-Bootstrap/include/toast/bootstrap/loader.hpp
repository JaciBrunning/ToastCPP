#pragma once

#include "toast/internal/loader.hpp"
#include "toast/environment.hpp"
#include <vector>
#include <string>

#ifdef OS_WIN
    #include <Windows.h>
    #include <direct.h>
    #include <process.h>
#else
    #include <dlfcn.h>
    #include <unistd.h>
#endif


namespace Toast {
    namespace Bootstrap {
        namespace Loader {
            void initialize();
            
            void search_modules();
            void create_subprocesses();
        }
    }
}

// TODO move to class model