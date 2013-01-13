//
//  Logger.h
//  wpi
//
//  Created by Matt Pleva on 10/10/12.
//  Copyright (c) 2012 Matt Pleva. All rights reserved.
//

#ifndef __wpi__Logger__
#define __wpi__Logger__

#include <string>

class Logger
{
public:
	Logger();
    ~Logger();
	void Write(std::string);
    static Logger* GetInstance();
    void Print(std::string);
    void Print(char[]);
private:
    static Logger *m_instance;
};

#endif /* defined(__wpi__Logger__) */
