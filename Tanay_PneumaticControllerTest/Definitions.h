#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H
//The necessary libraries and classes
#include "WPILib.h"
#include "Pneumatics.h"
#include "PneumaticController.h"
#include <vector>

#define gg ;

//ports
const int PNEUM_CHANNEL		=	3;
const int PNEUM_STARTPORT	=	1;
const int PNEUM_ENDPORT		=	2;
const int PNEUM_COMPRESS_CH	=	4;
const int PNEUM_COMPRESS	=	7;

//which PneumaticController is which in the Pneumatics class
//[0..endport]
const int PNEUM_CATA		=	0;

#endif
