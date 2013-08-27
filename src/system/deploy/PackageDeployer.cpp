/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : PackageDeployer.cpp
 *
 */

#include <string>

#include "PackageDeployer.h"
#include "system/Registry.h"
#include "system/config/ConfigLoaderFactory.h"

#include "system/io/IoManager.h"
#include "system/util/StringUtil.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

PackageDeployer::PackageDeployer()
{
	trace_enter();
}

PackageDeployer::~PackageDeployer()
{
	trace_enter();
}

bool PackageDeployer::onInit()
{
	trace_enter();

	m_requester.open("console_io");

	Config *cfg = getConfig();
	if (cfg == NULL) {
		log_debug("Config is NULL");
		return true;
	}

	return true;
}

bool PackageDeployer::onStart()
{
	trace_enter();

	m_fs.setDir(Registry::getRegistry()->getDeployDir());
	trace("deploy.directory = " << m_fs.getDir());

	if (m_fs.open()) {
		string loadType = getProperties()->getProperty("load.type", "none");
		if (loadType == "all") {
			vector<string> list;
			getRepositoryFileList(list);
			for (size_t i = 0; i < list.size(); i++) {
				deployPackage(list[i]);
			}
		}
		else if (loadType == "list") {
			Config *cfg = getConfig();
			if (cfg != NULL) {
				ConfigList list = cfg->getChildren("app_load");
				ConfigList::const_iterator iter = list.begin();
				for (; iter != list.end(); ++iter) {
					deployPackage((*iter)->getValue() + ".xml");
				}
			}
		}
	}

	return true;
}

void PackageDeployer::getRepositoryFileList(vector<string> &list)
{
	if (m_fs.isOpen()) {
		vector<string> myList;
		m_fs.listFiles(myList, "*.xml",false);

		for (size_t i = 0; i < myList.size(); ++i) {
			if (StringUtil::findLastOf(myList[i], ".xml")) {
				list.push_back(myList[i]);
			}
		}
	}
}

bool PackageDeployer::onStop()
{
	trace_enter();

	undeployPackageAll();

	m_fs.close();

	m_requester.close();

	return true;
}

bool PackageDeployer::onDestroy()
{
	trace_enter();

	return true;
}

bool PackageDeployer::deployPackage(const string &deployXmlFileName)
{
	trace_enter_msg("deployXmlFileName=" + deployXmlFileName);

	if (StringUtil::findLastOf(deployXmlFileName, ".xml") == false) {
		log_error("Not valid xml file : name=" << deployXmlFileName);
		return true;
	}

	// deploy file loading
	string dir = Registry::getRegistry()->getDeployDir();
	log_info("===========================================================================");
	log_info("Package Deploying START : xml=" << deployXmlFileName);
	log_info("===========================================================================");
	Package *pkg = m_pkgMgr.loadPackage(dir, deployXmlFileName);
	if (pkg == NULL) {
		log_error("Package Loading Error : dir=" << dir << "; xml=" << deployXmlFileName);
		return false;
	}

	if(deployPackage(pkg)) {
		log_info("===========================================================================");
		log_info("Package Deploying OK : pkg.name=" << deployXmlFileName);
		log_info("===========================================================================");

		return true;
	}
	else {
		log_info("===========================================================================");
		log_info("Package Deploying FAIL : pkg.name=" << deployXmlFileName);
		log_info("===========================================================================");

		return false;
	}
}

bool PackageDeployer::deployPackage(Package *pkg)
{
	trace_enter();

	if (pkg == NULL) {
		log_error("Package is NULL");
		return false;
	}

	if (pkg->getStatus() != PKG_LOADED) {
		log_error("Package is already deployed : pkg.name=" << pkg->getName());
		return false;
	}

	if (!doDeployPackage(pkg)) {
		undeployPackage(pkg->getName());
		return false;
	}
	return true;
}

bool PackageDeployer::doDeployPackage(Package *pkg)
{
	trace_enter();

	if (pkg == NULL) {
		log_error("Package is NULL");
		return false;
	}

	string pkgName = pkg->getName();
	if (pkg->getStatus() != PKG_LOADED) {
		log_error("Package is not LOADED : pkg.name=" << pkgName);
		return false;
	}

	if (!m_pkgMgr.connectPackage(pkgName)) {
		log_error("Package Connecting FAIL : pkg.name=" << pkgName);
		return false;
	}

	if (!m_pkgMgr.activatePackage(pkgName)) {
		log_error("Package Activating FAIL : pkg.name=" << pkgName);
		return false;
	}

	return true;
}

bool PackageDeployer::undeployPackage(const string &pkgName)
{
	trace_enter_msg("pkgName=" + pkgName);

	log_info("===========================================================================");
	log_info("Package Undeploying START : pkg.name=" << pkgName);
	log_info("===========================================================================");
	if (!m_pkgMgr.passivatePackage(pkgName)) {
		log_warn("   Passivating FAIL : pkg.name=" << pkgName);
	}

	if (!m_pkgMgr.disconnectPackage(pkgName)) {
		log_warn("   Disconnecting FAIL : pkg.name=" << pkgName);
	}

	if (m_pkgMgr.unloadPackage(pkgName)) {
		log_info("===========================================================================");
		log_info("Package Undeploying OK : pkg.name=" << pkgName);
		log_info("===========================================================================");
	}
	else {
		log_error("===========================================================================");
		log_error("Package Undeploying FAIL : pkg.name=" << pkgName);
		log_error("===========================================================================");
	}
	return true;
}

bool PackageDeployer::undeployPackageAll()
{
	trace_enter();

	StringList list = m_pkgMgr.getPackageNames();
	StringList::iterator iter = list.begin();
	for (; iter != list.end(); ++iter) {
		undeployPackage(*iter);
	}
	return true;
}

bool PackageDeployer::loadPackage(const string  &deployXmlFileName)
{
	trace_enter_msg("deployXmlFileName=" + deployXmlFileName);

	// deploy file loading
	string dir = Registry::getRegistry()->getDeployDir();
	log_debug("===========================================================================");
	log_debug("Package Loading : xml=" << deployXmlFileName);
	log_debug("===========================================================================");
	Package *pkg = m_pkgMgr.loadPackage(dir, deployXmlFileName);
	if (pkg == NULL) {
		log_error("Package Loading Error : path=" << dir << "; xml=" << deployXmlFileName);
		return false;
	}
	log_info("Package Loading OK : name=" << pkg->getName());

	return true;
}

bool PackageDeployer::connectPackage(const string  &pkgName)
{
	log_debug("pkgName=" + pkgName);

	log_debug("===========================================================================");
	log_debug("Package Connecting : name=" << pkgName);
	log_debug("===========================================================================");
	if (!m_pkgMgr.connectPackage(pkgName)) {
		log_error("Package Connecting FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Connecting OK : name=" << pkgName);

	return true;
}

bool PackageDeployer::activatePackage(const string  &pkgName)
{
	log_debug("===========================================================================");
	log_debug("Package Activating : name=" << pkgName);
	log_debug("===========================================================================");
	if (!m_pkgMgr.activatePackage(pkgName)) {
		log_error("*** Package Activating FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Activating OK : name=" << pkgName);

	return true;
}
bool PackageDeployer::passivatePackage(const string  &pkgName)
{
	log_debug("===========================================================================");
	log_debug("Package Passivating : name=" << pkgName);
	log_debug("===========================================================================");
	if (!m_pkgMgr.passivatePackage(pkgName)) {
		log_error("Package Passivating FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Passivating OK : pkg->name=" << pkgName);

	return true;
}

bool PackageDeployer::disconnectPackage(const string  &pkgName)
{
	log_debug("===========================================================================");
	log_debug("Package Disconnecting : name=" << pkgName);
	log_debug("===========================================================================");
	if (!m_pkgMgr.disconnectPackage(pkgName)) {
		log_error("Package Disconnecting FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Disconnecting OK : name=" << pkgName);

	return true;
}

bool PackageDeployer::unloadPackage(const string  &pkgName)
{
	log_debug("===========================================================================");
	log_debug("Package Unloading : name=" << pkgName);
	log_debug("===========================================================================");
	if (!m_pkgMgr.unloadPackage(pkgName)) {
		log_error("Package Unloading FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Unloading OK : name=" << pkgName);

	return true;
}

bool PackageDeployer::deleteApplicationFromRepository(const string &appName)
{
	string xmlPN, tarPN;
	xmlPN = appName + ".xml";
	tarPN = appName + ".tar";

	m_fs.rmdir_recusive(appName);
	if (m_fs.existsFile(xmlPN)) {
		if (m_fs.removeFile(xmlPN) == false) {
			log_error("Cannot remove the app profile : appName=" + appName);
			return false;
		}
	}
	else if (m_fs.existsFile(tarPN)) {
		if (m_fs.removeFile(tarPN) == false) {
			log_error("Cannot remove the app tar file : appName=" + appName);
			return false;
		}
	}
	else {
		log_error("There is no such application : appName=" + appName);
		return false;
	}

	return true;
}
