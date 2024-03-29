#include "TKOShooter.h"

/**************************************************************
 * TKOShooter class that is our basic Spinner device which is *
 * in charge of getting our motors up to speed and calculating*
 * velocity. Initial code written and class assembled by Tanay*
 * Nathan.													  *
 **************************************************************/
//Most commenting done by Vadim Korolik
TKOShooter::TKOShooter(int port1, int port2):
	j1(port1, CANJaguar::kSpeed),							//mode that uses encoder speed
	j2(port2, CANJaguar::kPercentVbus)						//mode that uses percent of voltage setting
{
	j1.SetPID(SPINNER_kP, SPINNER_kI, SPINNER_kD);
	j1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);  //sets encoder to be pluged into jaguar 1
	j1.ConfigEncoderCodesPerRev(250);
	j1.EnableControl(j1.GetPosition()); //testing with this line
	j1.SetSafetyEnabled(true);
	j2.SetSafetyEnabled(j1.IsSafetyEnabled());
	oldsetpoint = 500;
	setpoint = 500;
	counter = 0;
	volts = 0;
}

TKOShooter::~TKOShooter() {}

/***************************************************
* Ramps shooter speed up to setpoint.
***************************************************/
void TKOShooter::IncreaseSpeed(float sp) {
	oldsetpoint = setpoint;
	setpoint = sp;
	if (setpoint > 4250)
		setpoint = 4250;
	if (setpoint > oldsetpoint){
		_ready = false;
		for (float i = oldsetpoint; i < setpoint; i+= 50){
			j1.Set(-i);											
			/**************************************************
			* As the motors are ramping up,
			* sets jag 1 to value of i, 
			* which encreases from previous setpoint to new one.
			***************************************************/
			j2.Set(j1.GetOutputVoltage() / j1.GetBusVoltage()); 
			/***************************************************
			* Takes the voltage that jag 1 is sending to motors, 
			* divides it by bus voltage so that output
			* is same if battery voltage drops, 
			* and sends it to motor.
			***************************************************/
		}
		_ready = true;
		oldsetpoint = setpoint;
	}
}
//Vadim's testing to set all setpoints back to default value of 250
void TKOShooter::resetSetpoints()
{
     setpoint = 250;
     oldsetpoint = 250;
}
/***************************************************
* Ramps shooter speed down to setpoint.
***************************************************/
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
			/***************************************************
			* Does the same as in IncreaseSpeed,
			* but i decreases by 5 here. 
			***************************************************/
		}
		_ready = true;
		oldsetpoint = setpoint;
	}
}
/***************************************************
* Checks to see if the shooter is spinning at the
* speed it should be going at. 
***************************************************/
bool TKOShooter::IsUpToSpeed() 
{
	if (_ready == true && oldsetpoint == setpoint && setpoint > .25 * 4250) 
		return true;
	return false;
}
//bool TKOShooter::IsUpToSpeed2()
//{
//	if (IsUpToSpeed && ( (j1.GetSpeed < (setpoint + setpoint * 0.1)) || (j1.GetSpeed() > (setpoint - setpoint * 0.1)) ))
//		return true;
//	return false;
//}
/***************************************************
* Does what for loops do in the
* IncreaseSpeed and DecreaseSpeed,
* but without modifying setpoints.
***************************************************/
void TKOShooter::Shoot(){
	volts = j1.GetOutputVoltage() / j1.GetBusVoltage();
	j1.Set(-setpoint);
	j2.Set(volts);
}
/***************************************************
* Math function that calvulates velocity
* needed to shoot ball at for certain
* distance and height.
***************************************************/
float TKOShooter::CalculateVelocity(float dist, float height) {	
	float v = sqrt((HALF_G * dist * dist * (TAN_SPINNER_THETA * TAN_SPINNER_THETA + 1))/ (dist * TAN_SPINNER_THETA - height));
	return v * 2 * PI * RADIUS_WHEELS;
}
//Returns voltage.
void TKOShooter::ReadVoltage() {
	DSLog(5, "Voltage: %f", j1.GetOutputVoltage());
}
//Returns speed.
float TKOShooter::GetSpeed() {
	return j1.GetSpeed();
}
//Returns setpoint.
float TKOShooter::GetSetpoint() {
	return setpoint;
}
/***************************************************
* Reinitializes all encoder settings .
***************************************************/
void TKOShooter::Reset() {
	j1.SetPID(SPINNER_kP, SPINNER_kI, SPINNER_kD);
	j1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	j1.ConfigEncoderCodesPerRev(250);
	j1.SetSafetyEnabled(true);
	j2.SetSafetyEnabled(j1.IsSafetyEnabled());
}

void TKOShooter::PrintEncoder()
{
	printf("Encoder position : %f\r\n", j1.GetSpeed());  //and this line for testing
}

bool TKOShooter::IsAlive() {
	return j1.IsAlive() && j2.IsAlive();
}
