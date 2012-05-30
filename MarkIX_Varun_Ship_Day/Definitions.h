#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#include "WPILib.h"

#define DSClear()				DriverStationLCD::GetInstance()->Clear(); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();

const int STICK_1_PORT = 1;

const int DRIVE_L1_ID = 1;
const int DRIVE_L2_ID = 2;
const int DRIVE_R1_ID = 3;
const int DRIVE_R2_ID = 4;
const int ROLLER_ID = 5;
const int TURRET_ID = 6;
const int SPINNER_1_ID = 7;
const int SPINNER_2_ID = 8;
const int WRIST_1_ID = 9;
const int WRIST_2_ID = 10;
//const int LIFT_1_ID = 11;
//const int LIFT_2_ID = 12;

// TODO: Get these constants
const int SWITCH_1_PORT = 0;
const int SWITCH_2_PORT = 0;
const int SWITCH_3_PORT = 0;
const int SWITCH_4_PORT = 0;
const int SWTICH_RIGHT_TURRET = 0;			// TODO: Forward
const int SWTICH_LEFT_TURRET = 0;			// TODO: Reverse
const int LOWER_RELAY_PORT = 1;
const int UPPER_RELAY_PORT = 2;

// TODO: Get these setpoints constants too :D
const float WRIST_DOWN = 0.;
const float WRIST_UP = 0.;
const float ROLLER_ON = 0.;
const float ROLLER_OFF = 0.;

#endif
