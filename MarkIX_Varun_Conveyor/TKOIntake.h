#ifndef __TKOINTAKE_H
#define __TKOINTAKE_H

#include "Definitions.h"

class TKOIntake {
public:
	TKOIntake();
	~TKOIntake();
	void Enable();
	void Disable();
	bool IsEnabled();
	void Move(Joystick stick);
private:
	CANJaguar _wrist1, _wrist2;
	CANJaguar _roller;
	bool _enabled;
	bool _manual;
};

#endif
