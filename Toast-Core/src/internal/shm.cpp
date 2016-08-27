#include "toast/internal/shm.hpp"

using namespace Toast;
using namespace Toast::Internal;
using namespace std;

SHM_HANDLE SHM::create_shm_file(string name, int size) {
    #ifdef OS_WIN
	string c_s = "Local\\TOAST_" + name;
        return CreateFileMapping(
            INVALID_HANDLE_VALUE,
            nullptr,
            PAGE_READWRITE,
            0,
            size,
	    (char *)c_s.c_str()
        );
    #else
	string c_s = "/TOAST_" + name;
        return shm_open(
            (char *)c_s.c_str(),
            O_RDWR | O_CREAT,
            S_IRUSR | S_IWUSR
        );
    #endif
}

SHM_HANDLE SHM::open_shm_file(string name) {
    #ifdef OS_WIN
	string c_s = "Local\\TOAST_" + name;
        return OpenFileMapping(
            FILE_MAP_ALL_ACCESS,
            FALSE,
	    (char *)c_s.c_str()
        );
    #else
        string c_s = "/TOAST_" + name;
	return shm_open(
            (char *)c_s.c_str(),
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
	string c_s = "/TOAST_" + name;
        shm_unlink((char *)c_s.c_str());
    #endif
}
