#include "io/digital.hpp"
#include "toast/ipc.hpp"

#include "toast/concurrent/condition.hpp"

using namespace IO;
using namespace Toast::Memory;

static inline Toast::Concurrent::IPCMutex *mtx() {
	return shared_mutex()->dio;
}

DIO::DIO(int port, DIO::Mode mode) : _port(port) {
	_mem = shared()->dio(port);
	MTX_LOCK(mtx(), port);
	_mem->set_init(true);
	_mem->set_mode(mode);
	MTX_UNLOCK(mtx(), port);
}

int DIO::get_port() {
	return _port;
}

DIO::Mode DIO::get_mode() {
	return _mem->get_mode();
}

bool DIO::get() {
	MTX_RETURN(mtx(), _port, _mem->get_value());
}

void DIO::set(bool state) {
	MTX_WRAP(mtx(), _port, _mem->set_value(state));
}

void DIO::set_pulse(float length) {
	MTX_LOCK(mtx(), _port);
	_mem->set_pulse_length(length);
	_mem->set_pulse_pending(true);
	MTX_UNLOCK(mtx(), _port);
}

bool DIO::is_pulsing() {
	MTX_RETURN(mtx(), _port, _mem->is_pulsing());
}

void DIO::set_pwm_rate(float rate) {
	MTX_LOCK(mtx(), _port);
	_mem->set_pwm_rate(rate);
	_mem->set_pwm_rate_pending(true);
	MTX_UNLOCK(mtx(), _port);
}

void DIO::set_pwm_enable(float initial_duty_cycle) {
	MTX_LOCK(mtx(), _port);
	_mem->set_pwm_duty_cycle(initial_duty_cycle);
	_mem->set_pwm_enabled(true);
	_mem->set_pwm_enabled_pending(true);
	MTX_UNLOCK(mtx(), _port);
}

void DIO::set_pwm_disable() {
	MTX_LOCK(mtx(), _port);
	_mem->set_pwm_enabled(false);
	_mem->set_pwm_enabled_pending(true);
	MTX_UNLOCK(mtx(), _port);
}

void DIO::set_pwm_duty_cycle(float duty_cycle) {
	MTX_LOCK(mtx(), _port);
	_mem->set_pwm_duty_cycle(duty_cycle);
	_mem->set_pwm_duty_cycle_pending(true);
	MTX_UNLOCK(mtx(), _port);
}

void DIO::enable_interrupt() {
	InterruptData data = { (uint8_t)_port, true, true };
	Toast::IPC::send(DIO_IPC::INTERRUPT_ENABLE, (char *)&data, sizeof(data));
}

static DIO::InterruptHandler _handlers[26];
static bool listening = false;

static void _msg_handler(std::string handle, void *data, int data_len, int module_id, void *param) {
	DIO::InterruptData *idata = (DIO::InterruptData*)data;
	if (_handlers[idata->port] != NULL)
		_handlers[idata->port](*idata);
}

void DIO::on_interrupt(InterruptHandler handler) {
	_handlers[_port] = handler;
	if (!listening) {
		Toast::IPC::listen(DIO_IPC::INTERRUPT_TRIGGER, &_msg_handler);
		listening = true;
	}
}

bool DIO::wait_for_interrupt() {
	Toast::Concurrent::ConditionVariable cv(true);
	bool rising = 0;
	DIO::on_interrupt([&rising, &cv](InterruptData data) {
		rising = data.rising;
		cv.signal();
	});
	cv.lock();
	cv.wait();
	cv.unlock();
	return rising;
}

void DIO::add_glitch_filter(uint64_t period, DIO::GlitchFilterMode mode) {
//	DIO_IPC::GlitchFilterMessage msg = { _port, cycles };
	DIO_IPC::GlitchFilterMessage msg = { _port };
	if (mode == DIO::GlitchFilterMode::FPGA_CYCLES) {
		msg.period_low = (uint32_t)period;
		msg.period_fpga = true;
	} else {
		msg.period_low = (uint32_t)period;
		msg.period_high = (uint32_t)((period & 0xFFFFFFFF00000000LL) >> 32);
		msg.period_fpga = false;
	}
	Toast::IPC::send(DIO_IPC::GLITCH_FILTER_ADD, &msg, sizeof(msg));
}

void DIO::remove_glitch_filter() {
	DIO_IPC::GlitchFilterMessage msg = { _port };
	Toast::IPC::send(DIO_IPC::GLITCH_FILTER_REMOVE, &msg, sizeof(msg));
}

void DIO::update_now(bool wait) {
	if (wait)
		Toast::IPC::send_with_ack(DIO_IPC::DIGITAL_UPDATE, NULL, 0);
	else
		Toast::IPC::send(DIO_IPC::DIGITAL_UPDATE, NULL, 0);
}