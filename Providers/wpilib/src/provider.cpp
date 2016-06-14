#include "thp/provider.hpp"

using namespace std;

static ProviderInfo info = {
    "WPILib (2016)",
    false, true
};

static void (*_state_callback_periodic)(bool,bool,bool,bool);
static void (*_state_callback_transition)(bool,bool,bool,bool);

static Logger _provider_logger("THP - WPILib (2016)");
static THP_Base_Robot *robot;

ProviderInfo *provider_info() {
    return &info;
}

void provider_preinit() { }
void provider_init() {
    if (!HALInitialize()) {
        _provider_logger.fatal("HAL could not be initialized");
        throw new std::exception("HAL could not be initialized");
    }
    HALReport(HALUsageReporting::kResourceType_Language,
              HALUsageReporting::kLanguage_CPlusPlus);

    robot = new THP_Base_Robot();
    RobotBase::robotSetup(robot);
}

void provider_free() {
    free(robot);
}

void thp_state_set_callback(void (*callback_periodic)(bool,bool,bool,bool), void (*callback_transition)(bool,bool,bool,bool)) {
    _state_callback_periodic = callback_periodic;
    _state_callback_transition = callback_transition;
}

void thp_state_call_periodic(bool disabled, bool auton, bool teleop, bool test) {
    _state_callback_periodic(disabled, auton, teleop, test);
}

void thp_state_call_init(bool disabled, bool auton, bool teleop, bool test) {
    _state_callback_transition(disabled, auton, teleop, test);
}