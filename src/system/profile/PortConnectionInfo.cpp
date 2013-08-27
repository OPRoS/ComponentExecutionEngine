/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : PortConnectionInfo.cpp
 *
 */

#include "PortConnectionInfo.h"

using namespace std;

PortConnectionInfo::PortConnectionInfo()
{
}

PortConnectionInfo::~PortConnectionInfo()
{
}

void PortConnectionInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	try {
		portType = pcfg->getAttribute("port_type", "method");
		ioType = pcfg->getAttribute("io_type", "");

		Config *cfg = pcfg->getChild("source");
		if (cfg == NULL) throw IOException("<PortConnectionInfo::parse> There is no source component");
		sourceComponentName = cfg->getAttribute("component_name");
		sourcePortName = cfg->getAttribute("port_name");

		cfg = pcfg->getChild("target");
		if (cfg == NULL) throw IOException("<PortConnectionInfo::parse> There is no target component");
		targetComponentName = cfg->getAttribute("component_name");
		targetPortName = cfg->getAttribute("port_name");
	}
	catch (InvalidArgumentException e) {
		throw IOException("<PortConnectionInfo::parse> Excepton Occurred ", e);
	}
}

void PortConnectionInfo::print(ostream &os, int level)
{
	printNV(os, level, "portType", portType);

	printNV(os, level, "sourceComponentName", sourceComponentName);
	printNV(os, level, "sourcePortName", sourcePortName);

	printNV(os, level, "targetComponentName", targetComponentName);
	printNV(os, level, "targetPortName", targetPortName);
}
