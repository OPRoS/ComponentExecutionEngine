/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ServicePort.cpp
 *
 *
 */


#include "ServicePort.h"


#include <iostream>
#include <CTrace.h>
ServicePort::ServicePort() {
	m_type = OPROS_SERVICE;
}

ServicePort::ServicePort(const std::string &name) :
	Port(name, OPROS_SERVICE) {
}

ServicePort::~ServicePort() {
	ctrace("service port delete:"<<getName()<<std::endl);
}
