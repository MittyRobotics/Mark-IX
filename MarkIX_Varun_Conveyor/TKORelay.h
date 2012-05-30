#ifndef __TKORELAY_H
#define __TKORELAY_H

#include "Definitions.h"

class TKORelay {
public:
	TKORelay(int port);
	~TKORelay();
	void Off();
	void On();
	void Pulse();
private:
	Relay _relay;
	int _on;
};

#endif
