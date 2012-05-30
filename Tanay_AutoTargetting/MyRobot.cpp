#include "WPILib.h"
#include "AutoTarget.h"
#include "TKOTurret.h"
#include "string"

#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class AutoTargetTest : public SimpleRobot
{
	CANJaguar *drive1, *drive2, *drive3, *drive4;
	RobotDrive myRobot; // robot drive system
	Joystick stick1, stick2; // only joystick
	AutoTarget *target;
	TKOTurret *turret;
	bool WasEnabled;

public:
	AutoTargetTest(void):
		myRobot(drive1, drive2, drive3, drive4),
		stick1(1),
		stick2(2)
	{
		drive1 = new CANJaguar(1);
		drive2 = new CANJaguar(2);
		drive3 = new CANJaguar(3);
		drive4 = new CANJaguar(4);
		target = new AutoTarget();
		turret = TKOTurret::GetInstance();
		myRobot.SetExpiration(0.1);
		WasEnabled = false;
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		myRobot.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		myRobot.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			myRobot.ArcadeDrive(stick1.GetY(), stick2.GetX()); // drive with arcade style (use right stick)
			target->SetStatus(stick1.GetTrigger());
			if (turret->GetStatus() == "Manual") {
				if (stick1.GetRawButton(2))
					turret->SetSpeed(0.3);
				if (stick2.GetRawButton(3))
					turret->SetSpeed(-0.3);
			}
			DSLog(1, "AutoTargetting status: %i", target->GetStatus());
			DSLog(2, "Turret status: %c", turret->GetStatus());
			Wait(0.005);				// wait for a motor update time
		}
		WasEnabled = true;
	}
	~AutoTargetTest() {
		target->SetStatus(false);
		delete drive1;
		delete drive2;
		delete drive3;
		delete drive4;
		delete target;
	}
};

START_ROBOT_CLASS(AutoTargetTest);

