/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 9. 30
 *  @Author  : sby(sby@etri.re.kr)
 *
 *  @File    : SymbolMonitor.h
 *
 */

#ifndef _SYMBOL_MONITOR_H_
#define _SYMBOL_MONITOR_H_

#include <string>

#include "Monitor.h"
#include "SymbolMonitorInterface.h"
#include "SymbolVariable.h"
#include "system/control/AppController.h"
#include "system/control/ComponentController.h"
#include "system/cm/ComponentManager.h"

/**
	배포 관련 명령을 처리하는 클래스.	
*/ 



class SymbolMonitor : public Monitor, public SymbolMonitorInterface
{
protected :
	Registry *m_registry;
	ComponentManager *m_cm;

	virtual bool doCommand(MonitorWorker *mw, Properties &props);

	int symbolGet(MonitorWorker *mw, Properties &props);	
	int symbolSet(MonitorWorker *mw, Properties &props);
	int symbolCall(MonitorWorker *mw, Properties &props);	

	ComponentAdapterPtr findComponent(std::string &comname);

public:
	virtual int symbolGet(std::vector<SymbolVariable> &rqlist);
	virtual int symbolSet(std::vector<SymbolVariable> &rqlist);
	virtual int symbolCall(std::vector<SymbolFunction> &rqlist);

	virtual int symbolGet(SymbolVariable &rqlist);
	virtual int symbolSet(SymbolVariable &rqlist); 
	virtual int symbolCall(SymbolFunction &rqlist); 

	SymbolMonitor();
	virtual ~SymbolMonitor();

};

#endif  
