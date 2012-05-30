#ifndef __DRIVETRAIN_H
#define __DRIVETRAIN_H

#include "WPILib.h"

class DriveTrain {
public:
	DriveTrain();
	DriveTrain(RobotDrive *d, Joystick *s1, Joystick *s2);
	void SetDrive(int i);
	static void Drive(DriveTrain *_train);
	enum DriveType {kArcade, kTank};
	string GetDriveType();
private:
	RobotDrive *drive;
	Joystick *stick1, *stick2;
	DriveType Type;
};

#endif
