#pragma once

#include "toast/library.hpp"

typedef struct {
    char name[32];
    bool is_simulation;
    bool is_wpilib;
} ProviderInfo;

typedef void (*RawStateCallback)(bool, bool, bool, bool);

CAPI ProviderInfo *provider_info();

// Called before Toast Loading
CAPI void provider_preinit();

// Called after Toast loading
CAPI void provider_init();

// Called after provider_init() to start looping.
CAPI void provider_loop();

CAPI void provider_free();

CAPI void thp_state_set_callback(RawStateCallback callback_periodic, RawStateCallback callback_transition);
CAPI void thp_state_call_periodic(bool disabled, bool auton, bool teleop, bool test);
CAPI void thp_state_call_init(bool disabled, bool auton, bool teleop, bool test);