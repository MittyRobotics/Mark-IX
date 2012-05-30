#include "TKOJoystick.h"

#define DSClear()				DriverStationLCD::GetInstance()->Clear(); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();

class RobotDemo : public SimpleRobot
{
	TKOJoystick stick;

public:
	RobotDemo(void):
		stick(1)
	{}
	
	void RobotInit(void) {
		printf("FRC Team 1351 (TKO)\r\nRunning %s\r\nProject compiled in Wind River Workbench on %s at %s\r\n", __FILE__, __DATE__, __TIME__);
	}
	
	void OperatorControl(void)
	{
		while (IsOperatorControl())
		{
			DSClear();
			DSLog(1, "%d", (stick.GetButton(2) == TKOJoystick::kPressed));
			Wait(0.1);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
