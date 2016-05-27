#include "toast/memory.hpp"

#include "toast/util.hpp"
#include "toast/net/transport.hpp"

#include "toast/logger.hpp"

using namespace Toast;
using namespace std;

static SHM_HANDLE __shm_handle_shared;
static char *__shared_block;

void Memory::initialize_bootstrap() {
    __shm_handle_shared = Internal::SHM::create_shm_file(TOAST_SHARED_MEMPOOL_NAME, TOAST_SHARED_MEMPOOL_SIZE);
    __shared_block = Internal::SHM::map_shm_file(__shm_handle_shared, TOAST_SHARED_MEMPOOL_SIZE);

    Memory::Shared::zero();
    
    // Write the PID to the Shared Pool for client processes to work with
    Memory::Shared::set_bootstrap_pid(get_pid());
}

void Memory::initialize() {
    __shm_handle_shared = Toast::Internal::SHM::create_shm_file(TOAST_SHARED_MEMPOOL_NAME, TOAST_SHARED_MEMPOOL_SIZE);
    __shared_block = Toast::Internal::SHM::map_shm_file(__shm_handle_shared, TOAST_SHARED_MEMPOOL_SIZE);
}

void Memory::copy_private_pool(int module_idx, char *buffer) {
    Memory::Bridge b("module_private_" + to_string(module_idx), TOAST_PRIVATE_MEMPOOL_SIZE);
    b.open();
    memcpy(buffer, b.get(), b.size());
    b.destroy();
}

// -- SHARED MEMORY BLOCK STUFF -- //

void Memory::Shared::zero() {
    memset(__shared_block, 0, TOAST_SHARED_MEMPOOL_SIZE);
}

char *Memory::Shared::get() {
    return __shared_block;
}

void Memory::Shared::set_debug(bool is_debug) {
    __shared_block[0x04] = is_debug ? 0x01 : 0x00;
}

bool Memory::Shared::get_debug() {
    return __shared_block[0x04] == 0x01 ? true : false;
}

void Memory::Shared::set_bootstrap_pid(int pid) {
    Net::Transport::intToBytes(pid, __shared_block, 0);
}

int Memory::Shared::get_bootstrap_pid() {
    return Net::Transport::bytesToInt(__shared_block, 0);
}

// -- BRIDGED MEMORY STUFF -- //

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

void Memory::Bridge::close() {
    Toast::Internal::SHM::close_shm_file(_name, _handle);
}

void Memory::Bridge::zero() {
    memset(_block, 0, _size);
}

int Memory::Bridge::size() {
    return _size;
}