#include "toast/module/memory.hpp"
#include "toast/memory.hpp"

using namespace Toast;
using namespace std;

static string __mempool_id;
static SHM_HANDLE __shm_handle;
static char *__private_block;

// -- MODULE INTERNALS -- //

int Memory::Module::initialize(string private_mempool_id, int module_idx) {
    Memory::initialize();
    
    __mempool_id = private_mempool_id;
    __shm_handle = Toast::Internal::SHM::open_shm_file(private_mempool_id);
    __private_block = Toast::Internal::SHM::map_shm_file(__shm_handle, TOAST_PRIVATE_MEMPOOL_SIZE);
    
    if (__private_block[1] == PMP_VERIFY) {
        __private_block[1] = PMP_VALID;             // Valid Block
        int status = Memory::Shared::get()[0x10 + module_idx];
        Memory::Shared::get()[0x10 + module_idx] = status == 0x03 ? 0x04 : 0x02;
        return 0;
    } else {
        __private_block[1] = PMP_INVALID;           // Block Validation Error
        return -1;
    }
}

void Memory::Module::finalize_load() {
    if (__private_block[1] == PMP_VALID)
        __private_block[1] = PMP_LOAD_COMPLETE;     // Loading finalized properly
}

void Memory::Module::load_error(char state) {
    __private_block[1] = state;                     // Loading encountered an error, set the error condition
    Memory::Module::set_restart(false);             // Internal load errors may not restart
}

void Memory::Module::set_restart(bool restartable) {
    if (restartable) {
        __private_block[2] = 0x01;                  // This module may restart
    } else {
        __private_block[2] = 0x00;
    }
}

void Memory::Module::bind_info(Toast::ModuleInfo *info) {
    memcpy(&__private_block[0x10], info->name, 48);
    __private_block[3] = 0x01;                      // Information has been Set
    Memory::Module::set_restart(info->restartable);
}

string Memory::Module::private_mempool_id() {
    return __mempool_id;
}

char *Memory::Module::get_private_block() {
    return __private_block;
}

char *Memory::private_block() {
    return Memory::Module::get_private_block();
}