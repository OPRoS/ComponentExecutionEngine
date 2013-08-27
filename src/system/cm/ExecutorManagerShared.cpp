/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExecutorManagerShared.cpp
 *
 */


#include "ExecutorManagerShared.h"

#include <ReturnType.h>

#include "ComponentAdapterAtomic.h"

#include "ExecutorShared.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ExecutorManagerShared::ExecutorManagerShared()
{
	m_nextExecutorId = 1;
}

ExecutorManagerShared::~ExecutorManagerShared()
{
	close();
}

//-------------------------------------------------
// stop all executor and delete from the memory
//
void ExecutorManagerShared::close()
{
	trace_enter_msg("size = " << m_executorMap.size());

	// PEMap에 존재하는 모든 실행기를 중지시키고 메모리에서 삭제함
	ExecutorMap::iterator iter = m_executorMap.begin();
	for (; iter != m_executorMap.end(); ++iter) {
		// executor의 destroy 호출
		iter->second->destroyExecutor();

		// 메모리에서 실행기를 삭제함
		// delete iter->second;
	}
}

int ExecutorManagerShared::nextExecutorId()
{
	int next = m_nextExecutorId++;
	if (m_nextExecutorId == 0) m_nextExecutorId++;

	return next;
}


//-------------------------------------------------
// find a executor stored in ExecutorMap
//
ExecutorInterface *ExecutorManagerShared::findExecutor(int executorId)
{
	ExecutorMap::iterator iter = m_executorMap.find(executorId);
	if (iter == m_executorMap.end()) return NULL;

	return iter->second;
}

//-------------------------------------------------
// remove a executor stored in ExecutorMap
//
void ExecutorManagerShared::removeExecutor(int executorId)
{
	ExecutorMap::iterator iter = m_executorMap.find(executorId);
	if (iter == m_executorMap.end()) return;
	m_executorMap.erase(iter);
}

//-------------------------------------------------
// insert a executor to the ExecutorMap
//
void ExecutorManagerShared::addExecutor(int executorId, ExecutorInterface *executor)
{
	m_executorMap[executorId] = executor;
}

//-------------------------------------------------
// find a executor stored in PeriodMap
//
int ExecutorManagerShared::findExecutorIdFromPeriodPriorityMap(int period, int priority)
{
	if(priority < 0) priority = 0;
	else if (priority > MAX_PRIORITY) priority = MAX_PRIORITY;

	double periodDotPriority = period + ((double) priority) / (MAX_PRIORITY + 1);
	PeriodPriorityMap::iterator iter =m_periodPriorityMap.find(periodDotPriority);
	if (iter == m_periodPriorityMap.end()) return 0;

	return iter->second;
}

//-------------------------------------------------
// remove a executor stored in PeriodMap
//
void ExecutorManagerShared::removeExecutorIdFromPeriodPriorityMap(int period, int priority)
{
	if(priority < 0) priority = 0;
	else if (priority > MAX_PRIORITY) priority = MAX_PRIORITY;

	double periodDotPriority = period + ((double) priority) / (MAX_PRIORITY + 1);
	PeriodPriorityMap::iterator iter =m_periodPriorityMap.find(periodDotPriority);
	if (iter == m_periodPriorityMap.end()) return;
	m_periodPriorityMap.erase(iter);
}

//-------------------------------------------------
// insert a executor to the ExecutorMap
//
void ExecutorManagerShared::addExecutorIdToPeriodPriorityMap(int period, int priority, int executorId)
{
	if(priority < 0) priority = 0;
	else if (priority > MAX_PRIORITY) priority = MAX_PRIORITY;

	double periodDotPriority = period + ((double) priority) / (MAX_PRIORITY + 1);
	m_periodPriorityMap[periodDotPriority] = executorId;
}

bool ExecutorManagerShared::startExecutor(ExecutorInterface *executor)
{
	trace_enter();

	return executor->startExecutor() == OPROS_SUCCESS;
}

bool ExecutorManagerShared::stopExecutor(ExecutorInterface *executor)
{
	trace_enter();

	if (executor->getExecutorType() == "periodic-shared") 
	{
		if (executor->getComponentSize() > 0) 
		{
			log_debug("The executor has more components : size=" << executor->getComponentSize());
			return false;
		}
		removeExecutorIdFromPeriodPriorityMap(executor->getPeriod(), executor->getPriority());
	}
	executor->stopExecutor();

	return true;
}

//---------------------------------------------------------
// createExecutor
ExecutorInterface *ExecutorManagerShared::createExecutor(ComponentAdapterAtomic *atomic)
{
	trace_enter();

	ExecutorInterface *executor = NULL;
	ComponentProfile *profile = atomic->getAtomicComponentProfile();
	if (profile->execSemantics.type == "aperiodic" || \
		profile->execSemantics.type == "non-periodic" ||\
		profile->execSemantics.type == "periodic" || \
		profile->execSemantics.type == "periodic-exclusive") 
	{
		// 프로파일에서 주기를 가져옴
		int period = profile->execSemantics.period;
		int eid = nextExecutorId();
		if (profile->execSemantics.type == "aperiodic" || profile->execSemantics.type == "non-periodic") period = 0;

		int policy = OPROS_SCHED_OTHER;
		if (!profile->execSemantics.policy.compare("SCHED_FIFO")) policy = OPROS_SCHED_FIFO;
		else if (!profile->execSemantics.policy.compare("SCHED_RR")) policy = OPROS_SCHED_RR;		

		executor = new ExecutorShared(profile->execSemantics.priority, period, policy);
		executor->setExecutorId(eid);
		executor->setExecutorType(profile->execSemantics.type);
		startExecutor(executor);

		// 실행기에 해당 컴포넌트 추가
		if (executor->addComponent(atomic) == OPROS_SUCCESS) {
			addExecutor(eid, executor);
			atomic->setExecutorId(eid);
		}
		else {
			stopExecutor(executor);

			delete executor;
			log_error("Component is not added to the executor : comp.id=" << atomic->getId());
			return NULL;
		}
	}
	else if (profile->execSemantics.type == "periodic-shared") {
		// 프로파일에서 주기를 가져옴
		int period = profile->execSemantics.period;
		int priority = profile->execSemantics.priority;

		// 기존에 같은 주기로 만들어진 실행기가 있는지 검사
		int eid = findExecutorIdFromPeriodPriorityMap(period, priority);

		if (eid == 0) {
			log_debug("Creating a executor : period =" << period << ", priority=" << priority);

			// 해당 주기를 가지는 실행기가 없으면 새로 생성
			// 있으면 기존 실행기를 사용함
			eid = nextExecutorId();

			int policy = OPROS_SCHED_OTHER;
			if (!profile->execSemantics.policy.compare("SCHED_FIFO")) policy = OPROS_SCHED_FIFO;
			else if (!profile->execSemantics.policy.compare("SCHED_RR")) policy = OPROS_SCHED_RR;		

			executor = new ExecutorShared(profile->execSemantics.priority, period, policy);
			executor->setExecutorId(eid);
			executor->setExecutorType(profile->execSemantics.type);
			startExecutor(executor);

			// 실행기에 해당 컴포넌트 추가
			if (executor->addComponent(atomic) == OPROS_SUCCESS) {
				addExecutor(eid, executor);
				addExecutorIdToPeriodPriorityMap(period, priority, eid);
				atomic->setExecutorId(eid);
			}
			else {
				stopExecutor(executor);
				delete executor;
				log_error("Component is not added to the executor : comp.id=" << atomic->getId());
				return NULL;
			}	
		}
		else {
			log_debug("Reuse the executor : period =" << period << ", priority=" << priority);

			executor = findExecutor(eid);
			// 실행기에 해당 컴포넌트 추가
			if (executor->addComponent(atomic) == OPROS_SUCCESS) {
				atomic->setExecutorId(eid);
			}
			else {
				log_error("Component is not added to the executor : comp.id=" << atomic->getId());
				return NULL;
			}
		}
	}
	else {
		log_error("The type of execution semantics is invalid : comp.id=" << atomic->getId() << ".type=" << profile->execSemantics.type);
		return NULL;
	}

	return executor;
}

//---------------------------------------------------
// 실행기에 컴포넌트를 추가함
bool ExecutorManagerShared::addComponent(ComponentAdapter *comp)
{
	trace_enter();

	if (comp == NULL) {
		log_error("ComponentAdapter is NULL");
		return false;
	}

	// atoimc 컴포넌트 만이 실행기에 할당가능하므로 atomic 컴포넌트인지를 검사한다.
	ComponentAdapterAtomic *atomic = dynamic_cast<ComponentAdapterAtomic *>(comp);
	if (atomic == NULL) {
		log_error("ComponentAdapter is not a atomic component");
		return true;
	}

	int eid = atomic->getExecutorId();
	if (eid != 0) {
		log_warn("The compoent is already registered to the executor : comp.id=" << atomic->getId() \
			<< ", executor_id=" << eid);
		return true;
	}

	OSGuard guard(&m_mutex);
	return doAddComponent(atomic);
}

bool ExecutorManagerShared::doAddComponent(ComponentAdapterAtomic *atomic)
{
	trace_enter();

	ComponentProfile *profile = atomic->getAtomicComponentProfile();
	if (profile->execSemantics.type == "passive") {
		log_debug("The compoent is passive and don't need a executor : comp.id=" << atomic->getId());
		return true;
	}

	ExecutorInterface *executor = createExecutor(atomic);
	if (executor != NULL) 
	{
		log_debug("Component is added from the executor : exec.id=" << executor->getExecutorId() << ", comp.id=" << atomic->getId());
		return true;
	}
	else 
	{
		log_error("Component Adding to the executor is FAILED  : comp.id=" << atomic->getId());
		return false;
	}

}

//---------------------------------------------------
// removeComponent
bool ExecutorManagerShared::removeComponent(ComponentAdapter *comp)
{
	trace_enter();

	if (comp == NULL) {
		return true;
	}

	ComponentAdapterAtomic *atomic = dynamic_cast<ComponentAdapterAtomic *>(comp);
	if (atomic == NULL) {
		log_error("ComponentAdapter is not a atomic component");
		return true;
	}

	int eid = atomic->getExecutorId();
	if (eid == 0) {
		log_debug("Not allocated to the executor : comp.id=" << atomic->getId());
		return true;
	}

	// lock the mutex
	OSGuard guard(&m_mutex);
	return doRemoveComponent(atomic);
}

bool ExecutorManagerShared::doRemoveComponent(ComponentAdapterAtomic *atomic)
{
	trace_enter();

	int eid = atomic->getExecutorId();

	// find the executor from the CEMap
	ExecutorInterface *executor = findExecutor(eid);
	if (executor == NULL) {
		log_debug("Not allocated to the executor : comp.id=" << atomic->getId());
		return true;
	}

	log_debug("Component is removed from the executor START : exec.id=" << executor->getExecutorId() << ", comp.id=" << atomic->getId());
	// remove the compoent from the executor
	if (executor->removeComponent(atomic->getId()) != OPROS_SUCCESS) {
		log_error("Component is NOT removed from the executor : comp.id=" << atomic->getId());
		return false;
	}
	log_debug("Component is removed from the executor END : exec.id=" << executor->getExecutorId() << ", comp.id=" << atomic->getId());

	if (stopExecutor(executor)) {
		// remove the executor 
	    ExecutorInterface *pexi = findExecutor(executor->getExecutorId());
		removeExecutor(executor->getExecutorId());
        pexi->destroyExecutor();

		delete pexi;

	//	removeExecutor(executor->getExecutorId());
	}

	return true;
}

//---------------------------------------------------
// startComponent
bool ExecutorManagerShared::startComponent(ComponentAdapter *comp)
{
	trace_enter();

	if (comp == NULL) {
		log_error("ComponentAdapter is NULL");
		return false;
	}

	ComponentAdapterAtomic *atomic = dynamic_cast<ComponentAdapterAtomic *>(comp);
	if (atomic == NULL) {
		log_error("ComponentAdapter is not a atomic component");
		return true;
	}

	// get the executor id
	int eid = atomic->getExecutorId();
	if (eid == 0) {
		ComponentProfile *profile = atomic->getAtomicComponentProfile();

		if (profile->execSemantics.type == "passive") {
			// 실행기에 할당되지 않은 컴포넌트는 passive 컴포넌트이므로
			// 실행기를 통하지 않고 바로 컴포넌트의 start 함수를 호출함
			if (atomic->getComponent()->start() == OPROS_SUCCESS) {
				log_debug("Passive Component start success : comp.id=" << atomic->getId());
				return true;
			}
			else {
				log_error("Passive Component start fail : comp.id=" << atomic->getId());
				return false;
			}
		}
		else {
			log_warn("Not allocated to any executor. Start failed : comp.id=" << atomic->getId());
			return true;
		}

		return true;
	}
	else {
		// lock the mutex
		OSGuard guard(&m_mutex);

		ExecutorInterface *executor = findExecutor(eid);
		if (executor == NULL) return false;
		return doStartComponent(executor, atomic);
	}
}

bool ExecutorManagerShared::doStartComponent(ExecutorInterface *executor, ComponentAdapterAtomic *atomic)
{
	trace_enter();

	// start the compoent from the executor
	if (executor->startComponent(atomic) != OPROS_SUCCESS) {
		log_error("Component is NOT started from the executor : comp.id=" << atomic->getId());
		return false;
	}
	log_debug("Component is started from the executor : exec.id=" << executor->getExecutorId() << ", comp.id=" << atomic->getId());

	return true;
}

//---------------------------------------------------
// stopComponent
bool ExecutorManagerShared::stopComponent(ComponentAdapter *comp)
{
	trace_enter();

	if (comp == NULL) {
		log_error("ComponentAdapter is NULL");
		return false;
	}

	ComponentAdapterAtomic *atomic = dynamic_cast<ComponentAdapterAtomic *>(comp);
	if (atomic == NULL) {
		log_error("ComponentAdapter is not a atomic component");
		return true;
	}

	// get the executor id
	int eid = atomic->getExecutorId();
	if (eid == 0) {
		ComponentProfile *profile = atomic->getAtomicComponentProfile();

		if (profile->execSemantics.type == "passive") {
			// 실행기에 할당되지 않은 컴포넌트는 passive 컴포넌트이므로
			// 실행기를 통하지 않고 바로 컴포넌트의 stop 함수를 호출함
			if (atomic->getComponent()->stop() == OPROS_SUCCESS) {
				log_debug("Passive Component stop success : comp.id=" << atomic->getId());
				return true;
			}
			else {
				log_error("Passive Component stop fail : comp.id=" << atomic->getId());
				return false;
			}
		}
		else {
			log_warn("Not allocated to any executor. Stop failed : comp.id=" << atomic->getId());
			return true;
		}

		return true;
	}
	else {
		// lock the mutex
		OSGuard guard(&m_mutex);
		ExecutorInterface *executor = findExecutor(eid);
		if (executor == NULL) return false;
		return doStopComponent(executor, atomic);
	}
}

bool ExecutorManagerShared::doStopComponent(ExecutorInterface *executor, ComponentAdapterAtomic *atomic)
{
	trace_enter();

	// stop the compoent from the executor
	if (executor->stopComponent(atomic) != OPROS_SUCCESS) 
	{
		log_error("Component is NOT stopped from the executor : comp.id=" << atomic->getId());
		return false;
	}

	log_debug("Component is stopped from the executor : exec.id=" << executor->getExecutorId() << ", comp.id=" << atomic->getId());
	return true;
}

//---------------------------------------------------
// resetComponent
bool ExecutorManagerShared::resetComponent(ComponentAdapter *comp)
{
	trace_enter();

	if (comp == NULL) {
		log_error("ComponentAdapter is NULL");
		return false;
	}

	ComponentAdapterAtomic *atomic = dynamic_cast<ComponentAdapterAtomic *>(comp);
	if (atomic == NULL) {
		log_error("ComponentAdapter is not a atomic component");
		return true;
	}

	// get the executor id
	int eid = atomic->getExecutorId();
	if (eid == 0) {
		ComponentProfile *profile = atomic->getAtomicComponentProfile();

		if (profile->execSemantics.type == "passive") {
			// 실행기에 할당되지 않은 컴포넌트는 passive 컴포넌트이므로
			// 실행기를 통하지 않고 바로 컴포넌트의 reset 함수를 호출함
			if (atomic->getComponent()->reset() == OPROS_SUCCESS) {
				log_debug("Passive Component reset success : comp.id=" << atomic->getId());
				return true;
			}
			else {
				log_error("Passive Component reset fail : comp.id=" << atomic->getId());
				return false;
			}
		}
		else {
			log_error("Not allocated to any executor. Reset failed : comp.id=" << atomic->getId());
			return false;
		}

		return true;
	}
	else {
		// lock the mutex
		OSGuard guard(&m_mutex);
		ExecutorInterface *executor = findExecutor(eid);
		if (executor == NULL) return false;
		return doStopComponent(executor, atomic);
	}
}

bool ExecutorManagerShared::doResetComponent(ExecutorInterface *executor, ComponentAdapterAtomic *atomic)
{
	trace_enter();

	// reset the compoent from the executor
	if (executor->resetComponent(atomic) != OPROS_SUCCESS) {
		log_error("Component is NOT resetted from the executor : comp.id=" << atomic->getId());
		return false;
	}
	log_debug("Component is resetted from the executor : exec.id=" << executor->getExecutorId() << ", comp.id=" << atomic->getId());

	return true;
}

