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

#include "OutputDataPortInterface.h"
#include "system/os/OSMutex.h"
#include "system/os/OSGuard.h"

#include <list>
#include <CTrace.h>
typedef std::list<IDataPortLink *> data_con_list_type;
class OutputDataPortImpl {
protected:
	OutputDataPortInterface *m_caller;
	std::list<IDataPortLink *> m_con_list;

	OSMutex m_con_list_lock;

	OSMutex *getLock() { return &m_con_list_lock; };


public:
	OutputDataPortImpl(OutputDataPortInterface *caller) :
		m_caller(caller) {
	}
	virtual ~OutputDataPortImpl() {
		OSGuard g(getLock());
		m_con_list.clear();
	}
	virtual ReturnType push(opros_any & data) {
		if (m_caller->checkType(data) == false)
			return OPROS_BAD_INPUT_PARAMETER;

		OSGuard g(getLock());
		if (m_con_list.size() == 0) {
			// there is no connection, so parameter data is consumed.
			;
		} else {
			// data broad casting
			
			for (data_con_list_type::iterator it = m_con_list.begin(); it
					!= m_con_list.end(); ++it) {
				IDataPortLink *peer = *it;
				peer->push(data);
			}
		}

		return OPROS_SUCCESS;

	}

	virtual void addConnection(IDataPortLink * connection) {
		OSGuard g(getLock());
		m_con_list.push_back(connection);
		m_con_list.unique(); // removes all consecutive duplicate elements from the list.
	}

	virtual void removeConnection(IDataPortLink *connection) {
		OSGuard g(getLock());
		m_con_list.remove(connection);
	}

	virtual void clearConnection() {
		OSGuard g(getLock());
		m_con_list.clear(); // OutputDataPort do not delete each of connection
	}

	virtual ReturnType marshal(opros_any &data, std::string &outdata)
	{
		if (m_caller == NULL)
		{
			return OPROS_UNSUPPORTED_METHOD;
		}

		return m_caller->marshal(data,outdata);
	}

};

OutputDataPortInterface::OutputDataPortInterface() :
	m_impl(new OutputDataPortImpl(this)) {
	// TODO Auto-generated constructor stub
	m_arc = NULL;
}
OutputDataPortInterface::OutputDataPortInterface(const std::string &name) :
	DataPort(name), m_impl(new OutputDataPortImpl(this)) {
	m_arc = NULL;
}

OutputDataPortInterface::~OutputDataPortInterface() {
	delete m_impl;
}

ReturnType OutputDataPortInterface::push_data(opros_any & data) {
	ctrace_enter();
	return m_impl->push(data);
}

opros_any *OutputDataPortInterface::pop() {
	ctrace_enter();
	return NULL;
}

opros_any *OutputDataPortInterface::peek() {
	ctrace_enter();
	return NULL;
}

DataPortRole OutputDataPortInterface::getRole() {
	return OPROS_DATA_PORT_OUTPUT;
}

void OutputDataPortInterface::setArchiveFactory(ArchiveFactory *darc) {
	m_arc = darc;
}

void OutputDataPortInterface::addConnection(IDataPortLink * connection) {
	ctrace_enter();
	m_impl->addConnection(connection);
}


void OutputDataPortInterface::removeConnection(IDataPortLink * connection) {
	ctrace_enter();
	m_impl->removeConnection(connection);
}

void OutputDataPortInterface::clearConnection() {
	ctrace_enter();
	m_impl->clearConnection();
}
