/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MethodInfo.cpp
 *
 */

#include "MethodInfo.h"

using namespace std;

MethodInfo::MethodInfo()
{
}

MethodInfo::~MethodInfo()
{
}

void MethodInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	try {
		name = pcfg->getAttribute("name");
		returnType = pcfg->getAttribute("return_type");

		ConfigList list = pcfg->getChildren("param");
		ConfigList::iterator iter = list.begin();
		for (; iter != list.end(); ++iter) {
			ParamInfo profile;
			profile.parse(*iter);
			paramList.push_back(profile);
		}
	}
	catch (InvalidArgumentException e) {
		throw IOException("MethodInfo::parse : Excepton Occurred", e);
	}
}

void MethodInfo::print(ostream &os, int level)
{
	printNV(os, level, "name", name);
	printNV(os, level, "returnType", returnType);
	ParamInfoList::iterator iter = paramList.begin();
	for (; iter != paramList.end(); ++iter) {
		printNV(os, level, "param =========","");
		(*iter).print(os, level+1);
		os << '\n';
	}
}

