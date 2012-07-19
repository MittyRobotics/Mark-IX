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

#include "Definitions.h"
#include <fstream>

/**
 * Logging class used for Competition match logging to the CRIO.
 * 
 * This is used to write strings to a file on the crio at /log/comp.txt .
 * That also means you need to setup that directory on the CRIO or face errors.
 */
class TKOLogging : public SensorBase
{
public:
	/**
	 * This function is called to start logging.
	 * @return A pointer to the singleton TKOLogging.
	 */
	static TKOLogging *GetInstance();
	/**
	 * This function is called to open the file for logging.
	 * @return 1 on success and -1 on failure.
	 */
	int OpenFile();
	
	/**
	 * This function is called by the code to log a string to the file.
	 * @param s string to write to file.
	 * @return The number of character written to file on success and -1 on failure.
	 */
	int WriteToFile(string s);
private:
	DISALLOW_COPY_AND_ASSIGN(TKOLogging);
	TKOLogging();
	~TKOLogging();
	static TKOLogging *_instance;
	bool _opened;		// Whether the file is open or not
	ofstream _fp;		// The output stream
};

#endif
