/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSThread.cpp
 *
 */

#include "OSThread.h"

#include "system/Log.h"
#include "system/Trace.h"

#ifdef OS_WIN32 

static DWORD win32ThreadStartRoutine (void* param)
{
	OSRunnable *ro = (OSRunnable *) param;
	ro->runStarted();
	ro->run();
	ro->runFinished();

	if (ro->isOnFinished()) {
		delete ro;
	}

	ro = NULL;

	::Sleep(0);

	return 0;
}

bool OSThread::threadStart()
{
	if (m_runnable == NULL) {
		m_handle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) win32ThreadStartRoutine, (PVOID) this, 0, &m_id);
	}
	else {
		m_handle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) win32ThreadStartRoutine, (PVOID) m_runnable, 0, &m_id);
	}

	if (m_handle != NULL) return true;
	else return false;
}

bool OSThread::threadStop()
{
	if (TerminateThread(m_handle, 0)) return true;
	else return false;
}

bool OSThread::threadSuspend()
{
	if (SuspendThread(m_handle)) return true;
	else return false;
}

bool OSThread::threadResume()
{
	if (ResumeThread(m_handle)) return true;
	else return false;
}

void OSThread::threadYield()
{
	::Sleep(0);
}

bool OSThread::threadDetach()
{
	return true;
}

int OSThread::threadJoin(OSThreadId waiter_tid, ThreadFunctionReturn *status)
{
	return 0;
}

int OSThread::threadJoin(ThreadFunctionReturn *status)
{
	DWORD local_status;

	// Make sure that status is non-NULL.
	if (status == 0) status = &local_status;

	if (::WaitForSingleObject (m_handle, INFINITE) == WAIT_OBJECT_0
	  && ::GetExitCodeThread (m_handle, status) != FALSE)
	{
	  ::CloseHandle (m_handle);
	  return 0;
	}

	return 0;
}

OSThreadHandle OSThread::getCurrentThread()
{
	return ::GetCurrentThread();
}

#else
#include <cstring>

static void*  posixThreadStartRoutine (void* param)
{
	OSRunnable *ro = (OSRunnable *) param;
	ro->runStarted();
	ro->run();
	ro->runFinished();

	return 0;
}

bool OSThread::threadStart()
{
	trace_enter();

	pthread_attr_t attr;
	if (::pthread_attr_init(&attr) != 0) {
		log_error("pthread init failed");
		return false;
	}

#if defined (_POSIX_THREAD_PRIORITY_SCHEDULING)
	int min_p, max_p;

	// thread priority setting
	if (m_schedPolicy == SCHED_FIFO || m_schedPolicy == SCHED_RR || m_schedPolicy == SCHED_OTHER) {
		if (::pthread_attr_setschedpolicy (&attr, m_schedPolicy) != 0) {
			::pthread_attr_destroy (&attr);
			return false;
		}

		min_p = ::sched_get_priority_min(m_schedPolicy);
		max_p = ::sched_get_priority_max(m_schedPolicy);
		if (min_p != -1 && m_priority < min_p) m_priority = min_p;
		if (max_p != -1 && m_priority > max_p) m_priority = max_p;


		struct sched_param sparam;
		memset ((void *) &sparam, 0, sizeof sparam);
		sparam.sched_priority = m_priority;
		if (::pthread_attr_setschedparam (&attr, &sparam) != 0) {
			::pthread_attr_destroy (&attr);
		    return false;
		}
	}
#endif

	if (m_runnable == NULL) {
		if (::pthread_create (&m_handle, &attr, posixThreadStartRoutine , (void *)  this) != 0) {
			log_error("pthread create failed");
			return false;
		}
	}
	else {
		if (::pthread_create (&m_handle, &attr, posixThreadStartRoutine , (void *)  m_runnable) != 0) {
			log_error("pthread create failed");
			return false;
		}
	}

	::pthread_attr_destroy (&attr);

	m_id = m_handle;

	return true;
}

bool OSThread::threadStop()
{
#ifdef ANDROID
	if (::pthread_kill(m_handle, SIGSTOP) != 0) return true;
	else return false;
#else
	if (::pthread_cancel(m_handle) != 0) return true;
	else return false;
#endif
}

bool OSThread::threadSuspend()
{
//	if (::pthread_suspend(m_handle) != 0) return true;
//	else return false;
	return true;
}

bool OSThread::threadResume()
{
//	if (::pthread_resume(m_handle) != 0) return true;
//	else return false;

	return true;
}

void OSThread::threadYield()
{
	//::pthread_yield(m_handle);
}

bool OSThread::threadDetach()
{
	if (::pthread_detach(m_handle) != 0) return true;
	else return false;
}

int OSThread::threadJoin(OSThreadId waiter_tid, ThreadFunctionReturn *status)
{
	return 0;
}

int OSThread::threadJoin(ThreadFunctionReturn *status)
{
	ThreadFunctionReturn local_status;

	// Make sure that status is non-NULL.
	if (status == 0) status = &local_status;

	return ::pthread_join(m_handle, status);
}

OSThreadHandle OSThread::getCurrentThread()
{
	return ::pthread_self();
}

#endif


