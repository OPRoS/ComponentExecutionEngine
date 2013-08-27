/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentEngine.cpp
 *
 */

#include <iostream>

#include "ComponentEngine.h"

#include "system/config/ConfigLoaderFactory.h"
#include "system/config/ConfigLoader.h"
#include "system/config/Config.h"

#include "system/util/Properties.h"
#include "system/os/OSIncludes.h"

#include "system/Registry.h"
#include "system/io/IoManager.h"
#include "system/service/ServiceManager.h"
#include "system/cm/ComponentManager.h"
#include "system/deploy/PackageDeployer.h"
#include "system/control/ControllerManager.h"
#include "system/monitor/MonitorManager.h"
#include "modconfig/ModMWAdaptorConfig.h"

#ifdef MW_ADAPTOR
#include "system/mwadaptor/MWAdaptorManager.h"
#endif
// for test
#include "system/profile/ComponentProfile.h"
#include "system/profile/DataProfile.h"
#include "system/profile/MethodProfile.h"
#include "system/profile/CompositeComponentProfile.h"
#include "system/profile/ApplicationProfile.h"
#include "system/profile/FsmProfile.h"


// ArchiveManager
#include "ArchiveManager.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ComponentEngine::ComponentEngine()
{
	setName(COMPONENT_ENGINE);
}

ComponentEngine::~ComponentEngine() {
	destroy();
}

bool ComponentEngine::onInit()
{
	trace_enter();

	Registry *reg = Registry::getRegistry();

	log_info("ComponentEngine::onInit() starting...");
	try {
		//-----------------------------------------
		// create & register CommunicationManager
		//
		IoManager *ioMgr = new IoManager();
		ioMgr->setName(IO_MGNAGER);
		m_managers.insert(make_pair(IO_MGNAGER, ioMgr));
		reg->setManager(IO_MGNAGER, ioMgr);

		log_info("CommunicationManager created...");

		//-----------------------------------------
		// create & register ComponentManager
		//
		ComponentManager *componentMgr = new ComponentManager();
		componentMgr->setName(COMPONENT_MANAGER);
		m_managers.insert(make_pair(COMPONENT_MANAGER, componentMgr));
		reg->setManager(COMPONENT_MANAGER, componentMgr);

		//-----------------------------------------
		// create & register PackageDeployer
		//
		PackageDeployer *pkgDeployer = new PackageDeployer();
		pkgDeployer->setName(PACKAGE_DEPLOYER);
		m_managers.insert(make_pair(PACKAGE_DEPLOYER, pkgDeployer));
		reg->setManager(PACKAGE_DEPLOYER, pkgDeployer);

		//---------------------------------------------
		// create ArchiveManager for streaming protocol
		//
		ArchiveManager *arcMgr = new ArchiveManager(DEFAULT_ARCHIVE_TYPE);
		arcMgr->setName(ARCHIVE_MANAGER);
		m_managers.insert(make_pair(ARCHIVE_MANAGER, arcMgr));
		reg->setManager(ARCHIVE_MANAGER, arcMgr);

		
		//---------------------------------------------
		// ControllerManager 
		//
		ControllerManager *cntrMgr = new ControllerManager();
		cntrMgr->setName(CONTROLLER_MANAGER);
		m_managers.insert(make_pair(CONTROLLER_MANAGER, cntrMgr));
		reg->setManager(CONTROLLER_MANAGER, cntrMgr);

		//---------------------------------------------
		// MonitorManager
		//
		MonitorManager *mntMgr = new MonitorManager();
		mntMgr->setName(MONITOR_MANAGER);
		m_managers.insert(make_pair(MONITOR_MANAGER, mntMgr));
		reg->setManager(MONITOR_MANAGER, mntMgr);

		//-----------------------------------------
		// create & register ServiceManager
		//
		ServiceManager *serviceMgr = new ServiceManager();
		serviceMgr->setName(SERVICE_MGNAGER);
		m_managers.insert(make_pair(SERVICE_MGNAGER, serviceMgr));
		reg->setManager(SERVICE_MGNAGER, serviceMgr);

		//---------------------------------------------
		// MWAdaptorManager 
		//

#ifdef MW_ADAPTOR
		MWAdaptorManager *mwAdaptorMgr = new MWAdaptorManager();
		mwAdaptorMgr->setName(MWADAPTOR_MANAGER);
		m_managers.insert(make_pair(MWADAPTOR_MANAGER, mwAdaptorMgr));
		reg->setManager(MWADAPTOR_MANAGER, mwAdaptorMgr);
#endif


		//-----------------------------------------
		// initializing the managers
		//
		ManagerMap::const_iterator iter = m_managers.begin();
		for (; iter != m_managers.end(); ++iter) {
			log_info("Manager init entering : mgr.name=" << iter->second->getName());

			iter->second->setConfig(getSystemConfig()->getChild(iter->second->getName()));
			bool ret = iter->second->init();

			if (ret) {
				log_info("Manager init OK : mgr.name=" << iter->second->getName());
			}
			else {
				log_error("Manager init FAIL :  mgr.name=" << iter->second->getName());
			}
		}
	}
	catch (Exception &e) {
		log_error(e);
	}
	catch (...) {
		log_error("ComponentEngine::onInit() fail...");
		return false;
	}

	log_info("ComponentEngine::onInit() success...");
	return true;
}

bool ComponentEngine::onStart()
{
	trace_enter_msg("managers.size = " << m_managers.size());

	//-----------------------------------------
	// starting the managers
	//
	ManagerMap::const_iterator iter = m_managers.begin();
	for (; iter != m_managers.end(); ++iter) {
		log_info("Manager start entering : mgr.name=" << iter->second->getName());

		bool ret = iter->second->start();

		if (ret) {
			log_info("Manager start OK : mgr.name=" << iter->second->getName());
		}
		else {
			log_error("Manager start FAIL : mgr.name=" << iter->second->getName());
		}
	}

	return true;
}

bool ComponentEngine::onStop()
{
	trace_enter_msg("managers.size = " << m_managers.size());

	//-----------------------------------------
	// stop the managers
	//
	ManagerMap::reverse_iterator iter = m_managers.rbegin();
	for (; iter != m_managers.rend(); ++iter) {
		log_info("Manager stop entering : mgr.name=" << iter->second->getName());

		bool ret = iter->second->stop();

		if (ret) {
			log_info("Manager stop OK : mgr.name=" << iter->second->getName());
		}
		else {
			log_error("Manager stop FAIL : mgr.name=" << iter->second->getName());
		}
	}

	return true;
}

bool ComponentEngine::onDestroy()
{
	trace_enter_msg("managers.size = " << m_managers.size());

	//-----------------------------------------
	// destroy the managers
	//
	int idx = 1;
	ManagerMap::reverse_iterator iter = m_managers.rbegin();
	for (; iter != m_managers.rend(); ++iter) {
		log_info(idx++ << ". Manager destroy entering : mgr.name=" << iter->second->getName());

		bool ret = iter->second->destroy();

		if (ret) {
			log_info(idx++ << ". Manager destroy OK : mgr.name=" << iter->second->getName());
		}
		else {
			log_error(idx++ << ". Manager destroy FAIL : mgr.name=" << iter->second->getName());
		}

		Registry::getRegistry()->removeManager(iter->second->getName());

		delete iter->second;
	}

	// 엔진의 종료를 알림
	m_finished.notifyAll();

	return true;
}

void ComponentEngine::waitFinished()
{
	m_finished.wait();
}

Manager *ComponentEngine::getManager(const std::string &mgrName)
{
	ManagerMap::iterator iter = m_managers.find(mgrName);
	if (iter != m_managers.end()) return iter->second;
	else return NULL;
}

