/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 8
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSTimeVal.cpp
 *
 */

#include "OSTimeVal.h"

OSTimeVal::OSTimeVal()
{
}

OSTimeVal::OSTimeVal(long sec, long usec)
{
	m_tv.tv_sec = sec;
	m_tv.tv_usec = usec;
}

OSTimeVal::~OSTimeVal()
{
}

