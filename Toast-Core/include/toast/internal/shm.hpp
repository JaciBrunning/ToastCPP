#pragma once

#include "toast/environment.hpp"
#include "toast/filesystem.hpp"

#include <string>

// The Toast Shared Memory Internals

#define TOAST_PRIVATE_MEMPOOL_SIZE 128

#define TOAST_SHARED_MEMPOOL_SIZE (1024 * 1024)
#define TOAST_SHARED_MEMPOOL_NAME "shared_public"

// Private Memory Pool Init/Error Flags

#define PMP_VERIFY  0x7A
#define PMP_VALID   0x7B
#define PMP_LOAD_COMPLETE 0x7C
#define PMP_INVALID 0x80

#define PMP_LOAD_ERR 0x8A
#define PMP_INFO_ERR 0x8B
#define PMP_INIT_ERR 0x8C

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