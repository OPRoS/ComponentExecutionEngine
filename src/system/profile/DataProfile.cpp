/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataProfile.cpp
 *
 */

#include "DataProfile.h"
#include "system/Log.h"

using namespace std;

DataProfile::DataProfile()
{
}

DataProfile::~DataProfile()
{
}

bool DataProfile::doParse(Config *pcfg)
{
	if (pcfg == NULL) {
		log_error("<DataProfile::doParse> Config is NULL");
		return false;
	}

	try {
		ConfigList list = pcfg->getChildren("struct");
		ConfigList::iterator iter = list.begin();
		for (; iter != list.end(); ++iter) {
			DataStructInfo profile;
			profile.parse(*iter);
			structList.push_back(profile);
		}
	}
	catch (Exception &e) {
		log_error("<DataProfile::parse> Excepton Occurred <= " << e.getMessage());
		return false;
	}

	return true;
}

void DataProfile::print(ostream &os, int level)
{
	os << '\n';
	os << "========================================================================\n";
	os << '\n';
	printNV(os, level, "struct=========","");
	DataStructInfoList::iterator iter = structList.begin();
	for (; iter != structList.end(); ++iter) {
		(*iter).print(os, level+1);
		os << '\n';
	}
	os << "\n========================================================================";
	os << '\n';
}
