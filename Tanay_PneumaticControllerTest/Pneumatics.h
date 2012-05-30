#ifndef __PNEUMATICS_H
#define __PNEUMATICS_H
#include "PneumaticController.h"
#include <vector>

class Pneumatics {
public:
	Pneumatics(int port1, int start, int end, int compressch, int compressp);
	~Pneumatics();
	void Extend(int num);
	void Retract(int num);
	PneumaticController GetPneumaticController(int i);
	Solenoid* GetSolenoid(int i, int i2);
	bool GetState(int i);
	void StopCompressor();
private:
	int max;
	vector <PneumaticController> *p;
	Compressor *compress;
};

#endif
