#pragma once

#include "toast/library.hpp"

typedef struct {
    char name[32];
    bool is_simulation;
    bool is_wpilib;
} ProviderInfo;

CAPI ProviderInfo *provider_info();
CAPI void provider_init();

CAPI void set_base_callback(void (*callback_periodic)(bool,bool,bool,bool), void (*callback_transition)(bool,bool,bool,bool));