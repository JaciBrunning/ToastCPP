#include "thp/provider.hpp"
#include "thp/wpi/base_robot.hpp"

void THP_Base_Robot::RobotInit() {

}

void THP_Base_Robot::RobotPeriodic() {
    
}

void THP_Base_Robot::DisabledInit() {
    thp_state_call_init(true, false, false, false);
}

void THP_Base_Robot::AutonomousInit() {
    thp_state_call_init(false, true, false, false);
}

void THP_Base_Robot::TeleopInit() {
    thp_state_call_init(false, false, true, false);
}

void THP_Base_Robot::TestInit() {
    thp_state_call_init(false, false, false, true);
}

void THP_Base_Robot::DisabledPeriodic() {
    thp_state_call_periodic();
}

void THP_Base_Robot::AutonomousPeriodic() {
	thp_state_call_periodic();
}

void THP_Base_Robot::TeleopPeriodic() {
	thp_state_call_periodic();
}

void THP_Base_Robot::TestPeriodic() {
	thp_state_call_periodic();
}