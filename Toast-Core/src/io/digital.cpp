#include "io/digital.hpp"

using namespace IO;
using namespace Toast::Memory;

DIO::DIO(int port, DIO::Mode mode) : _port(port) {
	_mem = shared()->dio(port);
	_mem->set_init(true);
	_mem->set_mode(mode);
}

int DIO::get_port() {
	return _port;
}

DIO::Mode DIO::get_mode() {
	return _mem->get_mode();
}

bool DIO::get() {
	return _mem->get_value();
}

void DIO::set(bool state) {
	_mem->set_value(state);
}

void DIO::set_pulse(float length) {
	_mem->set_pulse_length(length);
	_mem->set_pulse_pending(true);
}

bool DIO::is_pulsing() {
	return _mem->is_pulsing();
}

void DIO::set_pwm_rate(float rate) {
	_mem->set_pwm_rate(rate);
	_mem->set_pwm_rate_pending(true);
}

void DIO::set_pwm_enable(float initial_duty_cycle) {
	_mem->set_pwm_duty_cycle(initial_duty_cycle);
	_mem->set_pwm_enabled(true);
	_mem->set_pwm_enabled_pending(true);
}

void DIO::set_pwm_disable() {
	_mem->set_pwm_enabled(false);
	_mem->set_pwm_enabled_pending(true);
}

void DIO::set_pwm_duty_cycle(float duty_cycle) {
	_mem->set_pwm_duty_cycle(duty_cycle);
	_mem->set_pwm_duty_cycle_pending(true);
}