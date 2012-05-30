#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "Definitions.h"

class TKOShooter {
public:
	TKOShooter(int port1, int port2);
	~TKOShooter();
	void IncreaseSpeed(float sp);
	void DecreaseSpeed(float sp);
	bool IsUpToSpeed();
	void Shoot();
private:
	CANJaguar j1, j2;
	float setpoint, oldsetpoint;
	bool _ready;
};

#endif
