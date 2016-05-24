#include "toast/bootstrap/memory.hpp"

using namespace Toast;
using namespace Toast::Bootstrap;
using namespace std;

static SHM_HANDLE __shm_handle_shared;
static char *__shared_block;

void Memory::initialize() {
    __shm_handle_shared = Internal::SHM::create_shm_file(TOAST_SHARED_MEMPOOL_NAME, TOAST_SHARED_MEMPOOL_SIZE);
    __shared_block = Internal::SHM::map_shm_file(__shm_handle_shared, TOAST_SHARED_MEMPOOL_SIZE);

    Memory::Shared::zero();
}

char *Memory::Shared::get() {
    return __shared_block;
}

void Memory::Shared::zero() {
    memset(__shared_block, 0, TOAST_SHARED_MEMPOOL_SIZE);
}