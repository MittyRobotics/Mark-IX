#ifndef __TKOJaguarController_H
#define __TKOJaguarController_H

#include "Definitions.h"
#include <stdarg.h>

class TKOJaguarController {
public:
	// For each ..., write the list of motor outputs, and remember to terminate the list with -1
	TKOJaguarController(int output, ...);
	~TKOJaguarController();
	void SetSpeed(float speed, int output, ...);
	void SetSpeedAll(float speed);
	void SetPID(float p, float i, float d, ...);				// TODO: they all have the same constants?
	void SetPIDAll(float p, float i, float d);

private:
	vector<Jaguar *> _jaguars;
	vector<int> _outputs;
	int _GetId(int output);
};

#endif
