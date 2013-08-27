/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : EventPortInfo.cpp
 *
 */

#include "EventPortInfo.h"

using namespace std;

EventPortInfo::EventPortInfo()
{
}

EventPortInfo::~EventPortInfo()
{
}

void EventPortInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	name = pcfg->getChildValue("name","");
	usage = pcfg->getChildValue("usage","");
	dataType = pcfg->getChildValue("data_type","");
	queueingPolicy = pcfg->getChildValue("queueing_policy","");
	queueSize = pcfg->getChildValueAsInt("queue_size", 1);
}

void EventPortInfo::print(ostream &os, int level)
{
	printNV(os, level, "name", name);
	printNV(os, level, "usage", usage);
	printNV(os, level, "dataType", dataType);
	printNV(os, level, "queueingPolicy", queueingPolicy);
	printNV(os, level, "queueSize", intToStr(queueSize));
}
