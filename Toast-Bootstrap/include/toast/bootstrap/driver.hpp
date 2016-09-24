#pragma once

#include "toast/internal/loader.hpp"
#include "toast/environment.hpp"
#include <vector>
#include <string>
#include <thread>
#include <stdlib.h>

namespace Toast {
    namespace Bootstrap {
        namespace Driver {
			typedef struct {
				std::string file;
				DYNAMIC lib;
			} BootstrapDriver;

			void initialize();
			void free();

			void search_drivers();
			void preinit_drivers();
			void init_drivers();
        }
    }
}