/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentProfile.cpp
 *
 */

#include "ComponentProfile.h"
#include "system/Log.h"

using namespace std;

ComponentProfile::ComponentProfile()
{
	fsmProfile = NULL;
}

ComponentProfile::~ComponentProfile()
{
	if (fsmProfile != NULL) delete fsmProfile;
}

bool ComponentProfile::doParse(Config *pcfg)
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
		execSemantics.parse(pcfg->getChild("execution_semantics"));

		cfg = pcfg->getChild("properties");
		if (cfg != NULL) cfg->toProperties(props,"property");

		cfg = pcfg->getChild("data_type_list");
		if (cfg != NULL) {
			ConfigList list = cfg->getChildren("reference");
			ConfigList::const_iterator iter = list.begin();
			for (; iter != list.end(); ++iter) {
				DataTypeInfo dtp;
				dtp.parse(*iter);
				dataTypeList.push_back(dtp);
			}
		}

		cfg = pcfg->getChild("fsm");
		if (cfg != NULL) fsm.parse(cfg);

		cfg = pcfg->getChild("ports");
		if (cfg != NULL) {
			// method ports
			ConfigList list = cfg->getChildren("service_port");
			ConfigList::iterator iter = list.begin();
			for (; iter != list.end(); ++iter) {
				ServicePortInfo profile;
				profile.parse(*iter);
				if (profile.usage == "required") requiredServicePortList.push_back(profile);
				else if (profile.usage == "provided") providedServicePortList.push_back(profile);
			}

			// data port
			list = cfg->getChildren("data_port");
			iter = list.begin();
			for (; iter != list.end(); ++iter) {
				DataPortInfo profile;
				profile.parse(*iter);
				if (profile.usage == "input") inputDataPortList.push_back(profile);
				else if (profile.usage == "output") outputDataPortList.push_back(profile);
			}

			// event port
			list = cfg->getChildren("event_port");
			iter = list.begin();
			for (; iter != list.end(); ++iter) {
				EventPortInfo profile;
				profile.parse(*iter);
				if (profile.usage == "input") inputEventPortList.push_back(profile);
				else if (profile.usage == "output") outputEventPortList.push_back(profile);
			}
		}
	}
	catch (Exception &e) {
		log_error("<ComponentProfile::parse> Excepton Occurred <= " << e.getMessage());
		return false;
	}
	return true;

}

void ComponentProfile::print(ostream &os, int level)
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
	printNV(os, level, "execSemantics==========", "");
	execSemantics.print(os, level+1);

	os << '\n';
	printNV(os, level, "properties=======","");
	props.print(os, level+1);

	os << '\n';
	printNV(os, level, "data_type_list=========","");
	DataTypeInfoList::iterator dtpIter = dataTypeList.begin();
	for (; dtpIter != dataTypeList.end(); ++dtpIter) {
		(*dtpIter).print(os, level+1);
	}

	os << '\n';
	printNV(os, level, "fsm==========", "");
	fsm.print(os, level+1);

	os << '\n';
	printNV(os, level, "required method port =========","");
	ServicePortInfoList::iterator rmpIter = requiredServicePortList.begin();
	for (; rmpIter != requiredServicePortList.end(); ++rmpIter) {
		(*rmpIter).print(os, level+1);
	}

	os << '\n';
	printNV(os, level, "provided method port =========","");
	ServicePortInfoList::iterator pmpIter = providedServicePortList.begin();
	for (; pmpIter != providedServicePortList.end(); ++pmpIter) {
		(*pmpIter).print(os, level+1);
	}

	os << '\n';
	printNV(os, level, "input data port =========","");
	DataPortInfoList::iterator idpIter = inputDataPortList.begin();
	for (; idpIter != inputDataPortList.end(); ++idpIter) {
		(*idpIter).print(os, level+1);
	}

	os << '\n';
	printNV(os, level, "output data port =========","");
	DataPortInfoList::iterator odpIter = outputDataPortList.begin();
	for (; odpIter != outputDataPortList.end(); ++odpIter) {
		(*odpIter).print(os, level+1);
	}

	os << '\n';
	printNV(os, level, "input event port =========","");
	EventPortInfoList::iterator iepIter = inputEventPortList.begin();
	for (; iepIter != inputEventPortList.end(); ++iepIter) {
		(*iepIter).print(os, level+1);
	}

	os << '\n';
	printNV(os, level, "output event port =========","");
	EventPortInfoList::iterator oepIter = outputEventPortList.begin();
	for (; oepIter != outputEventPortList.end(); ++oepIter) {
		(*oepIter).print(os, level+1);
	}
	os << "\n========================================================================";
	os << '\n';
}
