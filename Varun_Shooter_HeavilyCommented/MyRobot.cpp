#include "Definitions.h"
#include "TKOShooter.h"
#include "TKOTurret.h"

class RobotDemo : public SimpleRobot
{
	Joystick *stick1;
	TKOShooter *shooter;
//	TKOTurret *turret;
	DriverStation *ds;
public:
	RobotDemo(void)//:
	{
		ds = DriverStation::GetInstance();
		shooter = new TKOShooter(7,8);
//		turret = new TKOTurret(6);
		stick1 = new Joystick(1);
	}
	
	void Autonomous(void) {}
	
	void OperatorControl(void)
	{
		while (IsOperatorControl())
		{
			DSClear();
			DSLog(1, "Commented!!");
			if (stick1->GetRawButton(2)) {
				shooter->ChangePercentage(.863);				// Sets the setpoint to 86.3% to shoot from the key
				shooter->Shoot();
			}
			else if (stick1->GetRawButton(5)) {
				DSLog(2, "Shooting");
				shooter->ChangePercentage(ds->GetAnalogIn(3));	// Use this for testing purposes
				shooter->Shoot();
			} else {
				DSLog(2, "Ramping");
				shooter->ChangePercentage(0.);					// Sets the setpoint to 0% (idling)
			}
			Wait(0.005);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
