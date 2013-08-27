/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : AppDomainInfo.cpp
 *
 */

#include "AppDomainInfo.h"

using namespace std;

AppDomainInfo::AppDomainInfo()
{
}

AppDomainInfo::~AppDomainInfo()
{
}

void AppDomainInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	domain = pcfg->getChildValue("domain","");
	robotType = pcfg->getChildValue("robot_type","");
}

void AppDomainInfo::print(ostream &os, int level)
{
	printNV(os, level, "domain", domain);
	printNV(os, level, "robotType", robotType);
}
