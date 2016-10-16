#include "thp/provider.hpp"
#include "thp/wpi/base_robot.hpp"
#include "thp/wpi/interfaces.hpp"
#include "thp/wpi/wpi_provider.hpp"

#include "WPILib.h"

#include "toast/logger.hpp"
#include <string>

using namespace std;
using namespace Toast;

static ProviderInfo info = {
    "WPILib (2016)",
    false, true
};

static PeriodicStateCallback _state_callback_periodic;
static RawStateCallback _state_callback_transition;

static Logger _provider_logger("THP - WPILib (2016)");
static THP_Base_Robot *robot;

ProviderInfo *provider_info() {
    return &info;
}

THP_Base_Robot *get_robot() {
	return robot;
}

void provider_preinit() { }
void provider_init() {
	long starttime = current_time_millis();
    if (!HAL_Initialize(0)) {
        _provider_logger.severe("HAL could not be initialized");
        throw "HAL could not be initialized";
    }
    HAL_Report(HALUsageReporting::kResourceType_Language,
              HALUsageReporting::kLanguage_CPlusPlus);

    robot = new THP_Base_Robot();
	long endtime = current_time_millis();
	_provider_logger << "WPILib Start Time: " + to_string(endtime - starttime) + "ms";
}

void provider_loop() {
	robot->StartCompetition();
}

void provider_free() {
    free(robot);
}

void thp_state_set_callback(PeriodicStateCallback callback_periodic, RawStateCallback callback_transition) {
	_state_callback_periodic = callback_periodic;
	_state_callback_transition = callback_transition;
}

void thp_state_call_periodic() {
	tick_interfaces();
    _state_callback_periodic();
}

void thp_state_call_init(bool disabled, bool auton, bool teleop, bool test) {
    _state_callback_transition(disabled, auton, teleop, test);
}