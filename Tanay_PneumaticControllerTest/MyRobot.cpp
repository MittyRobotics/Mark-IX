#include "Definitions.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class PneumaticTest : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	Pneumatics pn;

public:
	PneumaticTest(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		pn(PNEUM_CHANNEL, PNEUM_STARTPORT, PNEUM_ENDPORT, PNEUM_COMPRESS_CH, PNEUM_COMPRESS)
	{
		myRobot.SetExpiration(0.1);
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
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			if (stick.GetTrigger()) {
				pn.Extend(PNEUM_CATA);
			}
			else {
				pn.Retract(PNEUM_CATA);
			}
			Wait(0.005);				// wait for a motor update time
		}
	}
	void Disabled() {
		pn.~Pneumatics();
	}
};

START_ROBOT_CLASS(PneumaticTest);

