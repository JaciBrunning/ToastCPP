#pragma once

#include "toast/library.hpp"
#include "toast/provider.hpp"

// Called before Toast Loading
CAPI void provider_preinit();

CAPI void provider_predriver();
CAPI void provider_preload();

// Called after Toast loading
CAPI void provider_init();

// Called after provider_init() to start looping.
CAPI void provider_loop();

CAPI void provider_free();

CAPI void thp_state_set_callback(PeriodicStateCallback callback_periodic, RawStateCallback callback_transition);
CAPI void thp_state_call_periodic();
CAPI void thp_state_call_init(bool disabled, bool auton, bool teleop, bool test);