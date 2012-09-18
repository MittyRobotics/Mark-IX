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
	float CalculateVelocity(float dist, float height);
	float GetSetpoint();
	float GetSpeed();
	void Reset();
	bool IsAlive();
	void ReadVoltage();
	void resetSetpoints();
private:
	CANJaguar j1, j2;
	float setpoint, oldsetpoint, ramping;
	bool _ready;
	int counter;
	float volts;
//	Timer _timer;
};

#endif
