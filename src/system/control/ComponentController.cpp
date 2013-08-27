/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentController.cpp
 *
 */


#include "ComponentController.h"


#include "system/Registry.h"
#include "system/SystemTypes.h"
#include "system/ComponentEngine.h"

#include "system/config/ConfigLoaderFactory.h"
#include "system/util/Properties.h"
#include "system/util/StringUtil.h"
#include "system/util/BasicTypes.h"

#include "system/profile/ComponentProfile.h"
#include "system/profile/ApplicationProfile.h"

#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"
#include "system/cm/ComponentAdapterAtomic.h"
#include "system/cm/ComponentAdapterComposite.h"

#include "Component.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/os/OSSetting.h"

#include <sstream> 
#include <iostream> 

using namespace std;

ComponentController::ComponentController()
{
}

ComponentController::~ComponentController()
{
}

bool ComponentController::init()
{
	trace_enter();

	m_registry = Registry::getRegistry();
	Manager *mgr = m_registry->getManager(COMPONENT_MANAGER);
	m_cm = portable_dynamic_cast(ComponentManager *, mgr);
	if (m_cm == NULL) {
		return false;
	}

	return true;
}

void ComponentController::compList(string &compName, vector<string> &list, vector<string> &compTypes)
{
	trace_enter_msg("comp.name=" << compName);

	if (compName != "") {
		ComponentAdapterPtr ca = m_cm->getComponentAdapter(compName);
		if (ca.isNULL()) return;
		if (ca->getType() != CA_COMPOSITE) return;

		ComponentAdapterComposite *composite = dynamic_cast<ComponentAdapterComposite *>(ca.get());
		ComponentAdapterList caList = composite->getComponentAdapterList();
		ComponentAdapterList::iterator iter = caList.begin();
		for (; iter != caList.end(); ++iter) {
			ComponentAdapterPtr ca = *iter;
			list.push_back(ca->getId());

			if (ca->getType() == CA_ATOMIC) compTypes.push_back("atomic");
			else if (ca->getType() == CA_COMPOSITE) compTypes.push_back("composite");
			else if (ca->getType() == CA_REMOTE_PROXY) compTypes.push_back("atomic");
			else if (ca->getType() == CA_LOCAL_PROXY) compTypes.push_back("atomic");
			else compTypes.push_back("unknown");
		}
	}
	else {
		ComponentAdapterList caList = m_cm->getComponentAdapterList();
		ComponentAdapterList::iterator iter = caList.begin();
		for (; iter != caList.end(); ++iter) {
			ComponentAdapterPtr ca = *iter;
			list.push_back(ca->getId());

			if (ca->getType() == CA_ATOMIC) compTypes.push_back("atomic");
			else if (ca->getType() == CA_COMPOSITE) compTypes.push_back("composite");
			else if (ca->getType() == CA_REMOTE_PROXY) compTypes.push_back("atomic");
			else if (ca->getType() == CA_LOCAL_PROXY) compTypes.push_back("atomic");
			else compTypes.push_back("unknown");
		}
	}
}

string ComponentController::compState(const string &compName)
{
	trace_enter_msg("comp.name=" << compName);

	string state;

	ComponentAdapterPtr ca = m_cm->getComponentAdapter(compName);
	if (ca.isNULL()) {
		state = "NOT_EXIST";
	}

	Component *comp = ca->getComponent();
	if (comp != NULL) {
		state = ca->stateToStr();
	}
	else {
		state = "NOT_EXIST";
	}

	return state;
}


bool ComponentController::compInfo(const string &compName, Properties &status)
{
	trace_enter_msg("comp.name=" << compName);

	ComponentAdapterPtr ca = m_cm->getComponentAdapter(compName);
	if (ca.isNULL()) {
		status.setProperty("error", compName + " does NOT Exist");
		return false;
	}

	Component *comp = ca->getComponent();
	ComponentProfile *profile = NULL;
	ComponentAdapterAtomic *atomic = NULL;

	if (comp != NULL) {
		status.setProperty("state", ca->stateToStr());
	}
	else {
		status.setProperty("state", "Unknown");
	}

	status.setProperty("id", ca->getId());
	switch(ca->getType()) {
		case CA_ATOMIC :
			atomic = portable_dynamic_cast(ComponentAdapterAtomic *, ca.get());

			profile = atomic->getAtomicComponentProfile();
			if (profile != NULL)
			{
				status.setProperty("type", "Atomic");
				status.setProperty("es.type", profile->execSemantics.type);
				status.setProperty("es.period", StringUtil::intToStr(profile->execSemantics.period));
				status.setProperty("es.priority", StringUtil::intToStr(profile->execSemantics.priority));
			}
			break;
		case CA_COMPOSITE : 
			status.setProperty("type", "Composite");
			break;
		case CA_LOCAL_PROXY : 
			status.setProperty("type", "LocalProxy");
			break;
		case CA_REMOTE_PROXY : 
			status.setProperty("type", "RemoteProxy");
			break;
		default : 
			status.setProperty("type", "Invalid");
			break;
	}

	return true;
}

bool ComponentController::compProps(const string &compName, Properties &status)
{
	trace_enter_msg("comp.name=" << compName);

	ComponentAdapterPtr ca = m_cm->getComponentAdapter(compName);
	if (ca.isNULL()) {
		status.setProperty("error", compName + " does NOT Exist");
		return false;
	}

	if (ca->getType() != CA_ATOMIC) {
		status.setProperty("error", compName + " is NOT Atomic");

		return false;
	}
	else {
		ComponentAdapterAtomic *atomic = portable_dynamic_cast(ComponentAdapterAtomic *, ca.get());
		ComponentProfile *profile = atomic->getAtomicComponentProfile();
		if (profile != NULL)
			status.copy(profile->props);

		return true;
	}
}

