/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : CopyrightInfo.cpp
 *
 */

#include "CopyrightInfo.h"

using namespace std;

CopyrightInfo::CopyrightInfo()
{
}

CopyrightInfo::~CopyrightInfo()
{
}

void CopyrightInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	company.parse(pcfg->getChild("company"));
	licensePolicy = pcfg->getChildValue("license_policy","");
	version = pcfg->getChildValue("version","");
}

void CopyrightInfo::print(ostream &os, int level)
{
	printNV(os, level, "licensePolicy", licensePolicy);
	printNV(os, level, "version", version);

	printNV(os, level, "company==========", "");
	company.print(os, level+1);
}
