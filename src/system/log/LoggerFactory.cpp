/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : LoggerFactory.cpp
 *
 */

#include "LoggerFactory.h"

#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "CoutLogger.h"
#include "CerrLogger.h"
#include "FileLogger.h"

#include "system/Registry.h"
#include "system/config/Config.h"

#include "system/Trace.h"

using namespace std;


class CNullLogger : public Logger
{
public:
	CNullLogger() { };
	virtual ~CNullLogger() {};

	/**
		std::cout에 로그 메시지를 출력하는 함수.
		StringStream::output_string() 함수를 구현한 함수.

		@param str 출력할 로그 메시지

		@see StringStream::output_string
	*/
	virtual void output_string(const std::string &str) {		
	};
};



/*
 * LoggerFactory
 */
LoggerFactory::LoggerFactory()
{
	m_defaultLoggerName = "default";
}

LoggerFactory::~LoggerFactory()
{
	destroy();
}

void LoggerFactory::destroy()
{
	LoggerMap::iterator pos = m_loggers.begin();
	for (; pos != m_loggers.end(); ++pos) {
		Logger *l = pos->second;
		if (l != NULL) {
			l->close();
			delete l;
		}
	}

	m_loggers.clear();
}

bool LoggerFactory::init(Config *cfg)
{
	if (cfg == NULL) return true;

	ConfigList list = cfg->getChildren("logger");
	ConfigList::const_iterator iter = list.begin();
	for(; iter != list.end(); ++iter) {
		try {
			if ((*iter)->getAttributeAsBool("enable", true)) {
				string name = (*iter)->getAttribute("name");
				string type = (*iter)->getAttribute("type", "cout");
				Properties props;
				(*iter)->toProperties(props, "prop");

				addLogger(name, type, props);
			}
		}
		catch (...) {
			trace("cannot create logger");
		}
	}
	Properties props;
	addLogger("system.logger", "null", props);
	
	return true;
}

void LoggerFactory::addLogger(const string &name, const string &type, Properties &props)
{
	try {
		Logger *l = getLogger(name);
		if (l == NULL) {
			l = createLogger(type);
			l->open(props);
			m_loggers.insert(make_pair(name, l));
		}
		if (l == NULL) {
			trace("<LoggerFactory::addLogger> logger is null : name=" + name);
			return;
		}
	}
	catch (Exception &ex) {
		trace(ex);
	}
}

Logger *LoggerFactory::getLogger(const string &name)
{
	std::string lname;
	if (name == "") lname = m_defaultLoggerName;
	else lname = name;

	LoggerMap::iterator iter = m_loggers.find(lname);
	if (iter != m_loggers.end()) return iter->second;
	else return NULL;
}

void LoggerFactory::removeLogger(const string &name)
{
	LoggerMap::iterator iter = m_loggers.find(name);
	m_loggers.erase(iter);
	if (iter->second != NULL) {
		iter->second->close();
		delete iter->second;
	}
}

Logger *LoggerFactory::createLogger(const string &type)
{
	if (type == "file") {
		return new FileLogger();
	}
	else if (type == "cerr") {
		return new CerrLogger();
	}
	else if (type == "null") {
		return new CNullLogger();
	}


	return new CoutLogger();
}
