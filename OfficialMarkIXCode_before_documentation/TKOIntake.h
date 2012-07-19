#ifndef __TKOINTAKE_H
#define __TKOINTAKE_H

#include "Definitions.h"

class TKOIntake {
public:
	TKOIntake(int port1, int port2, int port3);
	~TKOIntake();
	void WristMove(float y);
	void RollerMove(bool trigger);
private:
	CANJaguar _wrist1, _wrist2;
	CANJaguar _roller;
};

#endif
