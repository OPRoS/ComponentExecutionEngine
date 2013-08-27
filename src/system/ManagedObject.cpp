/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 4
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ManagedObject.cpp
 *
 */

#include <vector>
#include <algorithm>

#include "system/config/Config.h"

#include "ManagedObject.h"
#include "ManagedObjectListener.h"

#include "Log.h"
#include "Trace.h"

using namespace std;

ManagedObject::ManagedObject()
{
	m_state = MS_CREATED;
}

ManagedObject::~ManagedObject()
{
}

void ManagedObject::toProperties(Config *cfg)
{
	if (cfg != NULL) cfg->toProperties(m_props);
}

/*
 * init this manager
 *
 * @param cfg config info
 * @ret bool success or fail
 */
bool ManagedObject::init()
{
	trace_enter_msg("state=" << stateToStr());

	if(m_state != MS_CREATED) return false;

	toProperties(getConfig());
	if (!onInit()) {
		log_error("<ManagedObject::init> onInit FAILED...");
		return false;
	}

	m_state = MS_INITED;
	stateChanged(MS_INITED);
	return true;
}

/*
 * start this manager
 *
 * @param cfg config info
 * @ret bool success or fail
 */
bool ManagedObject::start()
{
	trace_enter_msg("state=" << stateToStr());

	if (m_state != MS_INITED && m_state != MS_STOPPED) return false;

	if (!onStart()) {
		log_error("<ManagedObject::start> onStart FAILED...");
		return false;
	}

	m_state = MS_STARTED;
	stateChanged(MS_STARTED);
	return true;
}

bool ManagedObject::stop()
{
	trace_enter_msg("state=" << stateToStr());

	if (m_state != MS_STARTED) return false;

	if (!onStop()) {
		log_error("<ManagedObject::stop> onStop FAILED...");
		return false;
	}

	m_state = MS_STOPPED;
	stateChanged(MS_STOPPED);
	return true;
}

bool ManagedObject::reset()
{
	trace_enter_msg("state=" << stateToStr());

	if (m_state != MS_INITED && m_state != MS_STOPPED) return false;

	if (!onReset()) {
		log_error("<ManagedObject::reset> onReset FAILED...");
		return false;
	}

	m_state = MS_CREATED;
	stateChanged(MS_CREATED);
	return true;
}

bool ManagedObject::destroy()
{
	trace_enter_msg("state=" << stateToStr());

	if (m_state == MS_STARTED) {
		stop();
	}
	onDestroy();

	m_state = MS_DESTROYED;
	stateChanged(MS_DESTROYED);
	return true;
}

bool ManagedObject::onInit()
{
	return true;
}

bool ManagedObject::onStart()
{
	return true;
}


bool ManagedObject::onStop()
{
	return true;
}

bool ManagedObject::onReset()
{
	return true;
}

bool ManagedObject::onDestroy()
{
	return true;
}


/*
 * Execution function in a dedicated thread of this manager.
 * The thread is allocated by the component engine
 * This function is call only if this manager is "runnable".
 *
 * @ret void
 */
void ManagedObject::run()
{
}

/*
 * add a manager listener to the end of the listener list
 *
 * @param l listener to be added
 * @ret void
 */
void ManagedObject::addListener(ManagedObjectListener *l)
{
	m_listeners.push_back(l);
}

/*
 * remove a manager listener to the list
 *
 * @param l listener to be removed
 * @ret void
 */
void ManagedObject::removeListener(ManagedObjectListener *l)
{
	remove(m_listeners.begin(), m_listeners.end(), l);
	/*
	ManagedObjectListenerList::iterator pos;
	pos = find(m_listeners.begin(), m_listeners.end(), l);
	if (pos != m_listeners.end()) m_listeners.erase(pos);
	*/
}

/*
 * get a manager listener list
 *
 * @ret ManagedObjectListenerList manager listener list
 */
ManagedObjectListenerList ManagedObject::getListenerList()
{
	return m_listeners;
}

/*
 * callback if the state of this manager is changed
 * based on the manager's current state
 *
 * @ret void
 */
void ManagedObject::stateChanged(ManagedObjectState state)
{
	ManagedObjectListenerList::iterator pos = m_listeners.begin();
	for (; pos != m_listeners.end(); ++pos) {
		ManagedObjectListener *l = *pos;
		l->stateChanged(this, state);
	}
}

ManagedObjectState ManagedObject::getState()
{
	return m_state;
}

string ManagedObject::stateToStr()
{
	switch (getState()) {
		case MS_CREATED : return "CREATED";
		case MS_INITED : return "INITED";
		case MS_STARTED : return "STARTED";
		case MS_STOPPED : return "STOPPED";
		case MS_DESTROYED : return "DESTROYED";
		default : return "Unknown";
	}
}

