#include "thp/wpi/interfaces.hpp"

#include "toast/memory.hpp"
#include "toast/ipc.hpp"
#include "toast/concurrent/mutex.hpp"
#include "io/digital.hpp"

#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "DigitalGlitchFilter.h"

using namespace Toast;

struct WPIInternalDIO {
	Memory::Shared::IO::DIO_Mode mode;
	int port;
	DigitalInput *input;
	DigitalOutput *output;
	DigitalSource *source;
	DigitalGlitchFilter *last_filter;
};

WPIInternalDIO dios[26];
static DigitalGlitchFilter *filters[26];	// One per port if needed
static Concurrent::Mutex tick_mutex;

static void _interrupt_callback(uint32_t interruptAssertedMask, void *param) {
	WPIInternalDIO *dio = ((WPIInternalDIO *)param);
	bool rise = false;
	if (dio->mode == Memory::Shared::IO::DIO_Mode::OUTPUT) {
		rise = dio->output->Get();
	} else {
		rise = dio->input->Get();
	}
	IO::DIO::InterruptData data = { (uint8_t)dio->port, rise, !rise };
	IPC::broadcast(IO::DIO_IPC::INTERRUPT_TRIGGER, &data, sizeof(data));
}

static void _msg_handler_interrupt(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::DIO::InterruptData idata = *(IO::DIO::InterruptData*)data;
	tick_itf_dio();
	int port = idata.port;
	if (port > 25 || port < 0) return;
	if (dios[port].input != NULL) {
		dios[port].input->RequestInterrupts(_interrupt_callback, &dios[port]);
		dios[port].input->SetUpSourceEdge(true, true);
		dios[port].input->EnableInterrupts();
	}
	else if (dios[port].output != NULL) {
		dios[port].output->RequestInterrupts(_interrupt_callback, &dios[port]);
		dios[port].output->SetUpSourceEdge(true, true);
		dios[port].output->EnableInterrupts();
	}
}

static void _msg_handler_glitch_add(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::DIO_IPC::GlitchFilterMessage *msg = (IO::DIO_IPC::GlitchFilterMessage*)data;
	WPIInternalDIO *dio = &dios[msg->port];
	if (dio->last_filter != NULL) dio->last_filter->Remove(dio->source);

	uint32_t period_high = msg->period_high;
	uint32_t period_low = msg->period_low;
	uint64_t total_period = ((uint64_t)period_high) << 32 | period_low;

	tick_itf_dio();

	for (int i = 0; i < 26; i++) {
		if (filters[i] == NULL) {
			filters[i] = new DigitalGlitchFilter();
			filters[i]->Add(dio->source);
			if (msg->period_fpga)
				filters[i]->SetPeriodCycles(period_low);
			else
				filters[i]->SetPeriodNanoSeconds(total_period);
			dio->last_filter = filters[i];
			break;
		} else if (filters[i]->GetPeriodCycles() == period_low && msg->period_fpga) {
			filters[i]->Add(dio->source);
			dio->last_filter = filters[i];
			break;
		} else if (filters[i]->GetPeriodNanoSeconds() == total_period && !msg->period_fpga) {
			filters[i]->Add(dio->source);
			dio->last_filter = filters[i];
			break;
		}
	}
}

static void _msg_handler_glitch_remove(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::DIO_IPC::GlitchFilterMessage *msg = (IO::DIO_IPC::GlitchFilterMessage*)data;
	if (dios[msg->port].last_filter != NULL) dios[msg->port].last_filter->Remove(dios[msg->port].source);
}

static void _msg_handler_update_now(std::string handle, void *data, int data_len, int module_id, void *param) {
	tick_itf_dio();
}

void init_itf_dio() {
	IPC::listen(IO::DIO_IPC::INTERRUPT_ENABLE, &_msg_handler_interrupt);

	IPC::listen(IO::DIO_IPC::GLITCH_FILTER_ADD, &_msg_handler_glitch_add);
	IPC::listen(IO::DIO_IPC::GLITCH_FILTER_REMOVE, &_msg_handler_glitch_remove);

	IPC::listen(IO::DIO_IPC::DIGITAL_UPDATE, &_msg_handler_update_now);
}

void tick_itf_dio() {
	tick_mutex.lock();
	Concurrent::IPCMutex *mtx = Memory::shared_mutex()->dio;
	for (int i = 0; i < 26; i++) {
		MTX_LOCK(mtx, i);
		Memory::Shared::IO::DIO *dio = Memory::shared()->dio(i);
		if (dio->get_init()) {
			Memory::Shared::IO::DIO_Mode mode = dio->get_mode();
			if (!dio->get_bootstrap()) {
				dios[i].mode = mode;
				dios[i].port = i;
				if (mode == Memory::Shared::IO::DIO_Mode::INPUT) {
					dios[i].input = new DigitalInput(i);
					dios[i].source = (DigitalSource *)dios[i].input;
				} else {
					dios[i].output = new DigitalOutput(i);
					dios[i].source = (DigitalSource *)dios[i].output;
				}
				dio->set_bootstrap(true);
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
	tick_mutex.unlock();
}