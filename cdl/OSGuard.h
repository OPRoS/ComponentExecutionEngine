/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSGuard.h
 *
 */

#ifndef _OS_GUARD_H
#define _OS_GUARD_H

#include "OSSetting.h"
#include "OSSync.h"

/**
	블럭 단위 락킹/해제를 수행하는 guard 구현 클래스.
*/
class OSGuard
{
private:
	OSSync *m_sync;
	bool m_isLocked;

public:
	OSGuard(OSSync *sync);
	~OSGuard();

	bool isLocked() { return m_isLocked; };
};

#endif
