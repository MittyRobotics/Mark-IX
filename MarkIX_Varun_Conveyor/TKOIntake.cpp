#include "TKOIntake.h"

TKOIntake::TKOIntake():
	_wrist1(WRIST_1_ID, CANJaguar::kPercentVbus),
	_wrist2(WRIST_2_ID, CANJaguar::kPercentVbus),
	_roller(ROLLER_ID, CANJaguar::kPercentVbus)			// TODO: which controlMode for CAN Bus?
{
}

TKOIntake::~TKOIntake() {}

void TKOIntake::Enable() {
	// TODO: Manual mode
	_wrist1.Set(WRIST_DOWN);
	_wrist2.Set(WRIST_DOWN);
	_roller.Set(ROLLER_ON);
	_enabled = true;
}

void TKOIntake::Disable() {
	// TODO: Manual mode
	_wrist1.Set(WRIST_UP);
	_wrist2.Set(WRIST_UP);
	_roller.Set(ROLLER_OFF);
	_enabled = false;
}

bool TKOIntake::IsEnabled() {
	return _enabled;
}

void TKOIntake::Move(Joystick stick){
	if (stick.GetTrigger()){
		_roller.Set(0.7);
	}
	else 
		_roller.Set(0);
	float y = stick.GetY();
	_wrist1.Set(y);
	_wrist2.Set(y);
}
