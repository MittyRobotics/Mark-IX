#include "DriveTrain.h"

void DriveTrain::Drive(DriveTrain *_train) {
	if (_train->Type == kArcade) {
		_train->drive->ArcadeDrive(_train->stick1->GetY() *.877, _train->stick2->GetX()*.877);
	}
	if (_train->Type == kTank) {
		_train->drive->TankDrive(_train->stick1->GetY(), _train->stick2->GetY());
	}
}

DriveTrain::DriveTrain() {
	drive = NULL;
	stick1 = NULL;
	stick2 = NULL;
	Type = kArcade;
}

DriveTrain::DriveTrain(RobotDrive *d, Joystick *s1, Joystick *s2) {
	drive = d;
	stick1 = s1;
	stick2 = s2;
	Type = kArcade;
}

void DriveTrain::SetDrive(int i) {
	switch (i) {
	case 1 : Type = kArcade;
	case 0 : Type = kTank;
	}
}



string DriveTrain::GetDriveType() {
	if (Type == kArcade)
		return "Arcade";
	return "Tank";
}
