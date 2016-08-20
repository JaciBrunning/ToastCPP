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

static inline Toast::Concurrent::IPCMutex *mtx_m() {
	return shared_mutex()->motor;
}

static inline Toast::Concurrent::IPCMutex *mtx_p() {
	return shared_mutex()->pwm;
}

static inline Toast::Concurrent::IPCMutex *mtx_s() {
	return shared_mutex()->servo;
}

// Motor

Motor::Motor(int port, Motor::Interface motor_i, Motor::Type type) : _port(port) {
	_internal_id = IO::get_motor_id(port, motor_i);
	_mem = shared()->motor(_internal_id);

	MTX_LOCK(mtx_m(), _internal_id);
	_mem->set_port(port);
	_mem->set_interface(motor_i);
	_mem->set_type(type);
	_mem->set_init(true);
	MTX_UNLOCK(mtx_m(), _internal_id);
}

int Motor::get_port() {
	return _port;
}

Motor::Interface Motor::get_interface() {
	MTX_RETURN(mtx_m(), _internal_id, _mem->get_interface());
}

Motor::Type Motor::get_type() {
	MTX_RETURN(mtx_m(), _internal_id, _mem->get_type());
}

float Motor::get() {
	MTX_RETURN(mtx_m(), _internal_id, _mem->get_value());
}

void Motor::set(float value) {
	MTX_WRAP(mtx_m(), _internal_id, _mem->set_value(value));
}

// PWM

PWM::PWM(int port) : _port(port) {
	_mem = shared()->pwm(port);
	MTX_WRAP(mtx_p(), _port, _mem->set_init(true));
}

int PWM::get_port() {
	return _port;
}

void PWM::set_period_multiplier(int multiplier) {
	MTX_LOCK(mtx_p(), _port);
	_mem->set_period_multiplier(multiplier);
	_mem->set_period_mult_pending(true);
	MTX_UNLOCK(mtx_p(), _port);
}

int PWM::get_period_multiplier() {
	MTX_RETURN(mtx_p(), _port, _mem->get_period_multiplier());
}

void PWM::set_zero_latch() {
	MTX_WRAP(mtx_p(), _port, _mem->set_zero_latch(true));
}

void PWM::set_raw(unsigned short value) {
	MTX_LOCK(mtx_p(), _port);
	_mem->set_raw(value);
	_mem->set_raw_pending(true);
	MTX_UNLOCK(mtx_p(), _port);
}

unsigned short PWM::get_raw() {
	MTX_RETURN(mtx_p(), _port, _mem->get_raw());
}

void PWM::enable_deadband_elimination(bool elim) {
	MTX_LOCK(mtx_p(), _port);
	_mem->set_deadband_elimination(elim);
	_mem->set_deadband_elimination_pending(true);
	MTX_UNLOCK(mtx_p(), _port);
}

bool PWM::get_deadband_elimination() {
	MTX_RETURN(mtx_p(), _port, _mem->get_deadband_elimination());
}

void PWM::set_position(float pos) {
	MTX_LOCK(mtx_p(), _port);
	_mem->set_position(pos);
	_mem->set_position_pending(true);
	MTX_UNLOCK(mtx_p(), _port);
}

float PWM::get_position() {
	MTX_RETURN(mtx_p(), _port, _mem->get_position());
}

void PWM::set_speed(float speed) {
	MTX_LOCK(mtx_p(), _port);
	_mem->set_speed(speed);
	_mem->set_speed_pending(true);
	MTX_UNLOCK(mtx_p(), _port);
}

float PWM::get_speed() {
	MTX_RETURN(mtx_p(), _port, _mem->get_speed());
}

void PWM::set_bounds(int32_t max, int32_t dbmax, int32_t center, int32_t dbmin, int32_t min) {
	MTX_LOCK(mtx_p(), _port);
	_mem->set_bounds_max((double)max);
	_mem->set_bounds_deadband_max((double)dbmax);
	_mem->set_bounds_center((double)center);
	_mem->set_bounds_deadband_min((double)dbmin);
	_mem->set_bounds_min((double)min);

	_mem->set_bounds_int32(true);
	_mem->set_bounds_pending(true);
	MTX_UNLOCK(mtx_p(), _port);
}

void PWM::set_bounds(double max, double dbmax, double center, double dbmin, double min) {
	MTX_LOCK(mtx_p(), _port);
	_mem->set_bounds_max(max);
	_mem->set_bounds_deadband_max(dbmax);
	_mem->set_bounds_center(center);
	_mem->set_bounds_deadband_min(dbmin);
	_mem->set_bounds_min(min);

	_mem->set_bounds_int32(false);
	_mem->set_bounds_pending(true);
	MTX_UNLOCK(mtx_p(), _port);
}

// Servo

Servo::Servo(int port) : _port(port) {
	_mem = shared()->servo(port);
	MTX_WRAP(mtx_s(), _port, _mem->set_init(true));
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
	MTX_LOCK(mtx_s(), _port);
	_mem->set_value(value);
	_mem->set_update_pending(true);
	MTX_UNLOCK(mtx_s(), _port);
}

float Servo::get() {
	MTX_RETURN(mtx_s(), _port, _mem->get_value());
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
	MTX_LOCK(mtx_s(), _port);
	_mem->set_offline(true);
	_mem->set_offline_pending(true);
	MTX_UNLOCK(mtx_s(), _port);
}