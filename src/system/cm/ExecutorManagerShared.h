/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExecutorManagerShared.h
 *
 */

#ifndef _ExecutorManagerShared_H_
#define _ExecutorManagerShared_H_

#include <string>
#include <map>

#include "ExecutorManager.h"

#include "ExecutorInterface.h"
#include "ComponentAdapterAtomic.h"
#include "system/os/OSIncludes.h"

#define MAX_PRIORITY 99
/** 
	컴포넌트가 할당된 Executor를 관리하는 객체 
*/
class ExecutorManagerShared : public ExecutorManager
{
protected :

	typedef std::map<int, ExecutorInterface *> ExecutorMap;
	typedef std::map<double, int> PeriodPriorityMap;

	/** 주기에 따라 executor를 저장하고 있는 맵. (period, executor *). */
	ExecutorMap m_executorMap;
	int m_nextExecutorId;

	/** ??? ?? executor id?? ??????? ??? ??. (period.priority, executor_id). */
	PeriodPriorityMap m_periodPriorityMap;

	/**  mutex for syncing the map */
	OSMutex m_mutex;

	int nextExecutorId();

	/**  
		create a executor for the given component

		@param comp 컴포넌트 
		@param ExecutorInterface*  실행기 
	*/
	virtual ExecutorInterface *createExecutor(ComponentAdapterAtomic *comp);

	/**  
		insert a executor to the ComponentExecutorMap

		@param compId 컴포넌트 아이디
		@param executor 추가하려고 하는 실행기 
	*/
	virtual void addExecutor(int executorId, ExecutorInterface *executor);

	/**  
		start the executor on adding it
	*/
	virtual bool startExecutor(ExecutorInterface *executor);

	/**  
		find a executor stored in ComponentExecutorMap 

		@param compId 찾고자하는 컴포넌트의 아이디

		@return 해당 컴포넌트를 포함하고 있는 실행기 객체
	*/
	virtual ExecutorInterface *findExecutor(int executorId);

	/**  
		remove a executor stored in ComponentExecutorMap 

		@param executorId 지우고자하는 실행기 아이디
	*/
	virtual void removeExecutor(int executorId);

	/**  
		stop the executor on removing it
	*/
	virtual bool stopExecutor(ExecutorInterface *executor);

	/**  
		find a executor stored in PeriodExecutorMap.

		@param period 실행기의 주기

		@return 해당 컴포넌트를 포함하고 있는 실행기 객체 id
	*/
	virtual int findExecutorIdFromPeriodPriorityMap(int period, int priority);

	/**  
		remove a executor stored in PeriodExecutorMap 

		@param period 실행기의 주기
	*/
	virtual void removeExecutorIdFromPeriodPriorityMap(int period, int priority);

	/**  
		insert a executor to the PeriodExecutorMap 

		@param compId 실행 주기
		@param executor 추가하려고 하는 실행기 
	*/
	virtual void addExecutorIdToPeriodPriorityMap(int period, int priority, int executorId);

protected :

	virtual bool doAddComponent(ComponentAdapterAtomic *comp);
	virtual bool doRemoveComponent(ComponentAdapterAtomic *comp);
	virtual bool doStartComponent(ExecutorInterface *executor, ComponentAdapterAtomic *comp);
	virtual bool doStopComponent(ExecutorInterface *executor, ComponentAdapterAtomic *comp);
	virtual bool doResetComponent(ExecutorInterface *executor, ComponentAdapterAtomic *comp);

	/**
		ExecutorManagerShared를 종료하는 함수.
	*/
	virtual void close();

public:
	ExecutorManagerShared();
	virtual ~ExecutorManagerShared();


	/**
		컴포넌트를 실행기에 추가.
		컴포넌트가 passive 인 경우는 추가하지 않음.

		@param comp 추가할 컴포넌트 객체

		@return bool 추가 성공 여부
	*/
	virtual bool addComponent(ComponentAdapter *comp);

	/** 
		실행기에서 컴포넌트를 삭제함. 

		@param comp 삭제할 컴포넌트 객체

		@return bool 삭제 성공 여부
	*/
	virtual bool removeComponent(ComponentAdapter *comp);

	/** 
		실행기에 할당된 컴포넌트를 시작시킴. 

		@param comp 시작할 컴포넌트 객체

		@return bool 성공 여부
	*/
	virtual bool startComponent(ComponentAdapter *comp);

	/** 
		실행기에 할당된 컴포넌트를 중지시킴. 

		@param comp 중지할 컴포넌트 객체

		@return bool 성공 여부
	*/
	virtual bool stopComponent(ComponentAdapter *comp);

	/** 
		실행기에 할당된 컴포넌트를 리셋시킴.

		@param comp 리셋할 컴포넌트 객체

		@return bool 성공 여부
	*/
	virtual bool resetComponent(ComponentAdapter *comp);
};

#endif
