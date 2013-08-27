/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataTypeInfo.cpp
 *
 */

#include "DataTypeInfo.h"

using namespace std;

DataTypeInfo::DataTypeInfo()
{
}

DataTypeInfo::~DataTypeInfo()
{
}

void DataTypeInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	filename = pcfg->getValue("");
}

void DataTypeInfo::print(ostream &os, int level)
{
	printNV(os, level, "filename", filename);
}
