#ifndef __TKOCONVEYOR_H
#define __TKOCONVEYOR_H

#include "Definitions.h"
#include "TKOIntake.h"
#include "TKORelay.h"
#include "TKOShooter.h"
#include <vector>

class TKOConveyor {
public:
	TKOConveyor();
	~TKOConveyor();
	void Run(bool shootButton, bool canShoot);
	bool Stuff();								// True means enable intake, false means disable intake
private:
	typedef enum {three, three_two, two, two_one, one, shot} TKOBall;
	vector<TKOBall> _balls;
	DigitalInput _switch1, _switch2, _switch3;
	TKORelay _upper, _lower;			// Change to TKORelay, if necessary
};

#endif
