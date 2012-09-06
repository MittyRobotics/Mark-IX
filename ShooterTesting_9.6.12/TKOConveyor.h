#ifndef __TKOCONVEYOR_H
#define __TKOCONVEYOR_H

#include "Definitions.h"
#include "TKOIntake.h"
#include "TKORelay.h"
#include "TKOShooter.h"
#include "TKOLogging.h"
#include <vector>

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
