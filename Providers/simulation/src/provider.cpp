#include "thp/provider.hpp"

static ProviderInfo info = {
    "Simulation",
    true, false
};

static RawStateCallback _state_callback_periodic;
static RawStateCallback _state_callback_transition;

ProviderInfo *provider_info() {
    return &info;
}

void provider_preinit() { }
void provider_init() { }
void provider_free() { }

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