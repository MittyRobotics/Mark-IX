//Last edited by Vadim Korolik
//on 11/1/2012
#ifndef __TKOAUTONOMOUS_H
#define __TKOAUTONOMOUS_H

#include "Definitions.h"
#include "TKOShooter.h"
#include "TKOConveyor.h"
#include "TKOIntake.h"
#include "TKORelay.h"

class TKOAutonomous
{
public:
	TKOAutonomous(int port1, int port2, int port3, int port4);
	~TKOAutonomous();
	void runAutonomous();
	void AutonomousCode();
	void init();
private:
	CANJaguar drive1, drive2, drive3, drive4;
	DriverStation *ds;
	Timer timer;
	TKOIntake intake;
	TKOConveyor conveyor;
	TKOShooter shooter;
	bool runningAuton;
};

#endif
