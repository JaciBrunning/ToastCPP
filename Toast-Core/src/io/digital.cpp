#include "io/digital.hpp"

using namespace Toast::Memory;

char *DIO::getBlockFor(int id) {
	return Shared::get() + ADDR_DIO_OFFSET + (id * LEN_DIO);
}

int DIO::init(int port, int mode) {
	char *block = DIO::getBlockFor(port);
	block[ADDR_DIO_PORT] = (char)port;
	block[ADDR_DIO_MODE] = (char)mode;
}

bool DIO::get(int port) {
	return DIO::getBlockFor(port)[ADDR_DIO_VALUE] == 1;
}

void DIO::set(int port, bool state) {
	DIO::getBlockFor(port)[ADDR_DIO_VALUE] = (state ? 1 : 0);
}

void DIO::set_pulse(int port, float len) {
	char *bl = DIO::getBlockFor(port);
	*(float *)(bl + ADDR_DIO_PULSE_LENGTH) = len;
	bl[ADDR_DIO_PWM_PULSE_ENABLE] |= (1 << 3);
}

bool DIO::is_pulsing(int port) {
	return DIO::getBlockFor(port)[ADDR_DIO_PWM_PULSE_ENABLE] & (1 << 2) != 0;
}

void DIO::set_pwm_rate(int port, float rate) {
	char *bl = DIO::getBlockFor(port);
	*(float *)(bl + ADDR_DIO_PWM_RATE) = rate;
	bl[ADDR_DIO_PWM_PULSE_ENABLE] |= (1 << 4);
}

void DIO::set_pwm_enable(int port, float dc) {
	char *bl = DIO::getBlockFor(port);
	*(float *)(ADDR_DIO_PWM_DUTY_CYCLE) = dc;
	bl[ADDR_DIO_PWM_PULSE_ENABLE] |= (1 << 0);
	bl[ADDR_DIO_PWM_PULSE_ENABLE] |= (1 << 1);
}

void DIO::set_pwm_disable(int port) {
	char *bl = DIO::getBlockFor(port);
	bl[ADDR_DIO_PWM_PULSE_ENABLE] |= (1 << 0);
	bl[ADDR_DIO_PWM_PULSE_ENABLE] |= (1 << 1);
}

void DIO::set_pwm_duty_cycle(int port, float dc) {
	char *bl = DIO::getBlockFor(port);
	*(float *)(ADDR_DIO_PWM_DUTY_CYCLE) = dc;
	bl[ADDR_DIO_PWM_PULSE_ENABLE] |= (1 << 5);
}