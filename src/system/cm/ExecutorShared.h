/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExecutorShared.h
 *
 */

#ifndef _ExecutorShared_H_
#define _ExecutorShared_H_

#include <string>

#include <Component.h>
#include <ReturnType.h>
#include <OPRoSTypes.h>

#include "executor/Executor.h"
#include "ExecutorInterface.h"

#include "system/Log.h"
#include "system/Trace.h"

class ExecutorShared : public ExecutorInterface, public Executor
{
public:
	ExecutorShared() : Executor() { 
	};
	ExecutorShared(int priority, int period, int sched_policy) : 
		Executor(priority,period, sched_policy) { 
	};
	virtual ~ExecutorShared() {};

	virtual ReturnType addComponent(ComponentAdapter *pComp) { 
		return add(pComp); 
	};
	
	virtual ReturnType removeComponent(ComponentAdapter *pComp) { 
		return remove(pComp); 
	};

	virtual ReturnType removeComponent(const std::string &compId) { 
		trace_enter();
		return remove(compId); 
	};

	virtual ReturnType startComponent(ComponentAdapter *pComp) {
		return start(pComp); 
	};
	virtual ReturnType startExecutor() { 
		return start(); 
	};
	
	virtual ReturnType stopComponent(ComponentAdapter *pComp) { 
		return stop(pComp); 
	};
	virtual ReturnType stopExecutor() { 
		return stop(); 
	};

	virtual ReturnType resetComponent(ComponentAdapter *pComp) {
		return reset(pComp); 
	};

	virtual ReturnType resetExecutor() { 
		return reset(); 
	};

	virtual ReturnType destroyExecutor() { 
		return destroy(); 
	};

	virtual ReturnType setPriority(int priority) { 
		return Executor::setPriority(priority); 
	};

	virtual int getPriority() { 
		return Executor::getPriority(); 
	};

	virtual ReturnType setPeriod(int period) { 
		return Executor::setPeriod(period); 
	};

	virtual int getPeriod() {
		return Executor::getPeriod(); 
	};

	virtual int getComponentSize() {
		return (int) m_compList.size();
	};

	virtual bool isExecutorStarted() {
		return m_runningState == OPROS_ES_ACTIVE; 
	};
};


#endif /* _ExecutorInterface_H_ */
