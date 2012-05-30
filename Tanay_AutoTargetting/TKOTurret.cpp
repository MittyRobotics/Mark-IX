#include "TKOTurret.h"

TKOTurret *TKOTurret::m_instance = NULL;

TKOTurret::TKOTurret() {
	m_jag = new CANJaguar (10);
	m_status = false;
	AddToSingletonList();
}

TKOTurret::~TKOTurret() {
	m_instance = NULL;
	delete m_jag;
}

TKOTurret *TKOTurret::GetInstance() {
	if (m_instance == NULL)
		m_instance = new TKOTurret();
	return m_instance;
}

void TKOTurret::SetSpeed(float speed) {
	m_jag->Set(speed);
}

void TKOTurret::SwitchStatus() {
	m_status = !m_status;
}

char* TKOTurret::GetStatus() {
	if (m_status)
		return "Automatic";
	return "Manual";
}
