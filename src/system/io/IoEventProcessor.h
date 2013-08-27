/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoEventProcessor.h
 *
 */

#ifndef _IO_EVENT_PROCESSOR_H_
#define _IO_EVENT_PROCESSOR_H_

#include <string>
#include "IoEventTarget.h"
#include "IoEventHandler.h"
#include "system/os/OSTimeVal.h"
#include "system/os/OSThread.h"

// IoEventProcessor
class IoEventProcessor : public OSThread
{
	std::string m_id;
	std::string m_type;
	long m_timeout;

public:
	IoEventProcessor() : m_timeout(0) {};
	virtual ~IoEventProcessor() {};

	void setId(const std::string &id) { m_id = id; };
	std::string getId() { return m_id; };

	void setType(const std::string &type) { m_type = type; };
	std::string getType() { return m_type; };

	void setTimeout(long to) { m_timeout = to; };
	long getTimeout() { return m_timeout; };

	virtual bool registerEventTarget(IoEventTarget *target) = 0;
	virtual bool unregisterEventTarget(IoEventTarget *target) = 0;

	virtual void processEvent() ;
	virtual void close() = 0;
};

#endif // _IO_EVENT_PROCESSOR_H_
