#include "toast/memory.hpp"

#include "toast/util.hpp"
#include "toast/net/transport.hpp"

#include "toast/logger.hpp"

using namespace Toast;
using namespace Toast::Concurrent;
using namespace std;

static SHM_HANDLE __shm_handle_shared;
static char *__shared_block;
static Memory::SharedPool _shared;

static Memory::SharedMutex _mtx;

void init_shared() {
	_shared.map_to(__shared_block);
}

void init_mutexes(bool own) {
	_mtx.logger = new IPCMutex("logger", 1, own);

	_mtx.onboard_accel = new IPCMutex("core_ob_accel", 1, own);
	_mtx.analog_out = new IPCMutex("core_an_out", 2, own);
	_mtx.analog_in = new IPCMutex("core_an_in", 8, own);
	_mtx.dio = new IPCMutex("core_dio", 26, own);
	_mtx.relay = new IPCMutex("core_relay", 4, own);
	_mtx.pcm = new IPCMutex("core_pcm", 2, own);
	_mtx.motor = new IPCMutex("core_motors", 26, own);
	_mtx.pwm = new IPCMutex("core_pwm", 20, own);
	_mtx.servo = new IPCMutex("core_servo", 20, own);
	_mtx.joy = new IPCMutex("core_joystick", 6, own);
	_mtx.power = new IPCMutex("core_power", 2, own);	// ID 0: PDP, ID 1: Controller
}

void Memory::initialize_bootstrap() {
    __shm_handle_shared = Internal::SHM::create_shm_file(TOAST_SHARED_MEMPOOL_NAME, Memory::SharedPool::SIZE);
    __shared_block = Internal::SHM::map_shm_file(__shm_handle_shared, Memory::SharedPool::SIZE);

	memset(__shared_block, 0, Memory::SharedPool::SIZE);
	init_shared();
	init_mutexes(true);
    
    // Write the Process ID to the Shared Pool for client processes to work with
	_shared.set_bootstrap_pid(get_pid());
	_shared.set_endian(Memory::get_endian_bit());
}

void Memory::initialize() {
    __shm_handle_shared = Toast::Internal::SHM::create_shm_file(TOAST_SHARED_MEMPOOL_NAME, Memory::SharedPool::SIZE);
    __shared_block = Toast::Internal::SHM::map_shm_file(__shm_handle_shared, Memory::SharedPool::SIZE);

	init_shared();
	init_mutexes(false);
}

char Memory::get_endian_bit() {
    union {
        int i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1 ? 1 : 2;  // 1 = Big, 2 = Little
}

void Memory::copy_private_pool(int module_idx, char *buffer) {
    Memory::Bridge b("module_private_" + to_string(module_idx), Memory::PrivatePool::SIZE);
    b.open();
    memcpy(buffer, b.get(), b.size());
    b.destroy();
}

// -- SHARED MEMORY BLOCK STUFF -- //

Memory::SharedPool *Memory::shared() {
	return &_shared;
}

Memory::SharedMutex *Memory::shared_mutex() {
	return &_mtx;
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