#include "thp/wpi/interfaces.hpp"

#include "toast/memory.hpp"

#include "SpeedController.h"
#include "SD540.h"
#include "Spark.h"
#include "Talon.h"
#include "Victor.h"
#include "VictorSP.h"
#include "CANTalon.h"
#include "TalonSRX.h"
#include "Servo.h"

using namespace Toast;

typedef Memory::Shared::IO::MotorType MType;

static SpeedController *motors[16];
static Servo *servos[20];

// TODO Initial Servo Statics
// TODO PWM, SRX

void tick_itf_motor() {
	Concurrent::IPCMutex *mtx_motor = Memory::shared_mutex()->motor;
	for (int i = 0; i < 16; i++) {
		MTX_LOCK(mtx_motor, i);
		Memory::Shared::IO::Motor *motor = Memory::shared()->motor(i);
		if (motor->get_init()) {
			Memory::Shared::IO::MotorType type = motor->get_type();
			Memory::Shared::IO::MotorInterface intf = motor->get_interface();
			int port = motor->get_port();
			if (!motor->get_bootstrap()) {
				SpeedController *controller = nullptr;
				if (type == MType::SD540)
					controller = (SpeedController *)(new SD540(port));
				else if (type == MType::Spark)
					controller = (SpeedController *)(new Spark(port));
				else if (type == MType::Talon)
					controller = (SpeedController *)(new Talon(port));
				else if (type == MType::Victor)
					controller = (SpeedController *)(new Victor(port));
				else if (type == MType::VictorSP)
					controller = (SpeedController *)(new VictorSP(port));
				else if (type == MType::TalonSRX)
					controller = (SpeedController *)(new TalonSRX(port));

				motors[i] = controller;
				motor->set_bootstrap(true);
			}

			SpeedController *m = motors[i];
			m->Set(motor->get_value());
		}
		MTX_UNLOCK(mtx_motor, i);
	}

	Concurrent::IPCMutex *mtx_servo = Memory::shared_mutex()->servo;
	for (int i = 0; i < 20; i++) {
		MTX_LOCK(mtx_servo, i);
		Memory::Shared::IO::Servo *servo = Memory::shared()->servo(i);
		if (servo->get_init()) {
			if (!servo->get_bootstrap()) {
				servos[i] = new Servo(i);
				servo->set_bootstrap(true);
			}

			Servo *srv = servos[i];
			if (servo->get_offline_pending()) {
				if (servo->get_offline()) srv->SetOffline();
				servo->set_offline_pending(false);
			}

			if (servo->get_update_pending()) {
				srv->Set(servo->get_value());
				servo->set_update_pending(false);
			}
		}
		MTX_UNLOCK(mtx_servo, i);
	}
}