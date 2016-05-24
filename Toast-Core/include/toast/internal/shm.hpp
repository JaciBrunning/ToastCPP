#pragma once

#include "toast/environment.hpp"
#include "toast/filesystem.hpp"

#include <string>

// The Toast Shared Memory Internals

// 128 Bytes. We only really need about 53 Bytes, but we have extra
// space for future use. In cases where this runs out, the last
// 16 Bytes are reserved for pointer(s) to a dedicated memory pool
// that contains the full information. For the most part, this
// expansion requires a specified protocol that may or may not
// be currently implemented.
#define TOAST_PRIVATE_MEMPOOL_SIZE 128

// 1 Megabyte. We don't actually need nearly this much, but we have
// places that are unallocated for future use, in case WPILib or 
// Toast itself adds functionality that requires Module <--> Bootstrap
// talk. The advantage of this is that modules that were built with
// older versions of Toast don't require a rebuild, as this value
// is unlikely to ever need expansion.
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