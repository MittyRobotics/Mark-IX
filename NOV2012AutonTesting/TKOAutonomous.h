//Last edited by Vadim Korolik
//on 11/06/2012
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
	void autonomousCode();
	void startAutonomous();
	void stopAutonomous();
	void setDrivePID(float P, float I, float D);
	void setDriveTarget(float target);
	void PIDDriveStraight();
	void turnDriving(float leftPow, float rightPow);
	void driveLeft();
	void driveRight();
	void initAutonomous();
	float getPosition(int jaguar);
	bool runningAuton;
	Timer autonTimer;
private:
	CANJaguar drive1, drive2, drive3, drive4;
	DriverStation *ds;
	TKOIntake intake;
	TKOConveyor conveyor;
	TKOShooter shooter;
	float driveTarget;
};

#endif
