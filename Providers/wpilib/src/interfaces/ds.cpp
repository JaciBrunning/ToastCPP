#include "thp/wpi/interfaces.hpp"
#include "thp/wpi/wpi_provider.hpp"

#include "toast/memory.hpp"

#include "DriverStation.h"
#include "Joystick.h"
#include "HAL/HAL.h"

using namespace Toast;

static Joystick *joys[6];

#define MIN(x,y)((x<y)?x:y)

void tick_itf_driverstation() {
	Concurrent::IPCMutex *mtx = Memory::shared_mutex()->ds;
	Memory::Shared::DS::DSInfo *info = Memory::shared()->ds_info();
	THP_Base_Robot *robot = get_robot();
	DriverStation *ds = robot->ds();
	
	MTX_LOCK(mtx, 0);
	info->set_ds_attached(ds->IsDSAttached());
	info->set_fms_attached(ds->IsFMSAttached());
	info->set_new_control_data(ds->IsNewControlData());
	info->set_system_active(ds->IsSysActive());
	info->set_match_time(ds->GetMatchTime());

	Memory::Shared::DS::Alliance alliance = Memory::Shared::DS::Alliance::Unknown;
	DriverStation::Alliance a = ds->GetAlliance();
	if (a == DriverStation::Alliance::kBlue)		alliance = Memory::Shared::DS::Alliance::Blue;
	else if (a == DriverStation::Alliance::kRed)	alliance = Memory::Shared::DS::Alliance::Red;
	info->set_alliance(alliance);

	info->set_alliance_station(ds->GetLocation());
	MTX_UNLOCK(mtx, 0);

	mtx = Memory::shared_mutex()->joy;
	for (int i = 0; i < 6; i++) {
		Memory::Shared::DS::Joystick *joy = Memory::shared()->joystick(i);
		MTX_LOCK(mtx, i);
		if (joy->get_init()) {
			if (!joy->get_bootstrap()) {
				joys[i] = new Joystick(i);
				joy->set_bootstrap(true);
			}

			Joystick *joyW = joys[i];
			int nAxis = joyW->GetAxisCount(), 
				nButt = joyW->GetButtonCount, // heh, butt
				nPOV = joyW->GetPOVCount();
			
			joy->set_num_axis(nAxis);
			joy->set_num_button(nButt);
			joy->set_num_pov(nPOV);

			HALJoystickButtons butts;
			HALGetJoystickButtons(i, &butts);

			joy->set_button_mask(butts.buttons);
			joyW->SetOutputs(joy->get_outputs_mask());

			for (int j = 0; j < MIN(nPOV, 4); j++) {
				joy->set_pov(j, joyW->GetPOV(j));
			}

			HALJoystickAxes axes;
			HALGetJoystickAxes(i, &axes);
			Memory::Shared::DS::JoystickDescriptor *desc = joy->get_descriptor();

			for (int j = 0; j < MIN(nAxis, 16); j++) {
				joy->set_axis(j, axes.axes[j]);
				desc->set_axis_type(j, (Memory::Shared::DS::JoystickAxisType)((int)(joyW->GetAxisType(j))));
			}

			joyW->SetRumble(Joystick::RumbleType::kLeftRumble, joy->get_rumble_l());
			joyW->SetRumble(Joystick::RumbleType::kRightRumble, joy->get_rumble_r());

			desc->set_is_xbox(joyW->GetIsXbox());
			desc->set_type((Memory::Shared::DS::JoystickType)((int)(joyW->GetType())));
			std::string name = joyW->GetName();
			desc->set_name_length(name.length());
			memcpy(desc->get_name(), (char *)name.c_str(), name.length());
			desc->set_axis_count(joyW->GetAxisCount());
		}
		MTX_UNLOCK(mtx, i);
	}
}