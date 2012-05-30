#include "TKOShooter.h"

/**
 * Constructor for TKOShooter with parameters for the two spinner ports.
 * 
 * Initializes the two internal CANJaguars in kPercentVbus mode.
 * 
 * @param port1 The first port
 * @param port2 The second port
 */
TKOShooter::TKOShooter(int port1, int port2):
	j1(port1, CANJaguar::kPercentVbus),
	j2(port2, CANJaguar::kPercentVbus)
{
//	j1.SetPID(SPINNER_kP, SPINNER_kI, SPINNER_kD);
//	j1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
//	j1.ConfigEncoderCodesPerRev(250);
	j1.SetSafetyEnabled(true);
	j2.SetSafetyEnabled(j1.IsSafetyEnabled());
}

/**
 * Destructor for TKOShooter.
 * 
 * Frees all resources associated with the TKOShooter.
 */
TKOShooter::~TKOShooter() {}

/**
 * Changes the percentage setpoint for the two spinner Jaguars.
 * The function blocks until the setpoint has been reached.
 * 
 * If the setpoint is above 0.95 or below 0., it is changed accordingly.
 * 
 * @param sp The desired percentage setpoint
 */
void TKOShooter::ChangePercentage(float sp) {
	oldsetpoint = setpoint;
	setpoint = sp;
	DSLog(3, "set: %f", setpoint);
	if (setpoint > 0.95)
		setpoint = 0.95;
	if (setpoint < 0.)
		setpoint = 0.;
	float change = (setpoint > oldsetpoint ? .000726 : -.0001);
	_ready = false;
	for (float i = oldsetpoint; i < setpoint; i += change){
		j1.Set(-i);
		j2.Set(-i);
	}
	_ready = true;
}

/**
 * Shoots the ball from the spinner.
 */
void TKOShooter::Shoot(){
	if (IsUpToSpeed()){
		j1.Set(-setpoint);
		j2.Set(-setpoint);
	}	
}

bool TKOShooter::IsUpToSpeed() {
	if (_ready == true && oldsetpoint == setpoint && setpoint > .3)
		return true;
	return false;
}
