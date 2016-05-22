#include "toast/internal/shm.hpp"

using namespace Toast;
using namespace Toast::Internal;
using namespace std;

SHM_HANDLE SHM::create_shm_file(string name, int size) {
    #ifdef OS_WIN
        return CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            size,
            ("Local\\" + name).c_str()
        );
    #else
        char *file = (char *)("/TOAST_" + name).c_str();
        return shm_open(
            file,
            O_RDWR | O_CREAT,
            S_IRUSR | S_IWUSR
        );
    #endif
}

SHM_HANDLE SHM::open_shm_file(string name) {
    #ifdef OS_WIN
        return OpenFileMapping(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            ("Local\\TOAST_" + name).c_str()
        );
    #else
        char *file = (char *)("/TOAST_" + name).c_str();
        return shm_open(
            file,
            O_RDWR | O_CREAT,
            S_IRUSR | S_IWUSR
        );
    #endif
}

char *SHM::map_shm_file(SHM_HANDLE handle, int size) {
    #ifdef OS_WIN
        return (char *)((PVOID) (
            (LPTSTR) MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, size)
        ));
    #else
        ftruncate(handle, size);
        return (char *) mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, handle, 0);
    #endif
}

void SHM::unmap_shm_file(void *addr, int size) {
    #ifdef OS_WIN
        UnmapViewOfFile((LPTSTR)addr);
    #else
        munmap(addr, size);
    #endif
}

void SHM::close_shm_file(string name, SHM_HANDLE handle) {
    #ifdef OS_WIN
        CloseHandle(handle);
    #else
        char *file = (char *)("/TOAST_" + name).c_str();
        shm_unlink(file);
    #endif
}