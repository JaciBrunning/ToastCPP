#pragma once

#include "toast/internal/shm.hpp"
#include "toast/library.hpp"

#include "shared.fwi.hpp"
#include "private.fwi.hpp"

#include <string>
#include <stdlib.h>

// Impl
namespace Toast {
    namespace Memory {
        API void initialize_bootstrap();
        API void initialize();
        API char get_endian_bit();
        
		API SharedPool *shared();

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