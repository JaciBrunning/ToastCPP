#include "thp/wpi/interfaces.hpp"

#include "toast/memory.hpp"

#include "Compressor.h"
#include "Solenoid.h"

using namespace Toast;

static Compressor *cmps[2];
static Solenoid *sols[2][8];

void tick_itf_pneumatic() {
	Concurrent::IPCMutex *mtx = Memory::shared_mutex()->pcm;
	for (int i = 0; i < 2; i++) {
		MTX_LOCK(mtx, i);
		Memory::Shared::IO::Pneumatics *pcm = Memory::shared()->pneumatics(i);
		if (pcm->get_init()) {
			Solenoid **sol = sols[i];
			int modid = pcm->get_pcm_can_id();
			if (!pcm->get_bootstrap()) {
				cmps[i] = new Compressor(modid);
				pcm->set_closed_loop(cmps[i]->GetClosedLoopControl());
				for (int j = 0; j < 8; j++) {
					sol[j] = new Solenoid(modid, j);
				}
				pcm->set_bootstrap(true);
			}

			Compressor *cmp = cmps[i];
			pcm->set_fault_comp_too_high(cmp->GetCompressorCurrentTooHighFault());
			pcm->set_fault_comp_not_conn_sticky(cmp->GetCompressorCurrentTooHighStickyFault());
			pcm->set_fault_comp_shorted(cmp->GetCompressorShortedFault());
			pcm->set_fault_comp_shorted_sticky(cmp->GetCompressorShortedStickyFault());
			pcm->set_fault_comp_not_conn(cmp->GetCompressorNotConnectedFault());
			pcm->set_fault_comp_not_conn_sticky(cmp->GetCompressorNotConnectedStickyFault());
			if (pcm->get_comp_sticky_clear_pending()) {
				cmp->ClearAllPCMStickyFaults();
				pcm->set_comp_sticky_clear_pending(false);
			}

			pcm->set_pressure_switch(cmp->GetPressureSwitchValue());
			if (pcm->get_closed_loop_mode_pending()) {
				cmp->SetClosedLoopControl(pcm->get_closed_loop());
				pcm->set_closed_loop_mode_pending(true);
			}
			pcm->set_is_enabled(cmp->Enabled());

			if (pcm->get_start_pending()) {
				cmp->Start();
				pcm->set_start_pending(false);
			}

			if (pcm->get_stop_pending()) {
				cmp->Stop();
				pcm->set_stop_pending(false);
			}

			for (int j = 0; j < 8; j++) {
				Solenoid *s = sol[j];
				s->Set(pcm->get_solenoid(j));
				pcm->set_solenoid_black(j, s->IsBlackListed());
			}

			Solenoid *s = sol[0];
			pcm->set_fault_sol_volt(s->GetPCMSolenoidVoltageFault(modid));
			pcm->set_fault_sol_volt_sticky(s->GetPCMSolenoidVoltageStickyFault(modid));
			if (pcm->get_sol_sticky_clear_pending()) {
				s->ClearAllPCMStickyFaults(modid);
				pcm->set_sol_sticky_clear_pending(true);
			}

			pcm->set_comp_current(cmp->GetCompressorCurrent());
		}
		MTX_UNLOCK(mtx, i);
	}
}