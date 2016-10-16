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


