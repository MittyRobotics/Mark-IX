#include "WPILib.h"
//#include "TKOIntake.h"
#include "TKORelay.h"
#include "TKOShooter.h"
#include "Definitions.h"
//#include "TKOConveyor.h"

class Testing : public SimpleRobot {
	Joystick stick, stick1, stick2;
//	TKOConveyor conveyor;
	CANJaguar _wrist1, _wrist2, _roller, d1, d2, d3, d4;
	TKORelay r1, r2;
	TKOShooter shooter;
public:
	// TODO: Set Jaguar ID's
	Testing(void):
		stick(STICK_1_PORT),
		stick1(2),
		stick2(3),
		d1(1),
		d2(2),
		d3(3),
		d4(4),
//		conveyor(),
		_wrist1(WRIST_1_ID),
		_wrist2(WRIST_2_ID),
		_roller(ROLLER_ID),
		r1(LOWER_RELAY_PORT),
		r2(UPPER_RELAY_PORT),
		shooter(SPINNER_1_ID, SPINNER_2_ID)
	{}

	void Autonomous(void) {}

	void OperatorControl(void) {
		int setpoint = 0;
		d1.SetSafetyEnabled(true);
		d2.SetSafetyEnabled(true);
		d3.SetSafetyEnabled(true);
		d4.SetSafetyEnabled(true);
		while (IsOperatorControl()) {
			Wait(0.005);
			float y1 = stick1.GetY();
			float y2 = stick2.GetY();
			d1.Set(y1);
			d2.Set(y1);
			d3.Set(y2);
			d4.Set(y2);
			if (stick1.GetTrigger())
				y1 = -y1;
			if (stick2.GetTrigger())
				y2 = -y2;
			if (stick.GetRawButton(6)){
				setpoint+= 50;
				shooter.RampToSpeed(setpoint);
			}
			if (stick.GetRawButton(7)){
				setpoint-=50;
				if(setpoint < 0)
					setpoint = 0;
				shooter.RampToSpeed(setpoint);
			}
			if (stick.GetRawButton(8)){
				setpoint = 0;
				shooter.RampToSpeed(setpoint);
			}
			shooter.Shoot();
			if (stick.GetRawButton(2)){
				r1.On();
				r2.On();
//				_wrist1.Set(WRIST_DOWN);
//				_wrist2.Set(WRIST_DOWN);
//				_roller.Set(ROLLER_ON);
				_roller.Set(-.5);
			}
			if (stick.GetRawButton(3)){
				r1.Off();
				r2.Off();
//				_wrist1.Set(WRIST_UP);
//				_wrist2.Set(WRIST_UP);
//				_roller.Set(ROLLER_OFF);
				_roller.Set(.5);
			}
			_wrist1.Set(stick.GetY());
			_wrist2.Set(stick.GetY());
			
		}
	}
};

START_ROBOT_CLASS(Testing);
