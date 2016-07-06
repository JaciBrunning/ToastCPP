#include "io/motor.hpp"
#include <inttypes.h>

using namespace Motor;
using namespace Toast::Memory;

static int __id_counter = 0;	// Todo, this needs to be in the shared pool

char *Motor::getBlockFor(int id) {
	BLOCK_CHECK(id, 16);
    return Shared::get() + ADDR_SPD_OFFSET + (id * LEN_SPD);
}

int Motor::init(int port, int motor_interface, int type) {
    int id = __id_counter;

	// AFAIK no real limit is imposed on CAN bus IDs, so we'll just check PWM ports
	if (motor_interface == MotorInterface_PWM) {
		PORT_CHECK(port, 20);
	}

    char *block = Motor::getBlockFor(id);
    block[ADDR_SPD_ID] = (char)id;
    block[ADDR_SPD_PORT] = (char)port;
    block[ADDR_SPD_INTERFACE] = (char)motor_interface;
    block[ADDR_SPD_TYPE] = (char)type;
	if (block[ADDR_SPD_BOOTINIT] == 0) block[ADDR_SPD_BOOTINIT] = -1;
    __id_counter += 1;
    return id;
}

void Motor::set(int id, float speed) {
    *(float *)(Motor::getBlockFor(id) + ADDR_SPD_SET) = speed;
}

float Motor::get(int id) {
    return *(float *)(Motor::getBlockFor(id) + ADDR_SPD_GET);
}