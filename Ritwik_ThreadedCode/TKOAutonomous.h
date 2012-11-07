#ifndef __TKOAUTONOMOUS_H
#define __TKOAUTONOMOUS_H

#include "Definitions.h"

///Code for the Spinner
/*!
Code for our basic spinner device which is
in charge of getting our motors up to speed.
Initial code written and class assembled by Tanay
Nathan.	
*/	
class TKOAutonomous {
public:
	TKOAutonomous(int port1, int port2);
	~TKOAutonomous();
	void PID();
	void Drive(float distMeter);
	void Rotate(int degrees);

private:
	float meters;
	int deg;
};

#endif
