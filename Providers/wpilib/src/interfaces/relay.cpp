#include "thp/wpi/interfaces.hpp"

#include "toast/memory.hpp"

#include "Relay.h"

using namespace Toast;

static Relay *relays[4];

void tick_itf_relay() {
	Concurrent::IPCMutex *mtx = Memory::shared_mutex()->relay;
	for (int i = 0; i < 4; i++) {
		MTX_LOCK(mtx, i);
		Memory::Shared::IO::Relay *relay = Memory::shared()->relay(i);
		if (relay->get_init()) {
			if (!relay->get_bootstrap()) {
				relays[i] = new Relay(i, Relay::kBothDirections);
				relay->set_bootstrap(true);
			}

			bool f = relay->get_fwd(), 
				 r = relay->get_rvs();

			if (f && r) {	// FWD + RVS
				relays[i]->Set(Relay::kOn);
			} else if (f) {	// FWD
				relays[i]->Set(Relay::kForward);
			} else if (r) {	// RVS
				relays[i]->Set(Relay::kReverse);
			} else {		// <off>
				relays[i]->Set(Relay::kOff);
			}
		}
		MTX_UNLOCK(mtx, i);
	}
}