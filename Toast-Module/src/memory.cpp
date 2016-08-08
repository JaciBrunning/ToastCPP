#include "toast/module/memory.hpp"

using namespace Toast;
using namespace std;

static string __mempool_id;
static SHM_HANDLE __shm_handle;
static char *__private_block;
static Memory::PrivatePool _private;

// -- MODULE INTERNALS -- //

int Memory::Module::initialize(string private_mempool_id, int module_idx) {
    Memory::initialize();
    
    __mempool_id = private_mempool_id;
    __shm_handle = Toast::Internal::SHM::open_shm_file(private_mempool_id);
    __private_block = Toast::Internal::SHM::map_shm_file(__shm_handle, Memory::PrivatePool::SIZE);
	_private.map_to(__private_block);
    
	Memory::Verification verify_byte = _private.get_verification();
	Memory::SharedPool *sp = Memory::shared();

    if (verify_byte == Memory::Verification::VERIFY_AWAIT) {
		_private.set_verification(Memory::Verification::VERIFY_CONFIRM);
		Memory::ModuleActState state = sp->get_module_activity_state(module_idx);
		sp->set_module_activity_state(module_idx, state == Memory::ModuleActState::CRASHED ? Memory::ModuleActState::RESTARTED : Memory::ModuleActState::ACTIVE);
        return 0;
    } else {
		_private.set_verification(Memory::Verification::VERIFY_DENIED);
		return -1;
    }
}

void Memory::Module::finalize_load() {
	if (_private.get_verification() == Memory::Verification::VERIFY_CONFIRM)
		_private.set_verification(Memory::Verification::VERIFY_VALID);
}

void Memory::Module::bind_info(Toast::ModuleInfo *info) {
	memcpy(_private.get_module_name(), info->name, _private.module_name_length());
	_private.set_info_set(true);
	_private.set_restart(info->restartable);
}

string Memory::Module::private_mempool_id() {
    return __mempool_id;
}

Memory::PrivatePool *Memory::Module::get_private() {
	return &_private;
}

Memory::PrivatePool *Memory::get_private() {
    return Memory::Module::get_private();
}