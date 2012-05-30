#ifndef __TKOTURRET_H
#define __TKOTURRET_H

#include "WPILib.h"
#include "SensorBase.h"
#include "Base.h"

class TKOTurret : SensorBase{
public:
	static TKOTurret* GetInstance();
	void SetSpeed(float speed);
	void SwitchStatus();
	char* GetStatus();
private:
	TKOTurret();
	~TKOTurret();
	DISALLOW_COPY_AND_ASSIGN(TKOTurret);
	static TKOTurret* m_instance;
	bool m_status;
	CANJaguar *m_jag;
};

#endif
