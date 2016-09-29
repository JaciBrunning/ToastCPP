#include "thp/wpi/interfaces.hpp"

#include "toast/memory.hpp"

#include "DigitalInput.h"
#include "DigitalOutput.h"

using namespace Toast;

static struct InternalDIO {
	Memory::Shared::IO::DIO_Mode mode;
	DigitalInput *input;
	DigitalOutput *output;
};

InternalDIO dios[26];

void tick_itf_dio() {
	Concurrent::IPCMutex *mtx = Memory::shared_mutex()->dio;
	for (int i = 0; i < 26; i++) {
		MTX_LOCK(mtx, i);
		Memory::Shared::IO::DIO *dio = Memory::shared()->dio(i);
		if (dio->get_init()) {
			Memory::Shared::IO::DIO_Mode mode = dio->get_mode();
			if (!dio->get_bootstrap()) {
				dios[i].mode = mode;
				if (mode == Memory::Shared::IO::DIO_Mode::INPUT) {
					dios[i].input = new DigitalInput(i);
				} else {
					dios[i].output = new DigitalOutput(i);
				}
			}

			if (mode == Memory::Shared::IO::DIO_Mode::INPUT) {
				DigitalInput *input = dios[i].input;
				dio->set_value(input->Get());
			} else {
				DigitalOutput *output = dios[i].output;
				output->Set(dio->get_value());
				if (dio->get_pwm_enabled_pending()) {
					if (dio->get_pwm_enabled()) 
						output->EnablePWM(dio->get_pwm_duty_cycle());
					else output->DisablePWM();
					dio->set_pwm_enabled_pending(false);
				}

				if (dio->get_pwm_enabled()) {
					if (dio->get_pwm_rate_pending()) {
						output->SetPWMRate(dio->get_pwm_rate());
						dio->set_pwm_rate_pending(false);
					}

					if (dio->get_pwm_duty_cycle_pending()) {
						output->UpdateDutyCycle(dio->get_pwm_duty_cycle());
						dio->set_pwm_rate_pending(false);
					}
				}

				if (dio->get_pulse_pending()) {
					output->Pulse(dio->get_pulse_length());
					dio->set_pulse_pending(false);
				}

				dio->set_is_pulsing(output->IsPulsing());
			}
		}
		MTX_UNLOCK(mtx, i);
	}
}