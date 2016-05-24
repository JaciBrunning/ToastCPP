#pragma once

#include "toast/internal/shm.hpp"
#include "toast/library.hpp"

#include <string>
#include <stdlib.h>

namespace Toast {
    namespace Bootstrap {
        namespace Memory {
            void initialize();
            
            namespace Shared {
                char *get();
                void zero();
            }
        }
    }
}