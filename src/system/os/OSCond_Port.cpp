/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSCond.cpp
 *
 */

#include "OSCond.h"
#include "OSGuard.h"

OSCond::OSCond(bool isInit)
{
	m_destroyed = true;
	if (isInit) init();
}

OSCond::~OSCond()
{
	if (!m_destroyed) destroy();
}

#ifdef OS_WIN32


bool OSCond::init()
{
	if (m_destroyed==false) return true;
	m_handle.waiters_count = 0;
	m_handle.wait_generation_count = 0;
	m_handle.release_count = 0;

	::InitializeCriticalSection (& (m_handle.waiters_count_lock));

	// Create a manual-reset event.
	m_handle.event = ::CreateEvent (NULL,  // no security
	                            TRUE,  // manual-reset
	                            FALSE, // non-signaled initially
	                            NULL); // unnamed

	if (m_handle.event == 0) return false;
	else {
		m_destroyed = false;
		return true;
	}
}

bool OSCond::destroy()
{
	if (m_destroyed) return true;

   ::DeleteCriticalSection(&m_handle.waiters_count_lock);
	
	if (m_handle.event!=0)
	::CloseHandle(m_handle.event);
	m_destroyed = true;
	
	
	return true;
}

bool OSCond::wait(OSMutex *mtx, unsigned long ms)
{
	// Avoid race conditions.
	::EnterCriticalSection (&m_handle.waiters_count_lock);

	// Increment count of waiters.
	m_handle.waiters_count++;

	// Store current generation in our activation record.
	int my_generation = m_handle.wait_generation_count;

	::LeaveCriticalSection (&m_handle.waiters_count_lock);
	mtx->unlock();

	for (;;) {
		unsigned long d;
		if (ms == 0) {
			// Wait until the event is signaled.
			d = ::WaitForSingleObject (m_handle.event, INFINITE);
		}
		else {
			// Wait until the event is signaled.
			d = ::WaitForSingleObject (m_handle.event, ms);
		}

		::EnterCriticalSection (&m_handle.waiters_count_lock);

		if (d == WAIT_TIMEOUT || d == WAIT_FAILED) {
			::LeaveCriticalSection (&m_handle.waiters_count_lock);
			return false;
		}

		// Exit the loop when the <m_handle.event> is signaled and
		// there are still waiting threads from this <wait_generation>
		// that haven't been released from this wait yet.
		int wait_done = m_handle.release_count > 0
						&& m_handle.wait_generation_count != my_generation;
		::LeaveCriticalSection (&m_handle.waiters_count_lock);

		if (wait_done)  break;
	}

	mtx->lock();

	::EnterCriticalSection (&m_handle.waiters_count_lock);

	m_handle.waiters_count--;
	m_handle.release_count--;
	int last_waiter = m_handle.release_count == 0;
	::LeaveCriticalSection (&m_handle.waiters_count_lock);

	// We're the last waiter to be notified,
	// so reset the manual event.
	if (last_waiter) ResetEvent (m_handle.event);

	return true;
}

bool OSCond::signal()
{
	::EnterCriticalSection (&m_handle.waiters_count_lock);
	if (m_handle.waiters_count > m_handle.release_count) {
		::SetEvent (m_handle.event); // Signal the manual-reset event.
		m_handle.release_count++;
		m_handle.wait_generation_count++;
	}
	::LeaveCriticalSection (&m_handle.waiters_count_lock);

	return true;
}

bool OSCond::broadcast()
{
	::EnterCriticalSection (&m_handle.waiters_count_lock);
	if (m_handle.waiters_count > 0) {
		::SetEvent (m_handle.event);
		// Release all the threads in this generation.
		m_handle.release_count = m_handle.waiters_count;

		// Start a new generation.
		m_handle.wait_generation_count++;
	}
	::LeaveCriticalSection (&m_handle.waiters_count_lock);

	return true;
}

#else // POSIX
#include <sys/time.h>
bool OSCond::init()
{
	if (m_destroyed==false) return true;
	if (::pthread_cond_init(&m_handle, NULL) != 0) return false;
	else {
		m_destroyed = false;
		return true;
	}
}

bool OSCond::destroy()
{
	if (m_destroyed) return true;

	m_destroyed = true;
	return ::pthread_cond_destroy(&m_handle) == 0;
}


bool OSCond::wait(OSMutex *mutex, unsigned long ms)
{
	if (ms ==0) {
		return ::pthread_cond_wait(&m_handle,  mutex->getHandle()) == 0;
	}
	else {
		struct timespec ts ;
		struct timeval now;

		gettimeofday(&now,NULL);

		unsigned long sec_t = ms / 1000;
		unsigned long s_ms = ms - sec_t * 1000;

		ts.tv_sec = now.tv_sec + sec_t;
		ts.tv_nsec = now.tv_usec * 1000 +  s_ms * 1000000;

		return ::pthread_cond_timedwait(&m_handle, mutex->getHandle(), &ts) == 0;
	}
}

bool OSCond::signal()
{
	return ::pthread_cond_signal(&m_handle) == 0;
}

bool OSCond::broadcast()
{
	return ::pthread_cond_broadcast(&m_handle) == 0;
}

#endif
