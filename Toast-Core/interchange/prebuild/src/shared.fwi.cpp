#include "shared.fwi.hpp"

FWIFUNC Toast::Memory::Shared::IO::Power * Toast::Memory::SharedPool::power() {
	return &_power;
}

FWIFUNC Toast::Memory::Shared::IO::DIO * Toast::Memory::SharedPool::dio(int index) {
	return &_dio[index];
}

FWIFUNC Toast::Memory::Shared::IO::Relay * Toast::Memory::SharedPool::relay(int index) {
	return &_relay[index];
}

FWIFUNC Toast::Memory::Shared::IO::AnalogOut * Toast::Memory::SharedPool::analog_out(int index) {
	return &_analog_out[index];
}

FWIFUNC Toast::Memory::Shared::IO::AnalogIn * Toast::Memory::SharedPool::analog_in(int index) {
	return &_analog_in[index];
}

FWIFUNC Toast::Memory::Shared::IO::AnalogInSettings * Toast::Memory::SharedPool::analog_in_settings() {
	return &_analog_in_settings;
}

FWIFUNC Toast::Memory::Shared::IO::Pneumatics * Toast::Memory::SharedPool::pneumatics(int index) {
	return &_pneumatics[index];
}

FWIFUNC Toast::Memory::Shared::IO::Motor * Toast::Memory::SharedPool::motor(int index) {
	return &_motors[index];
}

FWIFUNC Toast::Memory::Shared::IO::PWM * Toast::Memory::SharedPool::pwm(int index) {
	return &_pwm[index];
}

FWIFUNC Toast::Memory::Shared::IO::Servo * Toast::Memory::SharedPool::servo(int index) {
	return &_servos[index];
}

FWIFUNC Toast::Memory::Shared::IO::ServoStatic * Toast::Memory::SharedPool::servo_static() {
	return &_servo_static;
}

FWIFUNC Toast::Memory::Shared::IO::OnboardAccel * Toast::Memory::SharedPool::onboard_accel() {
	return &_onboard_accel;
}

FWIFUNC Toast::Memory::Shared::DS::Joystick * Toast::Memory::SharedPool::joystick(int index) {
	return &_joysticks[index];
}

FWIFUNC Toast::Memory::Shared::DS::DSInfo * Toast::Memory::SharedPool::ds_info() {
	return &_ds_info;
}

FWIFUNC uint8_t Toast::Memory::SharedPool::get_endian() {
	return FWI_MEM_VAL(uint8_t, _store, 1);
}
FWIFUNC void Toast::Memory::SharedPool::set_endian(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 1) = value;
}

FWIFUNC int32_t Toast::Memory::SharedPool::get_bootstrap_pid() {
	return FWI_MEM_VAL(int32_t, _store, 2);
}
FWIFUNC void Toast::Memory::SharedPool::set_bootstrap_pid(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 2) = value;
}

FWIFUNC Toast::Memory::RobotState Toast::Memory::SharedPool::get_state_current() {
	return (Toast::Memory::RobotState)(_store[6]);
}
FWIFUNC void Toast::Memory::SharedPool::set_state_current(Toast::Memory::RobotState value) {
	_store[6] = (char)value;
}

FWIFUNC Toast::Memory::RobotState Toast::Memory::SharedPool::get_state_last() {
	return (Toast::Memory::RobotState)(_store[7]);
}
FWIFUNC void Toast::Memory::SharedPool::set_state_last(Toast::Memory::RobotState value) {
	_store[7] = (char)value;
}

FWIFUNC uint8_t Toast::Memory::SharedPool::get_tick_timing() {
	return FWI_MEM_VAL(uint8_t, _store, 8);
}
FWIFUNC void Toast::Memory::SharedPool::set_tick_timing(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 8) = value;
}

FWIFUNC bool Toast::Memory::SharedPool::get_debug() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::SharedPool::set_debug(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC Toast::Memory::ModuleActState Toast::Memory::SharedPool::get_module_activity_state(int index) {
	return (Toast::Memory::ModuleActState)(_store[9 + (1 * index)]);
}
FWIFUNC void Toast::Memory::SharedPool::set_module_activity_state(int index, Toast::Memory::ModuleActState value) {
	_store[9 + (1 * index)] = (char)value;
}


void Toast::Memory::SharedPool::_update_ptr() {
	_power.map_to(_store + 137);
	int i;
	for (i = 0; i < 26; i++) {
		_dio[i].map_to(_store + 261 + (15 * i));
	}
	for (i = 0; i < 4; i++) {
		_relay[i].map_to(_store + 651 + (1 * i));
	}
	for (i = 0; i < 2; i++) {
		_analog_out[i].map_to(_store + 655 + (5 * i));
	}
	for (i = 0; i < 8; i++) {
		_analog_in[i].map_to(_store + 665 + (59 * i));
	}
	_analog_in_settings.map_to(_store + 1137);
	for (i = 0; i < 2; i++) {
		_pneumatics[i].map_to(_store + 1141 + (10 * i));
	}
	for (i = 0; i < 16; i++) {
		_motors[i].map_to(_store + 1161 + (8 * i));
	}
	for (i = 0; i < 20; i++) {
		_pwm[i].map_to(_store + 1289 + (33 * i));
	}
	for (i = 0; i < 20; i++) {
		_servos[i].map_to(_store + 1949 + (5 * i));
	}
	_servo_static.map_to(_store + 2049);
	_onboard_accel.map_to(_store + 2061);
	for (i = 0; i < 6; i++) {
		_joysticks[i].map_to(_store + 2075 + (120 * i));
	}
	_ds_info.map_to(_store + 2795);
}
