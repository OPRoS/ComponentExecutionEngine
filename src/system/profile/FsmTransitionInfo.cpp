/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : FsmTransitionInfo.cpp
 *
 */

#include "FsmTransitionInfo.h"

using namespace std;

FsmTransitionInfo::FsmTransitionInfo()
{
}

FsmTransitionInfo::~FsmTransitionInfo()
{
}

void FsmTransitionInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	try {
		eventPortName = pcfg->getChildValue("event_port");
		eventType = pcfg->getChildValue("event_type", "");
		pcfg->trace_on = true;
		eventId = pcfg->getChildValue("event_id");
		pcfg->trace_on = false;
		guardFuntionName = pcfg->getChildValue("guard_function");
		nextState = pcfg->getChildValue("goto");
	}
	catch (InvalidArgumentException &e) {
		throw IOException("<FsmTransitionInfo::parse> Excepton Occurred", e);
	}
}

void FsmTransitionInfo::print(ostream &os, int level)
{
	printNV(os, level, "eventPortName", eventPortName);
	printNV(os, level, "eventType", eventType);
	printNV(os, level, "eventId", eventId);
	printNV(os, level, "guardFuntionName", guardFuntionName);
	printNV(os, level, "nextState", nextState);
}
