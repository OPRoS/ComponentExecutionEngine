/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ParamInfo.cpp
 *
 */

#include "ParamInfo.h"

using namespace std;

ParamInfo::ParamInfo()
{
}

ParamInfo::~ParamInfo()
{
}

void ParamInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	try {
		index = pcfg->getAttributeAsInt("index", 0);
		type = pcfg->getChildValue("type");
		name = pcfg->getChildValue("name", "");
		defaultValue = pcfg->getChildValue("default", "");
	}
	catch (InvalidArgumentException e) {
		throw IOException("<ParamInfo::parse> Exception Occurred", e);
	}
}

void ParamInfo::print(ostream &os, int level)
{
	printNV(os, level, "index", intToStr(index));
	printNV(os, level, "type", type);
	printNV(os, level, "name", name);
	printNV(os, level, "defaultValue", defaultValue);
}
