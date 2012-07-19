#include "WPILib.h"
#include "TKOLogging.h"

class RobotDemo : public SimpleRobot
{
	Joystick stick;
	TKOLogging *logging;

public:
	RobotDemo(void):
		stick(1)
	{
		logging = TKOLogging::GetInstance();
	}
	
	void Autonomous(void) {}
	
	void OperatorControl(void)
	{
		while (IsOperatorControl()) {
			if (stick.GetTrigger()) {
				DSLog(1, "%d", logging->WriteToFile("Hello\r\n"));
			}
			if (stick.GetRawButton(3))
				FLog("%d %s %f", 2, "hello again", -3.14);
			Wait(0.005);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
