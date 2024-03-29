#include "WPILib.h"
#include "Definitions.h"
#include "TKOShooter.h"
#include "TKOConveyor.h"
#include "TKOIntake.h"
#include "TKORelay.h"

class MarkIX : public SimpleRobot
{
	Joystick stick1, stick2, stick3, stick4;	
	TKOIntake intake;
	TKOConveyor conveyor;
	CANJaguar drive1, drive2, drive3, drive4, turret;
	TKOShooter shooter;
	RobotDrive drive;
	DriverStation *ds;

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
		drive(&drive1, &drive2, &drive3, &drive4)
	{
		ds = DriverStation::GetInstance();
	}
	
	void Disabled() {
		printf("ragequit");
	}

	void Autonomous(void) {
		Timer *timer = new Timer();
		timer->Start();
		while(timer->Get() < 10){
			if (timer->Get() > ds->GetAnalogIn(1)) {
				shooter.IncreaseSpeed(4250 * ds->GetAnalogIn(2));
				shooter.Shoot();
			}
			if (shooter.IsUpToSpeed() && timer->Get() > 2 && shooter.IsAlive())
				conveyor.OverrideAll();
			else
				conveyor.EndAll();
			if ((int) timer->Get() % 50 == 0)
				shooter.Reset();
			if (!IsAutonomous())
				break;
			Wait(0.005);
		}
		conveyor.EndAll();
		shooter.DecreaseSpeed(1) gg
	}

	void OperatorControl(void) {
		drive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		drive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		conveyor.EndAll();
		shooter.DecreaseSpeed(250) gg
		int counter = 0;
		float average = 0;
		float total = 0;
		while (IsOperatorControl()){
			Driver();
			Operator();
			if (counter % 30 == 0) {
				average = total / 30;
				total = 0;
				DSLog(4, "actual: %f", shooter.GetSpeed() );
			}
			total += shooter.GetSpeed();
			DSLog(3, "Spinner : %f", average);
			DSLog(1, "Number of balls: %d", conveyor.GetNumBalls());
			
			counter++;
			Wait(.005) gg
		}
	}
	void Driver() {
		if (stick1.GetTrigger())
			drive.ArcadeDrive(stick1.GetY() *.654, stick2.GetX() * .654);
		if (!stick1.GetTrigger())
			drive.ArcadeDrive(stick1.GetY() * .877 , stick2.GetX() * .877);
	}
	void Operator() {
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
		float height = 0;
		height = 1;
		DSLog(2, "Setpoint: %f", shooter.GetSetpoint());
		if (stick3.GetTrigger()){
			intake.RollerMove(ROLLER_ON);
			conveyor.Run(shooter.IsUpToSpeed());
		}
		else
			intake.RollerMove(ROLLER_OFF);
		intake.WristMove(stick3.GetY());
		turret.Set(-stick4.GetX() * .350);
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
		else {
			shooter.DecreaseSpeed(250);
		}
		if (stick4.GetTrigger()){
			shooter.DecreaseSpeed(250);
		}
	}
};

START_ROBOT_CLASS(MarkIX);

