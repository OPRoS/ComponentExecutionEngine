/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : CompanyInfo.cpp
 *
 */

#include "CompanyInfo.h"

using namespace std;

CompanyInfo::CompanyInfo()
{
}

CompanyInfo::~CompanyInfo()
{
}

void CompanyInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	name = pcfg->getChildValue("name","");
	phone = pcfg->getChildValue("phone","");
	fax = pcfg->getChildValue("fax","");
	address = pcfg->getChildValue("address","");
	homepage = pcfg->getChildValue("homepage","");
}

void CompanyInfo::print(ostream &os, int level)
{
	printNV(os, level, "name", name);
	printNV(os, level, "phone", phone);
	printNV(os, level, "fax", fax);
	printNV(os, level, "address", address);
	printNV(os, level, "homepage", homepage);
}
