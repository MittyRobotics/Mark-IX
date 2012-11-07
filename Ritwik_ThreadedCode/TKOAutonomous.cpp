#include "TKOAutonomous.h"

///Constructor for the TKOAutonomous class
/*!
This is the autonomous code for the robot.
*/

TKOAutonomous::TKOAutonomous(int port1, int port2):
	j1(port1, CANJaguar::kPosition),
	j2(port2, CANJaguar::kPercentVbus)
{
	j1.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	j1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	j1.ConfigEncoderCodesPerRev(250);
	j1.SetSafetyEnabled(true);
	j2.SetSafetyEnabled(true);
}

///Destructor for the TKOShooter class
TKOAutonomous::~TKOAutonomous() {}

void TKOAutonomous::Drive(float distMeter )
{
	
	j1.Set(1);
}

void TKOAutonomous::Rotate(int degrees)
{
}






