#ifndef __PNEUMATICCONTROLLER_H
#define __PNEUMATICCONTROLLER_H

#include "WPILib.h"
#include <vector>

class PneumaticController {
public:
	PneumaticController (int port1, int port2, int port3);
	~PneumaticController();
	void Extend();
	void Retract();
	Solenoid* GetSolenoid(int i);
	bool GetState();
protected:
	vector<Solenoid*> *s;
};

#endif
