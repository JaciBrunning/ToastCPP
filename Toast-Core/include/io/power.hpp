#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "io/common.hpp"

#include <inttypes.h>

namespace Power {
	API char *getPowerBlock();

	namespace PDP {
		API void set_can_id(uint8_t id);
		API double get_voltage();
		API double get_temperature();
		API double get_current(uint8_t channel);
		API double get_total_current();
		API double get_total_power();
		API double get_total_energy();
	}

	namespace Controller {
		API double get_input_voltage();
		API double get_input_current();

		API double get_voltage_3V3();
		API double get_current_3V3();
		API bool get_enabled_3V3();
		API int get_fault_count_3V3();

		API double get_voltage_5V();
		API double get_current_5V();
		API bool get_enabled_5V();
		API int get_fault_count_5V();

		API double get_voltage_6V();
		API double get_current_6V();
		API bool get_enabled_6V();
		API int get_fault_count_6V();
	}
}