#include "TKOShooter.h"

/**************************************************************
 * TKOShooter class that is our basic Spinner device which is *
 * in charge of getting our motors up to speed and calculating*
 * velocity. Initial code written and class assembled by Tanay*
 * Nathan.													  *
 **************************************************************/

TKOShooter::TKOShooter(int port1, int port2):
	j1(port1, CANJaguar::kSpeed),
	j2(port2, CANJaguar::kPercentVbus)
{
	j1.SetPID(SPINNER_kP, SPINNER_kI, SPINNER_kD);
	j1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	j1.ConfigEncoderCodesPerRev(250);
	j1.SetSafetyEnabled(true);
	j2.SetSafetyEnabled(j1.IsSafetyEnabled());
	oldsetpoint = 500;
	setpoint = 500;
	counter = 0;
	volts = 0;
}

TKOShooter::~TKOShooter() {}

void TKOShooter::IncreaseSpeed(float sp) {
	oldsetpoint = setpoint;
	setpoint = sp;
	if (setpoint > 4250)
		setpoint = 4250;
	if (setpoint > oldsetpoint){
		_ready = false;
		for (float i = oldsetpoint; i < setpoint; i+= 50){
			j1.Set(-i);
			j2.Set(j1.GetOutputVoltage() / j1.GetBusVoltage());
		}
		_ready = true;
		oldsetpoint = setpoint;
	}
}

void TKOShooter::resetSetpoints()
{
     setpoint = 0;
     oldsetpoint = 0;
}

void TKOShooter::DecreaseSpeed(float sp) {
	oldsetpoint = setpoint;
	setpoint = sp;
	if (setpoint < 0)
		setpoint = 0;
	if (oldsetpoint > setpoint){
		_ready = false;
		for (float i = oldsetpoint; i > setpoint; i-= 5){
			j1.Set(-i);
			j2.Set(j1.GetOutputVoltage() / j1.GetBusVoltage());
		}
		_ready = true;
		oldsetpoint = setpoint;
	}
}

bool TKOShooter::IsUpToSpeed() {
	if (_ready == true && oldsetpoint == setpoint && setpoint > .25 * 4250)
		return true;
	return false;
}
void TKOShooter::Shoot(){
	volts = j1.GetOutputVoltage() / j1.GetBusVoltage();
	j1.Set(-setpoint);
	j2.Set(volts);	
}

float TKOShooter::CalculateVelocity(float dist, float height) {	
	float v = sqrt((HALF_G * dist * dist * (TAN_SPINNER_THETA * TAN_SPINNER_THETA + 1))/ (dist * TAN_SPINNER_THETA - height));
	return v * 2 * PI * RADIUS_WHEELS;
}

void TKOShooter::ReadVoltage() {
	DSLog(5, "Voltage: %f", j1.GetOutputVoltage());
}
float TKOShooter::GetSpeed() {
	return j1.GetSpeed();
}

float TKOShooter::GetSetpoint() {
	return setpoint;
}

void TKOShooter::Reset() {
	j1.SetPID(SPINNER_kP, SPINNER_kI, SPINNER_kD);
	j1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	j1.ConfigEncoderCodesPerRev(250);
	j1.SetSafetyEnabled(true);
	j2.SetSafetyEnabled(j1.IsSafetyEnabled());
}

bool TKOShooter::IsAlive() {
	return j1.IsAlive() && j2.IsAlive();
}
