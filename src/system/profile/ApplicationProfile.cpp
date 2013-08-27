/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ApplicationProfile.cpp
 *
 */

#include "ApplicationProfile.h"
#include "system/Log.h"

using namespace std;

ApplicationProfile::ApplicationProfile()
{
}

ApplicationProfile::~ApplicationProfile()
{
}

bool ApplicationProfile::parse(Config *cfg)
{
	if (cfg == NULL) {
		log_error("<Profile::parse> cfg = NULL");
		return false;
	}

	try {
		m_cfg = cfg->getChild(getProfileTag());
		if (m_cfg == NULL) { // for backward compatibility
			m_cfg = cfg->getChild(APP_PROFILE_TAG_OLD);

			if (m_cfg == NULL) {
				log_error("<Profile::parse> ProfileTag is NULL : tag=" << getProfileTag());
				return false;
			}
		}

		return doParse(m_cfg);
	}
	catch (Exception &e) {
		log_error("<Profile::parse> Excepton Occurred <= " << e.getMessage());
	}
	return false;
}

bool ApplicationProfile::doParse(Config *pcfg)
{
	if (pcfg == NULL) return false;

	try {
		Config *cfg = pcfg->getChild("name");
		if (cfg == NULL) throw IOException("There is no name");
		name = cfg->getValue();

		reference = pcfg->getChildValue("reference", name);
		description = pcfg->getChildValue("description", "");

		appDomain.parse(pcfg->getChild("app_domain"));
		copyright.parse(pcfg->getChild("copyright"));
		execEnv.parse(pcfg->getChild("execution_environment"));

		cfg = pcfg->getChild("subcomponents");
		if (cfg == NULL) {
			log_error("<ApplicationProfile::parse> There is no subcomponents");
			return false;
		}
		else {
			ConfigList list = cfg->getChildren("subcomponent");
			ConfigList::iterator iter = list.begin();
			for (; iter != list.end(); ++iter) {
				ComponentInfo profile;
				profile.parse(*iter);
				componentList.push_back(profile);
			}
		}

		cfg = pcfg->getChild("port_connections");
		if (cfg == NULL) {
			log_error("<ApplicationProfile::parse> There is no port_connections");
			return false;
		}
		else {
			ConfigList list = cfg->getChildren("port_connection");
			ConfigList::iterator iter = list.begin();
			for (; iter != list.end(); ++iter) {
				PortConnectionInfo profile;
				profile.parse(*iter);
				portConnectionList.push_back(profile);
			}
		}

		cfg = pcfg->getChild("coordination");
		if (cfg != NULL) {
			ConfigList list = cfg->getChildren("depends");
			ConfigList::iterator iter = list.begin();
			for (; iter != list.end(); ++iter) {
				CoordinationInfo profile;
				profile.parse(*iter);
				coordinationInfoList.push_back(profile);
			}

		}

	}
	catch (Exception &e) {
		log_error("<ApplicationProfile::parse> Excepton Occurred <= " << e.getMessage());
		return false;
	}
	return true;
}

void ApplicationProfile::print(ostream &os, int level)
{
	os << '\n';
	os << "========================================================================\n";
	printNV(os, level, "name", name);
	printNV(os, level, "reference", reference);
	printNV(os, level, "description", description);

	os << '\n';
	printNV(os, level, "app_domain==========", "");
	appDomain.print(os, level+1);

	os << '\n';
	printNV(os, level, "copyright==========", "");
	copyright.print(os, level+1);

	os << '\n';
	printNV(os, level, "execEnv==========", "");
	execEnv.print(os, level+1);

	os << '\n';
	printNV(os, level, "subcomponents=========","");
	ComponentInfoList::iterator cIter = componentList.begin();
	for (; cIter != componentList.end(); ++cIter) {
		printNV(os, level+1, "subcomponent=========","");
		(*cIter).print(os, level+2);
	}

	os << '\n';
	printNV(os, level, "port_connections=========","");
	PortConnectionInfoList::iterator pIter = portConnectionList.begin();
	for (; pIter != portConnectionList.end(); ++pIter) {
		printNV(os, level+1, "port_connection=========","");
		(*pIter).print(os, level+2);
	}

	os << "\n========================================================================";
	os << '\n';
}
