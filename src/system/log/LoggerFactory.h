/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : LoggerFactory.h
 *
 */

#ifndef _LOGGER_FACTORY_H_
#define _LOGGER_FACTORY_H_

#include <string>
#include <map>
#include <iostream>

namespace opros
{
	namespace engine
	{
class Config;
	}
}
#include "Logger.h"


class Properties;

using namespace opros::engine;

/**
	Logger 객체를 생성하고 관리하는 Factory 클래스.
*/
class LoggerFactory
{
	typedef std::map<std::string, Logger *> LoggerMap;

	/** 로거를 저장할 맵 */
	LoggerMap m_loggers;

	/** 디폴트 로거 이름 */
	std::string m_defaultLoggerName;

public:
	LoggerFactory();
	virtual ~LoggerFactory();

	/** LoggerFactory 초기화 함수 */
	virtual bool init(Config *cfg);

	/** LoggerFactory 삭제 함수 */
	virtual void destroy();

	/** 디폴트 로거 이름 세팅 함수 */
	void setDefaultLoggerName(const std::string &name) { m_defaultLoggerName = name; };

	/** 디폴트 로거 이름 반환 함수 */
	std::string getDefaultLoggerName() { return m_defaultLoggerName; };

	/** 
		새로운 로거를 추가하는 함수. 

		@param name 추가할 로거 이름
		@param type 추가할 로거 타입
		@param props 추가할 로거의 프로퍼티
	*/
	virtual void addLogger(const std::string &name, const std::string &type, Properties &props);

	/**
		Return the logger corresponding to the name.
		This function can create the logger instance if it is not there

		@param name logger name
	 */
	virtual Logger *getLogger(const std::string &name = "");

	/** 
		Remove the logger corresponding to the name.

		@param name logger name
	 */
	virtual void removeLogger(const std::string &name);

	/** 
		새로은 로거를 생성하는 함수.

		@param type 생성할 로거의 타입
	 */
	virtual Logger *createLogger(const std::string &type);

};

#endif 
