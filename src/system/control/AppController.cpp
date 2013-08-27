/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : AppController.cpp
 *
 */


#include "AppController.h"


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

#include "system/deploy/PackageDeployer.h"
#include "system/deploy/ApplicationPackage.h"

#include "modconfig/ModMWAdaptorConfig.h"

#ifdef MW_ADAPTOR
#include "system/mwadaptor/MWAdaptorManager.h"
#endif

#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"

#include "Component.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/os/OSSetting.h"

#include <sstream> 
#include <iostream> 

using namespace std;

AppController::AppController()
{
}

AppController::~AppController()
{
}

bool AppController::init()
{
	trace_enter();

	m_registry = Registry::getRegistry();
	Manager *mgr = m_registry->getManager(PACKAGE_DEPLOYER);
	m_deployer = portable_dynamic_cast(PackageDeployer *, mgr);
	if (m_deployer == NULL) {
		log_error("PackageDeployer is NULL");
		return false;
	}

	return true;
}

bool AppController::appSetName(const string &appName) 
{
	trace_enter_msg("app.name=" << appName);

	if (appName != "") {
		m_registry->setDefaultApp(appName);

#ifdef MW_ADAPTOR
		MWAdaptorManager *mwAdaptorMgr = portable_dynamic_cast(MWAdaptorManager *, Registry::getRegistry()->getManager(MWADAPTOR_MANAGER));
		if (mwAdaptorMgr != NULL) {
			mwAdaptorMgr->getReady(appName);
		}
#endif
		return true;
	}

	return false;
}

string AppController::appGetName() 
{
	trace_enter_msg("app.name=" << m_registry->getDefaultApp());

	return m_registry->getDefaultApp();
}

bool AppController::appRun(const string &appName) 
{
	trace_enter_msg("app.name=" << appName);

	return m_deployer->deployPackage(appName + ".xml");
}

bool AppController::appStop(const string &appName) 
{
	trace_enter_msg("app.name=" << appName);

	return m_deployer->undeployPackage(appName);
}

bool AppController::appLoad(const string &appName) 
{
	trace_enter_msg("app.name=" << appName);

	return m_deployer->loadPackage(appName + ".xml");
}

bool AppController::appConnect(const string &appName) 
{
	trace_enter_msg("app.name=" << appName);

	return m_deployer->connectPackage(appName);
}

bool AppController::appActivate(const string &appName) 
{
	trace_enter_msg("app.name=" << appName);

	return m_deployer->activatePackage(appName);
}

bool AppController::appPassivate(const string &appName) 
{
	trace_enter_msg("app.name=" << appName);

	return m_deployer->passivatePackage(appName);
}

bool AppController::appDisconnect(const string &appName) 
{
	trace_enter_msg("app.name=" << appName);

	return m_deployer->disconnectPackage(appName);
}

bool AppController::appUnload(const string &appName) 
{
	trace_enter_msg("app.name=" << appName);

	return m_deployer->unloadPackage(appName);
}

void AppController::appList(vector<string> &list) 
{
	trace_enter();

	m_deployer->getRepositoryFileList(list);
}

void AppController::appRunList(vector<string> &list) 
{
	trace_enter();

	PackageList pkgList =m_deployer->getPackageManager().getPackageList();
	PackageList::iterator iter = pkgList.begin();
	for (; iter != pkgList.end(); ++iter) {
		Package *pkg = *iter;
		list.push_back(pkg->getName());
	}
}

string AppController::appState(const string &appName)
{
	trace_enter_msg("app.name=" << appName);

	string state;

	Package *pkg =m_deployer->getPackageManager().getPackage(appName);
	if (pkg == NULL) {
		state = "NOT_EXIST";
	}
	else {
		state = pkg->statusToStr();
	}

	return state;
}

bool AppController::appInfo(const string &appName, Properties &status)
{
	trace_enter_msg("app.name=" << appName);

	Package *pkg =m_deployer->getPackageManager().getPackage(appName);
	if (pkg == NULL) {
		return false;
	}
	else {
		switch(pkg->getType()) {
			case PKG_APP		: status.setProperty("type", "app"); break;
			case PKG_ATOMIC		: status.setProperty("type", "atomic"); break;
			case PKG_COMPOSITE	: status.setProperty("type", "composite"); break;
			default				: status.setProperty("type", "unknown"); break;
		}
		status.setProperty("state", pkg->statusToStr());
		status.setProperty("ref", pkg->getReference());
		status.setProperty("path", pkg->getPath());

		return true;
	}
}

bool AppController::appCompList(const string &appName, vector<string> &list, vector<string> &compTypes)
{
	trace_enter_msg("app.name=" << appName);

	Package *pkg =m_deployer->getPackageManager().getPackage(appName);
	if (pkg == NULL) {
		return false;
	}
	else {
		ApplicationPackage *appPkg = portable_dynamic_cast(ApplicationPackage *, pkg);
		if (appPkg == NULL) {
			return false;
		}

		Manager *mgr = m_registry->getManager(COMPONENT_MANAGER);
		ComponentManager *cm = portable_dynamic_cast(ComponentManager *, mgr);
		string type = "unknown";

		ApplicationProfile *appProfile = appPkg->getApplicationProfile();
		if (appProfile ==NULL)
		{
			trace_error("appProfile cannot be NULL");
			return false;
		}

		ComponentInfoList::iterator pos = appProfile->componentList.begin();
		for (; pos != appProfile->componentList.end(); ++pos) {
			list.push_back(pos->name);

			if (cm != NULL) {
				ComponentAdapterPtr ca = cm->getComponentAdapter(pos->name);
				if (!ca.isNULL()) {
					if (ca->getType() == CA_ATOMIC) type = "atomic";
					else if (ca->getType() == CA_COMPOSITE) type = "composite";
					else if (ca->getType() == CA_REMOTE_PROXY) type = "atomic";
					else if (ca->getType() == CA_LOCAL_PROXY) type = "atomic";
				}
			}

			compTypes.push_back(type);
		}

		return true;
	}
}
bool AppController::appDelete(const string &appName)
{
	trace_enter_msg("app.name=" << appName);

	return m_deployer->deleteApplicationFromRepository(appName);
}

