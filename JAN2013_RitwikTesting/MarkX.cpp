#include <WPILib.h>
#include <Task.h>
#include "Definitions.h"
#include "TKOLogging.h"
#include "TKOShooter.h"
#include "TKOConveyor.h"
#include "TKOIntake.h"
#include "TKORelay.h"
#include "TKOAutonomous.h"

/*
 Main Robot Code, Documented by Ritwik Dutta of Team 1351
 The main change from last year is the implementation of multithreaded 
 code. The code is partially taken from the MarkIX.cpp file,
 but a lot of it has also been reimplemented. 

 TODO: Implement autonomous code
 TODO: Implement EVOM code
 */

class MarkX: public SimpleRobot
{

	Joystick stick1, stick2, stick3, stick4; // define joysticks
	CANJaguar drive1, drive2, drive3, drive4, turret;// define motors
	DriverStation *ds; // define driver station object
	Task *mainTask; // define autonomous task
	RobotDrive TKODrive; // define robot drive object
	TKOLogging* TKOLogger;
	TKOIntake intake;
	TKOConveyor conveyor;
	TKOShooter shooter;
	TKOAutonomous auton;
public:
	MarkX(void) :

				stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick 
				stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
				stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
				stick4(STICK_4_PORT), // initialize joystick 4 < first EVOM joystick

				drive1(DRIVE_L1_ID), // initialize motor 1 < first left drive motor
				drive2(DRIVE_L2_ID), // initialize motor 2 < second left drive motor
				drive3(DRIVE_R1_ID), // initialize motor 3 < first right drive motor
				drive4(DRIVE_R2_ID), // initialize motor 4 < second right drive motor
				turret(TURRET_ID),
			
				TKODrive(&drive1, &drive2/*Initialize drive system 
				 */, &drive3, &drive4), // already initialized drive motors

				intake(WRIST_1_ID, WRIST_2_ID, ROLLER_ID),
				
				conveyor(SWITCH_1_PORT, SWITCH_2_PORT, SWITCH_3_PORT,
						UPPER_RELAY_PORT, LOWER_RELAY_PORT),
				shooter(SPINNER_1_ID, SPINNER_2_ID),
				auton(DRIVE_L1_ID, DRIVE_L2_ID, DRIVE_R1_ID, DRIVE_R2_ID)

	{

		ds = DriverStation::GetInstance(); // Pulls driver station information 

		DSClear(); //
		MainFunc();

	}
private:
	bool tankDriveToggle;

	void Juke(void)
	{
		printf("Juke manuever started\n");
		Timer *timer = new Timer();
		timer->Start();

		while (timer->Get() < 0.0)
		{
			TKODrive.Drive(0.0, 0.0);
		}

		timer->Reset();
		timer->Start();

		while (timer->Get() < 0.0)
		{

			TKODrive.Drive(0.0, 0.0);

		}
	}

	void MainFunc()
	{
		tankDriveToggle = true;
		while (true)
		{
			if (IsAutonomous() && ds->IsEnabled() and auton.autonTimer.Get() < 30)
			{

				Autonomous();
			}

			if (IsOperatorControl() && ds->IsEnabled())
			{

				OperatorControl();
			}
			Wait(0.001);
		}
	}

	void Autonomous()
	{
		DSClear();

		printf("Autonomous started\n");

		auton.initAutonomous();
		auton.setDrivePID(20, 0.05, 0.01); 
		auton.startAutonomous();

		intake.WristMove(1);

		while (auton.autonTimer.Get() < 30 && auton.runningAuton)
		{
			if (!ds->IsAutonomous())
				auton.stopAutonomous(&shooter, &conveyor);
			if (auton.autonTimer.Get() > 5)
				conveyor.EndAll();

			auton.autonomousCode(&shooter, &conveyor);
			Wait(0.005);
		}
		auton.stopAutonomous(&shooter, &conveyor);
}

void OperatorControl()
{
	TKODrive.SetSafetyEnabled(true);
	TKODrive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
	TKODrive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);

	DSLog(1, "Jag 1 Pos: %f ", auton.getPosition(1));
	DSLog(2, "Gyro ang: %f", auton.getGyroAngle());
	printf( "Jag 1 Position: %f ", auton.getPosition(1));
	printf("\n");

	if (tankDriveToggle)
	{
		tankDrive();
	}

	else
	{
		arcadeDrive();
	}

	if (stick2.GetRawButton(7))
	tankDriveToggle = false;
	if (stick2.GetRawButton(6))
	tankDriveToggle = true;

	if (stick2.GetRawButton(3))
	{
		Juke();
	}

	if (stick2.GetRawButton(4))
	{
		drive1.StopMotor();
		drive2.StopMotor();
		drive3.StopMotor();
		drive4.StopMotor();
	}
	Operator();
}

void arcadeDrive()
{
	if (stick1.GetTrigger())
	{
		TKODrive.ArcadeDrive(stick1.GetY() * .654, stick2.GetX() * .654);
	}

	if (!stick1.GetTrigger())
	{
		TKODrive.ArcadeDrive(stick1.GetY() * .877, stick2.GetX() * .877);
	}
}

void tankDrive()
{
	if (stick1.GetTrigger())
	TKODrive.TankDrive(stick1.GetY() * .654, stick2.GetY() * .654);

	if (!stick1.GetTrigger())
	TKODrive.TankDrive(stick1.GetY() * .877, stick2.GetY() * .877);
}
void Operator()
{
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

	if (!stick3.GetRawButton(3) && !stick3.GetRawButton(5)
			&& !stick3.GetRawButton(4))
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
	else
	{
		shooter.DecreaseSpeed(250);
	}
	if (stick4.GetTrigger())
	{
		shooter.DecreaseSpeed(250);
	}
}

int AllLog(void)
{

	DSLog(1, "Robot on and enabled.");

	return 0;
}

};

START_ROBOT_CLASS(MarkX)
;

