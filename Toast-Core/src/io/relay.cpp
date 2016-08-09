#include "io/relay.hpp"

using namespace IO;
using namespace Toast::Memory;

Relay::Relay(int port) : _port(port) {
	_mem = shared()->relay(port);
	_mem->set_init(true);
}

int Relay::get_port() {
	return _port;
}

void Relay::set_forward(bool on) {
	_mem->set_fwd(on);
}

bool Relay::get_forward() {
	return _mem->get_fwd();
}

void Relay::set_reverse(bool on) {
	_mem->set_rvs(on);
}

bool Relay::get_reverse() {
	return _mem->get_rvs();
}