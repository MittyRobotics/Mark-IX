#include "TKORelay.h"

TKORelay::TKORelay(int port):
	_relay(port),
	_on(false)
{
	Off();
}

TKORelay::~TKORelay() {}

void TKORelay::Off() {
	_relay.Set(Relay::kOff);
}

void TKORelay::On() {
	_relay.Set(Relay::kForward);
}

void TKORelay::Pulse() {
	if (_on)
		Off();
	else
		On();
	_on = !_on;
}
