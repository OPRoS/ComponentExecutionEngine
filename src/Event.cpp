/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 19
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : Event.cpp
 *
 *
 */

#include "Event.h"

class EventImpl {
protected:
	Event *m_caller;
	std::string m_id;
	std::string m_topic;
	std::string m_comp_id;
	std::string m_port_id;
	opros_any *m_content;

public:
	EventImpl(Event *caller) :
		m_caller(caller) {
		m_content = NULL;
	}

	virtual ~EventImpl() {
		;
		if (m_content != NULL) {
			delete m_content;
		}
	}

	virtual std::string &getId() {
		return m_id;
	}

	virtual void setId(const std::string &strid) {
		m_id = strid;
	}

	virtual std::string &getTopic()
	{
		return m_topic;
	}

	virtual void setTopic(const std::string &topic)
	{
		m_topic = topic;
	}

	virtual std::string &getComponentId() {
		return m_comp_id;
	}

	virtual void setComponentId(const std::string &strid) {
		m_comp_id = strid;
	}

	virtual std::string &getPortId() {
		return m_port_id;
	}

	virtual void setPortId(const std::string &strid) {
		m_port_id = strid;
	}

	virtual opros_any &getContent() {
		if (m_content == NULL)
			m_content = new opros_any();
		return (*m_content);
	}

	virtual ReturnType setContent(opros_any &con) {
		if (m_caller->checkContentType(con)) {
			if (m_content != NULL)
				delete m_content;
			m_content = new opros_any(con);

			return OPROS_SUCCESS;
		}

		return OPROS_BAD_INPUT_PARAMETER;
	}

};

Event::Event() :
	m_impl(new EventImpl(this)) {
	// TODO Auto-generated constructor stub
}

Event::~Event() {
	if (m_impl != NULL)
		delete m_impl;
}

std::string &Event::getId() {
	return m_impl->getId();
}

void Event::setId(const std::string &strid) {
	m_impl->setId(strid);
}


std::string &Event::getTopic()
{
	return m_impl->getTopic();
}

void Event::setTopic(const std::string &topic)
{
	m_impl->setTopic(topic);
}

opros_any &Event::getContent() {
	return m_impl->getContent();
}

ReturnType Event::setContent(opros_any &con) {
	return m_impl->setContent(con);
}

std::string &Event::getComponentId() {
	return m_impl->getComponentId();
}

void Event::setComponentId(const std::string &strid) {
	m_impl->setComponentId(strid);
}

std::string &Event::getPortId() {
	return m_impl->getPortId();
}

void Event::setPortId(const std::string &strid) {
	m_impl->setPortId(strid);
}

Event &Event::operator=(Event &rhs)
{
	setId(rhs.getId());
	setTopic(rhs.getTopic());
	setComponentId(rhs.getComponentId());
	setPortId(rhs.getPortId());
	setContent(rhs.getContent());

	return *this;
}
