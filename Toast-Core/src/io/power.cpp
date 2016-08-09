#include "io/power.hpp"

#include <inttypes.h>

using namespace IO;
using namespace Toast::Memory;

static inline Shared::IO::Power *_power() {
	return shared()->power();
}

// Power Distribution Panel (PDP)

void PDP::set_can_id(int id) {
	_power()->set_pdp_can_id(id);
}

double PDP::get_voltage() {
	return _power()->get_pdp_voltage();
}

float PDP::get_temperature() {
	return _power()->get_pdp_temperature();
}

double PDP::get_current(int channel) {
	return _power()->get_pdp_current(channel);
}

double PDP::get_total_current() {
	return _power()->get_pdp_total_current();
}

double PDP::get_total_power() {
	return _power()->get_pdp_total_power();
}

double PDP::get_total_energy() {
	return _power()->get_pdp_total_energy();
}

// Controller (RoboRIO power rails)

double Controller::get_input_voltage() {
	return _power()->get_rio_input_voltage();
}

double Controller::get_input_current() {
	return _power()->get_rio_input_current();
}

double Controller::get_voltage_3V3() {
	return _power()->get_rio_voltage_3V3();
}

double Controller::get_current_3V3() {
	return _power()->get_rio_current_3V3();
}

bool Controller::get_enabled_3V3() {
	return _power()->get_rio_3V3_enabled();
}

int Controller::get_fault_count_3V3() {
	return _power()->get_rio_faults_3V3();
}

double Controller::get_voltage_5V() {
	return _power()->get_rio_voltage_5V();
}

double Controller::get_current_5V() {
	return _power()->get_rio_current_5V();
}

bool Controller::get_enabled_5V() {
	return _power()->get_rio_5V_enabled();
}

int Controller::get_fault_count_5V() {
	return _power()->get_rio_faults_5V();
}

double Controller::get_voltage_6V() {
	return _power()->get_rio_voltage_6V();
}

double Controller::get_current_6V() {
	return _power()->get_rio_current_6V();
}

bool Controller::get_enabled_6V() {
	return _power()->get_rio_6V_enabled();
}

int Controller::get_fault_count_6V() {
	return _power()->get_rio_faults_6V();
}

bool Controller::is_brownout() {
	return _power()->get_rio_brownout();
}