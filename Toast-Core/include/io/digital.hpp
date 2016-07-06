#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "io/common.hpp"

#include <inttypes.h>

enum DIO_Mode {
	dio_input = 0x01,
	dio_output = 0x02
};

namespace DIO {
	// Common
	API char *getBlockFor(int id);
	API int init(int port, int mode);
	API bool get(int port);

	// Digital Output Only
	API void set(int port, bool state);
	API void set_pulse(int port, float length);
	API bool is_pulsing(int port);
	API void set_pwm_rate(int port, float rate);
	API void set_pwm_enable(int port, float init_duty_cycle);
	API void set_pwm_disable(int port);
	API void set_pwm_duty_cycle(int port, float duty_cycle);
}