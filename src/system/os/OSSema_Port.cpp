/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSSema.cpp
 *
 */

#include "OSSema.h"

OSSema::OSSema(bool isInit, long max, long initial)
{
	m_destroyed = true;
	if (isInit) init(max, initial);
}

OSSema::~OSSema()
{
	if (!m_destroyed) destroy();
}

#ifdef OS_WIN32

bool OSSema::init(long max, long initial)
{
	m_maxCount = max;
	m_initialCount = initial;
	m_handle = ::CreateSemaphore(NULL, m_initialCount, m_maxCount, NULL);

	if (m_handle == 0) return false;
	else {
		m_destroyed = false;
		return true;
	}
}

bool OSSema::destroy()
{
	if (m_destroyed) return true;

	m_destroyed = true;
	::CloseHandle(m_handle);
	return true;
}

bool OSSema::lock(unsigned long ms)
{
	if (ms == 0) ms = INFINITE;
	unsigned long d = ::WaitForSingleObject(m_handle, ms);
	if (d == WAIT_TIMEOUT || d == WAIT_FAILED) return false;

	return true;
}

bool OSSema::tryLock()
{
	unsigned long d = ::WaitForSingleObject(m_handle, 0);
	if (d == WAIT_TIMEOUT || d == WAIT_FAILED) return false;
	else return true;
}

bool OSSema::unlock()
{
	::ReleaseSemaphore(m_handle, 1, NULL);
	return true;
}

#elif  defined( OS_SEMA_POSIX) // --- OS_SEMA_POSIX

bool OSSema::init(long max, long initial)
{
	int pshared;

	m_maxCount = max;



	if (::sem_init(&m_handle, 0, m_maxCount) == -1) return false;
	else {
		m_destroyed = false;
		return true;
	}
}

bool OSSema::destroy()
{
	if (m_destroyed) return true;

	m_destroyed = true;
	return ::sem_destroy(&m_handle) == 0;
}

bool OSSema::lock(unsigned long ms)
{
	if (ms == 0) {
		return ::sem_wait(&m_handle) == 0;
	}
	else {
		struct timespec ts;
		ts.tv_sec = ms / 1000;
		ts.tv_nsec = (ms % 1000) * 10000000L;

		return sem_timedwait(&m_handle, &ts);
	}
}

bool OSSema::tryLock()
{
	return ::sem_trywait(&m_handle) == 0;
}

bool OSSema::unlock()
{
	return ::sem_post(&m_handle) == 0;
}

#else // --- OS_SEMA_EMULATE

bool OSSema::init(long max, long initial)
{
	m_initialCount = initial;
	m_maxCount = max;

	if (!m_handle.lock.init()) return false;
	if (!m_handle.count_nonzero.init())	{
		m_handle.lock.destroy();
		return false;
	}
	m_handle.count = initial;
	m_handle.waiters_count = 0;

	return true;
}

bool OSSema::destroy()
{
	m_destroyed = true;
	m_handle.lock.destroy();
	m_handle.count_nonzero.destroy();
	return false;
}

bool OSSema::lock(unsigned long ms)
{
	// Acquire mutex to enter critical section.
	m_handle.lock.lock();

	// Keep track of the number of waiters so that <sema_post> works correctly.
	m_handle.waiters_count++;

	if (ms == 0) {
		// Wait until the semaphore count is > 0, then atomically release
		// <lock_> and wait for <count_nonzero_> to be signaled.
		while (m_handle.count == 0) {
			m_handle.count_nonzero.wait(&m_handle.lock);
		}
	}
	else {
		// Wait until the semaphore count is > 0, then atomically release
		// <lock_> and wait for <count_nonzero_> to be signaled.
		while (m_handle.count == 0) {
			long startTime = (long) ms;
			if (ms <= 0 || !m_handle.count_nonzero.wait(&m_handle.lock, ms)) {
				// Release mutex to leave critical section.
				m_handle.lock.unlock();
				return false;
			}
			// ms -= currentTime - startTime;
			//
		}
	}

	// <m_handle.lock_> is now held.

	// Decrement the waiters count.
	m_handle.waiters_count--;

	// Decrement the semaphore's count.
	m_handle.count--;

	// Release mutex to leave critical section.
	m_handle.lock.unlock();

	return true;
}

bool OSSema::tryLock()
{
	// Acquire mutex to enter critical section.
	m_handle.lock.lock();

	// Wait until the semaphore count is > 0, then atomically release
	// <lock_> and wait for <count_nonzero_> to be signaled.
	if (m_handle.count == 0) {
		// Release mutex to leave critical section.
		m_handle.lock.unlock();
		return false;
	}

	// Decrement the semaphore's count.
	m_handle.count--;

	// Release mutex to leave critical section.
	m_handle.lock.unlock();

	return true;
}

bool OSSema::unlock()
{
	m_handle.lock.lock();

	// Always allow one thread to continue if it is waiting.
	if (m_handle.waiters_count > 0) {
		m_handle.count_nonzero.signal();
	}

	// Increment the semaphore's count.
	m_handle.count++;

	m_handle.lock.unlock();

	return true;
}

#endif
