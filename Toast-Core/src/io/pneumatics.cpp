#include "io/pneumatics.hpp"

using namespace IO;
using namespace Toast::Memory;

int IO::get_PCM_shm_id(int module_id) {
	int i;
	for (i = 0; i < 2; i++) {
		char *block = get_PCM_block(i);
		int bl_mod_id = (int)(block[ADDR_PCM_CAN_ID]);
		if (bl_mod_id == module_id) return i;
		if (!IS_BIT_SET(block[ADDR_PCM_BOOTINIT], 0)) return i;
	}
	throw IO::CouldNotAllocateBlockException("Could not allocate block for PCM (have you registered more than 2 PCMs?)");
	return -1;
}

char *IO::get_PCM_block(int id) {
	BLOCK_CHECK(id, 2);
	return Shared::get() + ADDR_PCM_OFFSET + (id * LEN_PCM);
}

PCM::PCM(int module_id) : _mod_id(module_id) {
	_shm_id = get_PCM_shm_id(module_id);
	_shm = get_PCM_block(_shm_id);
	_shm[ADDR_PCM_CAN_ID] = module_id;
	SET_BIT(_shm[ADDR_PCM_BOOTINIT], 0);
}

int PCM::get_shm_id() {
	return _shm_id;
}

int PCM::get_module_id() {
	return _mod_id;
}

bool PCM::get_solenoid(int solenoid) {
	return IS_BIT_SET(_shm[ADDR_PCM_SOLENOID_GET], solenoid);
}

bool PCM::solenoid_blacklisted(int solenoid) {
	return IS_BIT_SET(_shm[ADDR_PCM_SOLENOID_BLACK], solenoid);
}

void PCM::set_solenoid(int solenoid, bool value) {
	SET_BIT_TO(_shm[ADDR_PCM_SOLENOID_SET], solenoid, value);
}

void PCM::start_compressor() {
	SET_BIT(_shm[ADDR_PCM_CONTROL_FLAGS], 3);
}

void PCM::stop_compressor() {
	SET_BIT(_shm[ADDR_PCM_CONTROL_FLAGS], 4);
}

bool PCM::compressor_enabled() {
	return IS_BIT_SET(_shm[ADDR_PCM_CONTROL_FLAGS], 2);
}

void PCM::set_closed_loop_control(bool state) {
	SET_BIT_TO(_shm[ADDR_PCM_CONTROL_FLAGS], 1, 1);
	SET_BIT(_shm[ADDR_PCM_CONTROL_FLAGS], 5);
}

bool PCM::closed_loop_control() {
	return IS_BIT_SET(_shm[ADDR_PCM_CONTROL_FLAGS], 1);
}

float PCM::compressor_current() {
	return MEM_VAL(float, _shm, ADDR_PCM_COMPRESSOR_CURRENT);
}

bool PCM::pressure_switch_low() {
	return IS_BIT_SET(_shm[ADDR_PCM_CONTROL_FLAGS], 0);
}

void PCM::clear_sticky_faults() {
	SET_BIT(_shm[ADDR_PCM_CMP_FAULT_STATUS], 6);
}

void PCM::clear_solenoid_sticky_faults() {
	SET_BIT(_shm[ADDR_PCM_SOLENOID_FAULT_STATUS], 2);
}

bool PCM::compressor_current_too_high_fault() {
	return IS_BIT_SET(_shm[ADDR_PCM_CMP_FAULT_STATUS], 0);
}

bool PCM::compressor_current_too_high_sticky_fault() {
	return IS_BIT_SET(_shm[ADDR_PCM_CMP_FAULT_STATUS], 1);
}

bool PCM::compressor_shorted_fault() {
	return IS_BIT_SET(_shm[ADDR_PCM_CMP_FAULT_STATUS], 2);
}

bool PCM::compressor_shorted_sticky_fault() {
	return IS_BIT_SET(_shm[ADDR_PCM_CMP_FAULT_STATUS], 3);
}

bool PCM::compressor_not_connected_fault() {
	return IS_BIT_SET(_shm[ADDR_PCM_CMP_FAULT_STATUS], 4);
}

bool PCM::compressor_not_connected_sticky_fault() {
	return IS_BIT_SET(_shm[ADDR_PCM_CMP_FAULT_STATUS], 5);
}

bool PCM::solenoid_voltage_fault() {
	return IS_BIT_SET(_shm[ADDR_PCM_SOLENOID_FAULT_STATUS], 0);
}

bool PCM::solenoid_voltage_sticky_fault() {
	return IS_BIT_SET(_shm[ADDR_PCM_SOLENOID_FAULT_STATUS], 1);
}