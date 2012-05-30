#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#include "WPILib.h"
#include "Vision/RGBImage.h"
#include <cmath>

// For printing to the Driver Station
#define DSClear()				DriverStationLCD::GetInstance()->Clear(); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define SDLog(line, msg, ...)	{ \
									char str[100]; \
									sprintf(str, msg, ##__VA_ARGS__); \
									SmartDashboard::Log(str, line); \
								}

const int STICK_1_PORT = 1;

const int BORDER_SIZE = 3;

const float PI = 3.14159265;
const float DEGREES_PER_RADIAN = 180 / PI;

const float TANGENT_VERTICAL_CAMERA_ANGLE = tan(19. / DEGREES_PER_RADIAN);
const float TANGENT_HORIZONTAL_CAMERA_ANGLE = tan(19. / DEGREES_PER_RADIAN);	// TODO: Find this
const int SCREEN_WIDTH_PIXELS = 320;
const int SCREEN_HEIGHT_PIXELS = 240;
const float TARGET_HEIGHT = 16.75;												// TODO: Fix for competition
const float TARGET_WIDTH = 24.;													// In inches
const float HOOP_DIAMETER = 18.;
const float BACKBOARD_TO_HOOP_RIM = 6.;

const float DISTANCE_CONSTANT = .5 * TARGET_HEIGHT * SCREEN_HEIGHT_PIXELS / TANGENT_VERTICAL_CAMERA_ANGLE;
const float QUARTER_TARGET_WIDTH_SQUARED = .25 * TARGET_WIDTH * TARGET_WIDTH;
const float BACKBOARD_TO_HOOP_CENTER = BACKBOARD_TO_HOOP_RIM + HOOP_DIAMETER / 2;
const float BACKBOARD_TO_HOOP_CENTER_SQUARED = BACKBOARD_TO_HOOP_CENTER * BACKBOARD_TO_HOOP_CENTER;

#endif
