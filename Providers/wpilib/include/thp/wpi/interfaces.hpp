#pragma once

void tick_itf_power();
void tick_itf_dio();
void tick_itf_relay();
//void tick_itf_analog();
void tick_itf_pneumatic();
void tick_itf_motor();
void tick_itf_driverstation();
void tick_itf_accel();

inline void tick_interfaces() {
	tick_itf_power();
	tick_itf_dio();
	tick_itf_relay();
//	tick_itf_analog();
	tick_itf_pneumatic();
	tick_itf_motor();
	tick_itf_driverstation();
	tick_itf_accel();
}

void init_itf_dio();

inline void init_interfaces() {
	init_itf_dio();
}