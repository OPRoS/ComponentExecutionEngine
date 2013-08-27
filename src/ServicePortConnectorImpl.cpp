/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ServicePortConnectorImpl.cpp
 *
 *
 */

#include "ServicePortConnectorImpl.h"
#include "MethodInvokeException.h"
#include "system/io/protocol/rpc/IoResponse.h"

ServicePortConnectorImpl::ServicePortConnectorImpl(ComponentRequester *worker) {
	m_worker = worker;

}

ServicePortConnectorImpl::~ServicePortConnectorImpl() {
	// TODO Auto-generated destructor stub
}

ReturnType ServicePortConnectorImpl::requestService(std::string & name, std::string & invalue, std::string & outvalue)
{

	unsigned char *pdata = (unsigned char *)invalue.c_str();

	IoResponse *resp = m_worker->methodCall(m_owner_name, m_name, name, pdata, invalue.length());

	if (resp != NULL)
	{
		byte *ptr = resp->getBuffer();
		int size = resp->getSize();

		if (ptr != NULL)
		{
			std::string nv((const char *)ptr, size);

			outvalue = nv;

			delete ptr;
		}

		delete resp;
	}else
	{
		throw MethodInvokeException("method call error.");
	}

	return OPROS_SUCCESS;
}

ReturnType ServicePortConnectorImpl::responseService(std::string & name, std::string & invalue, std::string & outvalue)
{
	return OPROS_UNSUPPORTED_METHOD;
}


void ServicePortConnectorImpl::setTargetComponentName(const std::string &name)
{
	m_owner_name = name;
}

std::string &ServicePortConnectorImpl::getTargetComponentName()
{
	return 	m_owner_name;
}

void ServicePortConnectorImpl::setTargetPortName(const std::string &name)
{
	m_name = name;
}

std::string &ServicePortConnectorImpl::getTargetPortName()
{
	return m_name;
}

