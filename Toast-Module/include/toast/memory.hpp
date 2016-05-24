#pragma once

#include "toast/internal/shm.hpp"
#include "toast/library.hpp"
#include "toast/module.hpp"

#include <string>
#include <stdlib.h>

namespace Toast {
    namespace Memory {
        namespace Internal {
            int initialize(std::string private_mempool_id);
            void finalize_load();
            
            void load_error(char state);
            void set_restart(bool restartable);
            
            void bind_info(Toast::ModuleInfo *info);
            
            std::string private_mempool_id();
            char *get_private_block();
            
            char *get_shared_block();
        };
        
        API char *private_block();
        API char *shared_block();
        
        class Bridge {
            public:
                Bridge(std::string name, int size);
                void create();
                void open();
                char *get();
                void destroy();
                void zero();
            private:
                SHM_HANDLE _handle;
                std::string _name;
                int _size;
                char *_block;
        };
    }
}