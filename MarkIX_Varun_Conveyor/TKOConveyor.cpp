#include "TKOConveyor.h"

TKOConveyor::TKOConveyor():
	_switch1(SWITCH_1_PORT),
	_switch2(SWITCH_2_PORT),
	_switch3(SWITCH_3_PORT),
	_upper(UPPER_RELAY_PORT),
	_lower(LOWER_RELAY_PORT)
{
	_balls.reserve(6);		// In case there are more than 3 balls in the system, make maximum size of list 6
}

TKOConveyor::~TKOConveyor() {}

// TODO: need to account for manual override
// TANAY THIS IS ALL YOU
void TKOConveyor::Run(bool shootButton, bool canShoot) {
	// Adding balls into system
	TKOBall lowestBall;
	if (_balls.size() == 0)
		lowestBall = shot;
	else
		lowestBall = _balls[_balls.size() - 1];
	if (lowestBall > two && _switch2.Get())
		_balls.push_back(two);
	if (lowestBall > three && _switch3.Get())
		_balls.push_back(three);
	
	// Moving balls through system
	TKOBall prevBall = shot;
	for (unsigned int i = 0; i < _balls.size(); i++) {
		if (_balls[i] == three_two || _balls[i] == two_one) {
			if (prevBall > one && _switch1.Get()) {
				_balls[i] = one;
				prevBall = _balls[i];
				continue;						// Go to next iteration of for loop; similar to a break statement
			}
			if (prevBall > two && _switch2.Get()) {
				_balls[i] = two;
				prevBall = _balls[i];
				continue;
			}
			if (prevBall > three && _switch3.Get()) {
				_balls[i] = three;
				prevBall = _balls[i];
				continue;
			}
		} else if (_balls[i] == two && !_switch2.Get())
			_balls[i] = two_one;
		else if (_balls[i] == three && !_switch3.Get())
			_balls[i] = three_two;
		prevBall = _balls[i];
	}
	
	// Controlling lower conveyor
	if (_balls.size() == 0 && !Stuff())
		_lower.Off();
	else
		_lower.On();
	
	// Controlling upper conveyor and shooter
	if (_balls.size() == 0) {
		_upper.Off();
		_shooter.RampToSpeed(0);
	} else if (_balls[0] == shot) {			// First ball has been set to shot state
		if (_switch1.Get())					// Turn upper on until first ball is gone
			_upper.On();
		else								// When gone, remove first ball from list
			_balls.erase(_balls.begin());
	} else if (_balls[0] == one) {
		_shooter.RampToSpeed(200);
		if (canShoot) {
			_balls[0] = shot;
			_upper.On();
		} else {
			_upper.Off();
		}
	} else if (_balls[0] == two_one || _balls[0] == two)
		_upper.On();						// TODO: This might need to pulse...
	else /*if balls[0] is at or below two*/ {
		_upper.Off();
		_shooter.RampToSpeed(0);
	}
}

bool TKOConveyor::Stuff() {
	return _balls.size() < 3;
}
