/*
 * Executor.h
 *
 *  Created on: 2009. 3. 9
 *      Author: jangcs@etri.re.kr
 */

#ifndef EXECUTOR_H_
#define EXECUTOR_H_

//#define _WIN32_WINNT 0x0500     // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#ifdef WIN32

//#ifndef __GNUC__
//#include "stdafx.h"
#ifdef __MINGW32__
#define _WIN32_WINNT 0x0500 // for MinGW
#endif
#include <windows.h>
#include <mmsystem.h>

#define USE_QUEUE_TIMER
//#endif
#endif

#ifdef NO_BOOST
#include <system/os/OSMutex.h>
#include <system/os/OSCond.h>
#else
#include <boost/thread/thread.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#endif

#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <list>
#include <string>

#include <Component.h>
#include <ReturnType.h>
#include <OPRoSTypes.h>
#include "ExecutorStarter.h"
#include <system/cm/ComponentAdapter.h>
#include "ThreadManager.h"

#include "ExecutorStarter.h"

using namespace std;

static int executorId = 0;

#define OPROS_SCHED_OTHER  0
#define OPROS_SCHED_RR       1
#define OPROS_SCHED_FIFO     2


class Executor {
public:
/*
	const static int CREATED	 	= 0;
//	const static int READY			= 1;  // 실행기는 READY 상태가 필요 없음
	const static int ACTIVE 		= 2;
	const static int INACTIVE 		= 3;
	const static int ERROR			= 4;
	const static int DESTROYED 		= 5;

	const static int PREPARE_TO_DESTROY = 99; // 실행기를 멈추기 위한 내부 상태
*/


public:
	Executor();
	Executor(int priority, int period, int sched_policy);
	virtual ~Executor();


	ReturnType add(ComponentAdapter *pComp);

	ReturnType remove(ComponentAdapter *pComp);
	ReturnType remove(const string &compID);

	ReturnType start(ComponentAdapter *pComp);
	ReturnType start();


	ReturnType stop(ComponentAdapter *pComp);
	ReturnType stop();

	ReturnType reset(ComponentAdapter *pComp);
	ReturnType reset();

	ReturnType destroy();

	ReturnType setPriority(int priority);
	int getPriority();

	ReturnType setPeriod(int period);
	int getPeriod();

	void run();

	bool isEmpty();

	bool m_bExecuting;
	int m_executorId;
	int m_sched_policy;

protected:
	virtual void executeComponents();
	virtual ReturnType destroyComponents();

protected:
	ExecutorState m_runningState;
	bool m_runActState;
	int m_priority;
	int m_period;

#ifdef USE_QUEUE_TIMER
	HANDLE m_hTimerHandle;
#endif

	list<ComponentAdapter *>  m_compList;

#ifdef NO_BOOST
	OSMutex m_mutexForList;
	OSMutex m_mutex;
	OSCond m_cond;
#else
	boost::mutex m_mutexForList;
	boost::mutex m_mutex;
	boost::condition_variable m_cond;
#endif

#ifdef ANDROID
	pthread_mutex_t m_rt_mutex;
	pthread_cond_t m_rt_cond;
#endif

	ExecutorStarter m_executorStarter;
};


#endif /* EXECUTOR_H_ */
