#pragma once

void tick_itf_power();
void tick_itf_dio();
void tick_itf_relay();
//void tick_itf_analog();
//void tick_itf_pneumatic();
//void tick_itf_motor();
//void tick_itf_driverstation();

inline void tick_interfaces() {
	tick_itf_power();
	tick_itf_dio();
	tick_itf_relay();
//	tick_itf_analog();
//	tick_itf_pneumatic();
//	tick_itf_motor();
//	tick_itf_driverstation();
}