#include "io/power.hpp"

#include <inttypes.h>

using namespace IO;
using namespace Toast::Memory;

char *IO::get_power_block() {
	return Shared::get() + ADDR_PWR_OFFSET;
}

// Power Distribution Panel (PDP)

void PDP::set_can_id(int id) {
	get_power_block()[ADDR_PWR_CAN_ID] = (char)id;
}

float PDP::get_voltage() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_PDP_VOLTAGE);
}

float PDP::get_temperature() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_PDP_TEMP);
}

float PDP::get_current(int channel) {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_PDP_CUR_OFFSET + (channel * 4));
}

float PDP::get_total_current() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_PDP_TOTAL_CUR);
}

float PDP::get_total_power() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_PDP_TOTAL_PWR);
}

float PDP::get_total_energy() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_PDP_TOTAL_ENG);
}

// Controller (RoboRIO power rails)

float Controller::get_input_voltage() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_CONTR_IN_VOLT);
}

float Controller::get_input_current() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_CONTR_IN_CUR);
}

float Controller::get_voltage_3V3() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_CONTR_VOLT_3V3);
}

float Controller::get_current_3V3() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_CONTR_CUR_3V3);
}

bool Controller::get_enabled_3V3() {
	return IS_BIT_SET(get_power_block()[ADDR_PWR_CONTR_BUS_ENABLE], 0);
}

int Controller::get_fault_count_3V3() {
	return (int)MEM_VAL(uint16_t, get_power_block(), ADDR_PWR_CONTR_FAULTS_3V3);
}

float Controller::get_voltage_5V() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_CONTR_VOLT_5V);
}

float Controller::get_current_5V() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_CONTR_CUR_5V);
}

bool Controller::get_enabled_5V() {
	return IS_BIT_SET(get_power_block()[ADDR_PWR_CONTR_BUS_ENABLE], 1);
}

int Controller::get_fault_count_5V() {
	return (int)MEM_VAL(uint16_t, get_power_block(), ADDR_PWR_CONTR_FAULTS_5V);
}

float Controller::get_voltage_6V() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_CONTR_VOLT_6V);
}

float Controller::get_current_6V() {
	return MEM_VAL(float, get_power_block(), ADDR_PWR_CONTR_CUR_6V);
}

bool Controller::get_enabled_6V() {
	return IS_BIT_SET(get_power_block()[ADDR_PWR_CONTR_BUS_ENABLE], 2);
}

int Controller::get_fault_count_6V() {
	return (int)MEM_VAL(uint16_t, get_power_block(), ADDR_PWR_CONTR_FAULTS_6V);
}

bool Controller::is_brownout() {
	return get_power_block()[ADDR_PWR_IS_BROWNOUT] == 1;
}