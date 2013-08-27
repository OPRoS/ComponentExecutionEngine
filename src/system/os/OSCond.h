/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSAbstractMutex.h
 *
 */

#ifndef _OS_COND_H
#define _OS_COND_H

#include "OSSetting.h"
#include "OSMutex.h"

#ifdef OS_WIN32

typedef struct {
	// Count of the number of waiters.
	int waiters_count;

	// Serialize access to <waiters_count_>.
	CRITICAL_SECTION waiters_count_lock;

	// Number of threads to release via a <pthread_cond_broadcast>
	// or a <pthread_cond_signal>.
	int release_count;

	// Keeps track of the current "generation" so that we don't allow
	// one thread to steal all the "releases" from the broadcast.
	int wait_generation_count;

	// A manual-reset event that's used to block and release waiting
	// threads.
	HANDLE event;
} OSCondHandle;

#else // -------------  Linux/POSIX

#include "pthread.h"

typedef pthread_cond_t OSCondHandle;

#endif

/**
	Condition Variable 래핑 클래스.
*/
class OSCond
{
	OSCondHandle m_handle;
	bool m_destroyed;

public :
	OSCond(bool isInit = true);
	~OSCond();

	bool init();
	bool destroy();


	bool wait(OSMutex *mtx, unsigned long ms = 0);
	bool signal();
	bool broadcast();

	OSCondHandle getHandle() { return m_handle; };
	void setHandle(OSCondHandle handle) { m_handle = handle; };
};

#endif

