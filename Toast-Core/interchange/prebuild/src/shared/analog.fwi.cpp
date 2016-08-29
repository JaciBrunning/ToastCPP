#include "shared/analog.fwi.hpp"

FWIFUNC bool Toast::Memory::Shared::IO::AnalogOut::get_init() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogOut::set_init(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::AnalogOut::get_bootstrapped() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogOut::set_bootstrapped(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC float Toast::Memory::Shared::IO::AnalogOut::get_voltage() {
	return FWI_MEM_VAL(float, _store, 1);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogOut::set_voltage(float value) {
	FWI_MEM_VAL(float, _store, 1) = value;
}


FWIFUNC bool Toast::Memory::Shared::IO::AnalogIn::get_init() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_init(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::AnalogIn::get_bootstrap() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_bootstrap(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC int16_t Toast::Memory::Shared::IO::AnalogIn::get_value() {
	return FWI_MEM_VAL(int16_t, _store, 1);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_value(int16_t value) {
	FWI_MEM_VAL(int16_t, _store, 1) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::AnalogIn::get_average_value() {
	return FWI_MEM_VAL(int32_t, _store, 3);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_average_value(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 3) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::AnalogIn::get_voltage() {
	return FWI_MEM_VAL(float, _store, 7);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_voltage(float value) {
	FWI_MEM_VAL(float, _store, 7) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::AnalogIn::get_average_voltage() {
	return FWI_MEM_VAL(float, _store, 11);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_average_voltage(float value) {
	FWI_MEM_VAL(float, _store, 11) = value;
}

FWIFUNC uint32_t Toast::Memory::Shared::IO::AnalogIn::get_average_bits() {
	return FWI_MEM_VAL(uint32_t, _store, 15);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_average_bits(uint32_t value) {
	FWI_MEM_VAL(uint32_t, _store, 15) = value;
}

FWIFUNC uint32_t Toast::Memory::Shared::IO::AnalogIn::get_oversample_bits() {
	return FWI_MEM_VAL(uint32_t, _store, 19);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_oversample_bits(uint32_t value) {
	FWI_MEM_VAL(uint32_t, _store, 19) = value;
}

FWIFUNC uint32_t Toast::Memory::Shared::IO::AnalogIn::get_lsb_weight() {
	return FWI_MEM_VAL(uint32_t, _store, 23);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_lsb_weight(uint32_t value) {
	FWI_MEM_VAL(uint32_t, _store, 23) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::AnalogIn::get_offset() {
	return FWI_MEM_VAL(int32_t, _store, 27);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_offset(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 27) = value;
}

FWIFUNC bool Toast::Memory::Shared::IO::AnalogIn::get_is_accum() {
	return FWI_IS_BIT_SET(_store[0], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_is_accum(bool value) {
	FWI_SET_BIT_TO(_store[0], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::AnalogIn::get_accum_pending() {
	return FWI_IS_BIT_SET(_store[0], 3);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 3, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::AnalogIn::get_accum_reset_pending() {
	return FWI_IS_BIT_SET(_store[0], 4);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_reset_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 4, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::AnalogIn::get_accum_initial_pending() {
	return FWI_IS_BIT_SET(_store[0], 5);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_initial_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 5, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::AnalogIn::get_accum_centre_pending() {
	return FWI_IS_BIT_SET(_store[0], 6);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_centre_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 6, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::AnalogIn::get_accum_deadband_pending() {
	return FWI_IS_BIT_SET(_store[0], 7);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_deadband_pending(bool value) {
	FWI_SET_BIT_TO(_store[0], 7, value ? 1 : 0);
}

FWIFUNC uint32_t Toast::Memory::Shared::IO::AnalogIn::get_accum_count() {
	return FWI_MEM_VAL(uint32_t, _store, 31);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_count(uint32_t value) {
	FWI_MEM_VAL(uint32_t, _store, 31) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::AnalogIn::get_accum_deadband() {
	return FWI_MEM_VAL(int32_t, _store, 35);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_deadband(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 35) = value;
}

FWIFUNC int32_t Toast::Memory::Shared::IO::AnalogIn::get_accum_centre() {
	return FWI_MEM_VAL(int32_t, _store, 39);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_centre(int32_t value) {
	FWI_MEM_VAL(int32_t, _store, 39) = value;
}

FWIFUNC int64_t Toast::Memory::Shared::IO::AnalogIn::get_accum_value() {
	return FWI_MEM_VAL(int64_t, _store, 43);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_value(int64_t value) {
	FWI_MEM_VAL(int64_t, _store, 43) = value;
}

FWIFUNC int64_t Toast::Memory::Shared::IO::AnalogIn::get_accum_initial() {
	return FWI_MEM_VAL(int64_t, _store, 51);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogIn::set_accum_initial(int64_t value) {
	FWI_MEM_VAL(int64_t, _store, 51) = value;
}


FWIFUNC float Toast::Memory::Shared::IO::AnalogInSettings::get_sample_rate() {
	return FWI_MEM_VAL(float, _store, 0);
}
FWIFUNC void Toast::Memory::Shared::IO::AnalogInSettings::set_sample_rate(float value) {
	FWI_MEM_VAL(float, _store, 0) = value;
}


