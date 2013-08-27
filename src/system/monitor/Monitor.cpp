/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Monitor.cpp
 *
 */


#include "Monitor.h"

#include "system/util/Exceptions.h"
#include "system/util/Properties.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

Monitor::Monitor()
{
}

Monitor::~Monitor()
{
}

bool Monitor::processCommand(MonitorWorker *mw, Properties &props)
{
	OSGuard guard(&m_mutex);

	try {
		doCommand(mw, props);
	}
	catch (Exception &ex) {
		log_error(ex);
		mw->returnFail(props, ex.getMessage());
	}

	return true;
}