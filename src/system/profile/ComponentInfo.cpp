/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentInfo.cpp
 *
 */

#include "ComponentInfo.h"

using namespace std;

ComponentInfo::ComponentInfo()
{
}

ComponentInfo::~ComponentInfo()
{
}

void ComponentInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	try {
		type = pcfg->getAttribute("type", "unknown");

		// the name of the component
		name = pcfg->getChildValue("name");

		// the node id in which the component resides.
		node_id = pcfg->getChildValue("node_id", "");

		reference = pcfg->getChildValue("reference");
	}
	catch (InvalidArgumentException &e) {
		throw IOException("<ComponentInfo::parse> Excepton Occurred",e);
	}
}

void ComponentInfo::print(ostream &os, int level)
{
	printNV(os, level, "type", name);
	printNV(os, level, "name", name);
	printNV(os, level, "node_id", node_id);
	printNV(os, level, "reference", reference);
}
