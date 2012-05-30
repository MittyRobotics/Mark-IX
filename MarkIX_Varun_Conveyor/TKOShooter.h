#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "Definitions.h"

class TKOShooter {
public:
	TKOShooter(int port1, int port2);
	~TKOShooter();
	void RampToSpeed(int sp);
	bool IsUpToSpeed();
	void Shoot();
private:
	CANJaguar _j1, _j2;
	int _setpoint, _cur;
};

#endif
