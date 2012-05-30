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
	bool autobalance;

	ADXL345_I2C::AllAxes axes;


public:
	RobotDemo(void):
//		gyro(1),
//		myRobot(1, 2),	// these must be initialized in the same order
		stick1(1),
		stick2(2),		// as they are declared above.
		sonar(1, 5)
//		jag1(2, CANJaguar::kPercentVbus),
//		jag2(3, CANJaguar::kPercentVbus),
//		jag3(4, CANJaguar::kPercentVbus),
//		jag4(5, CANJaguar::kPercentVbus)
//		accelerometer(1),
//		balance1(0.03, 0.0, 0.0, &gyro, &jag1),
//		balance2(0.03, 0.0, 0.0, &gyro, &jag2),
//		balance3(-0.03, 0.0, 0.0, &gyro, &jag3),
//		balance4(-0.03, 0.0, 0.0, &gyro, &jag4)
	{
//		myRobot.SetExpiration(0.1);
//
	}
//float CalculateVelocity(float dist, float height, float theta){
//	float top = 386.1*dist*dist;		//in inches
//	top *= ((tan(theta)))*(tan((theta))+1);
//	float bot = 2 *(-height + (dist*tan(theta)));
//	return sqrt(top/bot);
//}
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
//	float AnalyzeGyroAngle(Gyro &gyro)
//	{
//		float angle = gyro.GetAngle();
//		while (angle > 360)
//			angle -= 360;
//		while (angle < 0)
//			angle += 360;
//		return angle;
//	}
	void OperatorControl(void)
	{
//		autobalance = false;
//		jag1.ChangeControlMode(CANJaguar::kPercentVbus);
//		jag2.ChangeControlMode(CANJaguar::kPercentVbus);
//		jag3.ChangeControlMode(CANJaguar::kPercentVbus);
//		jag4.ChangeControlMode(CANJaguar::kPercentVbus);
//		jag1.SetSafetyEnabled(true);
//		jag2.SetSafetyEnabled(true);
//		jag3.SetSafetyEnabled(true);
//		jag4.SetSafetyEnabled(true);
//		jag1.ConfigEncoderCodesPerRev(250);
//		jag3.ConfigEncoderCodesPerRev(250);
//		myRobot.SetSafetyEnabled(true);
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
			float smallDistance = sonarDistance - 6;
			float sonarDistanceFeet = sonarDistance / 12; //feet
			DSLog(1, "Sonar Voltage: %f \r\n", sonarVoltage);
			DSLog(2, "Sonar Dist. in: %f \r\n", sonarDistance);
			DSLog(3, "Sonar Dist. ft: %f \r\n", sonarDistanceFeet);
			printf("Sonar Voltage: %f \r\n", sonarVoltage);
			printf("Sonar Distance: %f \r\n", sonarDistance);

			Wait(0.005);
		}
	}
//	void AutoBalance(void){
//		balance1.Enable();
//		balance2.Enable();
//		balance3.Enable();
//		balance4.Enable();
//		balance1.SetSetpoint(0);
//		balance2.SetSetpoint(0);
//		balance3.SetSetpoint(0);
//		balance4.SetSetpoint(0);
//		autobalance = true;
//		DSLog (3, "Gyro Angle : %f", gyro.GetAngle());	
//	}
//	void EndAutoBalance(void){
//		balance1.Disable();
//		balance2.Disable();
//		balance3.Disable();
//		balance4.Disable();
//		autobalance = false;		
//	}
};

START_ROBOT_CLASS(RobotDemo);

