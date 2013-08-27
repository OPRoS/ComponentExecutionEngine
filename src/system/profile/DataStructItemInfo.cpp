/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataStructItemInfo.cpp
 *
 */

#include "DataStructItemInfo.h"

using namespace std;

DataStructItemInfo::DataStructItemInfo()
{
}

DataStructItemInfo::~DataStructItemInfo()
{
}

void DataStructItemInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	try {
		type = pcfg->getAttribute("type");
		defaultValue = pcfg->getAttribute("default", "");
		name = pcfg->getValue("");
	}
	catch (InvalidArgumentException e) {
		throw IOException("<DataStructItemInfo::parse> Exception occurred...", e);
	}
}

void DataStructItemInfo::print(ostream &os, int level)
{
	printNV(os, level, "type", type);
	printNV(os, level, "defaultValue", defaultValue);
	printNV(os, level, "name", name);
}
