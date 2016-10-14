#pragma once

#include "toast/library.hpp"
#include "toast/config.hpp"
#include "toast/logger.hpp"
#include "toast/internal/loader.hpp"

CAPI void init_toast_bootstrap(int argc, char *argv[]);

namespace Toast {
	namespace Bootstrap {
		struct BootConfig : Config {
			CFG_FILE(BootConfig, "Toast-Bootstrap");

			struct Timings : SubConfig {
				struct States : SubConfig {
					CFG_PROPERTY(float, frequency, 50.0);
				};
				SUB_CONFIG(States, states);
			};
			SUB_CONFIG(Timings, timings);

			struct HTTP : SubConfig {
				CFG_PROPERTY(int, poll_frequency, 30);
				CFG_PROPERTY(int, port, 5801);
				CFG_PROPERTY(int, update_frequency, 30);
			};
			SUB_CONFIG(HTTP, http);
		};

		API BootConfig *get_config();
		API Logger *get_logger();
		API DYNAMIC *get_provider();
	}
}