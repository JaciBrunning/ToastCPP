#pragma once

#include "toast/internal/loader.hpp"
#include "toast/environment.hpp"
#include "toast/memory.hpp"
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
                int idx;
                
                SHM_HANDLE private_mempool_handle;
                char *private_mempool;
				Memory::PrivatePool priv;
                
                void clear() {
                    name_cache.clear();
                    memset(private_mempool, 0, Memory::PrivatePool::SIZE);
                }
                
                void dirty() {
                    name_cache.clear();
                }
                
                std::string get_mempool_id() {
                    return "module_private_" + std::to_string(idx);
                }
                
                std::string get_name();
            private:
                std::string name_cache;
            };

            void initialize();
			void free();

            void search_modules();
            void create_subprocesses();
            
            void create_module_process(ModuleAdapter *adapter);
            
            void create_private_mempool(ModuleAdapter *adapter);
            void free_private_mempool(ModuleAdapter *adapter);
        }
    }
}