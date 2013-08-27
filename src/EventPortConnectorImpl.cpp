/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : EventPortConnectorImpl.cpp
 *
 *
 */

#include "EventPortConnectorImpl.h"

EventPortConnectorImpl::EventPortConnectorImpl(ComponentRequester *worker) {
	m_worker = worker;

}

EventPortConnectorImpl::~EventPortConnectorImpl() {
	// TODO Auto-generated destructor stub
}

ReturnType EventPortConnectorImpl::sendEvent(unsigned char *data, int len)
{
	if (!m_worker->eventCall(m_owner_name, m_name, data, len))
	{
		return OPROS_INTERNAL_FAULT;
	}

	return OPROS_SUCCESS;
}

ReturnType EventPortConnectorImpl::recvEvent(unsigned char *data, int len)
{
	return OPROS_UNSUPPORTED_METHOD;
}

void EventPortConnectorImpl::setTargetComponentName(const std::string &name)
{
	m_owner_name = name;
}

std::string &EventPortConnectorImpl::getTargetComponentName()
{
	return 	m_owner_name;
}

void EventPortConnectorImpl::setTargetPortName(const std::string &name)
{
	m_name = name;
}

std::string &EventPortConnectorImpl::getTargetPortName()
{
	return m_name;
}
ReturnType EventPortConnectorImpl::pushEvent(unsigned char *data, int size)
{
	return sendEvent(data,size);
}
