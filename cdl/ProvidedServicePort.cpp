/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ProvidedServicePort.cpp
 *
 *
 */

#include "ProvidedServicePort.h"

ProvidedServicePort::ProvidedServicePort() {
	// TODO Auto-generated constructor stub

}

ProvidedServicePort::ProvidedServicePort(const std::string &name) :
	ServicePortInterface(name) {
	// TODO Auto-generated constructor stub
}

ProvidedServicePort::~ProvidedServicePort() {
	// TODO Auto-generated destructor stub
}

ReturnType ProvidedServicePort::callService(std::string &name,
		std::string &invalue, std::string &outvalue) {
	Method *p = getMethod(name);

	if (p == NULL)
		return OPROS_UNSUPPORTED_METHOD;

	return p->call(invalue, outvalue);
}

ServicePortRole ProvidedServicePort::getRole()
{
	return OPROS_SERVICE_PORT_PROVIDED;
}
