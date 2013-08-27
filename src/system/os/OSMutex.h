/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSMutex.h
 *
 */

#ifndef _OS_MUTEX_H
#define _OS_MUTEX_H

#include "OSSetting.h"
#include "OSSync.h"

#ifdef OS_WIN32

typedef OSHandle OSMutexHandle;
typedef OSHandle OSMutexAttr;

#else // -------------  Linux/POSIX

#include <pthread.h>

typedef pthread_mutex_t OSMutexHandle;
typedef pthread_mutexattr_t OSMutexAttr;

#endif

/**
	뮤텍스 관련 함수 래핑 클래스.
*/
class OSMutex : public OSSync
{
	OSMutexHandle m_handle;
	OSMutexAttr m_attr;
	bool m_destroyed;

public :
	OSMutex(bool isInit = true);
	virtual ~OSMutex();

	bool init();
	bool destroy();

	virtual bool lock(unsigned long ms = 0);
	virtual bool tryLock();
	virtual bool unlock();

	OSMutexHandle *getHandle() { return &m_handle; }
	OSMutexAttr *getAttr() { return &m_attr; };
};

#endif

