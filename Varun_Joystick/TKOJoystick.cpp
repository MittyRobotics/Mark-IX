#include "TKOJoystick.h"

TKOJoystick::TKOJoystick(int port):
	_stick(port)
{
	_buttons.resize(12);
	for (int i = 0; i < 12; i++)
		_buttons[i] = false;
}

TKOJoystick::~TKOJoystick() {}

float TKOJoystick::GetX() {
	return _stick.GetX();
}

float TKOJoystick::GetY() {
	return _stick.GetY();
}

float TKOJoystick::GetZ() {
	return _stick.GetZ();
}

float TKOJoystick::GetTwist() {
	return _stick.GetTwist();
}

float TKOJoystick::GetThrottle() {
	return _stick.GetThrottle();
}

TKOJoystick::ButtonState TKOJoystick::GetButton(int button) {
	std::vector<bool, std::allocator<bool> >::reference prev = _buttons[button - 1];
	if (_stick.GetRawButton(button)) {
		if (prev)
			return kDown;
		prev = true;
		return kPressed;
	}
	if (prev) {
		prev = false;
		return kReleased;
	}
	return kUp;
}
