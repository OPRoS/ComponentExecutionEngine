/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MonitoringMonitor.h
 *
 */

#ifndef _MONITORING_MONITOR_H_
#define _MONITORING_MONITOR_H_

#include <string>

#include "Monitor.h"
#include "system/control/AppController.h"
#include "system/control/ComponentController.h"

/**
	배포 관련 명령을 처리하는 클래스.	
*/ 
class MonitoringMonitor : public Monitor
{
protected :

	virtual bool doCommand(MonitorWorker *mw, Properties &props);

	AppController *getAppController(MonitorWorker *mw, Properties &props);
	ComponentController *getComponentController(MonitorWorker *mw, Properties &props);

	void appList(MonitorWorker *mw, Properties &props);
	void appRunList(MonitorWorker *mw, Properties &props);
	void appCompList(MonitorWorker *mw, Properties &props);
	void appState(MonitorWorker *mw, Properties &props);
	void appInfo(MonitorWorker *mw, Properties &props);

	void appRun(MonitorWorker *mw, Properties &props);
	void appLoad(MonitorWorker *mw, Properties &props);
	void appConnect(MonitorWorker *mw, Properties &props);
	void appActivate(MonitorWorker *mw, Properties &props);
	void appStop(MonitorWorker *mw, Properties &props);
	void appPassivate(MonitorWorker *mw, Properties &props);
	void appDisconnect(MonitorWorker *mw, Properties &props);
	void appUnload(MonitorWorker *mw, Properties &props);
	void appExist(MonitorWorker *mw, Properties &props);
	void appDelete(MonitorWorker *mw, Properties &props);

	void compInfo(MonitorWorker *mw, Properties &props);
	void compList(MonitorWorker *mw, Properties &props);
	void compProps(MonitorWorker *mw, Properties &props);

public:
	MonitoringMonitor();
	virtual ~MonitoringMonitor();

};

#endif  
