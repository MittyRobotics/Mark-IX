#include "Definitions.h"
//#include "TKOJaguarController.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
//	Gyro gyro;
//	RobotDrive myRobot;
	Joystick stick1, stick2;
    AnalogChannel sonar;
//	ADXL345_I2C accelerometer;
//	PIDController balance1, balance2, balance3, balance4;
	float distance;
//	CANJaguar jag1, jag2, jag3, jag4;

	ADXL345_I2C::AllAxes axes;


public:
	RobotDemo(void):
//		gyro(1),
//		myRobot(1, 2),	// these must be initialized in the same order
		stick1(1),
		stick2(2),		// as they are declared above.
		sonar(1, 5)

	{
//		myRobot.SetExpiration(0.1);
//
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		printf ("autonomous");
		DSLog(1, "autonomous");
	}

	/**
	 * Runs the motors with arcade steering. 
	 */

	void OperatorControl(void)
	{
		while (IsOperatorControl())
		{
//			if (newmode == true)
//			{
//				if (doset == true)
//				{float oldVoltage = newVoltage;}
//				float newVoltage = sonar.GetVoltage();
//				if (abs(newVoltage - oldVoltage <= 1))
//				{
//					doset = false;
//					printf("Sonar Voltage: %f \r\n", oldVoltage);
//				}else
//				{
//					
//				}
//				if ()
//			}
			DSClear();
			float sonarVoltage = sonar.GetVoltage();
			float sonarDistance = sonarVoltage * 100; //inches
			DSLog(1, "Sonar Voltage: %f \r\n", sonarVoltage);
			DSLog(2, "Sonar Dist. in: %f \r\n", sonarDistance);
			DSLog(3, "Sonar Dist. ft: %f \r\n", sonarDistanceFeet);
			printf("Sonar Voltage: %f \r\n", sonarVoltage);
			printf("Sonar Distance: %f \r\n", sonarDistance);

			Wait(0.005);
		}
	}

};

START_ROBOT_CLASS(RobotDemo);

