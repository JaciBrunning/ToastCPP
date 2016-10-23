#include "thp/wpi/interfaces.hpp"

#include "toast/memory.hpp"

#include "SpeedController.h"
#include "SD540.h"
#include "Spark.h"
#include "Talon.h"
#include "Victor.h"
#include "VictorSP.h"
#include "TalonSRX.h"
#include "Servo.h"
#include "PWM.h"

using namespace Toast;

typedef Memory::Shared::IO::MotorType MType;

static SpeedController *motors[16];
static Servo *servos[20];
static PWM *pwms[20];

void init_itf_motor() {
	Memory::Shared::IO::ServoStatic *stat = Memory::shared()->servo_static();
	stat->set_angle_range(Servo::GetMaxAngle() - Servo::GetMinAngle());
	stat->set_max_angle(Servo::GetMaxAngle());
	stat->set_min_angle(Servo::GetMinAngle());
}

void tick_itf_motor() {
	Concurrent::IPCMutex *mtx = Memory::shared_mutex()->motor;
	for (int i = 0; i < 16; i++) {
		MTX_LOCK(mtx, i);
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
		MTX_UNLOCK(mtx, i);
	}

	mtx = Memory::shared_mutex()->servo;
	for (int i = 0; i < 20; i++) {
		MTX_LOCK(mtx, i);
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
		MTX_UNLOCK(mtx, i);
	}

	mtx = Memory::shared_mutex()->pwm;
	for (int i = 0; i < 20; i++) {
		MTX_LOCK(mtx, i);
		Memory::Shared::IO::PWM *pwm = Memory::shared()->pwm;
		if (pwm->get_init()) {
			if (!pwm->get_bootstrap) {
				pwms[i] = new PWM(i);
				pwm->set_bootstrap(true);
			}

			PWM *p = pwms[i];
			if (pwm->get_period_mult_pending()) {
				p->SetPeriodMultiplier((PWM::PeriodMultiplier)(int)pwm->get_period_multiplier());
				pwm->set_period_mult_pending(false);
			}
			if (pwm->get_raw_pending()) {
				p->SetRaw(pwm->get_raw());
				pwm->set_raw_pending(false);
			}
			if (pwm->get_position_pending()) {
				p->SetPosition(pwm->get_position());
				pwm->set_position_pending(false);
			}
			if (pwm->get_speed_pending()) {
				p->SetSpeed(pwm->get_speed());
				pwm->set_speed_pending(false);
			}
			if (pwm->get_deadband_elimination_pending()) {
				p->EnableDeadbandElimination(pwm->get_deadband_elimination());
				pwm->set_deadband_elimination_pending(false);
			}
			if (pwm->get_bounds_pending()) {
				if (pwm->get_bounds_int32()) {
					p->SetRawBounds(
						(int)pwm->get_bounds_max(),
						(int)pwm->get_bounds_deadband_max(),
						(int)pwm->get_bounds_center(),
						(int)pwm->get_bounds_deadband_min(),
						(int)pwm->get_bounds_min()
					);
				} else {
					p->SetBounds(
						pwm->get_bounds_max(),
						pwm->get_bounds_deadband_max(),
						pwm->get_bounds_center(),
						pwm->get_bounds_deadband_min(),
						pwm->get_bounds_min()
					);
				}
				pwm->set_bounds_pending(false);
			}
			if (pwm->get_zero_latch()) {
				p->SetZeroLatch();
				pwm->set_zero_latch(false);
			}
			// This is just used so the module process can read, i.e. if GetRaw is called, GetSpeed is indirectly
			// changed.
			pwm->set_raw(p->GetRaw());
			pwm->set_position(p->GetPosition());
			pwm->set_speed(p->GetSpeed());
		}
		MTX_UNLOCK(mtx, i);
	}
}