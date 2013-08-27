/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 19
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : PackageManager.cpp
 *
 */

#include <string>

#include "PackageManager.h"

#include "ApplicationPackage.h"
#include "ComponentPackage.h"

#include "system/Registry.h"
#include "system/config/ConfigLoaderFactory.h"
#include "system/profile/ApplicationProfile.h"
#include "system/profile/CompositeComponentProfile.h"
#include "system/profile/ComponentProfile.h"
#include "system/os/OSIncludes.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

PackageManager::PackageManager()
{
}

PackageManager::~PackageManager()
{
	removePackageAll();
}

StringList PackageManager::getPackageNames()
{
	StringList list;
	PackageList::const_iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		list.push_back((*iter)->getName());
	}
	return list;
}

PackageList &PackageManager::getPackageList()
{
	return m_pkgList;
}

Package *PackageManager::getPackage(const std::string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName << ",pkgList.size=" << m_pkgList.size());

	Package *pkg = NULL;
	PackageList::const_iterator pos = m_pkgList.begin();
	for (; pos != m_pkgList.end(); ++pos) {
		trace("name=" << (*pos)->getName());
		if ((*pos)->getName() == pkgName) {
			pkg = *pos;
			break;
		}
	}

	return pkg;
}

bool PackageManager::addPackage(Package *pkg)
{
	trace_enter();

	if (pkg == NULL) return false;

	trace("pkg.name=" << pkg->getName());

	Package *tmp = getPackage(pkg->getName());
	if (tmp != NULL)
	{
		log_warn("package already loaded");
		return false;
	}

	m_pkgList.push_back(pkg);
	return true;
}

void PackageManager::removePackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = NULL;
	PackageList::iterator pos = m_pkgList.begin();
	for (; pos != m_pkgList.end(); ++pos) {
		if ((*pos)->getName() == pkgName) {
			pkg = *pos;
			m_pkgList.erase(pos);
			if (pkg != NULL) delete pkg;
			break;
		}
	}
}

void PackageManager::removePackageAll()
{
	trace_enter();

	PackageList::const_iterator pos = m_pkgList.begin();
	for (; pos != m_pkgList.end(); ++pos) {
		if (*pos != NULL) delete *pos;
	}
	m_pkgList.clear();
}

Package *PackageManager::createPackage(const string &path, const string &reference)
{
	trace_enter_msg("path=" + path + "; ref=" + reference);

	Package *pkg = NULL;

	// XML 프로파일의 경로 설정
	string profilePath = path + reference;
	trace("profilePath : " << profilePath);

	// XML 프로파일 로딩
	ConfigLoaderFactory *clf = Registry::getRegistry()->getConfigLoaderFactory();
	if (clf == NULL) {
		// 로딩 실패 시 NULL 리턴
		log_error("ConfigLoadFactory Loading Error");
		return pkg;
	}

	ConfigLoader *loader = clf->getConfigLoader(reference);
	Config *cfg = loader->loadFromFile(profilePath);
	if (cfg == NULL) {
		// 로딩 실패 시 NULL 리턴
		log_error("PackageProfile Config Loading Error");
		return pkg;
	}

	if (cfg->getChild(APP_PROFILE_TAG) != NULL || cfg->getChild(APP_PROFILE_TAG_OLD)) {
		// application profile인 경우

		// 기존에 이미 배포된 패키지가 있는 지 검사
		Config *scfg = cfg->getChild(APP_PROFILE_TAG);
		if (scfg==NULL) scfg=cfg->getChild(APP_PROFILE_TAG_OLD);
		string name = scfg->getChildValue("name","");
		if (name != "") {
			pkg = getPackage(name);

			// 이미 배포된 패키지가 있으면 리턴
			if (pkg != NULL) {
				delete cfg;
				return pkg;
			}
		}

		pkg = new ApplicationPackage();
	}
	else if (cfg->getChild(COMPOSITE_COMPONENT_PROFILE_TAG) != NULL) {
		// composite compoent profile인 경우

		// 기존에 이미 배포된 패키지가 있는 지 검사
		Config *scfg = cfg->getChild(COMPOSITE_COMPONENT_PROFILE_TAG);
		string name = scfg->getChildValue("name","");
		if (name != "") {
			pkg = getPackage(name);

			// 이미 배포된 패키지가 있으면 리턴
			if (pkg != NULL) {
				delete cfg;
				return pkg;
			}
		}

		pkg = new ComponentPackage();
	}
	else if (cfg->getChild(COMPONENT_PROFILE_TAG) != NULL) {
		// atomic component profile인 경우

		// 기존에 이미 배포된 패키지가 있는 지 검사
		Config *scfg = cfg->getChild(COMPONENT_PROFILE_TAG);
		string name = scfg->getChildValue("name","");
		if (name != "") {
			pkg = getPackage(name);

			// 이미 배포된 패키지가 있으면 리턴
			if (pkg != NULL) {
				delete cfg;
				return pkg;
			}
		}

		pkg = new ComponentPackage();
	}
	delete cfg;

	return pkg;
}


//------------------------------------------------
// Package load (component instance creation)
//
Package *PackageManager::loadPackage(const string &path, const string &reference)
{
	trace_enter_msg("path=" + path + "; ref=" + reference);

	Package *pkg = createPackage(path, reference);
	if (pkg == NULL) {
		log_error("Invalid Package Type : path=" << path << ",ref=" << reference);
		return NULL;
	}

	pkg->setPackageManager(this);
	pkg->setPath(path);
	pkg->setReference(reference);
	if (pkg->load()) {
		if (!addPackage(pkg))
		{
			pkg->unload();
			delete pkg;
			return NULL;
		}
	}
	else {
		pkg->unload();

		delete pkg;
		pkg = NULL;
	}

	return pkg;
}

//----------------------------------------------
// Package unload (delete from the memory)
//
bool PackageManager::unloadPackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL : pkg.name=" << pkgName);
		return false;
	}

	if (pkg->unload()) {
		removePackage(pkgName);
		return true;
	}
	else {
		return false;
	}
}

//-------------------------------------------------------------
// Package  ports connect between components in the package
//
bool PackageManager::connectPackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL");
		return false;
	}

	return pkg->connectPort();
}

//-------------------------------------------------------------
// Package  ports disconnect between components in the package
//
bool PackageManager::disconnectPackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL");
		return false;
	}

	return pkg->disconnectPort();
}

//-------------------------------------------------------------
// components activation(start) in the package
//
bool PackageManager::activatePackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL");
		return false;
	}

	return pkg->activate();
}

//-------------------------------------------------------------
// components passivation(stop) in the package
//
bool PackageManager::passivatePackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL");
		return false;
	}

	return pkg->passivate();
}

