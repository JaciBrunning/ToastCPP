#pragma once

#include "toast/library.hpp"
#include <string>

CAPI void toast_module_shutdown();
CAPI void init_toast_module(std::string module_name, std::string private_mempool_id);