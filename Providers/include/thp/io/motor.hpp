#pragma once

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
    
    thpMotorType_Jaguar = 0x20,
    thpMotorType_SD540 = 0x30,
    thpMotorType_Spark = 0x40
} thpMotorType;

void thp_motor_maybe_init(int port, int interface, int type);
void thp_motor_set(int port, int interface, float speed);
float thp_motor_get(int port, int interface);