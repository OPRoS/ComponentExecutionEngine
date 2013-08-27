/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : RequiredServicePort.cpp
 *
 *
 */

#include "RequiredServicePort.h"
#include "RequiredMethodInterface.h"
#include "ServicePortInterface.h"
class RequiredServicePortImpl {
	RequiredServicePort *m_caller;
	ServicePortConnector *m_con;
	ServicePort *m_peer;
	typedef std::map<std::string, Method *> service_base;
public:
	RequiredServicePortImpl(RequiredServicePort *caller) :
		m_caller(caller) {
		m_con = NULL;
		m_peer = NULL;
	}

	virtual ~RequiredServicePortImpl() {
		if (m_con !=NULL) delete m_con;
		Disconnect();

	}

	virtual ReturnType Disconnect() {
		m_peer = NULL;
		m_con = NULL;

		service_base::iterator it;
		for (it = m_caller->getFirst(); it != m_caller->getEnd(); ++it) {
			Method *cur = it->second;
			RequiredMethodInterface *pvt = SAFE_CASTING(RequiredMethodInterface *,cur);
			pvt->Disconnect();
		}
		return OPROS_SUCCESS;
	}

	virtual ReturnType setConnector(ServicePortConnector *con) {
		m_peer = NULL;
		m_con = con;
		service_base::iterator it;
		for (it = m_caller->getFirst(); it != m_caller->getEnd(); ++it) {
			Method *cur = it->second;

			RequiredMethodInterface *pvt = SAFE_CASTING(RequiredMethodInterface *,cur);

			pvt->Connect(con);
		}

		return OPROS_SUCCESS;
	}

	virtual ReturnType setPeer(ServicePort *peer) {

		if (peer->getRole() != OPROS_SERVICE_PORT_PROVIDED)
		{
			return OPROS_BAD_INPUT_PARAMETER;
		}

		m_con = NULL;
		m_peer = peer;
		ServicePortInterface *sport = SAFE_CASTING(ServicePortInterface *,peer);

		service_base::iterator it;
		for (it = m_caller->getFirst(); it != m_caller->getEnd(); ++it) {
			Method *cur = it->second;
			if (cur == NULL)
			{
					return OPROS_BAD_INPUT_PARAMETER;
			}


			const std::string &name = it->first;

			Method *target = sport->getMethod(name);

			if (target == NULL) {
				return OPROS_BAD_INPUT_PARAMETER;
			}
		}


		for (it = m_caller->getFirst(); it != m_caller->getEnd(); ++it) {
			Method *cur = it->second;
			if (cur == NULL)
			{
					return OPROS_BAD_INPUT_PARAMETER;
			}
			const std::string &name = it->first;

			Method *target = sport->getMethod(name);
			if (target != NULL)
			{
				RequiredMethodInterface *pvt = SAFE_CASTING(RequiredMethodInterface *,cur);
				if (pvt == NULL)
				{

					return OPROS_BAD_INPUT_PARAMETER;
				}

				pvt->Connect(target);
			}
		}

		return OPROS_SUCCESS;

	}

	virtual ReturnType callService(std::string &name, std::string &invalue,
			std::string &outvalue) {
		if (m_peer != NULL) {
			return m_peer->callService(name, invalue, outvalue);
		} else if (m_con != NULL) {
			return m_con->requestService(name, invalue, outvalue);
		}

		return OPROS_INTERNAL_FAULT;
	}
};

RequiredServicePort::RequiredServicePort() :
	m_rsimpl(new RequiredServicePortImpl(this)) {
	// TODO Auto-generated constructor stub

}

RequiredServicePort::RequiredServicePort(const std::string &name) :
	ServicePortInterface(name), m_rsimpl(new RequiredServicePortImpl(this)) {
	// TODO Auto-generated constructor stub
}

RequiredServicePort::~RequiredServicePort() {
	delete m_rsimpl;
}

ReturnType RequiredServicePort::Disconnect() {
	return m_rsimpl->Disconnect();
}

ReturnType RequiredServicePort::setConnector(ServicePortConnector *con) {
	return m_rsimpl->setConnector(con);
}

ReturnType RequiredServicePort::setPeer(ServicePort *peer) {
	return m_rsimpl->setPeer(peer);
}

ReturnType RequiredServicePort::callService(std::string &name,
		std::string &invalue, std::string &outvalue) {
	return m_rsimpl->callService(name, invalue, outvalue);
}

ServicePortRole RequiredServicePort::getRole()
{
	return OPROS_SERVICE_PORT_REQUIRED;
}
