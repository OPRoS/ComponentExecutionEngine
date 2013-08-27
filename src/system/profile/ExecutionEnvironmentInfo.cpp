/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExecutionEnvironmentInfo.cpp
 *
 */

#include "ExecutionEnvironmentInfo.h"

using namespace std;

ExecutionEnvironmentInfo::ExecutionEnvironmentInfo()
{
}

ExecutionEnvironmentInfo::~ExecutionEnvironmentInfo()
{
}

void ExecutionEnvironmentInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	Config *cfg = pcfg->getChild("os");
	if (cfg != NULL) {
		osName = cfg->getAttribute("name","");
		osVersion = cfg->getAttribute("version","");
	}
	cpu = pcfg->getChildValue("cpu","");
	libraryType = pcfg->getChildValue("library_type","");
	libraryName = pcfg->getChildValue("library_name","");
	implLanguage = pcfg->getChildValue("impl_language","");
	compiler = pcfg->getChildValue("compiler","");
}

void ExecutionEnvironmentInfo::print(ostream &os, int level)
{
	printNV(os, level, "osName", osName);
	printNV(os, level, "osVersion", osVersion);
	printNV(os, level, "cpu", cpu);
	printNV(os, level, "libraryType", libraryType);
	printNV(os, level, "libraryName", libraryName);
	printNV(os, level, "implLanguage", implLanguage);
	printNV(os, level, "compiler", compiler);
}
