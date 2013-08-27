/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 4
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoEventTargetListener.h
 *
 */

#ifndef _IO_EVENT_TARGET_LISTENER_H_
#define _IO_EVENT_TARGET_LISTENER_H_

#include <list>

class IoEventTarget;

class IoEventTargetListener {
public:
	virtual void onOpened(IoEventTarget *ioObj) = 0;
	virtual void onClosed(IoEventTarget *ioObj) = 0;
};

typedef std::list<IoEventTargetListener *> IoEventTargetListenerList;

#endif /* _IO_EVENT_TARGET_LISTENER_H_ */
