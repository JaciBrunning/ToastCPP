#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "toast/environment.hpp"
#include "toast/internal/loader.hpp"
#include "toast/module.hpp"
#include "toast/logger.hpp"

#include <iostream>
#include <string>

using namespace Toast;
using namespace std;

CAPI void init_toast_module(string module_name, string private_mempool_id) {
    if (Memory::Internal::initialize(private_mempool_id) != 0) return;
    Log::initialize(module_name);
    
    DYNAMIC dyn = Internal::Loader::load_dynamic_library(module_name);
    if (dyn == NULL) {
        Memory::Internal::load_error(PMP_LOAD_ERR);
        return;
    }
    SYMBOL info_sym = Internal::Loader::get_symbol(dyn, "_get_module_information");
    if (info_sym == NULL) {
        Memory::Internal::load_error(PMP_INFO_ERR);
        return;
    }
    ModuleInfo *info = reinterpret_cast<ModuleInfo *(*)()>(info_sym)();
    Memory::Internal::bind_info(info);
    
    SYMBOL init_sym = Internal::Loader::get_symbol(dyn, "_allocate_module_instance");
    if (init_sym == NULL) {
        Memory::Internal::load_error(PMP_INIT_ERR);
        return;
    }
    Module *module = reinterpret_cast<Module *(*)()>(init_sym)();
    module->construct();
    
    Memory::Internal::finalize_load();
}