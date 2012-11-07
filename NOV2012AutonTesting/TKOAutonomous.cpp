//Last edited by Vadim Korolik
//on 11/06/2012
#include "TKOAutonomous.h"

///Constructor for the TKOAutonomous class

TKOAutonomous::TKOAutonomous(int port1, int port2, int port3, int port4) :
			drive1(port1, CANJaguar::kPosition),
			drive2(port2, CANJaguar::kPercentVbus),
			drive3(port3, CANJaguar::kPosition),
			drive4(port4, CANJaguar::kPercentVbus),
			intake(WRIST_1_ID, WRIST_2_ID, ROLLER_ID),
			conveyor(SWITCH_1_PORT, SWITCH_2_PORT, SWITCH_3_PORT, UPPER_RELAY_PORT,
					LOWER_RELAY_PORT), shooter(SPINNER_1_ID, SPINNER_2_ID)
{
	initAutonomous();
}
void TKOAutonomous::initAutonomous()
{
	drive1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	drive1.ConfigEncoderCodesPerRev(250);
	drive1.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive1.EnableControl();
	drive1.SetSafetyEnabled(true);

	drive3.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	drive3.ConfigEncoderCodesPerRev(250);
	drive3.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive3.EnableControl();
	drive3.SetSafetyEnabled(true);

	drive2.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive2.EnableControl();
	drive2.SetSafetyEnabled(true);

	drive4.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive4.EnableControl();
	drive4.SetSafetyEnabled(true);

	driveTarget = 0;
	runningAuton = false;
	printf("Initializing Autonomous");
}

void TKOAutonomous::startAutonomous()
{
	ds = DriverStation::GetInstance();
	autonTimer.Reset();
	autonTimer.Start();
	runningAuton = true;
	//runAutonomous();
	printf("Started Autonomous");
	DSLog(2, "Started Autonomous");
}
void TKOAutonomous::stopAutonomous()
{
	runningAuton = false;
	autonTimer.Reset();
	printf("Stopped Autonomous");
}
void TKOAutonomous::autonomousCode()
{
	if (ds->IsDisabled())
		stopAutonomous();
	if (!ds->IsAutonomous())
			stopAutonomous();
	if (autonTimer.Get() > ds->GetAnalogIn(1) && autonTimer.Get() < 5)
	{
		shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(2));
		shooter.Shoot();
	}
	if (shooter.ReadyToFire() && shooter.IsAlive() && autonTimer.Get() > 1)
		conveyor.OverrideAll();
	else
		conveyor.EndAll();

	if (autonTimer.Get() >= 1)
	{
		PIDDriveStraight(); //this is the driving part of code, in the PIDDrive function in this class
	}
	if ((int) autonTimer.Get() % 50 == 0)
		shooter.Reset();
	DSLog(1, "Running Auton: %i", runningAuton);
	DSLog(3, "Timer: %f", autonTimer.Get());
	DSLog(4, "Drive1 pos: %f", getPosition(1)); //uses the TKOAutonomous getPosition
	DSLog(5, "Drive3 pos: %f", getPosition(3)); //uses the TKOAutonomous getPosition
	DSLog(6, "ReadyToFire: %d", shooter.ReadyToFire());
}
void TKOAutonomous::driveLeft()
{
	drive1.Set(driveTarget); //sets pid drive target
	drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage()); 
}
void TKOAutonomous::driveRight()
{
	drive3.Set(driveTarget); //same, but for jag 3 since only 1 and 3 have encoders
	drive4.Set(-drive3.GetOutputVoltage() / drive3.GetBusVoltage()); //sets second and fourth jags in slave mode
}

void TKOAutonomous::turnDriving(float leftPow, float rightPow)
{
	if ((leftPow > -1 && leftPow < 1) && (rightPow > -1 && rightPow < 1))
	{
		drive1.Set(leftPow * driveTarget); //sets pid drive target
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage()); 
		drive3.Set(rightPow * driveTarget); //same, but for jag 3 since only 1 and 3 have encoders
		drive4.Set(-drive3.GetOutputVoltage() / drive3.GetBusVoltage()); //sets second and fourth jags in slave mode
	}
}

void TKOAutonomous::PIDDriveStraight()
{
	//driveLeft();
	driveRight();
}
void TKOAutonomous::setDrivePID(float P, float I, float D) //Sets drive1 and drive3 PID because only they have encoders
{
	drive1.SetPID(P, I, D);
	drive3.SetPID(P, I, D);
}
void TKOAutonomous::setDriveTarget(float target)
{
	driveTarget = -target;
}
float TKOAutonomous::getPosition(int jaguar) //1 and 3 are only ones with encoders
{
	if (jaguar == 1)
		return -drive1.GetPosition();
	else if (jaguar == 3)
		return -drive3.GetPosition();
	else
		return 13.51; //if it gives 13.51, then you entered wrong jaguar id
}

///Destructor for the TKOShooter class
TKOAutonomous::~TKOAutonomous()
{
}
