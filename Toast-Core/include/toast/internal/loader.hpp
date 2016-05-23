#pragma once

#include "toast/environment.hpp"
#include "toast/filesystem.hpp"
#include <string>

#ifdef OS_WIN
    #include <windows.h>
    #define DYN_LD LoadLibrary
    #define DYN_SYM GetProcAddress
    #define DYN_ERR Toast::Internal::Loader::Platform::win_dlerror
    #define SYMBOL FARPROC
    #define DYNAMIC HMODULE
    
    #define OS_LIB ".dll"
    #define OS_PREFIX ""
    #define OS_SUFFIX ""
#else
    #include <dlfcn.h>
    #define DYN_LD Toast::Internal::Loader::Platform::unix_dlopen
    #define DYN_SYM dlsym
    #define DYN_ERR dlerror
    #define SYMBOL void*
    #define DYNAMIC void*
    
    #define OS_PREFIX "lib"
    #ifdef OS_MAC
        #define OS_LIB ".dylib"
        #define OS_SUFFIX ".dylib"
    #else
        #define OS_LIB ".so"
        #define OS_SUFFIX ".so"
    #endif
#endif

namespace Toast {
    namespace Internal {
        namespace Loader {
            namespace Platform {
                #ifdef OS_UNIX
                    DYNAMIC unix_dlopen(char *file);
                #endif
                
                #ifdef OS_WIN
                    std::string win_dlerror();
                #endif
            }
            
            API void initialize();
            
            API DYNAMIC load_dynamic_library(std::string file);
            API SYMBOL get_symbol(DYNAMIC library, std::string sym);
            API std::string get_dynamic_error();
            
            API std::string library_name(std::string library);
        }
    }
}