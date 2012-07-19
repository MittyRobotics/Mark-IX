#include "WPILib.h"

#define DSClear()				DriverStationLCD::GetInstance()->Clear(); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define FLog(msg, ...)			{ \
									char str[100]; \
									sprintf(str, msg, ##__VA_ARGS__); \
									TKOLogging::GetInstance()->WriteToFile(str); \
								}
#define gg ;
