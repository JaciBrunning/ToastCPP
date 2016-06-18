#include "io/motor.hpp"
#include <inttypes.h>

#include "toast/logger.hpp"

using namespace Motor;
using namespace Toast::Memory;

static int __id_counter = 0;

char *Motor::getBlockFor(int id) {
    return Shared::get() + ADDR_SPD_OFFSET + (id * LEN_SPD);
}

int Motor::init(int port, int motor_interface, int type) {
    int id = __id_counter;
    char *block = Motor::getBlockFor(id);
    block[ADDR_SPD_ID] = (char)id;
    block[ADDR_SPD_PORT] = (char)port;
    block[ADDR_SPD_INTERFACE] = (char)motor_interface;
    block[ADDR_SPD_TYPE] = (char)type;
    __id_counter += 1;
    return id;
}

void Motor::set(int id, float speed) {
    *(float *)(Motor::getBlockFor(id) + ADDR_SPD_SET) = speed;
}

float Motor::get(int id) {
    return *(float *)(Motor::getBlockFor(id) + ADDR_SPD_GET);
}