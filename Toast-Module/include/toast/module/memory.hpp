#pragma once

#include "toast/internal/shm.hpp"
#include "toast/library.hpp"
#include "toast/module.hpp"

#include <string>
#include <stdlib.h>

namespace Toast {
    namespace Memory {
        namespace Module {
            int initialize(std::string private_mempool_id);
            void finalize_load();
            
            void load_error(char state);
            void set_restart(bool restartable);
            
            void bind_info(Toast::ModuleInfo *info);
            
            std::string private_mempool_id();
            char *get_private_block();
        };
        
        API char *private_block();
    }
}