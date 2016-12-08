#include "toast/bootstrap/loader.hpp"
#include "toast/filesystem.hpp"
#include "toast/util.hpp"
#include "toast/logger.hpp"
#include "toast/memory.hpp"
#include "toast/resources.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <thread>
#include <stdlib.h>

#ifdef OS_WIN
	#include <Windows.h>
	#include <direct.h>
	#include <process.h>
	#include <signal.h>
#else
	#include <dlfcn.h>
	#include <unistd.h>
	#include <sys/types.h>
#endif


using namespace Toast;
using namespace Toast::Bootstrap;
using namespace std;

static vector<Loader::ModuleAdapter *> __modules;
static Logger _log("Bootstrap-Loader");
static char _load_idx = 0x00;

string Loader::ModuleAdapter::get_name() {
    if (!name_cache.empty()) return name_cache;
	if (!priv.get_info_set()) return file;
    char name[48];
	memcpy(name, priv.get_module_name(), 48);
    name_cache = string(name);
    return name_cache;
}

void Loader::initialize() {
    Loader::search_modules();
    Loader::create_subprocesses();
}

void Loader::free() {
	for (std::vector< Loader::ModuleAdapter * >::iterator it = __modules.begin(); it != __modules.end(); ++it) {
		Loader::free_private_mempool(*it);
		delete (*it);
	}
}

void Loader::search_modules() {
    vector<string> module_files = Filesystem::ls_local(Filesystem::path("modules"));
    for (auto module : module_files) {
		if (ends_with(module, OS_LIB)) {
			Loader::ModuleAdapter *adapter = new Loader::ModuleAdapter();
			adapter->file = module;
			adapter->idx = (int)(_load_idx);
			Memory::shared()->set_module_activity_state(_load_idx, Memory::ModuleActState::DISCOVERED);
			_load_idx += 1;
			if (_load_idx == 128) {
				// Overflow
				_log.error("Wow! You've loaded more than 128 modules. Unfortunately, we don't support more than 128 modules due to technical limitations. File a bug report to the ToastC++ Repository on OpenRIO and we'll start working on support for more modules!");
				_log.error("Any further modules will be ignored.");
			} else {
				__modules.push_back(adapter);
			}
			_log.debug("Discovered Module File: " + module);
		} else if (ends_with(module, ".trx")) {
			// Toast Resource Extension (trx) file.
			Resources::link_trx_file(Filesystem::basename(module), module);
        } else {
            _log.warn("Module File " + module + " does not have a valid file extension! Are you sure this is the correct download for your platform? Valid File Extension: " + OS_LIB);
        }
    }
}

void Loader::create_subprocesses() {
    for (auto module : __modules) {
        Loader::create_module_process(module);
		Memory::shared()->set_module_activity_state(module->idx, Memory::ModuleActState::CRASHED);
    }
}

static void create_process(Loader::ModuleAdapter *adapter) {
    string mempool_handle = adapter->get_mempool_id();
    string file = adapter->file;
    
	adapter->priv.set_verification(Memory::Verification::VERIFY_AWAIT);
	memcpy(adapter->priv.get_module_filepath(), file.c_str(), file.length());

    _log.debug("Launching Process for Module: " + adapter->file);
    #ifdef OS_WIN
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        
        if (CreateProcess(NULL, 
            (char *)("toast_launcher __TOAST_MODULE_LD_FILE " + file + " " + mempool_handle + " " + to_string(adapter->idx)).c_str(), 
            NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
			DWORD exit_code;
			GetExitCodeProcess(pi.hProcess, &exit_code);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			bool sigint = false;
			if (exit_code == (128 + SIGINT)) {
				_log << "Module " + adapter->file + " requested Signal Interrupt (CTRL+C). Going for Shutdown.";
				raise(SIGINT);
				return;
			}
        }
    #else
        int result = system(("./toast_launcher __TOAST_MODULE_LD_FILE " + file + " " + mempool_handle + " " + to_string(adapter->idx)).c_str());
		int exit_code = WEXITSTATUS(result);
		if (exit_code == 128 + SIGINT) {
			_log << "Module " + adapter->file + " requested Signal Interrupt (CTRL+C). Going for Shutdown.";
			raise(SIGINT);
			return;
		}
	#endif
}

static void create_process_thread(Loader::ModuleAdapter *adapter) {
    _log.debug("Creating Private Mempool for " + adapter->file + " (id: " + adapter->get_mempool_id() + ")");
    create_private_mempool(adapter);
    do {
        adapter->clear();
        create_process(adapter);
        _log.warn("Module " + adapter->get_name() + " has exited or crashed! " + (adapter->priv.get_restart() ? "Restarting..." : "Not Restarting..."));
    } while (adapter->priv.get_restart());
}

void Loader::create_module_process(Loader::ModuleAdapter *adapter) {
    _log.debug("Creating Module Process Thread for " + adapter->file);
    thread t(create_process_thread, adapter);
    t.detach();
}

void Loader::create_private_mempool(Loader::ModuleAdapter *adapter) {
    adapter->private_mempool_handle = Internal::SHM::create_shm_file(adapter->get_mempool_id(), Memory::PrivatePool::SIZE);
    adapter->private_mempool = Internal::SHM::map_shm_file(adapter->private_mempool_handle, Memory::PrivatePool::SIZE);
	adapter->priv.map_to(adapter->private_mempool);
    adapter->priv.set_endian(Memory::get_endian_bit());
}

void Loader::free_private_mempool(Loader::ModuleAdapter *adapter) {
    Internal::SHM::unmap_shm_file(adapter->private_mempool, Memory::PrivatePool::SIZE);
    Internal::SHM::close_shm_file(adapter->get_mempool_id(), adapter->private_mempool_handle);
}