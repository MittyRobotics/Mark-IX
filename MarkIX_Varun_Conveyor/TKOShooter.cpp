#include "TKOShooter.h"

TKOShooter::TKOShooter(int port1, int port2):
	_j1(port1),
	_j2(port2)
{
	_setpoint = 0;
	_cur = 0;
}

TKOShooter::~TKOShooter() {}

void TKOShooter::RampToSpeed(int sp) {
	_setpoint = sp;
	if (_cur < _setpoint) {
		_j1.Set(_cur);
		_j2.Set(_j1.GetOutputVoltage());
		_cur += 1;
	} else if (_cur > _setpoint) {
		_j1.Set(_cur);
		_j2.Set(_j1.GetOutputVoltage());
		_cur -= 1;
	}
}

bool TKOShooter::IsUpToSpeed() {
	return (_cur == _setpoint);
}

void TKOShooter::Shoot(){
	if (IsUpToSpeed()) {
		_j1.Set(_setpoint);
		_j2.Set(_j1.GetOutputVoltage());
	}	
	else
		return ;
}
