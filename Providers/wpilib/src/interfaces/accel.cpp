#include "thp/wpi/interfaces.hpp"

#include "toast/memory.hpp"

#include "BuiltInAccelerometer.h"

using namespace Toast;

static BuiltInAccelerometer *accel;

void tick_itf_accel() {
	Concurrent::IPCMutex *mtx = Memory::shared_mutex()->onboard_accel;
	Memory::Shared::IO::OnboardAccel *mem = Memory::shared()->onboard_accel();
	MTX_LOCK(mtx, 0);
	if (mem->get_init()) {
		if (!mem->get_bootstrap()) {
			accel = new BuiltInAccelerometer();
			mem->set_bootstrap(true);
		}
		if (mem->get_range_update()) {
			Memory::Shared::IO::AccelRange _range = mem->get_range();
			accel->SetRange((Accelerometer::Range)(int)_range);
			mem->set_range_update(false);
		}
		mem->set_x(accel->GetX());
		mem->set_y(accel->GetY());
		mem->set_z(accel->GetZ());
	}
	MTX_UNLOCK(mtx, 0);
}