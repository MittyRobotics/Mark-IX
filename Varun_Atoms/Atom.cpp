#include "Atom.h"

Atom::Atom(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight)
{
	_frontLeft = frontLeft;
	_rearLeft = rearLeft;
	_frontRight = frontRight;
	_rearRight = rearRight;
	_done = false;
	_started = false;
}

bool Atom::Done() {
	return _done;
}

LineAtom::LineAtom(float kP, float kI, float kD,
				   CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight,
				   float distance):
Atom(frontLeft, rearLeft, frontRight, rearRight)
{
	_kP = kP;
	_kI = kI;
	_kD = kD;
	_distance = distance;
}

LineAtom::~LineAtom() {}

// TODO: Find inches per rotation
void LineAtom::Execute() {
	if (!_started) {
		_frontLeft->ChangeControlMode(CANJaguar::kPosition);
		_rearLeft->ChangeControlMode(CANJaguar::kVoltage);
		_frontRight->ChangeControlMode(CANJaguar::kPosition);
		_rearRight->ChangeControlMode(CANJaguar::kVoltage);
		_frontLeft->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		_frontRight->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		_frontLeft->SetPID(_kP, _kI, _kD);
		_rearLeft->SetPID(_frontLeft->GetP(), _frontLeft->GetI(), _frontLeft->GetD());
		_frontRight->SetPID(_kP, _kI, _kD);
		_rearRight->SetPID(_frontRight->GetP(), _frontRight->GetI(), _frontRight->GetD());
		_frontLeft->Set(_distance * INVERSE_TWO_PI_RADIUS);
		_rearLeft->Set(_frontLeft->GetOutputVoltage());
		_frontRight->Set(_distance * INVERSE_TWO_PI_RADIUS);
		_rearRight->Set(_frontRight->GetOutputVoltage());
		_started = true;
	}
	DSLog(1, "%d %d %d %d", floor(_frontLeft->GetSpeed()), floor(_frontLeft->GetOutputVoltage()), floor(_frontLeft->GetOutputCurrent()), floor(_frontLeft->GetOutputVoltage() * _frontLeft->GetOutputCurrent()));
	DSLog(2, "%d %d %d %d", floor(_frontRight->GetSpeed()), floor(_frontRight->GetOutputVoltage()), floor(_frontRight->GetOutputCurrent()), floor(_frontRight->GetOutputVoltage() * _frontRight->GetOutputCurrent()));
	// TODO: if GetSpeed() doesn't work, use GetOutputVoltage() and/or GetOutputCurrent()
	if (abs(_frontLeft->GetPosition() - _frontLeft->Get()) < .05 * INVERSE_TWO_PI_RADIUS && abs(_frontLeft->GetSpeed()) < .3 * INVERSE_TWO_PI_RADIUS
	 && abs(_frontRight->GetPosition() - _frontRight->Get()) < .05 * INVERSE_TWO_PI_RADIUS && abs(_frontRight->GetSpeed()) < .3 * INVERSE_TWO_PI_RADIUS)
		_done = true;
}

TurnAtom::TurnAtom(float kP, float kI, float kD,
				   CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight,
				   Gyro *gyro, float angle):
Atom(frontLeft, rearLeft, frontRight, rearRight),
_p1(kP, kI, kD, gyro, frontLeft),
_p2(kP, kI, kD, gyro, rearLeft),
_p3(kP, kI, kD, gyro, frontRight),
_p4(kP, kI, kD, gyro, rearRight)
{
	_gyro = gyro;
	_angle = angle;
}

TurnAtom::~TurnAtom() {}

void TurnAtom::Execute() {
	if (!_started) {
		_p1.SetSetpoint(_angle);
		_p2.SetSetpoint(_angle);
		_p3.SetSetpoint(_angle);
		_p4.SetSetpoint(_angle);
		_p1.Enable();
		_p2.Enable();
		_p3.Enable();
		_p4.Enable();
		_started = true;
	}
	// TODO: I just took this from the code from 2 years ago lol
	if (abs(_p1.Get()) < .05 && abs(_p1.GetError()) < .3 * GYRO_TICKS_PER_DEG
	 && abs(_p2.Get()) < .05 && abs(_p2.GetError()) < .3 * GYRO_TICKS_PER_DEG
	 && abs(_p3.Get()) < .05 && abs(_p3.GetError()) < .3 * GYRO_TICKS_PER_DEG
	 && abs(_p4.Get()) < .05 && abs(_p4.GetError()) < .3 * GYRO_TICKS_PER_DEG)
		_done = true;
}

PauseAtom::PauseAtom(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight,
					 float seconds):
Atom(frontLeft, rearLeft, frontRight, rearRight)
{
	_time = seconds;
}

PauseAtom::~PauseAtom() {}

void PauseAtom::Execute() {
	if (!_started) {
		_clock.Start();
		_frontLeft->Set(0.);
		_rearLeft->Set(0.);
		_frontRight->Set(0.);
		_rearRight->Set(0.);
		_started = true;
	}
	if (_clock.Get() >= _time)
		_done = true;
}

WristAtom::WristAtom(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight):
Atom(frontLeft, rearLeft, frontRight, rearRight)
{}

WristAtom::~WristAtom() {}

void WristAtom::Execute() {}

ShootAtom::ShootAtom(CANJaguar *frontLeft, CANJaguar *rearLeft, CANJaguar *frontRight, CANJaguar *rearRight):
Atom(frontLeft, rearLeft, frontRight, rearRight)
{}

ShootAtom::~ShootAtom() {}

void ShootAtom::Execute() {}
