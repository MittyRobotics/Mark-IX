#ifndef __TKOCAMERA_H
#define __TKOCAMERA_H
#include "WPILib.h"

class TKOCamera {
public:
	TKOCamera();
	~TKOCamera();
	float GetAngle();
private:
	float m_angle;
};


#endif
