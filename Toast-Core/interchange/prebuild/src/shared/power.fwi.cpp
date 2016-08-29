#include "shared/power.fwi.hpp"

FWIFUNC uint8_t Toast::Memory::Shared::IO::Power::get_pdp_can_id() {
	return FWI_MEM_VAL(uint8_t, _store, 1);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_can_id(uint8_t value) {
	FWI_MEM_VAL(uint8_t, _store, 1) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_pdp_voltage() {
	return FWI_MEM_VAL(double, _store, 2);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_voltage(double value) {
	FWI_MEM_VAL(double, _store, 2) = value;
}

FWIFUNC float Toast::Memory::Shared::IO::Power::get_pdp_temperature() {
	return FWI_MEM_VAL(float, _store, 10);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_temperature(float value) {
	FWI_MEM_VAL(float, _store, 10) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_pdp_total_power() {
	return FWI_MEM_VAL(double, _store, 14);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_total_power(double value) {
	FWI_MEM_VAL(double, _store, 14) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_pdp_total_energy() {
	return FWI_MEM_VAL(double, _store, 22);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_total_energy(double value) {
	FWI_MEM_VAL(double, _store, 22) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_pdp_total_current() {
	return FWI_MEM_VAL(double, _store, 30);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_total_current(double value) {
	FWI_MEM_VAL(double, _store, 30) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_pdp_current(int index) {
	return FWI_MEM_VAL(double, _store, 38 + (8 * index));
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_pdp_current(int index, double value) {
	FWI_MEM_VAL(double, _store, 38 + (8 * index)) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_rio_input_voltage() {
	return FWI_MEM_VAL(double, _store, 166);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_input_voltage(double value) {
	FWI_MEM_VAL(double, _store, 166) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_rio_input_current() {
	return FWI_MEM_VAL(double, _store, 174);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_input_current(double value) {
	FWI_MEM_VAL(double, _store, 174) = value;
}

FWIFUNC uint16_t Toast::Memory::Shared::IO::Power::get_rio_faults_3V3() {
	return FWI_MEM_VAL(uint16_t, _store, 182);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_faults_3V3(uint16_t value) {
	FWI_MEM_VAL(uint16_t, _store, 182) = value;
}

FWIFUNC uint16_t Toast::Memory::Shared::IO::Power::get_rio_faults_5V() {
	return FWI_MEM_VAL(uint16_t, _store, 184);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_faults_5V(uint16_t value) {
	FWI_MEM_VAL(uint16_t, _store, 184) = value;
}

FWIFUNC uint16_t Toast::Memory::Shared::IO::Power::get_rio_faults_6V() {
	return FWI_MEM_VAL(uint16_t, _store, 186);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_faults_6V(uint16_t value) {
	FWI_MEM_VAL(uint16_t, _store, 186) = value;
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

FWIFUNC double Toast::Memory::Shared::IO::Power::get_rio_voltage_3V3() {
	return FWI_MEM_VAL(double, _store, 188);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_voltage_3V3(double value) {
	FWI_MEM_VAL(double, _store, 188) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_rio_voltage_5V() {
	return FWI_MEM_VAL(double, _store, 196);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_voltage_5V(double value) {
	FWI_MEM_VAL(double, _store, 196) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_rio_voltage_6V() {
	return FWI_MEM_VAL(double, _store, 204);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_voltage_6V(double value) {
	FWI_MEM_VAL(double, _store, 204) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_rio_current_3V3() {
	return FWI_MEM_VAL(double, _store, 212);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_current_3V3(double value) {
	FWI_MEM_VAL(double, _store, 212) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_rio_current_5V() {
	return FWI_MEM_VAL(double, _store, 220);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_current_5V(double value) {
	FWI_MEM_VAL(double, _store, 220) = value;
}

FWIFUNC double Toast::Memory::Shared::IO::Power::get_rio_current_6V() {
	return FWI_MEM_VAL(double, _store, 228);
}
FWIFUNC void Toast::Memory::Shared::IO::Power::set_rio_current_6V(double value) {
	FWI_MEM_VAL(double, _store, 228) = value;
}


