#include "io/relay.hpp"

using namespace IO;
using namespace Toast::Memory;

static inline Toast::Concurrent::IPCMutex *mtx() {
	return shared_mutex()->relay;
}

Relay::Relay(int port) : _port(port) {
	_mem = shared()->relay(port);
	MTX_WRAP(mtx(), _port, _mem->set_init(true));
}

int Relay::get_port() {
	return _port;
}

void Relay::set_forward(bool on) {
	MTX_WRAP(mtx(), _port, _mem->set_fwd(on));
}

bool Relay::get_forward() {
	MTX_RETURN(mtx(), _port, _mem->get_fwd());
}

void Relay::set_reverse(bool on) {
	MTX_WRAP(mtx(), _port, _mem->set_rvs(on));
}

bool Relay::get_reverse() {
	MTX_RETURN(mtx(), _port, _mem->get_rvs());
}