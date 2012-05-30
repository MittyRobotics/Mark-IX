#include "WPILib.h"
#include "Molecule.h"

class RobotDemo : public SimpleRobot
{
	Joystick stick;
	CANJaguar jag1, jag2, jag3, jag4;
	Gyro gyro;
	Molecule glucose, fructose;

public:
	RobotDemo(void):
		stick(1),
		jag1(1),		// Front left
		jag2(2),		// Rear right
		jag3(3),		// Front right
		jag4(4),		// Rear left
		gyro(1),
		glucose(&jag1, &jag4, &jag3, &jag2, &gyro),
		fructose(&jag1, &jag4, &jag3, &jag2, &gyro)
	{
		
	}
	
	void Autonomous(void) {
		while (IsAutonomous()) {
			glucose.Execute();
			Wait(0.005);
		}
	}
	
	void OperatorControl(void) {
		while (IsOperatorControl()) {
			Wait(0.005);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
