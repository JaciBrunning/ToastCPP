#include "thp/provider.hpp"
#include "thp/wpi/base_robot.hpp"

#include "WPILib.h"

#include "toast/logger.hpp"
#include <string>

using namespace std;
using namespace Toast;

static ProviderInfo info = {
    "WPILib (2016)",
    false, true
};

static RawStateCallback _state_callback_periodic;
static RawStateCallback _state_callback_transition;

static Logger _provider_logger("THP - WPILib (2016)");
static THP_Base_Robot *robot;

ProviderInfo *provider_info() {
    return &info;
}

void provider_preinit() { }
void provider_init() {
    if (!HALInitialize()) {
        _provider_logger.severe("HAL could not be initialized");
        throw "HAL could not be initialized";
    }
    HALReport(HALUsageReporting::kResourceType_Language,
              HALUsageReporting::kLanguage_CPlusPlus);

    robot = new THP_Base_Robot();
}

void provider_loop() {
    RobotBase::robotSetup(robot);
}

void provider_free() {
    free(robot);
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