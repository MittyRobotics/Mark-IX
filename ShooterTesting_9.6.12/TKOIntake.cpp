#include "TKOIntake.h"

TKOIntake::TKOIntake(int port1, int port2, int port3):
	_wrist1(port1, CANJaguar::kPercentVbus),
	_wrist2(port2, CANJaguar::kPercentVbus),
	_roller(port3, CANJaguar::kPercentVbus)			// TODO: which controlMode for CAN Bus?
{
//	_wrist1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
//	_wrist1.SetPID(0.5, 0.0, 0.0);
//	_wrist1.ConfigEncoderCodesPerRev(250);
}

TKOIntake::~TKOIntake() {}

void TKOIntake::WristMove(float y) {
	_wrist1.Set(y);
	_wrist2.Set(y);
}

void TKOIntake::RollerMove(bool trigger) {
	if (trigger)
		_roller.Set(ROLLER_ON);
	else
		_roller.Set(ROLLER_OFF);
}
