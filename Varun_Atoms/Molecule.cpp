#include "Molecule.h"

Molecule::Molecule(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight, Gyro *gyro) {
	_frontLeft = frontLeft;
	_rearLeft = rearLeft;
	_frontRight = frontRight;
	_rearRight = rearRight;
	_gyro = gyro;
}

Molecule::~Molecule() {
	Atom *a;
	while (_formula.size() > 0) {
		a = _formula.front();
		_formula.pop();
		delete a;
	}
}

// TODO: Master and slave
void Molecule::AddLineAtom(float kP, float kI, float kD, float distance) {
	_formula.push(new LineAtom(kP, kI, kD, _frontLeft, _rearLeft, _frontRight, _rearRight, distance));
}
void Molecule::AddTurnAtom(float kP, float kI, float kD, float angle) {
	_formula.push(new TurnAtom(kP, kI, kD, _frontLeft, _rearLeft, _frontRight, _rearRight, _gyro, angle));
}
void Molecule::AddPauseAtom(float seconds) {
	_formula.push(new PauseAtom(_frontLeft, _rearLeft, _frontRight, _rearRight, seconds));
}

void Molecule::Execute() {
	if (_formula.size() == 0)
		return;
	Atom *a = _formula.front();
	if (a->Done()) {
		_formula.pop();
		delete a;
	} else {
		a->Execute();
		DSLog(4, "Executing new atom");
	}
}

bool Molecule::Done() {
	return (_formula.size() == 0);
}
