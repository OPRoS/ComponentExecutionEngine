/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 6
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoEventTargetManager.cpp
 *
 */

#include "IoEventTargetManager.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoEventTargetManager::IoEventTargetManager()
{
}

IoEventTargetManager::~IoEventTargetManager()
{
	trace_enter_msg("id=" + getEventTargetManagerId());

	removeEventTargetAll();
}

IoEventTarget *IoEventTargetManager::getEventTarget(const string &id)
{
	trace_enter();

	IoEventTargetMap::iterator iter = m_evtTargetMap.find(id);
	if (iter != m_evtTargetMap.end()) {
		return iter->second;
	}
	else {
		trace_error("connection does not exist : obj.id=" << id);
		return NULL;
	}
}

bool IoEventTargetManager::attachEventTarget(IoEventTarget *obj)
{
	trace_enter();

	if (obj == NULL) {
		trace_error("connection is null");
		return false;
	}

	IoEventTargetMap::iterator pos = m_evtTargetMap.find(obj->getId());
	if (pos != m_evtTargetMap.end()) {
		trace_error("already exists. erase and close it. : obj.id=" << obj->getId());
		pos->second->close();

		m_evtTargetMap.erase(pos);
	}

	m_evtTargetMap.insert(make_pair(obj->getId(), obj));

	trace("Registered event target : id="<<obj->getId());
	return true;
}

IoEventTarget *IoEventTargetManager::detachEventTarget(const std::string &id)
{
	trace_enter();

	IoEventTarget *obj = getEventTarget(id);
	if (obj == NULL) {
		trace_error("connection does not exist : obj.id=" << id);
	}
	else {
		m_evtTargetMap.erase(id);
	}
	return obj;
}

// erase from the list and delete
void IoEventTargetManager::removeEventTarget(const std::string &id)
{
	trace_enter();

	IoEventTargetMap::iterator pos = m_evtTargetMap.find(id);
	if (pos != m_evtTargetMap.end()) {
		IoEventTarget *obj = pos->second;
		m_evtTargetMap.erase(pos);
		delete obj;
	}
}

void IoEventTargetManager::removeEventTargetAll()
{
	trace_enter();

	IoEventTargetMap::iterator pos = m_evtTargetMap.begin();

	for (; pos != m_evtTargetMap.end(); ++pos) {
		IoEventTarget *obj = pos->second;
		delete obj;
	}
	m_evtTargetMap.clear();
}
