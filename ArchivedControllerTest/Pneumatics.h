#ifndef __PNEUMATICS_H
#define __PNEUMATICS_H
#include "PneumaticController.h"
#include <vector>

class Pneumatics {
public:
	Pneumatics(int port1, int start, int end);
	~Pneumatics();
	void Extend(int num);
	void Retract(int num);
private:
	int max;
	vector <PneumaticController> *p;
	Compressor *compress;
};

#endif
