/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ControllerManager.cpp
 *
 */


#include "ControllerManager.h"

#include "AppController.h"
#include "ComponentController.h"
#include "EngineController.h"

#include "system/util/Properties.h"

#include "system/Log.h"
#include "system/Trace.h"

#include <cstring>

using namespace std;

ControllerManager::ControllerManager()
{
}

ControllerManager::~ControllerManager()
{
	clearControllers();
}

bool ControllerManager::onInit()
{
	AppController *dc = new AppController();
	if (dc->init()) {
		setController("pkg", dc);
	}

	ComponentController *cc = new ComponentController();
	if (cc->init()) setController("comp", cc);

	EngineController *ec = new EngineController();
	if (ec->init()) setController("engine", ec);

	return true;
}

bool ControllerManager::onStart()
{
	return true;
}

bool ControllerManager::onStop()
{
	return true;
}

bool ControllerManager::onDestroy()
{
	return true;
}

Controller*	ControllerManager::getController(const string &name)
{
	ControllerMap::iterator pos;
	
	std::string tName = name;

	if (tName=="app") tName = "pkg";
	
	pos = m_controllerMap.find(tName);
	if (pos == m_controllerMap.end()) {
		return NULL;
	}
	else {
		return pos->second;
	}
}

void ControllerManager::setController(const string &name, Controller *monitor)
{
	m_controllerMap.erase(name);
	m_controllerMap.insert(make_pair(name, monitor));
}

void ControllerManager::removeController(const string &name)
{
	ControllerMap::iterator pos;
	pos = m_controllerMap.find(name);
	if (pos != m_controllerMap.end()) {
		Controller *m = pos->second;
		delete m;

		m_controllerMap.erase(pos);
	}
}

void ControllerManager::clearControllers()
{
	ControllerMap::iterator pos;
	
	pos = m_controllerMap.begin();
	for (; pos != m_controllerMap.end(); ++pos) {
		Controller *m = pos->second;
		delete m;
	}
	m_controllerMap.clear();
}

