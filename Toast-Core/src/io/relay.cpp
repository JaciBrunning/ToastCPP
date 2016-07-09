#include "io/relay.hpp"

using namespace IO;
using namespace Toast::Memory;

char *IO::get_relay_block(int port) {
	PORT_CHECK(port, 4);
	return Shared::get() + ADDR_RELAY_OFFSET + (port * LEN_RELAY);
}

Relay::Relay(int port) : _port(port) {
	_shm = get_relay_block(port);
	SET_BIT(_shm[ADDR_RELAY_BOOTINIT], 0);
}

int Relay::get_port() {
	return _port;
}

void Relay::set_forward(bool on) {
	SET_BIT_TO(_shm[ADDR_RELAY_DIR_MASK], 0, on);
}

bool Relay::get_forward() {
	return IS_BIT_SET(_shm[ADDR_RELAY_DIR_MASK], 0);
}

void Relay::set_reverse(bool on) {
	SET_BIT_TO(_shm[ADDR_RELAY_DIR_MASK], 1, on);
}

bool Relay::get_reverse() {
	return IS_BIT_SET(_shm[ADDR_RELAY_DIR_MASK], 1);
}