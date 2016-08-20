#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"

#include "io/common.hpp"

namespace IO {
	class Joystick {
	public:
		API Joystick(int port);
		API virtual ~Joystick() = default;

		API bool operator==(Joystick &j2) const {
			return j2._port == _port;
		}

		API int get_port();

		// Todo common joystick bindings (e.g. getX, getY) here or in a child class?

		API int get_num_axis();
		API int get_num_button();
		API int get_num_pov();

		API bool get_raw_button(int id);
		API float get_raw_axis(int id);
		API int get_pov(int id);

		API void set_rumble_left(float magnitude);
		API float get_rumble_left();
		API void set_rumble_right(float magnitude);
		API float get_rumble_right();
		API void set_output(int id, bool value);
		API void set_outputs(uint32_t outputs);
		API bool get_output(int id);
		API uint32_t get_outputs();
	private:
		int _port;
		Toast::Memory::Shared::DS::Joystick *_mem;
	};

	namespace DS {
		typedef Toast::Memory::Shared::DS::Alliance Alliance;
		API bool is_ds_attached();
		API bool is_fms_attached();
		API bool is_new_control_data();
		API bool is_system_active();
		API float get_match_time();
		API Alliance get_alliance();
		API int get_alliance_station();
	}
}