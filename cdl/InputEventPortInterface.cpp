/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 23
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : InputEventPortInterface.cpp
 *
 *
 */

#include "InputEventPortInterface.h"
#include <Component.h>
#include <OPRoSTypes.h>
#include <CTrace.h>
class InputEventPortImpl {
public: 
//	Event *m_queue; 

	Component *m_owner;
	const InputEventPortInterface *m_caller;

public:
	InputEventPortImpl(InputEventPortInterface *caller) :
		m_caller(caller) {
//		m_queue = NULL;
		m_owner = NULL;
	}

	virtual ~InputEventPortImpl() {

	}

	void clear() {
//		if (m_queue != NULL)
//			delete m_queue;
	}


	ReturnType push(Event *data) {
//			clear();
//			m_queue = data;
		if (m_owner == NULL) return OPROS_CALLER_ERROR;
		return m_owner->onEvent(data);
	}

	void setOwner(Component *owner)
	{
		m_owner = owner;
	}
};

InputEventPortInterface::InputEventPortInterface() :
	m_impl(new InputEventPortImpl(this)) {
	m_arc = NULL;

}

InputEventPortInterface::InputEventPortInterface(const std::string &name) :
	EventPort(name), m_impl(new InputEventPortImpl(this)) {
	m_arc = NULL;
}

InputEventPortInterface::~InputEventPortInterface() {
	delete m_impl;
}

void InputEventPortInterface::setArchiveFactory(ArchiveFactory *darc) {
	m_arc = darc;
}

EventPortRole InputEventPortInterface::getRole() {
	return OPROS_EVENT_PORT_INPUT;
}

void InputEventPortInterface::setOwner(Component *owner)
{
	ctrace_enter();
	m_impl->setOwner(owner);
}

ReturnType InputEventPortInterface::push_event(Event *data) {
	ctrace_enter();
	if (checkType(data)) {

		return m_impl->push(data);
	}

	return OPROS_BAD_INPUT_PARAMETER;
}
