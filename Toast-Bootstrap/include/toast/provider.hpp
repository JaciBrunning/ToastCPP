#pragma once

#pragma once

#include "toast/library.hpp"

typedef struct {
	char name[32];
	bool is_simulation;
	bool is_wpilib;
} ProviderInfo;

typedef void(*RawStateCallback)(bool, bool, bool, bool);

CAPI ProviderInfo *provider_info();