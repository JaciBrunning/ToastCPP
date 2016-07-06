#include "io/analog.hpp"

using namespace Analog;
using namespace Toast::Memory;

float Analog::get_sample_rate() {
	return *(float *)(Shared::get() + ADDR_AN_SS_OFFSET + ADDR_AN_SS_SAMPLE_RATE);
}

// Out

char *Out::getBlockFor(int id) {
	PORT_CHECK(id, 2);
	return Shared::get() + ADDR_AN_OUT_OFFSET + (id * LEN_AN_OUT);
}

void Out::init(int port) {
	char *b = Out::getBlockFor(port);
	b[ADDR_AN_OUT_PORT] = (char)port;
	if (b[ADDR_AN_OUT_BOOTINIT] == 0) b[ADDR_AN_OUT_BOOTINIT] = -1;
}

void Out::set(int port, float volts) {
	*(float *)(Out::getBlockFor(port) + ADDR_AN_OUT_VOLTAGE) = volts;
}

// In

char *In::getBlockFor(int id) {
	PORT_CHECK(id, 8);
	return Shared::get() + ADDR_AN_IN_OFFSET + (id * LEN_AN_IN);
}

void In::init(int port) {
	char *b = In::getBlockFor(port);
	b[ADDR_AN_IN_PORT] = (char)port;
	if (b[ADDR_AN_IN_BOOTINIT] == 0) b[ADDR_AN_IN_BOOTINIT] = -1;
}

int16_t In::get_value(int port) {
	return *(int16_t *)(In::getBlockFor(port) + ADDR_AN_IN_VALUE);
}

int32_t In::get_avg_value(int port) {
	return *(int32_t *)(In::getBlockFor(port) + ADDR_AN_IN_AVG_VAL);
}

float In::get_voltage(int port) {
	return *(float *)(In::getBlockFor(port) + ADDR_AN_IN_VOLTS);
}

float In::get_avg_voltage(int port) {
	return *(float *)(In::getBlockFor(port) + ADDR_AN_IN_AVG_VOLTS);
}

void In::set_avg_bits(int port, uint32_t bits) {
	*(uint32_t *)(In::getBlockFor(port) + ADDR_AN_IN_AVG_BITS) = bits;
}

uint32_t In::get_avg_bits(int port) {
	return *(uint32_t *)(In::getBlockFor(port) + ADDR_AN_IN_AVG_BITS);
}

void In::set_oversample_bits(int port, uint32_t bits) {
	*(uint32_t *)(In::getBlockFor(port) + ADDR_AN_IN_OVSMPL_BITS) = bits;
}

uint32_t In::get_oversample_bits(int port) {
	return *(uint32_t *)(In::getBlockFor(port) + ADDR_AN_IN_OVSMPL_BITS);
}

uint32_t In::get_lsb_weight(int port) {
	return *(uint32_t *)(In::getBlockFor(port) + ADDR_AN_IN_LSB_WEIGHT);
}

int32_t In::get_offset(int port) {
	return *(int32_t *)(In::getBlockFor(port) + ADDR_AN_IN_OFFSET_VAL);
}

bool In::is_accum(int port) {
	return In::getBlockFor(port)[ADDR_AN_IN_ACCUM_MASK] & 1 != 0;
}

void In::init_accum(int port) {
	In::getBlockFor(port)[ADDR_AN_IN_ACCUM_MASK] |= (1 << 1);
}

void In::set_accum_initial(int port, int64_t initial) {
	char *b = In::getBlockFor(port);
	*(int64_t *)(b + ADDR_AN_IN_ACCUM_INIT) = initial;
	b[ADDR_AN_IN_ACCUM_MASK] |= (1 << 3);
}

void In::reset_accum(int port) {
	In::getBlockFor(port)[ADDR_AN_IN_ACCUM_MASK] |= (1 << 2);
}

void In::set_accum_center(int port, int32_t centre) {
	char *b = In::getBlockFor(port);
	*(int32_t *)(b + ADDR_AN_IN_ACCUM_CENTRE) = centre;
	b[ADDR_AN_IN_ACCUM_MASK] |= (1 << 4);
}

void In::set_accum_deadband(int port, int32_t deadband) {
	char *b = In::getBlockFor(port);
	*(int32_t *)(b + ADDR_AN_IN_ACCUM_DB) = deadband;
	b[ADDR_AN_IN_ACCUM_MASK] |= (1 << 5);
}

int64_t In::get_accum_value(int port) {
	return *(int64_t *)(In::getBlockFor(port) + ADDR_AN_IN_ACCUM_VALUE);
}

uint32_t In::get_accum_count(int port) {
	return *(uint32_t *)(In::getBlockFor(port) + ADDR_AN_IN_ACCUM_COUNT);
}