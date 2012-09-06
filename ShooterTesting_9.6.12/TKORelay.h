#ifndef __TKORELAY_H
#define __TKORELAY_H
#include "Definitions.h"

class TKORelay {
public:
	TKORelay(int port);
	~TKORelay();
	void SetOn(int d);
	void Pulse();
private:
	Relay _relay;
	int _pulsing;
};

#endif
