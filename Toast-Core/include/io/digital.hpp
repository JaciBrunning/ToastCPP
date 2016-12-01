#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"

#include <functional>

namespace IO {
	// This is internal, but we keep it here since both the Bootstrap and Toast-Core process use it.
	namespace DIO_IPC {
		const std::string INTERRUPT_ENABLE = "io.digital.interrupt.enable";
		const std::string INTERRUPT_TRIGGER = "io.digitial.interrupt.trigger";

		const std::string GLITCH_FILTER_ADD = "io.digital.glitch.add";
		const std::string GLITCH_FILTER_REMOVE = "io.digital.glitch.remove";

		const std::string DIGITAL_UPDATE = "io.digital.update";
		
		struct GlitchFilterMessage {
			int port;
			uint32_t period_low, period_high;
			bool period_fpga;
		};
	}

	class DIO {
	public:
		typedef Toast::Memory::Shared::IO::DIO_Mode Mode;
		struct InterruptData {
			uint8_t port;
			bool rising, falling;
		};
		enum class GlitchFilterMode {
			FPGA_CYCLES = 0,
			NANOSECONDS = 1
		};
		typedef std::function<void(InterruptData)> InterruptHandler;

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

		API void enable_interrupt();
		API void on_interrupt(InterruptHandler handler);
		API bool wait_for_interrupt();

		API void add_glitch_filter(uint64_t fpga_cycles_or_nanoseconds, GlitchFilterMode mode);
		API void remove_glitch_filter();

		API static void update_now(bool wait = true);
	private:
		int _port;
		Toast::Memory::Shared::IO::DIO *_mem;
	};
}