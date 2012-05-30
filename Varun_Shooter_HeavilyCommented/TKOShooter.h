#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "Definitions.h"

/**
 * TKOShooter class is our basic Spinner device.
 * It allows the user to ramp the spinner to a particular setpoint and shoot at the desired time.
 */
class TKOShooter {
public:
	TKOShooter(int port1, int port2);
	~TKOShooter();
	void ChangePercentage(float sp);
	void Shoot();
private:
	bool IsUpToSpeed();
	CANJaguar j1, j2;
	float setpoint, oldsetpoint;
	bool _ready;
};

#endif
