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
	Logger ��ü�� �����ϰ� �����ϴ� Factory Ŭ����.
*/
class LoggerFactory
{
	typedef std::map<std::string, Logger *> LoggerMap;

	/** �ΰŸ� ������ �� */
	LoggerMap m_loggers;

	/** ����Ʈ �ΰ� �̸� */
	std::string m_defaultLoggerName;

public:
	LoggerFactory();
	virtual ~LoggerFactory();

	/** LoggerFactory �ʱ�ȭ �Լ� */
	virtual bool init(Config *cfg);

	/** LoggerFactory ���� �Լ� */
	virtual void destroy();

	/** ����Ʈ �ΰ� �̸� ���� �Լ� */
	void setDefaultLoggerName(const std::string &name) { m_defaultLoggerName = name; };

	/** ����Ʈ �ΰ� �̸� ��ȯ �Լ� */
	std::string getDefaultLoggerName() { return m_defaultLoggerName; };

	/** 
		���ο� �ΰŸ� �߰��ϴ� �Լ�. 

		@param name �߰��� �ΰ� �̸�
		@param type �߰��� �ΰ� Ÿ��
		@param props �߰��� �ΰ��� ������Ƽ
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
		������ �ΰŸ� �����ϴ� �Լ�.

		@param type ������ �ΰ��� Ÿ��
	 */
	virtual Logger *createLogger(const std::string &type);

};

#endif 
