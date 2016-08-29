#include "shared/ds.fwi.hpp"

FWIFUNC bool Toast::Memory::Shared::DS::JoystickDescriptor::get_is_xbox() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::DS::JoystickDescriptor::set_is_xbox(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC Toast::Memory::Shared::DS::JoystickType Toast::Memory::Shared::DS::JoystickDescriptor::get_type() {
	return (Toast::Memory::Shared::DS::JoystickType)(_store[1]);
}
FWIFUNC void Toast::Memory::Shared::DS::JoystickDescriptor::set_type(Toast::Memory::Shared::DS::JoystickType value) {
	_store[1] = (char)value;
}

FWIFUNC uint8_t Toast::Memory::Shared::DS::JoystickDescriptor::get_name_length() {
	return FWI_MEM_VAL(uint8_t, _store, 2);
}
FWIFUNC void Toast::Memory::Shared::DS::JoystickDescriptor::set_name_length(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 2) = value;
}

FWIFUNC char * Toast::Memory::Shared::DS::JoystickDescriptor::get_name() {
	return (_store + 3);
}
FWIFUNC int Toast::Memory::Shared::DS::JoystickDescriptor::name_length() {
	return 60;
}

FWIFUNC uint8_t Toast::Memory::Shared::DS::JoystickDescriptor::get_axis_count() {
	return FWI_MEM_VAL(uint8_t, _store, 63);
}
FWIFUNC void Toast::Memory::Shared::DS::JoystickDescriptor::set_axis_count(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 63) = value;
}

FWIFUNC Toast::Memory::Shared::DS::JoystickAxisType Toast::Memory::Shared::DS::JoystickDescriptor::get_axis_type(int index) {
	return (Toast::Memory::Shared::DS::JoystickAxisType)(_store[64 + (1 * index)]);
}
FWIFUNC void Toast::Memory::Shared::DS::JoystickDescriptor::set_axis_type(int index, Toast::Memory::Shared::DS::JoystickAxisType value) {
	_store[64 + (1 * index)] = (char)value;
}


FWIFUNC Toast::Memory::Shared::DS::JoystickDescriptor * Toast::Memory::Shared::DS::Joystick::get_descriptor() {
	return &_descriptor;
}

FWIFUNC bool Toast::Memory::Shared::DS::Joystick::get_init() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_init(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::DS::Joystick::get_bootstrap() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_bootstrap(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC uint8_t Toast::Memory::Shared::DS::Joystick::get_num_axis() {
	return FWI_MEM_VAL(uint8_t, _store, 1);
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_num_axis(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 1) = value;
}

FWIFUNC uint8_t Toast::Memory::Shared::DS::Joystick::get_num_button() {
	return FWI_MEM_VAL(uint8_t, _store, 2);
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_num_button(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 2) = value;
}

FWIFUNC uint8_t Toast::Memory::Shared::DS::Joystick::get_num_pov() {
	return FWI_MEM_VAL(uint8_t, _store, 3);
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_num_pov(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 3) = value;
}

FWIFUNC uint32_t Toast::Memory::Shared::DS::Joystick::get_button_mask() {
	return FWI_MEM_VAL(uint32_t, _store, 4);
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_button_mask(uint32_t value) {
	FWI_MEM_VAL(uint32_t, _store, 4) = value;
}

FWIFUNC uint32_t Toast::Memory::Shared::DS::Joystick::get_outputs_mask() {
	return FWI_MEM_VAL(uint32_t, _store, 8);
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_outputs_mask(uint32_t value) {
	FWI_MEM_VAL(uint32_t, _store, 8) = value;
}

FWIFUNC int16_t Toast::Memory::Shared::DS::Joystick::get_pov(int index) {
	return FWI_MEM_VAL(int16_t, _store, 12 + (2 * index));
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_pov(int index, int16_t value) {
	FWI_MEM_VAL(int16_t, _store, 12 + (2 * index)) = value;
}

FWIFUNC int8_t Toast::Memory::Shared::DS::Joystick::get_axis(int index) {
	return FWI_MEM_VAL(int8_t, _store, 20 + (1 * index));
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_axis(int index, int8_t value) {
	FWI_MEM_VAL(int8_t, _store, 20 + (1 * index)) = value;
}

FWIFUNC int16_t Toast::Memory::Shared::DS::Joystick::get_rumble_l() {
	return FWI_MEM_VAL(int16_t, _store, 36);
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_rumble_l(int16_t value) {
	FWI_MEM_VAL(int16_t, _store, 36) = value;
}

FWIFUNC int16_t Toast::Memory::Shared::DS::Joystick::get_rumble_r() {
	return FWI_MEM_VAL(int16_t, _store, 38);
}
FWIFUNC void Toast::Memory::Shared::DS::Joystick::set_rumble_r(int16_t value) {
	FWI_MEM_VAL(int16_t, _store, 38) = value;
}


void Toast::Memory::Shared::DS::Joystick::_update_ptr() {
	_descriptor.map_to(_store + 40);
}
FWIFUNC bool Toast::Memory::Shared::DS::DSInfo::get_ds_attached() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::DS::DSInfo::set_ds_attached(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::DS::DSInfo::get_new_control_data() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::DS::DSInfo::set_new_control_data(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::DS::DSInfo::get_fms_attached() {
	return FWI_IS_BIT_SET(_store[0], 2);
}
FWIFUNC void Toast::Memory::Shared::DS::DSInfo::set_fms_attached(bool value) {
	FWI_SET_BIT_TO(_store[0], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::DS::DSInfo::get_system_active() {
	return FWI_IS_BIT_SET(_store[0], 3);
}
FWIFUNC void Toast::Memory::Shared::DS::DSInfo::set_system_active(bool value) {
	FWI_SET_BIT_TO(_store[0], 3, value ? 1 : 0);
}

FWIFUNC float Toast::Memory::Shared::DS::DSInfo::get_match_time() {
	return FWI_MEM_VAL(float, _store, 1);
}
FWIFUNC void Toast::Memory::Shared::DS::DSInfo::set_match_time(float value) {
	FWI_MEM_VAL(float, _store, 1) = value;
}

FWIFUNC Toast::Memory::Shared::DS::Alliance Toast::Memory::Shared::DS::DSInfo::get_alliance() {
	return (Toast::Memory::Shared::DS::Alliance)(_store[5]);
}
FWIFUNC void Toast::Memory::Shared::DS::DSInfo::set_alliance(Toast::Memory::Shared::DS::Alliance value) {
	_store[5] = (char)value;
}

FWIFUNC uint8_t Toast::Memory::Shared::DS::DSInfo::get_alliance_station() {
	return FWI_MEM_VAL(uint8_t, _store, 6);
}
FWIFUNC void Toast::Memory::Shared::DS::DSInfo::set_alliance_station(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 6) = value;
}


