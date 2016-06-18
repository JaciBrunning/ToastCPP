#include "toast.hpp"

#include "io/motor.hpp"

#include <iostream>

using namespace Toast;
using namespace std;

class MyModule : public Module {
    public:
        virtual void construct() {
            Motor::init(10, MotorInterface_PWM, MotorType_Talon);
            Motor::init(12, MotorInterface_CAN, MotorType_CANTalon);

            Motor::set(1, 0.14);

            ofstream outfile("shared.txt", ios::binary);
            outfile.write(Memory::Shared::get(), TOAST_SHARED_MEMPOOL_SIZE);
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);