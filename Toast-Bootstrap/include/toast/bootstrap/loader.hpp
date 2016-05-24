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

namespace Toast {
    namespace Bootstrap {
        namespace Loader {
            class ModuleAdapter {
            public:
                ModuleAdapter() { }
                std::string file;
                std::string given_unq;
                
                SHM_HANDLE private_mempool_handle;
                char *private_mempool;
                
                void clear() {
                    name_cache.clear();
                    memset(private_mempool, 0, TOAST_PRIVATE_MEMPOOL_SIZE);
                }
                
                void dirty() {
                    name_cache.clear();
                }
                
                std::string get_name();
            private:
                std::string name_cache;
            };
            
            void initialize();
            
            void search_modules();
            void create_subprocesses();
            
            void create_module_process(ModuleAdapter *adapter);
            std::string super_horrible_hash_func(std::string original);
            
            void create_shared_mempool();
            void free_shared_mempool();
            void create_private_mempool(ModuleAdapter *adapter);
            void free_private_mempool(ModuleAdapter *adapter);
        }
    }
}

// TODO move to class model