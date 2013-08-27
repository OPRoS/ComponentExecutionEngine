/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : FsmInfo.cpp
 *
 */

#include "FsmInfo.h"

using namespace std;

FsmInfo::FsmInfo()
{
}

FsmInfo::~FsmInfo()
{
}

void FsmInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	name = pcfg->getChildValue("name","");
	type = pcfg->getChildValue("type","");
	reference = pcfg->getChildValue("reference","");
}

void FsmInfo::print(ostream &os, int level)
{
	printNV(os, level, "name", name);
	printNV(os, level, "type", type);
	printNV(os, level, "reference", reference);
}
