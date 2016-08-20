#include "io/accel.hpp"

using namespace IO;
using namespace Toast::Memory;

static inline Shared::IO::OnboardAccel *_onboard_accel() {
	return shared()->onboard_accel();
}

static inline Toast::Concurrent::IPCMutex *mtx() {
	return shared_mutex()->onboard_accel;
}

// Built In

BuiltInAccelerometer::BuiltInAccelerometer() {
	MTX_WRAP(mtx(), 0, _onboard_accel()->set_init(true));
}

void BuiltInAccelerometer::set_range(Accelerometer::Range range) {
	MTX_LOCK(mtx(), 0);
	_onboard_accel()->set_range(range);
	_onboard_accel()->set_range_update(true);
	MTX_UNLOCK(mtx(), 0);
}

float BuiltInAccelerometer::x() {
	MTX_RETURN(mtx(), 0, _onboard_accel()->get_x());
}

float BuiltInAccelerometer::y() {
	MTX_RETURN(mtx(), 0, _onboard_accel()->get_y());
}

float BuiltInAccelerometer::z() {
	MTX_RETURN(mtx(), 0, _onboard_accel()->get_z());
}