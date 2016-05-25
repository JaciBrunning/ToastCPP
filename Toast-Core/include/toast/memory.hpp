#pragma once

#include "toast/internal/shm.hpp"
#include "toast/library.hpp"

#include <string>
#include <stdlib.h>

namespace Toast {
    namespace Memory {
        API void initialize_bootstrap();
        API void initialize();
        
        namespace Shared {
            API char *get();
            API void zero();
            
            API void set_debug(bool is_debug);
            API bool get_debug();
        }
        
        class Bridge {
            public:
                API Bridge(std::string name, int size);
                
                API void create();
                API void open();
                API char *get();
                API void destroy();
                API void zero();
            private:
                SHM_HANDLE _handle;
                std::string _name;
                int _size;
                char *_block;
        };
    }
}