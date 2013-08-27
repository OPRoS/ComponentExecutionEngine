/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : PortInterface.cpp
 *
 *
 */

#include "PortInterface.h"
#include <map>
#include <CTrace.h>

class PortInterfaceImpl {
	typedef std::map<std::string, Port *> port_base;

	port_base m_service_ports;
	port_base m_data_ports;
	port_base m_event_ports;

public:
	PortInterfaceImpl() {
	}
	;

	virtual ~PortInterfaceImpl() {
		if (!m_service_ports.empty()) {
			for (port_base::iterator s_it = m_service_ports.begin(); s_it
					!= m_service_ports.end(); ++s_it) {
				Port *s_port = s_it->second;
				delete s_port;
			}
			m_service_ports.clear();
		}

		if (!m_data_ports.empty()) {
			for (port_base::iterator d_it = m_data_ports.begin(); d_it
					!= m_data_ports.end(); ++d_it) {
				//Port *ds_port = d_it->second;
				//delete ds_port;  // 보통 port 가 heap alloc 하지 않으므로
			}
			m_data_ports.clear();
		}

		if (!m_event_ports.empty()) {
			for (port_base::iterator e_it = m_event_ports.begin(); e_it
					!= m_event_ports.end(); ++e_it) {
				//Port *es_port = e_it->second;
				//delete es_port;
			}
			m_event_ports.clear();
		}

	}
	;

	virtual ReturnType addPort(const std::string &name, Port *port) {
		if (port->getType() == OPROS_SERVICE) {
			if (m_service_ports.count(name) != 0) {
				Port *p = m_service_ports[name];
				delete p;
			}
			m_service_ports[name] = port;

		} else if (port->getType() == OPROS_DATA) {
			if (m_data_ports.count(name) != 0) {
				//Port *p = m_data_ports[name];
				//delete p;
			}
			m_data_ports[name] = port;

		} else if (port->getType() == OPROS_EVENT) {
			if (m_event_ports.count(name) != 0) {
				//Port *p = m_event_ports[name];
				//delete p;
			}
			m_event_ports[name] = port;
		}

		return OPROS_SUCCESS;
	}

	virtual Port *getPort(const std::string &name) {

		port_base::iterator d_it = m_data_ports.find(name);
		if (d_it != m_data_ports.end()) {
			return d_it->second;
		}

		port_base::iterator e_it = m_event_ports.find(name);
		if (e_it != m_event_ports.end()) {
			return e_it->second;
		}

		port_base::iterator s_it = m_service_ports.find(name);
		if (s_it != m_service_ports.end()) {
			return s_it->second;
		}

		return NULL;
	}

	virtual void portReset()
	{
		if (!m_service_ports.empty()) {
			for (port_base::iterator s_it = m_service_ports.begin(); s_it
					!= m_service_ports.end(); ++s_it) {
				Port *s_port = s_it->second;
				s_port->reset();
				
			}
		}

		if (!m_data_ports.empty()) {
			for (port_base::iterator d_it = m_data_ports.begin(); d_it
					!= m_data_ports.end(); ++d_it) {
				Port *ds_port = d_it->second;
				ds_port->reset();
			}
		}

		if (!m_event_ports.empty()) {
			for (port_base::iterator e_it = m_event_ports.begin(); e_it
					!= m_event_ports.end(); ++e_it) {
				Port *es_port = e_it->second;
				es_port->reset();
			}
		}

	}
};
PortInterface::PortInterface() :
	m_impl(new PortInterfaceImpl) {	
	
}

PortInterface::~PortInterface() {
	delete m_impl;
}

ReturnType PortInterface::addPort(const std::string &name, Port *port) {
	ctrace_enter();
	return m_impl->addPort(name, port);
}

Port *PortInterface::getPort(const std::string &name) {
	ctrace_enter();
	return m_impl->getPort(name);
}

void PortInterface::portReset()
{
	ctrace_enter();
	return m_impl->portReset();
}
