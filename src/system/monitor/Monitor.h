/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Monitor.h
 *
 */

#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <string>

#include "system/Manager.h"
#include "system/util/Properties.h"
#include "system/os/OSMutex.h"

#include "MonitorWorker.h"

/**
	원격에서 컴포넌트 실행엔진에 전달된 명령을 처리하는 클래스.	
*/ 
class Monitor 
{
protected:
	/** 요청 처리를 순차적으로 처리하기 위한 뮤텍스 */
	OSMutex m_mutex;

	/** 요청을 실제 처리하는 함수. */
	virtual bool doCommand(MonitorWorker *mw, Properties &props) = 0;

public:
	Monitor();
	virtual ~Monitor();

	/** 
		요청을 처리하는 함수. 
		요청을 순차적으로 처리하기 위해 뮤텍스에 락을 걸고
		요청이 종료되면 락을 해제함.
	*/
	virtual bool processCommand(MonitorWorker *mw, Properties &props);
};

#endif  
