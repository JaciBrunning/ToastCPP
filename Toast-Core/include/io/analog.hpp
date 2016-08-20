#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "io/common.hpp"

#include <inttypes.h>

namespace IO {
	API float get_analog_sample_rate();

	class AnalogOutput {
	public:
		API AnalogOutput(int port);
		API virtual ~AnalogOutput() = default;

		API int get_port() const;

		API bool operator==(AnalogOutput &a2) const {
			return a2._port == _port;
		}

		API void set(float volts);
		API float get();
	private:
		int _port;
		Toast::Memory::Shared::IO::AnalogOut *_mem;
	};

	class AnalogInput {
	public:
		API AnalogInput(int port);
		API virtual ~AnalogInput() = default;

		API bool operator==(AnalogInput &a2) const {
			return a2._port == _port;
		}

		API int get_port();

		API int16_t get_value();
		API int32_t get_average_value();

		API float get();
		API float get_average();

		API void set_average_bits(uint32_t bits);
		API uint32_t get_average_bits();
		API void set_oversample_bits(uint32_t bits);
		API uint32_t get_oversample_bits();
		API uint32_t get_lsb_weight();
		API int32_t get_offset();

		API bool is_accumulator();
		API void init_accumulator();
		API void set_accumulator_initial(int64_t initial);
		API void reset_accumulator();
		API void set_accumulator_center(int32_t center);
		API void set_accumulator_deadband(int32_t deadband);
		API int64_t get_accumulator_value();
		API uint32_t get_accumulator_count();
	private:
		int _port;
		Toast::Memory::Shared::IO::AnalogIn *_mem;
	};
}