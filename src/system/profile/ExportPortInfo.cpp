/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExportPortInfo.cpp
 *
 */

#include "ExportPortInfo.h"

using namespace std;

ExportPortInfo::ExportPortInfo()
{
}

ExportPortInfo::~ExportPortInfo()
{
}

void ExportPortInfo::parse(Config *pcfg) throw (IOException)
{
	if (pcfg == NULL) return;

	try {
		componentName = pcfg->getChildValue("component_name");
		portName = pcfg->getChildValue("port_name");
		portType = pcfg->getChildValue("port_type");
		portDir = pcfg->getChildValue("port_dir");
		exportPortName = pcfg->getChildValue("export_port_name", portName);
	}
	catch (InvalidArgumentException e) {
		throw IOException("<ExportPortInfo::parse> Excepton Occurred", e);
	}
}

void ExportPortInfo::print(ostream &os, int level)
{
	printNV(os, level, "componentName", componentName);
	printNV(os, level, "portName", portName);
	printNV(os, level, "portType", portType);
	printNV(os, level, "portDir", portDir);
	printNV(os, level, "exportPortName", exportPortName);
}
