#include "shared/pneumatics.fwi.hpp"

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_init() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_init(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_bootstrap() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_bootstrap(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC uint8_t Toast::Memory::Shared::IO::Pneumatics::get_pcm_can_id() {
	return FWI_MEM_VAL(uint8_t, _store, 3);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_pcm_can_id(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 3) = value;
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_fault_comp_too_high() {
	return FWI_IS_BIT_SET(_store[0], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_fault_comp_too_high(bool value) {
	FWI_SET_BIT_TO(_store[0], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_fault_comp_too_high_sticky() {
	return FWI_IS_BIT_SET(_store[0], 3);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_fault_comp_too_high_sticky(bool value) {
	FWI_SET_BIT_TO(_store[0], 3, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_fault_comp_shorted() {
	return FWI_IS_BIT_SET(_store[0], 4);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_fault_comp_shorted(bool value) {
	FWI_SET_BIT_TO(_store[0], 4, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_fault_comp_shorted_sticky() {
	return FWI_IS_BIT_SET(_store[0], 5);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_fault_comp_shorted_sticky(bool value) {
	FWI_SET_BIT_TO(_store[0], 5, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_fault_comp_not_conn() {
	return FWI_IS_BIT_SET(_store[0], 6);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_fault_comp_not_conn(bool value) {
	FWI_SET_BIT_TO(_store[0], 6, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_fault_comp_not_conn_sticky() {
	return FWI_IS_BIT_SET(_store[0], 7);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_fault_comp_not_conn_sticky(bool value) {
	FWI_SET_BIT_TO(_store[0], 7, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_comp_sticky_clear_pending() {
	return FWI_IS_BIT_SET(_store[1], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_comp_sticky_clear_pending(bool value) {
	FWI_SET_BIT_TO(_store[1], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_pressure_switch() {
	return FWI_IS_BIT_SET(_store[1], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_pressure_switch(bool value) {
	FWI_SET_BIT_TO(_store[1], 1, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_closed_loop() {
	return FWI_IS_BIT_SET(_store[1], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_closed_loop(bool value) {
	FWI_SET_BIT_TO(_store[1], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_is_enabled() {
	return FWI_IS_BIT_SET(_store[1], 3);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_is_enabled(bool value) {
	FWI_SET_BIT_TO(_store[1], 3, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_start_pending() {
	return FWI_IS_BIT_SET(_store[1], 4);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_start_pending(bool value) {
	FWI_SET_BIT_TO(_store[1], 4, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_stop_pending() {
	return FWI_IS_BIT_SET(_store[1], 5);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_stop_pending(bool value) {
	FWI_SET_BIT_TO(_store[1], 5, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_closed_loop_mode_pending() {
	return FWI_IS_BIT_SET(_store[1], 6);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_closed_loop_mode_pending(bool value) {
	FWI_SET_BIT_TO(_store[1], 6, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_solenoid(int index) {
	return FWI_IS_BIT_SET(_store[4 + index / 8], index % 8);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_solenoid(int index, bool value) {
	FWI_SET_BIT_TO(_store[4 + index / 8], index % 8, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_solenoid_black(int index) {
	return FWI_IS_BIT_SET(_store[5 + index / 8], index % 8);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_solenoid_black(int index, bool value) {
	FWI_SET_BIT_TO(_store[5 + index / 8], index % 8, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_fault_sol_volt() {
	return FWI_IS_BIT_SET(_store[1], 7);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_fault_sol_volt(bool value) {
	FWI_SET_BIT_TO(_store[1], 7, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_fault_sol_volt_sticky() {
	return FWI_IS_BIT_SET(_store[2], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_fault_sol_volt_sticky(bool value) {
	FWI_SET_BIT_TO(_store[2], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Pneumatics::get_sol_sticky_clear_pending() {
	return FWI_IS_BIT_SET(_store[2], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_sol_sticky_clear_pending(bool value) {
	FWI_SET_BIT_TO(_store[2], 1, value ? 1 : 0);
}

FWIFUNC float Toast::Memory::Shared::IO::Pneumatics::get_comp_current() {
	return FWI_MEM_VAL(float, _store, 6);
}
FWIFUNC void Toast::Memory::Shared::IO::Pneumatics::set_comp_current(float value) {
	FWI_MEM_VAL(float, _store, 6) = value;
}


