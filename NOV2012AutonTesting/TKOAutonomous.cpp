//Last edited by Vadim Korolik
//on 11/1/2012
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
	drive1.SetPID(30, 0, .1);
	drive1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	drive1.ConfigEncoderCodesPerRev(250);
	drive1.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive1.EnableControl();
	drive2.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive2.EnableControl();
	drive1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	drive1.ConfigEncoderCodesPerRev(250);
	drive1.SetSafetyEnabled(true);
}

void TKOAutonomous::init()
{
	ds = DriverStation::GetInstance();
	Timer *timer = new Timer();
	timer->Start();
	runningAuton = true;
}
void TKOAutonomous::runAutonomous()
{
	while (timer.Get() < 15 && runningAuton)
	{
		AutonomousCode();
		Wait(0.005);
	}
}
void TKOAutonomous::AutonomousCode() //TODO Add stop atonomous
{
	if (timer.Get() > ds->GetAnalogIn(1) && timer.Get() < 5) {
	 shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(2));
	 shooter.Shoot();
	 }
	 if (shooter.ReadyToFire() && shooter.IsAlive() && timer.Get() > 1)
	 conveyor.OverrideAll();
	 else
	 conveyor.EndAll();
	/*if (timer->Get() >= 7 && timer->Get() < 8)  //Old timed moving
		drive.TankDrive(0.7, 0.7);
	if (timer->Get() >= 9 && timer->Get() < 10)
		drive.TankDrive(0.8, -0.8);*/
	if (timer.Get() >= 1)
	{
		drive1.Set(100000);
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage()); //voltage
	}

	if ((int) timer.Get() % 50 == 0)
		shooter.Reset();
	DSLog(4, "Drive1 speed: %f", drive1.GetSpeed());
	DSLog(5, "Drive1 pos: %f", drive1.GetPosition());
	DSLog(6, "ReadyToFire: %d", shooter.ReadyToFire());
}

///Destructor for the TKOShooter class
TKOAutonomous::~TKOAutonomous()
{
}
