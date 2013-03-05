#include "Definitions.h"
#include <cmath>
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
    AnalogChannel sonar1, sonar2;
//	ADXL345_I2C accelerometer;
//	PIDController balance1, balance2, balance3, balance4;
	float distance;
	float robotWidth; 
	Timer TKOTimer;
//	CANJaguar jag1, jag2, jag3, jag4;

//	ADXL345_I2C::AllAxes axes;


public:
	RobotDemo(void):
//		gyro(1),
//		myRobot(1, 2),	// these must be initialized in the same order
		
		stick1(1),
		stick2(2),		// as they are declared above.
		
		sonar1(1, 1),
		sonar2(1, 7)

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
	void OperatorControl(void)
	{
		robotWidth = 18.75;
		while (IsOperatorControl())
		{
			DSClear();
			
			TKOTimer.Start();
			TKOTimer.Reset();
			
			float sonar1Voltage = sonar1.GetVoltage();
			float sonar1Distance = sonar1Voltage * 100;
			float sonar2Voltage = sonar2.GetVoltage();
			float sonar2Distance = sonar2Voltage * 100;
			float sonarAngle = asin((sonar1Distance-sonar2Distance)/robotWidth);
			
			DSLog(1, "Sonar 1 Distance: %f \r\n", sonar1Distance);
			DSLog(2, "Sonar 2 Distance: %f \r\n", sonar2Distance);
			DSLog(3, "Sonar angle: %f \r\n", sonarAngle);
			DSLog(4, "Sonar angle: %f \r\n", sonarAngle * 180 / 3.1415);
			
			TKOTimer.Stop(); 
			
			Wait(0.01 - TKOTimer.Get());
		}
	}
	void Test(void)
	{
		DSLog(6, "Test mode...");
	}

};

START_ROBOT_CLASS(RobotDemo);

