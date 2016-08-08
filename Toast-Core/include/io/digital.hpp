#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"

namespace IO {
	class DIO {
	public:
		typedef Toast::Memory::Shared::IO::DIO_Mode Mode;

		API DIO(int port, Mode mode = Mode::INPUT);
		API virtual ~DIO() = default;

		API bool operator==(DIO &d2) const {
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
		Toast::Memory::Shared::IO::DIO *_mem;
	};
}