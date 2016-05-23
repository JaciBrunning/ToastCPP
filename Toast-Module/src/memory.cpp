#include "toast/memory.hpp"

using namespace Toast;
using namespace std;

static string __mempool_id;
static SHM_HANDLE __shm_handle;
static char *__private_block;

// -- MODULE INTERNALS -- //

int Memory::Internal::initialize(string private_mempool_id) {
    __mempool_id = private_mempool_id;
    __shm_handle = Toast::Internal::SHM::open_shm_file(private_mempool_id);
    __private_block = Toast::Internal::SHM::map_shm_file(__shm_handle, TOAST_PRIVATE_MEMPOOL_SIZE);
    
    if (__private_block[0] == PMP_VERIFY) {
        __private_block[0] = PMP_VALID;             // Valid Block
        return 0;
    } else {
        __private_block[0] = PMP_INVALID;           // Block Validation Error
        return -1;
    }
}

void Memory::Internal::finalize_load() {
    if (__private_block[0] == PMP_VALID)
        __private_block[0] = PMP_LOAD_COMPLETE;     // Loading finalized properly
}

void Memory::Internal::load_error(char state) {
    __private_block[0] = state;                     // Loading encountered an error, set the error condition
    Memory::Internal::set_restart(false);           // Internal load errors may not restart
}

void Memory::Internal::set_restart(bool restartable) {
    if (restartable) {
        __private_block[1] = 0x01;                  // This module may restart
    } else {
        __private_block[1] = 0x00;
    }
}

void Memory::Internal::bind_info(Toast::ModuleInfo *info) {
    memcpy(&__private_block[10], info->name, 50);
    memcpy(&__private_block[60], info->unique, 50);
    Memory::Internal::set_restart(info->restartable);
}

string Memory::Internal::private_mempool_id() {
    return __mempool_id;
}

char *Memory::Internal::get_private_block() {
    return __private_block;
}