#include "toast/bootstrap/loader.hpp"
#include "toast/filesystem.hpp"
#include "toast/util.hpp"
#include "toast/logger.hpp"
#include "toast/memory.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace Toast;
using namespace Toast::Bootstrap;
using namespace std;

static vector<Loader::ModuleAdapter *> __modules;
static Logger _log("Bootstrap-Loader");
static char _load_idx = 0x00;

string Loader::ModuleAdapter::get_name() {
    if (!name_cache.empty()) return name_cache;
    if (private_mempool[ADDR_INFO_SET] != 0x01) return file;
    char name[LEN_MOD_NAME];
    memcpy(name, &private_mempool[ADDR_MOD_NAME], LEN_MOD_NAME);
    name_cache = string(name);
    return name_cache;
}

void Loader::initialize() {
    Loader::search_modules();
    Loader::create_subprocesses();
}

void Loader::search_modules() {
    vector<string> module_files = Filesystem::ls_local(Filesystem::path("modules"));
    for (auto module : module_files) {
        if (ends_with(module, OS_LIB)) {
            Loader::ModuleAdapter *adapter = new Loader::ModuleAdapter();
            adapter->file = module;
            adapter->idx = (int)(_load_idx);
            Memory::Shared::get()[ADDR_MOD_ACT_STATE + _load_idx] = 0x01;
            _load_idx += 1;
            if (_load_idx == 128) {
                // Overflow
                _log.error("Wow! You've loaded more than 128 modules. Unfortunately, we don't support more than 128 modules due to technical limitations. File a bug report to the ToastC++ Repository on OpenRIO and we'll start working on support for more modules!");
                _log.error("Any further modules will be ignored.");
            } else {
                __modules.push_back(adapter);
            }
            _log.debug("Discovered Module File: " + module);
        } else {
            _log.warn("Module File " + module + " does not have a valid file extension! Are you sure this is the correct download for your platform? Valid File Extension: " + OS_LIB);
        }
    }
}

void Loader::create_subprocesses() {
    for (auto module : __modules) {
        Loader::create_module_process(module);
        Memory::Shared::get()[ADDR_MOD_ACT_STATE + module->idx] = 0x03;
    }
	sleep_ms(10000);
}

static void create_process(Loader::ModuleAdapter *adapter) {
    string mempool_handle = adapter->get_mempool_id();
    string file = adapter->file;
    
    adapter->private_mempool[1] = PMP_VERIFY;    // Verification Byte
    memcpy(adapter->private_mempool + ADDR_MOD_FILE, file.c_str(), file.length());
    
    _log.debug("Launching Process for Module: " + adapter->file);
    #ifdef OS_WIN
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        
        if (CreateProcess(NULL, 
            (char *)("toast __TOAST_MODULE_LD_FILE " + file + " " + mempool_handle + " " + to_string(adapter->idx)).c_str(), 
            NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    #else
        system(("./toast __TOAST_MODULE_LD_FILE " + file + " " + mempool_handle + " " + to_string(adapter->idx)).c_str());
    #endif
}

static void create_process_thread(Loader::ModuleAdapter *adapter) {
    _log.debug("Creating Private Mempool for " + adapter->file + " (id: " + adapter->get_mempool_id() + ")");
    create_private_mempool(adapter);
    do {
        adapter->clear();
        create_process(adapter);
        _log.warn("Module " + adapter->get_name() + " has exited or crashed! " + (adapter->private_mempool[1] == 0x01 ? "Restarting..." : "Not Restarting..."));
    } while (adapter->private_mempool[ADDR_RESTART] == 0x01);
}

void Loader::create_module_process(Loader::ModuleAdapter *adapter) {
    _log.debug("Creating Module Process Thread for " + adapter->file);
    thread t(create_process_thread, adapter);
    t.detach();
}

void Loader::create_private_mempool(Loader::ModuleAdapter *adapter) {
    adapter->private_mempool_handle = Internal::SHM::create_shm_file(adapter->get_mempool_id(), TOAST_PRIVATE_MEMPOOL_SIZE);
    adapter->private_mempool = Internal::SHM::map_shm_file(adapter->private_mempool_handle, TOAST_PRIVATE_MEMPOOL_SIZE);
    adapter->private_mempool[ADDR_ENDIAN] = Memory::get_endian_bit();
}

void Loader::free_private_mempool(Loader::ModuleAdapter *adapter) {
    Internal::SHM::unmap_shm_file(adapter->private_mempool, TOAST_PRIVATE_MEMPOOL_SIZE);
    Internal::SHM::close_shm_file(adapter->get_mempool_id(), adapter->private_mempool_handle);
}