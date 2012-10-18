#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "Definitions.h"

///Code for the Spinner
/*!
Code for our basic spinner device which is
in charge of getting our motors up to speed.
Initial code written and class assembled by Tanay
Nathan.	
*/	
class TKOShooter {
public:
	TKOShooter(int port1, int port2);
	~TKOShooter();
	void IncreaseSpeed(float sp);
	void DecreaseSpeed(float sp);
	bool IsUpToSpeed();
	void Shoot();
	float GetSetpoint();
	float GetSpeed();
	void Reset();
	bool IsAlive();
	void ReadVoltage();
	float AverageSpeed();
	void ClearAverage();
	bool ReadyToFire();
	float GetJag1Speed();
	float GetJag2Speed();
private:
	vector <float> averagevalues;
	CANJaguar j1, j2;
	float setpoint, oldsetpoint, ramping;
	bool _ready;
	int counter;
	float error;
	float volts;  /**< bool opened. Whether the file is open or not. */
//	Timer _timer;
};

#endif
