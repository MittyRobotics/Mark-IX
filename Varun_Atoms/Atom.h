#ifndef __ATOM_H
#define __ATOM_H

#include "Definitions.h"
#include <cmath>

class Atom {
public:
	Atom(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight);
	virtual ~Atom() {};
	virtual void Execute() = 0;
	virtual bool Done();

protected:
	CANJaguar *_frontLeft, *_rearLeft, *_frontRight, *_rearRight;
	bool _done;
	bool _started;
};

class LineAtom : public Atom {
public:
	LineAtom(float kP, float kI, float kD,
			 CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight,
			 float distance);
	~LineAtom();
	void Execute();

private:
	float _kP, _kI, _kD, _distance;
};

class TurnAtom : public Atom {
public:
	TurnAtom(float kP, float kI, float kD,
			 CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight,
			 Gyro *gyro, float angle);
	~TurnAtom();
	void Execute();

private:
	Gyro *_gyro;
	PIDController _p1, _p2, _p3, _p4;
	float _angle;
};

class PauseAtom : public Atom {
public:
	PauseAtom(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight,
			  float seconds);
	~PauseAtom();
	void Execute();

private:
	float _time;
	Timer _clock;
};

// TODO: Include atoms for enabling/disabling wrist and shooting?
// Other possibility would be to have multiple molecules, and to shoot/enable wrist in between...............
// If that's the case, delete WristAtom and ShootAtom
class WristAtom : public Atom {
public:
	WristAtom(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight);
	~WristAtom();
	void Execute();
};

class ShootAtom : public Atom {
public:
	ShootAtom(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight);
	~ShootAtom();
	void Execute();
};

#endif
