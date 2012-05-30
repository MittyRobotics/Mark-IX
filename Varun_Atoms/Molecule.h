#ifndef __MOLECULE_H
#define __MOLECULE_H

#include "Definitions.h"
#include "Atom.h"
#include <queue>

class Molecule {
public:
	Molecule(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight, Gyro *gyro);
	~Molecule();
	void AddLineAtom(float kP, float kI, float kD, float distance);
	void AddTurnAtom(float kP, float kI, float kD, float angle);
	void AddPauseAtom(float seconds);
	void Execute();
	bool Done();

private:
	CANJaguar *_frontLeft, *_rearLeft, *_frontRight, *_rearRight;
	Gyro *_gyro;
	std::queue<Atom *> _formula;
};

#endif
