#include "shared/motor.fwi.hpp"

FWIFUNC bool Toast::Memory::Shared::IO::Motor::get_init() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::Motor::set_init(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Motor::get_bootstrap() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Motor::set_bootstrap(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC uint8_t Toast::Memory::Shared::IO::Motor::get_port() {
	return FWI_MEM_VAL(uint8_t, _store, 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Motor::set_port(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 1) = value;
}

FWIFUNC Toast::Memory::Shared::IO::MotorType Toast::Memory::Shared::IO::Motor::get_type() {
	return (Toast::Memory::Shared::IO::MotorType)(_store[2]);
}
FWIFUNC void Toast::Memory::Shared::IO::Motor::set_type(Toast::Memory::Shared::IO::MotorType value) {
	_store[2] = (char)value;
}

FWIFUNC Toast::Memory::Shared::IO::MotorInterface Toast::Memory::Shared::IO::Motor::get_interface() {
	return (Toast::Memory::Shared::IO::MotorInterface)(_store[3]);
}
FWIFUNC void Toast::Memory::Shared::IO::Motor::set_interface(Toast::Memory::Shared::IO::MotorInterface value) {
	_store[3] = (char)value;
}

FWIFUNC float Toast::Memory::Shared::IO::Motor::get_value() {
	return FWI_MEM_VAL(float, _store, 4);
}
FWIFUNC void Toast::Memory::Shared::IO::Motor::set_value(float value) {
	FWI_MEM_VAL(float, _store, 4) = value;
}


FWIFUNC bool Toast::Memory::Shared::IO::MotorSRXExtended::get_lim_fwd_closed() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_lim_fwd_closed(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::MotorSRXExtended::get_lim_fwd_ok() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_lim_fwd_ok(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::MotorSRXExtended::get_lim_rvs_closed() {
	return FWI_IS_BIT_SET(_store[0], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_lim_rvs_closed(bool value) {
	FWI_SET_BIT_TO(_store[0], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::MotorSRXExtended::get_lim_rvs_ok() {
	return FWI_IS_BIT_SET(_store[0], 3);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_lim_rvs_ok(bool value) {
	FWI_SET_BIT_TO(_store[0], 3, value ? 1 : 0);
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_p() {
	return FWI_MEM_VAL(float, _store, 1);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_p(float value) {
	FWI_MEM_VAL(float, _store, 1) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_i() {
	return FWI_MEM_VAL(float, _store, 5);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_i(float value) {
	FWI_MEM_VAL(float, _store, 5) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_d() {
	return FWI_MEM_VAL(float, _store, 9);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_d(float value) {
	FWI_MEM_VAL(float, _store, 9) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_f() {
	return FWI_MEM_VAL(float, _store, 13);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_f(float value) {
	FWI_MEM_VAL(float, _store, 13) = value;
}

FWIFUNC bool Toast::Memory::Shared::IO::MotorSRXExtended::get_pid_pending() {
	return FWI_IS_BIT_SET(_store[0], 4);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_pid_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 4, value ? 1 : 0);
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_bus_voltage() {
	return FWI_MEM_VAL(float, _store, 17);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_bus_voltage(float value) {
	FWI_MEM_VAL(float, _store, 17) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_output_voltage() {
	return FWI_MEM_VAL(float, _store, 21);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_output_voltage(float value) {
	FWI_MEM_VAL(float, _store, 21) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_output_current() {
	return FWI_MEM_VAL(float, _store, 25);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_output_current(float value) {
	FWI_MEM_VAL(float, _store, 25) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_temperature() {
	return FWI_MEM_VAL(float, _store, 29);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_temperature(float value) {
	FWI_MEM_VAL(float, _store, 29) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_position() {
	return FWI_MEM_VAL(float, _store, 33);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_position(float value) {
	FWI_MEM_VAL(float, _store, 33) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::MotorSRXExtended::get_speed() {
	return FWI_MEM_VAL(float, _store, 37);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_speed(float value) {
	FWI_MEM_VAL(float, _store, 37) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_enc_position() {
	return FWI_MEM_VAL(int32_t, _store, 41);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_enc_position(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 41) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_enc_velocity() {
	return FWI_MEM_VAL(int32_t, _store, 45);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_enc_velocity(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 45) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_analog_in() {
	return FWI_MEM_VAL(int32_t, _store, 49);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_analog_in(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 49) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_analog_in_raw() {
	return FWI_MEM_VAL(int32_t, _store, 53);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_analog_in_raw(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 53) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_analog_in_vel() {
	return FWI_MEM_VAL(int32_t, _store, 57);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_analog_in_vel(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 57) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_quad_a() {
	return FWI_MEM_VAL(int32_t, _store, 61);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_quad_a(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 61) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_quad_b() {
	return FWI_MEM_VAL(int32_t, _store, 65);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_quad_b(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 65) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_quad_idx() {
	return FWI_MEM_VAL(int32_t, _store, 69);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_quad_idx(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 69) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_quad_idx_rises() {
	return FWI_MEM_VAL(int32_t, _store, 73);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_quad_idx_rises(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 73) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_pulse_width_position() {
	return FWI_MEM_VAL(int32_t, _store, 77);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_pulse_width_position(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 77) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_pulse_width_vel() {
	return FWI_MEM_VAL(int32_t, _store, 81);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_pulse_width_vel(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 81) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_pulse_width_rtf() {
	return FWI_MEM_VAL(int32_t, _store, 85);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_pulse_width_rtf(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 85) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_pulse_width_rtr() {
	return FWI_MEM_VAL(int32_t, _store, 89);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_pulse_width_rtr(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 89) = value;
}

FWIFUNC bool Toast::Memory::Shared::IO::MotorSRXExtended::get_quad_idx_rises_pending() {
	return FWI_IS_BIT_SET(_store[0], 5);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_quad_idx_rises_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 5, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::MotorSRXExtended::get_pulse_width_position_pending() {
	return FWI_IS_BIT_SET(_store[0], 6);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_pulse_width_position_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 6, value ? 1 : 0);
}

FWIFUNC uint16_t Toast::Memory::Shared::IO::MotorSRXExtended::get_faults() {
	return FWI_MEM_VAL(uint16_t, _store, 93);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_faults(uint16_t value) {
	FWI_MEM_VAL(uint16_t, _store, 93) = value;
}

FWIFUNC uint16_t Toast::Memory::Shared::IO::MotorSRXExtended::get_faults_sticky() {
	return FWI_MEM_VAL(uint16_t, _store, 95);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_faults_sticky(uint16_t value) {
	FWI_MEM_VAL(uint16_t, _store, 95) = value;
}

FWIFUNC bool Toast::Memory::Shared::IO::MotorSRXExtended::get_clear_sticky_pending() {
	return FWI_IS_BIT_SET(_store[0], 7);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_clear_sticky_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 7, value ? 1 : 0);
}

FWIFUNC uint32_t Toast::Memory::Shared::IO::MotorSRXExtended::get_firmware_version() {
	return FWI_MEM_VAL(uint32_t, _store, 97);
}
FWIFUNC void Toast::Memory::Shared::IO::MotorSRXExtended::set_firmware_version(uint32_t value) {
	FWI_MEM_VAL(uint32_t, _store, 97) = value;
}


FWIFUNC bool Toast::Memory::Shared::IO::Servo::get_init() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::Servo::set_init(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Servo::get_bootstrap() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Servo::set_bootstrap(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Servo::get_offline() {
	return FWI_IS_BIT_SET(_store[0], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::Servo::set_offline(bool value) {
	FWI_SET_BIT_TO(_store[0], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Servo::get_offline_pending() {
	return FWI_IS_BIT_SET(_store[0], 3);
}
FWIFUNC void Toast::Memory::Shared::IO::Servo::set_offline_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 3, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Servo::get_update_pending() {
	return FWI_IS_BIT_SET(_store[0], 4);
}
FWIFUNC void Toast::Memory::Shared::IO::Servo::set_update_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 4, value ? 1 : 0);
}

FWIFUNC float Toast::Memory::Shared::IO::Servo::get_value() {
	return FWI_MEM_VAL(float, _store, 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Servo::set_value(float value) {
	FWI_MEM_VAL(float, _store, 1) = value;
}


FWIFUNC float Toast::Memory::Shared::IO::ServoStatic::get_max_angle() {
	return FWI_MEM_VAL(float, _store, 0);
}
FWIFUNC void Toast::Memory::Shared::IO::ServoStatic::set_max_angle(float value) {
	FWI_MEM_VAL(float, _store, 0) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::ServoStatic::get_min_angle() {
	return FWI_MEM_VAL(float, _store, 4);
}
FWIFUNC void Toast::Memory::Shared::IO::ServoStatic::set_min_angle(float value) {
	FWI_MEM_VAL(float, _store, 4) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::ServoStatic::get_angle_range() {
	return FWI_MEM_VAL(float, _store, 8);
}
FWIFUNC void Toast::Memory::Shared::IO::ServoStatic::set_angle_range(float value) {
	FWI_MEM_VAL(float, _store, 8) = value;
}


FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_init() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_init(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_bootstrap() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_bootstrap(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC uint8_t Toast::Memory::Shared::IO::PWM::get_period_multiplier() {
	return FWI_MEM_VAL(uint8_t, _store, 2);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_period_multiplier(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 2) = value;
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_period_mult_pending() {
	return FWI_IS_BIT_SET(_store[0], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_period_mult_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_raw_pending() {
	return FWI_IS_BIT_SET(_store[0], 3);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_raw_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 3, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_position_pending() {
	return FWI_IS_BIT_SET(_store[0], 4);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_position_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 4, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_speed_pending() {
	return FWI_IS_BIT_SET(_store[0], 5);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_speed_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 5, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_bounds_pending() {
	return FWI_IS_BIT_SET(_store[0], 6);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_bounds_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 6, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_zero_latch() {
	return FWI_IS_BIT_SET(_store[0], 7);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_zero_latch(bool value) {
	FWI_SET_BIT_TO(_store[0], 7, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_deadband_elimination() {
	return FWI_IS_BIT_SET(_store[1], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_deadband_elimination(bool value) {
	FWI_SET_BIT_TO(_store[1], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_deadband_elimination_pending() {
	return FWI_IS_BIT_SET(_store[1], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_deadband_elimination_pending(bool value) {
	FWI_SET_BIT_TO(_store[1], 1, value ? 1 : 0);
}

FWIFUNC uint16_t Toast::Memory::Shared::IO::PWM::get_raw() {
	return FWI_MEM_VAL(uint16_t, _store, 3);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_raw(uint16_t value) {
	FWI_MEM_VAL(uint16_t, _store, 3) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::PWM::get_position() {
	return FWI_MEM_VAL(float, _store, 5);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_position(float value) {
	FWI_MEM_VAL(float, _store, 5) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::PWM::get_speed() {
	return FWI_MEM_VAL(float, _store, 9);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_speed(float value) {
	FWI_MEM_VAL(float, _store, 9) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::PWM::get_bounds_max() {
	return FWI_MEM_VAL(float, _store, 13);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_bounds_max(float value) {
	FWI_MEM_VAL(float, _store, 13) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::PWM::get_bounds_min() {
	return FWI_MEM_VAL(float, _store, 17);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_bounds_min(float value) {
	FWI_MEM_VAL(float, _store, 17) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::PWM::get_bounds_deadband_max() {
	return FWI_MEM_VAL(float, _store, 21);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_bounds_deadband_max(float value) {
	FWI_MEM_VAL(float, _store, 21) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::PWM::get_bounds_deadband_min() {
	return FWI_MEM_VAL(float, _store, 25);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_bounds_deadband_min(float value) {
	FWI_MEM_VAL(float, _store, 25) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::PWM::get_bounds_center() {
	return FWI_MEM_VAL(float, _store, 29);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_bounds_center(float value) {
	FWI_MEM_VAL(float, _store, 29) = value;
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_bounds_int32() {
	return FWI_IS_BIT_SET(_store[1], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_bounds_int32(bool value) {
	FWI_SET_BIT_TO(_store[1], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_if() {
	return FWI_IS_BIT_SET(_store[1], 3);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_if(bool value) {
	FWI_SET_BIT_TO(_store[1], 3, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::PWM::get_casting() {
	return FWI_IS_BIT_SET(_store[1], 4);
}
FWIFUNC void Toast::Memory::Shared::IO::PWM::set_casting(bool value) {
	FWI_SET_BIT_TO(_store[1], 4, value ? 1 : 0);
}


