#include "thp/sim/sim_memory.hpp"

#include "toast/memory.hpp"
#include "io/common.hpp"

// Set a bunch of 'placeholder' values in the Shared Memory Pool.
void Sim::Memory::init() {
	Toast::Memory::SharedPool *mem = Toast::Memory::shared();
	Toast::Memory::SharedMutex *mtx = Toast::Memory::shared_mutex();

	MTX_LOCK(mtx->power, 0);
	mem->power()->set_pdp_can_id(0);
	mem->power()->set_pdp_voltage(12.90);
	mem->power()->set_pdp_temperature(30.0);
	
	mem->power()->set_rio_input_voltage(12.9);
	mem->power()->set_rio_3V3_enabled(true);
	mem->power()->set_rio_5V_enabled(true);
	mem->power()->set_rio_6V_enabled(true);
	mem->power()->set_rio_voltage_3V3(3.3);
	mem->power()->set_rio_voltage_5V(5.0);
	mem->power()->set_rio_voltage_6V(6.0);
	MTX_UNLOCK(mtx->power, 0);
}