#include "toast/library.hpp"
#include "toast/module/memory.hpp"
#include "toast/environment.hpp"
#include "toast/internal/loader.hpp"
#include "toast/module.hpp"
#include "toast/logger.hpp"

#include <iostream>
#include <string>

using namespace Toast;
using namespace std;

static Logger _logger("Module");

CAPI void init_toast_module(string module_name, string private_mempool_id) {
    if (Memory::Module::initialize(private_mempool_id) != 0) return;
    Log::initialize(module_name);
    _logger.set_name("Module-" + module_name);
    _logger.debug("Loading Module...");
    
    DYNAMIC dyn = Internal::Loader::load_dynamic_library(module_name);
    if (dyn == NULL) {
        Memory::Module::load_error(PMP_LOAD_ERR);
        _logger.severe("Error! Dynamic Library could not be found or is corrupt.");
        _logger.severe("Dynamic Error: " + Internal::Loader::get_dynamic_error());
        return;
    }
    SYMBOL info_sym = Internal::Loader::get_symbol(dyn, "_get_module_information");
    if (info_sym == NULL) {
        Memory::Module::load_error(PMP_INFO_ERR);
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
        Memory::Module::load_error(PMP_INIT_ERR);
        _logger.severe("Error! Module Instantiation for " + module_name + " could not be done. Check your _allocate_module_instance method and Module Constructor.");
        _logger.severe("Dynamic Error: " + Internal::Loader::get_dynamic_error());
        return;
    }
    Module *module = reinterpret_cast<Module *(*)()>(init_sym)();
    module->construct();
    
    Memory::Module::finalize_load();
    _logger << "Module Loaded: " + string(info->name);
}