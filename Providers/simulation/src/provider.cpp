#include "thp/provider.hpp"

#include "thp/sim/ds_comms.hpp"
#include "thp/sim/sim_memory.hpp"
#include "thp/sim/sim_provider.hpp"
#include "thp/sim/webui.hpp"

#include "toast/config.hpp"
#include "toast/util.hpp"

static ProviderInfo info = {
    "Simulation",
    true, false
};

static PeriodicStateCallback _state_callback_periodic;
static RawStateCallback _state_callback_transition;

static SimConfig _config;
static Toast::Logger _logger("THP - Simulation");

ProviderInfo *provider_info() {
    return &info;
}

SimConfig *get_simulation_config() {
    return &_config;
}

void provider_preinit() { }
void provider_predriver() { }
void provider_preload() { }
void provider_init() {
    _logger.info("Simulation Provider Loaded");
	_config.load();
    Sim::DriverStationComms::start();
	Sim::Web::init();

    Sim::Memory::init();
}

void provider_loop() {
    while (true) {
        Sim::DriverStationComms::periodic_update();
		thp_state_call_periodic();
        sleep_ms(1000 / _config.update_frequency);
    }
}

void provider_free() {
    Sim::DriverStationComms::stop();
}

void thp_state_set_callback(PeriodicStateCallback callback_periodic, RawStateCallback callback_transition) {
    _state_callback_periodic = callback_periodic;
    _state_callback_transition = callback_transition;
}

void thp_state_call_periodic() {
    _state_callback_periodic();
}

void thp_state_call_init(bool disabled, bool auton, bool teleop, bool test) {
    _state_callback_transition(disabled, auton, teleop, test);
}