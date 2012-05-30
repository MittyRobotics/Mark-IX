#include "WPILib.h"
//#include "TKOShooter.h"
#include "TKORelay.h"
#include "Definitions.h"

// TODO: d1 and d2 need to be inverted

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
Joystick stick1, stick2, stick3;
//	TKOConveyor conveyor;
CANJaguar d1, d2, d3, d4, _roller, turret, _spinner1, _spinner2, _wrist1, _wrist2;
TKORelay r1, r2;
//TKOShooter shooter;
public:
// TODO: Set Jaguar ID's
	RobotDemo(void):
		stick1(STICK_1_PORT),
		stick2(2),
		stick3(3),
//		conveyor(),
		d1(1),
		d2(2),
		d3(3),
		d4(4),
		_roller(ROLLER_ID),
		turret(6),
		_spinner1(7),
		_spinner2(8),
		_wrist1(WRIST_1_ID),
		_wrist2(WRIST_2_ID),
		r1(LOWER_RELAY_PORT),
		r2(UPPER_RELAY_PORT)
//		shooter(SPINNER_1_ID, SPINNER_2_ID)
{
	d1.ChangeControlMode(CANJaguar::kPercentVbus);
	d2.ChangeControlMode(CANJaguar::kPercentVbus);
	d3.ChangeControlMode(CANJaguar::kPercentVbus);
	d4.ChangeControlMode(CANJaguar::kPercentVbus);
	d1.SetSafetyEnabled(true);
	d2.SetSafetyEnabled(true);
	d3.SetSafetyEnabled(true);
	d4.SetSafetyEnabled(true);
	_roller.SetSafetyEnabled(true);
	turret.SetSafetyEnabled(true);
	_spinner1.SetSafetyEnabled(true);
	_spinner2.SetSafetyEnabled(true);
	_wrist1.SetSafetyEnabled(true);
	_wrist2.SetSafetyEnabled(true);
}

void Autonomous(void) {}

void OperatorControl(void) {
	float setpoint = 0;
	while (IsOperatorControl()) {
		DSClear();
		Wait(0.005);
		float y2, y3;
		y2 = stick2.GetY();
		y3 = stick3.GetY();
		if (stick2.GetTrigger())
			y2 = -y2;
		if (stick3.GetTrigger())
			y3 = -y3;
		d1.Set(y2);
		d2.Set(y2);
		d3.Set(y3);
		d4.Set(y3);
		DSLog(1, "d1 = d2 = %f", y2);
		DSLog(2, "d3 = d4 = %f", y3);
		if (stick1.GetRawButton(6)){
			setpoint+= 50;
//			shooter.IncreaseSpeed(setpoint);
		}
		if (stick1.GetRawButton(7)){
			setpoint-=50;
			if(setpoint < 0)
				setpoint = 0;
//			shooter.DecreaseSpeed(setpoint);
		}
		if (stick1.GetRawButton(8)){
			setpoint = 0;
//			shooter.DecreaseSpeed(setpoint);
		}
//		shooter.Shoot();
		if (stick1.GetRawButton(2)){
			r1.On();
			r2.On();
//				_wrist1.Set(WRIST_DOWN);
//				_wrist2.Set(WRIST_DOWN);
//				_roller.Set(ROLLER_ON);
			_roller.Set(-.5);
		}
		if (stick1.GetRawButton(3)){
			r1.Off();
			r2.Off();
//				_wrist1.Set(WRIST_UP);
//				_wrist2.Set(WRIST_UP);
//				_roller.Set(ROLLER_OFF);
			_roller.Set(.5);
		}
		_wrist1.Set(stick1.GetY());
		_wrist2.Set(stick1.GetY());
		
	}
	}
};

START_ROBOT_CLASS(RobotDemo);

