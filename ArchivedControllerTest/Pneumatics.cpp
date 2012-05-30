#include "Pneumatics.h"

using namespace std;

Pneumatics::Pneumatics(int port1, int start, int end) {
	for (int i = start; i < end - 1; i += 2) {
		p->push_back(PneumaticController(port1, i, i + 1));
	}
	max = p->size();
	compress->Start();
}

Pneumatics::~Pneumatics() {
	p->clear();
	compress->Stop();
}

void Pneumatics::Extend(int num) {
	if (num > 0 && num < max)
		p->at(num).Extend();
}

void Pneumatics::Retract(int num) {
	if (num > 0 && num < max)
		p->at(num).Retract();
}
