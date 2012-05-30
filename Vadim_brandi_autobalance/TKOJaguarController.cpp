#include "TKOJaguarController.h"

TKOJaguarController::TKOJaguarController(int output, ...)
{
	va_list args;
	va_start(args, output);
	int i = MIN_JAGUAR_ID;
	int x = output;
	do {
		_outputs.push_back(x);
		Jaguar *jag = new Jaguar(i);
		jag->Set(0.0, 0x80);
		_jaguars.push_back(jag);
		x = va_arg(args, int);
		i++;
	} while (x >= 0);
}

TKOJaguarController::~TKOJaguarController() {
	for (unsigned int i = 0; i < _jaguars.size(); i++)
		delete _jaguars[i];
}

void TKOJaguarController::SetSpeed(float speed, int output, ...) {
	va_list args;
	va_start(args, output);
	int x = output;
	do {
		int id = _GetId(x);
		_jaguars[id]->Set(speed, 0x80);
		CANJaguar::UpdateSyncGroup(0x80);
		x = va_arg(args, int);
	} while (x >= 0);
}

void TKOJaguarController::SetSpeedAll(float speed) {
	DSLog(4, "Hello %d", _jaguars.size());
	for (unsigned int i = 0; i < _jaguars.size(); i++) {
		_jaguars[i]->Set(speed, 0x80);
		CANJaguar::UpdateSyncGroup(0x80);
	}
}

void TKOJaguarController::SetPID(float p, float i, float d, ...) {
	va_list args;
	while (true) {
		int output = va_arg(args, int);
		if (output < 0)
			break;
//		int id = _GetId(output);
//		_jaguars[id]->SetPID(p, i, d);
	}
}

void TKOJaguarController::SetPIDAll(float p, float i, float d) {
	for (unsigned int i = 0; i < _jaguars.size(); i++) {
//		_jaguars[i]->SetPID(p, i, d);
	}
}

int TKOJaguarController::_GetId(int output) {
	for (unsigned int i = 0; i < _outputs.size(); i++) {
		if (_outputs[i] == output)
			return i;
	}
	return -1;
}
