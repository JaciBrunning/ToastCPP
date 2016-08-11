#pragma once

#include "toast/library.hpp"
#include "toast/config.hpp"
#include "toast/logger.hpp"

CAPI void init_toast_bootstrap(int argc, char *argv[]);

namespace Toast {
	namespace Bootstrap {
		API Config *get_config();
		API Logger *get_logger();
	}
}