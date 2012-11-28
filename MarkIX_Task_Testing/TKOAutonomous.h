//Last edited by Vadim Korolik
//on 11/06/2012
#ifndef __TKOAUTONOMOUS_H
#define __TKOAUTONOMOUS_H

#include "Definitions.h"
#include "TKOShooter.h"
#include "TKOConveyor.h"

class TKOAutonomous
{
public:
	TKOAutonomous(int port1, int port2, int port3, int port4);
	~TKOAutonomous();
	void autonomousCode(TKOShooter* shooter, TKOConveyor* conveyor);
	void startAutonomous();
	void stopAutonomous(TKOShooter* shooter, TKOConveyor* conveyor);
	void setDrivePID(float P, float I, float D);
	void setDriveTargetStraight(float target);
	void setDriveTargetLeft(float target);
	void setDriveTargetRight(float target);
	void PIDDriveStraight();
	void driveLeft();
	void driveRight();
	void shooting(TKOShooter* shooter, TKOConveyor* conveyor);
	void initAutonomous();
	float getPosition(int jaguar);
	bool runningAuton;
	Timer autonTimer;
private:
	CANJaguar drive1, drive2, drive3, drive4;
	DriverStation *ds;
	float driveTargetLeft;
	float driveTargetRight;
};

#endif
