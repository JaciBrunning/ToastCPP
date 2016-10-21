#include "io/analog.hpp"

using namespace IO;
using namespace Toast::Memory;

float IO::get_analog_sample_rate() {
	MTX_RETURN(shared_mutex()->analog_in_static, 0, shared()->analog_in_settings()->get_sample_rate());
}

static inline Toast::Concurrent::IPCMutex *mtx_o() {
	return shared_mutex()->analog_out;
}

static inline Toast::Concurrent::IPCMutex *mtx_i() {
	return shared_mutex()->analog_in;
}

// Analog Output
AnalogOutput::AnalogOutput(int port) : _port(port) {
	_mem = shared()->analog_out(port);
	MTX_WRAP(mtx_o(), port, _mem->set_init(true));
}

int AnalogOutput::get_port() const {
	return _port;
}

void AnalogOutput::set(float volts) {
	MTX_WRAP(mtx_o(), _port, _mem->set_voltage(volts));
}

float AnalogOutput::get() {
	MTX_RETURN(mtx_o(), _port, _mem->get_voltage());
}

// Analog Input
AnalogInput::AnalogInput(int port) : _port(port) {
	_mem = shared()->analog_in(port);
	MTX_WRAP(mtx_i(), port, _mem->set_init(true));
}

int AnalogInput::get_port() {
	return _port;
}

int16_t AnalogInput::get_value() {
	MTX_RETURN(mtx_i(), _port, _mem->get_value());
}

int32_t AnalogInput::get_average_value() {
	MTX_RETURN(mtx_i(), _port, _mem->get_average_value());
}

float AnalogInput::get() {
	MTX_RETURN(mtx_i(), _port, _mem->get_voltage());
}

float AnalogInput::get_average() {
	MTX_RETURN(mtx_i(), _port, _mem->get_average_voltage());
}

void AnalogInput::set_average_bits(uint32_t bits) {
	MTX_WRAP(mtx_i(), _port, _mem->set_average_bits(bits));
}

uint32_t AnalogInput::get_average_bits() {
	MTX_RETURN(mtx_i(), _port, _mem->get_average_bits());
}

void AnalogInput::set_oversample_bits(uint32_t bits) {
	MTX_WRAP(mtx_i(), _port, _mem->set_oversample_bits(bits));
}

uint32_t AnalogInput::get_oversample_bits() {
	MTX_RETURN(mtx_i(), _port, _mem->get_oversample_bits());
}

uint32_t AnalogInput::get_lsb_weight() {
	MTX_RETURN(mtx_i(), _port, _mem->get_lsb_weight());
}

int32_t AnalogInput::get_offset() {
	MTX_RETURN(mtx_i(), _port, _mem->get_offset());
}

bool AnalogInput::is_accumulator() {
	MTX_RETURN(mtx_i(), _port, _mem->get_is_accum());
}

void AnalogInput::init_accumulator() {
	MTX_WRAP(mtx_i(), _port, _mem->set_accum_pending(true));
}

void AnalogInput::set_accumulator_initial(int64_t initial) {
	MTX_LOCK(mtx_i(), _port);
	_mem->set_accum_initial(initial);
	_mem->set_accum_initial_pending(true);
	MTX_UNLOCK(mtx_i(), _port);
}

void AnalogInput::reset_accumulator() {
	MTX_WRAP(mtx_i(), _port, _mem->set_accum_reset_pending(true));
}

void AnalogInput::set_accumulator_center(int32_t center) {
	MTX_LOCK(mtx_i(), _port);
	_mem->set_accum_centre(center);
	_mem->set_accum_centre_pending(true);
	MTX_UNLOCK(mtx_i(), _port);
}

void AnalogInput::set_accumulator_deadband(int32_t deadband) {
	MTX_LOCK(mtx_i(), _port);
	_mem->set_accum_deadband(deadband);
	_mem->set_accum_deadband_pending(true);
	MTX_UNLOCK(mtx_i(), _port);
}

int64_t AnalogInput::get_accumulator_value() {
	MTX_RETURN(mtx_i(), _port, _mem->get_accum_value());
}

uint32_t AnalogInput::get_accumulator_count() {
	MTX_RETURN(mtx_i(), _port, _mem->get_accum_count());
}