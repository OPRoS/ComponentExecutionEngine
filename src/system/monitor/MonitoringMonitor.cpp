/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MonitoringMonitor.cpp
 *
 */


#include "MonitoringMonitor.h"


#include "system/Registry.h"
#include "system/control/ControllerManager.h"

#include "system/Log.h"
#include "system/Trace.h"
#include "system/os/OSFunctions.h"

using namespace std;

MonitoringMonitor::MonitoringMonitor()
{
}

MonitoringMonitor::~MonitoringMonitor()
{
}

bool MonitoringMonitor::doCommand(MonitorWorker *mw, Properties &props)
{
	try {
		string cmd = props.getProperty("cmd");
		if (cmd == "pkg.list" || cmd == "app.list") {
			appList(mw, props);
		}
		else if (cmd == "pkg.run_list" || cmd == "app.run_list") {
			appRunList(mw, props);
		}
		else if (cmd == "pkg.comp_list" || cmd == "app.comp_list") {
			appCompList(mw, props);
		}
		else if (cmd == "pkg.run" || cmd == "app.run") {
			appRun(mw, props);
		}
		else if (cmd == "pkg.load" || cmd == "app.load") {
			appLoad(mw, props);
		}
		else if (cmd == "pkg.connect" || cmd == "app.connect") {
			appConnect(mw, props);
		}
		else if (cmd == "pkg.activate" || cmd == "app.activate") {
			appActivate(mw, props);
		}
		else if (cmd == "pkg.stop" || cmd == "app.stop") {
			appStop(mw, props);
		}
		else if (cmd == "pkg.passivate" || cmd == "app.passivate") {
			appPassivate(mw, props);
		}
		else if (cmd == "pkg.disconnect" || cmd == "app.disconnect") {
			appDisconnect(mw, props);
		}
		else if (cmd == "pkg.unload" || cmd == "app.unload") {
			appUnload(mw, props);
		}
		else if (cmd == "pkg.info" || cmd == "app.info") {
			appInfo(mw, props);
		}
		else if (cmd == "pkg.exist" || cmd == "app.exist") {
			appExist(mw, props);
		}
		else if (cmd == "pkg.delete" || cmd == "app.delete") {
			appDelete(mw, props);
		}
		else if (cmd == "comp.info") {
			compInfo(mw, props);
		}
		else if (cmd == "comp.list") {
			compList(mw, props);
		}
		else if (cmd == "comp.props") {
			compProps(mw, props);
		}
		else {
			mw->returnFail("monitor", props.getProperty("cmd", "unknown"),"Invalid command");
		}
	}
	catch (Exception &ex) {
		log_error(ex);
		const char *res = ex.what();
		string errs;

		if (res == NULL) errs = "Unknown Error";
		else errs = res;
		mw->returnFail("deploy", props.getProperty("cmd", "unknown"),errs);		
	}

	return true;
}

AppController *MonitoringMonitor::getAppController(MonitorWorker *mw, Properties &props)
{
	Manager *mgr = Registry::getRegistry()->getManager(CONTROLLER_MANAGER);
	ControllerManager *cntlMgr = portable_dynamic_cast(ControllerManager *, mgr);
	if (cntlMgr == NULL) {
		mw->returnFail("monitor", props.getProperty("cmd", "unknown"),"Server error(No ControllerManager)");
		return NULL;
	}
	AppController *ctl = portable_dynamic_cast(AppController *, cntlMgr->getController("app"));
	if (ctl == NULL) {
		mw->returnFail("monitor", props.getProperty("cmd", "unknown"),"Server error(No AppController)");
		return NULL;
	}

	return ctl;
}

ComponentController *MonitoringMonitor::getComponentController(MonitorWorker *mw, Properties &props)
{
	Manager *mgr = Registry::getRegistry()->getManager(CONTROLLER_MANAGER);
	ControllerManager *cntlMgr = portable_dynamic_cast(ControllerManager *, mgr);
	if (cntlMgr == NULL) {
		mw->returnFail("monitor", props.getProperty("cmd", "unknown"),"Server error(No ControllerManager)");
		return NULL;
	}
	ComponentController *ctl = portable_dynamic_cast(ComponentController *, cntlMgr->getController("comp"));
	if (ctl == NULL) {
		mw->returnFail("monitor", props.getProperty("cmd", "unknown"),"Server error(No ComponentController)");
		return NULL;
	}

	return ctl;
}

void MonitoringMonitor::appList(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	vector<string> list;
	ctl->appList(list);

	string ret;
	for (size_t i = 0; i < list.size(); ++i) {
		if (i == 0) {
			ret.append(list[i]);
		}
		else {
			ret.append(",");
			ret.append(list[i]);
		}
	}

	mw->returnOK(props, ret);
}

void MonitoringMonitor::appExist(MonitorWorker *mw, Properties &props)
{
	string appName = props.getProperty("app.name", "");

	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	vector<string> list;
	ctl->appList(list);

	string ret;
	for (size_t i = 0; i < list.size(); ++i) {
		if (appName == OS::splitExt(list[i])) {
			ret = "yes";
			mw->returnOK(props, ret);
			return;
		}
	}

	ret = "no";
	mw->returnOK(props, ret);
}
void MonitoringMonitor::appDelete(MonitorWorker *mw, Properties &props)
{
	string appName = props.getProperty("app.name", "");

	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	vector<string> list;
	bool isExist = false;
	string ret;

	ctl->appList(list);

	for (size_t i = 0; i < list.size(); ++i) {		
		std::string n = OS::splitExt(list[i]);

		if (appName == n) {
		
		//if (appName == OS::splitExt(list[i])) {
			isExist = true;
		}
	}
	if (isExist == false) {
		ret = "Not exist : app.name=" + appName;
		mw->returnFail(props, ret);
		return;
	}

	list.clear();
	isExist = false;
	ctl->appRunList(list);
	for (size_t i = 0; i < list.size(); ++i) {

		if (appName == list[i]) {
			isExist = true;
		}
	}

	if (isExist == true) {
		ret = "The application is running : app.name=" + appName;
		mw->returnFail(props, ret);
		return;
	}

	if (ctl->appDelete(appName)) {
		ret = "ok";
		mw->returnOK(props, ret);
	}
	else {
		ret = "fail";
		mw->returnOK(props, ret);
	}
}


void MonitoringMonitor::appRunList(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	vector<string> list;
	ctl->appRunList(list);

	string ret;
	for (size_t i = 0; i < list.size(); ++i) {
		if (i == 0) {
			ret.append(list[i]);
		}
		else {
			ret.append(",");
			ret.append(list[i]);
		}
	}
	mw->returnOK(props, ret);
}

void MonitoringMonitor::appCompList(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");
	vector<string> list, compTypes;
	if (ctl->appCompList(appName, list, compTypes) == false) {
		mw->returnFail(props, "error");
	}
	else {
		string ret;
		for (size_t i = 0; i < list.size(); ++i) {
			if (i == 0) {
				ret.append(list[i] + ":" + compTypes[i]);
			}
			else {
				ret.append(",");
				ret.append(list[i] + ":" + compTypes[i]);
			}
		}
		mw->returnOK(props, ret);
	}
}

void MonitoringMonitor::appRun(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");
	if (ctl->appRun(appName) == false) {
		mw->returnFail(props, "error");
	}
	else {
		mw->returnOK(props, "ok");
	}
}

void MonitoringMonitor::appLoad(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");
	if (ctl->appLoad(appName) == false) {
		mw->returnFail(props, "error");
	}
	else {
		mw->returnOK(props, "ok");
	}
}

void MonitoringMonitor::appConnect(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");
	if (ctl->appConnect(appName) == false) {
		mw->returnFail(props, "error");
	}
	else {
		mw->returnOK(props, "ok");
	}
}

void MonitoringMonitor::appActivate(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");
	if (ctl->appActivate(appName) == false) {
		mw->returnFail(props, "error");
	}
	else {
		mw->returnOK(props, "ok");
	}
}

void MonitoringMonitor::appStop(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");
	if (ctl->appStop(appName) == false) {
		mw->returnFail(props, "error");
	}
	else {
		mw->returnOK(props, "ok");
	}
}

void MonitoringMonitor::appPassivate(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");
	if (ctl->appPassivate(appName) == false) {
		mw->returnFail(props, "error");
	}
	else {
		mw->returnOK(props, "ok");
	}
}

void MonitoringMonitor::appDisconnect(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");
	if (ctl->appDisconnect(appName) == false) {
		mw->returnFail(props, "error");
	}
	else {
		mw->returnOK(props, "ok");
	}
}

void MonitoringMonitor::appUnload(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");
	if (ctl->appUnload(appName) == false) {
		mw->returnFail(props, "error");
	}
	else {
		mw->returnOK(props, "ok");
	}
}

void MonitoringMonitor::appState(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");

	string state = ctl->appState(appName);
	mw->returnOK(props, state);
}


void MonitoringMonitor::appInfo(MonitorWorker *mw, Properties &props)
{
	AppController *ctl = getAppController(mw, props);
	if (ctl == NULL) return;

	string appName = props.getProperty("app.name");

	Properties status;
	if (ctl->appInfo(appName, status) == false) {
		mw->returnFail(props, status.getProperty("error","error"));
	}
	else {
		string ret;
		
		string type = status.getProperty("type", "unknown");
		string state = status.getProperty("state", "unknown");
		string ref = status.getProperty("ref", "unknown");
		string path = status.getProperty("path", "unknown");

		ret.append("type:" + type);
		ret.append(",state:" + state);
		ret.append(",ref:" + ref);
		ret.append(",path:" + path);

		mw->returnOK(props, ret);
	}
}

void MonitoringMonitor::compInfo(MonitorWorker *mw, Properties &props)
{
	ComponentController *ctl = getComponentController(mw, props);
	if (ctl == NULL) return;

	string compName = props.getProperty("comp.name");

	Properties status;
	if (ctl->compInfo(compName, status) == false) {
		mw->returnFail(props, status.getProperty("error","error"));
	}
	else {
		string ret;
		
		string state = status.getProperty("state", "NOT_EXIST");
		string type = status.getProperty("type", "unknown");

		string exType = status.getProperty("es.type", "unknown");
		string period    = status.getProperty("es.period", "unknown");

		ret.append("state:" + state);
		ret.append(",type:" + type);
		ret.append(",exec_type:" + exType);
		ret.append(",exec_period:" + period);

		mw->returnOK(props, ret);
	}
}

void MonitoringMonitor::compList(MonitorWorker *mw, Properties &props)
{
	ComponentController *ctl = getComponentController(mw, props);
	if (ctl == NULL) return;

	string compName = props.getProperty("comp.name", "");

	vector<string> list, compTypes;
	ctl->compList(compName, list, compTypes);

	string ret;
	for (size_t i = 0; i < list.size(); ++i) {
		if (i == 0) {
			ret.append(list[i] + ":" + compTypes[i]);
		}
		else {
			ret.append(",");
			ret.append(list[i] + ":" + compTypes[i]);
		}
	}
	mw->returnOK(props, ret);
}

void MonitoringMonitor::compProps(MonitorWorker *mw, Properties &props)
{
	ComponentController *ctl = getComponentController(mw, props);
	if (ctl == NULL) return;

	string compName = props.getProperty("comp.name");

	Properties cp;
	if (ctl->compProps(compName, cp) == false) {
		mw->returnFail(props, cp.getProperty("error", "error"));
	}
	else {
		vector<string> list;
		cp.getNames(list);

		string ret;
		for (size_t i = 0; i < list.size(); ++i) {
			if (i == 0) {
				ret.append(list[i] + ":" + cp.getProperty(list[i]));
			}
			else {
				ret.append(",");
				ret.append(list[i] + ":" + cp.getProperty(list[i]));
			}
		}
		mw->returnOK(props, ret);
	}
}

