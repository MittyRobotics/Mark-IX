#ifndef __TKOCONVEYOR_H
#define __TKOCONVEYOR_H

#include "Definitions.h"
#include "TKOIntake.h"
#include "TKORelay.h"
#include "TKOShooter.h"
#include "TKOLogging.h"
#include <vector>

///Code for the Conveyor
/*!
	(TODO: implement singleton model)
	The TKOConveyor class represents the lower conveyor and the upper conveyor.
	Values from a limit switch at the base of the lower conveyor, a limit switch between the two conveyors,
	and a limit switch at the top of the upper conveyor form the basis of the state system,
	which turns the conveyors forward or off automatically to move balls up into specific spots for shooting.
	The TKOConveyor class also provides functions for manual control of the conveyors.
*/

class TKOConveyor {
public:
	TKOConveyor(int port1 = 3, int port2 = 2, int port3 = 1, int port4 = 2, int port5 = 1);
	~TKOConveyor();
	void Run(bool canShoot);
	void OverrideAll();
	bool CanEnableIntake();								// True means enable intake, false means disable intake
	void EndAll();
	void Reverse();
	int GetNumBalls();
private:
	typedef enum {three, three_two, two, two_one, one, shot} TKOBall;
	vector<TKOBall> _balls;
	DigitalInput _switch1, _switch2, _switch3;
	TKORelay _upper, _lower;			// Change to TKORelay, if necessary
};

#endif
