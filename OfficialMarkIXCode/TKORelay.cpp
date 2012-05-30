#include "TKORelay.h"

TKORelay::TKORelay(int index):
	_relay(1, index)
{
	SetOn(false);
	_pulsing = 0;
}

TKORelay::~TKORelay() {}

void TKORelay::SetOn(int d) {
	if (d == 1)
		_relay.Set(Relay::kForward);
	else if (d == 2)
		_relay.Set(Relay::kReverse);
	else 
		_relay.Set(Relay::kOff);
}

void TKORelay::Pulse() {
	SetOn(_pulsing++ % 20 < 10);
}
