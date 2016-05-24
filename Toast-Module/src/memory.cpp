#include "toast/memory.hpp"

using namespace Toast;
using namespace std;

static string __mempool_id;
static SHM_HANDLE __shm_handle;
static char *__private_block;

static SHM_HANDLE __shm_handle_shared;
static char *__shared_block;

// -- MODULE INTERNALS -- //

int Memory::Internal::initialize(string private_mempool_id) {
    __mempool_id = private_mempool_id;
    __shm_handle = Toast::Internal::SHM::open_shm_file(private_mempool_id);
    __private_block = Toast::Internal::SHM::map_shm_file(__shm_handle, TOAST_PRIVATE_MEMPOOL_SIZE);
    
    __shm_handle_shared = Toast::Internal::SHM::create_shm_file(TOAST_SHARED_MEMPOOL_NAME, TOAST_SHARED_MEMPOOL_SIZE);
    __shared_block = Toast::Internal::SHM::map_shm_file(__shm_handle_shared, TOAST_SHARED_MEMPOOL_SIZE);
    
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
    __private_block[2] = 0x01;                      // Information has been Set
    Memory::Internal::set_restart(info->restartable);
}

string Memory::Internal::private_mempool_id() {
    return __mempool_id;
}

char *Memory::Internal::get_private_block() {
    return __private_block;
}

char *Memory::Internal::get_shared_block() {
    return __shared_block;
}

char *Memory::private_block() {
    return Memory::Internal::get_private_block();
}

char *Memory::shared_block() {
    return Memory::Internal::get_shared_block();
}

Memory::Bridge::Bridge(string name, int size) {
    _name = name;
    _size = size;
}

void Memory::Bridge::create() {
    _handle = Toast::Internal::SHM::create_shm_file(_name, _size);
    _block = Toast::Internal::SHM::map_shm_file(_handle, _size);
}

void Memory::Bridge::open() {
    _handle = Toast::Internal::SHM::open_shm_file(_name);
    _block = Toast::Internal::SHM::map_shm_file(_handle, _size);
}

char *Memory::Bridge::get() {
    return _block;
}

void Memory::Bridge::destroy() {
    Toast::Internal::SHM::unmap_shm_file(_block, _size);
    Toast::Internal::SHM::close_shm_file(_name, _handle);
}

void Memory::Bridge::zero() {
    memset(_block, 0, _size);
}