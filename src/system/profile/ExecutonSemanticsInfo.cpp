/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExecutonSemanticsInfo.cpp
 *
 */

#include "ExecutonSemanticsInfo.h"
#include "system/util/TraceUtil.h"
using namespace std;

ExecutonSemanticsInfo::ExecutonSemanticsInfo()
{
}

ExecutonSemanticsInfo::~ExecutonSemanticsInfo()
{
}

void ExecutonSemanticsInfo::parse(Config *pcfg)// throw (IOException)
{
	if (pcfg == NULL) return;

	type = pcfg->getChildValue("type","");
	if (type.compare("periodic") &&  type.compare("aperiodic") && type.compare("passive") & type.compare("periodic-shared") & type.compare("periodic-exclusive"))
	{
		if (!type.compare("non-periodic"))
		{
			tx_warn("non-periodic is changed to aperiodic");
			type="aperiodic";
		}
		else
		throw SystemException("Illegal execution_semantics.type is reported");
	}

	period = pcfg->getChildValueAsInt("period", -1);
	priority = pcfg->getChildValueAsInt("priority", -1);
	policy = pcfg->getChildValue("policy","SCHED_OTHER");
	creationType = pcfg->getChildValue("creation_type","");
	lifecycleType = pcfg->getChildValue("lifecycle_type","start");
}

void ExecutonSemanticsInfo::print(ostream &os, int level)
{
	printNV(os, level, "type", type);
	printNV(os, level, "period", intToStr(period));
	printNV(os, level, "priority", intToStr(priority));
	printNV(os, level, "policy", policy);
	printNV(os, level, "creationType", creationType);
	printNV(os, level, "lifecycle_type", lifecycleType);
}
