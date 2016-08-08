#include "io/accel.hpp"

using namespace IO;
using namespace Toast::Memory;

static inline Shared::IO::OnboardAccel *_onboard_accel() {
	return shared()->onboard_accel();
}

// Built In

BuiltInAccelerometer::BuiltInAccelerometer() {
	_onboard_accel()->set_init(true);
}

void BuiltInAccelerometer::set_range(Accelerometer::Range range) {
	_onboard_accel()->set_range(range);
	_onboard_accel()->set_range_update(true);
}

float BuiltInAccelerometer::x() {
	return _onboard_accel()->get_x();
}

float BuiltInAccelerometer::y() {
	return _onboard_accel()->get_y();
}

float BuiltInAccelerometer::z() {
	return _onboard_accel()->get_z();
}