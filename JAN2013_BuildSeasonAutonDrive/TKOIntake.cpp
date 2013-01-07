#include "TKOIntake.h"
///Constructor for the TKOIntake class
/*!
	Initializes the 2 wrist jaguars and the roller jaguar in percentVbus mode
	\param int port1 - wrist1 Jaguar ID
	\param int port2 - wrist2 Jaguar ID
	\param int port3 - roller Jaguar ID
*/
TKOIntake::TKOIntake(int port1, int port2, int port3):
	_wrist1(port1, CANJaguar::kPercentVbus),
	_wrist2(port2, CANJaguar::kPercentVbus),
	_roller(port3, CANJaguar::kPercentVbus)			// TODO: which controlMode for CAN Bus?
{
//	_wrist1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
//	_wrist1.SetPID(0.5, 0.0, 0.0);
//	_wrist1.ConfigEncoderCodesPerRev(250);
}
///Destructor for the TKOIntake class
TKOIntake::~TKOIntake() {}

///Moves the Wrist
/*!
	Sets the motors by percentVbus
	\param float y - Voltage that both wrist Jaguars are set to
*/
void TKOIntake::WristMove(float y) {
	_wrist1.Set(y);
	_wrist2.Set(y);
}

///Moves the Roller
/*!
	If the trigger is pulled, then the jaguars are set to ROLLER_ON (1.0), and is otherwise set to ROLLER_OFF (0.0) (Values are defined in definitions.h)
	\param bool trigger - true if trigger is depressed
*/

void TKOIntake::RollerMove(bool trigger) {
	if (trigger)
		_roller.Set(ROLLER_ON);
	else
		_roller.Set(ROLLER_OFF);
}
