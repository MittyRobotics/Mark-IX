#include "AutoTarget.h"

static void Targetting (AutoTarget *t) {
	float angle = 0;
	while (1) {
		if (t->GetStatus()) {
			if (t->GetTurret()->GetStatus() == "Manual")
				t->GetTurret()->SwitchStatus();
			angle = t->GetCamera()->GetAngle();
			t->GetTurret()->SetSpeed(angle / 90.0);
		}
		else {
			if (t->GetTurret()->GetStatus() == "Automatic")
				t->GetTurret()->SwitchStatus();
		}
		Wait(0.005);
	}
}

AutoTarget::AutoTarget() {
	m_status = false;
	m_turret = TKOTurret::GetInstance();
	m_camera = new TKOCamera();
	m_target = new Task("AutoTarget", (FUNCPTR) Targetting, 50);
	m_target->Start();
}

AutoTarget::~AutoTarget() {
	m_target->Stop();
	delete m_camera;
	delete m_target;
}

void AutoTarget::SetStatus(bool status) {
	m_status = status;
}

bool AutoTarget::GetStatus() {
	return m_status;
}

TKOCamera* AutoTarget::GetCamera() {
	return m_camera;
}

TKOTurret* AutoTarget::GetTurret() {
	return m_turret;
}
