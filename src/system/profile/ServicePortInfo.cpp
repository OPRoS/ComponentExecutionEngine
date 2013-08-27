/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ServicePortInfo.cpp
 *
 */

#include "ServicePortInfo.h"

using namespace std;

ServicePortInfo::ServicePortInfo()
{
}

ServicePortInfo::ServicePortInfo(const ServicePortInfo &profile)
{
	name = profile.name;
	type = profile.type;;
	usage = profile.usage;
	reference = profile.reference;
}

ServicePortInfo::~ServicePortInfo()
{
}

void ServicePortInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	name = pcfg->getChildValue("name","");
	type = pcfg->getChildValue("type","");
	usage = pcfg->getChildValue("usage","");
	reference = pcfg->getChildValue("reference","");
}

void ServicePortInfo::print(ostream &os, int level)
{
	printNV(os, level, "name", name);
	printNV(os, level, "type", type);
	printNV(os, level, "usage", usage);
	printNV(os, level, "reference", reference);
}
