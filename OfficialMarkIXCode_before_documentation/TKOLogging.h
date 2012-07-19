//
//  Logging.h
//  Logging
//
//  Created by Hails Pleva on 3/14/12.
//  Copyright (c) 2012 SJSU. All rights reserved.
//

#ifndef __TKOLOGGING_H
#define __TKOLOGGING_H

/*! \mainpage Logging CRIO Competition Page
 *
 * \section intro_sec Introduction
 *
 * Logging during matches will be helpful for fine tuning the robot and for seeing what the CRIO thinks it is doing.
 * Versus what you told it to do...
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Make folder "log" in the root folder of the CRIO
 * \subsection setp2 Step 2: Create comp.txt and move it to the log folder you just created on the CRIO
 * \subsection step3 Step 3: Test it.
 * \subsection step4 Step 4: There is no step 4. Move along.
 */

#include "WPILib.h"
#include "Definitions.h"
#include <fstream>

/**
 * You know you like structs!
 * This is a struct for the logging file.
 */
typedef struct logFile
{
	bool opened; /**< bool opened. Whether the file is open or not. */
    std::ofstream fp; /**< ofstream fp. The file. */
}logFile;

/**
 * Logging class used for Competition match logging to the CRIO.
 * 
 * This is used to write strings to a file on the crio at /log/comp.txt .
 * That also means you need to setup that directory on the CRIO or face errors.
 */
class TKOLogging
{
public:
	TKOLogging();
	~TKOLogging();
	
	/**
	 * This function is called to open the file for logging.
	 * @return This func returns 1 if it is succeeds and -1 if it fails.
	 */
	int openFile();
	
	/**
	 * This function is called by the code to log a string to the file.
	 * @param s string to write to file
	 * @return This func returns the number of character written to file and -1 if it fails.
	 */
	int writeToFile(string s);
private:
	/**
	 * The struct used to keep the values for logging.
	 * 
	 * They include the file errorno for error reporting opened for if the file is open and
	 * a flag to tell us if there is an error.
	 */
	logFile lg;
};

#endif
