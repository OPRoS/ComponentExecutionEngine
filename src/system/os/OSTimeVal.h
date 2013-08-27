/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 8
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSTimeVal.h
 *
 */

#ifndef _OS_TIME_VAL_H_
#define _OS_TIME_VAL_H_

#include "OSSetting.h"
#include <ctime>

#ifdef OS_WIN32

#include <time.h>

#else 

#include <sys/time.h>

#endif

/**
	timeval 관련 함수를 래핑한 클래스.
*/
class OSTimeVal
{
	struct timeval m_tv;

public:
	OSTimeVal();
	OSTimeVal(long sec, long usec);
	virtual ~OSTimeVal();

	void setSec(long sec) { m_tv.tv_sec = sec; };
	long getSec() { return m_tv.tv_sec; };

	void setUSec(long usec) { m_tv.tv_usec = usec; };
	long getUSec() { return m_tv.tv_usec; };

	struct timeval getTimeVal() { return m_tv; };

};

#endif 
