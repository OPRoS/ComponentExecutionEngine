/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : DataPortConnectorImpl.cpp
 *
 *
 */

#include "DataPortConnectorImpl.h"

DataPortConnectorImpl::DataPortConnectorImpl(ComponentRequester *worker) {
	m_worker = worker;

}

DataPortConnectorImpl::~DataPortConnectorImpl() {
	// TODO Auto-generated destructor stub
}

ReturnType DataPortConnectorImpl::sendData(unsigned char *data, int len)
{
	if (!m_worker->dataCall(m_owner_name, m_name, data, len))
	{
		return OPROS_INTERNAL_FAULT;
	}

	return OPROS_SUCCESS;
}

ReturnType DataPortConnectorImpl::recvData(unsigned char *data, int len)
{
	return OPROS_UNSUPPORTED_METHOD;
}

void DataPortConnectorImpl::setTargetComponentName(const std::string &name)
{
	m_owner_name = name;
}

std::string &DataPortConnectorImpl::getTargetComponentName()
{
	return 	m_owner_name;
}

void DataPortConnectorImpl::setTargetPortName(const std::string &name)
{
	m_name = name;
}

std::string &DataPortConnectorImpl::getTargetPortName()
{
	return m_name;
}

ReturnType DataPortConnectorImpl::pushData(unsigned char *data, int size)
{
	return sendData(data, size);
}



