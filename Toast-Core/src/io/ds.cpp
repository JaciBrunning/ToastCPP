#include "io/ds.hpp"

#include <inttypes.h>

using namespace IO;
using namespace Toast::Memory;

static inline Toast::Concurrent::IPCMutex *mtx() {
	return shared_mutex()->joy;
}

// Joysticks

Joystick::Joystick(int port) : _port(port) {
	_mem = shared()->joystick(port);
	MTX_WRAP(mtx(), port, _mem->set_init(true));
}

int Joystick::get_port() {
	return _port;
}

int Joystick::get_num_axis() {
	MTX_RETURN(mtx(), _port, _mem->get_num_axis());
}

int Joystick::get_num_button() {
	MTX_RETURN(mtx(), _port, _mem->get_num_button());
}

int Joystick::get_num_pov() {
	MTX_RETURN(mtx(), _port, _mem->get_num_pov());
}

bool Joystick::get_raw_button(int id) {
	MTX_RETURN(mtx(), _port, FWI_IS_BIT_SET(_mem->get_button_mask(), id));
}

float Joystick::get_raw_axis(int id) {
	MTX_WRAP(mtx(), _port, int8_t val = _mem->get_axis(id));
	if (val < 0) {
		return val / 128.0f;
	} else {
		return val / 127.0f;
	}
}

int Joystick::get_pov(int id) {
	MTX_RETURN(mtx(), _port, _mem->get_pov(id));
}

void Joystick::set_rumble_left(float mag) {
	if (mag > 1)		mag = 1.0;
	else if (mag < 0)	mag = 0.0;
	MTX_WRAP(mtx(), _port, _mem->set_rumble_l((uint16_t)(mag * 65535)));
}

float Joystick::get_rumble_left() {
	MTX_RETURN(mtx(), _port, _mem->get_rumble_l() / 65535.0f);
}

void Joystick::set_rumble_right(float mag) {
	if (mag > 1)		mag = 1.0;
	else if (mag < 0)	mag = 0.0;
	MTX_WRAP(mtx(), _port, _mem->set_rumble_r((int16_t)(mag * 65535)));
}

float Joystick::get_rumble_right() {
	MTX_RETURN(mtx(), _port, _mem->get_rumble_r() / 65535.0f);
}

void Joystick::set_output(int id, bool value) {
	MTX_LOCK(mtx(), _port);
	uint32_t op = _mem->get_outputs_mask();
	FWI_SET_BIT_TO(op, id, value);
	_mem->set_outputs_mask(op);
	MTX_UNLOCK(mtx(), _port);
}

void Joystick::set_outputs(uint32_t outputs) {
	MTX_WRAP(mtx(), _port, _mem->set_outputs_mask(outputs));
}

bool Joystick::get_output(int id) {
	MTX_RETURN(mtx(), _port, FWI_IS_BIT_SET(_mem->get_outputs_mask(), id));
}

uint32_t Joystick::get_outputs() {
	MTX_RETURN(mtx(), _port, _mem->get_outputs_mask());
}

// Driver Station Comms (Note these don't need a mutex because they are single-write)

bool DS::is_ds_attached() {
	return shared()->ds_info()->get_ds_attached();
}

bool DS::is_fms_attached() {
	return shared()->ds_info()->get_fms_attached();
}

bool DS::is_new_control_data() {
	return shared()->ds_info()->get_new_control_data();
}

bool DS::is_system_active() {
	return shared()->ds_info()->get_system_active();
}

float DS::get_match_time() {
	return shared()->ds_info()->get_match_time();
}

DS::Alliance DS::get_alliance() {
	return shared()->ds_info()->get_alliance();
}

int DS::get_alliance_station() {
	return shared()->ds_info()->get_alliance_station();
}