#include <WPILib.h>
#include <Task.h>
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
	

class MarkX : public SimpleRobot
{
		
	Joystick stick1, stick2, stick3, stick4;	           // define joysticks
	CANJaguar drive1, drive2, drive3, drive4, evom1, evom2;// define motors
	DriverStation *driverStationObj;                  	   // define driver station object
	Task *autonomousTask;                             	   // define autonomous task
	Task *operDriveTask;                              	   // define operator task
	Task *operEvomTask;                               	   // define EVOM task
	RobotDrive TKODrive;                              	   // define robot drive object
		
public:
	MarkX(void): 
		
		
		
		stick1(STICK_1_PORT),   // initialize joystick 1 < first drive joystick 
		stick2(STICK_2_PORT),   // initialize joystick 2 < second drive joystick
		stick3(STICK_3_PORT),   // initialize joystick 3 < first EVOM joystick
		stick4(STICK_4_PORT),   // initialize joystick 4 < first EVOM joystick
		
		drive1(DRIVE_L1_ID),    // initialize motor 1 < first left drive motor
		drive2(DRIVE_L2_ID),    // initialize motor 2 < second left drive motor
		drive3(DRIVE_R1_ID),    // initialize motor 3 < first right drive motor
		drive4(DRIVE_R2_ID),    // initialize motor 4 < second right drive motor
		evom1(EVOM_1_ID),       // initialize motor 5 < first EVOM motor
		evom2(EVOM_2_ID),         // initialize motor 5 < first EVOM motor
		
		
		TKODrive(&drive1,&drive2/*Initialize drive system 
		*/,&drive3,&drive4) 	// already initialized drive motors
	{
		
		driverStationObj = DriverStation::GetInstance(); // Pulls driver station information 
		
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
		
		
		autonomousTask = new Task("AutonomousTask", (FUNCPTR)(MarkX::TaskAutonomous)());
		autonomousTask->Start();
		// Defines and starts Autonomous movement task
		// This will run the Autonomous robot movement
				
		operDriveTask = new Task("OperatorDriveTask", (FUNCPTR)(MarkX::OperatorDrive)());
		operDriveTask->Start();
		// Defines and starts Operator movement task
		// This will allow the operator to move the robot
		
		operEvomTask = new Task("OperatorEVOMTask", (FUNCPTR)(MarkX::OperatorEvom)());
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
		while (true ) // Loop necessary to make sure that the robot does actually run 
				{
					if (driverStationObj->IsAutonomous()) // Making sure the robot doesn't run prematurely
					{	
						// Autonomous code goes here
					}
				}
		return 0;
	}
	
	// These are the default called functions, unused because they cannot be overridden.
	void Autonomous(){}
	void OperatorControl(){}
	
	void arcadeDrive() // Code to drive the robot using the arcade method 
	{				   // This code was pulled from the MarkIX.cpp file
		if (stick1.GetTrigger())  // Slow movement
			TKODrive.ArcadeDrive(stick1.GetY() *.654, stick2.GetX() * .654);
		if (!stick1.GetTrigger()) // Fast movement
			TKODrive.ArcadeDrive(stick1.GetY() * .877 , stick2.GetX() * .877);
	}
	
	int OperatorDrive(void) // This code is for the operator control of the 
	{                       // robot's planar motion
		TKODrive.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			arcadeDrive(); // For now, the drive method is arcade only

		}
		return 0;
	}
	
	int OperatorEvom(void) // This code is for the operator control of the
	{					   // robot's EVOM apparatus
		while (true)
		{
			// Evom code goes here.
		}
		return 0;
	}
};

START_ROBOT_CLASS(MarkX); // Starts running the Robot Class

/* Code written by Ritwik Dutta, Daniel Braginsky, and Vadim Korolik, with
   help from Matt Pleva
*/
