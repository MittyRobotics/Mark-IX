#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#include "WPILib.h"

#define DSClear()				DriverStationLCD::GetInstance()->Clear(); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();

// PID Constants used in Atom
const float P_DISTANCE					= 0.;
const float I_DISTANCE					= 0.;
const float D_DISTANCE					= 0.;
const float P_ANGLE						= 0.;
const float I_ANGLE						= 0.;
const float D_ANGLE						= 0.;

const int MOTOR_L1						=	1;
const int MOTOR_L2						=	4;
const int MOTOR_R1						=	3;
const int MOTOR_R2						=	2;

const int GYRO_PORT						=	1;	//TODO: Make sure this is right

const float PI = 3.14159265358979323846;
const float RADIUS_WHEEL = 12345.;				// TODO: Get this
const float INVERSE_TWO_PI_RADIUS		= .5 / PI / RADIUS_WHEEL;

const float GYRO_TICKS_PER_DEG			= 13/36.;

#endif
