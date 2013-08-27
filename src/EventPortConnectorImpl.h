/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : EventPortConnectorImpl.h
 *
 *
 */

#ifndef EVENTPORTCONNECTORIMPL_H_
#define EVENTPORTCONNECTORIMPL_H_
#include "EventPortConnector.h"
#include "RemoteEventPort.h"
#include <string>
#include "system/cm/ComponentRequester.h"

class EventPortConnectorImpl : public EventPortConnector, public RemoteEventPort{
	std::string m_owner_name;
	std::string m_name;
	ComponentRequester *m_worker;
public:
	EventPortConnectorImpl(ComponentRequester *worker);
	virtual ~EventPortConnectorImpl();
	virtual void setTargetComponentName(const std::string &name);
	virtual std::string &getTargetComponentName();
	virtual void setTargetPortName(const std::string &name);
	virtual std::string &getTargetPortName();

	virtual ReturnType sendEvent(unsigned char *data, int len);
	virtual ReturnType recvEvent(unsigned char *data, int len);
	virtual ReturnType pushEvent(unsigned char *data, int size);
};

#endif /* EVENTPORTCONNECTORIMPL_H_ */
