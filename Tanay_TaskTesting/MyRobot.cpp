#include "WPILib.h"
#include "SemLib.h"
#include "DriveTrain.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class TaskTest : public SimpleRobot
{
	CANJaguar *d1, *d2, *d3, *d4;
	RobotDrive *drive; // robot drive system
	Joystick *stick1, *stick2; // only joysticks
	DriveTrain *_backup;
	Task *t_drive;
	SEM_ID _sync;

public:
	TaskTest(void)
	{
		d1 = new CANJaguar(1);
		d2 = new CANJaguar(2);
		d3 = new CANJaguar(3);
		d4 = new CANJaguar(4);
		drive = new RobotDrive(d1, d2, d3, d4);
		stick1 = new Joystick(1);
		stick2 = new Joystick(2);
		_backup = new DriveTrain(drive, stick1, stick2);
		t_drive = new Task("Drive", (FUNCPTR) _backup->Drive);
		t_drive->Start();
		_sync = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void) {
		t_drive->Suspend();
		Timer timer;
		timer.Reset();
		timer.Start();
		while(timer.Get() < 2.0) {
			drive->ArcadeDrive(0.5, 0.05);
			Wait (0.005);
		}
		drive->ArcadeDrive(0.0, 0.0);
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		drive->SetSafetyEnabled(true);
		t_drive->Restart();
		while (IsOperatorControl())
		{
			if (stick1->GetTrigger() && !t_drive->IsSuspended())
				t_drive->Suspend();
			if (t_drive->IsSuspended() && !stick1->GetTrigger())
				t_drive->Restart();
			if (stick1->GetRawButton(3))
				SwitchtoArcadeDrive();
			if (stick2->GetRawButton(3))
				SwitchtoTankDrive();
			Wait(0.001);				// wait for a motor update time
		}
	}
	~TaskTest() {
		delete d1;
		delete d2;
		delete d3;
		delete d4;
		delete drive;
		delete t_drive;
		delete _backup;
		delete stick1;
		delete stick2;
	}
	void Lock() {
		semTake(_sync, WAIT_FOREVER);
	}
	void Unlock() {
		semGive(_sync);
	}
	void SwitchtoArcadeDrive() {
		Lock();
		_backup->SetDrive(0);
		Unlock();
	}
	void SwitchtoTankDrive() {
		Lock();
		_backup->SetDrive(1);
		Unlock();
	}
};

START_ROBOT_CLASS(TaskTest);

