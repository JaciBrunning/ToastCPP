#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "io/common.hpp"

namespace IO {
	API char *get_dio_block(int port);

	class DIO {
	public:
		enum Mode {
			OUTPUT = 0x01, INPUT = 0x02
		};

		API DIO(int port, Mode mode = Mode::INPUT);
		API virtual ~DIO() = default;

		API bool operator==(DIO &d2) {
			return d2._port == _port;
		}

		API int get_port();
		API Mode get_mode();
		API bool is_output() { return get_mode() == Mode::OUTPUT; }
		API bool is_input() { return get_mode() == Mode::INPUT; }

		API bool get();

		// Output only
		API void set(bool state);
		API void set_pulse(float length);
		API bool is_pulsing();
		API void set_pwm_rate(float rate);
		API void set_pwm_enable(float initial_duty_cycle);
		API void set_pwm_disable();
		API void set_pwm_duty_cycle(float duty_cycle);
	private:
		int _port;
		char *_shm;
	};
}