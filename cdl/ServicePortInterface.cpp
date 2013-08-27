/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ServicePortInterface.cpp
 *
 *
 */

#include "ServicePortInterface.h"
#include <map>
#include <CTrace.h>

class ServicePortImpl {
	typedef std::map<std::string, Method *> service_base;

	service_base m_services;

	ServicePortInterface *m_caller;

	ArchiveFactory *m_arc;

public:
	ServicePortImpl(ServicePortInterface *caller) :
		m_caller(caller) {
			m_arc=NULL;
	}

	virtual ~ServicePortImpl() {
		if (!m_services.empty()) {
			for (service_base::iterator it = m_services.begin(); it
					!= m_services.end(); ++it) {
				Method *s_srv = it->second;
				delete s_srv;
			}
			m_services.clear();
		}
	}

	virtual ReturnType addMethod(const std::string &name, Method *srv) {
		if (srv == NULL)
			return OPROS_BAD_INPUT_PARAMETER;
		if (m_services.count(name) != 0) {
			Method *p = m_services[name];
			delete p;
		}
	
		m_services[name] = srv;

		if (m_arc != NULL) {
			srv->setArchiveFactory(m_arc);
		}

		return OPROS_SUCCESS;
	}

	virtual Method *getMethod(const std::string &name) {
		service_base::iterator it = m_services.find(name);
		if (it != m_services.end())
			return it->second;

		return NULL;
	}

	virtual service_base::iterator getFirst() {
		return m_services.begin();
	}

	virtual service_base::iterator getEnd() {
		return m_services.end();
	}

	virtual void setArchiveFactory(ArchiveFactory *arc) {
		for (service_base::iterator it = m_services.begin(); it
				!= m_services.end(); ++it) {
			Method *s_srv = it->second;

			s_srv->setArchiveFactory(arc);
		}
	}
};

ServicePortInterface::ServicePortInterface() :
	m_impl(new ServicePortImpl(this)) {
}

ServicePortInterface::ServicePortInterface(const std::string &name) :
	ServicePort(name), m_impl(new ServicePortImpl(this)) {

}

ServicePortInterface::~ServicePortInterface() {
	delete m_impl;
}

ServicePortInterface::service_base::iterator ServicePortInterface::getFirst() {
	ctrace_enter();
	return m_impl->getFirst();
}

ServicePortInterface::service_base::iterator ServicePortInterface::getEnd() {
	ctrace_enter();
	return m_impl->getEnd();
}

ReturnType ServicePortInterface::addMethod(const std::string &name, Method *srv) {
	ctrace_enter();
	return m_impl->addMethod(name, srv);
}

Method *ServicePortInterface::getMethod(const std::string &name) {
	ctrace_enter();
	return m_impl->getMethod(name);
}

void ServicePortInterface::setArchiveFactory(ArchiveFactory *darc) {
	ctrace_enter();

	return m_impl->setArchiveFactory(darc);
}
