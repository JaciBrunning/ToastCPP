#include "io/power.hpp"

using namespace Power;
using namespace Toast::Memory;

char *Power::getPowerBlock() {
	return Shared::get() + ADDR_PWR_OFFSET;
}

// PDP

void PDP::set_can_id(uint8_t id) {
	*(uint8_t *)(Power::getPowerBlock() + ADDR_PWR_CAN_ID) = id;
}

double PDP::get_voltage() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_PDP_VOLTAGE);
}

double PDP::get_temperature() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_PDP_TEMP);
}

double PDP::get_current(uint8_t channel) {
	return (double) *(float *)
		(Power::getPowerBlock() + ADDR_PWR_PDP_CUR_OFFSET + (channel * 4));
}

double PDP::get_total_current() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_PDP_TOTAL_CUR);
}

double PDP::get_total_power() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_PDP_TOTAL_PWR);
}

double PDP::get_total_energy() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_PDP_TOTAL_ENG);
}

// Controller

double Controller::get_input_voltage() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_CONTR_IN_VOLT);
}

double Controller::get_input_current() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_CONTR_IN_CUR);
}

double Controller::get_voltage_3V3() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_CONTR_VOLT_3V3);
}

double Controller::get_current_3V3() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_CONTR_CUR_3V3);
}

bool Controller::get_enabled_3V3() {
	uint8_t mask = *(uint8_t *)(Power::getPowerBlock() + ADDR_PWR_CONTR_BUS_ENABLE);
	return (mask & (1 << 0)) != 0;			// 3V3 at Position 0
}

int Controller::get_fault_count_3V3() {
	return (int) *(uint16_t *)(Power::getPowerBlock() + ADDR_PWR_CONTR_FAULTS_3V3);
}

double Controller::get_voltage_5V() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_CONTR_VOLT_5V);
}

double Controller::get_current_5V() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_CONTR_CUR_5V);
}

bool Controller::get_enabled_5V() {
	uint8_t mask = *(uint8_t *)(Power::getPowerBlock() + ADDR_PWR_CONTR_BUS_ENABLE);
	return (mask & (1 << 1)) != 0;			// 5V at Position 1
}

int Controller::get_fault_count_5V() {
	return (int) *(uint16_t *)(Power::getPowerBlock() + ADDR_PWR_CONTR_FAULTS_5V);
}

double Controller::get_voltage_6V() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_CONTR_VOLT_6V);
}

double Controller::get_current_6V() {
	return (double) *(float *)(Power::getPowerBlock() + ADDR_PWR_CONTR_CUR_6V);
}

bool Controller::get_enabled_6V() {
	uint8_t mask = *(uint8_t *)(Power::getPowerBlock() + ADDR_PWR_CONTR_BUS_ENABLE);
	return (mask & (1 << 2)) != 0;			// 6V at Position 2
}

int Controller::get_fault_count_6V() {
	return (int) *(uint16_t *)(Power::getPowerBlock() + ADDR_PWR_CONTR_FAULTS_6V);
}