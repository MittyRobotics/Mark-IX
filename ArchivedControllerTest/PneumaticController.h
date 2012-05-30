#ifndef __PNEUMATICCONTROLLER_H
#define __PNEUMATICCONTROLLER_H

#include "WPILib.h"

class PneumaticController {
public:
	PneumaticController (int port1, int port2, int port3);
	~PneumaticController();
	void Extend();
	void Retract();
protected:
	Solenoid* s[2];
};

#endif
