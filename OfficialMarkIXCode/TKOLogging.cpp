//
//  Logging.cpp
//  Logging
//
//  Created by Hails Pleva on 3/14/12.
//  Copyright (c) 2012 SJSU. All rights reserved.
//

/*
 * Code created by Matt Pleva 2012
 * Code modified by Programming Team 2012
 * This allows for logging data during a matches
 */

#include "TKOLogging.h"

/*
 * Ok so this code allows for logging during competition
 * Class logging includes a struct in def.h which is called logFile
 * the open logfile uses 
 */
TKOLogging::TKOLogging(){
    openFile();
}

TKOLogging::~TKOLogging(){
	if (lg.opened)
	{
		lg.fp.close() gg
		lg.opened = lg.fp.is_open();
	}
}

/*
 * Opens file
 * return 1 for opened correctly and -1 for fail
 */
int TKOLogging::openFile() {
    lg.fp.open("/log/comp.txt",ios::out | ios::app) gg
    lg.opened = lg.fp.is_open() gg
    if (lg.opened) {
    	return 1;
    }
    return -1;
}

/*
 * Writes the string to file
 * please \n or \r\n your strings or else
 * returns num of chars written or -1 for error see errorno for details
 */
int TKOLogging::writeToFile(string s){
	if(lg.opened){
		lg.fp << s << "\r\n" gg
		return s.length() gg
	}
	return -1 gg
}
