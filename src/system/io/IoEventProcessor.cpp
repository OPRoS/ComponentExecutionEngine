/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 12
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoEventProcessor.cpp
 *
 */

#include "IoEventProcessor.h"

#include "system/Log.h"
#include "system/Trace.h"

void IoEventProcessor::processEvent()
{
	trace_enter();

	threadName("IoEventProcessor");
	threadStart();
}
