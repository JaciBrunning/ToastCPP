#include "io/pneumatics.hpp"

using namespace IO;
using namespace Toast::Memory;

int IO::get_pcm_id(int module_id) {
	int i;
	for (i = 0; i < 2; i++) {
		Shared::IO::Pneumatics *_p = shared()->pneumatics(i);
		if (module_id == _p->get_pcm_can_id()) return i;
		if (!_p->get_init()) return i;
	}
	return -1;
}

static inline Toast::Concurrent::IPCMutex *mtx() {
	return shared_mutex()->pcm;
}

PCM::PCM(int module_id) : _mod_id(module_id) {
	_internal_id = IO::get_pcm_id(module_id);
	_mem = shared()->pneumatics(_internal_id);
	MTX_LOCK(mtx(), _internal_id);
	_mem->set_pcm_can_id(module_id);
	_mem->set_init(true);
	MTX_UNLOCK(mtx(), _internal_id);
}

int PCM::get_module_id() {
	return _mod_id;
}

bool PCM::get_solenoid(int solenoid) {
	MTX_RETURN(mtx(), _internal_id, _mem->get_solenoid(solenoid));
}

bool PCM::solenoid_blacklisted(int solenoid) {
	MTX_RETURN(mtx(), _internal_id, _mem->get_solenoid_black(solenoid));
}

void PCM::set_solenoid(int solenoid, bool value) {
	MTX_WRAP(mtx(), _internal_id, _mem->set_solenoid(solenoid, value));
}

void PCM::start_compressor() {
	MTX_WRAP(mtx(), _internal_id, _mem->set_start_pending(true));
}

void PCM::stop_compressor() {
	MTX_WRAP(mtx(), _internal_id, _mem->set_stop_pending(true));
}

bool PCM::compressor_enabled() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_is_enabled());
}

void PCM::set_closed_loop_control(bool state) {
	MTX_LOCK(mtx(), _internal_id);
	_mem->set_closed_loop(true);
	_mem->set_closed_loop_mode_pending(true);
	MTX_UNLOCK(mtx(), _internal_id);
}

bool PCM::closed_loop_control() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_closed_loop());
}

float PCM::compressor_current() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_comp_current());
}

bool PCM::pressure_switch_low() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_pressure_switch());
}

void PCM::clear_sticky_faults() {
	MTX_WRAP(mtx(), _internal_id, _mem->set_comp_sticky_clear_pending(true));
}

void PCM::clear_solenoid_sticky_faults() {
	MTX_WRAP(mtx(), _internal_id, _mem->set_sol_sticky_clear_pending(true));
}

bool PCM::compressor_current_too_high_fault() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_fault_comp_too_high());
}

bool PCM::compressor_current_too_high_sticky_fault() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_fault_comp_too_high_sticky());
}

bool PCM::compressor_shorted_fault() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_fault_comp_shorted());
}

bool PCM::compressor_shorted_sticky_fault() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_fault_comp_shorted_sticky());
}

bool PCM::compressor_not_connected_fault() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_fault_comp_not_conn());
}

bool PCM::compressor_not_connected_sticky_fault() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_fault_comp_not_conn_sticky());
}

bool PCM::solenoid_voltage_fault() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_fault_sol_volt());
}

bool PCM::solenoid_voltage_sticky_fault() {
	MTX_RETURN(mtx(), _internal_id, _mem->get_fault_sol_volt_sticky());
}