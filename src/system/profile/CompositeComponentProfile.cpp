/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : CompositeComponentProfile.cpp
 *
 */

#include "CompositeComponentProfile.h"
#include "system/Log.h"

using namespace std;

CompositeComponentProfile::CompositeComponentProfile()
{
}

CompositeComponentProfile::~CompositeComponentProfile()
{
}

bool CompositeComponentProfile::doParse(Config *pcfg)
{
	if (pcfg == NULL) return false;

	try {
		Config *cfg = pcfg->getChild("name");
		if (cfg == NULL) throw IOException("There is no name");
		name = cfg->getValue();

		cfg = pcfg->getChild("description");
		if (cfg != NULL) description = cfg->getValue();

		appDomain.parse(pcfg->getChild("app_domain"));
		copyright.parse(pcfg->getChild("copyright"));
		execEnv.parse(pcfg->getChild("execution_environment"));

		cfg = pcfg->getChild("subcomponents");
		if (cfg == NULL) {
			log_error("<CompositeComponentProfile::parse> There is no subcomponents");
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

		cfg = pcfg->getChild("export_ports");
		if (cfg == NULL) {
			log_error("<CompositeComponentProfile::parse> There is no export_ports");
			return false;
		}
		else {
			ConfigList list = cfg->getChildren("export_port");
			ConfigList::iterator iter = list.begin();
			for (; iter != list.end(); ++iter) {
				ExportPortInfo profile;
				profile.parse(*iter);
				exportPortList.push_back(profile);
			}
		}

		cfg = pcfg->getChild("port_connections");
		if (cfg == NULL) {
			log_error("<CompositeComponentProfile::parse> There is no port_connections");
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
		log_error("<CompositeComponentProfile::parse> Excepton Occurred <= " << e.getMessage());
		return false;
	}
	return true;
}

void CompositeComponentProfile::print(ostream &os, int level)
{
	os << '\n';
	os << "========================================================================\n";
	printNV(os, level, "name", name);
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
	printNV(os, level, "export_ports=========","");
	ExportPortInfoList::iterator eIter = exportPortList.begin();
	for (; eIter != exportPortList.end(); ++eIter) {
		printNV(os, level+1, "export_port=========","");
		(*eIter).print(os, level+2);
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
