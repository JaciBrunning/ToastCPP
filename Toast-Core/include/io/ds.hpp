#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"

#define MAP_AXIS(name, axis_id) API inline float name() { return get_raw_axis(axis_id); }
#define MAP_BUTT(name, butt_id) API inline bool name() { return get_raw_button(butt_id); }
#define MAP_BUTTS(button_count, off) API inline bool button(int id) { if (id < button_count) return get_raw_button(id - off); return false; }

namespace IO {
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

	// Please note everything in Toast is indexed from 0, unlike WPILib where Joysticks
	// and button IDs are indexed from 1.
	class Joystick {
	public:
		API Joystick(int port);
		API virtual ~Joystick() = default;

		API bool operator==(Joystick &j2) const {
			return j2._port == _port;
		}

		API int get_port();

		API int get_num_axis();
		API int get_num_button();
		API int get_num_pov();

		API bool get_raw_button(int id);
		API float get_raw_axis(int id);
		API int get_pov(int id = 0);

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

	// Common Joystick Classes
	namespace Mapping {
		class XboxController : public Joystick {
		public:
			API XboxController(int port) : Joystick(port) { }

			MAP_AXIS(left_trigger, 2);
			MAP_AXIS(right_trigger, 3);
			MAP_AXIS(left_x, 0);
			MAP_AXIS(left_y, 1);
			MAP_AXIS(right_x, 4);
			MAP_AXIS(right_y, 5);
			MAP_BUTT(a, 0);
			MAP_BUTT(b, 1);
			MAP_BUTT(x, 2);
			MAP_BUTT(y, 3);
			MAP_BUTT(left_bumper, 4);
			MAP_BUTT(right_bumper, 5);
			MAP_BUTT(select, 6);
			MAP_BUTT(start, 7);
			MAP_BUTT(left_stick, 8);
			MAP_BUTT(right_stick, 9);
		};

		class LogitechExtreme3D : public Joystick {
		public:
			API LogitechExtreme3D(int port) : Joystick(port) { }

			MAP_AXIS(x, 0);
			MAP_AXIS(y, 1);
			MAP_AXIS(twist, 2);
			MAP_AXIS(slider, 3);

			MAP_BUTT(trigger, 0);
			MAP_BUTT(bumper, 1);
			MAP_BUTTS(12, 1);
		};
	}
}

#undef MAP_AXIS
#undef MAP_BUTT