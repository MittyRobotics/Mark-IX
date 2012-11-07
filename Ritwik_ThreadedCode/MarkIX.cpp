#include "WPILib.h"
#include "Definitions.h"
#include "TKOShooter.h"
#include "TKOConveyor.h"
#include "TKOIntake.h"
#include "TKORelay.h"
#include "Task.h"

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
		//drive1.SetPID(30, 0, .1);
		drive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		drive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		drive1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
		drive1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		drive1.ConfigEncoderCodesPerRev(250);
//		drive1.SetPID(30, 0, .1);
		drive1.SetSafetyEnabled(true);
		ds = DriverStation::GetInstance();
		Task disabled_task("Disabled", Disabled);
		Task auton_task("Autonomous", Autonomous);
		Task op_control("Op_Control", OperatorControl);
		Task tank_dr("Tank_Drive", tankDrive);
		Task arcade_dr("Arcade Drive", arcadeDrive);
		Task oper("Operator", Operator);
		Task TKO_Tasks[6] = { disabled_task, auton_task, op_control, tank_dr, arcade_dr, open };
		
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
//		drive1.SetPID(1, SPINNER_kI, SPINNER_kD);
//		drive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
//		drive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
//		drive1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
//		drive1.ConfigEncoderCodesPerRev(250);
//		drive1.SetSafetyEnabled(true);
//		drive1.EnableControl(0);
		drive1.ChangeControlMode(CANJaguar::kPosition);
		drive2.ChangeControlMode(CANJaguar::kVoltage);
//		drive1.ConfigMaxOutputVoltage(k_driveMaxOutputVoltage);
		drive1.SetPID(30, 0, .1);											//TODO: think i solved the stupid autonomous problem, need to initialize again in position mode, need to test this theory
		drive1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		drive1.ConfigEncoderCodesPerRev(250);
		drive1.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		drive1.EnableControl();
		drive2.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		drive2.EnableControl();
		conveyor.EndAll();
		shooter.DecreaseSpeed(250);
		Timer *timer = new Timer();
//		drive1.ChangeControlMode(CANJaguar::kPosition);
		timer->Start();
		while(timer->Get() < 15){
			/*if (timer->Get() > ds->GetAnalogIn(1) && timer->Get() < 5) {
				shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(2));
				shooter.Shoot();
			}
			if (shooter.ReadyToFire() && shooter.IsAlive() && timer->Get() > 1)
				conveyor.OverrideAll();
			else
				conveyor.EndAll();
//			if (timer->Get() >= 7 && timer->Get() < 8)
//				drive.TankDrive(0.7, 0.7);
//			if (timer->Get() >= 9 && timer->Get() < 10)
//				drive.TankDrive(0.8, -0.8);*/
			if (timer->Get() >= 1)
			{
//				drive1.EnableControl();
				drive1.Set(100000);
				drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage()); //voltage
				//drive3.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
				//drive4.Set(drive1.GetOutputVoltage() / drive1.GetBusVoltage());
//				drive2.Set(2*METERREVS);
//				drive3.Set(2*METERREVS);
//				drive4.Set(2*METERREVS);
			}
			
			if ((int) timer->Get() % 50 == 0)
				shooter.Reset();
			if (!IsAutonomous())
				break;
			Wait(0.005);
			DSLog(4, "Drive1 speed: %f", drive1.GetSpeed());
			DSLog(5, "Drive1 pos: %f", drive1.GetPosition());
			DSLog(6, "ReadyToFire: %d", shooter.ReadyToFire());
		}
		conveyor.EndAll();
		shooter.DecreaseSpeed(250);
	}
	//! Operator Control Initialize and runs the Operator Control loop
	/*!
		Initializes drive motors, Prints number and location of balls and shooter's speed to DSLog
	*/
	void OperatorControl(void) {
		drive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		drive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
//		drive1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
//		drive1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
//		drive1.ConfigEncoderCodesPerRev(250);
//		drive1.SetSafetyEnabled(true);
//		drive1.ChangeControlMode(CANJaguar::kSpeed);
		conveyor.EndAll();
		shooter.DecreaseSpeed(250);
		int counter = 0;
		float average = 0;
		float total = 0;
		float sonarVoltage = 0;
		float sonarDistance = 0;
		float loopStartTime = 0;
		float loopEndTime = 0;
		float loopDuration = 0;
		float ticks = 0;
		Timer *timer = new Timer();
		bool timerStarted = false;
		usingTank = true;
		while (IsOperatorControl()){
			if (!timerStarted)
			{
				timer->Start();
				timerStarted = true;
			}
			else
				loopStartTime = timer->Get();
			DSLog(5, "Position: %f ", drive1.GetPosition());
			DSLog(6, "LoopDuration: %f", loopDuration);
			ticks += drive1.GetSpeed() / 60 / loopDuration; // if not set second to *
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
			loopEndTime = timer->Get();
			loopDuration = loopEndTime - loopStartTime;
		}
	}
	//! Driving and EVOM code
	/*!
	*/
	/*void moveDistance(double m){
		
	}*/
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

//START_ROBOT_CLASS(MarkIX);
for ( int i = 0 ; i < 6 ; i++ )
{
	TKO_Tasks[i].Start();
}
}

