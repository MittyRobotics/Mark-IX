#ifndef __TKOCAMERA_H
#define __TKOCAMERA_H

#include "Definitions.h"

class TKOCamera : public PIDSource {
public:
	TKOCamera();
	virtual ~TKOCamera();
	double PIDGet();				// Do not change these method declarations; only change code in .cpp file
};

#endif
