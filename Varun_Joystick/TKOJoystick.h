#include "WPILib.h"

class TKOJoystick {
public:
	typedef enum {kPressed, kDown, kReleased, kUp} ButtonState;
	TKOJoystick(int port);
	~TKOJoystick();
	float GetX();
	float GetY();
	float GetZ();
	float GetTwist();
	float GetThrottle();
	ButtonState GetButton(int button);
private:
	Joystick _stick;
	std::vector<bool> _buttons;
};
