#include <WPILib.h>
#include <Gyro.h>
#include <Timer.h>
#include <SpeedController.h>
#include "Definitions.h"
#include "TKOLogging.h"

/*
 Main Robot Code, Documented by Ritwik Dutta of Team 1351
 The main change from last year is the implementation of multithreaded 
 code. The code is partially taken from the MarkIX.cpp file,
 but a lot of it has also been reimplemented. 

 Code originally implemented by: 
 Ritwik Dutta, 2012, Team #1351

 Code modified by:
 Ritwik Dutta, 2012, Team #1351
 Vadim Korolik, 2013, Team #1351
 Ritwik Dutta, 2013, Team #1351

 TODO: Add processsing for the PID drive and create a PID Drive class


 */

class MarkX: public SimpleRobot
{

	Joystick stick1, stick2, stick3, stick4; // define joysticks
	CANJaguar drive1, drive2, drive3, drive4; // define motors
	DriverStation *ds; // define driver station object
	//RobotDrive TKODrive; 					  // define robot drive object
	TKOLogging* TKOLogger; // define robot logging object
	Gyro gyro;
	Timer timer;
	double* newValues[2];
	double* joystickValuesLeft[5];
	double* joystickValuesRight[5];

public:
	MarkX(void) :

		stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick 
				stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
				stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
				stick4(STICK_4_PORT), // initialize joystick 4 < first EVOM joystick

				drive1(DRIVE_L1_ID, CANJaguar::kSpeed), // initialize motor 1 < first left drive motor
				drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus), // initialize motor 2 < second left drive motor
				drive3(DRIVE_R1_ID, CANJaguar::kSpeed), // initialize motor 3 < first right drive motor
				drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus), // initialize motor 4 < second right drive motor


				/*TKODrive(&drive1, &drive2 Initialize drive system 
				 , &drive3, &drive4),  already initialized drive motors */

				gyro(1)

	{

		ds = DriverStation::GetInstance(); // Pulls driver station information 
		DSClear();
		drive1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
		drive1.ConfigEncoderCodesPerRev(250);
		drive1.SetPID(.2, .004, .0);
		drive1.SetSafetyEnabled(true);
		drive2.SetSafetyEnabled(drive1.IsSafetyEnabled());
		drive3.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
		drive3.ConfigEncoderCodesPerRev(250);
		drive3.SetPID(.2, .004, .0);
		drive3.SetSafetyEnabled(true);
		drive4.SetSafetyEnabled(drive3.IsSafetyEnabled());
		MainFunc();

	}
private:
	int driveToggle;
	int jukeNum;
	int movingAverageToggle;
	int burnoutIndexLeft;
	int burnoutIndexRight;
	int movingAverageIndex;
	double cs_str;
	double cs_rot;

	void JukeR(void)
	{
		printf("Juke manuever started\n");
		timer.Start();
		timer.Reset();
		while (timer.Get() < .6)
		{
			drive1.Set(-5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(-5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < .65)
		{
			drive1.Set(-5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < .3)
		{
			drive1.Set(-5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(-5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < .6)
		{
			drive1.Set(5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < 1)
		{
			drive3.Set(5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < .15)
		{
			drive1.Set(5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < 1.2)
		{
			drive1.Set(5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(-5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();

		timer.Stop();
	}

	void JukeL(void)
	{
		printf("Juke manuever started\n");
		timer.Start();
		timer.Reset();
		while (timer.Get() < .6)
		{
			drive1.Set(-5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(-5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < .65)
		{
			drive3.Set(-5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < .3)
		{
			drive1.Set(-5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(-5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < .6)
		{
			drive1.Set(5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < 1)
		{
			drive1.Set(5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < .15)
		{
			drive1.Set(5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();
		while (timer.Get() < 1.2)
		{
			drive1.Set(5310);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(-5310);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		timer.Reset();

		timer.Stop();
	}

	void MainFunc()
	{
		driveToggle = 1;
		cs_str = .5;
		cs_rot = .1;
		burnoutIndexLeft = 0;
		burnoutIndexRight = 0;

		joystickValuesLeft[0] = 0;
		joystickValuesLeft[1] = 0;
		joystickValuesLeft[2] = 0;
		joystickValuesLeft[3] = 0;
		joystickValuesLeft[4] = 0;

		movingAverageIndex = 0;
		movingAverageToggle = 1;

		while (true)
		{
			timer.Start();
			timer.Reset();
			if (IsAutonomous() and (IsEnabled()))
			{

				Autonomous();
			}

			if (IsOperatorControl() and (IsEnabled()))
			{

				OperatorControl();
			}
			Wait(.01 - timer.Get());
			timer.Stop();
			DSLog(6, "%f", timer.Get());
		}
	}

	void Autonomous()
	{
		DSClear();

		printf("Autonomous started\n");

		//Autonomous goes here
	}

	void TKOTankDrive()
	{
		//double left_joystick_x = stick1.GetX();
		double left_joystick_y = stick1.GetY();
		//double right_joystick_x = stick2.GetX();
		double right_joystick_y = stick2.GetY();
		joystickValuesLeft[movingAverageIndex] = &left_joystick_y;
		joystickValuesRight[movingAverageIndex] = &right_joystick_y;
		double final_velocity_left;
		double final_velocity_right;
		if (movingAverageToggle == 1)
		{
			left_joystick_y = ((*joystickValuesLeft[0] + *joystickValuesLeft[1]
					+ *joystickValuesLeft[2] + /*
			 */*joystickValuesLeft[3] + *joystickValuesLeft[4]) / 5);
			right_joystick_y = ((*joystickValuesRight[0]
					+ *joystickValuesRight[1] + *joystickValuesRight[2] + /*
			 */*joystickValuesRight[3] + *joystickValuesRight[4]) / 5);
		}
		/*if ((fabs(left_joystick_y) - fabs(right_joystick_y)) > .2 )
		 {*/

		if (fabs(left_joystick_y) > .2)
		{
			if ((left_joystick_y > 0.2 and right_joystick_y < 0.2)
					or (right_joystick_y > 0.2 and left_joystick_y < 0.2))
			{
				final_velocity_left = left_joystick_y * 5310 * cs_rot;
			}
			else
			{
				final_velocity_left = left_joystick_y * 5310 * cs_str;
			}
		}

		else
		{
			final_velocity_left = 0;
		}
		if (fabs(right_joystick_y) > .2)
		{
			if ((left_joystick_y > 0.2 and right_joystick_y < 0.2)
					or (right_joystick_y > 0.2 and left_joystick_y < 0.2))
			{
				final_velocity_right = right_joystick_y * 5310 * cs_rot;
			}
			else
			{
				final_velocity_right = right_joystick_y * 5310 * cs_str;
			}
		}
		else
		{
			final_velocity_right = 0;
		}
		if ((right_joystick_y >= 0 and left_joystick_y >= 0)
				or (right_joystick_y <= 0 and left_joystick_y <= 0))
		{
			if (fabs(fabs(left_joystick_y) - fabs(right_joystick_y)) < .1)
			{
				if (final_velocity_right < final_velocity_left)
				{
					final_velocity_left = final_velocity_right;
				}
				else
				{
					final_velocity_right = final_velocity_left;
				}
			}
		}
		/*}
		 else
		 {
		 final_velocity_left = 5310 * cs_str * ( (left_joystick_y+right_joystick_y)/2 );
		 DSLog(6, "%f", fabs(left_joystick_y) - fabs(right_joystick_y));
		 
		 }*/
		// final_velocity_left
		DSLog(1, "Joystick 1 y: %f", left_joystick_y);
		DSLog(2, "Joystick 2 y: %f", right_joystick_y);
		DSLog(3, "RPM Left: %f", final_velocity_left);
		DSLog(4, "RPM Right: %f", final_velocity_right);

		if ((final_velocity_left - drive3.GetSpeed()) > 1000)
		{
			burnoutIndexLeft++;
		}
		else
		{
			burnoutIndexLeft = 0;
		}

		if ((final_velocity_right - drive1.GetSpeed()) > 1000)
		{
			burnoutIndexRight++;
		}
		else
		{
			burnoutIndexRight = 0;
		}

		if (burnoutIndexLeft > 100)
		{
			final_velocity_left = 0;
		}

		if (burnoutIndexRight > 100)
		{
			final_velocity_right = 0;
		}

		// implement processing for left_joystick_x, left_joystick_y, right_joystick_x, and right_joystick_y to account for PID and other factors
		// then we pass these values to the SetLeftRightMotorsOutput() function of TKODrive
		drive1.Set(final_velocity_right);
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		drive3.Set(final_velocity_left);
		drive4.Set(-drive3.GetOutputVoltage() / drive3.GetBusVoltage());

		//TKODrive.SetLeftRightMotorOutputs(final_velocity_left, final_velocity_right);
	}

	void OperatorControl()
	{
		jukeNum = 0;
		if (movingAverageIndex > 4)
		{
			movingAverageIndex = 0;
		}
		else
		{
			movingAverageIndex++;
		}
		//TKODrive.SetSafetyEnabled(true);


		//TKODrive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		//TKODrive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);

		DSLog(1, "Operator");
		DSClear();

		if (driveToggle == 1)
		{
			tankDrive();
		}

		else if (driveToggle == 2)
		{
			arcadeDrive();
		}
		else if (driveToggle == 3)
		{
			TKOTankDrive();
		}

		if (stick2.GetRawButton(7))
		{
			driveToggle = 1;
			DSClear();
			DSLog(3, "Switching to arcade drive.");
		}

		if (stick2.GetRawButton(6))
		{
			driveToggle = 2;
			DSClear();
			DSLog(4, "Switching to tank drive.");
		}

		if (stick2.GetRawButton(8))
		{
			driveToggle = 3;
			DSClear();
			DSLog(5, "Switching to experimental PID drive.");
		}

		if (stick2.GetRawButton(4))
		{
			drive1.Set(0);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(0);
			drive4.Set(-drive3.GetOutputVoltage() / drive3.GetBusVoltage());
		}

		if (stick1.GetTrigger())
		{
			JukeL();
		}

		if (stick2.GetTrigger())
		{
			JukeR();
		}

		if (stick2.GetRawButton(9))
		{
			cs_str = 0 - cs_str;
			cs_rot = 0 - cs_rot;
			DSLog(6, "%f %f", cs_str, cs_rot);
		}

		if (stick2.GetRawButton(3))
		{
			if (movingAverageToggle == 1)
			{
				movingAverageToggle = 0;
			}
			else
			{
				movingAverageToggle = 1;
			}
		}

	}

	void arcadeDrive()
	{
		if (stick1.GetTrigger())
		{
			//TKODrive.ArcadeDrive(stick1.GetY() * .654, stick2.GetX() * .654);
		}

		if (!stick1.GetTrigger())
		{
			//TKODrive.ArcadeDrive(stick1.GetY() * .877, stick2.GetX() * .877);
		}

	}

	void tankDrive()
	{
		if (stick1.GetTrigger())
		{
			//TKODrive.TankDrive(stick1.GetY() * .654, stick2.GetY() * .654);
		}

		if (!stick1.GetTrigger())
		{
			//TKODrive.TankDrive(stick1.GetY() * .877, stick2.GetY() * .877);
		}
	}

	void AllLog(void)
	{

		DSLog(1, "Robot on and enabled.");

	}

};

START_ROBOT_CLASS( MarkX);
