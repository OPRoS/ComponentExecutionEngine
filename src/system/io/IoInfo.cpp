/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoInfo.cpp
 *
 */

#include "IoInfo.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

bool IoInfo::load(Config *pcfg)
{
	if (pcfg == NULL) return true;

	try {
		enabled = pcfg->getAttributeAsBool("enable", true);
		if (enabled == false) return false;

		id = pcfg->getAttribute("id");
		protocol = pcfg->getAttribute("protocol", "tcp");
		role = pcfg->getAttribute("role", "server");

		pcfg->toProperties(props);
	}
	catch (Exception &ex) {
		enabled = false;
		log_error(ex);
		return false;
	}

	return true;
}
