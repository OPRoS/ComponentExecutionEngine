/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 7
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoSelector.h
 *
 */

#ifndef _IO_SELECTOR_H_
#define _IO_SELECTOR_H_

#include <vector>
#include <map>

#include "system/os/OSMutex.h"

#include "IoEventProcessor.h"
#include "IoEventTarget.h"

#define IO_SELECTOR_TIMEOUT 1000 // 1 sec

class IoSelector : public IoEventProcessor
{
	typedef std::map<OSSocketHandle, IoEventTarget *> IoEventTargetSelectorMap;
	typedef std::vector<IoEventTarget *> IoEventTargetList;

	IoEventTargetSelectorMap m_readyList;

	OSMutex m_mutex;
	OSSocketHandle m_maxSockHandle;

	fd_set m_rset;
	fd_set m_wset;
	fd_set m_eset;

	bool m_closed;

	void removeEventTarget(IoEventTarget *target);
	void removeEventTargetAll();

public:
	IoSelector();
	virtual ~IoSelector();

	virtual bool registerEventTarget(IoEventTarget *target);
	virtual bool unregisterEventTarget(IoEventTarget *target);

	virtual void run();
	virtual void close();
};

#endif 

