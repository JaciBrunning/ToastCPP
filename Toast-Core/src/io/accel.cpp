#include "io/accel.hpp"

using namespace IO;
using namespace Toast::Memory;

char *IO::get_onboard_accel_block() {
	return Shared::get() + ADDR_OB_ACCEL_OFFSET;
}

// Built In

BuiltInAccelerometer::BuiltInAccelerometer() {
	SET_BIT(get_onboard_accel_block()[ADDR_OB_ACCEL_BOOTINIT], 0);
}

void BuiltInAccelerometer::set_range(Accelerometer::Range range) {
	char *block = get_onboard_accel_block();
	block[ADDR_OB_ACCEL_RANGE] = (char)range;
	block[ADDR_OB_RANGE_UPDATE] = 1;
}

float BuiltInAccelerometer::x() {
	return MEM_VAL(float, get_onboard_accel_block(), ADDR_OB_ACCEL_X);
}

float BuiltInAccelerometer::y() {
	return MEM_VAL(float, get_onboard_accel_block(), ADDR_OB_ACCEL_Y);
}

float BuiltInAccelerometer::z() {
	return MEM_VAL(float, get_onboard_accel_block(), ADDR_OB_ACCEL_Z);
}