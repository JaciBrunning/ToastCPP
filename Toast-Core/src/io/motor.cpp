#include "io/motor.hpp"

using namespace IO;
using namespace Toast::Memory;

int IO::get_motor_id(int port, Shared::IO::MotorInterface motor_interface) {
	int i;
	for (i = 0; i < 16; i++) {
		Shared::IO::Motor *_m = shared()->motor(i);
		if (_m->get_port() == port && _m->get_interface() == motor_interface) return i;
		if (!_m->get_init()) return i;
	}
	return -1;
}

// Motor

Motor::Motor(int port, Motor::Interface motor_i, Motor::Type type) : _port(port) {
	_internal_id = IO::get_motor_id(port, motor_i);
	_mem = shared()->motor(_internal_id);

	_mem->set_port(port);
	_mem->set_interface(motor_i);
	_mem->set_type(type);
	_mem->set_init(true);
}

int Motor::get_port() {
	return _port;
}

Motor::Interface Motor::get_interface() {
	return _mem->get_interface();
}

Motor::Type Motor::get_type() {
	return _mem->get_type();
}

float Motor::get() {
	return _mem->get_value();
}

void Motor::set(float value) {
	_mem->set_value(value);
}

// PWM

PWM::PWM(int port) : _port(port) {
	_mem = shared()->pwm(port);
	_mem->set_init(true);
}

int PWM::get_port() {
	return _port;
}

void PWM::set_period_multiplier(int multiplier) {
	_mem->set_period_multiplier(multiplier);
	_mem->set_period_mult_pending(true);
}

int PWM::get_period_multiplier() {
	return _mem->get_period_multiplier();
}

void PWM::set_zero_latch() {
	_mem->set_zero_latch(true);
}

void PWM::set_raw(unsigned short value) {
	_mem->set_raw(value);
	_mem->set_raw_pending(true);
}

unsigned short PWM::get_raw() {
	return _mem->get_raw();
}

void PWM::enable_deadband_elimination(bool elim) {
	_mem->set_deadband_elimination(elim);
	_mem->set_deadband_elimination_pending(true);
}

bool PWM::get_deadband_elimination() {
	return _mem->get_deadband_elimination();
}

void PWM::set_position(float pos) {
	_mem->set_position(pos);
	_mem->set_position_pending(true);
}

float PWM::get_position() {
	return _mem->get_position();
}

void PWM::set_speed(float speed) {
	_mem->set_speed(speed);
	_mem->set_speed_pending(true);
}

float PWM::get_speed() {
	return _mem->get_speed();
}

void PWM::set_bounds(int32_t max, int32_t dbmax, int32_t center, int32_t dbmin, int32_t min) {
	_mem->set_bounds_max((double)max);
	_mem->set_bounds_deadband_max((double)dbmax);
	_mem->set_bounds_center((double)center);
	_mem->set_bounds_deadband_min((double)dbmin);
	_mem->set_bounds_min((double)min);

	_mem->set_bounds_int32(true);
	_mem->set_bounds_pending(true);
}

void PWM::set_bounds(double max, double dbmax, double center, double dbmin, double min) {
	_mem->set_bounds_max(max);
	_mem->set_bounds_deadband_max(dbmax);
	_mem->set_bounds_center(center);
	_mem->set_bounds_deadband_min(dbmin);
	_mem->set_bounds_min(min);

	_mem->set_bounds_int32(false);
	_mem->set_bounds_pending(true);
}

// Servo

Servo::Servo(int port) : _port(port) {
	_mem = shared()->servo(port);
	_mem->set_init(true);
}

float Servo::get_max_angle() {
	return shared()->servo_static()->get_max_angle();
}

float Servo::get_min_angle() {
	return shared()->servo_static()->get_min_angle();
}

float Servo::get_angle_range() {
	return shared()->servo_static()->get_angle_range();
}

int Servo::get_port() {
	return _port;
}

void Servo::set(float value) {
	_mem->set_value(value);
	_mem->set_update_pending(true);
}

float Servo::get() {
	return _mem->get_value();
}

void Servo::set_angle(float angle) {
	if (angle < get_min_angle()) {
		angle = get_min_angle();
	} else if (angle > get_max_angle()) {
		angle = get_max_angle();
	}

	set(((float)(angle - get_min_angle())) / (get_max_angle() - get_min_angle()));
}

float Servo::get_angle() {
	return (float)get() * (get_max_angle() - get_min_angle()) + get_min_angle();
}

void Servo::set_offline() {
	_mem->set_offline(true);
	_mem->set_offline_pending(true);
}