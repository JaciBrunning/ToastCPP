#include "thp/wpi/interfaces.hpp"

#include "toast/memory.hpp"

#include "PowerDistributionPanel.h"
#include "ControllerPower.h"
#include "HAL/HAL.h"

using namespace Toast;

static int last_pdp = -1;
static PowerDistributionPanel *panel;

void tick_itf_power() {
	Memory::Shared::IO::Power *power = Memory::shared()->power();
	// PDP
	MTX_LOCK(Memory::shared_mutex()->power, 0);
	int pdp_can = power->get_pdp_can_id();
	if (last_pdp != pdp_can) {
		last_pdp = pdp_can;
		panel = new PowerDistributionPanel(pdp_can);
	}
	power->set_pdp_voltage(panel->GetVoltage());
	power->set_pdp_temperature(panel->GetTemperature());
	power->set_pdp_total_power(panel->GetTotalPower());
	power->set_pdp_total_energy(panel->GetTotalEnergy());
	power->set_pdp_total_current(panel->GetTotalCurrent());

	for (int i = 0; i < 16; i++) {
		power->set_pdp_current(i, panel->GetCurrent(i));
	}
	MTX_UNLOCK(Memory::shared_mutex()->power, 0);

	// Controller
	MTX_LOCK(Memory::shared_mutex()->power, 1);
	power->set_rio_input_voltage(ControllerPower::GetInputVoltage());
	power->set_rio_input_current(ControllerPower::GetInputCurrent());
	power->set_rio_faults_3V3(ControllerPower::GetFaultCount3V3());
	power->set_rio_faults_5V(ControllerPower::GetFaultCount5V());
	power->set_rio_faults_6V(ControllerPower::GetFaultCount6V());
	power->set_rio_3V3_enabled(ControllerPower::GetEnabled3V3());
	power->set_rio_5V_enabled(ControllerPower::GetEnabled5V());
	power->set_rio_6V_enabled(ControllerPower::GetEnabled6V());
	int32_t hal_status = 0;
	power->set_rio_brownout(HAL_GetBrownedOut(&hal_status));
	power->set_rio_voltage_3V3(ControllerPower::GetVoltage3V3());
	power->set_rio_voltage_5V(ControllerPower::GetVoltage5V());
	power->set_rio_voltage_6V(ControllerPower::GetVoltage6V());
	power->set_rio_current_3V3(ControllerPower::GetCurrent3V3());
	power->set_rio_current_5V(ControllerPower::GetCurrent5V());
	power->set_rio_current_6V(ControllerPower::GetCurrent6V());
	MTX_UNLOCK(Memory::shared_mutex()->power, 1);
}