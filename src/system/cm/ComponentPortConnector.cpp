/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentPortConnector.cpp
 *
 */

#include "ComponentPortConnector.h"

#include "ComponentManager.h"
#include "ComponentAdapter.h"

#include "system/Registry.h"
#include "system/SystemTypes.h"
#include "system/profile/ApplicationProfile.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ComponentPortConnector::ComponentPortConnector()
{
}

ComponentPortConnector::~ComponentPortConnector()
{
}

//
// PORT connection
//
bool ComponentPortConnector::connectPortList(ComponentAdapterList &caList, PortConnectionInfoList &infoList)
{
	trace_enter();

	PortConnectionInfoList::iterator iter = infoList.begin();
	for (; iter != infoList.end(); ++iter) {
		connectPort(caList, *iter);
	}

	return true;
}

bool ComponentPortConnector::connectPort(ComponentAdapterList &caList, PortConnectionInfo &info)
{
	trace_enter_msg("connectPort =========================> name=" + info.sourceComponentName
			+ ";port_type=" + info.portType);

	if (info.portType == "service") {
		return connectServicePort(caList, info);
	}
	else if (info.portType == "data") {
		return connectDataPort(caList, info);

	}
	else if (info.portType == "event") {
		return connectEventPort(caList, info);
	}
	else {
		log_error("invalid port_type = " << info.portType);
		return false;
	}

	return true;
}

bool ComponentPortConnector::connectServicePort(ComponentAdapterList &caList, PortConnectionInfo &info)
{
	trace_enter();

	log_debug("   [***] MethodPort Connecting------------------------------------------------------------");
	log_debug("         - source : componentName=" <<  info.sourceComponentName << "; portName=" << info.sourcePortName);
	log_debug("         - target : componentName=" <<  info.targetComponentName << "; portName=" << info.targetPortName);
	log_debug("   -------------------------------------------------------------------------------------");

	//
	// source
	//
	ComponentAdapterPtr sCA = findComponentAdapter(caList, info.sourceComponentName);
	if (sCA.isNULL()) {
		log_error("Source Component is NULL");
		return false;
	}

	//
	// target
	//
	ComponentAdapterPtr tCA = findComponentAdapter(caList, info.targetComponentName);
	if (tCA.isNULL()) {
		log_error("Target Component is NULL");
		return false;
	}

	return sCA->connectServicePort(tCA, info);
}

bool ComponentPortConnector::connectDataPort(ComponentAdapterList &caList, PortConnectionInfo &info)
{
	trace_enter();

	log_debug("   [***] DataPort Connecting------------------------------------------------------------");
	log_debug("         - source : componentName=" <<  info.sourceComponentName << "; portName=" << info.sourcePortName);
	log_debug("         - target : componentName=" <<  info.targetComponentName << "; portName=" << info.targetPortName);
	log_debug("   -------------------------------------------------------------------------------------");

	//
	// source
	//
	ComponentAdapterPtr sCA = findComponentAdapter(caList, info.sourceComponentName);
	if (sCA.isNULL()) {
		log_error("Source Component is NULL");
		return false;
	}

	//
	// target
	//
	ComponentAdapterPtr tCA = findComponentAdapter(caList, info.targetComponentName);
	if (tCA.isNULL()) {
		log_error("Target Component is NULL");
		return false;
	}

	return sCA->connectDataPort(tCA, info);
}

bool ComponentPortConnector::connectEventPort(ComponentAdapterList &caList, PortConnectionInfo &info)
{
	trace_enter();

	log_debug("   [***] EventPort Connecting------------------------------------------------------------");
	log_debug("         - source : componentName=" <<  info.sourceComponentName << "; portName=" << info.sourcePortName);
	log_debug("         - target : componentName=" <<  info.targetComponentName << "; portName=" << info.targetPortName);
	log_debug("   -------------------------------------------------------------------------------------");

	//
	// source
	//
	ComponentAdapterPtr sCA = findComponentAdapter(caList, info.sourceComponentName);
	if (sCA.isNULL()) {
		log_error("Source Component is NULL");
		return false;
	}

	//
	// target
	//
	ComponentAdapterPtr tCA = findComponentAdapter(caList, info.targetComponentName);
	if (tCA.isNULL()) {
		log_error("Target Component is NULL");
		return false;
	}

	return sCA->connectEventPort(tCA, info);
}


//
// PORT disconnection
//
bool ComponentPortConnector::disconnectPortList(ComponentAdapterList &caList, PortConnectionInfoList &list)
{
	trace_enter();

	PortConnectionInfoList::iterator iter = list.begin();
	for (; iter != list.end(); ++iter) {
		disconnectPort(caList, *iter);
	}

	return true;
}

bool ComponentPortConnector::disconnectPort(ComponentAdapterList &caList, PortConnectionInfo &info)
{
	trace_enter();

	if (info.portType == "service") {
		return disconnectServicePort(caList, info);
	}
	else if (info.portType == "data") {
		return disconnectDataPort(caList, info);

	}
	else if (info.portType == "event") {
		return disconnectEventPort(caList, info);
	}
	else {
		log_error("invalid port_type = " << info.portType);
		return false;
	}

	return true;
}

bool ComponentPortConnector::disconnectServicePort(ComponentAdapterList &caList, PortConnectionInfo &info)
{
	trace_enter();

	//
	// source
	//
	ComponentAdapterPtr sCA = findComponentAdapter(caList, info.sourceComponentName);
	if (sCA.isNULL()) {
		log_error("Source Component is NULL");
		return false;
	}
	return sCA->disconnectServicePort(info);
}

bool ComponentPortConnector::disconnectDataPort(ComponentAdapterList &caList, PortConnectionInfo &info)
{
	trace_enter();

	//
	// source
	//
	ComponentAdapterPtr sCA = findComponentAdapter(caList, info.sourceComponentName);
	if (sCA.isNULL()) {
		log_error("Source Component is NULL");
		return false;
	}

	return sCA->disconnectDataPort(info);
}


bool ComponentPortConnector::disconnectEventPort(ComponentAdapterList &caList, PortConnectionInfo &info)
{
	trace_enter();

	//
	// source
	//
	ComponentAdapterPtr sCA = findComponentAdapter(caList, info.sourceComponentName);
	if (sCA.isNULL()) {
		log_error("Source Component is NULL");
		return false;
	}

	return sCA->disconnectEventPort(info);
}

ComponentAdapterPtr ComponentPortConnector::findComponentAdapter(ComponentAdapterList &caList, const string &compName)
{
	trace_enter_msg("compName=" << compName);

	ComponentAdapterPtr ret(NULL);

	ComponentAdapterList::iterator iter = caList.begin();
	for (; iter != caList.end(); ++iter) {
		if ((*iter)->getId() == compName) {
			return *iter;
		}
	}
	return ret;
}
