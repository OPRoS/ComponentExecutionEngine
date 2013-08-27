/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoConnector.h
 *
 */

#ifndef _IO_CONNECTOR_H_
#define _IO_CONNECTOR_H_

#include <string>
#include <map>

#include "IoConnection.h"
#include "IoEventTarget.h"

class IoConnector : public IoEventTarget
{
public:
	IoConnector() { setTargetEvent(IoEventUtil::connectEvent()); };
	virtual ~IoConnector() {};

	virtual EventTargetType getType() { return EVT_TARGET_CONNECTOR; };

	virtual IoEvent enabledEvent() { return IO_EVT_CONNECT | IO_EVT_CLOSE; };

	virtual IoConnection *connect() = 0;

	virtual IoConnection *detachConnection() = 0;
	virtual bool attachConnection(IoConnection *cnn) = 0;
};

typedef std::map<std::string, IoConnector *> IoConnectorMap;

#endif  //_IO_CONNECTOR_H_
