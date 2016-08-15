#pragma once

#include "toast/environment.hpp"
#include "toast/filesystem.hpp"

#include <string>

// The Toast Shared Memory Internals
#define TOAST_SHARED_MEMPOOL_NAME "shared_public"
#define TOAST_SHARED_MEMPOOL_MTX_NAME "shared_public_MTX"

#ifdef OS_WIN
    #include <windows.h>
    #include <conio.h>
    #include <tchar.h>
    
    #define SHM_HANDLE HANDLE
#else
    #include <stdlib.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <sys/mman.h>
    #include <fcntl.h>
    
    #define SHM_HANDLE int
#endif

namespace Toast {
    namespace Internal {
        namespace SHM {
            API SHM_HANDLE create_shm_file(std::string name, int size);
            API SHM_HANDLE open_shm_file(std::string name);
            API char *map_shm_file(SHM_HANDLE handle, int size);
            API void unmap_shm_file(void *addr, int size);
            API void close_shm_file(std::string name, SHM_HANDLE handle);
        }
    }
}