/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : FsmStateInfo.cpp
 *
 */

#include "FsmStateInfo.h"

using namespace std;

FsmStateInfo::FsmStateInfo()
{
}

FsmStateInfo::~FsmStateInfo()
{
	FsmTransitionInfoList::iterator iter = transitionList.begin();
	for (; iter != transitionList.end(); ++iter) {
		delete *iter;
	}
}

void FsmStateInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	try {
		type = pcfg->getAttribute("type", "normal");

		Config *cfg = pcfg->getChild("name");
		if (cfg == NULL) throw IOException("There is no name");
		name = cfg->getValue();

		cfg = pcfg->getChild("description");
		if (cfg != NULL) description = cfg->getValue();

		ConfigList list = pcfg->getChildren("transition");
		ConfigList::iterator iter = list.begin();
		for (; iter != list.end(); ++iter) {
			FsmTransitionInfo *info = new FsmTransitionInfo();
			info->parse(*iter);
			transitionList.push_back(info);
		}
	}
	catch (InvalidArgumentException e) {
		throw IOException("<FsmStateInfo::parse> Excepton Occurred", e);
	}
}

void FsmStateInfo::print(ostream &os, int level)
{
	printNV(os, level, "type", type);
	printNV(os, level, "name", name);
	printNV(os, level, "description", description);

	os << '\n';
	FsmTransitionInfoList::iterator tIter = transitionList.begin();
	for (; tIter != transitionList.end(); ++tIter) {
		printNV(os, level, "transition=========","");
		(*tIter)->print(os, level+1);
	}
}
