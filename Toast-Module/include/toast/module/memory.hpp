#pragma once

#include "toast/internal/shm.hpp"
#include "toast/library.hpp"
#include "toast/module.hpp"
#include "toast/memory.hpp"

#include <string>
#include <stdlib.h>

namespace Toast {
    namespace Memory {
        namespace Module {
            int initialize(std::string private_mempool_id, int module_idx);
            void finalize_load();
            
            void bind_info(Toast::ModuleInfo *info);
            
            std::string private_mempool_id();
			API Memory::PrivatePool *get_private();
        };
        
		API Memory::PrivatePool *get_private();
    }
}