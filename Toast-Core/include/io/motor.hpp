#pragma once

#include "toast/library.hpp"
#include "toast/memory.hpp"

#include <inttypes.h>

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

#define MOTOR_CHILD_CLASS(class_name, interf, type)    \
	class class_name : public IO::Motor {   \
	public: \
		class_name(int port) : IO::Motor(port, IO::Motor::Interface::interf, IO::Motor::Type::type) {} \
	}

namespace IO {
	API int get_motor_id(int port, Toast::Memory::Shared::IO::MotorInterface motor_interface);

	class Motor {
	public:
		typedef Toast::Memory::Shared::IO::MotorInterface Interface;
		typedef Toast::Memory::Shared::IO::MotorType Type;

		API Motor(int port, Interface motor_interface = Interface::PWM, Type type = Type::Talon);
		API virtual ~Motor() = default;

		API int get_port();
		API Interface get_interface();
		API Type get_type();

		API float get();
		API void set(float value);
		
		API bool operator==(Motor &m2) {
			return (m2.get_interface() == get_interface()) && (m2._port == _port);
		}
	private:
		int _port;
		int _internal_id;
		Toast::Memory::Shared::IO::Motor *_mem;
	};

	MOTOR_CHILD_CLASS(Talon, PWM, Talon);
	MOTOR_CHILD_CLASS(TalonSRX, PWM, TalonSRX);
	MOTOR_CHILD_CLASS(Victor, PWM, Victor);
	MOTOR_CHILD_CLASS(VictorSP, PWM, VictorSP);
	MOTOR_CHILD_CLASS(SD540, PWM, SD540);
	MOTOR_CHILD_CLASS(Spark, PWM, Spark);

	class PWM {
	public:
		API PWM(int port);
		API virtual ~PWM() = default;

		API int get_port();

		API void set_period_multiplier(int multiplier);
		API int get_period_multiplier();
		API void set_zero_latch();

		API void set_raw(unsigned short value);
		API unsigned short get_raw();

		API void enable_deadband_elimination(bool elim);
		API bool get_deadband_elimination();

		API void set_position(float pos);
		API float get_position();

		API void set_speed(float speed);
		API float get_speed();

		API void set_bounds(int32_t max, int32_t deadband_max, int32_t center, int32_t deadband_min, int32_t min);
		API void set_bounds(double max, double deadband_max, double center, double deadband_min, double min);
	private:
		int _port;
		Toast::Memory::Shared::IO::PWM *_mem;
	};

	class Servo {
	public:
		API Servo(int port);
		API virtual ~Servo() = default;

		API static float get_max_angle();
		API static float get_min_angle();
		API static float get_angle_range();

		API int get_port();

		API void set(float value);
		API float get();
		
		API void set_angle(float angle);
		API float get_angle();
		
		API void set_offline();
	private:
		int _port;
		Toast::Memory::Shared::IO::Servo *_mem;
	};
}