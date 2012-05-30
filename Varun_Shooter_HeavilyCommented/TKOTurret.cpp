#include "TKOTurret.h"

/**
 * Constructor for TKOTurret with a parameter for the turret port. 
 * 
 * Initializes the internal CANJaguar in kPosition mode.
 * 
 * @param port The turret port
 */
TKOTurret::TKOTurret(int port):
	gyro(1),
	_turret(port, CANJaguar::kPosition)
{
	_turret.SetPID(150.0,.20,0.0);
	_turret.EnableControl(0);
	_turret.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_turret.ConfigEncoderCodesPerRev(360);
	_turret.ConfigSoftPositionLimits(-1.2, 1.2 );
	_manual = false;
	setpoint = 0;
}

/**
 * Destructor for TKOTurret.
 * 
 * Frees all resources associated with the TKOShooter.
 */
TKOTurret::~TKOTurret() {}

/**
 * Enables manual control for the turret.
 */
void TKOTurret::Enable() {
	_turret.EnableControl(_turret.GetPosition());
	_manual = false;
}

/**
 * Disables manual control for the turret.
 */
void TKOTurret::Disable() {
	_turret.DisableControl();
	_manual = true;
}

/**
 * Returns true if the turret is in manual mode, and false otherwise.
 */
bool TKOTurret::IsManual() {
	return _manual;
}

/**
 * Shoots the ball from the spinner.
 */
void TKOTurret::Automated(float setpoint) {
	this->setpoint += setpoint;
	if (!IsManual()){
		_turret.Set(this->setpoint);
	}
}

/**
 * If the turret is in manual mode, moves the turret to the setpoint if the turret.
 * If not, the setpoint is set, but the turret is not moved.
 * 
 * @param setpoint The desired setpoint
 */
void TKOTurret::Move(float setpoint) {
	setpoint/=3;
	this->setpoint += setpoint;
	if (IsManual())
		_turret.Set(this->setpoint);
}
