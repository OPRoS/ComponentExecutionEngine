/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Registry.cp
 *
 */

#include "Registry.h"
#include "ComponentEngine.h"
#include "system/config/ConfigLoaderFactory.h"
#include "system/log/LoggerFactory.h"

#include "system/os/OSFunctions.h"
#include "system/os/OSMutex.h"
#include "system/os/OSGuard.h"
#include "system/log/CoutLogger.h"


using namespace std;

//Registry *Registry::m_registry= NULL;
Registry Registry::m_registry;

Registry::Registry()
{
	m_clf = NULL;
	m_lf = NULL;
	m_syslogger = NULL;
}

Registry::~Registry()
{
	if (m_clf != NULL) delete m_clf;
	//if (m_syslogger != NULL) delete m_syslogger;
	if (m_lf != NULL) delete m_lf;
	
}


CoutLogger m_default_logger;

Logger *Registry::getSystemLogger()
{ 
	if (m_syslogger==NULL)
	{
		return &m_default_logger;
	}

	return m_syslogger; 
};

Registry *Registry::getRegistry()
{
	//if (m_registry == NULL) {
	//	m_registry = new Registry();
	//}

	return &m_registry;
}

Properties *Registry::getSystemProperties()
{
	return &m_sysProps;
}

void Registry::setSystemProperties(Properties &props)
{
	m_sysProps = props;
	m_engineDir = OS::toDir(props.getProperty("engine.dir", ".."));

	m_deployDir = m_engineDir;
	m_deployDir.append(DEPLOY_PATH);
	m_deployDir.push_back(OS::fileSeparator());

	m_logDir = m_engineDir;
	m_logDir.append(LOG_PATH);
	m_logDir.push_back(OS::fileSeparator());

	m_configDir = m_engineDir;
	m_configDir.append(CONFIG_PATH);
	m_configDir.push_back(OS::fileSeparator());

	m_sysConfigFileName = props.getProperty("cfg.file");
	m_sysConfigFilePath = newConfigFilePath(m_sysConfigFileName);
}

void Registry::toDeployDir(const string &dir) 
{
	string rdir = dir;
	StringUtil::replaceAll(rdir, "${engine.dir}", m_engineDir);
	m_deployDir = OS::toDir(rdir);
}

void Registry::toLogDir(const string &dir) 
{
	string rdir = dir;
	StringUtil::replaceAll(rdir, "${engine.dir}", m_engineDir);
	m_logDir = OS::toDir(rdir);
}

void Registry::toConfigDir(const string &dir) 
{
	string rdir = dir;
	StringUtil::replaceAll(rdir, "${engine.dir}", m_engineDir);
	m_configDir = OS::toDir(rdir);
}

Manager *Registry::getManager(const string &mgrName)
{
	ManagerMap::iterator iter = m_managers.find(mgrName);
	if (iter != m_managers.end()) return iter->second;
	else return NULL;
}

void Registry::setManager(const string &mgrName, Manager *mgr)
{
	m_managers.insert(make_pair(mgrName, mgr));
}

void Registry::removeManager(const std::string  &mgrName)
{
	ManagerMap::iterator pos = m_managers.find(mgrName);
	if (pos != m_managers.end()) {
		m_managers.erase(pos);
	}
}