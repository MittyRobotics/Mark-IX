#include "TKOConveyor.h"
///Constructor for the TKOConveyor class
/*!
	Initializes the 3 switches for the state system and the 2 conveyor jaguars.
	\param int port1 - ID for switch1
	\param int port2 - ID for switch2
	\param int port3 - ID for switch3
	\param int port4 - ID for upper conveyor's Jaguar
	\param int port5 - ID for lower conveyor's Jaguar
*/
TKOConveyor::TKOConveyor(int port1, int port2, int port3, int port4, int port5):
	_switch1(port1),
	_switch2(port2),
	_switch3(port3),
	_upper(port4),
	_lower(port5)
{
	_balls.reserve(6);		// In case there are more than 3 balls in the system, make maximum size of list 6
}
///Destructor for the TKOConveyor class
TKOConveyor::~TKOConveyor() {}

///Runs the conveyors using the state system, which is based on inputs from limit switches
/*!
	The state system classifies the location of each ball as on the top limit switch, on the middle limit switch, on the bottom limit switch,
	between two switches, or shot. The location of each ball moves up whenever it releases a limit switch or presses a new limit switch.
	When 3 balls are in the system, the state system will not allow the conveyors to move up unless canShoot is true. However,
	even if there are fewer than 3 balls in the system, canShoot can be set to true to turn on the upper conveyor and shoot the balls.
	If the state system is ever overridden, the location of each ball will be lost, and the ball count will be set to 0.
	The state system never turns the conveyors backward to move a ball down.
	\param bool canShoot - true if the robot can turn on the upper conveyor to shoot the topmost ball, false otherwise
*/
void TKOConveyor::Run(bool canShoot) {
	// Add balls into system if and only if a switch is pressed and the lowest ball is above the switch
	TKOBall lowestBall;
	if (_balls.size() == 0)
		lowestBall = shot;
	else
 		lowestBall = _balls[_balls.size() - 1];
	if (lowestBall > two && !_switch2.Get())
		_balls.push_back(two);
	if (lowestBall > three && !_switch3.Get())
		_balls.push_back(three);
	
	// Moving balls through system
	TKOBall prevBall = shot;
	for (unsigned int i = 0; i < _balls.size(); i++) {
		// If a switch is pressed and no TKOBall is on it, increment the state of the current TKOBall
 		if (prevBall > one && _balls[i] < one && !_switch1.Get())
			_balls[i] = one;
		else if (prevBall > two && _balls[i] < two && !_switch2.Get())
			_balls[i] = two;
		else if (prevBall > three && !_switch3.Get())
			_balls[i] = three;
		// Set state of current ball to transitioning when its switch is released
		else if (_balls[i] == two && _switch2.Get())
			_balls[i] = two_one;
		else if (_balls[i] == three && _switch3.Get())
			_balls[i] = three_two;
		prevBall = _balls[i];
	}	
	// Lower conveyor is off if and only if the third ball is at the third switch or further 
	if (_balls.size() >= 3 && _balls[2] >= three)
		_lower.SetOn(false);
	else
		_lower.SetOn(1);
	
	// If there are no balls, upper conveyor is off
	if (_balls.size() == 0) {
		_upper.SetOn(false);
	} else if (_balls[0] == shot) {			// If first ball has been set to shot state
		if (_switch1.Get())					// Turn upper on until first ball is gone
			_upper.SetOn(2);
		else {								// When first ball is gone, remove first ball from list
			_balls.erase(_balls.begin());
		}
	} else if (_balls[0] == one) {
		if (canShoot) {
			_balls[0] = shot;
			_upper.SetOn(2);
		} else
			_upper.SetOn(false);
	} else if (_balls[0] == two_one || _balls[0] == two)
		_upper.SetOn(2);						// TODO: This might need to pulse...
	else /*if balls[0] is at or below two*/ {
		_upper.SetOn(false);
	}
}

///Turns both conveyors forward
/*!
	Allows all balls to exit through the shooter, and sets internal ball count to 0.
*/

void TKOConveyor::OverrideAll() {
	DSLog(2, "%d %d %d", _switch1.Get(), _switch2.Get(), _switch3.Get());
	_upper.SetOn(2);
	_lower.SetOn(1);
	while (_balls.size() > 0)
		_balls.erase(_balls.begin());
}

///Checks if robot is allowed to pick up balls
/*!
	\return bool - true if amount of balls stored is less than 3, false otherwise.
*/

bool TKOConveyor::CanEnableIntake() {
	return _balls.size() < 3;
}

///Stops both conveyors
/*!
*/

void TKOConveyor::EndAll() {
	_upper.SetOn(0);
	_lower.SetOn(0);
}

///Turns both conveyors backward
/*!
	Allows all balls to exit through the bottom of the robot, and sets internal ball count to 0.
*/

void TKOConveyor::Reverse() {
	DSLog(2, "%d %d %d", _switch1.Get(), _switch2.Get(), _switch3.Get());
	_upper.SetOn(1);
	_lower.SetOn(2);
	while (_balls.size() > 0)
		_balls.erase(_balls.begin());
}

///Get the amount of balls stored in the robot
/*!
	\return int - the number of balls stored
*/

int TKOConveyor::GetNumBalls() {
	return _balls.size();
}
