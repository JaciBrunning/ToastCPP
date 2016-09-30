#pragma once

#include "fwi.hpp"
namespace Toast {
	namespace Memory {
		namespace Shared {
			namespace IO {
				enum class MotorInterface {
					PWM = 1,
					CAN = 2
				};
				enum class MotorType {
					Talon = 0,
					TalonSRX = 1,
					CANTalon = 2,
					Victor = 0,
					VictorSP = 1,
					SD540 = 0,
					Spark = 0
				};
				struct Motor;
				struct MotorSRXExtended;
				struct Servo;
				struct ServoStatic;
				struct PWM;
			}
		}
	}
}

namespace Toast {
	namespace Memory {
		namespace Shared {
			namespace IO {
				struct Motor : public FWI::Block {
					static const int SIZE = 8;

					FWIFUNC bool get_init();
					FWIFUNC void set_init(bool value);

					FWIFUNC bool get_bootstrap();
					FWIFUNC void set_bootstrap(bool value);

					FWIFUNC uint8_t get_port();
					FWIFUNC void set_port(uint8_t value);

					FWIFUNC MotorType get_type();
					FWIFUNC void set_type(MotorType value);

					FWIFUNC MotorInterface get_interface();
					FWIFUNC void set_interface(MotorInterface value);

					FWIFUNC float get_value();
					FWIFUNC void set_value(float value);


				}; // struct: Motor
				struct MotorSRXExtended : public FWI::Block {
					static const int SIZE = 101;

					FWIFUNC bool get_lim_fwd_closed();
					FWIFUNC void set_lim_fwd_closed(bool value);

					FWIFUNC bool get_lim_fwd_ok();
					FWIFUNC void set_lim_fwd_ok(bool value);

					FWIFUNC bool get_lim_rvs_closed();
					FWIFUNC void set_lim_rvs_closed(bool value);

					FWIFUNC bool get_lim_rvs_ok();
					FWIFUNC void set_lim_rvs_ok(bool value);

					FWIFUNC float get_p();
					FWIFUNC void set_p(float value);

					FWIFUNC float get_i();
					FWIFUNC void set_i(float value);

					FWIFUNC float get_d();
					FWIFUNC void set_d(float value);

					FWIFUNC float get_f();
					FWIFUNC void set_f(float value);

					FWIFUNC bool get_pid_pending();
					FWIFUNC void set_pid_pending(bool value);

					FWIFUNC float get_bus_voltage();
					FWIFUNC void set_bus_voltage(float value);

					FWIFUNC float get_output_voltage();
					FWIFUNC void set_output_voltage(float value);

					FWIFUNC float get_output_current();
					FWIFUNC void set_output_current(float value);

					FWIFUNC float get_temperature();
					FWIFUNC void set_temperature(float value);

					FWIFUNC float get_position();
					FWIFUNC void set_position(float value);

					FWIFUNC float get_speed();
					FWIFUNC void set_speed(float value);

					FWIFUNC int32_t get_enc_position();
					FWIFUNC void set_enc_position(int32_t value);

					FWIFUNC int32_t get_enc_velocity();
					FWIFUNC void set_enc_velocity(int32_t value);

					FWIFUNC int32_t get_analog_in();
					FWIFUNC void set_analog_in(int32_t value);

					FWIFUNC int32_t get_analog_in_raw();
					FWIFUNC void set_analog_in_raw(int32_t value);

					FWIFUNC int32_t get_analog_in_vel();
					FWIFUNC void set_analog_in_vel(int32_t value);

					FWIFUNC int32_t get_quad_a();
					FWIFUNC void set_quad_a(int32_t value);

					FWIFUNC int32_t get_quad_b();
					FWIFUNC void set_quad_b(int32_t value);

					FWIFUNC int32_t get_quad_idx();
					FWIFUNC void set_quad_idx(int32_t value);

					FWIFUNC int32_t get_quad_idx_rises();
					FWIFUNC void set_quad_idx_rises(int32_t value);

					FWIFUNC int32_t get_pulse_width_position();
					FWIFUNC void set_pulse_width_position(int32_t value);

					FWIFUNC int32_t get_pulse_width_vel();
					FWIFUNC void set_pulse_width_vel(int32_t value);

					FWIFUNC int32_t get_pulse_width_rtf();
					FWIFUNC void set_pulse_width_rtf(int32_t value);

					FWIFUNC int32_t get_pulse_width_rtr();
					FWIFUNC void set_pulse_width_rtr(int32_t value);

					FWIFUNC bool get_quad_idx_rises_pending();
					FWIFUNC void set_quad_idx_rises_pending(bool value);

					FWIFUNC bool get_pulse_width_position_pending();
					FWIFUNC void set_pulse_width_position_pending(bool value);

					FWIFUNC uint16_t get_faults();
					FWIFUNC void set_faults(uint16_t value);

					FWIFUNC uint16_t get_faults_sticky();
					FWIFUNC void set_faults_sticky(uint16_t value);

					FWIFUNC bool get_clear_sticky_pending();
					FWIFUNC void set_clear_sticky_pending(bool value);

					FWIFUNC uint32_t get_firmware_version();
					FWIFUNC void set_firmware_version(uint32_t value);


				}; // struct: MotorSRXExtended
				struct Servo : public FWI::Block {
					static const int SIZE = 5;

					FWIFUNC bool get_init();
					FWIFUNC void set_init(bool value);

					FWIFUNC bool get_bootstrap();
					FWIFUNC void set_bootstrap(bool value);

					FWIFUNC bool get_offline();
					FWIFUNC void set_offline(bool value);

					FWIFUNC bool get_offline_pending();
					FWIFUNC void set_offline_pending(bool value);

					FWIFUNC bool get_update_pending();
					FWIFUNC void set_update_pending(bool value);

					FWIFUNC float get_value();
					FWIFUNC void set_value(float value);


				}; // struct: Servo
				struct ServoStatic : public FWI::Block {
					static const int SIZE = 12;

					FWIFUNC float get_max_angle();
					FWIFUNC void set_max_angle(float value);

					FWIFUNC float get_min_angle();
					FWIFUNC void set_min_angle(float value);

					FWIFUNC float get_angle_range();
					FWIFUNC void set_angle_range(float value);


				}; // struct: ServoStatic
				struct PWM : public FWI::Block {
					static const int SIZE = 33;

					FWIFUNC bool get_init();
					FWIFUNC void set_init(bool value);

					FWIFUNC bool get_bootstrap();
					FWIFUNC void set_bootstrap(bool value);

					FWIFUNC uint8_t get_period_multiplier();
					FWIFUNC void set_period_multiplier(uint8_t value);

					FWIFUNC bool get_period_mult_pending();
					FWIFUNC void set_period_mult_pending(bool value);

					FWIFUNC bool get_raw_pending();
					FWIFUNC void set_raw_pending(bool value);

					FWIFUNC bool get_position_pending();
					FWIFUNC void set_position_pending(bool value);

					FWIFUNC bool get_speed_pending();
					FWIFUNC void set_speed_pending(bool value);

					FWIFUNC bool get_bounds_pending();
					FWIFUNC void set_bounds_pending(bool value);

					FWIFUNC bool get_zero_latch();
					FWIFUNC void set_zero_latch(bool value);

					FWIFUNC bool get_deadband_elimination();
					FWIFUNC void set_deadband_elimination(bool value);

					FWIFUNC bool get_deadband_elimination_pending();
					FWIFUNC void set_deadband_elimination_pending(bool value);

					FWIFUNC uint16_t get_raw();
					FWIFUNC void set_raw(uint16_t value);

					FWIFUNC float get_position();
					FWIFUNC void set_position(float value);

					FWIFUNC float get_speed();
					FWIFUNC void set_speed(float value);

					FWIFUNC float get_bounds_max();
					FWIFUNC void set_bounds_max(float value);

					FWIFUNC float get_bounds_min();
					FWIFUNC void set_bounds_min(float value);

					FWIFUNC float get_bounds_deadband_max();
					FWIFUNC void set_bounds_deadband_max(float value);

					FWIFUNC float get_bounds_deadband_min();
					FWIFUNC void set_bounds_deadband_min(float value);

					FWIFUNC float get_bounds_center();
					FWIFUNC void set_bounds_center(float value);

					FWIFUNC bool get_bounds_int32();
					FWIFUNC void set_bounds_int32(bool value);

					FWIFUNC bool get_if();
					FWIFUNC void set_if(bool value);

					FWIFUNC bool get_casting();
					FWIFUNC void set_casting(bool value);


				}; // struct: PWM
			} // namespace: IO
		} // namespace: Shared
	} // namespace: Memory
} // namespace: Toast
