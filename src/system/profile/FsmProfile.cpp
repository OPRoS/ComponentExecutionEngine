/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : FsmProfile.cpp
 *
 */

#include "FsmProfile.h"
#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

FsmProfile::FsmProfile()
{
	name = "";
}

FsmProfile::~FsmProfile()
{
	FsmStateInfoList::iterator iter = stateList.begin();
	for (; iter != stateList.end(); ++iter) {
		delete *iter;
	}
}

bool FsmProfile::doParse(Config *pcfg)
{
	if (pcfg == NULL) return false;

	try {
		Config *cfg = pcfg->getChild("name");
		if (cfg == NULL) {
			log_error("<FsmProfile::parse> There is no name");
			return false;
		}
		name = cfg->getValue();
		description = pcfg->getChildValue("description", "");

		ConfigList list = pcfg->getChildren("state");
		ConfigList::iterator iter = list.begin();
		for (; iter != list.end(); ++iter) {
			FsmStateInfo *info = new FsmStateInfo();
			info->parse(*iter);
			stateList.push_back(info);
		}
	}
	catch (Exception &e) {
		log_error("<FsmProfile::parse> Excepton Occurred <= " << e.getMessage());
		return false;
	}
	return true;
}

void FsmProfile::print(ostream &os, int level)
{
	os << '\n';
	os << "========================================================================\n";
	printNV(os, level, "name", name);
	printNV(os, level, "description", description);

	os << '\n';
	FsmStateInfoList::iterator sIter = stateList.begin();
	for (; sIter != stateList.end(); ++sIter) {
		printNV(os, level, "state=========","");
		(*sIter)->print(os, level+1);
	}

	os << "\n========================================================================";
	os << '\n';
}
