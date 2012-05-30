#include "TKOShooter.h"

TKOShooter::TKOShooter(int port1, int port2):
	j1(port1),
	j2(port2)
{
	j1.SetSafetyEnabled(true);
	j2.SetSafetyEnabled(true);
}

TKOShooter::~TKOShooter() {}

void TKOShooter::IncreaseSpeed(float sp) {
	oldsetpoint = setpoint;
	setpoint = sp;
	if (setpoint > oldsetpoint){
		_ready = false;
		for (float i = oldsetpoint; i < setpoint; i+= 1){
			j1.Set(i);
			j2.Set(j1.GetOutputVoltage());
		}
		_ready = true;
	}
}

void TKOShooter::DecreaseSpeed(float sp) {
	oldsetpoint = setpoint;
	setpoint = sp;
	if (oldsetpoint > setpoint){
		_ready = false;
		for (float i = oldsetpoint; i > setpoint; i-= 1){
			j1.Set(i);
			j2.Set(j2.GetOutputVoltage());
		}
		_ready = true;
	}
}

bool TKOShooter::IsUpToSpeed() {
	if (_ready == true)
		return true;
	return false;
}
void TKOShooter::Shoot(){
	if (IsUpToSpeed()){
		j1.Set(setpoint);
		j2.Set(j1.GetOutputVoltage());
	}	
	else
		return ;
}
