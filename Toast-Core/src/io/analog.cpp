#include "io/analog.hpp"

using namespace IO;
using namespace Toast::Memory;

float IO::get_analog_sample_rate() {
	return *(float *)(Shared::get() + ADDR_AN_SS_OFFSET + ADDR_AN_SS_SAMPLE_RATE);
}

char *IO::get_analog_out_block(int port) {
	PORT_CHECK(port, 2);
	return Shared::get() + ADDR_AN_OUT_OFFSET + (port * LEN_AN_OUT);
}

char *IO::get_analog_in_block(int port) {
	PORT_CHECK(port, 8);
	return Shared::get() + ADDR_AN_IN_OFFSET + (port * LEN_AN_IN);
}

// Analog Output
AnalogOutput::AnalogOutput(int port) : _port(port) {
	_shm = get_analog_out_block(port);

	_shm[ADDR_AN_OUT_PORT] = (char)port;
	SET_BIT(_shm[ADDR_AN_OUT_BOOTINIT], 0);
}

int AnalogOutput::get_port() {
	return _port;
}

void AnalogOutput::set(float volts) {
	MEM_VAL(float, _shm, ADDR_AN_OUT_VOLTAGE) = volts;
}

float AnalogOutput::get() {
	return MEM_VAL(float, _shm, ADDR_AN_OUT_VOLTAGE);
}

// Analog Input
AnalogInput::AnalogInput(int port) : _port(port) {
	_shm = get_analog_in_block(port);
	_shm[ADDR_AN_IN_PORT] = (char)port;
	SET_BIT(_shm[ADDR_AN_IN_BOOTINIT], 0);
}

int AnalogInput::get_port() {
	return _port;
}

int16_t AnalogInput::get_value() {
	return MEM_VAL(int16_t, _shm, ADDR_AN_IN_VALUE);
}

int32_t AnalogInput::get_average_value() {
	return MEM_VAL(int32_t, _shm, ADDR_AN_IN_AVG_VAL);
}

float AnalogInput::get() {
	return MEM_VAL(float, _shm, ADDR_AN_IN_VOLTS);
}

float AnalogInput::get_average() {
	return MEM_VAL(float, _shm, ADDR_AN_IN_AVG_VOLTS);
}

void AnalogInput::set_average_bits(uint32_t bits) {
	MEM_VAL(uint32_t, _shm, ADDR_AN_IN_AVG_BITS) = bits;
}

uint32_t AnalogInput::get_average_bits() {
	return MEM_VAL(uint32_t, _shm, ADDR_AN_IN_AVG_BITS);
}

void AnalogInput::set_oversample_bits(uint32_t bits) {
	MEM_VAL(uint32_t, _shm, ADDR_AN_IN_OVSMPL_BITS) = bits;
}

uint32_t AnalogInput::get_oversample_bits() {
	return MEM_VAL(uint32_t, _shm, ADDR_AN_IN_OVSMPL_BITS);
}

uint32_t AnalogInput::get_lsb_weight() {
	return MEM_VAL(uint32_t, _shm, ADDR_AN_IN_LSB_WEIGHT);
}

int32_t AnalogInput::get_offset() {
	return MEM_VAL(int32_t, _shm, ADDR_AN_IN_OFFSET_VAL);
}

bool AnalogInput::is_accumulator() {
	return IS_BIT_SET(_shm[ADDR_AN_IN_ACCUM_MASK], 0);
}

void AnalogInput::init_accumulator() {
	SET_BIT(_shm[ADDR_AN_IN_ACCUM_MASK], 1);
}

void AnalogInput::set_accumulator_initial(int64_t initial) {
	MEM_VAL(int64_t, _shm, ADDR_AN_IN_ACCUM_INIT) = initial;
	SET_BIT(_shm[ADDR_AN_IN_ACCUM_MASK], 3);
}

void AnalogInput::reset_accumulator() {
	SET_BIT(_shm[ADDR_AN_IN_ACCUM_MASK], 2);
}

void AnalogInput::set_accumulator_center(int32_t center) {
	MEM_VAL(int32_t, _shm, ADDR_AN_IN_ACCUM_CENTRE) = center;
	SET_BIT(_shm[ADDR_AN_IN_ACCUM_MASK], 4);
}

void AnalogInput::set_accumulator_deadband(int32_t deadband) {
	MEM_VAL(int32_t, _shm, ADDR_AN_IN_ACCUM_DB) = deadband;
	SET_BIT(_shm[ADDR_AN_IN_ACCUM_MASK], 5);
}

int64_t AnalogInput::get_accumulator_value() {
	return MEM_VAL(int64_t, _shm, ADDR_AN_IN_ACCUM_VALUE);
}

uint32_t AnalogInput::get_accumulator_count() {
	return MEM_VAL(uint32_t, _shm, ADDR_AN_IN_ACCUM_COUNT);
}