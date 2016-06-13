#include "provider/provider.hpp"

using namespace std;

static ProviderInfo info = {
    "WPILib (2016)",
    false, true
};

static void (*_state_callback_periodic)(bool,bool,bool,bool);
static void (*_state_callback_transition)(bool,bool,bool,bool);

ProviderInfo *provider_info() {
    return &info;
}

void provider_preinit() { }
void provider_init() { }

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