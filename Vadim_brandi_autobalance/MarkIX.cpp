#include "Definitions.h"
//#include "TKOJaguarController.h"
#include "TKOGyro.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	TKOGyro gyro;
//	RobotDrive myRobot;
	Joystick stick1, stick2, stick3;
	CANJaguar jag1, jag2, jag3, jag4;
//	Ultrasonic sonar;
	Compressor compressor;
	ADXL345_I2C accelerometer;
	DigitalInput digitalinput;
	PIDController balance1, balance2, balance3, balance4;
	
	float distance;
	bool autobalance;
	bool disableeverything;
	bool pauseenabled;
	bool pauseactive;
	bool gyroangleautoreset;

	ADXL345_I2C::AllAxes axes;


public:
	RobotDemo(void):
		gyro(1, 1),
//		myRobot(1, 2),	// these must be initialized in the same order
		stick1(1),
		stick2(2),		// as they are declared above.
		stick3(4),
//		sonar(1, 1),
		jag1(2, CANJaguar::kPercentVbus),
		jag2(3, CANJaguar::kPercentVbus),
		jag3(4, CANJaguar::kPercentVbus),
		jag4(5, CANJaguar::kPercentVbus),
		compressor(1, PRESSURE_SWITCH_PORT, 1, COMPRESSOR_RELAY_PORT),
		accelerometer(1),
		digitalinput (1, 1),
		balance1(-0.02, -0, 0.00195, &gyro, &jag1),
		balance2(-0.02, -0, 0.00195, &gyro, &jag2),
		balance3(0.02, 0, -0.00195, &gyro, &jag3),
		balance4(0.02, 0, -0.00195, &gyro, &jag4)
	{
//		myRobot.SetExpiration(0.1);
		compressor.Start();
	}
float CalculateVelocity(float dist, float height, float theta){
	float top = 386.1*dist*dist;		//in inches
	top *= ((tan(theta)))*(tan((theta))+1);
	float bot = 2 *(-height + (dist*tan(theta)));
	return sqrt(top/bot);
}
	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		//    for 2 seconds
		jag1.ChangeControlMode(CANJaguar::kSpeed);
		jag2.ChangeControlMode(CANJaguar::kSpeed);
		jag3.ChangeControlMode(CANJaguar::kSpeed);
		jag4.ChangeControlMode(CANJaguar::kSpeed);
		jag1.SetSpeedReference(CANJaguar::kSpeedRef_Encoder);
		jag3.SetSpeedReference(CANJaguar::kSpeedRef_Encoder);
		jag1.SetPID(4.0, 0.0, 0.0);
		jag2.SetPID(jag1.GetP(), jag1.GetI(), jag1.GetD());
		jag3.SetPID(4.0, 0.0, 0.0);
		jag4.SetPID(jag3.GetP(),jag3.GetI(), jag3.GetD());
		float y1 = 160;
		jag1.Set(y1);
		jag2.Set(y1);
		float y2 = 160;
		jag3.Set(y2);
		jag4.Set(y2);
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	float AnalyzeGyroAngle(TKOGyro &gyro)
	{
		float angle = gyro.GetAngle();
		while (angle > 360)
			angle -= 360;
		while (angle < 0)
			angle += 360;
		return angle;
	}
	void OperatorControl(void)
	{
		gyroangleautoreset = false;	
		autobalance = false;
		pauseactive = true;
		jag1.ChangeControlMode(CANJaguar::kPercentVbus);
		jag2.ChangeControlMode(CANJaguar::kPercentVbus);
		jag3.ChangeControlMode(CANJaguar::kPercentVbus);
		jag4.ChangeControlMode(CANJaguar::kPercentVbus);
		jag1.SetSafetyEnabled(true);
		jag2.SetSafetyEnabled(true);
		jag3.SetSafetyEnabled(true);
		jag4.SetSafetyEnabled(true);
		jag1.ConfigEncoderCodesPerRev(250);
		jag3.ConfigEncoderCodesPerRev(250);
		float p1 = balance1.GetP();
		float p2 = balance3.GetP();
		disableeverything = false;
//		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			if (disableeverything==false){
			axes = accelerometer.GetAccelerations();
			DSClear();
			if (stick1.GetTrigger() && autobalance == false)
				gyro.Reset();
			if (stick2.GetRawButton(5)){ //increases
				p1+=0.0001;
				p2-=0.0001;
				balance1.SetPID(p1,0,0);
				balance2.SetPID(p1,0,0);
				balance3.SetPID(p2,0,0);
				balance4.SetPID(p2,0,0);
			}
			if (stick2.GetRawButton(4)){ //decreases
				p1-=0.0001;
				p2+=0.0001;
				balance1.SetPID(p1,0,0);
				balance2.SetPID(p1,0,0);
				balance3.SetPID(p2,0,0);
				balance4.SetPID(p2,0,0);
			}
			
			if (stick1.GetRawButton(3) == true || autobalance == true){
				DSLog(1, "AUTOBALANCING!!!");
//				DSLog(1,"P VALUE BALANCE1: %f", p1);
//				DSLog(2,"P VALUE BALANCE3: %f", p2);
				AutoBalance();
			}
			
			if (stick2.GetRawButton(9) == true)
			{pauseactive = true;}
			if (stick3.GetRawButton(8) == true)
			{gyroangleautoreset = true;}
			if (stick3.GetRawButton(9) == true)
			{gyroangleautoreset = false;}
			if (stick2.GetRawButton(8) == true)
			{pauseactive = false;}
			if (stick1.GetRawButton(4) == true && autobalance == true){
				EndAutoBalance();
			}
			if (stick3.GetRawButton(1))
				disableeverything=true;
			if (autobalance == false){
				jag1.Set(-stick1.GetY());
				jag2.Set(-stick1.GetY());
				jag3.Set(stick2.GetY());
				jag4.Set(stick2.GetY());
				DSLog(1, "jag1 = jag2 = %f", jag1.Get());
				DSLog(2, "jag3 = jag4 = %f", jag3.Get());
				float angle2 = gyro.GetAngle();
				float gyroangle = gyro.GetAngle(); // - anglereducer;
//				anglereducer *= 2;
				DSLog(3, "Gyro Angle: %f", gyroangle);
				printf("Gyro Angle: %f \r\n", angle2);
				
//				bool buttontest = digitalinput.Get();
//				printf("Test Switch Stuff: %d \r\n", buttontest);  //Digital Sidecar Testing
//				DSLog(4, "Test Switch Stuff: %d \r\n", buttontest);
				pauseenabled = false;
//				if (gyroangleautoreset == true)
//				{
//				float tempgyroangle = gyro.GetAngle();
//							
//				float anglechange = tempgyroangle - angle2;
//							
//					if (anglechange/PAUSE_TIME <= abs(1))
//					{
//						gyro.Reset();
//					}
//				}			
				Wait(MAINLOOP_PAUSE);
			}
			}
			if (disableeverything==true){
				if(stick3.GetRawButton(2)==true)
					disableeverything = false;
					EndAutoBalance();
			}
			
		}
	}
	void AutoBalance(void){
		balance1.Enable();
		balance2.Enable();
		balance3.Enable();
		balance4.Enable();
		balance1.SetSetpoint(0);
		balance2.SetSetpoint(0);
		balance3.SetSetpoint(0);
		balance4.SetSetpoint(0);
		autobalance = true;
		float gyroangle2 = gyro.GetAngle();
		DSLog(2, "Gyro Angle : %f", gyro.GetAngle());
//		DSLog(4, "jag1 = jag2 = %f", jag1.Get());
//		DSLog(5, "jag3 = jag4 = %f", jag3.Get());
		if (gyroangle2 <= PAUSE_ANGLE && gyroangle2 >= -PAUSE_ANGLE && pauseactive == true)
		{ 
			int pauselooptime = int(PAUSE_TIME / MAINLOOP_PAUSE);
			for (int i = 0; i <= pauselooptime; i++)
			{
			Wait(MAINLOOP_PAUSE); 
			if (stick1.GetRawButton(4))
			{EndAutoBalance();}
			if (stick2.GetRawButton(8) == true)
			{pauseactive = false;}
			}
			pauseenabled = true;
		}
		if(pauseenabled == true)
		{
			DSLog(3, "Pause Enabled:%f", PAUSE_TIME);
		}
		
	}
	void EndAutoBalance(void){
		balance1.Disable();
		balance2.Disable();
		balance3.Disable();
		balance4.Disable();
		autobalance = false;		
	}
};

START_ROBOT_CLASS(RobotDemo);

