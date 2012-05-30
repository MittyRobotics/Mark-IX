#ifndef __TKOTURRET_H
#define __TKOTURRET_H

#include "Definitions.h"

/**
 * TKOTurret class is our basic Turret device.
 * It allows the user to automate the motion of the turret or control it manually.
 */
class TKOTurret {
public:
	TKOTurret(int port);
	~TKOTurret();
	void Move(float setpoint);
	void Automated (float setpoint);
	void Enable();
	void Disable();
	bool IsManual();
private:
	Gyro gyro;
	CANJaguar _turret;
	float setpoint;
	bool _manual;
};
#endif
