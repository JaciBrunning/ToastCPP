#include "thp/wpi/interfaces.hpp"

#include "toast/memory.hpp"

#include "AnalogInput.h"
#include "AnalogOutput.h"

using namespace Toast;

static AnalogInput *ains[8];
static AnalogOutput *aouts[2];
static bool at_least_one_registered = false;

void tick_itf_analog() {
	// NOTE: See issue #284 on https://github.com/wpilibsuite/allwpilib/
//	if (at_least_one_registered)
//		MTX_WRAP(Memory::shared_mutex()->analog_in_static, 0,
//			Memory::shared()->analog_in_settings()->set_sample_rate(AnalogInput::GetSampleRate()));

	Concurrent::IPCMutex *mtx = Memory::shared_mutex()->analog_in;
	for (int i = 0; i < 8; i++) {
		MTX_LOCK(mtx, i);
		Memory::Shared::IO::AnalogIn *mem = Memory::shared()->analog_in(i);
		if (mem->get_init()) {
			if (!mem->get_bootstrap()) {
				ains[i] = new AnalogInput(i);
				at_least_one_registered = true;
				mem->set_bootstrap(true);
			}
			AnalogInput *a = ains[i];
			mem->set_value(a->GetValue());
			mem->set_average_value(a->GetAverageValue());
			mem->set_voltage(a->GetVoltage());
			mem->set_average_voltage(a->GetAverageVoltage());

			mem->set_average_bits(a->GetAverageBits());
			mem->set_oversample_bits(a->GetOversampleBits());
			mem->set_lsb_weight(a->GetLSBWeight());
			mem->set_offset(a->GetOffset());
			
			// Accum Logic
			if (mem->get_accum_pending()) {
				a->InitAccumulator();
				mem->set_accum_pending(false);
			}
			if (mem->get_accum_reset_pending()) {
				a->ResetAccumulator();
				mem->set_accum_reset_pending(false);
			}
			if (mem->get_accum_initial_pending()) {
				a->SetAccumulatorInitialValue(mem->get_accum_initial());
				mem->set_accum_initial_pending(false);
			}
			if (mem->get_accum_centre_pending()) {
				a->SetAccumulatorCenter(mem->get_accum_centre());
				mem->set_accum_centre_pending(false);
			}
			if (mem->get_accum_deadband_pending()) {
				a->SetAccumulatorDeadband(mem->get_accum_deadband());
				mem->set_accum_deadband_pending(false);
			}
			mem->set_is_accum(a->IsAccumulatorChannel());
			if (a->IsAccumulatorChannel()) {
				mem->set_accum_count(a->GetAccumulatorCount());
				mem->set_accum_value(a->GetAccumulatorValue());
			}
		}
		MTX_UNLOCK(mtx, i);
	}

	mtx = Memory::shared_mutex()->analog_out;
	for (int i = 0; i < 2; i++) {
		MTX_LOCK(mtx, i);
		Memory::Shared::IO::AnalogOut *mem = Memory::shared()->analog_out(i);
		if (mem->get_init()) {
			if (!mem->get_bootstrapped()) {
				aouts[i] = new AnalogOutput(i);
				mem->set_bootstrapped(true);
			}
			aouts[i]->SetVoltage(mem->get_voltage());
		}
		MTX_UNLOCK(mtx, i);
	}
}