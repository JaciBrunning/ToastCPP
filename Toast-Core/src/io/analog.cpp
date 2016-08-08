#include "io/analog.hpp"

using namespace IO;
using namespace Toast::Memory;

float IO::get_analog_sample_rate() {
	return shared()->analog_in_settings()->get_sample_rate();
}

// Analog Output
AnalogOutput::AnalogOutput(int port) : _port(port) {
	_mem = shared()->analog_out(port);
	_mem->set_init(true);
}

int AnalogOutput::get_port() const {
	return _port;
}

void AnalogOutput::set(float volts) {
	_mem->set_voltage(volts);
}

float AnalogOutput::get() {
	return _mem->get_voltage();
}

// Analog Input
AnalogInput::AnalogInput(int port) : _port(port) {
	_mem = shared()->analog_in(port);
	_mem->set_init(true);
}

int AnalogInput::get_port() {
	return _port;
}

int16_t AnalogInput::get_value() {
	return _mem->get_value();
}

int32_t AnalogInput::get_average_value() {
	return _mem->get_average_value();
}

float AnalogInput::get() {
	return _mem->get_voltage();
}

float AnalogInput::get_average() {
	return _mem->get_average_voltage();
}

void AnalogInput::set_average_bits(uint32_t bits) {
	_mem->set_average_bits(bits);
}

uint32_t AnalogInput::get_average_bits() {
	return _mem->get_average_bits();
}

void AnalogInput::set_oversample_bits(uint32_t bits) {
	_mem->set_oversample_bits(bits);
}

uint32_t AnalogInput::get_oversample_bits() {
	return _mem->get_oversample_bits();
}

uint32_t AnalogInput::get_lsb_weight() {
	return _mem->get_lsb_weight();
}

int32_t AnalogInput::get_offset() {
	return _mem->get_offset();
}

bool AnalogInput::is_accumulator() {
	return _mem->get_is_accum();
}

void AnalogInput::init_accumulator() {
	_mem->set_accum_pending(true);
}

void AnalogInput::set_accumulator_initial(int64_t initial) {
	_mem->set_accum_initial(initial);
	_mem->set_accum_initial_pending(true);
}

void AnalogInput::reset_accumulator() {
	_mem->set_accum_reset_pending(true);
}

void AnalogInput::set_accumulator_center(int32_t center) {
	_mem->set_accum_centre(center);
	_mem->set_accum_centre_pending(true);
}

void AnalogInput::set_accumulator_deadband(int32_t deadband) {
	_mem->set_accum_deadband(deadband);
	_mem->set_accum_deadband_pending(true);
}

int64_t AnalogInput::get_accumulator_value() {
	return _mem->get_accum_value();
}

uint32_t AnalogInput::get_accumulator_count() {
	return _mem->get_accum_count();
}