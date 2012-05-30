#include "PneumaticController.h"

PneumaticController::PneumaticController(int port1, int port2, int port3) {
	s[0] = new Solenoid (port1, port2);
	s[1] = new Solenoid (port1, port3);
	s[0]->Set(false);
	s[1]->Set(false);
}

PneumaticController::~PneumaticController() {
	s[0] = 0;
	s[1] = 0;
}

void PneumaticController::Extend() {
	s[0]->Set(true);
	s[1]->Set(false);
}

void PneumaticController::Retract() {
	s[0]->Set(false);
	s[1]->Set(true);
}
