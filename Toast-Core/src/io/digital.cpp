#include "io/digital.hpp"

using namespace IO;
using namespace Toast::Memory;

static inline Toast::Concurrent::IPCMutex *mtx() {
	return shared_mutex()->dio;
}

DIO::DIO(int port, DIO::Mode mode) : _port(port) {
	_mem = shared()->dio(port);
	MTX_LOCK(mtx(), port);
	_mem->set_init(true);
	_mem->set_mode(mode);
	MTX_UNLOCK(mtx(), port);
}

int DIO::get_port() {
	return _port;
}

DIO::Mode DIO::get_mode() {
	return _mem->get_mode();
}

bool DIO::get() {
	MTX_RETURN(mtx(), _port, _mem->get_value());
}

void DIO::set(bool state) {
	MTX_WRAP(mtx(), _port, _mem->set_value(state));
}

void DIO::set_pulse(float length) {
	MTX_LOCK(mtx(), _port);
	_mem->set_pulse_length(length);
	_mem->set_pulse_pending(true);
	MTX_UNLOCK(mtx(), _port);
}

bool DIO::is_pulsing() {
	MTX_RETURN(mtx(), _port, _mem->is_pulsing());
}

void DIO::set_pwm_rate(float rate) {
	MTX_LOCK(mtx(), _port);
	_mem->set_pwm_rate(rate);
	_mem->set_pwm_rate_pending(true);
	MTX_UNLOCK(mtx(), _port);
}

void DIO::set_pwm_enable(float initial_duty_cycle) {
	MTX_LOCK(mtx(), _port);
	_mem->set_pwm_duty_cycle(initial_duty_cycle);
	_mem->set_pwm_enabled(true);
	_mem->set_pwm_enabled_pending(true);
	MTX_UNLOCK(mtx(), _port);
}

void DIO::set_pwm_disable() {
	MTX_LOCK(mtx(), _port);
	_mem->set_pwm_enabled(false);
	_mem->set_pwm_enabled_pending(true);
	MTX_UNLOCK(mtx(), _port);
}

void DIO::set_pwm_duty_cycle(float duty_cycle) {
	MTX_LOCK(mtx(), _port);
	_mem->set_pwm_duty_cycle(duty_cycle);
	_mem->set_pwm_duty_cycle_pending(true);
	MTX_UNLOCK(mtx(), _port);
}