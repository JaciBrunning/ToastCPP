#include "toast/module/statics.hpp"
#include "toast/net/socket.hpp"
#include "toast/library.hpp"
#include "toast/module/memory.hpp"
#include "toast/environment.hpp"
#include "toast/internal/loader.hpp"
#include "toast/module.hpp"
#include "toast/logger.hpp"
#include "toast/state.hpp"
#include "toast/util.hpp"
#include "toast/memory.hpp"
#include "toast/crash.hpp"
#include "toast/ipc.hpp"

#include <iostream>
#include <string>

using namespace Toast;
using namespace std;

static Logger _logger("Module");

void toast_module_shutdown() {
    _logger << "Shutting Down Module...";
    Log::flush();
    Log::close();

	Toast::Memory::free_memory(false);
}

void init_toast_module(string module_name, string private_mempool_id, int module_idx) {
    long start_time = current_time_millis();
    if (Memory::Module::initialize(private_mempool_id, module_idx) != 0) return;
    
    Crash::on_shutdown(toast_module_shutdown);
    
    Net::Socket::socket_init();
    Log::initialize(module_name);
    _logger.set_name("Module-" + module_name);

	// We put this so early so IO interfaces like I2C don't run into initialization errors if
	// they are created as global members.
	_logger.debug("Init IPC...");
	IPC::start(module_idx);

    _logger.debug("Loading Module...");
    
    DYNAMIC dyn = Internal::Loader::load_dynamic_library(module_name);
    if (dyn == NULL) {
		//Memory::Module::load_error(PMP_LOAD_ERR);
		Memory::get_private()->set_verification(Memory::Verification::VERIFY_CORRUPT_LIBRARY);
        _logger.severe("Error! Dynamic Library could not be found or is corrupt.");
        _logger.severe("Dynamic Error: " + Internal::Loader::get_dynamic_error());
        return;
    }
    SYMBOL info_sym = Internal::Loader::get_symbol(dyn, "_get_module_information");
    if (info_sym == NULL) {
		Memory::get_private()->set_verification(Memory::Verification::VERIFY_INFO_METHOD_MISSING);
        _logger.severe("Error! Module Information could not be gathered. Check your _get_module_information method.");
        _logger.severe("Dynamic Error: " + Internal::Loader::get_dynamic_error());
        return;
    }
    ModuleInfo *info = reinterpret_cast<ModuleInfo *(*)()>(info_sym)();
    Memory::Module::bind_info(info);
    _logger.set_name("Module-" + string(info->name));
    _logger.debug("Module Info Bound");
    
    SYMBOL init_sym = Internal::Loader::get_symbol(dyn, "_allocate_module_instance");
    if (init_sym == NULL) {
		Memory::get_private()->set_verification(Memory::Verification::VERIFY_ALLOC_METHOD_MISSING);
        _logger.severe("Error! Module Instantiation for " + module_name + " could not be done. Check your _allocate_module_instance method and Module Constructor.");
        _logger.severe("Dynamic Error: " + Internal::Loader::get_dynamic_error());
        return;
    }

    Module *module = reinterpret_cast<Module *(*)()>(init_sym)();
    module->construct();
    
    Memory::Module::finalize_load();
	module->start();

    _logger << "Module Loaded: " + string(info->name) + " [" + to_string(Memory::shared()->get_bootstrap_pid()) + " -> " + to_string(get_pid()) + "]";
    
	long end_time = current_time_millis();
    _logger << "Total Module Startup Time: " + to_string(end_time - start_time) + "ms";
    // MAIN LOOP
    States::start_tracker();
    // SHUTDOWN 
    
    toast_module_shutdown();
}