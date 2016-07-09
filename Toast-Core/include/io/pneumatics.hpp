#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "io/common.hpp"

namespace IO {
	API int get_PCM_shm_id(int module_id);
	API char *get_PCM_block(int id);

	class PCM {
	public:
		API PCM(int module_id);
		API virtual ~PCM() = default;

		API bool operator==(PCM &p2) {
			return p2._mod_id == _mod_id;
		}

		API int get_shm_id();
		API int get_module_id();

		API bool get_solenoid(int solenoid);
		API bool solenoid_blacklisted(int solenoid);
		API void set_solenoid(int solenoid, bool value);

		API void start_compressor();
		API void stop_compressor();
		API bool compressor_enabled();

		API void set_closed_loop_control(bool state);
		API bool closed_loop_control();

		API float compressor_current();
		API bool pressure_switch_low();

		API void clear_sticky_faults();
		API void clear_solenoid_sticky_faults();

		API bool compressor_current_too_high_fault();
		API bool compressor_current_too_high_sticky_fault();
		API bool compressor_shorted_fault();
		API bool compressor_shorted_sticky_fault();
		API bool compressor_not_connected_fault();
		API bool compressor_not_connected_sticky_fault();
		API bool solenoid_voltage_fault();
		API bool solenoid_voltage_sticky_fault();
	private:
		int _shm_id;
		int _mod_id;
		char *_shm;
	};
}