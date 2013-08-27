/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2009 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 8. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : CoordinationInfo.cpp
 *
 */

#include "CoordinationInfo.h"

using namespace std;

CoordinationInfo::CoordinationInfo()
{
}

CoordinationInfo::~CoordinationInfo()
{
}

void CoordinationInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	try {
		sourceComponentName = pcfg->getAttribute("source");
		targetComponentName = pcfg->getAttribute("target");		 
	}
	catch (InvalidArgumentException e) {
		throw IOException("<CoordinationInfo::parse> Excepton Occurred ", e);
	}
}

void CoordinationInfo::print(ostream &os, int level)
{
	printNV(os, level, "sourceComponentName", sourceComponentName);

	printNV(os, level, "targetComponentName", targetComponentName);
}
