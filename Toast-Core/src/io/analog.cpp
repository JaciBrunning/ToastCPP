#include "io/analog.hpp"

using namespace Analog;
using namespace Toast::Memory;

float Analog::get_sample_rate() {
	return *(float *)(Shared::get() + ADDR_AN_SS_OFFSET + ADDR_AN_SS_SAMPLE_RATE);
}

// Out

char *Out::getBlockFor(int id) {
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
	char *b = In::getBlockFor(port);
}