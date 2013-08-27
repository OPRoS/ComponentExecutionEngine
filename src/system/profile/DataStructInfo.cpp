/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataStructInfo.cpp
 *
 */

#include "DataStructInfo.h"

using namespace std;

DataStructInfo::DataStructInfo()
{
}

DataStructInfo::~DataStructInfo()
{
}

void DataStructInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	ConfigList list = pcfg->getChildren("item");
	ConfigList::iterator iter = list.begin();
	for (; iter != list.end(); ++iter) {
		DataStructItemInfo profile;
		profile.parse(*iter);
		itemList.push_back(profile);
	}
}

void DataStructInfo::print(ostream &os, int level)
{
	DataStructItemInfoList::iterator iter = itemList.begin();
	for (; iter != itemList.end(); ++iter) {
		(*iter).print(os, level+1);
		os << '\n';
	}
}
