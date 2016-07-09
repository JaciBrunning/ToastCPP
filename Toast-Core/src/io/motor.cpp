#include "io/motor.hpp"

using namespace IO;
using namespace Toast::Memory;

int IO::get_motor_shm_id(int port, int motor_interface) {
	int i;
	for (i = 0; i < 16; i++) {
		char *block = get_motor_block(i);
		int bl_port = (int)(block[ADDR_SPD_PORT]);
		if (bl_port == port) return i;
		if (!IS_BIT_SET(block[ADDR_SPD_BOOTINIT], 0)) return i;
	}
	throw IO::CouldNotAllocateBlockException("Could not allocate block for Motor (have you registered more than 16 Motors?)");
	return -1;
}

char *IO::get_motor_block(int id) {
	BLOCK_CHECK(id, 16)
	return Shared::get() + ADDR_SPD_OFFSET + (id * LEN_SPD);
}

char *IO::get_pwm_block(int port) {
	PORT_CHECK(port, 20);
	return Shared::get() + ADDR_PWM_OFFSET + (port * LEN_PWM);
}

char *IO::get_servo_block() {
	return Shared::get() + ADDR_SERVO_OFFSET;
}

// Motor

Motor::Motor(int port, Motor::Interface motor_i, Motor::Type type) : _port(port), _interface((int)motor_i) {
	_shm_id = get_motor_shm_id(port, motor_i);
	_shm = get_motor_block(_shm_id);
	_shm[ADDR_SPD_PORT] = port;
	_shm[ADDR_SPD_INTERFACE] = (int)motor_i;
	_shm[ADDR_SPD_ID] = _shm_id;
	SET_BIT(_shm[ADDR_SPD_BOOTINIT], 0);
}

int Motor::get_shm_id() {
	return _shm_id;
}

int Motor::get_port() {
	return _port;
}

Motor::Interface Motor::get_interface() {
	return (Motor::Interface)(_shm[ADDR_SPD_INTERFACE]);
}

Motor::Type Motor::get_type() {
	return (Motor::Type)(_shm[ADDR_SPD_TYPE]);
}

float Motor::get() {
	return MEM_VAL(float, _shm, ADDR_SPD_GET);
}

void Motor::set(float value) {
	MEM_VAL(float, _shm, ADDR_SPD_SET) = value;
}

// PWM

PWM::PWM(int port) : _port(port) {
	_shm = get_pwm_block(port);
	_shm[ADDR_PWM_PORT] = port;
	SET_BIT(_shm[ADDR_PWM_BOOTINIT], 0);
}

int PWM::get_port() {
	return _port;
}

void PWM::set_period_multiplier(int multiplier) {
	_shm[ADDR_PWM_PERIOD_MULT] = (char)multiplier;
	SET_BIT(_shm[ADDR_PWM_CONTROL_FLAGS], 7);
}

int PWM::get_period_multiplier() {
	return (int)_shm[ADDR_PWM_PERIOD_MULT];
}

void PWM::set_zero_latch() {
	SET_BIT(_shm[ADDR_PWM_CONTROL_FLAGS], 0);
}

void PWM::set_raw(unsigned short value) {
	uint16_t val = (uint16_t)value;
	MEM_VAL(uint16_t, _shm, ADDR_PWM_RAW_SET) = val;
	SET_BIT(_shm[ADDR_PWM_CONTROL_FLAGS], 4);
}

unsigned short PWM::get_raw() {
	return (unsigned short) MEM_VAL(uint16_t, _shm, ADDR_PWM_RAW_GET);
}

void PWM::enable_deadband_elimination(bool elim) {
	SET_BIT_TO(_shm[ADDR_PWM_CONTROL_FLAGS], 1, elim);
	SET_BIT(_shm[ADDR_PWM_CONTROL_FLAGS], 2);
}

bool PWM::get_deadband_elimination() {
	return IS_BIT_SET(_shm[ADDR_PWM_CONTROL_FLAGS], 1);
}

void PWM::set_position(float pos) {
	MEM_VAL(float, _shm, ADDR_PWM_POS_SET) = pos;
	SET_BIT(_shm[ADDR_PWM_CONTROL_FLAGS], 5);
}

float PWM::get_position() {
	return MEM_VAL(float, _shm, ADDR_PWM_POS_GET);
}

void PWM::set_speed(float speed) {
	MEM_VAL(float, _shm, ADDR_PWM_POS_SET) = speed;
	SET_BIT(_shm[ADDR_PWM_CONTROL_FLAGS], 6);
}

float PWM::get_speed() {
	return MEM_VAL(float, _shm, ADDR_PWM_POS_GET);
}

void PWM::set_bounds(int32_t max, int32_t dbmax, int32_t center, int32_t dbmin, int32_t min) {
	MEM_VAL(int32_t, _shm, ADDR_PWM_BOUNDS_MAX) = max;
	MEM_VAL(int32_t, _shm, ADDR_PWM_BOUNDS_DB_MAX) = dbmax;
	MEM_VAL(int32_t, _shm, ADDR_PWM_BOUNDS_CENTRE) = center;
	MEM_VAL(int32_t, _shm, ADDR_PWM_BOUNDS_DB_MIN) = dbmin;
	MEM_VAL(int32_t, _shm, ADDR_PWM_BOUNDS_MIN) = min;
	_shm[ADDR_PWM_BOUNDS_MODE] = 0;
	SET_BIT(_shm[ADDR_PWM_CONTROL_FLAGS], 3);
}

void PWM::set_bounds(double max, double dbmax, double center, double dbmin, double min) {
	MEM_VAL(float, _shm, ADDR_PWM_BOUNDS_MAX) = (float)max;
	MEM_VAL(float, _shm, ADDR_PWM_BOUNDS_DB_MAX) = (float)dbmax;
	MEM_VAL(float, _shm, ADDR_PWM_BOUNDS_CENTRE) = (float)center;
	MEM_VAL(float, _shm, ADDR_PWM_BOUNDS_DB_MIN) = (float)dbmin;
	MEM_VAL(float, _shm, ADDR_PWM_BOUNDS_MIN) = (float)min;
	_shm[ADDR_PWM_BOUNDS_MODE] = 1;
	SET_BIT(_shm[ADDR_PWM_CONTROL_FLAGS], 3);
}

// Servo

Servo::Servo(int port) : _port(port) {
	PORT_CHECK(port, 20);
	SET_BIT_MULTIWIDE(uint64_t, get_servo_block(), ADDR_SERVO_BOOTINIT_MASK, port * 2);
}

float Servo::get_max_angle() {
	return MEM_VAL(float, get_servo_block(), ADDR_SERVO_MAX_ANGLE);
}

float Servo::get_min_angle() {
	return MEM_VAL(float, get_servo_block(), ADDR_SERVO_MIN_ANGLE);
}

float Servo::get_angle_range() {
	return MEM_VAL(float, get_servo_block(), ADDR_SERVO_ANGLE_RANGE);
}

int Servo::get_port() {
	return _port;
}

void Servo::set(float value) {
	MEM_VAL(float, get_servo_block(), ADDR_SERVO_VALUE_OFFSET + (_port * 4)) = value;
	SET_BIT_MULTIWIDE(uint32_t, get_servo_block(), ADDR_SERVO_UPDATE_PEND_MASK, _port);
}

float Servo::get() {
	return MEM_VAL(float, get_servo_block(), ADDR_SERVO_VALUE_OFFSET + (_port * 4));
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
	SET_BIT_MULTIWIDE(uint32_t, get_servo_block(), ADDR_SERVO_OFFLINE_PEND_MASK, _port);
}