#pragma once

#include "toast/internal/shm.hpp"
#include "toast/library.hpp"

#include <string>
#include <stdlib.h>

namespace Toast {
    namespace Memory {
        API void initialize_bootstrap();
        API void initialize();
        API char get_endian_bit();
        
        namespace Shared {
            API char *get();
            API void zero();
            
            API void set_debug(bool is_debug);
            API bool get_debug();
            
            API void set_bootstrap_pid(int pid);
            API int get_bootstrap_pid();
        }
        
        class Bridge {
            public:
                API Bridge(std::string name, int size);
                
                API void create();
                API void open();
                API char *get();
                API void destroy();
                API void close();
                API void zero();
                API int size();
            private:
                SHM_HANDLE _handle;
                std::string _name;
                int _size;
                char *_block;
        };
        
        API void copy_private_pool(int module_idx, char *buffer);
    }
}