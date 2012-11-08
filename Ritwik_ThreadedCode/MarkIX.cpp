#include "WPILib.h"
#include "Definitions.h"
#include "Task.h"

/*
This code attempts to delete all the unnecessary extra code from the robot files and instead using Task object to outsource the processing to other files.
This code will modularize the robot code greatly.1
  */

class MarkIX : public SimpleRobot
/// Main Robot Code
/*
	Documented By Ritwik Dutta of Team 1351 */
{
	Joystick stick1, stick2, stick3, stick4;
	TKOIntake intake;
	TKOConveyor conveyor;
	CANJaguar drive1, drive2, drive3, drive4, turret;
	TKOShooter shooter;
	RobotDrive drive;
	DriverStation *ds;
	AnalogChannel sonar;
	bool usingTank;

}
