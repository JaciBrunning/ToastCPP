#include "shared/digital.fwi.hpp"

FWIFUNC bool Toast::Memory::Shared::IO::DIO::get_init() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_init(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::DIO::get_bootstrap() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_bootstrap(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC Toast::Memory::Shared::IO::DIO_Mode Toast::Memory::Shared::IO::DIO::get_mode() {
	return (Toast::Memory::Shared::IO::DIO_Mode)(_store[2]);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_mode(Toast::Memory::Shared::IO::DIO_Mode value) {
	_store[2] = (char)value;
}

FWIFUNC bool Toast::Memory::Shared::IO::DIO::get_value() {
	return FWI_IS_BIT_SET(_store[0], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_value(bool value) {
	FWI_SET_BIT_TO(_store[0], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::DIO::get_pwm_enabled() {
	return FWI_IS_BIT_SET(_store[0], 3);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_pwm_enabled(bool value) {
	FWI_SET_BIT_TO(_store[0], 3, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::DIO::get_pwm_enabled_pending() {
	return FWI_IS_BIT_SET(_store[0], 4);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_pwm_enabled_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 4, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::DIO::is_pulsing() {
	return FWI_IS_BIT_SET(_store[0], 5);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_is_pulsing(bool value) {
	FWI_SET_BIT_TO(_store[0], 5, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::DIO::get_pulse_pending() {
	return FWI_IS_BIT_SET(_store[0], 6);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_pulse_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 6, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::DIO::get_pwm_rate_pending() {
	return FWI_IS_BIT_SET(_store[0], 7);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_pwm_rate_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 7, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::DIO::get_pwm_duty_cycle_pending() {
	return FWI_IS_BIT_SET(_store[1], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_pwm_duty_cycle_pending(bool value) {
	FWI_SET_BIT_TO(_store[1], 0, value ? 1 : 0);
}

FWIFUNC float Toast::Memory::Shared::IO::DIO::get_pulse_length() {
	return FWI_MEM_VAL(float, _store, 3);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_pulse_length(float value) {
	FWI_MEM_VAL(float, _store, 3) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::DIO::get_pwm_duty_cycle() {
	return FWI_MEM_VAL(float, _store, 7);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_pwm_duty_cycle(float value) {
	FWI_MEM_VAL(float, _store, 7) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::DIO::get_pwm_rate() {
	return FWI_MEM_VAL(float, _store, 11);
}
FWIFUNC void Toast::Memory::Shared::IO::DIO::set_pwm_rate(float value) {
	FWI_MEM_VAL(float, _store, 11) = value;
}


