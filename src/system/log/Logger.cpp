/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Logger.cpp
 *
 */

#include "Logger.h"

#include <cstdarg>
#include <cstdio>

#include "system/util/Exceptions.h"
#include "system/os/OSIncludes.h"

using namespace std;

Logger::Logger()
{
	setup();
}

Logger::~Logger()
{
}

void Logger::setup()
{
	m_level = LOG_LEVEL_DEBUG;
}

bool Logger::open(Properties &props)
{
	string strLevel = props.getProperty("log.level", "debug");
	m_level = strToLevel(strLevel);

	return true;
}

void Logger::close()
{
}

string Logger::levelToStr(int level)
{
	switch (level) {
		case LOG_LEVEL_DEBUG : return "DEBUG";
		case LOG_LEVEL_INFO : return "INFO";
		case LOG_LEVEL_WARN : return "WARN";
		case LOG_LEVEL_ERROR : return "***ERROR***";
		case LOG_LEVEL_NONE : return "NONE";
		default : return "Unknown";
	}
}

int Logger::strToLevel(const string &str)
{
	if (str == "debug") return LOG_LEVEL_DEBUG;
	else if (str == "info") return LOG_LEVEL_INFO;
	else if (str == "warn") return LOG_LEVEL_WARN;
	else if (str == "error") return LOG_LEVEL_ERROR;
	else if (str == "none") return LOG_LEVEL_NONE;

	return LOG_LEVEL_DEBUG;
}

ostream &Logger::debug()
{
	return log(LOG_LEVEL_DEBUG);
}

ostream &Logger::info()
{
	return log(LOG_LEVEL_INFO);
}

ostream &Logger::warn()
{
	return log(LOG_LEVEL_WARN);
}
ostream &Logger::error()
{
	return log(LOG_LEVEL_ERROR);
}

ostream &Logger::log(int level)
{
	if (getLevel() < level) return m_nullStream;

	*this << '[' << getLogHeader(level) << "] ";
	return *this;
}

ostream &Logger::debug(const string &filename, const string &fnname, int line)
{
	return log(LOG_LEVEL_DEBUG, filename, fnname, line);
}

ostream &Logger::info(const string &filename, const string &fnname, int line)
{
	return log(LOG_LEVEL_INFO, filename, fnname, line);
}

ostream &Logger::warn(const string &filename, const string &fnname, int line)
{
	return log(LOG_LEVEL_WARN, filename, fnname, line);
}

ostream &Logger::error(const string &filename, const string &fnname, int line)
{
	return log(LOG_LEVEL_ERROR, filename, fnname, line);
}

ostream &Logger::log(int level, const string &filename, const string &fnname, int line)
{
	if (getLevel() < level) return m_nullStream;

	*this << '[' << getLogHeader(level) << "-(" << fnname << ':' << line << ")] ";

	return *this;
}

string Logger::getLogHeader(int level)
{
	OSTime dt;
	OSTime::getCurrentTime(dt);

	return levelToStr(level) + '-' + dt.dateTimeToString();
}
