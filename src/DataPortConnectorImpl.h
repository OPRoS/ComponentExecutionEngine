/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : DataPortConnectorImpl.h
 *
 *
 */

#ifndef DATAPORTCONNECTORIMPL_H_
#define DATAPORTCONNECTORIMPL_H_
#include "DataPortConnector.h"
#include "RemoteDataPort.h"
#include <string>
#include "system/cm/ComponentRequester.h"

class DataPortConnectorImpl: public DataPortConnector , public RemoteDataPort{
	std::string m_owner_name;
	std::string m_name;
	ComponentRequester *m_worker;
public:
	DataPortConnectorImpl(ComponentRequester *worker);
	virtual ~DataPortConnectorImpl();

	virtual void setTargetComponentName(const std::string &name);
	virtual std::string &getTargetComponentName();
	virtual void setTargetPortName(const std::string &name);
	virtual std::string &getTargetPortName();

	virtual ReturnType sendData(unsigned char *data, int len);
	virtual ReturnType recvData(unsigned char *data, int len);
	virtual ReturnType pushData(unsigned char *data, int size); // use encoded data
};

#endif /* DATAPORTCONNECTORIMPL_H_ */
