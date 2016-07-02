#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"

#include <inttypes.h>

namespace Analog {
	API float get_sample_rate();
	
	namespace Out {
		API char *getBlockFor(int port);
		API void init(int port);
		API void set(int port, float volts);
	}

	namespace In {
		API char *getBlockFor(int port);
		API void init(int port);
		
		API int16_t get_value(int port);
		API int32_t get_avg_value(int port);
		
		API float get_voltage(int port);
		API float get_avg_voltage(int port);

		API void set_avg_bits(int port, uint32_t bits);
		API uint32_t get_avg_bits(int port);
		API void set_oversample_bits(int port, uint32_t bits);
		API uint32_t get_oversample_bits(int port);
		API uint32_t get_lsb_weight(int port);
		API int32_t get_offset();

		API bool is_accum(int port);
		API void init_accum(int port);
		API void set_accum_initial(int port, int64_t initial);
		API void reset_accume(int port);
		API void set_accum_center(int port, int32_t center);
		API void set_accum_deadband(int port, int32_t deadband);
		API int64_t get_accum_value(int port);
		API uint32_t get_accum_count(int port);
	}
}