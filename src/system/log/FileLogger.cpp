/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : FileLogger.cpp
 *
 */

#include <cstdarg>
#include <cstdio>

#include "FileLogger.h"
#include "system/os/OSIncludes.h"
#include "system/Registry.h"

using namespace std;

FileLogger::FileLogger()
{
	m_opened = false;
}

FileLogger::~FileLogger()
{
	if (m_opened) m_ofs.close();
}


bool FileLogger::open(Properties &props)
{
	Logger::open(props);

	OSTime ct;
	ct.resetToCurrentTime();

	string filename = props.getProperty("log.file", "log.file." + ct.dateTimeToString());
	string logDir;
	
	try {
		logDir = props.getProperty("log.dir");
		if (logDir == "system.dir") {
			logDir = Registry::getRegistry()->getLogDir();
		}
	}
	catch (...) {
		logDir = Registry::getRegistry()->getLogDir();
	}

	string filepath = logDir + filename;
	
	m_ofs.open(filepath.c_str(), fstream::out);

	m_opened = true;

	return true;
}

void FileLogger::output_string(const string &str) 
{
	if (m_opened) { 
		m_ofs << str; 
		m_ofs.flush(); 
	};
}



