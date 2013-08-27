/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 19
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : InputDataPortInterface.cpp
 *
 *
 */


#include "InputDataPortInterface.h"

#include <OPRoSTypes.h>
#include <list>
#include <CTrace.h>
#include "system/os/OSMutex.h"
#include "system/os/OSGuard.h"

class InputDataPortImpl {
public:
	std::list<opros_any> m_queue;
	unsigned int m_queue_limit;
	DataPortPolicy m_data_policy;
	OSMutex m_lock;

	InputDataPortInterface *m_caller;
public:
	InputDataPortImpl( DataPortPolicy data_policy,unsigned int queue_limit,
			InputDataPortInterface *caller) :
		m_queue_limit(queue_limit), m_data_policy(data_policy), m_caller(
				caller) {

	}

	virtual ~InputDataPortImpl() {
		 clear();
	}

	void clear() {
		m_queue.clear();
	}

	OSMutex *getLock() { return &m_lock; };

	opros_any *peek() {
		OSGuard g(getLock());
		if (getSize() == 0)
			return NULL;

		opros_any *results = new opros_any();

		if (m_data_policy == OPROS_LIFO) {
			(*results) = *(--m_queue.end());
		} else {
			(*results) = *(m_queue.begin());
		}

		return results;
	}

	opros_any *pop() {
		OSGuard g(getLock());

		if (getSize() == 0)
			return NULL;

		opros_any *results = new opros_any();

		if (m_data_policy == OPROS_LIFO) {
			(*results) = *(--m_queue.end());
			m_queue.pop_back();
		} else {
			(*results) = *(m_queue.begin());
			m_queue.pop_front();
		}

		return results;
	}

	int getSize() {
		return m_queue.size();
	}

	ReturnType push(opros_any &data) {
		OSGuard g(getLock());

		if (data.empty()) {
			return OPROS_BAD_INPUT_PARAMETER;
		}
		switch (m_data_policy) {
		case OPROS_UFIFO:
			m_queue.push_back(data);
			break;
		case OPROS_LIFO:
			m_queue.push_back(data);
			break;
		case OPROS_FIFO:
			if (m_queue.size() == m_queue_limit) {
				m_queue.push_back(data);
				m_queue.erase(m_queue.begin());
			} else {
				m_queue.push_back(data);
			}
			break;
		case OPROS_LAST:
			if (m_queue.empty() == false) {
				m_queue.clear();
			}

			m_queue.push_back(data);
			break;

		}


		return OPROS_SUCCESS;
	}
};

InputDataPortInterface::InputDataPortInterface() :
	m_impl(new InputDataPortImpl(OPROS_FIFO,DEFAULT_QUEUE_LIMIT, this)) {
	m_arc = NULL;

}
InputDataPortInterface::InputDataPortInterface(	DataPortPolicy data_policy, unsigned int queue_limit) :
	m_impl(new InputDataPortImpl(data_policy, queue_limit, this)) {
	m_arc = NULL;

}

InputDataPortInterface::InputDataPortInterface(const std::string &name) :
	DataPort(name), m_impl(new InputDataPortImpl(OPROS_FIFO, DEFAULT_QUEUE_LIMIT,
			this)) {
	m_arc = NULL;

}

InputDataPortInterface::InputDataPortInterface(const std::string &name,
		DataPortPolicy data_policy, unsigned int queue_limit) :
	DataPort(name), m_impl(
			new InputDataPortImpl(data_policy, queue_limit, this)) {
	m_arc = NULL;

}

InputDataPortInterface::~InputDataPortInterface() {
	delete m_impl;
}

void InputDataPortInterface::setArchiveFactory(ArchiveFactory *darc) {
	m_arc = darc;
}

DataPortRole InputDataPortInterface::getRole()
{
	return OPROS_DATA_PORT_INPUT;
}

ReturnType InputDataPortInterface::push_data(opros_any & data) {
	ctrace_enter();
	if (checkType(data)) {
		return m_impl->push(data);
	}

	return OPROS_BAD_INPUT_PARAMETER;
}


opros_any *InputDataPortInterface::peek() {
	ctrace_enter();
	return m_impl->peek();
}

opros_any *InputDataPortInterface::pop() {
	ctrace_enter();
	return m_impl->pop();
}

int InputDataPortInterface::getSize() {
	return m_impl->getSize();
}

void InputDataPortInterface::reset() {
	m_impl->clear();
}
