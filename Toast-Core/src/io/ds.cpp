#include "io/ds.hpp"

#include <inttypes.h>

using namespace IO;
using namespace Toast::Memory;

char *IO::get_joystick_block(int port) {
	PORT_CHECK(port, 6);
	return Shared::get() + ADDR_JOY_OFFSET + (port * LEN_JOY);
}

char *IO::get_ds_info_block() {
	return Shared::get() + ADDR_DS_INFO_OFFSET;
}

// Joysticks

Joystick::Joystick(int port) : _port(port) {
	_shm = get_joystick_block(port);

	SET_BIT(_shm[ADDR_JOY_BOOTINIT], 0);
}

int Joystick::get_port() {
	return _port;
}

int Joystick::get_num_axis() {
	return (int)_shm[ADDR_JOY_NUM_AXIS];
}

int Joystick::get_num_button() {
	return (int)_shm[ADDR_JOY_NUM_BUTTON];
}

int Joystick::get_num_pov() {
	return (int)_shm[ADDR_JOY_NUM_POV];
}

uint32_t Joystick::get_button_mask() {
	return MEM_VAL(uint32_t, _shm, ADDR_JOY_BUTTON_MASK);
}

bool Joystick::get_raw_button(int id) {
	return IS_BIT_SET_MULTIWIDE(uint32_t, _shm, ADDR_JOY_BUTTON_MASK, id);
}

float Joystick::get_raw_axis(int id) {
	int8_t val = (int8_t)_shm[ADDR_JOY_AXIS_OFFSET + (id)];
	if (val < 0) {
		return val / 128.0f;
	} else {
		return val / 127.0f;
	}
}

int Joystick::get_pov(int id) {
	int16_t val = MEM_VAL(int16_t, _shm, ADDR_JOY_POV_OFFSET + (id * 2));
	return (int)val;
}

void Joystick::set_rumble_left(float mag) {
	if (mag > 1)		mag = 1.0;
	else if (mag < 0)	mag = 0.0;
	MEM_VAL(int16_t, _shm, ADDR_JOY_RUMBLE_L) = (int16_t)(mag * 65535);
}

float Joystick::get_rumble_left() {
	return MEM_VAL(int16_t, _shm, ADDR_JOY_RUMBLE_L) / 65535.0f;
}

void Joystick::set_rumble_right(float mag) {
	if (mag > 1)		mag = 1.0;
	else if (mag < 0)	mag = 0.0;
	MEM_VAL(int16_t, _shm, ADDR_JOY_RUMBLE_R) = (int16_t)(mag * 65535);
}

float Joystick::get_rumble_right() {
	return MEM_VAL(int16_t, _shm, ADDR_JOY_RUMBLE_R) / 65535.0f;
}

void Joystick::set_output(int id, bool value) {
	uint32_t op = _shm[ADDR_JOY_OUTPUTS];
	SET_BIT_TO(op, id, value);
	_shm[ADDR_JOY_OUTPUTS] = op;
}

void Joystick::set_outputs(int id, uint32_t outputs) {
	_shm[ADDR_JOY_OUTPUTS] = outputs;
}

bool Joystick::get_output(int id) {
	return IS_BIT_SET(_shm[ADDR_JOY_OUTPUTS], id);
}

uint32_t Joystick::get_outputs() {
	return _shm[ADDR_JOY_OUTPUTS];
}

// Driver Station Comms

bool DS::is_ds_attached() {
	return get_ds_info_block()[ADDR_DS_INFO_DS_ATTACHED] == 1;
}

bool DS::is_fms_attached() {
	return get_ds_info_block()[ADDR_DS_INFO_FMS_ATTACHED] == 1;
}

bool DS::is_new_control_data() {
	return get_ds_info_block()[ADDR_DS_INFO_NEW_CTRL_DATA] == 1;
}

bool DS::is_system_active() {
	return get_ds_info_block()[ADDR_DS_INFO_SYSTEM_ACTIVE] == 1;
}

float DS::get_match_time() {
	return MEM_VAL(float, get_ds_info_block(), ADDR_DS_INFO_MATCH_TIME);
}

DS::Alliance DS::get_alliance() {
	return static_cast<DS::Alliance>((int)get_ds_info_block()[ADDR_DS_INFO_ALLIANCE]);
}

int DS::get_alliance_station() {
	return (int) get_ds_info_block()[ADDR_DS_INFO_ALLIANCE_STATION];
}