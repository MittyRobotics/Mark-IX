#include "TKOShooter.h"

///Constructor for the TKOShooter class
/*!
	Initializes the Master spinner jaguar in Speed mode and the Slave spinner jaguar in percentVbus mode
	\param int port1 - Jaguar1's ID (Master in Speed Mode)
	\param int port2 - Jaguar2's ID (Slave in percentVbus Mode)
*/ 

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
	error = 0;
}

///Destructor for the TKOShooter class
TKOShooter::~TKOShooter() {}

///Increases TKOShooter speed
/*!
	Ramps up speed to the new setpoint (4250 max)
	\param float sp - New Setpoint in RPM
*/
void TKOShooter::IncreaseSpeed(float sp) {
	oldsetpoint = setpoint;
	setpoint = sp;
	if (setpoint > 4250)
		setpoint = 4250;
	if (setpoint > oldsetpoint){
		_ready = false;
		for (float i = oldsetpoint; i < setpoint; i+= 50){
			j1.Set(-i);  //J1 IS IN SPEED MODE
			
			j2.Set(j1.GetOutputVoltage() / j1.GetBusVoltage());  //J2 IS IN %VBUS
		}
		_ready = true;
		oldsetpoint = setpoint;
	}
	
	if (setpoint > 0 && setpoint < 1000)
		error = 50;
	else if (setpoint > 1000 & setpoint < 2500)
		error = 150;
	else
		error = 400;
}

///Decreases TKOShooter speed
/*!
	Ramps down speed to the new setpoint (0 min)
	\param float sp - New Setpoint in RPM
*/
float TKOShooter::AverageSpeed(){
	averagevalues.push_back(j1.GetSpeed());
	float avg = 0;
	for(int i = 0; i < averagevalues.size(); i++){
		avg += averagevalues.at(i);
	}
	avg /= averagevalues.size();
	return avg;
}
void TKOShooter::ClearAverage(){
	for(int i = 0; i < averagevalues.size(); i++){
		averagevalues.pop_back();
	}
}
bool TKOShooter::ReadyToFire() { //TODO: use setpoint and getspeed to determine whether or not it is up to speed
	//setpoint is in rpm, and getspeed is in rpm
	if(!(setpoint <= 250) && (-j1.GetSpeed() >= (setpoint - (error/2 + error/5))) && (-j1.GetSpeed() <= (setpoint + error/2)) )
		return true;
	return false;
}
float TKOShooter::GetJag1Speed() {
	return -j1.GetSpeed();
}
float TKOShooter::GetJag2Speed() {
	return -j2.GetSpeed();
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

///Checks if TKOShooter is up to speed
/*!
	Returns true if TKOShooter actual speed matches the setpoint (speed it is supposed to be at)
	\return bool - true if it is up to speed, false if not
	\return bool - true if it is up to speed, false if not
*/

bool TKOShooter::IsUpToSpeed() {
	if (_ready == true && oldsetpoint == setpoint && setpoint > .25 * 4250)
		return true;
	return false;
}

///Shoots the ball
/*!
	Sets jaguar1 to the setpoint (RPM) and sets jaguar2 to voltage of j1 divided by bus voltage at j1. Thus, if battery and therefore bus voltage is low, j1 and j2 output voltage and velocity will remain equal.
*/

void TKOShooter::Shoot(){
	volts = j1.GetOutputVoltage() / j1.GetBusVoltage();
	j1.Set(-setpoint);
	j2.Set(volts);	
}

///	Prints output voltage of j1 (Master Jaguar) to the DSLog
/*!
*/

void TKOShooter::ReadVoltage() {
	DSLog(5, "Voltage: %f", j1.GetOutputVoltage());
}

/// Gets the speed of j1 (Master Jaguar)
/*!
*/

float TKOShooter::GetSpeed() {
	return j1.GetSpeed();
}
double TKOShooter::GetPosition() {
	double a = j1.GetPosition();
	return a;
}

/// Returns the setpoint of the TKOShooter
/*!
	\return float setpoint - The setpoint which the spinner is trying to reach
*/

float TKOShooter::GetSetpoint() {
	return setpoint;
}

/// Resets the Jaguars
/*!
	Sets PID, SpeedReference to the QuadEncoder, EncoderCodes/Rev = 250, and Set Safety is enabled on both Jaguars
	 
	 
*/

void TKOShooter::Reset() {
	j1.SetPID(SPINNER_kP, SPINNER_kI, SPINNER_kD);
	j1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	j1.ConfigEncoderCodesPerRev(250);
	j1.SetSafetyEnabled(true);
	j2.SetSafetyEnabled(j1.IsSafetyEnabled());
}

///Checks if the TKOShooter's jaguars are alive
/*!
	 
	\return j1.IsAlive()
	\return j2.IsAlive()
*/

bool TKOShooter::IsAlive() {
	return j1.IsAlive() && j2.IsAlive();
}

