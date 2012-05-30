#ifndef __AUTOTARGET_H
#define __AUTOTARGET_H

#include "WPILib.h"
#include "TKOCamera.h"
#include "TKOTurret.h"

class AutoTarget {
public:
	AutoTarget();
	~AutoTarget();
	void SetStatus(bool status);
	bool GetStatus();
	TKOCamera* GetCamera();
	TKOTurret* GetTurret();
private:
	bool m_status;
	TKOTurret *m_turret;
	TKOCamera *m_camera;
	Task *m_target;
};

#endif
