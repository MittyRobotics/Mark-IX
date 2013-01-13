//Last edited by Vadim Korolik
//on 11/13/2012

#include "Definitions.h"

/*
 Main Robot Code, Documented by Ritwik Dutta of Team 1351
 The main change from last year is the implementation of multithreaded 
 code. The code is partially taken from the MarkIX.cpp file,
 but a lot of it has also been reimplemented. 

 TODO: Implement autonomous code
 TODO: Implement EVOM code
 TODO: Test general multi-threaded code
 TODO: Create other general files
 TODO: Create camera analysis class

 */

class MarkX: public SimpleRobot
{

	Joystick stick1, stick2, stick3, stick4; // define joysticks
	CANJaguar drive1, drive2, drive3, drive4, turret;// define motors
	DriverStation *driverStationObj; // define driver station object
	TKOIntake intake;
	TKOConveyor conveyor;
	TKOShooter shooter;
	TKOAutonomous auton;
	Task *autonomousTask; // define autonomous task
	Task *operDriveTask; // define operator task
	Task *operEvomTask; // define EVOM task
	RobotDrive TKODrive; // define robot drive object

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
				intake(WRIST_1_ID, WRIST_2_ID, ROLLER_ID),
				conveyor(SWITCH_1_PORT, SWITCH_2_PORT, SWITCH_3_PORT, UPPER_RELAY_PORT,
						LOWER_RELAY_PORT), turret(TURRET_ID), shooter(SPINNER_1_ID, SPINNER_2_ID),
				auton(DRIVE_L1_ID, DRIVE_L2_ID, DRIVE_R1_ID, DRIVE_R2_ID),

				TKODrive(&drive1, &drive2/*Initialize drive system 
				 */, &drive3, &drive4) // already initialized drive motors

	{

		driverStationObj = DriverStation::GetInstance(); // Pulls driver station information 

		TKODrive.SetSafetyEnabled(true);
		TKODrive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		TKODrive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		
		
		/* 
		 Task creation is documented below, and it is necessary to 
		 run multiple operations simultaneously. 
		 
		 taskObject = new Task ( "Name", (FUNCPTR)(Robot::Function)());
		 ^           ^         ^                    ^
		 | Name of  | Create      |Name of      |Function pointer to       |
		 | the task | task object |task         |function that will be run |
		 
		 
		 taskObject->Start(); 
		 ^
		 | Starts the task object | 
		 */

		autonomousTask = new Task("AutonomousTask", (FUNCPTR) (MarkX::TaskAutonomous)());
		autonomousTask->Start();
		// Defines and starts Autonomous movement task
		// This will run the Autonomous robot movement

		operDriveTask = new Task("OperatorDriveTask", (FUNCPTR) (MarkX::OperatorDrive)());
		operDriveTask->Start();
		// Defines and starts Operator movement task
		// This will allow the operator to move the robot

		operEvomTask = new Task("OperatorEVOMTask", (FUNCPTR) (MarkX::OperatorEvom)());
		operEvomTask->Start();
		// Defines and starts OperatorEVOM Task
		// This will allow the operator to move the EVOM
	}

	/*The function name is different because
	 if you keep the default name, building returns an
	 error because the default function type cannot be 
	 overridden, and if their return types are void,
	 building returns another error. 
	 */
	int TaskAutonomous()
	{
		while (true) // Loop necessary to make sure that the robot does actually run 
		{
			DSLog(1, "Running Autonomous");
			if (driverStationObj->IsAutonomous()) // Making sure the robot doesn't run prematurely
			{
				auton.initAutonomous();
				auton.setDrivePID(50, 0.05, .01);
				auton.setDriveTargetStraight(REVS_PER_METER);
				auton.startAutonomous();

				while (auton.autonTimer.Get() < 15 && auton.runningAuton)
				{
					if (!driverStationObj->IsAutonomous())
						auton.stopAutonomous(&shooter, &conveyor);
					auton.autonomousCode(&shooter, &conveyor);
					Wait(0.005);
				}
				auton.stopAutonomous(&shooter, &conveyor);
			}
		}
		return 0;
	}

	// These are the default called functions, unused because they cannot be overridden.
	void Autonomous()
	{
	}
	void OperatorControl()
	{
	}

	void arcadeDrive() // Code to drive the robot using the arcade method 
	{ // This code was pulled from the MarkIX.cpp file
		if (stick1.GetTrigger()) // Slow movement
			TKODrive.ArcadeDrive(stick1.GetY() * .654, stick2.GetX() * .654);
		if (!stick1.GetTrigger()) // Fast movement
			TKODrive.ArcadeDrive(stick1.GetY() * .877, stick2.GetX() * .877);
	}
	void tankDrive()
	{
		if (!stick1.GetTrigger())
			TKODrive.TankDrive(stick1.GetY() * .877, stick2.GetY() * .877);
		if (stick1.GetTrigger())
			TKODrive.TankDrive(stick1.GetY() * .654, stick2.GetY() * .654);
	}

	int OperatorDrive(void) // This code is for the operator control of the 
	{ // robot's planar motion
		while (driverStationObj->IsOperatorControl())
		{
			TKODrive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
			TKODrive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
			conveyor.EndAll();
			shooter.DecreaseSpeed(250);
			usingTank = true;

			DSLog(5, "Position: %f ", auton.getPosition(1));
			Operator();

			Wait(.005);
		}
		return 0;
	}

	int OperatorEvom(void) // This code is for the operator control of the
	{ // robot's EVOM apparatus
		while (true)
		{
			int counter = 0;
			float average = 0;
			float total = 0;
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
		}
		return 0;
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
			shooter.IncreaseSpeed(4250 * driverStationObj->GetAnalogIn(4));
			shooter.Shoot();
		}
		else if (stick4.GetRawButton(4))
		{
			shooter.IncreaseSpeed(4250 * driverStationObj->GetAnalogIn(3));
			shooter.Shoot();
		}
		else if (stick4.GetRawButton(5))
		{
			shooter.IncreaseSpeed(4250 * driverStationObj->GetAnalogIn(2));
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

START_ROBOT_CLASS(MarkX)
; // Starts running the Robot Class

/* Code written by Ritwik Dutta, Daniel Braginsky, and Vadim Korolik, with
 help from Matt Pleva
 */
