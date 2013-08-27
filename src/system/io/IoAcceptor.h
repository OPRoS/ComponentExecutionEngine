/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoAcceptor.h
 *
 */

#ifndef IO_ACCEPTOR_H
#define IO_ACCEPTOR_H

#include <string>
#include <map>

#include "IoConnection.h"
#include "IoEventTargetManager.h"
#include "IoEventTarget.h"

class IoAcceptor : public IoEventTargetManager, public IoEventTarget
{
public:
	IoAcceptor() { setTargetEvent(IoEventUtil::acceptEvent()); 	setEventTargetManagerId("IoAcceptor"); };
	virtual ~IoAcceptor() {};

	virtual EventTargetType getType() { return EVT_TARGET_ACCEPTOR; };

	virtual IoEvent enabledEvent() { return IO_EVT_ACCEPT | IO_EVT_CLOSE; };

	virtual IoConnection *accept() = 0;

	virtual IoConnection *detachConnection(const std::string &cnnId);
	virtual bool attachConnection(IoConnection *cnn);
};

typedef std::map<std::string, IoAcceptor *> IoAcceptorMap;

#endif  //IO_ACCEPTOR_H
