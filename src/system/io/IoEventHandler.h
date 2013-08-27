/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 7
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoEventHandler.h
 *
 */

#ifndef _IO_EVENT_HANDLER_H_
#define _IO_EVENT_HANDLER_H_

#include "IoEventUtil.h"

class IoEventTarget;

class IoEventHandler
{
public:
	IoEventHandler() {};
	virtual ~IoEventHandler() {};

	virtual bool handleAccept(IoEventTarget *target) = 0;
	virtual bool handleConnect(IoEventTarget *target) = 0;
	virtual bool handleRead(IoEventTarget *target) = 0;
	virtual bool handleWrite(IoEventTarget *target) = 0;
	virtual bool handleClose(IoEventTarget *target) = 0;
	virtual bool handleOther(IoEventTarget *target, IoEvent evt) = 0;
};

#endif /* _IO_EVENT_HANDLER_H_ */
