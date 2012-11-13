//Last edited by Vadim Korolik
//on 11/06/2012
#include "WPILib.h"
#include "Definitions.h"
#include "TKOShooter.h"
#include "TKOConveyor.h"
#include "TKOIntake.h"
#include "TKORelay.h"
#include "TKOAutonomous.h"

class MarkIX: public SimpleRobot
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
	TKOAutonomous auton;
	DriverStation *ds;
	AnalogChannel sonar;
	bool usingTank;

public:
	MarkIX(void) :
				stick1(STICK_1_PORT),
				stick2(STICK_2_PORT),
				stick3(STICK_3_PORT),
				stick4(STICK_4_PORT),
				intake(WRIST_1_ID, WRIST_2_ID, ROLLER_ID),
				conveyor(SWITCH_1_PORT, SWITCH_2_PORT, SWITCH_3_PORT, UPPER_RELAY_PORT,
						LOWER_RELAY_PORT), drive1(DRIVE_L1_ID), drive2(DRIVE_L2_ID),
				drive3(DRIVE_R1_ID), drive4(DRIVE_R2_ID), turret(TURRET_ID),
				shooter(SPINNER_1_ID, SPINNER_2_ID), drive(&drive1, &drive2, &drive3, &drive4),
				auton(DRIVE_L1_ID, DRIVE_L2_ID, DRIVE_R1_ID, DRIVE_R2_ID), sonar(1, 8)
	{
		drive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		drive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		ds = DriverStation::GetInstance();
	}

	//! Notifies driver if robot is disabled. Prints "Robot Died!" to console if it is disabled
	/*!
	 */
	void Disabled()
	{
		printf("Robot Died!");
	}

	//! Autonomous code
	/*!
	 */
	void Autonomous(void)
	{
		auton.initAutonomous();
		auton.setDrivePID(50, 0.05, .01);
		auton.setDriveTargetStraight(REVS_PER_METER);
		auton.startAutonomous();

		while (auton.autonTimer.Get() < 15 && auton.runningAuton)
		{
			if (!ds->IsAutonomous())
				auton.stopAutonomous(&shooter, &conveyor);
			auton.autonomousCode(&shooter,&conveyor);
			Wait(0.005);
		}
		auton.stopAutonomous(&shooter, &conveyor);

	}

	
	
	
	//! Operator Control Initialize and runs the Operator Control loop
	/*!
	 Initializes drive motors, Prints number and location of balls and shooter's speed to DSLog
	 */

	void OperatorControl(void)
	{
		drive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		drive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		conveyor.EndAll();
		shooter.DecreaseSpeed(250);
		int counter = 0;
		float average = 0;
		float total = 0;
		float loopStartTime = 0;
		float loopEndTime = 0;
		float loopDuration = 0;
		Timer *timer = new Timer();
		bool timerStarted = false;
		usingTank = true;
		while (IsOperatorControl())
		{
			if (!timerStarted)
			{
				timer->Start();
				timerStarted = true;
			}
			else
				loopStartTime = timer->Get();
			DSLog(5, "Position: %f ", auton.getPosition(1));
			DSLog(6, "LoopDuration: %f", loopDuration);
			printf("Spinner Jag 1: %f \r\n", shooter.GetJag1Speed());
			printf("Spinner Jag 2: %f \r\n", shooter.GetJag2Speed());
			Operator();
			if (counter % 30 == 0)
			{
				average = total / 30;
				total = 0;
				DSLog(4, "Actual: %f", shooter.GetSpeed() );
			}
			total += shooter.GetSpeed();
			DSLog(3, "Spinner : %f", average);
			DSLog(1, "Number of balls: %d", conveyor.GetNumBalls());
			DSLog(2, "Setpoint: %f", shooter.GetSetpoint());

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
	void arcadeDrive()
	{
		if (stick1.GetTrigger())
			drive.ArcadeDrive(stick1.GetY() * .654, stick2.GetX() * .654);
		if (!stick1.GetTrigger())
			drive.ArcadeDrive(stick1.GetY() * .877, stick2.GetX() * .877);
	}
	void tankDrive()
	{
		if (!stick1.GetTrigger())
			drive.TankDrive(stick1.GetY() * .877, stick2.GetY() * .877);
		if (stick1.GetTrigger())
			drive.TankDrive(stick1.GetY() * .654, stick2.GetY() * .654);
	}
	void Operator()
	{
		//BEGIN DRIVE
		if (usingTank == true)
			tankDrive();
		if (!usingTank)
			arcadeDrive();

		//BEGIN EVOM
		if (stick3.GetRawButton(5) && !stick3.GetRawButton(3))
		{
			conveyor.OverrideAll(); // Currently uses a DSLog(2)
		}
		if (!stick3.GetRawButton(5) && stick3.GetRawButton(3))
		{
			conveyor.Run(shooter.IsUpToSpeed());
		}
		else if (stick3.GetRawButton(4))
			conveyor.Reverse();

		if (!stick3.GetRawButton(3) && !stick3.GetRawButton(5) && !stick3.GetRawButton(4))
		{
			conveyor.EndAll();
		}
		if (stick3.GetRawButton(2) && shooter.ReadyToFire())
		{
			conveyor.OverrideAll();
		}
		if (stick3.GetTrigger())
		{
			intake.RollerMove(ROLLER_ON);
			conveyor.Run(shooter.IsUpToSpeed());
		}
		else
			intake.RollerMove(ROLLER_OFF);
		intake.WristMove(stick3.GetY());
		turret.Set(-stick4.GetX() * .350);

		if (stick2.GetRawButton(8))
		{
			DSLog(5, "Average: %f", shooter.AverageSpeed());
		}
		else
			shooter.ClearAverage();

		if (stick4.GetRawButton(3))
		{
			shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(4));
			shooter.Shoot();
		}
		else if (stick4.GetRawButton(4))
		{
			shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(3));
			shooter.Shoot();
		}
		else if (stick4.GetRawButton(5))
		{
			shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(2));
			shooter.Shoot();
		}
		else if (stick2.GetRawButton(7))
		{
			usingTank = false;
		}
		else if (stick2.GetRawButton(6))
		{
			usingTank = true;
		}
		else
		{
			shooter.DecreaseSpeed(250);
		}
		if (stick4.GetTrigger())
		{
			shooter.DecreaseSpeed(250);
		}
	}
};

START_ROBOT_CLASS(MarkIX)
;

