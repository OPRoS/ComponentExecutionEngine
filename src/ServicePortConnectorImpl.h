/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ServicePortConnectorImpl.h
 *
 *
 */

#ifndef SERVICEPORTCONNECTORIMPL_H_
#define SERVICEPORTCONNECTORIMPL_H_

#include "ServicePortConnector.h"
#include "system/cm/ComponentRequester.h"

class ServicePortConnectorImpl: public ServicePortConnector {
protected:
	std::string m_owner_name;
	std::string m_name;
	ComponentRequester *m_worker;
public:
	ServicePortConnectorImpl(ComponentRequester *worker);
	virtual ~ServicePortConnectorImpl();

	virtual void setTargetComponentName(const std::string &name);
	virtual std::string &getTargetComponentName();
	virtual void setTargetPortName(const std::string &name);
	virtual std::string &getTargetPortName();

	virtual ReturnType requestService(std::string &name, std::string &invalue, std::string &outvalue);
	virtual ReturnType responseService(std::string &name, std::string &invalue, std::string &outvalue);
};

#endif /* SERVICEPORTCONNECTORIMPL_H_ */
