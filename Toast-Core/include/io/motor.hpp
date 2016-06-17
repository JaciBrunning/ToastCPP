#include "toast/library.hpp"

typedef enum {
    thpMotorInterface_PWM = 0x01,
    thpMotorInterface_CAN = 0x02
} thpMotorInterface;

typedef enum {
    thpMotorType_Talon = 0x01,
    thpMotorType_TalonSRX = 0x02,
    thpMotorType_CANTalon = 0x03,

    thpMotorType_Victor = 0x10,
    thpMotorType_VictorSP = 0x11,
    
    thpMotorType_SD540 = 0x20,
    thpMotorType_Spark = 0x30
} thpMotorType;

namespace Motor {
    API void set(int port, int interface, int type, float speed);
    API float get(int port, int interface);
}