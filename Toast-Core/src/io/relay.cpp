#include "io/relay.hpp"

using namespace Toast::Memory;

char *Relay::getBlockFor(int id) {
	PORT_CHECK(id, 4);
	return Shared::get() + ADDR_RELAY_OFFSET + (id * LEN_RELAY);
}

void Relay::init(int port) {
	char *block = Relay::getBlockFor(port);
	block[ADDR_RELAY_PORT] = (char)port;
	block[ADDR_RELAY_BOOTINIT] = -1;
}

void Relay::set_forward(int port, bool on) {
	char *b = Relay::getBlockFor(port);
	b[ADDR_RELAY_DIR_MASK] ^= (-(on ? 1 : 0) ^ b[ADDR_RELAY_DIR_MASK]) & (1 << 0);
}

bool Relay::get_forward(int port) {
	return Relay::getBlockFor(port)[ADDR_RELAY_DIR_MASK] & (1 << 0) != 0;
}

void Relay::set_reverse(int port, bool on) {
	char *b = Relay::getBlockFor(port);
	b[ADDR_RELAY_DIR_MASK] ^= (-(on ? 1 : 0) ^ b[ADDR_RELAY_DIR_MASK]) & (1 << 1);
}

bool Relay::get_reverse(int port) {
	return Relay::getBlockFor(port)[ADDR_RELAY_DIR_MASK] & (1 << 1) != 0;
}