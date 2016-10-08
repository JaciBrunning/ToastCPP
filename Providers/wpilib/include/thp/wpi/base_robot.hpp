#pragma once

#include "WPILib.h"

class THP_Base_Robot : public IterativeRobot {
public:
    virtual void RobotInit();
	virtual void RobotPeriodic();
	virtual void DisabledInit();
	virtual void DisabledPeriodic();
	virtual void AutonomousInit();
	virtual void AutonomousPeriodic();
	virtual void TeleopInit();
	virtual void TeleopPeriodic();
    virtual void TestInit();
	virtual void TestPeriodic();

	virtual DriverStation *ds() {
		return &m_ds;
	}
};