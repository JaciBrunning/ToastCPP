#include "thp/provider.hpp"

#include "thp/sim/ds_comms.hpp"
#include "thp/sim/sim_memory.hpp"
#include "thp/sim/sim_provider.hpp"

#include "toast/config.hpp"
#include "toast/util.hpp"

static ProviderInfo info = {
    "Simulation",
    true, false
};

static RawStateCallback _state_callback_periodic;
static RawStateCallback _state_callback_transition;

static Toast::Config _config("Toast-Simulation");
static Toast::Logger _logger("THP - Simulation");

ProviderInfo *provider_info() {
    return &info;
}

Toast::Config *get_simulation_config() {
	return &_config;
}

void provider_preinit() { }
void provider_init() {
	_logger.info("Simulation Provider Loaded");
	_config.load();
	Sim::DriverStationComms::start();

	Sim::Memory::init();
}

void provider_loop() {
    int update_freq = _config.get_int("update_frequency", 50);
	while (true) {
		Sim::DriverStationComms::periodic_update();
		sleep_ms(1000 / update_freq);
	}
}

void provider_free() {
	Sim::DriverStationComms::stop();
}

void thp_state_set_callback(RawStateCallback callback_periodic, RawStateCallback callback_transition) {
    _state_callback_periodic = callback_periodic;
    _state_callback_transition = callback_transition;
}

void thp_state_call_periodic(bool disabled, bool auton, bool teleop, bool test) {
    _state_callback_periodic(disabled, auton, teleop, test);
}

void thp_state_call_init(bool disabled, bool auton, bool teleop, bool test) {
    _state_callback_transition(disabled, auton, teleop, test);
}