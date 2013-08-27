/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 20
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : OutputDataPortInterface.cpp
 *
 *
 */

#include "OutputEventPortInterface.h"
#include "system/os/OSMutex.h"
#include "system/os/OSGuard.h"


#include <list>
#include <CTrace.h>

typedef std::list<IEventPortLink *> event_con_list_type;
class OutputEventPortImpl {
protected:
	OutputEventPortInterface *m_caller;
	std::list<IEventPortLink *> m_con_list;

	OSMutex m_con_list_lock;

	OSMutex *getLock() { return &m_con_list_lock; };

public:
	OutputEventPortImpl(OutputEventPortInterface *caller) :
		m_caller(caller) {
	}
	virtual ~OutputEventPortImpl() {
		m_con_list.clear();
	}
	virtual ReturnType push(Event *data) {
		if (m_caller->checkType(data) == false)
			return OPROS_BAD_INPUT_PARAMETER;

		if (m_con_list.size() == 0) {
			// there is no connection, so parameter Event is consumed.
			;
		} else {
			// Event broad casting
			OSGuard g(getLock());
			for (event_con_list_type::iterator it = m_con_list.begin(); it
					!= m_con_list.end(); ++it) {
				IEventPortLink *peer = *it;
				peer->push(data);
			}
		}

		return OPROS_SUCCESS;

	}

	virtual void addConnection(IEventPortLink * connection) {
		OSGuard g(getLock());
		m_con_list.push_back(connection);
		m_con_list.unique(); // removes all consecutive duplicate elements from the list.
	}

	virtual void removeConnection(IEventPortLink *connection) {
		OSGuard g(getLock());
		m_con_list.remove(connection);
	}

	virtual void clearConnection() {
		OSGuard g(getLock());
		m_con_list.clear(); // OutputEventPort do not delete each of connection
	}

	virtual ReturnType marshal(Event *data, std::string &outdata)
	{
		if (m_caller == NULL)
		{
			return OPROS_UNSUPPORTED_METHOD;
		}

		return m_caller->marshal(data,outdata);
	}

};

OutputEventPortInterface::OutputEventPortInterface() :
	m_impl(new OutputEventPortImpl(this)) {
	// TODO Auto-generated constructor stub
	m_arc = NULL;
}
OutputEventPortInterface::OutputEventPortInterface(const std::string &name) :
	EventPort(name), m_impl(new OutputEventPortImpl(this)) {
	m_arc = NULL;
}

OutputEventPortInterface::~OutputEventPortInterface() {
	delete m_impl;
}

ReturnType OutputEventPortInterface::push_event(Event *data) {
	ctrace_enter();
	return m_impl->push(data);
}


EventPortRole OutputEventPortInterface::getRole() {
	return OPROS_EVENT_PORT_OUTPUT;
}

void OutputEventPortInterface::setArchiveFactory(ArchiveFactory *darc) {
	m_arc = darc;
}

void OutputEventPortInterface::addConnection(IEventPortLink * connection) {
	ctrace_enter();
	m_impl->addConnection(connection);
}

void OutputEventPortInterface::removeConnection(IEventPortLink * connection) {
	ctrace_enter();
	m_impl->removeConnection(connection);
}

void OutputEventPortInterface::clearConnection() {
	ctrace_enter();
	m_impl->clearConnection();
}
