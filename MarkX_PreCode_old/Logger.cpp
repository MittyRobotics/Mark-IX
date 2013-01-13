//
//  Logger.cpp
//  wpi
//
//  Created by Matt Pleva on 10/10/12.
//  Copyright (c) 2012 Matt Pleva. All rights reserved.
//

#include "Logger.h"
#include <fstream>

std::ofstream out;

Logger* Logger::m_instance = NULL;

Logger::Logger()
{
    out.open("log.txt", std::ios::out | std::ios::app);
}
Logger::~Logger()
{
    out.close();
    m_instance = NULL;
}

Logger* Logger::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new Logger();
	}
	return m_instance;
}

void Logger::Write(std::string s){
    out << s;
}
void Logger::Print(std::string s){
    printf("%s",s.c_str());
}
void Logger::Print(char s[]){
    printf("%s",s);
}
