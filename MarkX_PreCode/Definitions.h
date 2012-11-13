#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

/*! \file Definitions.h
	\brief Defines constants and macros used by the other files in the project.
*/
#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Math.h"

/*!
  \def DSClear()
  Clears the Driver Station LCD of all text.
*/
/*!
  \def DSLog(line, msg, ...)
  Formatted printing of msg to the specified line on the Driver Station LCD.
*/
#define DSClear()				DriverStationLCD::GetInstance()->Clear(); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define gg ;

/*! \mainpage Welcome to TKO 1351's Mark X's Code Documentation!
 *
 * 	This is the Robot Code for the 2013 Season of <TBA>
 *  
 *  If you haven't already, please visit our <a href="http://www.amhsrobotics.com">website as 
 *  well as our <a href="http://amhsprogramming.blogspot.com">blog.</a>.
 */


// Joystick port definition
const int STICK_1_PORT = 1; // First Joystick port definition  < first drive joystick 
const int STICK_2_PORT = 2; // Second Joystick port definition < second drive joystick 
const int STICK_3_PORT = 3; // Third Joystick port definition  < first EVOM joystick 
const int STICK_4_PORT = 4; // Fourth Joystick port definition < second EVOM joystick 

//Jaguar ports
const int DRIVE_L1_ID = 1; // Define motor port 1 < first left drive motor
const int DRIVE_L2_ID = 2; // Define motor port 2 < second left drive motor
const int DRIVE_R1_ID = 3; // Define motor port 4 < first right drive motor
const int DRIVE_R2_ID = 4; // Define motor port 3 < second right drive motor
const int EVOM_1_ID = 5;   // Define motor port 5 < first EVOM motor
const int EVOM_2_ID = 6;   // Define motor port 6 < first EVOM motor


// Ports of the various switches
const int SWITCH_1_PORT = 3;
const int SWITCH_2_PORT = 2;
const int SWITCH_3_PORT = 1;
const int LOWER_RELAY_PORT = 1;
const int UPPER_RELAY_PORT = 2;

//PID constants
const float SPINNER_kP = 0.2;		//working P: 0.20, working I: .0034
const float SPINNER_kI = 0.004;
const float SPINNER_kD = 0.0;

// Camera Constants
const int BORDER_SIZE = 3;

const float PI = 3.14159265;
const float DEGREES_PER_RADIAN = 180 / PI;

const float TANGENT_VERTICAL_CAMERA_ANGLE = tan(19 / DEGREES_PER_RADIAN);
const float TANGENT_HORIZONTAL_CAMERA_ANGLE = tan(27. / DEGREES_PER_RADIAN);	// TODO: Find this
const int SCREEN_WIDTH_PIXELS = 320;
const int SCREEN_HEIGHT_PIXELS = 240;
const float TARGET_HEIGHT = 17.;
const float TARGET_WIDTH = 24.;													// In inches

const float DISTANCE_CONSTANT = .5 * TARGET_HEIGHT * SCREEN_HEIGHT_PIXELS / TANGENT_VERTICAL_CAMERA_ANGLE;
const float QUARTER_TARGET_WIDTH_SQUARED = .25 * TARGET_WIDTH * TARGET_WIDTH;

#endif
