/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExecutorInterface.h
 *
 */

#ifndef _ExecutorInterface_H_
#define _ExecutorInterface_H_

#include <string>

#include <Component.h>
#include <ReturnType.h>
#include <OPRoSTypes.h>

#include "ComponentAdapter.h"

class ExecutorInterface 
{
	int m_executorId;
	std::string m_executorType;

public:
	ExecutorInterface() {
		m_executorId = 0; 
	};
	ExecutorInterface(int priority, int period) {
		m_executorId = 0; 
	};
	virtual ~ExecutorInterface() {};

	virtual ReturnType addComponent(ComponentAdapter *pComp) = 0;
	virtual ReturnType removeComponent(ComponentAdapter *pComp) = 0;
	virtual ReturnType removeComponent(const std::string &compId) = 0;
	virtual ReturnType startComponent(ComponentAdapter *pComp) = 0;
	virtual ReturnType startExecutor() = 0;
	virtual ReturnType stopComponent(ComponentAdapter *pComp) = 0;
	virtual ReturnType stopExecutor() = 0;
	virtual ReturnType resetComponent(ComponentAdapter *pComp) = 0;
	virtual ReturnType resetExecutor() = 0;

	virtual ReturnType destroyExecutor() = 0;

	virtual ReturnType setPriority(int priority) = 0;
	virtual int getPriority() = 0;

	virtual ReturnType setPeriod(int period) = 0;
	virtual int getPeriod() = 0;

	virtual void setExecutorId(int id) { m_executorId = id; };
	virtual int getExecutorId() { return m_executorId; };

	virtual void setExecutorType(const std::string &type) { m_executorType = type; };
	virtual std::string getExecutorType() { return m_executorType; };

	virtual int getComponentSize() = 0;

	virtual bool isExecutorStarted() = 0;
};


#endif /* _ExecutorInterface_H_ */
