#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"
#include "io/common.hpp"

enum MotorInterface {
    MotorInterface_PWM = 0x01,
    MotorInterface_CAN = 0x02
};

enum MotorType {
    MotorType_Talon = 0x01,
    MotorType_TalonSRX = 0x02,
    MotorType_CANTalon = 0x03,

    MotorType_Victor = 0x10,
    MotorType_VictorSP = 0x11,
    
    MotorType_SD540 = 0x20,
    MotorType_Spark = 0x30
};

namespace Motor {
    API char *getBlockFor(int id);
    API int init(int port, int motor_interface, int type);
    API void set(int id, float speed);
    API float get(int id);
}