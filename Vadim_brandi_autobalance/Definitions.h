#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#include "WPILib.h"
#include <cmath>

// For printing to the Driver Station
#define DSClear()				DriverStationLCD::GetInstance()->Clear();
#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define gg ;

// Ports for joysticks
const int STICK_PORT_1		=	1;
const int STICK_PORT_2		= 	2;

// Analog ports
const int GYRO_PORT			=	1;

const int ACCELEROMETER_MODULE = 1;


// Minimum Jaguar ID for CAN Bus
const int MIN_JAGUAR_ID		=	1;

double const BALANCE_PVALUE    = 0.01;

//double const BALANCE_PVALUEORIG = 0.01;

float const PAUSE_TIME = 0.5; // pause time if angle is small
float const PAUSE_ANGLE = 16.5;
float const MAINLOOP_PAUSE = 0.005;

const int COMPRESSOR_RELAY_PORT		= 1;
const int PRESSURE_SWITCH_PORT		= 14; //In slot 4 (!)

// Motor outputs - exact values don't matter, as long as they're different
const int FRONT_LEFT		=	1;
const int REAR_LEFT			=	2;
const int FRONT_RIGHT		=	3;
const int REAR_RIGHT		=	4;

#endif
