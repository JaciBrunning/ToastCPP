#pragma once

#include "toast/config.hpp"

struct SimConfig : Toast::Config {
	CFG_FILE(SimConfig, "Toast-Simulation");

	struct MDNS : Toast::SubConfig {
		CFG_PROPERTY(std::string, service_name, "roborio-9999-frc");
		CFG_PROPERTY(std::string, host_name, "roborio-9999-frc");
		CFG_PROPERTY(std::string, ip_address, "auto");
		CFG_PROPERTY(std::string, broadcast_ip, "224.0.0.251");
	};
	SUB_CONFIG(MDNS, mdns);

	struct HTTP : Toast::SubConfig {
		CFG_PROPERTY(int, joystick_update_frequency, 30);
	};
	SUB_CONFIG(HTTP, http);

	CFG_PROPERTY(int, update_frequency, 50);
};

SimConfig *get_simulation_config();