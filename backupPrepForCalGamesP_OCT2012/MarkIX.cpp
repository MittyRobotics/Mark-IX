#include "WPILib.h"
#include "Definitions.h"
#include "TKOShooter.h"
#include "TKOConveyor.h"
#include "TKOIntake.h"
#include "TKORelay.h"

class MarkIX : public SimpleRobot
/// Main Robot Code
/*!
	Documented By Daniel Braginsky of Team 1351
*/
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

public:
	MarkIX(void):
		stick1(STICK_1_PORT),
		stick2(STICK_2_PORT),
		stick3(STICK_3_PORT),
		stick4(STICK_4_PORT),
		intake(WRIST_1_ID, WRIST_2_ID, ROLLER_ID),
		conveyor(SWITCH_1_PORT, SWITCH_2_PORT, SWITCH_3_PORT, UPPER_RELAY_PORT, LOWER_RELAY_PORT),
		drive1(DRIVE_L1_ID),
		drive2(DRIVE_L2_ID),
		drive3(DRIVE_R1_ID),
		drive4(DRIVE_R2_ID),
		turret(TURRET_ID),
		shooter(SPINNER_1_ID, SPINNER_2_ID),
		drive(&drive1, &drive2, &drive3, &drive4),
		sonar(1, 8)
	{
		ds = DriverStation::GetInstance();
	}
	
	//! Notifies driver if robot is disabled. Prints "Robot Died!" to console if it is disabled
	/*!
	*/
	void Disabled() {
		printf("Robot Died!");
	}

	//! Autonomous code
	/*!
	*/
	void Autonomous(void) {
		Timer *timer = new Timer();
		timer->Start();
		while(timer->Get() < 10){
			if (timer->Get() > ds->GetAnalogIn(1) && timer->Get() < 7) {
				shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(4));
				shooter.Shoot();
			}
			if (shooter.ReadyToFire() && shooter.IsAlive() && timer->Get() > 1)
				conveyor.OverrideAll();
			else
				conveyor.EndAll();
			if (timer->Get() >= 7 && timer->Get() < 8)
				drive.TankDrive(.8, .8);
			if (timer->Get() >= 8 && timer->Get() < 9)
				drive.TankDrive(.8, 0);
			if (timer->Get() >= 9)
				drive.TankDrive(.8, .8);
			
			if ((int) timer->Get() % 50 == 0)
				shooter.Reset();
			if (!IsAutonomous())
				break;
			Wait(0.005);
			DSLog(6, "ReadyToFire: %d", shooter.ReadyToFire());
		}
		conveyor.EndAll();
		//shooter.DecreaseSpeed(250);
	}
	//! Operator Control Initialize and runs the Operator Control loop
	/*!
		Initializes drive motors, Prints number and location of balls and shooter's speed to DSLog
	*/
	void OperatorControl(void) {
		drive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		drive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		conveyor.EndAll();
		//shooter.DecreaseSpeed(250);
		int counter = 0;
		float average = 0;
		float total = 0;
		float sonarVoltage = 0;
		float sonarDistance = 0;
		usingTank = true;
		while (IsOperatorControl()){
			DSLog(5, "Sonar Dist. in: %f \r\n", sonarDistance);
			DSLog(6, "ReadyToFire: %d", shooter.ReadyToFire());
//			printf("Sonar Voltage: %f \r\n", sonarVoltage);
//			printf("Sonar Distance: %f \r\n", sonarDistance);
			printf("Spinner Jag 1: %f \r\n", shooter.GetJag1Speed());
			printf("Spinner Jag 2: %f \r\n", shooter.GetJag2Speed());
			Operator();
			if (counter % 30 == 0) {
				average = total / 30;
				total = 0;
				DSLog(4, "actual: %f", shooter.GetSpeed() );
//				sonarVoltage = sonar.GetVoltage();
//				sonarDistance = sonarVoltage * 100; //inches
			}
			total += shooter.GetSpeed();
			DSLog(3, "Spinner : %f", average);
			DSLog(1, "Number of balls: %d", conveyor.GetNumBalls());
			
			counter++;
			Wait(.005);
		}
	}
	//! Driving and EVOM code
	/*!
	*/
	void arcadeDrive() {
		if (stick1.GetTrigger())
			drive.ArcadeDrive(stick1.GetY() *.654, stick2.GetX() * .654);
		if (!stick1.GetTrigger())
			drive.ArcadeDrive(stick1.GetY() * .877 , stick2.GetX() * .877);
	}
	void tankDrive() {
		if (!stick1.GetTrigger())
			drive.TankDrive(stick1.GetY() * .877, stick2.GetY() * .877);
		if (stick1.GetTrigger())
			drive.TankDrive(stick1.GetY() * .654 , stick2.GetY() * .654);
	}
	void Operator() {
		//BEGIN DRIVE
		if (usingTank == true)
			tankDrive();
		if (!usingTank)
			arcadeDrive();
		
		//BEGIN EVOM
		if (stick3.GetRawButton(5) && !stick3.GetRawButton(3)) {
			conveyor.OverrideAll();			// Currently uses a DSLog(2)
		} 
		if (!stick3.GetRawButton(5) && stick3.GetRawButton(3)){
			conveyor.Run(shooter.IsUpToSpeed());
		}
		else if (stick3.GetRawButton(4))
			conveyor.Reverse();
		
		if (!stick3.GetRawButton(3) && !stick3.GetRawButton(5) && !stick3.GetRawButton(4)) {
			conveyor.EndAll();
		}	
		if(stick3.GetRawButton(2) && shooter.ReadyToFire()){
			conveyor.OverrideAll();
		}
		DSLog(2, "Setpoint: %f", shooter.GetSetpoint());
		if (stick3.GetTrigger()){
			intake.RollerMove(ROLLER_ON);
			conveyor.Run(shooter.IsUpToSpeed());
		}
		else
			intake.RollerMove(ROLLER_OFF);
		intake.WristMove(stick3.GetY());
		turret.Set(-stick4.GetX() * .350);
		
		if(stick2.GetRawButton(8)){
			DSLog(5, "Average: %f", shooter.AverageSpeed());
		}
		else
			shooter.ClearAverage();
		
		if (stick4.GetRawButton(3)) {
			shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(4));
			shooter.Shoot();
		}
		else if (stick4.GetRawButton(4)) {
				shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(3));
				shooter.Shoot();
		}
		else if (stick4.GetRawButton(5)) {
				shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(2));
				shooter.Shoot();
		}
		else if (stick2.GetRawButton(7)) {
			usingTank = false;
		}
		else if (stick2.GetRawButton(6)) {
			usingTank = true;
		}
		else {
			shooter.DecreaseSpeed(250);
		}
		if (stick4.GetTrigger()){
			shooter.DecreaseSpeed(250);
		}
	}
};

START_ROBOT_CLASS(MarkIX);

