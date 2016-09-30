#include "shared/power.fwi.hpp"

FWIFUNC uint8_t Toast::Memory::Shared::IO::Power::get_pdp_can_id() {
	return FWI_MEM_VAL(uint8_t, _store, 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_can_id(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 1) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_pdp_voltage() {
	return FWI_MEM_VAL(float, _store, 2);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_voltage(float value) {
	FWI_MEM_VAL(float, _store, 2) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_pdp_temperature() {
	return FWI_MEM_VAL(float, _store, 6);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_temperature(float value) {
	FWI_MEM_VAL(float, _store, 6) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_pdp_total_power() {
	return FWI_MEM_VAL(float, _store, 10);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_total_power(float value) {
	FWI_MEM_VAL(float, _store, 10) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_pdp_total_energy() {
	return FWI_MEM_VAL(float, _store, 14);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_total_energy(float value) {
	FWI_MEM_VAL(float, _store, 14) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_pdp_total_current() {
	return FWI_MEM_VAL(float, _store, 18);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_total_current(float value) {
	FWI_MEM_VAL(float, _store, 18) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_pdp_current(int index) {
	return FWI_MEM_VAL(float, _store, 22 + (4 * index));
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_current(int index, float value) {
	FWI_MEM_VAL(float, _store, 22 + (4 * index)) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_rio_input_voltage() {
	return FWI_MEM_VAL(float, _store, 86);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_input_voltage(float value) {
	FWI_MEM_VAL(float, _store, 86) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_rio_input_current() {
	return FWI_MEM_VAL(float, _store, 90);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_input_current(float value) {
	FWI_MEM_VAL(float, _store, 90) = value;
}

FWIFUNC uint16_t Toast::Memory::Shared::IO::Power::get_rio_faults_3V3() {
	return FWI_MEM_VAL(uint16_t, _store, 94);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_faults_3V3(uint16_t value) {
	FWI_MEM_VAL(uint16_t, _store, 94) = value;
}

FWIFUNC uint16_t Toast::Memory::Shared::IO::Power::get_rio_faults_5V() {
	return FWI_MEM_VAL(uint16_t, _store, 96);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_faults_5V(uint16_t value) {
	FWI_MEM_VAL(uint16_t, _store, 96) = value;
}

FWIFUNC uint16_t Toast::Memory::Shared::IO::Power::get_rio_faults_6V() {
	return FWI_MEM_VAL(uint16_t, _store, 98);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_faults_6V(uint16_t value) {
	FWI_MEM_VAL(uint16_t, _store, 98) = value;
}

FWIFUNC bool Toast::Memory::Shared::IO::Power::get_rio_3V3_enabled() {
	return FWI_IS_BIT_SET(_store[0], 0);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_3V3_enabled(bool value) {
	FWI_SET_BIT_TO(_store[0], 0, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Power::get_rio_5V_enabled() {
	return FWI_IS_BIT_SET(_store[0], 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_5V_enabled(bool value) {
	FWI_SET_BIT_TO(_store[0], 1, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Power::get_rio_6V_enabled() {
	return FWI_IS_BIT_SET(_store[0], 2);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_6V_enabled(bool value) {
	FWI_SET_BIT_TO(_store[0], 2, value ? 1 : 0);
}

FWIFUNC bool Toast::Memory::Shared::IO::Power::get_rio_brownout() {
	return FWI_IS_BIT_SET(_store[0], 3);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_brownout(bool value) {
	FWI_SET_BIT_TO(_store[0], 3, value ? 1 : 0);
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_rio_voltage_3V3() {
	return FWI_MEM_VAL(float, _store, 100);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_voltage_3V3(float value) {
	FWI_MEM_VAL(float, _store, 100) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_rio_voltage_5V() {
	return FWI_MEM_VAL(float, _store, 104);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_voltage_5V(float value) {
	FWI_MEM_VAL(float, _store, 104) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_rio_voltage_6V() {
	return FWI_MEM_VAL(float, _store, 108);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_voltage_6V(float value) {
	FWI_MEM_VAL(float, _store, 108) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_rio_current_3V3() {
	return FWI_MEM_VAL(float, _store, 112);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_current_3V3(float value) {
	FWI_MEM_VAL(float, _store, 112) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_rio_current_5V() {
	return FWI_MEM_VAL(float, _store, 116);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_current_5V(float value) {
	FWI_MEM_VAL(float, _store, 116) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_rio_current_6V() {
	return FWI_MEM_VAL(float, _store, 120);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_current_6V(float value) {
	FWI_MEM_VAL(float, _store, 120) = value;
}


