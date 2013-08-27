/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 24
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : LogService.h
 *
 */

#ifndef LOG_SERVICE_H_
#define LOG_SERVICE_H_

#include "system/service/Service.h"
#include "LoggerFactory.h"

/**
	컴포넌트 나름대로의 LogFactory를 만들어 사용하고 싶을 때,
	LogFactory는 시스템이 제공하는 것 중 하나를 사용하기 위한 클래스.
	예) DefaultLogFactory, Log4jLogFactory 등
	현재는 구현되어 있지 않음.
*/
using namespace opros::engine;

class LogService: public Service {
	LoggerFactory *m_loggerFac;

public:
	LogService();
	virtual ~LogService();

	void setLoggerFactory(LoggerFactory *fac) { m_loggerFac = fac; };
	LoggerFactory *getLoggerFactory() { return m_loggerFac; };

};

#endif 
