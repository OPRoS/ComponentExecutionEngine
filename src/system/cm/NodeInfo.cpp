/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : NodeInfo.cpp
 *
 */

#include "NodeInfo.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

bool NodeInfo::load(Config *pcfg)
{
	if (pcfg == NULL) return false;

	try {
		node_id= pcfg->getAttribute("id");
		io.protocol = pcfg->getAttribute("protocol");
		io.role = "client";
		io.enabled = true;
		pcfg->toProperties(io.props);
	}
	catch (Exception &ex) {
		log_error(ex);
		return false;
	}

	return true;
}

NodeInfo::~NodeInfo()
{
}
