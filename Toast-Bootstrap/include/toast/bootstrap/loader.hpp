#pragma once

#include "toast/internal/loader.hpp"
#include "toast/environment.hpp"
#include "toast/internal/shm.hpp"
#include <vector>
#include <string>
#include <thread>
#include <stdlib.h>

#ifdef OS_WIN
    #include <Windows.h>
    #include <direct.h>
    #include <process.h>
#else
    #include <dlfcn.h>
    #include <unistd.h>
#endif

#define TOAST_SPECIFIC_MEMPOOL_SIZE 256


namespace Toast {
    namespace Bootstrap {
        namespace Loader {
            class ModuleAdapter {
            public:
                ModuleAdapter() { }
                std::string file;
                std::string given_unq;
                
                SHM_HANDLE specific_mempool_handle;
                char *specific_mempool;
            };
            
            void initialize();
            
            void search_modules();
            void create_subprocesses();
            
            void create_module_process(ModuleAdapter *adapter);
            std::string super_horrible_hash_func(std::string original);
            
            void create_specific_mempool(ModuleAdapter *adapter);
            void free_specific_mempool(ModuleAdapter *adapter);
        }
    }
}

// TODO move to class model