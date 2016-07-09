#include "io/digital.hpp"

using namespace IO;
using namespace Toast::Memory;

char *IO::get_dio_block(int port) {
	PORT_CHECK(port, 26);
	return Shared::get() + ADDR_DIO_OFFSET + (port * LEN_DIO);
}

DIO::DIO(int port, DIO::Mode mode) : _port(port) {
	_shm = get_dio_block(port);

	_shm[ADDR_DIO_MODE] = (char)mode;
	SET_BIT(_shm[ADDR_DIO_BOOTINIT], 0);
}

int DIO::get_port() {
	return _port;
}

DIO::Mode DIO::get_mode() {
	return (DIO::Mode)((int)_shm[ADDR_DIO_MODE]);
}

bool DIO::get() {
	return bool(_shm[ADDR_DIO_VALUE]);
}

void DIO::set(bool state) {
	_shm[ADDR_DIO_VALUE] = (char)state;
}

void DIO::set_pulse(float length) {
	MEM_VAL(float, _shm, ADDR_DIO_PULSE_LENGTH) = length;
	SET_BIT(_shm[ADDR_DIO_PWM_PULSE_ENABLE], 3);
}

bool DIO::is_pulsing() {
	return IS_BIT_SET(_shm[ADDR_DIO_PWM_PULSE_ENABLE], 2);
}

void DIO::set_pwm_rate(float rate) {
	MEM_VAL(float, _shm, ADDR_DIO_PWM_RATE) = rate;
	SET_BIT(_shm[ADDR_DIO_PWM_PULSE_ENABLE], 4);
}

void DIO::set_pwm_enable(float initial_duty_cycle) {
	MEM_VAL(float, _shm, ADDR_DIO_PWM_DUTY_CYCLE) = initial_duty_cycle;
	SET_BIT(_shm[ADDR_DIO_PWM_PULSE_ENABLE], 0);
	SET_BIT(_shm[ADDR_DIO_PWM_PULSE_ENABLE], 1);
}

void DIO::set_pwm_disable() {
	UNSET_BIT(_shm[ADDR_DIO_PWM_PULSE_ENABLE], 0);
	SET_BIT(_shm[ADDR_DIO_PWM_PULSE_ENABLE], 1);
}

void DIO::set_pwm_duty_cycle(float duty_cycle) {
	MEM_VAL(float, _shm, ADDR_DIO_PWM_DUTY_CYCLE) = duty_cycle;
	SET_BIT(_shm[ADDR_DIO_PWM_PULSE_ENABLE], 5);
}