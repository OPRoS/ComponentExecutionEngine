/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MethodProfile.cpp
 *
 */

#include "MethodProfile.h"
#include "system/Log.h"

using namespace std;

MethodProfile::MethodProfile()
{
}

MethodProfile::~MethodProfile()
{
}

bool MethodProfile::doParse(Config *pcfg)
{
	if (pcfg == NULL) return false;

	try
	{
		name = pcfg->getChildValue("name");

		ConfigList list = pcfg->getChildren("method");
		ConfigList::iterator iter = list.begin();
		for (; iter != list.end(); ++iter) {
			MethodInfo profile;
			profile.parse(*iter);
			methodList.push_back(profile);
		}
	}
	catch (Exception &e) {
		log_error("<MethodProfile::parse> Excepton Occurred <= " << e.getMessage());
		return false;
	}
	return true;
}

void MethodProfile::print(ostream &os, int level)
{
	os << '\n';
	os << "========================================================================\n";
	os << '\n';
	printNV(os, level, "name", name);

	MethodInfoList::iterator iter = methodList.begin();
	for (; iter != methodList.end(); ++iter) {
		printNV(os, level, "method =========","");
		(*iter).print(os, level+1);
		os << '\n';
	}
	os << "\n========================================================================";
	os << '\n';
}
