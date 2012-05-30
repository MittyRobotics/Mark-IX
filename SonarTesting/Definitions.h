#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H
#include "WPILib.h"
#include "math.h"
#define DSClear()				DriverStationLCD::GetInstance()->Clear();
#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define gg ;



//heights

const float bot = 0; //TODO: fill heights
const float top = 0;
const float mid = 0;

#endif
