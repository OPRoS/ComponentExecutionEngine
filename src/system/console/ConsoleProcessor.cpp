/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ConsoleProcessor.cpp
 *
 */

#include <iostream>

#include "ConsoleProcessor.h"

#include "system/Registry.h"
#include "system/SystemTypes.h"
#include "system/ComponentEngine.h"

#include "system/config/ConfigLoaderFactory.h"
#include "system/util/Properties.h"
#include "system/util/StringUtil.h"

#include "system/profile/ComponentProfile.h"
#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"
#include "system/cm/ComponentAdapterAtomic.h"
#include "system/deploy/PackageDeployer.h"

#include "Component.h"

#include "system/control/ControllerManager.h"
#include "system/control/EngineController.h"
#include "system/control/AppController.h"
#include "system/control/ComponentController.h"
#include "modconfig/ModMWAdaptorConfig.h"

#ifdef MW_ADAPTOR
#include "system/mwadaptor/MWAdaptorManager.h"
#endif
#include "system/Log.h"
#include "system/Trace.h"

#include "system/os/OSSetting.h"

#include "system/util/LifecycleObject.h"

using namespace std;

extern bool g_isTrace;
extern bool g_logSync;

ConsoleProcessor::ConsoleProcessor(std::ostream *os, std::istream *is)
{
	m_os = os;
	m_is = is;
}

ConsoleProcessor::~ConsoleProcessor()
{
}

bool ConsoleProcessor::printHelp()
{
	string package_name = Registry::getRegistry()->getDefaultApp();

	*m_os << "\r\n";
	*m_os << "[Command Format]===========================================================\r\n";
	*m_os << "   exit : exit the Program\r\n";
//	*m_os << "   monitor list : print the list of the loaded applications\r\n";
//	*m_os << "   trace   on/off : trace on, off, sync on, sync off\r\n";
//	*m_os << "   log     sync/nosync : log sync on, sync off\r\n";
	*m_os << "\r\n";
	*m_os << "   pkgname <package_name> : set the current package name (current=" << package_name << ")\r\n";
	*m_os << "   pkg set <package_name> : set the current package name (current=" << package_name << ")\r\n";
	*m_os << "   pkg get		: print the current package name (current=" << package_name << ")\r\n";
	*m_os << "   pkg list		: print the package list(xml file) in the deploy directory\r\n";
	*m_os << "   pkg runlist		: print the list of the running packages\r\n";
	*m_os << "   pkg state		: print the state of the package\r\n";
	*m_os << "   pkg complist		: print the component list in the current application\r\n";
	*m_os << "   pkg run		: load/connect/activate components in the application\r\n";
	*m_os << "   pkg stop		: passivate/disconnect/unload components in the application\r\n";
	*m_os << "   pkg load/unload : load/unload the application \r\n";
	*m_os << "   pkg connect/disconnect : connect/disconnect ports of the components in the package \r\n";
	*m_os << "   pkg activate/passivate : activate/passivate the package\r\n";
	*m_os << "\r\n";
	*m_os << "   comp list	: print the list of the loaded components\r\n";
	*m_os << "   comp props <component_name> : print the component properties\r\n";
	*m_os << "   comp state <component_name> : print the component state\r\n";
	*m_os << "==========================================================================\r\n";
	*m_os << "Default Package = [" + package_name + "]\r\n\r\n";
	return true;
}

void ConsoleProcessor::processConsoleCommand()
{
	trace_enter();

	bool isDone = false;

	printHelp();

	char line[80];
	while (!isDone) {
		*m_os << "\r\ncommand >> ";
		(*m_is).getline(line, 80);

		ComponentEngine *engine = (ComponentEngine *) Registry::getRegistry()->getManager(COMPONENT_ENGINE);
		if (engine == NULL || engine->getState() == MS_DESTROYED) {
			*m_os << "ComponentEngine destroyed...\r\n";
			break;
		}

		try {
			string cmd(line);
			isDone = doCommand(cmd);
		}
		catch (Exception &ex) {
			log_error(ex);
		}
	}
}

bool ConsoleProcessor::doCommand(const string &cmd)
{
	Registry *reg = Registry::getRegistry();

#ifdef MW_ADAPTOR
	Manager *mgr = reg->getManager(MWADAPTOR_MANAGER);
	MWAdaptorManager *mwAdaptor = portable_dynamic_cast(MWAdaptorManager *,mgr);
#endif

	vector<string> cmdList = StringUtil::split(cmd, ' ');
	if (cmdList.size() < 1) return false;

#ifdef MW_ADAPTOR

	if (cmdList[0] == "mw.package_name") {
		if (cmdList.size() < 2) {
			*m_os << "[Error] enter app name" << "\r\n";
		}
		else {
			if (mwAdaptor != NULL)
			mwAdaptor->package_name((char *) cmdList[1].c_str());
		}
	}
	else if (cmdList[0] == "mw.pkg.run") {
		if (mwAdaptor != NULL)
		mwAdaptor->appRun();
	}
	else if (cmdList[0] == "mw.pkg.stop") {
		if (mwAdaptor != NULL)
		mwAdaptor->appStop();
	}
	else if (cmdList[0] == "mw.pkg.load") {
		if (mwAdaptor != NULL)
		mwAdaptor->appLoad();
	}
	else if (cmdList[0] == "mw.pkg.unload") {
		if (mwAdaptor != NULL)
		mwAdaptor->appUnload();
	}
	else if (cmdList[0] == "mw.pkg.connect") {
		if (mwAdaptor != NULL)
		mwAdaptor->appConnect();
	}
	else if (cmdList[0] == "mw.pkg.disconnect") {
		if (mwAdaptor != NULL)
		mwAdaptor->appDisconnect();
	}
	else if (cmdList[0] == "mw.pkg.activate") {
		if (mwAdaptor != NULL)
		mwAdaptor->appActivate();
	}
	else if (cmdList[0] == "mw.pkg.passivate") {
		if (mwAdaptor != NULL)
		mwAdaptor->appPassivate();
	}
	else if (cmdList[0] == "mw.pkg.state") {
		if (mwAdaptor != NULL)
		{
		*m_os << "pkg.state = " << mwAdaptor->appStatus() << "\r\n";
		}
	}
	else if (cmdList[0] == "mw.pkg.list") {
		if (mwAdaptor != NULL)
		{
			char *list = mwAdaptor->appList();
			if (list != NULL) {
				*m_os << "pkg.list = " <<  list << "\r\n";
				delete list;
			}
		}
	}
	else if (cmdList[0] == "mw.comp.state") {
		if (mwAdaptor != NULL) {
			if (cmdList.size() < 2) {
				*m_os << "[Error] enter component name" << "\r\n";
			}
			else {
				*m_os << "comp.state = " << mwAdaptor->compStatus((char *) cmdList[1].c_str()) << "\r\n";
			}
		}
	}
	else

#endif	 // #ifdef MW_ADAPTOR
	//-------------------------------------
	// process command
	if (cmdList[0] == "exit") {
		ComponentEngine *engine = (ComponentEngine *) reg->getManager(COMPONENT_ENGINE);
		if (engine != NULL){			
			reg->removeManager(COMPONENT_ENGINE);			
			delete engine;
		}
		return true;
	}
	else if (cmdList[0] == "comp") {
		if (cmdList.size() < 2) {
			*m_os << "Invalid comp Command\r\n";
			printHelp();
		}
		else {
			processComponent(cmdList);
		}
	}
	else if (cmdList[0] == "pkg" || cmdList[0] == "app") {
		if (cmdList.size() < 2) {
			*m_os << "Invalid app Command\r\n";
			printHelp();
		}
		else {
			processApp(cmdList);
		}
	}
	else if (cmdList[0] == "pkgname" || cmdList[0] == "appname") {
		if (cmdList.size() < 2) {
			*m_os << "Invalid package_name Command\r\n";
			printHelp();
		}
		else {
			Manager *mgr = reg->getManager(CONTROLLER_MANAGER);
			ControllerManager *cntlMgr = portable_dynamic_cast(ControllerManager *, mgr);
			if (cntlMgr == NULL) {
				*m_os << "[package_name.set] Fail : There is no ControllerManager\r\n";
			}
			AppController *ctl = portable_dynamic_cast(AppController *, cntlMgr->getController("app"));
			if (ctl == NULL) {
				*m_os << "[package_name.set] Fail : There is no AppController\r\n";
			}
			else {
				if (ctl->appSetName(cmdList[1])) {
					*m_os << "[package_name.set] OK : pkg.name = " << cmdList[1] << "\r\n";
				}
				else {
					*m_os << "[package_name.set] Fail : pkg.name = " << cmdList[1] << " failed...\r\n";
				}
			}
		}
	}
	else if (cmdList[0] == "monitor") {
		if (cmdList.size() < 2) {
			*m_os << "Invalid Monitor Command\r\n";
			printHelp();
		}
		else {
			processMonitor(cmdList[1]);
		}
	}
	else if (cmdList[0] == "help") {
		printHelp();
	}
	else {
		processEngine(cmdList);
	}

	return false;
}


bool ConsoleProcessor::processEngine(vector<string> &cmdList)
{
	string cmd = cmdList[0];

	trace_enter_msg("cmd=" << cmd);

	Manager *mgr = Registry::getRegistry()->getManager(CONTROLLER_MANAGER);
	ControllerManager *cntlMgr = portable_dynamic_cast(ControllerManager *, mgr);
	if (cntlMgr == NULL) {
		*m_os << "There is no ControllerManager\r\n";
		return true;
	}
	EngineController *ctl = portable_dynamic_cast(EngineController *, cntlMgr->getController("engine"));
	if (ctl == NULL) {
		*m_os << "There is no EngineController\r\n";
		return true;
	}

	if (cmd == "init") {
		if (ctl->engineInit() == false) {
			*m_os << "Engine init fail..."<< "\r\n";
		}
		else {
			*m_os << "Engine init success..." << "\r\n";
		}
	}
	else if (cmd == "start") {
		if (ctl->engineStart() == false) {
			*m_os << "Engine start fail..."<< "\r\n";
		}
		else {
			*m_os << "Engine start success..." << "\r\n";
		}
	}
	else if (cmd == "stop") {
		if (ctl->engineStop() == false) {
			*m_os << "Engine stop fail..."<< "\r\n";
		}
		else {
			*m_os << "Engine stop success..." << "\r\n";
		}
	}
	else if (cmd == "reset") {
		if (ctl->engineReset() == false) {
			*m_os << "Engine reset fail..."<< "\r\n";
		}
		else {
			*m_os << "Engine reset success..." << "\r\n";
		}
	}
	else if (cmd == "destroy") {
		if (ctl->engineDestroy() == false) {
			*m_os << "Engine destroy fail..."<< "\r\n";
		}
		else {
			*m_os << "Engine destroy success..." << "\r\n";
		}
		return false;
	}
	else if (cmd == "trace") {
		if (cmdList.size() < 2) {
			*m_os << "Invalid engine trace command." << "\r\n";
			return true;
		}

		bool isTrace;
		if (cmdList[1] == "on") isTrace = true;
		else isTrace = false;

		ctl->traceSet(isTrace);
		*m_os << "Trace [" << cmdList[1] << "] success..."<< "\r\n";
	}
	else if (cmd == "log") {
		if (cmdList.size() < 2) {
			*m_os << "Invalid engine trace command." << "\r\n";
			return true;
		}

		bool isSync;
		if (cmdList[1] == "on") isSync = true;
		else isSync = false;

		ctl->logSetSync(isSync);
		*m_os << "Log.Sync [" << cmdList[1] << "] success..."<< "\r\n";
	}
	else {
		*m_os << "Invalid Engine command : cmd=" << cmd << "\r\n";
	}
	return true;
}

bool ConsoleProcessor::processComponent(vector<string> &cmdList)
{
	string cmd = cmdList[1];

	trace_enter_msg("cmd=" << cmd);

	Manager *mgr = Registry::getRegistry()->getManager(CONTROLLER_MANAGER);
	ControllerManager *cntlMgr = portable_dynamic_cast(ControllerManager *, mgr);
	if (cntlMgr == NULL) {
		*m_os << "There is no ControllerManager\r\n";
		return true;
	}
	ComponentController *ctl = portable_dynamic_cast(ComponentController *, cntlMgr->getController("comp"));
	if (ctl == NULL) {
		*m_os << "There is no ComponentController\r\n";
		return true;
	}

	if (cmd == "list") {
		vector<string> clist, compTypes;
		string compName("");
		ctl->compList(compName, clist, compTypes);

		int size = clist.size();
		*m_os << "\r\n[Component List] size=" << size << "\r\n";
		*m_os << " ---------------------------------------------------------------------------------------------------\r\n";
		*m_os << "\tName                   Type           State       ExecType   Priority\r\n";
		*m_os << " ---------------------------------------------------------------------------------------------------\r\n";
		Properties props;
		for (int i = 0; i < size; ++i) {
			*m_os << "\t";
			printStr(clist[i], 23);

			if (ctl->compInfo(clist[i], props) == false) {
				*m_os << "Invalid Status";
			}
			else {
				string id = props.getProperty("id", "");
				string type = props.getProperty("type", "");
				string state = props.getProperty("state", "");

				printStr(type, 15);
				printStr(state, 12);

				if (type == "Atomic") {
					string exType;;
					string pri;

					exType = props.getProperty("es.type", "");
					pri    = props.getProperty("es.priority", "");

					printStr(exType, 12);
					printStr(pri, 11);
				}
			}

			*m_os << "\r\n";
		}
		*m_os << " ---------------------------------------------------------------------------------------------------\r\n";
	}
	else if (cmd == "state") {
		if (cmdList.size() < 3) {
			*m_os << "There is not a component name." << "\r\n";
			return true;
		}

		string state = ctl->compState(cmdList[2]);
		*m_os << cmdList[2] << ".state =  " << state << "\r\n";
	}
	else if (cmd == "props") {
		if (cmdList.size() < 3) {
			*m_os << "There is not a component name." << "\r\n";
			return true;
		}

		Properties props;
		if (ctl->compProps(cmdList[2], props) == false) {
			*m_os << "[comp.props] Fail : " << props.getProperty("error","Unknown error") << "\r\n";
		}
		else {
			vector<string> list;
			props.getNames(list);

			int size = list.size();
			*m_os << "\r\n[Component Properties] size=" << size << "\r\n";
			*m_os << " ---------------------------\r\n";
			*m_os << "\tName            Value\r\n";
			*m_os << " ---------------------------\r\n";
			for (int i = 0; i < size; ++i) {
				string value = props.getProperty(list[i], "");

				*m_os << "\t";
				printStr(list[i], 16);
				printStr(value, 25);
				*m_os << "\r\n";
			}
			*m_os << " ---------------------------\r\n";
		}
	}
	else {
		*m_os << "Invalid Component command : cmd=" << cmd << "\r\n";
	}

	return true;
}

bool ConsoleProcessor::processApp(vector<string> &cmdList)
{
	string cmd = cmdList[1];
	trace_enter_msg("cmd=" << cmd);

	Manager *mgr = Registry::getRegistry()->getManager(CONTROLLER_MANAGER);
	ControllerManager *cntlMgr = portable_dynamic_cast(ControllerManager *, mgr);
	if (cntlMgr == NULL) {
		*m_os << "There is no ControllerManager\r\n";
		return true;
	}
	AppController *ctl = portable_dynamic_cast(AppController *, cntlMgr->getController("app"));
	if (ctl == NULL) {
		*m_os << "There is no Controller For Packages\r\n";
		return true;
	}

	string appDefault = Registry::getRegistry()->getDefaultApp();

	if (cmd == "set") {
		if (cmdList.size() < 3) {
			*m_os << "There is not a package name." << "\r\n";
			return true;
		}

		if (ctl->appSetName(cmdList[2])) {
			*m_os << "[package_name.set] OK : pkg.name = " << cmdList[2] << "\r\n";
		}
		else {
			*m_os << "[package_name.set] Fail : pkg.name = " << cmdList[2] << " failed...\r\n";
		}
	}
	else if (cmd == "get") {
		*m_os << "[package_name.get] OK : pkg.name = " << ctl->appGetName() << "\r\n";
	}
	else if (cmd == "run") {
		if (ctl->appRun(appDefault)) {
			*m_os << "[pkg.run] OK : pkg.name=" << appDefault << "\r\n";
		}
		else {
			*m_os << "[pkg.run] Fail : pkg.name=" << appDefault << "\r\n";
		}
	}
	else if (cmd == "stop") {
		if (ctl->appStop(appDefault)) {
			*m_os << "[pkg.stop] OK : pkg.name=" << appDefault << "\r\n";
		}
		else {
			*m_os << "[pkg.stop] Fail : pkg.name=" << appDefault << "\r\n";
		}
	}
	else if (cmd == "load") {
		if (ctl->appLoad(appDefault)) {
			*m_os << "[pkg.load] OK : pkg.name=" << appDefault << "\r\n";
		}
		else {
			*m_os << "[pkg.load] Fail : pkg.name=" << appDefault << "\r\n";
		}
	}
	else if (cmd == "connect") {
		if (ctl->appConnect(appDefault)) {
			*m_os << "[pkg.connect] OK : pkg.name=" << appDefault << "\r\n";
		}
		else {
			*m_os << "[pkg.connect] Fail : pkg.name=" << appDefault << "\r\n";
		}
	}
	else if (cmd == "activate") {
		if (ctl->appActivate(appDefault)) {
			*m_os << "[pkg.activate] OK : pkg.name=" << appDefault << "\r\n";
		}
		else {
			*m_os << "[pkg.activate] Fail : pkg.name=" << appDefault << "\r\n";
		}
	}
	else if (cmd == "passivate") {
		if (ctl->appPassivate(appDefault)) {
			*m_os << "[pkg.passivate] OK : pkg.name=" << appDefault << "\r\n";
		}
		else {
			*m_os << "[pkg.passivate] Fail : pkg.name=" << appDefault << "\r\n";
		}
	}
	else if (cmd == "disconnect") {
		if (ctl->appDisconnect(appDefault)) {
			*m_os << "[pkg.disconnect] OK : pkg.name=" << appDefault << "\r\n";
		}
		else {
			*m_os << "[pkg.disconnect] Fail : pkg.name=" << appDefault << "\r\n";
		}
	}
	else if (cmd == "unload") {
		if (ctl->appUnload(appDefault)) {
			*m_os << "[pkg.unload] OK : pkg.name=" << appDefault << "\r\n";
		}
		else {
			*m_os << "[pkg.unload] Fail : pkg.name=" << appDefault << "\r\n";
		}
	}
	else if (cmd == "state") {
		*m_os << "[pkg.state] OK : " << appDefault << ".state = " << ctl->appState(appDefault) << "\r\n";
	}
	else if (cmd == "list") {
		vector<string> list;
		ctl->appList(list);

		*m_os << "\r\n[Package List] deploy.dir=" << Registry::getRegistry()->getDeployDir() << "\r\n";
		*m_os << " ------------------------------------------\r\n";
		*m_os << "\t[File Names] size = " << list.size() << "\r\n";
		*m_os << " ------------------------------------------\r\n";
		for (size_t i = 0; i < list.size(); ++i) {
			*m_os << "\t " << list[i] << "\r\n";
		}
		*m_os << " ------------------------------------------\r\n";
	}
	else if (cmd == "runlist") {
		vector<string> list;
		ctl->appRunList(list);

		*m_os << "\r\n[Running Package List] size=" << list.size() << "\r\n";
		*m_os << " ---------------------------------------------------------------------------------------------------\r\n";
		*m_os << "\tName                   Type         State       Reference               Path\r\n";
		*m_os << " ---------------------------------------------------------------------------------------------------\r\n";
		for (size_t i = 0; i < list.size(); ++i) {
			string name, type, state, ref, path;

			*m_os << "\t";
			printStr(list[i], 23);

			Properties props;

			if (ctl->appInfo(list[i], props) == false) {
				*m_os << "Invalid package";
			}
			else {
				type = props.getProperty("type", "");
				state = props.getProperty("state", "");
				ref = props.getProperty("ref", "");
				path = props.getProperty("path", "");

				printStr(type, 13);
				printStr(state, 12);
				printStr(ref, 24);
				printStr(path, 30);
			}

			*m_os << "\r\n";
		}
		*m_os << " ---------------------------------------------------------------------------------------------------\r\n";
	}
	else if (cmd == "complist") {
		vector<string> list, compTypes;

		if (ctl->appCompList(appDefault, list, compTypes) == false) {
			*m_os << "[pkg.complist] Fail : pkg.name=" << appDefault << "\r\n";
		}
		else {
			*m_os << "\r\n[Component List] size=" << list.size() << "\r\n";
			*m_os << " ------------------------------------------\r\n";
			*m_os << "\tName                  Type\r\n";
			*m_os << " ------------------------------------------\r\n";
			for (size_t i = 0; i < list.size(); ++i) {
				*m_os << "\t ";
				printStr(list[i], 23);
				printStr(compTypes[i], 10);
				*m_os << "\r\n";
			}
			*m_os << " ------------------------------------------\r\n";
		}
	}
	else {
		*m_os << "Invalid Package command : cmd=" << cmd << "\r\n";
	}

	return true;
}

bool ConsoleProcessor::processMonitor(const string &cmd)
{
	trace_enter_msg("cmd=" << cmd);
	return true;
}

void ConsoleProcessor::printStr(const string &str, int size)
{
	int bsize = size - str.size();

	*m_os << str;
	for (int i = 0; i < bsize; i++) *m_os << ' ';
}
