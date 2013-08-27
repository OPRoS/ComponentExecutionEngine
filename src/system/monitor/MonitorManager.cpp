/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MonitorManager.cpp
 *
 */


#include "modconfig/ModConfig.h"
#include "MonitorManager.h"

#include "system/util/Properties.h"

#include "DeployMonitor.h"

#include "MonitoringMonitor.h"

#ifdef MOD_OPROS_SYMBOLMONITOR
#include "SymbolMonitor.h"
#endif

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

MonitorManager::MonitorManager()
{
}

MonitorManager::~MonitorManager()
{
	clearMonitors();
}

bool MonitorManager::onInit()
{
	setMonitor("deploy", new DeployMonitor());
	setMonitor("monitor", new MonitoringMonitor());

#ifdef MOD_OPROS_SYMBOLMONITOR
	setMonitor("symbol",new SymbolMonitor());
#endif





	m_monitorProtocol.setMonitorManager(this);
	m_monitorProtocol.open("external_io");

	return true;
}

bool MonitorManager::onStart()
{
	return true;
}

bool MonitorManager::onStop()
{
	return true;
}

bool MonitorManager::onDestroy()
{
	return true;
}

Monitor *MonitorManager::getMonitor(const std::string &monitorName)
{
	MonitorMap::iterator pos;
	
	pos = m_monitorMap.find(monitorName);
	if (pos == m_monitorMap.end()) {
		return NULL;
	}
	else {
		return pos->second;
	}
}

void MonitorManager::setMonitor(const std::string &monitorName, Monitor *monitor)
{
	m_monitorMap.erase(monitorName);
	m_monitorMap.insert(make_pair(monitorName, monitor));
}

void MonitorManager::removeMonitor(const std::string &monitorName)
{
	MonitorMap::iterator pos;
	pos = m_monitorMap.find(monitorName);
	if (pos != m_monitorMap.end()) {
		Monitor *m = pos->second;
		delete m;

		m_monitorMap.erase(pos);
	}
}

void MonitorManager::clearMonitors()
{
	MonitorMap::iterator pos;
	
	pos = m_monitorMap.begin();
	for (; pos != m_monitorMap.end(); ++pos) {
		Monitor *m = pos->second;
		delete m;
	}
	m_monitorMap.clear();
}
